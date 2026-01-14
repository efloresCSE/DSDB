// TypeScript interface for TXT2DB WASM module
import type { EmscriptenModule } from "emscripten"

export interface TXT2DBModule extends EmscriptenModule {
  initDatabase: () => string
  executeCommand: (command: string) => string
  listTables: () => string
  cleanup: () => void
}

export interface QueryResult {
  type?: "create" | "insert" | "select"
  table?: string
  message?: string
  output?: string
  error?: string
}

export interface TableColumn {
  name: string
  type?: string
}

export interface TableSchema {
  name: string
  columns: TableColumn[]
}

class MockTXT2DB {
  private tables = new Map<string, { columns: string[]; rows: any[][] }>()
  private initialized = false

  async initialize(): Promise<void> {
    this.initialized = true
    console.log("[TXT2DB Mock] Mock database initialized for development")
  }

  private parseValue(value: string): string {
    return value.trim().replace(/^["']|["']$/g, "")
  }

  private evaluateWhere(row: any[], columns: string[], whereClause: string): boolean {
    // Remove "where" keyword
    const condition = whereClause.replace(/^\s*where\s+/i, "").trim()

    // Handle OR (lower precedence)
    if (/\s+or\s+/i.test(condition)) {
      const orParts = condition.split(/\s+or\s+/i)
      return orParts.some((part) => this.evaluateWhere(row, columns, part))
    }

    // Handle AND (higher precedence)
    if (/\s+and\s+/i.test(condition)) {
      const andParts = condition.split(/\s+and\s+/i)
      return andParts.every((part) => this.evaluateWhere(row, columns, part))
    }

    // Parse single condition: column operator value
    const conditionMatch = condition.match(/(\w+)\s*(=|>|<|>=|<=|!=)\s*(.+)/)
    if (!conditionMatch) return true

    const [, colName, operator, valueStr] = conditionMatch
    const colIndex = columns.findIndex((c) => c.toLowerCase() === colName.toLowerCase())

    if (colIndex === -1) return false

    const rowValue = String(row[colIndex] || "")
    const compareValue = this.parseValue(valueStr)

    // Try numeric comparison first
    const numRowValue = Number(rowValue)
    const numCompareValue = Number(compareValue)
    const isNumeric = !isNaN(numRowValue) && !isNaN(numCompareValue)

    switch (operator) {
      case "=":
        return isNumeric ? numRowValue === numCompareValue : rowValue === compareValue
      case "!=":
        return isNumeric ? numRowValue !== numCompareValue : rowValue !== compareValue
      case ">":
        return isNumeric ? numRowValue > numCompareValue : rowValue > compareValue
      case "<":
        return isNumeric ? numRowValue < numCompareValue : rowValue < compareValue
      case ">=":
        return isNumeric ? numRowValue >= numCompareValue : rowValue >= compareValue
      case "<=":
        return isNumeric ? numRowValue <= numCompareValue : rowValue <= compareValue
      default:
        return true
    }
  }

  async executeQuery(sql: string): Promise<QueryResult> {
    const trimmed = sql.trim()

    if (/^make\s+table/i.test(trimmed)) {
      const match = trimmed.match(/make\s+table\s+(\w+)\s+fields\s+(.+)/i)
      if (match) {
        const tableName = match[1]
        const fieldsStr = match[2]
        const columns = fieldsStr.split(",").map((col) => col.trim())

        this.tables.set(tableName, { columns, rows: [] })

        return {
          type: "create",
          table: tableName,
          message: `Table ${tableName} created with fields: ${columns.join(", ")}`,
        }
      }
      throw new Error("Invalid MAKE TABLE syntax")
    }

    if (/^insert\s+into/i.test(trimmed)) {
      const match = trimmed.match(/insert\s+into\s+(\w+)\s+values\s+(.+)/i)
      if (match) {
        const tableName = match[1]
        const valuesStr = match[2]
        const values = valuesStr.split(",").map((v) => this.parseValue(v))

        const table = this.tables.get(tableName)
        if (!table) {
          throw new Error(`Table ${tableName} does not exist`)
        }

        table.rows.push(values)

        return {
          type: "insert",
          table: tableName,
          message: `1 row inserted into ${tableName}`,
        }
      }
      throw new Error("Invalid INSERT syntax")
    }

    if (/^select/i.test(trimmed)) {
      const fromMatch = trimmed.match(/from\s+(\w+)(?:\s+where\s+(.+))?/i)
      if (fromMatch) {
        const tableName = fromMatch[1]
        const whereClause = fromMatch[2]
        const table = this.tables.get(tableName)

        if (!table) {
          throw new Error(`Table ${tableName} does not exist`)
        }

        // Filter rows based on WHERE clause
        let filteredRows = table.rows
        if (whereClause) {
          filteredRows = table.rows.filter((row) => this.evaluateWhere(row, table.columns, whereClause))
        }

        // Format output
        const output = table.columns.join("\t") + "\n" + filteredRows.map((row) => row.join("\t")).join("\n")

        return {
          type: "select",
          output,
          message: `${filteredRows.length} row(s) selected from ${tableName}`,
        }
      }
      throw new Error("Invalid SELECT syntax")
    }

    throw new Error(`Unsupported command: ${trimmed.substring(0, 50)}`)
  }

  async getTables(): Promise<string[]> {
    return Array.from(this.tables.keys())
  }

  async getTableSchema(tableName: string): Promise<TableSchema | null> {
    const table = this.tables.get(tableName)
    if (!table) return null

    return {
      name: tableName,
      columns: table.columns.map((col) => ({ name: col, type: "TEXT" })),
    }
  }

  async getAllSchemas(): Promise<TableSchema[]> {
    const schemas: TableSchema[] = []
    for (const tableName of this.tables.keys()) {
      const schema = await this.getTableSchema(tableName)
      if (schema) schemas.push(schema)
    }
    return schemas
  }

  cleanup(): void {
    this.tables.clear()
    this.initialized = false
  }

  isInitialized(): boolean {
    return this.initialized
  }
}

class TXT2DBWrapper {
  private module: TXT2DBModule | null = null
  private mockDB: MockTXT2DB | null = null
  private initialized = false
  private initPromise: Promise<void> | null = null
  private useMock = false

  async initialize(): Promise<void> {
    if (this.initialized) {
      return
    }

    if (this.initPromise) {
      return this.initPromise
    }

    this.initPromise = (async () => {
      try {
        const response = await fetch("/txt2db.wasm", { method: "HEAD" })

        if (!response.ok) {
          throw new Error("WASM not found")
        }

        // @ts-ignore - Dynamic import of WASM module
        const createModule = (await import("/txt2db.js")).default

        this.module = await createModule({
          locateFile: (path: string) => {
            if (path.endsWith(".wasm")) {
              return "/txt2db.wasm"
            }
            return path
          },
          print: (text: string) => {
            console.log("[WASM stdout]:", text)
          },
          printErr: (text: string) => {
            console.error("[WASM stderr]:", text)
          },
        })

        // Initialize the database
        const initResult = this.module.initDatabase()
        console.log("[TXT2DB]", initResult)

        this.initialized = true
      } catch (error) {
        console.warn("[TXT2DB] WASM not available, using mock database. Run GitHub Actions to build WASM.")
        this.useMock = true
        this.mockDB = new MockTXT2DB()
        await this.mockDB.initialize()
        this.initialized = true
      }
    })()

    return this.initPromise
  }

  async executeQuery(sql: string): Promise<QueryResult> {
    if (!this.initialized) {
      throw new Error("Database not initialized. Call initialize() first.")
    }

    if (this.useMock && this.mockDB) {
      return await this.mockDB.executeQuery(sql)
    }

    try {
      const resultJson = this.module!.executeCommand(sql.trim())
      const result: QueryResult = JSON.parse(resultJson)

      if (result.error) {
        throw new Error(result.error)
      }

      return result
    } catch (error) {
      if (error instanceof SyntaxError) {
        // JSON parse error - likely a C++ exception was caught
        throw new Error("Invalid SQL syntax or database error")
      }
      throw error
    }
  }

  async getTables(): Promise<string[]> {
    if (!this.initialized) {
      throw new Error("Database not initialized")
    }

    if (this.useMock && this.mockDB) {
      return await this.mockDB.getTables()
    }

    try {
      const resultJson = this.module!.listTables()
      const result = JSON.parse(resultJson)
      return result.tables || []
    } catch (error) {
      console.error("Failed to get tables:", error)
      return []
    }
  }

  async getAllSchemas(): Promise<TableSchema[]> {
    if (!this.initialized) {
      throw new Error("Database not initialized")
    }

    if (this.useMock && this.mockDB) {
      return await this.mockDB.getAllSchemas()
    }

    try {
      const resultJson = this.module!.listTables()
      const result = JSON.parse(resultJson)
      return result.schemas || []
    } catch (error) {
      console.error("Failed to get schemas:", error)
      return []
    }
  }

  cleanup(): void {
    if (this.mockDB) {
      this.mockDB.cleanup()
      this.mockDB = null
    }
    if (this.module) {
      this.module.cleanup()
      this.initialized = false
      this.module = null
    }
  }

  isInitialized(): boolean {
    return this.initialized
  }
}

// Singleton instance
let instance: TXT2DBWrapper | null = null

export function getTXT2DB(): TXT2DBWrapper {
  if (!instance) {
    instance = new TXT2DBWrapper()
  }
  return instance
}

export function resetTXT2DB(): void {
  if (instance) {
    instance.cleanup()
    instance = null
  }
}
