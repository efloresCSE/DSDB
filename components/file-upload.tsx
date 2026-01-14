"use client"

import type React from "react"

import { useCallback, useState } from "react"
import { Button } from "@/components/ui/button"
import { Upload, FileText } from "lucide-react"
import { Alert, AlertDescription } from "@/components/ui/alert"

interface FileUploadProps {
  onFileLoad: (content: string, filename: string) => void
}

export function FileUpload({ onFileLoad }: FileUploadProps) {
  const [isDragging, setIsDragging] = useState(false)
  const [error, setError] = useState<string | null>(null)

  const handleFile = useCallback(
    (file: File) => {
      setError(null)

      if (!file.name.endsWith(".txt")) {
        setError("Please upload a .txt file with SQL-like commands")
        return
      }

      const reader = new FileReader()
      reader.onload = (e) => {
        const content = e.target?.result as string
        onFileLoad(content, file.name)
      }
      reader.onerror = () => {
        setError("Failed to read file")
      }
      reader.readAsText(file)
    },
    [onFileLoad],
  )

  const handleDrop = useCallback(
    (e: React.DragEvent<HTMLDivElement>) => {
      e.preventDefault()
      setIsDragging(false)

      const file = e.dataTransfer.files[0]
      if (file) {
        handleFile(file)
      }
    },
    [handleFile],
  )

  const handleDragOver = useCallback((e: React.DragEvent<HTMLDivElement>) => {
    e.preventDefault()
    setIsDragging(true)
  }, [])

  const handleDragLeave = useCallback((e: React.DragEvent<HTMLDivElement>) => {
    e.preventDefault()
    setIsDragging(false)
  }, [])

  const handleFileSelect = useCallback(
    (e: React.ChangeEvent<HTMLInputElement>) => {
      const file = e.target.files?.[0]
      if (file) {
        handleFile(file)
      }
    },
    [handleFile],
  )

  return (
    <div className="space-y-3">
      <div
        onDrop={handleDrop}
        onDragOver={handleDragOver}
        onDragLeave={handleDragLeave}
        className={`border-2 border-dashed rounded-lg p-6 text-center transition-colors ${
          isDragging ? "border-primary bg-primary/5" : "border-border bg-muted/30"
        }`}
      >
        <FileText className="size-8 mx-auto mb-3 text-muted-foreground" />
        <p className="text-sm text-muted-foreground mb-3">
          Drag and drop a .txt file with SQL-like commands, or click to browse
        </p>
        <Button variant="outline" size="sm" asChild>
          <label className="cursor-pointer">
            <Upload className="size-4 mr-2" />
            Choose File
            <input type="file" accept=".txt" className="hidden" onChange={handleFileSelect} />
          </label>
        </Button>
      </div>

      {error && (
        <Alert variant="destructive">
          <AlertDescription>{error}</AlertDescription>
        </Alert>
      )}
    </div>
  )
}
