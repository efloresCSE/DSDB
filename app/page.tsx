"use client"

import { useState, useEffect } from "react"
import { SqlEditor } from "@/components/sql-editor"
import { ResultsTable } from "@/components/results-table"
import { SchemaPanel } from "@/components/schema-panel"
import { GuideModal } from "@/components/guide-modal"
import { FileUpload } from "@/components/file-upload"
import { BatchExecutor } from "@/components/batch-executor"
import { Button } from "@/components/ui/button"
import { Play, Database, Loader2, Upload, Download, Info } from "lucide-react"
import { Alert, AlertDescription } from "@/components/ui/alert"
import { useDatabase } from "@/hooks/use-database"
import {
  Dialog,
  DialogContent,
  DialogDescription,
  DialogHeader,
  DialogTitle,
  DialogTrigger,
} from "@/components/ui/dialog"
import { QueryHistory, type HistoryEntry } from "@/components/query-history"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs"

export default function Home() {
  const { initialized, loading, error: dbError, executeQuery } = useDatabase()

  const [query, setQuery] = useState("make table student fields fname, lname, major, age")
  const [results, setResults] = useState<any[] | null>(null)
  const [error, setError] = useState<string | null>(null)
  const [isExecuting, setIsExecuting] = useState(false)
  const [executionTime, setExecutionTime] = useState<number | null>(null)
  const [successMessage, setSuccessMessage] = useState<string | null>(null)
  const [batchQueries, setBatchQueries] = useState<string[]>([])
  const [showBatchDialog, setShowBatchDialog] = useState(false)
  const [lastBatchQuery, setLastBatchQuery] = useState<string | null>(null)
  const [queryHistory, setQueryHistory] = useState<HistoryEntry[]>([])
  const [activeTab, setActiveTab] = useState<"results" | "history">("results")
  const [showGuideModal, setShowGuideModal] = useState(false)

  useEffect(() => {
    console.log("[v0] Database state:", { initialized, loading, dbError })
    if (dbError) {
      setError(dbError)
    }
  }, [initialized, loading, dbError])

  const handleExecuteQuery = async (queryToExecute?: string) => {
    const finalQuery = queryToExecute || query

    if (!initialized) {
      setError("Database is still initializing. Please wait...")
      return
    }

    setIsExecuting(true)
    setError(null)
    setSuccessMessage(null)

    const historyEntry: HistoryEntry = {
      id: `${Date.now()}-${Math.random()}`,
      query: finalQuery,
      timestamp: new Date(),
      executionTime: 0,
      success: false,
    }

    try {
      const result = await executeQuery(finalQuery)

      historyEntry.executionTime = result.executionTime || 0
      historyEntry.success = true

      const isInsert = /^insert\s+into\s+(\w+)/i.test(finalQuery.trim())
      const isCreate = /^make\s+table\s+(\w+)/i.test(finalQuery.trim())

      if (result.rows && result.rows.length > 0) {
        setResults(result.rows)
        setSuccessMessage(null)
        historyEntry.result = {
          columns: result.columns,
          rows: result.rows,
        }
      } else if (result.message) {
        setSuccessMessage(result.message)

        if (isInsert || isCreate) {
          const tableMatch = finalQuery.match(/(?:insert\s+into|make\s+table)\s+(\w+)/i)
          if (tableMatch) {
            const tableName = tableMatch[1]
            try {
              const selectResult = await executeQuery(`select * from ${tableName}`)
              if (selectResult.rows && selectResult.rows.length > 0) {
                setResults(selectResult.rows)
                setSuccessMessage(`${result.message} - Showing table contents:`)
              } else {
                setResults(null)
              }
            } catch (selectError) {
              setResults(null)
            }
          }
        } else {
          setResults(null)
        }
      } else {
        setResults([])
      }

      setExecutionTime(result.executionTime || null)

      window.dispatchEvent(new CustomEvent("refreshSchema"))
    } catch (err) {
      const errorMessage = err instanceof Error ? err.message : "Query execution failed"
      setError(errorMessage)
      setResults(null)
      historyEntry.success = false
      historyEntry.error = errorMessage
    } finally {
      setQueryHistory((prev) => [historyEntry, ...prev])
      setIsExecuting(false)
    }
  }

  const handleFileLoad = (content: string, filename: string) => {
    const queries = content
      .split("\n")
      .map((line) => line.trim())
      .filter((line) => line && !line.startsWith("//") && !line.startsWith("/*"))

    setBatchQueries(queries)
    setSuccessMessage(`Successfully loaded "${filename}" with ${queries.length} commands. Ready to execute.`)
    setShowBatchDialog(true)
  }

  const handleBatchExecute = async (query: string) => {
    const historyEntry: HistoryEntry = {
      id: `${Date.now()}-${Math.random()}`,
      query,
      timestamp: new Date(),
      executionTime: 0,
      success: false,
    }

    try {
      const result = await executeQuery(query)
      historyEntry.executionTime = result.executionTime || 0
      historyEntry.success = true

      if (result.rows && result.rows.length > 0) {
        historyEntry.result = {
          columns: result.columns,
          rows: result.rows,
        }
      }
    } catch (err) {
      historyEntry.success = false
      historyEntry.error = err instanceof Error ? err.message : "Query failed"
    } finally {
      setQueryHistory((prev) => [historyEntry, ...prev])
    }
  }

  const handleBatchComplete = () => {
    if (lastBatchQuery) {
      setSuccessMessage(`Batch execution completed! Last query: ${lastBatchQuery}`)
    } else {
      setSuccessMessage("Batch execution completed!")
    }
    setShowBatchDialog(false)
    window.dispatchEvent(new CustomEvent("refreshSchema"))
  }

  const handleBatchQueryExecuted = (query: string, index: number, total: number) => {
    setLastBatchQuery(query)
    setQuery(query)
    setSuccessMessage(`Executing batch: ${index}/${total} - ${query}`)
  }

  const handleClearHistory = () => {
    setQueryHistory([])
  }

  const handleExportHistory = () => {
    const output = queryHistory
      .map((entry, index) => {
        let text = `\n[${queryHistory.length - index - 1}] ${entry.query}\n`
        text += `Timestamp: ${entry.timestamp.toLocaleString()}\n`
        text += `Execution Time: ${entry.executionTime}ms\n`

        if (entry.error) {
          text += `Error: ${entry.error}\n`
        } else if (entry.result && entry.result.rows.length > 0) {
          text += `\nResults (${entry.result.rows.length} rows):\n`
          text += entry.result.columns.join("\t") + "\n"
          entry.result.rows.forEach((row) => {
            text += entry.result!.columns.map((col) => row[col] || "").join("\t") + "\n"
          })
        } else {
          text += "Success\n"
        }

        text += "\nSQL: DONE.\n"
        return text
      })
      .reverse()
      .join("\n")

    const blob = new Blob([output], { type: "text/plain" })
    const url = URL.createObjectURL(blob)
    const a = document.createElement("a")
    a.href = url
    a.download = `dsdb_history_${new Date().toISOString().split("T")[0]}.txt`
    document.body.appendChild(a)
    a.click()
    document.body.removeChild(a)
    URL.revokeObjectURL(url)
  }

  const handleExportCurrentResults = () => {
    if (!results || results.length === 0) return

    const columns = Object.keys(results[0])
    let output = `Query: ${query}\n`
    output += `Execution Time: ${executionTime}ms\n`
    output += `Results: ${results.length} rows\n\n`
    output += columns.join("\t") + "\n"
    results.forEach((row) => {
      output += columns.map((col) => row[col] || "").join("\t") + "\n"
    })

    const blob = new Blob([output], { type: "text/plain" })
    const url = URL.createObjectURL(blob)
    const a = document.createElement("a")
    a.href = url
    a.download = `dsdb_results_${new Date().toISOString().split("T")[0]}.txt`
    document.body.appendChild(a)
    a.click()
    document.body.removeChild(a)
    URL.revokeObjectURL(url)
  }

  return (
    <div className="flex h-screen">
      {/* Schema Panel */}
      <SchemaPanel
        onShowTable={(tableName) => {
          const newQuery = `select * from ${tableName}`
          setQuery(newQuery)
          handleExecuteQuery(newQuery)
          setActiveTab("results")
        }}
      />

      {/* Main Content */}
      <div className="flex-1 flex flex-col">
        {/* Header */}
        <header className="border-b border-border bg-card px-6 py-4">
          <div className="flex items-center justify-between">
            <div className="flex items-center gap-3">
              <Database className="size-8 text-primary" />
              <div>
                <h1 className="text-xl font-semibold text-foreground">DSDB</h1>
                <p className="text-sm text-muted-foreground">
                  {loading && "Initializing..."}
                  {initialized && "Ready"}
                </p>
              </div>
            </div>
            <div className="flex items-center gap-2">
              <Button variant="outline" onClick={() => setShowGuideModal(true)}>
                <Info className="size-4 mr-2" />
                Info
              </Button>

              <Dialog>
                <DialogTrigger asChild>
                  <Button variant="outline">
                    <Upload className="size-4 mr-2" />
                    Load File
                  </Button>
                </DialogTrigger>
                <DialogContent>
                  <DialogHeader>
                    <DialogTitle>Upload SQL-like Commands</DialogTitle>
                    <DialogDescription>
                      Upload a .txt file containing SQL-like commands to execute them in batch
                    </DialogDescription>
                  </DialogHeader>
                  <FileUpload onFileLoad={handleFileLoad} />
                </DialogContent>
              </Dialog>

              <Button onClick={() => handleExecuteQuery()} disabled={isExecuting || loading || !initialized} size="lg">
                {isExecuting ? (
                  <>
                    <Loader2 className="size-4 mr-2 animate-spin" />
                    Executing...
                  </>
                ) : (
                  <>
                    <Play className="size-4 mr-2" />
                    Run Query
                  </>
                )}
              </Button>
            </div>
          </div>
        </header>

        {/* Query Editor */}
        <div className="border-b border-border">
          <SqlEditor value={query} onChange={setQuery} />
        </div>

        {/* Results Section */}
        <div className="flex-1 overflow-hidden flex">
          <div className="flex-1 flex flex-col min-h-0">
            <Tabs
              value={activeTab}
              onValueChange={(v) => setActiveTab(v as "results" | "history")}
              className="flex-1 flex flex-col min-h-0"
            >
              <div className="border-b border-border px-6 pt-4 shrink-0">
                <TabsList>
                  <TabsTrigger value="results">Results</TabsTrigger>
                  <TabsTrigger value="history">
                    History
                    {queryHistory.length > 0 && (
                      <span className="ml-2 text-xs bg-primary/20 text-primary px-2 py-0.5 rounded-full">
                        {queryHistory.length}
                      </span>
                    )}
                  </TabsTrigger>
                </TabsList>
              </div>

              <TabsContent
                value="results"
                className="flex-1 overflow-y-auto p-6 mt-0 data-[state=inactive]:hidden scrollbar-thin scrollbar-thumb-border/50 scrollbar-track-transparent hover:scrollbar-thumb-border scrollbar-dark"
              >
                {loading && (
                  <Alert className="mb-4">
                    <Loader2 className="size-4 animate-spin" />
                    <AlertDescription>Loading database engine...</AlertDescription>
                  </Alert>
                )}

                {error && (
                  <Alert variant="destructive" className="mb-4">
                    <AlertDescription>{error}</AlertDescription>
                  </Alert>
                )}

                {successMessage && (
                  <Alert className="mb-4">
                    <AlertDescription>{successMessage}</AlertDescription>
                  </Alert>
                )}

                {results && (
                  <>
                    <div className="mb-4 flex items-center justify-between">
                      <div className="text-sm text-muted-foreground">
                        Query executed in {executionTime}ms • {results.length} rows returned
                      </div>
                      <Button variant="outline" size="sm" onClick={handleExportCurrentResults}>
                        <Download className="size-4 mr-2" />
                        Export Results
                      </Button>
                    </div>
                    <ResultsTable data={results} />
                  </>
                )}

                {!results && !error && !successMessage && !loading && (
                  <div className="flex items-center justify-center h-64 text-muted-foreground">
                    <div className="text-center">
                      <Database className="size-12 mx-auto mb-3 opacity-50" />
                      <p>Run a query to see results</p>
                    </div>
                  </div>
                )}
              </TabsContent>

              <TabsContent value="history" className="flex-1 mt-0 min-h-0 data-[state=inactive]:hidden">
                <QueryHistory
                  history={queryHistory}
                  onClearHistory={handleClearHistory}
                  onExportHistory={handleExportHistory}
                  onSelectQuery={(q) => {
                    setQuery(q)
                    setActiveTab("results")
                  }}
                />
              </TabsContent>
            </Tabs>
          </div>
        </div>
      </div>

      <Dialog open={showBatchDialog} onOpenChange={setShowBatchDialog}>
        <DialogContent className="max-w-2xl">
          <DialogHeader>
            <DialogTitle>Batch Execution</DialogTitle>
            <DialogDescription>
              Execute {batchQueries.length} SQL-like commands from the uploaded file
            </DialogDescription>
          </DialogHeader>
          <BatchExecutor
            queries={batchQueries}
            onExecute={handleBatchExecute}
            onComplete={handleBatchComplete}
            onQueryExecuted={handleBatchQueryExecuted}
          />
        </DialogContent>
      </Dialog>

      {/* Guide Modal */}
      <GuideModal
        open={showGuideModal}
        onOpenChange={setShowGuideModal}
        onSelectQuery={(q) => {
          setQuery(q)
          setActiveTab("results")
        }}
      />
    </div>
  )
}
