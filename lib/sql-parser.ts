// SQL parsing utilities for the frontend

export interface ParsedQuery {
  command: "create" | "insert" | "select" | "unknown"
  table?: string
  fields?: string[]
  values?: string[]
  where?: string
  raw: string
}

export function parseSQL(sql: string): ParsedQuery {
  const trimmed = sql.trim()
  const lower = trimmed.toLowerCase()

  const result: ParsedQuery = {
    command: "unknown",
    raw: trimmed,
  }

  // Parse CREATE/MAKE TABLE
  if (lower.startsWith("create table") || lower.startsWith("make table")) {
    result.command = "create"
    const tableMatch = trimmed.match(/table\s+(\w+)/i)
    if (tableMatch) {
      result.table = tableMatch[1]
    }
    const fieldsMatch = trimmed.match(/fields\s+(.+)/i)
    if (fieldsMatch) {
      result.fields = fieldsMatch[1].split(",").map((f) => f.trim())
    }
  }
  // Parse INSERT
  else if (lower.startsWith("insert into")) {
    result.command = "insert"
    const tableMatch = trimmed.match(/insert\s+into\s+(\w+)/i)
    if (tableMatch) {
      result.table = tableMatch[1]
    }
    const valuesMatch = trimmed.match(/values\s+(.+)/i)
    if (valuesMatch) {
      result.values = valuesMatch[1].split(",").map((v) => v.trim())
    }
  }
  // Parse SELECT
  else if (lower.startsWith("select")) {
    result.command = "select"
    const fromMatch = trimmed.match(/from\s+(\w+)/i)
    if (fromMatch) {
      result.table = fromMatch[1]
    }
    const whereMatch = trimmed.match(/where\s+(.+)/i)
    if (whereMatch) {
      result.where = whereMatch[1]
    }
  }

  return result
}

export function formatTableOutput(output: string): {
  columns: string[]
  rows: Array<Record<string, string>>
} {
  console.log("[v0] Formatting output:", output)

  const lines = output.split("\n").filter((line) => line.trim())

  if (lines.length === 0) {
    return { columns: [], rows: [] }
  }

  // First line is headers, rest are data rows
  const headerLine = lines[0]
  const columns = headerLine.split("\t").map((col) => col.trim())

  console.log("[v0] Parsed columns:", columns)

  const rows: Array<Record<string, string>> = []

  for (let i = 1; i < lines.length; i++) {
    const line = lines[i].trim()
    if (!line) continue

    const values = line.split("\t").map((v) => v.trim())
    const row: Record<string, string> = {}

    for (let j = 0; j < columns.length && j < values.length; j++) {
      row[columns[j]] = values[j]
    }

    rows.push(row)
  }

  console.log("[v0] Parsed rows:", rows)

  return { columns, rows }
}
