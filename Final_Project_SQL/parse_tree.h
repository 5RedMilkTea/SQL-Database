#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "../../B Tree/bplus_multimap/mmap.h"
#include "../../B Tree/bplusmap/map.h"
#include "debug_format.h"
#include "state_machine_config.h"
#include "../../Reusables/StringTokenizer/String_Tokenizer/stokenizer.h"
#include "../../Reusables/IteratedQueue/queue/queue.h"

class parse_tree
{
public:
    parse_tree();
    parse_tree(char* s);

    //Returns _PTree
    MMap<string, string> PTree();

    //Allows buffer to set a new string and updates PTree
    bool set_string(char* s);

    //Returns success boolean
    bool success();

    //Returns if there is a condition or not
    bool contains_condition();


private:
    //Enumerated commands
    enum commands {SELECT = 1, FROM = 2, STAR = 3, SYMBOL = 4, WHERE = 5,
                   CREATE = 6, DROP = 7, INSERT = 8, AND = 9, OR = 10,
                   DELETE = 11, COMMA = 12, VALUES = 13, INTO = 14,
                   LPARENTESIS = 15, RPARENTHESIS = 16, MAKE = 17, TABLE = 18,
                   FIELDS = 19, LAST};

    char _buffer[SQL_MAX_BUFFER];

    //String tokenizer
    STokenizer _tokenizer;

    //Queue of tokens
    queue<Token> _inputq;

    //Map of keywords
      //Keys are the keywords
      //Values are the associated number in the adjacency matrix
    Map<string, int> _keywords;

    //Maps keyword to associated constant in the keywords map
    int get_column(Token keyword);

    //Adjacency Table - keywords/commands machine
    static int _adj_table[SQL_MAX_ROWS][SQL_MAX_COLUMNS];

    //Fail flag
    bool _success;

    bool _condition;

    //Resets the PTree multimap and "Where" (condition bool) to false;
    void reset();

    bool get_parse_tree();
    //Table of inputed commands
      //Keys are Command, Table, Fields, Where, Condition
      //Value list are what the user inputs
    MMap<string, string> _PTree;

    //Initializes adjacency matrix
    void build_matrix(int _table[][SQL_MAX_COLUMNS]);
    //Initializes _keywords map
    void build_keywords_list();

    //Removes quotation from a string
    string remove_quotations(const string &tokenstr);



};

#endif // PARSE_TREE_H
