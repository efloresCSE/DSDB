#include "sql.h"

#include <cstring> // std::memcpy
#include <vector>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <ctime>

// -----------------------------------------------------------------------------
// Session + output directory helpers (no header changes required)
// -----------------------------------------------------------------------------
namespace fs = std::filesystem;

static fs::path g_project_root;
static fs::path g_session_dir;
static bool g_session_ready = false;

static fs::path find_project_root()
{
    fs::path p = fs::current_path();

    // Walk up until we find a folder that contains "src"
    for (int i = 0; i < 12; ++i)
    {
        if (fs::exists(p / "src") && fs::is_directory(p / "src"))
            return p;

        if (!p.has_parent_path())
            break;
        fs::path parent = p.parent_path();
        if (parent == p)
            break;
        p = parent;
    }

    // Fallback: current working directory
    return fs::current_path();
}

static std::string make_timestamp()
{
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d_%I-%M%p");
    return oss.str();
}

static std::string sanitize_for_folder(std::string s)
{
    for (size_t i = 0; i < s.size(); ++i)
    {
        char &c = s[i];
        if (!(std::isalnum((unsigned char)c) || c == '-' || c == '_'))
            c = '_';
    }
    return s;
}

static void ensure_session_dir()
{
    if (g_session_ready)
        return;

    g_project_root = find_project_root();

    fs::path outputs_root = g_project_root / "outputs";
    fs::create_directories(outputs_root);

    g_session_dir = outputs_root / ("session_" + make_timestamp());
    fs::create_directories(g_session_dir);

    // Make all runtime-generated DB artifacts land in the session folder
    fs::current_path(g_session_dir);

    g_session_ready = true;

    std::cout << "Session output folder: " << g_session_dir.string() << std::endl;
}

// Resolve batch input path even though we changed cwd into outputs/session_...
static fs::path resolve_input_path(std::string filename)
{
    fs::path p(filename);

    if (p.is_absolute())
        return p;

    // Try: relative to project root
    fs::path a = g_project_root / p;
    if (fs::exists(a))
        return a;

    // Try: relative to project root/examples
    fs::path b = g_project_root / "examples" / p;
    if (fs::exists(b))
        return b;

    // Try: relative to project root/src (just in case)
    fs::path c = g_project_root / "src" / p;
    if (fs::exists(c))
        return c;

    // Try current path (session dir) as last resort (unlikely)
    fs::path d = fs::current_path() / p;
    if (fs::exists(d))
        return d;

    return fs::path(); // empty = not found
}

// -----------------------------------------------------------------------------
// SQL implementation
// -----------------------------------------------------------------------------

// set number of commands for session equal to zero
SQL::SQL()
{
    commNum = 0;
}

void SQL::run()
{
    ensure_session_dir();

    vector<string> RPN;
    [[maybe_unused]] bool debug = false;

    while (1)
    {
        try
        {
            // get a command
            string line;
            cout << "Command: ";
            getline(cin, line);
            fflush(stdin);

            // exit if line == exit
            if (line == "exit")
            {
                cout << "THANK YOU!" << endl;
                exit(0);
            }

            // FIX: safe null-terminated buffer for Parser(char*)
            std::vector<char> command(line.size() + 1);
            std::memcpy(command.data(), line.c_str(), line.size() + 1);

            // parse the command, and get ptree
            Parser temp(command.data());
            ptree = temp.get_parse_tree();

            // do shunting yard if select->values
            if (ptree["command"][0] == "select" &&
                !ptree["values"].empty())
            {
                RPN = temp.shuntingYard();
            }

            // Creating table
            if (ptree["command"][0] == "create" || ptree["command"][0] == "make")
            {
                Table t(ptree["table_name"][0], ptree["fields"]);
                display_create(line);
                commNum++;
            }

            // inserting into table
            else if (ptree["command"][0] == "insert")
            {
                Table t(ptree["table_name"][0]);
                t.insert(ptree["values"]);
                display_insert(line);
                commNum++;
            }

            // selecting records from table
            else if (ptree["command"][0] == "select")
            {
                Table t(ptree["table_name"][0]);
                if (ptree["fields"][0] == "*")
                {
                    if (!ptree["values"].empty())
                    {
                        Table tempT = t.select_all(RPN);
                        display_select_all(line, tempT);
                        tempT.clean_up();
                    }
                    else
                    {
                        Table tempT = t.select_all();
                        display_select_all(line, tempT);
                        tempT.clean_up();
                    }
                    commNum++;
                }
            }

            // run a batch file
            else if (ptree["command"][0] == "batch")
            {
                run_batch(ptree["file_name"][0]);
            }
        }
        catch (exception &e)
        {
            cout << e.what() << endl
                 << endl;
        }
        catch (...)
        {
            cout << endl
                 << "An unknown error has occured." << endl
                 << endl;
        }
    }
}

