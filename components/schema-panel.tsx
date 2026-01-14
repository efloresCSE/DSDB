"use client"

import { useState, useEffect } from "react"
import { ChevronRight, TableIcon, Database, Eye } from "lucide-react"
import { cn } from "@/lib/utils"
import { useDatabase } from "@/hooks/use-database"
import { Button } from "@/components/ui/button"
import type { TableSchema } from "@/lib/wasm-loader"

export function SchemaPanel({ onShowTable }: { onShowTable?: (tableName: string) => void }) {
  const [expandedTables, setExpandedTables] = useState<Set<string>>(new Set())
  const [schemas, setSchemas] = useState<TableSchema[]>([])
  const [loading, setLoading] = useState(false)
  const { initialized, getSchemas } = useDatabase()

  const refreshSchemas = async () => {
    if (!initialized) return

    setLoading(true)
    try {
      const fetchedSchemas = await getSchemas()
      setSchemas(fetchedSchemas)
    } catch (error) {
      console.error("Failed to fetch schemas:", error)
    } finally {
      setLoading(false)
    }
  }

  useEffect(() => {
    refreshSchemas()
  }, [initialized])

  useEffect(() => {
    const handleRefresh = () => {
      refreshSchemas()
    }

    window.addEventListener("refreshSchema", handleRefresh)
    return () => window.removeEventListener("refreshSchema", handleRefresh)
  }, [initialized])

  const toggleTable = (tableName: string) => {
    const newExpanded = new Set(expandedTables)
    if (newExpanded.has(tableName)) {
      newExpanded.delete(tableName)
    } else {
      newExpanded.add(tableName)
    }
    setExpandedTables(newExpanded)
  }

  return (
    <aside className="w-64 border-r border-border flex flex-col">
      <div className="p-4 border-b border-border">
        <div className="flex items-center">
          <div className="flex items-center gap-2 text-sm font-semibold text-foreground">
            <Database className="size-4" />
            Schema
          </div>
        </div>
        {schemas.length > 0 && <div className="text-xs text-muted-foreground mt-1">{schemas.length} table(s)</div>}
      </div>
      <div className="flex-1 overflow-auto p-2">
        {schemas.length === 0 && (
          <div className="text-center text-sm text-muted-foreground p-4">
            {initialized ? "No tables yet" : "Loading..."}
          </div>
        )}
        {schemas.map((table) => (
          <div key={table.name} className="mb-1">
            <div className="flex items-center gap-1">
              <button
                onClick={() => toggleTable(table.name)}
                className="flex-1 flex items-center gap-2 px-3 py-2 text-sm text-foreground hover:bg-muted rounded-md transition-colors"
              >
                <ChevronRight
                  className={cn("size-4 transition-transform", expandedTables.has(table.name) && "rotate-90")}
                />
                <TableIcon className="size-4 text-primary" />
                <span className="font-medium">{table.name}</span>
              </button>
              <Button
                variant="ghost"
                size="sm"
                onClick={() => onShowTable?.(table.name)}
                className="h-8 w-8 p-0 shrink-0"
                title={`Show all rows from ${table.name}`}
              >
                <Eye className="size-3.5" />
              </Button>
            </div>
            {expandedTables.has(table.name) && (
              <div className="ml-6 mt-1 space-y-1">
                {table.columns.map((column) => (
                  <div
                    key={column.name}
                    className="flex items-center justify-between px-3 py-1.5 text-xs rounded-md hover:bg-muted/50"
                  >
                    <span className="text-foreground">{column.name}</span>
                  </div>
                ))}
              </div>
            )}
          </div>
        ))}
      </div>
    </aside>
  )
}
