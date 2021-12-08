#include "sql.h"

int SQL::query_num = 1;

//Right now if user inputs a fieldname (case sensitive) thats not there...
// need to patch ^
//Default CTOR
SQL::SQL() //Needs to open existing tables
{
    //Name of file that holds table names
    _table_file = "Existing_Tables.tbl";

    //Reads from file that stares existing table names
    open_existing();
    //Initializes the map of table objects
    initialize_map();
}

//Constructor for batch file
SQL::SQL(char batch_file_name[])
{

    //Name of file that holds table names
    _table_file = "Existing_Tables.tbl";

    //Does not need this b/c not interactive
    /*
    //Reads from file that stares existing table names
    open_existing();
    //Initializes the map of table objects
    initialize_map();
    */

    batch_file = batch_file_name;
}

//Runs interactive SQL DATABASE
void SQL::run()
{
    //Prints table names of existing tables
    print_existing_table();
    //Max length of user input
    const size_t MAX_LENGTH = 200;
    //Declaration of a c-string
    char command[MAX_LENGTH];

    //Take in user input
    cout << ": ";
    cin.getline(command, MAX_LENGTH, '\n');

    //Program ends when "exit" is inputted
    while (strcmp(command, "exit") != 0)
    {
        //Don't need to print query in interactive
        //cout << " Query <" << query_num << "> : " << command << endl;
        //Runs SQL command based on given user input
        if (run_command(command))
        {
            cout << endl << "-SQL DONE-" << endl;
        }
        else
        {
            cout << "ERROR: invalid input" << endl;
        }
        //Continues looping
        cout << ": ";
        cin.getline(command, MAX_LENGTH, '\n');
    }

}

void SQL::run_batch()
{
    //Fstream object
    fstream f;

    //Opens batch file
    open_batch(f);

    //Prints table names of existing tables
    print_existing_table();

    //Max length of buffer
    const size_t MAX_LENGTH = 200;
    //Declaration of a c-string
    char command[MAX_LENGTH];

    //Get command from batch file
    f.getline(command, MAX_LENGTH, '\n');

    //While not  the end of file
    while (!f.eof())
    {
        //Checks if there are any backslash characters
        char* note;
        note = strchr(command,'/');

        //Backslash character not found so command is not a note
        if (note == nullptr)
        {
            //Prints query
            cout << " Query <" << query_num << "> : " << command << endl;
            if (run_command(command))
            {
                cout << endl << "-SQL DONE-" << endl;
            }
            else if (strcmp(command, "\n"))
            {
                //Ignore empty lines (endl)
            }
            else
            {
                cout << "ERROR: invalid input" << endl;
            }
        }
        else
        {
            cout << command << endl;
        }
        f.getline(command, MAX_LENGTH, '\n');
    }
}

//Process the command
bool SQL::run_command(char* command)
{
    query_num++;
    const bool DEBUG = false;

    //Updates parser
    parser.set_string(command);

    //Command keyword
    string cmd;
    //If user input is ptree valid
    if (_get_SQL_Command(cmd))
    {
        if (DEBUG)
        {
            cout << "---Parser---" << endl;
            ptree.print_map();
            cout << "------------" << endl;
            cout << "Command: " << ptree["Command"] << endl;
            //cout << "CMD: " << cmd << endl;
        }
        //If the table exists and the table is not empty
        if (tables.contains(ptree["Table"][0]))
        {
            if (cmd == "select")
            {
                //Checks if table is empty
                if (!tables[ptree["Table"][0]].empty())
                {
                    //Passes Table name into select function
                    //Calls appropriate select on table
                    _select(ptree["Table"][0]);
                }
                else
                {
                    cout << "-ERROR: Empty Table-" << endl;
                }

            }
            else if (cmd == "insert")
            {
                //Insert fields into the table
                _insert(ptree["Table"][0]);
            }
            //Create/Make new table
            else if (cmd == "create" || cmd == "make")
            {
                //This would be a good time to implement the error stuff you
                //taught us in CS3A but I forgot how to utilize those 3
                //functions
                cout << "-ERROR: Table already exists-" << endl;
                //Trying to create a table that already exists is a waste of
                //memory
            }
            else if (cmd == "drop")
            {
                _drop_table(ptree["Table"][0]);
            }
        }
        //If the command is create
        else if ((cmd == "create" || cmd == "make") &&
                 !tables.contains(ptree["Table"][0]))
        {
            _create_Table(ptree["Table"][0]);
        }
        else
        {
            cout << "-ERROR: INVALID TABLE-" << endl;
        }
    }
    else //Returns false if parser fails to get ptree
    {
        return false;
    }
    return true;
}

void SQL::print_existing_table()
{
    cout << endl << "---------- Table Names ----------" << endl;
    _table_names.print_list(cout);
    cout << "---------------------------------" << endl;
}

