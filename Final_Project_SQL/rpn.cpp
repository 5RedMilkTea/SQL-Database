#include "rpn.h"


rpn::rpn()
{
    //Intentionally blank
}

//Given postfix expression
rpn::rpn(queue<r_Token*> postfix, vector<MMap<string, long> > indices,
         Map<string, size_t> fields_map)
{
    //Initializes Queue of tokens to given tokens
    _hold = postfix;
    //Initializes vector of multimaps<string, long> to given indices
    _indices = indices;
    //Initializes fields map
    _fields_map = fields_map;
}

//Need to do parenthesis***
vector<long> rpn::filter()
{
    const bool DEBUG = false;

    if (DEBUG)
    {
        cout << "-----RPN FILTER-----" << endl;
    }
    //While theres still stuff in the queue
    while (!_hold.isEmpty())
    {
        //If front of queue is a SYMBOL
        if (_hold.front()->getType() == 0)
        {
            //Pushes symbol into stack
            _stack.push(_hold.pop());
        }
        //If front of queue is a LOGICAL operator
        else if (_hold.front()->getType() >= 1
                 && _hold.front()->getType() <= 3)
        {
            //Stack should NOT be empty when a logical operator is called
            assert (!_processed.isEmpty()
                    && "Attempting to perform logical operator with no values "
                       "in stack");

            //UNIONS vectors in vector stack if OR
            if (_hold.front()->getType() == 1)
            {
                //Takes the operator token
                r_Token* OR = _hold.pop();

                assert(OR->token() == "or" && "EXPECTED 'OR' TOKEN");
                assert(OR->is_Valid() && "INVALID 'OR' TOKEN");

                //Pops the stack for vectors
                vector<long> lhs = _processed.pop();
                vector<long> rhs = _processed.pop();

                if (DEBUG)
                {
                    cout << "--OR operator--" << endl;
                    cout << spaced_left << "LHS record numbers: " << lhs
                         << endl;
                    cout << spaced_left << "RHS record numbers: " << rhs
                         << endl;
                }
                lhs.Union(rhs);

                if (DEBUG)
                {
                    cout << spaced_left << "UNION record numbers: " << lhs
                         << endl;
                }

                _processed.push(lhs);
            }
            //INTERSECTS vectors in vector stack if AND
            else if (_hold.front()->getType() == 2)
            {
                //Creates new vector to hold intersection
                vector<long>* intersect = new (vector<long>);

                //Check if right token
                r_Token* AND = _hold.pop();
                //Assert if not the right token
                assert(AND->token() == "and" && "EXPECTED 'AND' TOKEN");
                assert(AND->is_Valid() && "INVALID 'AND' TOKEN");

                //Pops stack to get two vectors
                vector<long> lhs = _processed.pop();
                vector<long> rhs = _processed.pop();

                //Intersects the two vectors popped and stores in vector
                intersect->Intersection(lhs, rhs);

                //DEBUG
                if (DEBUG)
                {
                    cout << "--AND operator--" << endl;
                    cout << spaced_left << "LHS record numbers: " << lhs
                         << endl;
                    cout << spaced_left << "RHS record numbers: " << rhs
                         << endl;
                    cout << spaced_left << "INTERSECTION: " << *intersect
                         << endl;
                }

                //Pushes new vector back into the stack of processed vectors
                _processed.push(*intersect);
            }
            //NOT
            else if (_hold.front()->getType() == 3)
            {
                //Not implemented for now
            }
        }
        //If front of queue is a RELATIONAL operator
        else if (_hold.front()->getType() >= 4 && _hold.front()->getType()
                 <= 8)
        {
            //Pops two values from stack of tokens
            r_Token* value = _stack.pop();
            r_Token* field = _stack.pop();
            //Pops the operator
            r_Token* op = _hold.pop();
            assert(op->is_Valid() && "Relational Operator is INVALID");

            if (DEBUG)
            {
                cout << "--RELATIONAL OPERATOR--" << endl;
                cout << spaced_left << "Field: " << field << endl;
                cout << spaced_left << "Operator: " << op << endl;
                cout << spaced_left << "Value: " << value << endl;
             }

            //Perform operation
            vector<long> rec_no = _process(field, value, op);
            _processed.push(rec_no);
        }
    }
    return _processed.top();

}

