#include "tokenizer.h"

Tokenizer::Tokenizer()
{
    //Does nothing
}

Tokenizer::Tokenizer(vector<string> conditions)
{
    _buffer = conditions;
}

//private member functions
void Tokenizer::set_condition(vector<string> conditions)
{
    _buffer = conditions;
}

queue<r_Token*> Tokenizer::convert()
{
    const bool DEBUG = false;

    if (DEBUG)
    {
        cout << " ----- TOKENIZER: Convert -----" << endl;
    }

    //Goes through vector of string (infix) and converts it into tokens
    for (size_t i = 0; i < _buffer.size(); i++)
    {
        if (_buffer[i] == " ")
        {
            //Ignore b/c space
        }
        else
        {
            //Token constructor automatically determines the token type
            tokenized.push(new r_Token(_buffer[i]));
        }
    }

    if (DEBUG)
    {
        cout << " Tokenized Queue: " << tokenized << endl;
    }
    return tokenized;
}
