#include "parse_tree.h"

int parse_tree::_adj_table[SQL_MAX_ROWS][SQL_MAX_COLUMNS];

parse_tree::parse_tree()
{
    //Set fail flag
    _success = false;
    //Build adj matrix
    build_matrix(_adj_table);
    //Build map of keywords
    build_keywords_list();
}

parse_tree::parse_tree(char* s)
{
    //Copies string into buffer
    strcpy(_buffer, s);

    //Builds adjacency matrix
    build_matrix(_adj_table);
    //Builds keywords map
    build_keywords_list();
    //Tokenizes string and sets tokens in queue and calls get_parse tree
    _success = set_string(s);

}

//Builds command interpreter machine
void parse_tree::build_matrix(int _table[][SQL_MAX_COLUMNS])
{
    markCells(_table);
}

//Builds the keywords
void parse_tree::build_keywords_list()
{
    //Debug
    const bool DEBUG = false;
    if (DEBUG)
    {
        cout << "--BUILDING KEYWORDS--" << endl;
    }

    //Array of keywords
    string keywords[21] = {"not_used", "select", "from", "*", "symbol",
                          "where", "create", "drop", "insert", "and", "or",
                           "delete", ",", "values", "into", "(", ")", "make"
                          ,"table", "fields"};

    //Inserting keywords into map and setting their values to the enumerated
     //value
    for (size_t i = SELECT; i != LAST; i++) //enumerator initialized in .h
    {
        //Insert into map
        _keywords.insert(keywords[i], i);

        if (DEBUG)
        {
            cout << "Key Word: " << spaced_left << keywords[i]
                 << " inserted into map with value " << i << endl;
        }
    }

    if (DEBUG)
    {
        cout << "Keywords Map: " << endl;
        cout << _keywords << endl;
    }
}

//initializees the input queue by tokenizing string into tokens
bool parse_tree::set_string(char* s)
{
    const bool DEBUG = false;

    if (DEBUG)
    {
        cout << "--Set String--" << endl;
    }

    //Reset fail flag
    _success = true;
    //Update String Tokenizer buffer
    _tokenizer.set_string(s);

    //Tokenize string (user input) into inputqueue
    Token t;
    //Extract token from buffer into storing token
    _tokenizer >> t; //This does the tokenizing
                    //t is holding the token that just got tokenized

    //Push tokens into input queue (Sets string)
    while (!_tokenizer.done())
    {
        //DEBUG - prints out the token and token type
        if (DEBUG)
        {
            cout << "| " << setw(12) << left << t.type_string() << "|";
            cout << setw(10) << right << "|" << t << "|" << endl;
        }

        //Pushes token into queue
        _inputq.push(t);

        //Extract token from buffer into storing token
        _tokenizer >> t;
    }

    if(DEBUG)
    {
        cout << "-Calling get_parse_tree-" << endl;
    }
    _success = get_parse_tree();

    //Return success/fail flag
    return _success;
}

