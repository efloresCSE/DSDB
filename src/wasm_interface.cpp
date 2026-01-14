// WebAssembly interface for TXT2DB
// Exposes C++ functions to JavaScript

#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "sql.h"
#include "table.h"
#include <sstream>
#include <string>
#include <vector>

using namespace emscripten;
using namespace std;

// Global SQL instance for the web interface
SQL* globalSQL = nullptr;

// Initialize the SQL engine
string initDatabase() {
    try {
        if (globalSQL != nullptr) {
            delete globalSQL;
        }
        globalSQL = new SQL();
        return "Database initialized successfully";
    } catch (const exception& e) {
        return string("Error initializing database: ") + e.what();
    }
}

// Execute a single SQL command and return the result as JSON-like string
string executeCommand(string command) {
    if (globalSQL == nullptr) {
        return "{\"error\": \"Database not initialized. Call initDatabase() first.\"}";
    }
    
    try {
        // Parse the command
        Parser p(const_cast<char*>(command.c_str()));
        MMap<string, string> ptree = p.get_parse_tree();
        
        // Check if parse was successful
        if (ptree.empty()) {
            return "{\"error\": \"Invalid SQL syntax\"}";
        }
        
        ostringstream result;
        result << "{";
        
        // Handle CREATE/MAKE TABLE
        if (ptree["command"][0] == "create" || ptree["command"][0] == "make") {
            Table t(ptree["table_name"][0], ptree["fields"]);
            result << "\"type\": \"create\", ";
            result << "\"table\": \"" << ptree["table_name"][0] << "\", ";
            result << "\"message\": \"Table created successfully\"";
        }
        // Handle INSERT
        else if (ptree["command"][0] == "insert") {
            Table t(ptree["table_name"][0]);
            t.insert(ptree["values"]);
            result << "\"type\": \"insert\", ";
            result << "\"table\": \"" << ptree["table_name"][0] << "\", ";
            result << "\"message\": \"Record inserted successfully\"";
        }
        // Handle SELECT
        else if (ptree["command"][0] == "select") {
            Table t(ptree["table_name"][0]);
            Table resultTable = ptree["values"].empty() ? 
                t.select_all() : 
                t.select_all(p.shuntingYard());
            
            // Capture table output
            ostringstream tableOutput;
            resultTable.print_table(tableOutput);
            string tableStr = tableOutput.str();
            
            // Parse table name and record count from output
            size_t namePos = tableStr.find("Table name: ");
            size_t recordsPos = tableStr.find("records: ");
            size_t newlinePos = tableStr.find('\n');
            
            string tableName = resultTable.getName();
            
            // Escape quotes in table string for JSON
            size_t pos = 0;
            while ((pos = tableStr.find('\"', pos)) != string::npos) {
                tableStr.replace(pos, 1, "\\\"");
                pos += 2;
            }
            
            result << "\"type\": \"select\", ";
            result << "\"table\": \"" << tableName << "\", ";
            result << "\"output\": \"" << tableStr << "\"";
            
            resultTable.clean_up();
        }
        else {
            result << "\"error\": \"Unknown command type\"";
        }
        
        result << "}";
        return result.str();
        
    } catch (const error& e) {
        return string("{\"error\": \"") + e.what() + "\"}";
    } catch (const exception& e) {
        return string("{\"error\": \"") + e.what() + "\"}";
    } catch (...) {
        return "{\"error\": \"Unknown error occurred\"}";
    }
}

// Get list of all tables (reads from file system)
string listTables() {
    // This would need to scan the virtual file system for .bin files
    // For now, return a simple message
    return "{\"message\": \"Table listing not yet implemented\"}";
}

// Cleanup function
void cleanup() {
    if (globalSQL != nullptr) {
        delete globalSQL;
        globalSQL = nullptr;
    }
}

// Bind C++ functions to JavaScript
EMSCRIPTEN_BINDINGS(txt2db_module) {
    emscripten::function("initDatabase", &initDatabase);
    emscripten::function("executeCommand", &executeCommand);
    emscripten::function("listTables", &listTables);
    emscripten::function("cleanup", &cleanup);
}
