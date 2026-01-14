# Makefile for building TXT2DB to WebAssembly
# Requires Emscripten SDK (emsdk)

# Compiler
CXX = em++

# Flags
CXXFLAGS = -std=c++17 -O3 -Isrc
LDFLAGS = -sWASM=1 \
          -sEXPORT_ES6=1 \
          -sMODULARIZE=1 \
          -sEXPORT_NAME=createTXT2DBModule \
          -sALLOW_MEMORY_GROWTH=1 \
          -sINITIAL_MEMORY=16MB \
          -sMAXIMUM_MEMORY=512MB \
          -sFILESYSTEM=1 \
          -sNO_DISABLE_EXCEPTION_CATCHING \
          -sEXPORTED_RUNTIME_METHODS=['FS'] \
          -lembind \
          --bind

# Source files
SOURCES = src/wasm_interface.cpp \
          src/sql.cpp \
          src/parser.cpp \
          src/table.cpp \
          src/stokenizer.cpp \
          src/ftokenizer.cpp

# Output
OUTPUT = public/txt2db.js

all: $(OUTPUT)

$(OUTPUT): $(SOURCES)
	@mkdir -p public
	$(CXX) $(CXXFLAGS) $(SOURCES) $(LDFLAGS) -o $(OUTPUT)
	@echo "Build complete! Output: $(OUTPUT) and $(OUTPUT:.js=.wasm)"

clean:
	rm -f public/txt2db.js public/txt2db.wasm

.PHONY: all clean
