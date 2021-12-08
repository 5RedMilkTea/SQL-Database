#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "../../Reusables/IteratedQueue/queue/queue.h"
#include "../../Reusables/VectorClass/VectorClass/vector.h"
#include "r_token.h"

//This class gets a vectorstr from "Commands" in PTree and tokenizes each
//element into a queue
class Tokenizer
{
public:
    //Default Constructor
    Tokenizer();
    //Constructor
    Tokenizer(vector<string> conditions);

    //Mutator - sets buffer to given condition
    void set_condition(vector<string> conditions);

    //Tokenizes vectorstr and returns queue of tokens
    queue<r_Token*> convert();

private:
    //Condition
    vector<string> _buffer;
    //Tokenized result
    queue<r_Token*> tokenized;

};


#endif // TOKENIZER_H
