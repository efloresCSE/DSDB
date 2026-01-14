"use client"

import type React from "react"

import { useState } from "react"
import { Download, Trash2, ChevronDown, ChevronRight, Clock } from "lucide-react"
import { Button } from "@/components/ui/button"
import { Card } from "@/components/ui/card"
import { cn } from "@/lib/utils"

export interface HistoryEntry {
  id: string
  query: string
  timestamp: Date
  executionTime: number
  success: boolean
  error?: string
  result?: {
    columns: string[]
    rows: Array<Record<string, string>>
  }
}

interface QueryHistoryProps {
  history: HistoryEntry[]
  onClearHistory: () => void
  onExportHistory: () => void
  onSelectQuery: (query: string) => void
}

export function QueryHistory({ history, onClearHistory, onExportHistory, onSelectQuery }: QueryHistoryProps) {
  const [expandedIds, setExpandedIds] = useState<Set<string>>(new Set())
  const [fullyExpandedResults, setFullyExpandedResults] = useState<Set<string>>(new Set())

  const toggleExpanded = (id: string) => {
    const newExpanded = new Set(expandedIds)
    if (newExpanded.has(id)) {
      newExpanded.delete(id)
    } else {
      newExpanded.add(id)
    }
    setExpandedIds(newExpanded)
  }

  const toggleFullResults = (id: string, e: React.MouseEvent) => {
    e.stopPropagation()
    const newFullyExpanded = new Set(fullyExpandedResults)
    if (newFullyExpanded.has(id)) {
      newFullyExpanded.delete(id)
    } else {
      newFullyExpanded.add(id)
    }
    setFullyExpandedResults(newFullyExpanded)
  }

  const formatTimestamp = (date: Date) => {
    return new Intl.DateTimeFormat("en-US", {
      hour: "2-digit",
      minute: "2-digit",
      second: "2-digit",
    }).format(date)
  }

  return (
    <div className="flex flex-col h-full min-h-0">
      <div className="p-4 border-b border-border flex items-center justify-between shrink-0">
        <h3 className="font-semibold text-sm">Query History</h3>
        <div className="flex items-center gap-2">
          <Button variant="ghost" size="sm" onClick={onExportHistory} disabled={history.length === 0}>
            <Download className="size-4" />
          </Button>
          <Button variant="ghost" size="sm" onClick={onClearHistory} disabled={history.length === 0}>
            <Trash2 className="size-4" />
          </Button>
        </div>
      </div>

      <div className="flex-1 min-h-0 overflow-y-auto scrollbar-dark">
        <div className="p-2 space-y-2">
          {history.length === 0 ? (
            <div className="text-center text-muted-foreground text-sm py-8">
              <Clock className="size-8 mx-auto mb-2 opacity-50" />
              <p>No query history yet</p>
            </div>
          ) : (
            history.map((entry) => {
              const isExpanded = expandedIds.has(entry.id)
              const isFullyExpanded = fullyExpandedResults.has(entry.id)
              const hasMoreRows = entry.result && entry.result.rows.length > 5
              const rowsToShow = isFullyExpanded ? entry.result?.rows.length : 5

              return (
                <Card
                  key={entry.id}
                  className={cn(
                    "p-3 cursor-pointer hover:bg-accent/50 transition-colors",
                    !entry.success && "border-destructive/50",
                  )}
                >
                  <div onClick={() => toggleExpanded(entry.id)}>
                    <div className="flex items-start gap-2">
                      {isExpanded ? (
                        <ChevronDown className="size-4 mt-1 shrink-0" />
                      ) : (
                        <ChevronRight className="size-4 mt-1 shrink-0" />
                      )}
                      <div className="flex-1 min-w-0">
                        <div className="flex items-center gap-2 mb-1">
                          <span className="text-xs text-muted-foreground">{formatTimestamp(entry.timestamp)}</span>
                          <span className="text-xs text-muted-foreground">•</span>
                          <span className="text-xs text-muted-foreground">{entry.executionTime}ms</span>
                          {!entry.success && (
                            <>
                              <span className="text-xs text-muted-foreground">•</span>
                              <span className="text-xs text-destructive">Failed</span>
                            </>
                          )}
                        </div>
                        <code className="text-xs font-mono block truncate">{entry.query}</code>
                      </div>
                    </div>

                    {isExpanded && (
                      <div className="mt-3 pt-3 border-t border-border">
                        <div className="space-y-3">
                          <div>
                            <div className="text-xs text-muted-foreground mb-1">Query:</div>
                            <code
                              className="text-xs font-mono bg-muted p-2 rounded block whitespace-pre-wrap break-all cursor-pointer hover:bg-muted/80"
                              onClick={(e) => {
                                e.stopPropagation()
                                onSelectQuery(entry.query)
                              }}
                            >
                              {entry.query}
                            </code>
                          </div>

                          {entry.error && (
                            <div>
                              <div className="text-xs text-destructive mb-1">Error:</div>
                              <div className="text-xs bg-destructive/10 text-destructive p-2 rounded">
                                {entry.error}
                              </div>
                            </div>
                          )}

                          {entry.result && entry.result.rows.length > 0 && (
                            <div>
                              <div className="text-xs text-muted-foreground mb-1">
                                Result: {entry.result.rows.length} row(s)
                              </div>
                              <div
                                className={cn(
                                  "text-xs bg-muted p-2 rounded overflow-x-auto",
                                  isFullyExpanded && hasMoreRows && "max-h-[400px] overflow-y-auto",
                                )}
                              >
                                <table className="w-full">
                                  <thead className="sticky top-0 bg-muted z-10">
                                    <tr className="border-b border-border">
                                      {entry.result.columns.map((col) => (
                                        <th key={col} className="text-left p-1 font-semibold">
                                          {col}
                                        </th>
                                      ))}
                                    </tr>
                                  </thead>
                                  <tbody>
                                    {entry.result.rows.slice(0, rowsToShow).map((row, idx) => (
                                      <tr key={idx} className="border-b border-border/50">
                                        {entry.result!.columns.map((col) => (
                                          <td key={col} className="p-1">
                                            {row[col]}
                                          </td>
                                        ))}
                                      </tr>
                                    ))}
                                  </tbody>
                                </table>
                                {hasMoreRows && (
                                  <button
                                    onClick={(e) => toggleFullResults(entry.id, e)}
                                    className="w-full text-center text-muted-foreground hover:text-foreground mt-2 py-1 hover:bg-accent/50 rounded transition-colors"
                                  >
                                    {isFullyExpanded ? (
                                      <span className="flex items-center justify-center gap-1">
                                        <ChevronDown className="size-3" />
                                        Show less
                                      </span>
                                    ) : (
                                      <span>
                                        ... show all {entry.result.rows.length} rows (currently showing {rowsToShow})
                                      </span>
                                    )}
                                  </button>
                                )}
                              </div>
                            </div>
                          )}
                        </div>
                      </div>
                    )}
                  </div>
                </Card>
              )
            })
          )}
        </div>
      </div>
    </div>
  )
}
