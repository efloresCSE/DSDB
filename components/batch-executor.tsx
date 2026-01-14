"use client"

import { useState } from "react"
import { Button } from "@/components/ui/button"
import { Play, Pause, RotateCcw, AlertCircle } from "lucide-react"
import { Alert, AlertDescription } from "@/components/ui/alert"
import { Progress } from "@/components/ui/progress"

interface BatchExecutorProps {
  queries: string[]
  onExecute: (query: string) => Promise<void>
  onComplete: () => void
  onQueryExecuted?: (query: string, index: number, total: number) => void
}

export function BatchExecutor({ queries, onExecute, onComplete, onQueryExecuted }: BatchExecutorProps) {
  const [isRunning, setIsRunning] = useState(false)
  const [currentIndex, setCurrentIndex] = useState(0)
  const [errors, setErrors] = useState<Array<{ query: string; error: string; index: number }>>([])

  const validQueries = queries.filter((q) => {
    const trimmed = q.trim()
    return trimmed && !trimmed.startsWith("//") && !trimmed.startsWith("/*") && !trimmed.startsWith("*")
  })

  const startExecution = async () => {
    setIsRunning(true)
    setErrors([])
    setCurrentIndex(0)

    for (let i = 0; i < validQueries.length; i++) {
      const query = validQueries[i].trim()

      try {
        console.log(`[v0] Executing query ${i + 1}/${validQueries.length}: ${query.substring(0, 60)}...`)
        await onExecute(query)
        if (onQueryExecuted) {
          onQueryExecuted(query, i + 1, validQueries.length)
        }
        // Small delay to allow UI to update
        await new Promise((resolve) => setTimeout(resolve, 50))
      } catch (error) {
        console.error(`[v0] Error on query ${i + 1}:`, error)
        setErrors((prev) => [
          ...prev,
          {
            query: query.substring(0, 100),
            error: error instanceof Error ? error.message : "Unknown error",
            index: i + 1,
          },
        ])
      }

      setCurrentIndex(i + 1)
    }

    setIsRunning(false)
    onComplete()
  }

  const reset = () => {
    setCurrentIndex(0)
    setErrors([])
    setIsRunning(false)
  }

  const progress = validQueries.length > 0 ? (currentIndex / validQueries.length) * 100 : 0

  return (
    <div className="space-y-4">
      <div className="flex items-center gap-3">
        <Button onClick={startExecution} disabled={isRunning || currentIndex >= validQueries.length} size="sm">
          {isRunning ? (
            <>
              <Pause className="size-4 mr-2" />
              Running...
            </>
          ) : (
            <>
              <Play className="size-4 mr-2" />
              Execute Batch
            </>
          )}
        </Button>
        <Button onClick={reset} variant="outline" size="sm" disabled={isRunning}>
          <RotateCcw className="size-4 mr-2" />
          Reset
        </Button>
        <span className="text-sm text-muted-foreground">
          {currentIndex} / {validQueries.length} queries
        </span>
      </div>

      <Progress value={progress} className="h-2" />

      {errors.length > 0 && (
        <div className="space-y-2 max-h-64 overflow-y-auto">
          <div className="flex items-center gap-2 text-sm text-destructive mb-2">
            <AlertCircle className="size-4" />
            <span className="font-medium">{errors.length} error(s) encountered</span>
          </div>
          {errors.map((error, idx) => (
            <Alert key={idx} variant="destructive" className="text-xs">
              <AlertDescription>
                <div className="font-semibold mb-1">Query #{error.index}</div>
                <div className="font-mono bg-destructive/10 p-2 rounded mb-2">{error.query}</div>
                <div className="text-xs">{error.error}</div>
              </AlertDescription>
            </Alert>
          ))}
        </div>
      )}
    </div>
  )
}
