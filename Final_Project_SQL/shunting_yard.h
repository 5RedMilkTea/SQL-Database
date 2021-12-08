#ifndef SHUNTING_YARD_H
#define SHUNTING_YARD_H


#include "r_token.h"
#include "../../Reusables/IteratedQueue/queue/queue.h"
#include "../../Reusables/IteratedStack/stack/stack.h"

class Shunting_Yard
{
public:
    Shunting_Yard();
    Shunting_Yard(queue<r_Token*> infix);

    void set_infix(queue<r_Token*> infix);

    //Returns Postfix expresssion from infix
    queue<r_Token*> postfix();

private:
    //Stores infix expression
    queue<r_Token*> _infix;
};


#endif // SHUNTING_YARD_H