/* Process
 * PRE-CONDITION : Takes in 3 tokens: Field, value, and operator in that order
 *                  specifically.
 * POST-CONDITION: Performs operator on the field and value. Returns vector
 *                  of record numbers
*/
vector<long> rpn::_process(r_Token* field, r_Token* value, r_Token* op)
{
    const bool DEBUG = false;
    //Operator token should not be a symbol
    assert (op->getType() != 0 && "Token op is not an operator"); //Symbol is type 0
    //Operator should be relational
    assert (op->getType() > 3 && op->getType() < 9 && "Operator is not "
                                                      "relational");

    // Relational operators
     //4 =
     //5 >
     //6 >=
     //7 <
     //8 <=

    if (DEBUG)
    {
        cout << "RPN --PROCESS--" << endl;
        cout << spaced_left << "Field: " << field << endl;
        cout << spaced_left << "Value: " << value << endl;
        cout << spaced_left << "Operator" << op << endl;
    }

    //Return this
    vector<long> record_numbers;

    //If operator is = (type 4)
    if (op->getType() == 4)
    {
        if (DEBUG)
        {
            cout << "-----RPN Process: Type 4 = -----" << endl;
            cout << "Field Map[" << field->token() << "]: "
                 << _fields_map[field->token()] << endl;
            cout << "Indices[field_map[]]: "
                 << _indices[_fields_map[field->token()]];
        }

        record_numbers = _indices[_fields_map[field->token()]]
                                    [value->token()];
    }
    //If operator is > (type 5)
    else if (op->getType() == 5)
    {
        //Iterator to upper bound of the map
        MMap<string, long>::Iterator upper = _indices
                [_fields_map[field->token()]].upper_bound(value->token());

        if (DEBUG)
        {
            cout << "-----RPN Process: TYPE 5 > -----" << endl;
            cout << "Upper: " << *upper << endl;
            MMap<string, long>::Iterator end =
                    _indices[_fields_map[field->token()]].end();
            cout << "End : " << *end << endl;

            cout << "T/F: " << (upper != MMap<string,long>::Iterator(nullptr))
                 << endl;
        }

        //While iterator is not pointing to the end of the mmap
        while (upper != MMap<string,long>::Iterator(nullptr))
        {
            MPair<string, long> temp = *upper;
            //Combine record numbers
            record_numbers.Union(temp.value_list);
            upper++;
        }
    }
    //If operator is >= (type 6)
    else if (op->getType() == 6)
    {
        //Lower bound is >= for some reason... LOL sooo yeah using lower bound
        //for greater equal
        MMap<string, long>::Iterator upper_equal = _indices
                 [_fields_map[field->token()]].lower_bound(value->token());

        if (DEBUG)
        {
            cout << "-----RPN Process: TYPE 6 >= -----" << endl;
            cout << "Upper equal: " << *upper_equal << endl;
            MMap<string, long>::Iterator end =
                    _indices[_fields_map[field->token()]].end();
            cout << "End : " << *end << endl;

            cout << "T/F: " << (upper_equal !=
                    MMap<string,long>::Iterator(nullptr)) << endl;
        }

        //Logic needs to be fixed
        /* 1) check if equal
         * 2) Loop - Union
         * 3) it++
        */

        //While iterator is not pointing to the end of the mmap
        while (upper_equal != MMap<string,long>::Iterator(nullptr))
        {
            MPair<string, long> temp = *upper_equal;
            //Combine record numbers
            record_numbers.Union(temp.value_list);
            upper_equal++;
        }
    }
    //If operator is < (type 7)
    else if (op->getType() == 7)
    {
        //Lower bound returns value < should stop before
        MMap<string, long>::Iterator lower = _indices
                 [_fields_map[field->token()]].lower_bound(value->token());

        //Begin iterator - Iterator at the beginning
        MMap<string, long>::Iterator begin =
                _indices[_fields_map[field->token()]].begin();
        if (DEBUG)
        {
            cout << "-----RPN Process: TYPE 7 < -----" << endl;
            cout << "Lower end bound: " << *lower << endl;

            cout << "Begin : " << *begin << endl;

            cout << "T/F: " << (begin != lower)
                 << endl;
        }

        //While iterator is not pointing to the lower bound of the mmap
        while (begin != lower)
        {

            MPair<string, long> temp = *begin;
            //Combine record numbers
            record_numbers.Union(temp.value_list);
            //Increment begin iterator forward
            begin++;
        }
    }
    //If operator is <= (type 8)
    else if (op->getType() == 8)
    {
        //Lower bound returns value < should stop before
        MMap<string, long>::Iterator lower_equal = _indices
                 [_fields_map[field->token()]].lower_bound(value->token());

        //Begin iterator - Iterator at the beginning
        MMap<string, long>::Iterator begin =
                _indices[_fields_map[field->token()]].begin();
        if (DEBUG)
        {
            cout << "-----RPN Process: TYPE 8 <= -----" << endl;
            cout << "Lower end bound: " << *lower_equal << endl;

            cout << "Begin : " << *begin << endl;

            cout << "T/F: " << (begin != lower_equal)
                 << endl;
        }

        //Increment forward so the key is included when extracting record num
        lower_equal++;

        //While iterator is not pointing to the lower bound of the mmap
        while (begin != lower_equal)
        {
            MPair<string, long> temp = *begin;
            //Combine record numbers
            record_numbers.Union(temp.value_list);
            begin++;
        }
    }

    return record_numbers;
}