void SQL::run_batch(string filename)
{
    ensure_session_dir();

    [[maybe_unused]] bool debug = false;
    fstream f;
    fstream g;
    string line = "";
    vector<string> RPN;

    // Allow "FinalTest" or "FinalTest.txt"
    std::string original = filename;
    if (filename.find('.') > filename.size())
        filename += ".txt";

    // Find the batch input file from project root, even though cwd is session folder
    fs::path input_path = resolve_input_path(filename);
    if (input_path.empty())
    {
        std::cout << "Batch file not found: " << filename << std::endl;
        throw error("Batch file not found");
    }

    // Create a per-batch folder inside the session directory
    std::string base = fs::path(original).stem().string();
    std::string batch_folder = "batch_" + make_timestamp() + "_" + sanitize_for_folder(base);
    fs::path batch_dir = g_session_dir / batch_folder;
    fs::create_directories(batch_dir);

    // Temporarily run the batch inside its folder so all artifacts land there
    fs::path prev_cwd = fs::current_path();
    fs::current_path(batch_dir);

    // Open batch input (READ)
    f.open(input_path.string().c_str(), std::fstream::in);
    if (f.fail())
    {
        fs::current_path(prev_cwd);
        throw error("file failed to open.");
    }

    // Output txt file inside the batch folder (NO subdirs in name)
    std::string out_name = fs::path(filename).stem().string() + "_output.txt";
    t_open_fileRW(g, out_name);

    while (getline(f, line))
    {
        try
        {
            // if our line does not start with an m, i, or s
            if (line.empty() || (line[0] != 'm' && line[0] != 'i' && line[0] != 's'))
            {
                cout << line << endl;
                g << line << endl;
                continue;
            }

            // safe null-terminated buffer for Parser(char*)
            std::vector<char> command(line.size() + 1);
            std::memcpy(command.data(), line.c_str(), line.size() + 1);

            Parser temp(command.data());
            ptree = temp.get_parse_tree();

            if (ptree["command"][0] == "select" &&
                !ptree["values"].empty())
            {
                RPN = temp.shuntingYard();
            }

            if (ptree["command"][0] == "create" || ptree["command"][0] == "make")
            {
                Table t(ptree["table_name"][0], ptree["fields"]);
                display_create(line);
                display_create(line, g);
                commNum++;
            }
            else if (ptree["command"][0] == "insert")
            {
                Table t(ptree["table_name"][0]);
                t.insert(ptree["values"]);
                display_insert(line);
                display_insert(line, g);
                commNum++;
            }
            else if (ptree["command"][0] == "select")
            {
                Table t(ptree["table_name"][0]);
                if (ptree["fields"][0] == "*")
                {
                    if (!ptree["values"].empty())
                    {
                        Table tempT = t.select_all(RPN);
                        display_select_all(line, tempT);
                        display_select_all(line, tempT, g);
                        tempT.clean_up();
                    }
                    else
                    {
                        Table tempT = t.select_all();
                        display_select_all(line, tempT);
                        display_select_all(line, tempT, g);
                        tempT.clean_up();
                    }
                    commNum++;
                }
            }
            else if (ptree["command"][0] == "batch")
            {
                // Nested batch: will create another folder inside session
                run_batch(ptree["file_name"][0]);
            }
        }
        catch (exception &e)
        {
            cout << e.what() << endl
                 << endl;
            g << e.what() << endl
              << endl;
        }
        catch (...)
        {
            cout << endl
                 << "An unknown error has occured." << endl
                 << endl;
            g << endl
              << "An unknown error has occured." << endl
              << endl;
        }
    }

    cout << "---------------------------" << endl;
    cout << "End of Batch Process" << endl;
    g << "---------------------------" << endl;
    g << "End of Batch Process" << endl;

    f.close();
    g.close();

    // Restore previous working directory (session dir)
    fs::current_path(prev_cwd);

    cout << "Batch outputs saved to: " << batch_dir.string() << endl;
}

// displays a message after create
void SQL::display_create(string command, ostream &outs)
{
    outs << "[" << commNum << "] ";
    outs << command << endl;
    outs << "Table Created: " << ptree["table_name"][0] << endl
         << endl
         << endl;

    outs << "SQL: DONE." << endl
         << endl;
}

// displays a message after insert
void SQL::display_insert(string command, ostream &outs)
{
    outs << "[" << commNum << "] ";
    outs << command << endl;
    outs << "SQL::run: inserted into table: " << ptree["table_name"][0] << endl
         << endl
         << endl;

    outs << "SQL: DONE." << endl
         << endl;
}

// displays a message after select all
void SQL::display_select_all(string command, Table t, ostream &outs)
{
    outs << "[" << commNum << "] ";
    outs << command << endl
         << endl;

    outs << t << endl
         << endl;
    outs << "SQL: DONE." << endl
         << endl;
}

// checks if a text file exists
bool SQL::t_file_exists(string file_name)
{
    const bool debug = false;
    fstream ff;
    ff.open(file_name, std::fstream::in);
    if (ff.fail())
    {
        if (debug)
            cout << "file_exists(): File does NOT exist: " << file_name << endl;
        return false;
    }
    if (debug)
        cout << "file_exists(): File DOES exist: " << file_name << endl;
    ff.close();
    return true;
}

// Post: opens a text file for reading and writing
void SQL::t_open_fileRW(fstream &f, const string file_name)
{
    const bool debug = false;
    if (!t_file_exists(file_name))
    {
        f.open(file_name, std::fstream::out);
        if (f.fail())
        {
            cout << "file open (RW) failed: with out|" << file_name << "]" << endl;
            throw("file RW failed  ");
        }
        else
        {
            if (debug)
                cout << "open_fileRW: file created successfully: " << file_name << endl;
        }
    }
    else
    {
        // opens for input and output
        f.open(file_name, std::fstream::in | std::fstream::out);
        if (f.fail())
        {
            cout << "file open (RW) failed. [" << file_name << "]" << endl;
            throw("file failed to open.");
        }
    }
}
