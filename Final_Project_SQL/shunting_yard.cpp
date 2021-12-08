#include "shunting_yard.h"

//DEFAULT CTOR
Shunting_Yard::Shunting_Yard()
{

}

//Constructor
Shunting_Yard::Shunting_Yard(queue<r_Token*> infix)
{
    _infix = infix;
}

//Changes infix to postfix expression
queue<r_Token*> Shunting_Yard::postfix()
{
    //Queue that holds postfix
    queue<r_Token*> returnThis;
    //Stack that holds operators
    stack<r_Token*> holdOp;

    while (_infix.isEmpty() == false)
    {
        //Number/variable
        if (_infix.front()->getType() == 0)
        {
            //Pushes Number/Variable into postfix
            returnThis.push(_infix.pop());
        }

        //Checks if stack is empty and if precedence of operator is lower than
            //operator in stack
                  //                    Not a left parenthesis
        else if (!holdOp.isEmpty() && holdOp.top()->getType() != 9 &&
                 _infix.front()->getPrecedence()
                 <= holdOp.top()->getPrecedence())
        {
            returnThis.push(holdOp.pop());
        }
        //Parenthesis
        else if (_infix.front()->getType() == 10) //R Parenthesis
        {
            while (holdOp.top()->getType() != 9) //L Parenthesis
            {
                returnThis.push(holdOp.pop());
            }
            //Pop Right Parenthesis
            _infix.pop();
            //Pop Left Parenthesis
            holdOp.pop();
        }
        else
        {
            //Push Operator into stack
            holdOp.push(_infix.pop());
        }

    }
    //Returns operators left in stack until queue is empty
    while (holdOp.isEmpty() == false)
    {
        returnThis.push(holdOp.pop());
    }
    return returnThis;
}

//Mutator
void Shunting_Yard::set_infix(queue<r_Token *> infix)
{
    _infix = infix;
}
