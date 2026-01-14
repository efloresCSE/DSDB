"use client"

import { Table, TableBody, TableCell, TableHead, TableHeader, TableRow } from "@/components/ui/table"
import { useState } from "react"
import { ArrowUpDown } from "lucide-react"

interface ResultsTableProps {
  data: Record<string, any>[]
}

export function ResultsTable({ data }: ResultsTableProps) {
  const [sortColumn, setSortColumn] = useState<string | null>(null)
  const [sortDirection, setSortDirection] = useState<"asc" | "desc">("asc")

  if (!data || data.length === 0) {
    return <div className="text-center py-12 text-muted-foreground">No results to display</div>
  }

  const columns = Object.keys(data[0])

  const handleSort = (column: string) => {
    if (sortColumn === column) {
      setSortDirection(sortDirection === "asc" ? "desc" : "asc")
    } else {
      setSortColumn(column)
      setSortDirection("asc")
    }
  }

  const sortedData = [...data].sort((a, b) => {
    if (!sortColumn) return 0

    const aVal = a[sortColumn]
    const bVal = b[sortColumn]

    if (aVal < bVal) return sortDirection === "asc" ? -1 : 1
    if (aVal > bVal) return sortDirection === "asc" ? 1 : -1
    return 0
  })

  return (
    <div className="rounded-lg border border-border dark-scrollbar max-h-96 overflow-y-auto">
      <Table>
        <TableHeader className="sticky top-0 bg-muted/50 z-10">
          <TableRow>
            {columns.map((column) => (
              <TableHead
                key={column}
                className="font-semibold cursor-pointer hover:bg-muted/70 transition-colors"
                onClick={() => handleSort(column)}
              >
                <div className="flex items-center gap-2">
                  {column}
                  <ArrowUpDown className="size-3 text-muted-foreground" />
                </div>
              </TableHead>
            ))}
          </TableRow>
        </TableHeader>
        <TableBody>
          {sortedData.map((row, i) => (
            <TableRow key={i} className="hover:bg-muted/30">
              {columns.map((column) => (
                <TableCell key={column} className="font-mono text-sm">
                  {row[column]?.toString() || "-"}
                </TableCell>
              ))}
            </TableRow>
          ))}
        </TableBody>
      </Table>
    </div>
  )
}
