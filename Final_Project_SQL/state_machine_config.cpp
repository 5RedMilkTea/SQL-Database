#include "state_machine_config.h"

#include <iostream>
#include <map>
using namespace std;

//Sets table to -1
/* initTable
 * Pre-Condition : uninitialized 2D int array
 * Post-Condition: 2D int array initialized to -1's
*/
void initTable(int machine[][SQL_MAX_COLUMNS])
{
    for (int i = 0; i < SQL_MAX_ROWS; i++)
    {
        for (int j = 0; j < SQL_MAX_COLUMNS; j++)
        {
            machine[i][j] = -1;
        }
    }
}

/* markCells
 * Pre-Condition : unconfigured 2D int array
 * Post-Condition: Sets value of cells
*/
void markCells(int machine[][SQL_MAX_COLUMNS])
{
    initTable(machine);

    markSuccess(machine);

    //overall SQL Commands
    markSelect(machine);
    markInsert(machine);
    markCreate(machine);
    markDrop(machine);

    markFrom(machine);
    markStar(machine);
    markSymbol(machine);
    markInto(machine);
    markWhere(machine);
    markConditionalOperator(machine);
    markComma(machine);
    markValues(machine);
    markParenthesis(machine);
    markTable(machine);
    markFields(machine);



    //printTable(machine);
}

/* markSuccess
 * Pre-Condition : unconfigured 2D int array
 * Post-Condition: Specifies which state (rows) are success states and which
 *                 are not
*/
void markSuccess(int machine[][SQL_MAX_COLUMNS])
{
    //Only col 0
    //Base state - when table is initialized
    machine[0][0] = 0;

    //Looking for command
    machine[1][0] = 0;
    //Symbol (field name)
    machine[2][0] = 0;
    //From
    machine[3][0] = 0;
    //Symbol (table name)
    machine[4][0] = 1;

    //CONDITION
    //Where was inputted - expecting condition
    machine[5][0] = 0;

    //Route 1 from WHERE no parenthesis
    //Field name ex: last
    machine[6][0] = 0;
    //Relational Operator ex: '=', '<=', '>=', '>', '<'
    machine[7][0] = 0;
    //Specific value ex: Wu
    machine[8][0] = 1;
    //Comma after fieldname; expects another field name
    machine[9][0] = 0;
    //Logical Operator - expects symbol condition
    machine[10][0] = 0;

    //Route 2 from WHERE
    //L Parenthesis
    machine[11][0] = 0;
    //Condition Field SYMBOL
    machine[12][0] = 1;

    //Insert
    machine[20][0] = 0; //Keyword Insert - expects table name
    machine[21][0] = 0; //Keyword INTO
    machine[22][0] = 0; //Table name - expects keyword VALUES
    machine[23][0] = 0; //VALUES - expects SYMBOL value
    //Insert success state
    machine[24][0] = 1; //SYMBOL value - expects comma if more values
    machine[25][0] = 0; //COMMA - expects SYMBOL values

    //Make/Create table
    machine[30][0] = 0; //make      keyword CREATE or MAKE
    machine[31][0] = 0; //table     keyword TABLE
    machine[32][0] = 0; //employee  symbol tablename
    machine[33][0] = 0; //fields    keyword FIELDS
    machine[34][0] = 1; //fname     symbol FIELD NAME
    machine[35][0] = 0; //,         keyword COMMMA

    //Drop table
    machine[40][0] = 0; //Drop
    machine[41][0] = 0; //TABLE
    machine[42][0] = 1; //Table name
}

//Mark select States
void markSelect(int machine[][SQL_MAX_COLUMNS])
{
    //Select index is index 1
    //"Select" after base state is row 1

    //Marks success states of when select is the input
    machine[0][1] = 1;

}

void markInsert(int machine[][SQL_MAX_COLUMNS])
{
    //From base state to insert command
    //Insert keyword is col 8
    machine[0][8] = 20; //Expects keyword INTO or symbol table name

}

void markCreate(int machine[][SQL_MAX_COLUMNS])
{
    //17 - Make
    // 6 - Create
    machine[0][17] = 30;
    machine[0][6] = 30;
}

