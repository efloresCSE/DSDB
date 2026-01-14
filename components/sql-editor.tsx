"use client"

import { useEffect, useRef } from "react"

interface SqlEditorProps {
  value: string
  onChange: (value: string) => void
}

export function SqlEditor({ value, onChange }: SqlEditorProps) {
  const textareaRef = useRef<HTMLTextAreaElement>(null)

  useEffect(() => {
    if (textareaRef.current) {
      textareaRef.current.style.height = "auto"
      textareaRef.current.style.height = textareaRef.current.scrollHeight + "px"
    }
  }, [value])

  return (
    <div className="relative bg-background">
      <div className="absolute left-0 top-0 bottom-0 w-12 bg-background flex flex-col items-center pt-4 text-xs text-muted-foreground font-mono">
        {value.split("\n").map((_, i) => (
          <div key={i} className="leading-6">
            {i + 1}
          </div>
        ))}
      </div>
      <textarea
        ref={textareaRef}
        value={value}
        onChange={(e) => onChange(e.target.value)}
        className="w-full min-h-[200px] bg-transparent pl-16 pr-6 py-4 font-mono text-sm text-foreground resize-none focus:outline-none focus:ring-2 focus:ring-primary/20"
        placeholder="Enter your SQL-like query here..."
        spellCheck={false}
      />
    </div>
  )
}