//Builds the PTree multimap
bool parse_tree::get_parse_tree()
{
    const bool DEBUG = false;

    if (DEBUG)
    {
        cout << "---get parse tree---" << endl;
        cout << "_input q: " << _inputq << endl;
    }

    //Resets ParseTree Multimap and sets _condition to false (designates no
                // condition)
    reset();

    //Initial adjacent table state
    int state = 0;

    //While input queue is not empty and success state is not false
    while (!_inputq.isEmpty() && _success != false)
    {
        //Token = next token in queue
        Token t = _inputq.pop();

        if (DEBUG)
        {
            cout << "PROCESSING TOKEN: " << t << endl;
        }

        //Processes Token
            //Update adj table state
        //Checks if the lowercase string is a keyword, if not then its a symbol
        if (_keywords.contains(to_lower(t.token_str()))) //To_lower is
                                                        //kinda inefficient
        {

            if (DEBUG)
            {
                cout << "Keyword identified - matrix value: "
                     << _keywords[to_lower(t.token_str())] << endl;
                                    //to_lower Inefficient
                cout << "State: " << state << endl;
            }

                                    //Map enumerator value returned
            state = _adj_table[state][get_column(t)];
                                    //to_lower inefficient
        }
        //If token is a space (space are type 3)
        else if (t.type() == 3)
        {
            if (DEBUG)
            {
                cout << "Space - IGNORE" << endl;
            }
            //Intentionally empty
            //Does not process spaces
        }
        else //If token is not found in the keywords map, then its a symbol
        {
            //Debug
            if (DEBUG)
            {
                cout << "Keyword not identified - assuming SYMBOL " << endl;
                cout << "State: " << state << endl;
             }

            //Updates state
            state = _adj_table[state][SYMBOL];
        }

        //Breaks loop if state is in fail state
        if (state == -1)
        {
            //Set fail flag
            _success = false;
            //Reset Queue
            _inputq.clear();
            //Break loop
            break;
        }

        //DEBUG
        if (DEBUG)
        {
            cout << "Current State: " << state << endl;
        }

        //Can build a map to clean up this function
        //Update PTree
        //Command State - Token is a command
        //  1) Select
        // 20) Insert  21) into
        // 30) Make/Create
        // 40) Drop
        if (state == 1 || state == 20 || state == 21 || state == 30 ||
                state == 40)
        {
            //Pushes token into key: command in the multimap
            if (t.type() != 3)
            {
                _PTree["Command"] += t.token_str();
            }
        }
        //---Field--- Token is a field
        else if (state == 2 || state == 24 || state == 34)
        {
            //If not a space
            if (t.type() != 3)
            {
                _PTree["Fields"] += remove_quotations(t.token_str());
            }

        }
        //Token is "from"
        else if (state == 3)
        {
            //Do nothing
        }
        //---Table Name---
        else if (state == 4 || state == 22 || state == 32 || state == 42)
        {
            _PTree["Table"] += t.token_str();
        }
        //WHERE
        else if (state == 5) //Token is a WHERE keyword
        {
            //cout << "-----------WHERE------------------" << endl;
            if (t.type() != 3)
            {
                _PTree["Where"] += "True";
            }
            _condition = true;
        }
        //CONDITION
        else if (state == 6 || state == 7 || state == 8 || state == 10
                 || state == 11 || state == 12)
        {
            //If token is not a SPACE
            if (t.type() != 3)
            {
                //If it is a logical operator
                if (state == 10)
                {
                    //Make logical operator lowercase and push into conditions
                        //vector
                    _PTree["Condition"] += to_lower(t.token_str());
                }
                else
                {
                    //Symbols are case sensitive so leave as is
                    _PTree["Condition"] += remove_quotations(t.token_str());
                }
            }
        }
    }

    if (DEBUG)
    {
        cout << "STATE: " << state << endl;
        cout << "Success: " << _adj_table[state][0] << endl;
    }

    //Reached end of the queue
    if (_adj_table[state][0] == 1)
    {
        _success = true;
    }
    else //Not a success state maybe a neutral state (0)
    {
        //Reset PTree
        _PTree.clear();
        //Return false
        _success = false;
    }
    return _success;
}

//Goes into the map with the token keyword string (ex: select, from, or
    //create) and retrieves the mapped enumerator and returns that constant
int parse_tree::get_column(Token keyword)
{
    const bool DEBUG = false;
    if (DEBUG)
    {
        cout << "--get_column--: " << keyword.token_str() << " = "
             << _keywords[to_lower(keyword.token_str())] << endl;;
    }
    //Goes into the map with the token keyword string (ex: select, from, or
        //create) and retrieves the mapped enumerator and returns that constant
    return _keywords[to_lower(keyword.token_str())];
}

//Resets the _PTree multimap
void parse_tree::reset()
{
    _PTree.clear();
    _condition = false;
}

//Returns Parse Tree
MMap<string, string> parse_tree::PTree()
{
    return _PTree;
}

bool parse_tree::success()
{
    return _success;
}

bool parse_tree::contains_condition()
{
    return _condition;
}

string parse_tree::remove_quotations(const string& tokenstr)
{
    //String to return
    string no_quotes = tokenstr;
    //Index position in string of quote
    char quotation = no_quotes.find('"');

    //In a position of quotation is found
    while ( quotation != string::npos)
    {
        //Remove quotation
        no_quotes.erase(quotation, 1);
        //Check if theres more quotations
        quotation = no_quotes.find('"');
    }
    return no_quotes;
}
