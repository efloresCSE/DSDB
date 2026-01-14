"use client"

// React hook for database operations

import { useState, useEffect, useCallback } from "react"
import { getTXT2DB, type QueryResult, type TableSchema } from "@/lib/wasm-loader"
import { parseSQL, formatTableOutput } from "@/lib/sql-parser"

export interface DatabaseState {
  initialized: boolean
  loading: boolean
  error: string | null
}

export interface QueryResultData {
  columns: string[]
  rows: Array<Record<string, string>>
  message?: string
  executionTime?: number
}

export function useDatabase() {
  const [state, setState] = useState<DatabaseState>({
    initialized: false,
    loading: true,
    error: null,
  })

  useEffect(() => {
    const initDB = async () => {
      try {
        const db = getTXT2DB()
        await db.initialize()
        setState({
          initialized: true,
          loading: false,
          error: null,
        })
      } catch (error) {
        setState({
          initialized: false,
          loading: false,
          error: error instanceof Error ? error.message : "Failed to initialize database",
        })
      }
    }

    initDB()
  }, [])

  const executeQuery = useCallback(
    async (sql: string): Promise<QueryResultData> => {
      if (!state.initialized) {
        throw new Error("Database not initialized")
      }

      const startTime = performance.now()
      const db = getTXT2DB()
      const result: QueryResult = await db.executeQuery(sql)
      const executionTime = performance.now() - startTime

      const parsed = parseSQL(sql)

      if (result.type === "select" && result.output) {
        const { columns, rows } = formatTableOutput(result.output)
        return {
          columns,
          rows,
          executionTime: Math.round(executionTime),
        }
      }

      return {
        columns: [],
        rows: [],
        message: result.message || `${parsed.command} executed successfully`,
        executionTime: Math.round(executionTime),
      }
    },
    [state.initialized],
  )

  const getTables = useCallback(async (): Promise<string[]> => {
    if (!state.initialized) {
      return []
    }

    const db = getTXT2DB()
    return await db.getTables()
  }, [state.initialized])

  const getSchemas = useCallback(async (): Promise<TableSchema[]> => {
    if (!state.initialized) {
      return []
    }

    const db = getTXT2DB()
    return await db.getAllSchemas()
  }, [state.initialized])

  return {
    ...state,
    executeQuery,
    getTables,
    getSchemas,
  }
}