//Private member select
void SQL::_select(string table_name)
{
    const bool DEBUG = false;
    if (DEBUG)
    {
        cout << "--SQL select--" << endl;
        ptree.print_map();
        cout << "---------" << endl;
    }
    //If command contains a field and a condition
    if (parser.contains_condition() && ptree["Fields"][0] != "*")
    {
        //Then utilize the select function (process both fieldnames
            // and condition
        //cout << "Select" << endl;
        cout << tables[table_name].select(ptree["Fields"], ptree["Condition"])
             << endl;
    }
    //If it contains a condition but selecting all fields
    else if (parser.contains_condition())
    {
        //This condition means that the field is *
        assert(ptree["Fields"][0] == "*" && "Wrong select being called");
        //cout << "Select conditional " << endl;
        cout << tables[table_name].select_conditional(ptree["Condition"])
             << endl;
    }
    //If there are no conditions
    else if (ptree["Fields"][0] == "*")
    {
        assert(ptree["Condition"].size() == 0 && "Wrong select being called");
        //cout << "Select all" << endl;
        cout << tables[table_name].select_all() << endl;
    }
    else //If there are no conditions and thera are specified fields
    {
        assert(ptree["Condition"].size() == 0 && "Wrong select being called");
        assert(ptree["Fields"][0] != "*" && "Wrong select being called");
        //cout << "Select Fields" << endl;
        cout << tables[table_name].select_fields(ptree["Fields"]) << endl;
    }
}

void SQL::_create_Table(string table_name)
{
    //Should this be dynamic?
    Table new_Table(table_name, ptree["Fields"]);
    //Inserts table into the map of tables
    tables.insert(table_name, new_Table);
    //Updates table_names
    _table_names.insert(table_name);
    //Updates file of table names
    update_existing();
}


void SQL::_drop_table(string table_name)
{
    assert(tables.contains(table_name) && "Table not found!" );
    //Erases table object
    tables.erase(table_name);
    //Erases table name from B+Tree of table names
    _table_names.remove(table_name);
    //Updates existing files
    update_existing();
    //Why are we using a vector to hold table names...
}

//Inserts fields into table my using table's insert into function
void SQL::_insert(string table_name)
{
    assert (parser.success() == true);
    tables[table_name].insert_into(ptree["Fields"]);
}


//Returns command through passed-by-ref parameter command
bool SQL::_get_SQL_Command(string& command)
{
    //Sets fail flag
    bool isValid = false;

    //If PTree object is valid
    if (parser.success())
    {
        //Sets multimap ptree
        set_Ptree();
        //Sets flag
        isValid = true;
        //Updates command to command in PTree
        command = ptree["Command"][0];
    }

    return isValid;
}

//Updates ptree
//Assumes parser is valid
void SQL::set_Ptree()
{
    assert(parser.success() && "Attempting to obtain ptree when PARSER is in "
                               "INVALID STATE");
    ptree = parser.PTree();
}

//Opens file of existing table names and pushes it into B+Tree of table names
void SQL::open_existing()
{
    const bool DEBUG = false;

    if (DEBUG)
    {
        cout << "--SQL open existing file-- " << endl;
    }

    //Fstream object
    fstream file;
    //Opens table
    file.open(_table_file);

    if (!file)
    {
        cout << "ERROR: Unable to open file" << endl;
        exit(1);   // call system to stop
    }
    string table_name;
    //Initializes B+tree of table names
    while (file >> table_name)
    {
        _table_names.insert(table_name);
        if (DEBUG)
        {
            cout << "Inserted: " << table_name << endl;
        }
    }

    if (DEBUG)
    {
        cout << "number of tables: " << _table_names.size() << endl;
        cout << "Table Names: " << endl;
        _table_names.print_list(cout);
    }

}

//Updates existing table names in file with table names present in SQL obj
void SQL::update_existing()
{
    ofstream outFile;
    outFile.open(_table_file);

    if (!outFile)
    {
        cout << "ERROR: Unable to open file" << endl;
        exit(1);   // call system to stop
    }

    BPlusTree<string>::Iterator it = _table_names.begin();

    //Overwrites file with existing table names
    while (it != BPlusTree<string>::Iterator(nullptr))
    {
        outFile << *it << endl;
        it++;
    }

}

//Initializes map of table objects - called after table names are initalized
void SQL::initialize_map()
{
    BPlusTree<string>::Iterator it = _table_names.begin();

    while (it != BPlusTree<string>::Iterator(nullptr))
    {

        string name = *it;

        tables[name] = Table(name);

        //Creates new table object with name in table names b+tree
        /*
        Table* existing = new Table(*it);
        //Insert new table object into the map
        tables.insert(*it, *existing);
        */
        it++;

    }
}

//Opens batch file
void SQL::open_batch(fstream& f)
{

    f.open(batch_file);

    if (!f)
    {
        cout << "ERROR: Unable to open file" << endl;
        exit(1);   // call system to stop
    }
}
