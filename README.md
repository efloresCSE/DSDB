# DSDB - Text to Database Web Interface

A web-based SQL-like database interface that runs entirely in your browser using WebAssembly. Provides a WEBUI for my TXT2DB Project: https://github.com/efloresCSE/TXT2DB.

## Features

- **SQL Query Editor** with syntax highlighting
- **Interactive Results Table** with sortable columns
- **Schema Browser** to view table structures
- **Example Queries** to get started quickly
- **Error Handling** with helpful messages
- **100% Client-Side** - runs in browser via WebAssembly

## GitHub Pages Deployment

This project is configured to automatically deploy to GitHub Pages via GitHub Actions.

## SQL Syntax

### Create Table
```sql
make table student fields fname, lname, major, age
```

### Insert Data
```sql
insert into student values Flo, Yao, CS, 20
```

### Select Data
```sql
select * from student
select * from student where major = CS
select * from student where age > 20 and major = CS
```

## License

MIT License - See LICENSE file for details
