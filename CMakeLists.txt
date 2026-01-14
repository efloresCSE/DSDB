cmake_minimum_required(VERSION 3.10)
project(TXT2DB_WASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/wasm_interface.cpp
    src/sql.cpp
    src/parser.cpp
    src/table.cpp
    src/stokenizer.cpp
    src/ftokenizer.cpp
)

# Emscripten flags
set(EMSCRIPTEN_FLAGS
    -sWASM=1
    -sEXPORT_ES6=1
    -sMODULARIZE=1
    -sEXPORT_NAME=createTXT2DBModule
    -sALLOW_MEMORY_GROWTH=1
    -sINITIAL_MEMORY=16MB
    -sMAXIMUM_MEMORY=512MB
    -sFILESYSTEM=1
    -sNO_DISABLE_EXCEPTION_CATCHING
    -sEXPORTED_RUNTIME_METHODS=['FS']
    -lembind
    --bind
)

# Build target
add_executable(txt2db ${SOURCES})

# Apply Emscripten flags
target_compile_options(txt2db PRIVATE ${EMSCRIPTEN_FLAGS})
target_link_options(txt2db PRIVATE ${EMSCRIPTEN_FLAGS})

# Set output names
set_target_properties(txt2db PROPERTIES
    OUTPUT_NAME "txt2db"
    SUFFIX ".js"
)
