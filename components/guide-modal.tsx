"use client"

import { Dialog, DialogContent, DialogDescription, DialogHeader, DialogTitle } from "@/components/ui/dialog"
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs"
import { Button } from "@/components/ui/button"
import { Code2, Database } from "lucide-react"

interface GuideModalProps {
  open: boolean
  onOpenChange: (open: boolean) => void
  onSelectQuery?: (query: string) => void
}

const sampleContent = `//****************************************************************************
//		CREATE TABLES AND INSERT RECORDS
//****************************************************************************


make table employee fields  last, 		first, 			dep,  		salary, 	year
insert into employee values Blow, 		Joe, 			CS,		 	100000, 	2018
insert into employee values Blow, 		JoAnn,			Physics, 	200000, 	2016
insert into employee values Johnson, 	Jack, 			HR, 		150000, 	2014
insert into employee values Johnson, 	"Jimmy", 		Chemistry,	140000, 	2018
insert into employee values Yao,	 	Jimmy, 			Math,		145000, 	2014
insert into employee values "Yao", 		Flo, 			CS,			147000, 	2012
insert into employee values Yang, 		Bo, 			CS,			160000, 	2013
insert into employee values Jackson,	"Flo", 			Math,		165000, 	2017
insert into employee values Jackson,	Bo,	 			Chemistry,	130000, 	2011
insert into employee values "Jackson",	Billy, 			Math,	 	170000, 	2017
insert into employee values Johnson,	"Mary Ann", 	Math,		165000, 	2014
insert into employee values Johnson,	"Billy Bob", 	Physics,	142000, 	2014
insert into employee values Johnson,	Billy, 			"Phys Ed",	102000, 	2014
insert into employee values "Van Gogh",	Vincent, 		Art,		240000, 	2015
insert into employee values "Van Gogh",	Vincent, 		CS,			245000, 	2015
insert into employee values "Van Gogh",	"Jim Bob", 		"Phys Ed",	230000, 	2010
select * from employee 

make table student fields fname, lname, major, age, company
insert into student values Flo, 			Yao, 		CS, 				20, 	Google
insert into student values Bo, 				Yang, 		CS, 				28,		Microsoft
insert into student values "Sammuel L.", 	Jackson, 	CS, 				40,		Uber
insert into student values "Flo", 			"Jackson", 	Math,	 			21,		Google
insert into student values "Greg", 			"Pearson", 	Physics,			20,		Amazon
insert into student values "Jim Bob", 		Smith, 		Math,	 			23,		Verizon
insert into student values Calvin, 			Woo, 		Physics,			22,		Uber
insert into student values "Anna Grace", 	"Del Rio", 	CS,	 				22,		USAF
insert into student values "Teresa Mae", 	Gowers, 	Chemistry,			22,		Verizon
insert into student values Alex,			Smith,		"Gender Studies", 	53,		Amazon
select * from student


//****************************************************************************
//		SIMPLE SELECT:
//****************************************************************************

select * from student

//------- simple strings -------------------
select * from student where lname = Jackson
select * from student where major = CS
select * from student where company = Uber

//----- quoted strings ---------------------
select * from student where lname = "Del Rio"
select * from student where fname = "Jim Bob"
select * from student where fname = "Anna Grace"
select * from student where fname = "Teresa Mae"

//-------- non-existing string ------------------
select * from student where lname = "Does Not Exist"

//****************************************************************************
//		RELATIONAL OPERATORS:
//****************************************************************************

select * from student where lname > Yang
select * from student where major > Math
select * from employee where salary > 200000
select * from employee where dep > Chemistry

//****************************************************************************
//		LOGICAL OPERATORS
//****************************************************************************

select * from student where fname = "Flo" and lname = "Yao"
select * from student where major = "CS" and age < 25
select * from employee where dep = CS and salary > 150000
select * from employee where last = Jackson and year < 2015

select * from student where fname = Flo or lname = Jackson
select * from student where age >=40  or company = Verizon
select * from employee where first = Bo or last = Johnson
select * from employee where year >= 2015 or dep = CS`

