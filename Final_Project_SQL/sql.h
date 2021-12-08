#ifndef SQL_H
#define SQL_H


#include "parse_tree.h"
#include "table.h"
#include "../../Reusables/FileTokenizer/file_Tokenizer/ftokenizer.h"

class SQL
{
public:
    //Need something to hold prev session data
    SQL();
    SQL(char batch_file_name[]); //Opens batch file

    //Puts string in parsetree .c_str
    void run(); //Calls run command in a loop
    //Runs batch file
    void run_batch();


private:
    static int query_num;

    //Private members

    //Map of table objects
    Map<string, Table> tables;
    //records table names
    BPlusTree<string> _table_names;

    //File that holds names of tables
    string _table_file;
    //Name of batch file
    string batch_file;

    //Parsers
    //PTree["Command"] returns the command
    parse_tree parser;
    //ptree
    MMap<string, string> ptree;

    //Runs commands
    bool run_command(char* command);

    //Prints existing table names
    void print_existing_table();


    //Table functions
    void _insert(string table_name);
    void _select(string table_name);
    void _create_Table(string table_name);
    void _drop_table(string table_name);

    //Gets command from PTree
    bool _get_SQL_Command(string& command);

    //Update ptree
    void set_Ptree();

    //Initializes Map of table objects
    void initialize_map();

    //Opens existing file of table names
    void open_existing();

    //Updates file of table names
    void update_existing();

    void open_batch(fstream& f);
};

#endif // SQL_H
