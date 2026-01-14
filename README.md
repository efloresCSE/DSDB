# TXT2DB - Text to Database Web Interface

A web-based SQL database interface that runs entirely in your browser using WebAssembly. Create tables, insert data, and query with SQL - no backend required!

## Features

- **SQL Query Editor** with syntax highlighting
- **Interactive Results Table** with sortable columns
- **Schema Browser** to view table structures
- **Example Queries** to get started quickly
- **Error Handling** with helpful messages
- **100% Client-Side** - runs in browser via WebAssembly

## Local Development

### Prerequisites

- Node.js 20+
- Emscripten SDK (for building WASM)

### Install Emscripten (First Time Only)

```bash
# Clone the emsdk repository
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk

# Install and activate the latest SDK
./emsdk install latest
./emsdk activate latest

# Set up environment variables
source ./emsdk_env.sh
```

### Build and Run

```bash
# Install dependencies
npm install

# Build the WASM module
make

# Run development server
npm run dev
```

Open [http://localhost:3000](http://localhost:3000) in your browser.

## GitHub Pages Deployment

This project is configured to automatically deploy to GitHub Pages via GitHub Actions.

### Setup Steps

1. **Enable GitHub Pages**:
   - Go to your repository Settings > Pages
   - Under "Build and deployment", select "GitHub Actions" as the source

2. **Push to main branch**:
   ```bash
   git add .
   git commit -m "Initial commit"
   git push origin main
   ```

3. **Monitor deployment**:
   - Go to the "Actions" tab in your repository
   - Watch the build and deploy workflow
   - Once complete, your site will be live at `https://<username>.github.io/<repo-name>/`

### Manual Deployment

To deploy manually:

```bash
npm run build
```

The static site will be generated in the `out/` directory.

## Project Structure

```
.
├── src/                    # C++ source code
│   ├── wasm_interface.cpp  # WASM bindings
│   ├── sql.cpp             # SQL engine
│   ├── parser.cpp          # SQL parser
│   └── table.cpp           # Table operations
├── app/                    # Next.js pages
│   └── page.tsx            # Main UI
├── components/             # React components
│   ├── sql-editor.tsx      # Query editor
│   ├── results-table.tsx   # Results display
│   └── schema-panel.tsx    # Schema browser
├── lib/                    # TypeScript utilities
│   └── wasm-loader.ts      # WASM initialization
├── public/                 # Static assets
│   ├── txt2db.js           # Generated WASM glue code
│   └── txt2db.wasm         # Compiled WASM module
├── Makefile                # WASM build configuration
└── .github/workflows/      # CI/CD configuration
    └── build-and-deploy.yml
```

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

## Technologies

- **Frontend**: Next.js 16, React 19, TypeScript
- **UI Components**: Radix UI, Tailwind CSS
- **Backend**: C++ compiled to WebAssembly via Emscripten
- **Deployment**: GitHub Actions + GitHub Pages

## License

MIT License - See LICENSE file for details
