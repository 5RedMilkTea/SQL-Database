#ifndef RPN_H
#define RPN_H

#include "../../Reusables/IteratedQueue/queue/queue.h"
#include "../../Reusables/IteratedStack/stack/stack.h"
#include "r_token.h"

#include "debug_format.h"
#include "../../Reusables/VectorClass/VectorClass/vector.h"
#include "../../B Tree/bplus_multimap/mmap.h"
#include "../../B Tree/bplusmap/map.h"

//#include union and intersection


//Takes in a postfix expresssion from shunting yard and returns a vector of rec
//numbers
class rpn
{
public:
    rpn();
    rpn(queue<r_Token*> postfix, vector<MMap<string, long>> indices,
        Map<string, size_t> fields_map);

    //Evaluate expression
    vector<long> filter();


    //Temporarily public
    //vector<long> _process(r_Token* sym1, r_Token* sym2, r_Token* op);


private:
    //Holds evaluated value
    queue<r_Token*> _hold;
    //Stack of r_token*
    stack<r_Token*> _stack;
    //Processed tokens
    stack<vector<long>> _processed;

    Map<string, size_t> _fields_map;

    vector<MMap<string, long>> _indices;



    vector<long> _process(r_Token* sym1, r_Token* sym2, r_Token* op);


};


#endif // RPN_H