void markDrop(int machine[][SQL_MAX_COLUMNS])
{
    //DROP - 7
    machine[0][7] = 40;
}

//Mark from
void markFrom(int machine[][SQL_MAX_COLUMNS])
{
    //From col index is index 2
    //"From" after symbol (field_name) is row 3

    //when "from" input after field_name state 2
    machine[2][2] = 3;
}

void markStar(int machine[][SQL_MAX_COLUMNS])
{
    //If user inputs *  in the select state
    machine[1][3] = 2; //Select all
}

void markTable(int machine[][SQL_MAX_COLUMNS])
{
    //Table - 18
    machine[30][18] = 31;

    //DROP
    machine[40][18] = 41;
}

void markFields(int machine[][SQL_MAX_COLUMNS])
{
    machine[32][19] = 33;
}

void markSymbol(int machine[][SQL_MAX_COLUMNS])
{
    //Symbol col index is 4

    //When field name is inputted after "select" command
    machine[1][4] = 2; //Go to neutral state

    //MARKING COMMA
    machine[9][4] = 2; //Expects another fieldname after comma

    //When file name is inputted after "from" command
    machine[3][4] = 4; //Go to success state

    //Where a symbol is input after "where"
    machine[5][4] = 6; //Expects fieldname

    machine[6][4] = 7; //Expects relational operator ex: "="

    machine[7][4] = 8; //Expects symbol - a success state

    //Goes back to condition state after a logical operator (ex: AND)
    machine[10][4] = 6;

    //From left parenthesis condition field inputted
     //Route 2 Condition
    machine[11][4] = 6; //Field inputed

    //Table name after INSERT command (insert into this table)
    machine[21][4] = 22;
    //VALUES to be inserted
    machine[23][4] = 24;
    machine[24][4] = 24;
    //Comma -> symbol
    machine[25][4] = 24;

    //Table name from MAKE/CREATE TABLE
    machine[31][4] = 32; //Table name -> expecting keyword FIELDS
    machine[33][4] = 34; //FIELDS-> Symbol
    machine[35][4] = 34;

    //DROP table tablename
    machine[41][4] = 42;

}

void markInto(int machine[][SQL_MAX_COLUMNS])
{
    //Into is collumn 14
    machine[20][14] = 21; //Insert command -> INTO
}

void markComma(int machine[][SQL_MAX_COLUMNS])
{
    //Marking Comma
    machine[2][12] = 9; //User input a comma after the field name (column)
    machine[24][12] = 25;
    machine[34][12] = 35;
}

void markConditionalOperator (int machine[][SQL_MAX_COLUMNS])
{
    //From condition success state - user inputs 'AND' keyword
    machine[8][9] = 10;
    //From condition success state - user inputs 'OR' keyword
    machine[8][10] = 10;

    //After right parenthesis success state -> AND/OR
    machine[12][9] = 10;
    machine[12][10] = 10;

}

void markValues(int machine[][SQL_MAX_COLUMNS])
{
    //Values is col 13
    machine[22][13] = 23;
}

void markParenthesis(int machine[][SQL_MAX_COLUMNS])
{
    //Left Parenthesis
    machine[5][15] = 11;
    machine[11][15] = 11;
    machine[10][15] = 11;

    //Right Parenthesis
    machine[8][16] = 12;
    machine[12][16] = 12;
}

void markWhere(int machine[][SQL_MAX_COLUMNS])
{
    //Where column index is 5

    //Where means condition
    //Select * from file where (condition)
    machine[4][5] = 5;

}



/* printTable
 * Pre-Condition : 2D int array
 * Post-Condition: Output 2D int array onto console
*/
void printTable(int machine[][SQL_MAX_COLUMNS])
{
    for (int i = 0; i < SQL_MAX_ROWS; i++)
    {
        cout << "ROW: " << setw(2) << i << "|";
        for (int j = 0; j < SQL_MAX_COLUMNS; j++)
        {
            cout << setw(2) << machine[i][j] << " ";
        }
        cout << '|' << endl;
    }
}