export function GuideModal({ open, onOpenChange, onSelectQuery }: GuideModalProps) {
  const handleTryQuery = (query: string) => {
    onSelectQuery?.(query)
    onOpenChange(false)
  }

  const handleDownloadSample = () => {
    const blob = new Blob([sampleContent], { type: "text/plain" })
    const url = URL.createObjectURL(blob)
    const link = document.createElement("a")
    link.href = url
    link.download = "sample-batch.txt"
    document.body.appendChild(link)
    link.click()
    document.body.removeChild(link)
    URL.revokeObjectURL(url)
  }

  return (
    <Dialog open={open} onOpenChange={onOpenChange}>
      <DialogContent className="max-w-6xl max-h-[90vh] flex flex-col p-6">
        <DialogHeader className="flex-shrink-0 pb-3">
          <DialogTitle className="text-xl md:text-2xl">DSDB Quick Start Guide</DialogTitle>
          <DialogDescription className="text-xs md:text-sm">Learn the basics of DSDB syntax</DialogDescription>
        </DialogHeader>

        <Tabs defaultValue="about" className="flex-1 flex flex-col min-h-0 overflow-hidden">
          <TabsList className="flex flex-shrink-0 h-9 bg-muted rounded-lg p-1">
            <TabsTrigger value="about" className="flex-1 text-xs whitespace-nowrap">
              About
            </TabsTrigger>
            <div className="w-px bg-border/40 self-stretch my-1.5" />
            <TabsTrigger value="tables" className="flex-1 text-xs whitespace-nowrap">
              Create Tables
            </TabsTrigger>
            <div className="w-px bg-border/40 self-stretch my-1.5" />
            <TabsTrigger value="insert" className="flex-1 text-xs whitespace-nowrap">
              Insert Data
            </TabsTrigger>
            <div className="w-px bg-border/40 self-stretch my-1.5" />
            <TabsTrigger value="select" className="flex-1 text-xs whitespace-nowrap">
              Select Queries
            </TabsTrigger>
            <div className="w-px bg-border/40 self-stretch my-1.5" />
            <TabsTrigger value="batch" className="flex-1 text-xs whitespace-nowrap">
              Batch Files
            </TabsTrigger>
          </TabsList>

          <div className="flex-1 overflow-y-auto mt-4 pr-2 scrollbar-hide">
            <TabsContent value="about" className="space-y-4 mt-0 mr-2">
              <div className="flex items-center gap-3 mb-4">
                <Database className="size-8 text-primary" />
                <div>
                  <h3 className="font-semibold text-lg md:text-xl">DSDB</h3>
                </div>
              </div>

              <div className="space-y-3">
                <div>
                  <h4 className="font-medium text-sm md:text-base mb-2">What is DSDB?</h4>
                  <p className="text-xs md:text-sm text-muted-foreground leading-relaxed">
                    DSDB is a web-based SQL-like database interface that runs entirely in your browser using
                    WebAssembly. Create tables, insert data, and query with SQL-like commands. The C++ database engine
                    is compiled to WebAssembly for native performance directly in your browser.
                  </p>
                </div>

                <div>
                  <h4 className="font-medium text-sm md:text-base mb-2">Key Features</h4>
                  <ul className="space-y-1.5 text-xs md:text-sm text-muted-foreground">
                    <li className="flex items-start gap-2">
                      <span className="text-primary mt-0.5">•</span>
                      <span>SQL-like query editor with line numbers</span>
                    </li>
                    <li className="flex items-start gap-2">
                      <span className="text-primary mt-0.5">•</span>
                      <span>Results table with export functionality</span>
                    </li>
                    <li className="flex items-start gap-2">
                      <span className="text-primary mt-0.5">•</span>
                      <span>Live schema browser with quick table viewing</span>
                    </li>
                    <li className="flex items-start gap-2">
                      <span className="text-primary mt-0.5">•</span>
                      <span>Batch file execution with drag-and-drop and progress tracking</span>
                    </li>
                    <li className="flex items-start gap-2">
                      <span className="text-primary mt-0.5">•</span>
                      <span>Complete query history with execution times and export</span>
                    </li>
                    <li className="flex items-start gap-2">
                      <span className="text-primary mt-0.5">•</span>
                      <span>100% client-side - no server required</span>
                    </li>
                  </ul>
                </div>

                <div>
                  <h4 className="font-medium text-sm md:text-base mb-2">Getting Started</h4>
                  <p className="text-xs md:text-sm text-muted-foreground leading-relaxed">
                    Use the tabs above to learn about creating tables, inserting data, and querying. Each example
                    includes a "Try" button to load it into the query editor.
                  </p>
                </div>
              </div>
            </TabsContent>

            <TabsContent value="batch" className="space-y-3 mt-0 mr-2">
              <div>
                <h3 className="font-semibold text-base md:text-lg mb-2">Using Batch Files</h3>
                <p className="text-xs md:text-sm text-muted-foreground mb-3">
                  Load and execute multiple SQL-like commands from a text file
                </p>
              </div>

              <div className="space-y-3">
                <div>
                  <h4 className="font-medium text-sm md:text-base mb-2">What are Batch Files?</h4>
                  <p className="text-xs md:text-sm text-muted-foreground leading-relaxed">
                    Batch files contain multiple SQL-like commands in a text file. DSDB will execute each command
                    sequentially, making it easy to set up entire databases with multiple tables and records.
                  </p>
                </div>

                <div>
                  <h4 className="font-medium text-sm md:text-base mb-2">File Format</h4>
                  <ul className="space-y-1.5 text-xs md:text-sm text-muted-foreground">
                    <li className="flex items-start gap-2">
                      <span className="text-primary mt-0.5">•</span>
                      <span>One SQL-like command per line</span>
                    </li>
                    <li className="flex items-start gap-2">
                      <span className="text-primary mt-0.5">•</span>
                      <span>Comments start with // and are ignored</span>
                    </li>
                    <li className="flex items-start gap-2">
                      <span className="text-primary mt-0.5">•</span>
                      <span>Empty lines are ignored</span>
                    </li>
                    <li className="flex items-start gap-2">
                      <span className="text-primary mt-0.5">•</span>
                      <span>Plain text format (.txt extension)</span>
                    </li>
                  </ul>
                </div>

                <div>
                  <h4 className="font-medium text-sm md:text-base mb-2">How to Use</h4>
                  <ol className="space-y-2 text-xs md:text-sm text-muted-foreground list-decimal list-inside">
                    <li>Click the "Load File" button in the header</li>
                    <li>Select your .txt batch file or drag it in</li>
                    <li>Click "Execute Batch" to run all commands</li>
                    <li>View progress and results in the History tab</li>
                  </ol>
                </div>

                <div className="bg-muted/50 p-3 rounded-lg">
                  <h4 className="font-medium text-xs md:text-sm mb-2">Example Batch File Structure</h4>
                  <div className="h-[300px] bg-background p-2 rounded overflow-y-auto scrollbar-hide">
                    <pre className="text-[10px] font-mono leading-tight">{sampleContent}</pre>
                  </div>
                </div>

                <div className="flex justify-center pt-2">
                  <Button onClick={handleDownloadSample} size="sm" className="text-xs md:text-sm">
                    <Database className="size-4 mr-2" />
                    Download Sample Batch File
                  </Button>
                </div>
              </div>

              <div className="bg-blue-500/10 border border-blue-500/20 p-3 rounded-lg">
                <p className="text-xs md:text-sm text-muted-foreground">
                  <span className="font-medium text-blue-600 dark:text-blue-400">Tip:</span> Download the sample batch
                  file to see a complete example with multiple tables and complex queries.
                </p>
              </div>
            </TabsContent>

            <TabsContent value="tables" className="space-y-3 mt-0 mr-2">
              <div>
                <h3 className="font-semibold text-base md:text-lg mb-2">Creating Tables</h3>
                <p className="text-xs md:text-sm text-muted-foreground mb-3">
                  Use <code className="bg-muted px-1.5 py-0.5 rounded text-xs">make table</code> to create tables
                </p>
              </div>

              <div className="space-y-3">
                <div className="bg-muted/50 p-3 rounded-lg">
                  <div className="flex items-start justify-between mb-2">
                    <div className="flex-1 min-w-0">
                      <h4 className="font-medium text-xs md:text-sm">Student Table</h4>
                      <p className="text-xs text-muted-foreground">Create with multiple fields</p>
                    </div>
                    <Button
                      size="sm"
                      variant="ghost"
                      className="h-7 text-xs px-3 ml-2 flex-shrink-0"
                      onClick={() => handleTryQuery("make table student fields fname, lname, major, age")}
                    >
                      <Code2 className="size-3 mr-1" />
                      Try
                    </Button>
                  </div>
                  <pre className="bg-background p-2 rounded text-xs font-mono overflow-x-auto">
                    make table student fields fname, lname, major, age
                  </pre>
                </div>
              </div>

              <div className="bg-blue-500/10 border border-blue-500/20 p-3 rounded-lg">
                <p className="text-xs md:text-sm text-muted-foreground">
                  <span className="font-medium text-blue-600 dark:text-blue-400">Note:</span> Field names are
                  comma-separated. No data types needed.
                </p>
              </div>
            </TabsContent>

            <TabsContent value="insert" className="space-y-3 mt-0 mr-2">
              <div>
                <h3 className="font-semibold text-base md:text-lg mb-2">Inserting Data</h3>
                <p className="text-xs md:text-sm text-muted-foreground mb-3">
                  Use <code className="bg-muted px-1.5 py-0.5 rounded text-xs">insert into</code> to add rows
                </p>
              </div>

              <div className="space-y-3">
                <div className="bg-muted/50 p-3 rounded-lg">
                  <div className="flex items-start justify-between mb-2">
                    <div className="flex-1 min-w-0">
                      <h4 className="font-medium text-xs md:text-sm">With Quotes</h4>
                      <p className="text-xs text-muted-foreground">For values with spaces</p>
                    </div>
                    <Button
                      size="sm"
                      variant="ghost"
                      className="h-7 text-xs px-3 ml-2 flex-shrink-0"
                      onClick={() => handleTryQuery('insert into student values "Flo", "Yao", "CS", 20')}
                    >
                      <Code2 className="size-3 mr-1" />
                      Try
                    </Button>
                  </div>
                  <pre className="bg-background p-2 rounded text-xs font-mono overflow-x-auto">
                    insert into student values "Flo", "Yao", "CS", 20
                  </pre>
                </div>

                <div className="bg-muted/50 p-3 rounded-lg">
                  <div className="flex items-start justify-between mb-2">
                    <div className="flex-1 min-w-0">
                      <h4 className="font-medium text-xs md:text-sm">Without Quotes</h4>
                      <p className="text-xs text-muted-foreground">Single-word values</p>
                    </div>
                    <Button
                      size="sm"
                      variant="ghost"
                      className="h-7 text-xs px-3 ml-2 flex-shrink-0"
                      onClick={() => handleTryQuery("insert into student values Bo, Yang, CS, 28")}
                    >
                      <Code2 className="size-3 mr-1" />
                      Try
                    </Button>
                  </div>
                  <pre className="bg-background p-2 rounded text-xs font-mono overflow-x-auto">
                    insert into student values Bo, Yang, CS, 28
                  </pre>
                </div>
              </div>

              <div className="bg-blue-500/10 border border-blue-500/20 p-3 rounded-lg">
                <p className="text-xs md:text-sm text-muted-foreground">
                  <span className="font-medium text-blue-600 dark:text-blue-400">Note:</span> Values must match field
                  order. Use quotes for spaces/special chars.
                </p>
              </div>
            </TabsContent>

            <TabsContent value="select" className="space-y-3 mt-0 mr-2">
              <div>
                <h3 className="font-semibold text-base md:text-lg mb-2">Querying Data</h3>
                <p className="text-xs md:text-sm text-muted-foreground mb-3">
                  Use <code className="bg-muted px-1.5 py-0.5 rounded text-xs">select</code> to retrieve data
                </p>
              </div>

              <div className="space-y-3">
                <div className="bg-muted/50 p-3 rounded-lg">
                  <div className="flex items-start justify-between mb-2">
                    <div className="flex-1 min-w-0">
                      <h4 className="font-medium text-xs md:text-sm">Select All</h4>
                      <p className="text-xs text-muted-foreground">Get all records</p>
                    </div>
                    <Button
                      size="sm"
                      variant="ghost"
                      className="h-7 text-xs px-3 ml-2 flex-shrink-0"
                      onClick={() => handleTryQuery("select * from student")}
                    >
                      <Code2 className="size-3 mr-1" />
                      Try
                    </Button>
                  </div>
                  <pre className="bg-background p-2 rounded text-xs font-mono overflow-x-auto">
                    select * from student
                  </pre>
                </div>

                <div className="bg-muted/50 p-3 rounded-lg">
                  <div className="flex items-start justify-between mb-2">
                    <div className="flex-1 min-w-0">
                      <h4 className="font-medium text-xs md:text-sm">Where Clause</h4>
                      <p className="text-xs text-muted-foreground">Filter with conditions</p>
                    </div>
                    <Button
                      size="sm"
                      variant="ghost"
                      className="h-7 text-xs px-3 ml-2 flex-shrink-0"
                      onClick={() => handleTryQuery('select * from student where major = "CS"')}
                    >
                      <Code2 className="size-3 mr-1" />
                      Try
                    </Button>
                  </div>
                  <pre className="bg-background p-2 rounded text-xs font-mono overflow-x-auto">
                    select * from student where major = "CS"
                  </pre>
                </div>

                <div className="bg-muted/50 p-3 rounded-lg">
                  <div className="flex items-start justify-between mb-2">
                    <div className="flex-1 min-w-0">
                      <h4 className="font-medium text-xs md:text-sm">Multiple Conditions</h4>
                      <p className="text-xs text-muted-foreground">Use AND/OR operators</p>
                    </div>
                    <Button
                      size="sm"
                      variant="ghost"
                      className="h-7 text-xs px-3 ml-2 flex-shrink-0"
                      onClick={() => handleTryQuery('select * from student where major = "CS" and age < 25')}
                    >
                      <Code2 className="size-3 mr-1" />
                      Try
                    </Button>
                  </div>
                  <pre className="bg-background p-2 rounded text-xs font-mono overflow-x-auto">
                    select * from student where major = "CS" and age {"<"} 25
                  </pre>
                </div>
              </div>

              <div className="bg-blue-500/10 border border-blue-500/20 p-3 rounded-lg">
                <p className="text-xs md:text-sm text-muted-foreground">
                  <span className="font-medium text-blue-600 dark:text-blue-400">Operators:</span> = {">"} {"<"} {">="}{" "}
                  {"<="} and, or
                </p>
              </div>
            </TabsContent>
          </div>
        </Tabs>
      </DialogContent>
    </Dialog>
  )
}
