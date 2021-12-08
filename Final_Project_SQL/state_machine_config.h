#ifndef STATE_MACHINE_CONFIG_H
#define STATE_MACHINE_CONFIG_H


#include <iostream>
#include <iomanip>
using namespace std;

//TABLE DIMENSIONS
const int SQL_MAX_ROWS = 50;                     //Max Number of states
const int SQL_MAX_COLUMNS = 25;                 //Max number of different keywords
//MAX STRING LENGTH TAKEN
const int SQL_MAX_BUFFER = 100;

/* Enumerated Keyword
 * 1) Select
 * 2) From
 * 3) * AKA all
 * 4) Symbol
 * 5) Where
 * 6) Create
 * 7) Drop
 * 8) Insert
 * 9) AND
 * 10) OR
 * ---- NOT --- implement in future
 * 11) DELETE
 * 12) Comma ,
 * 13) VALUES
 * 14) INTO
 * 15) ( //Left Parenthesis
 * 16) ) //Right Parenthesis
 * 17) Make
 * 18) Table
 * 19) Fields
 *

 States:
 *-1) FAIL state
 * 0) Base state

 * 1) User input keyword command SELECT
 * 2) user input SYMBOL Fields
 * 3) User input keyword FROM
 * 4) User input SYMBOL Table name
 * 5) User input WHERE initiating a condition
 * 6) Condition Field (column) SYMBOL
 * 7) Condition relational operator SYMBOL
 * 8) Condition value SYMBOL
 * 9) User input KEYWORD Comma from state 2
 * 10) User inputs a SYMBOL logical operator after a condition
 * 11) User inputs KEYWORD Left Parenthesis (
 * 12) User inputs right parenthesis

 * 20) User input keyword command INSERT
 * 21) User input keyword INTO
 * 22) User input SYMBOL table name
 * 23) User input keyword VALUES
 * 24) User input SYMBOL values
 * 25) User input KEYWORD comma from state 24
 *
 * 30) User input keyword command CREATE / Make
 * 31) User input keyword TABLE
 * 32) User input symbol table name
 * 33) User input keyword FIELDS
 * 34) User input symbol field_name
 * 35) user input comma
 *
 * 40) User input keyword command DROP
 * 41) User input keyword TABLE
 * 42) User input SYMBOL table name


 * */


//Initializes table to -1 values
void initTable(int machine[][SQL_MAX_COLUMNS]);

//Mark first column for success states
void markSuccess(int machine[][SQL_MAX_COLUMNS]);

//Mark cells for next state
void markCells(int machine[][SQL_MAX_COLUMNS]);

//Called in markCells
//Select command
void markSelect(int machine[][SQL_MAX_COLUMNS]);
//Insert command
void markInsert(int machine[][SQL_MAX_COLUMNS]);
//Make/Create Table
void markCreate(int machine[][SQL_MAX_COLUMNS]);
//Drop Table
void markDrop(int machine[][SQL_MAX_COLUMNS]);

void markFrom(int machine[][SQL_MAX_COLUMNS]);
void markStar(int machine[][SQL_MAX_COLUMNS]);
void markSymbol(int machine[][SQL_MAX_COLUMNS]);
void markInto (int machine[][SQL_MAX_COLUMNS]);
void markWhere(int machine[][SQL_MAX_COLUMNS]);
void markConditionalOperator(int machine[][SQL_MAX_COLUMNS]);
void markComma(int machine[][SQL_MAX_COLUMNS]);
void markValues(int machine[][SQL_MAX_COLUMNS]);
void markParenthesis(int machine[][SQL_MAX_COLUMNS]);
void markTable(int machine[][SQL_MAX_COLUMNS]);
void markFields(int machine[][SQL_MAX_COLUMNS]);
//void markSpecial(int machine[][MAX_COLUMNS]);

/*
//Special Cases
void markEllipsis(int machine[][MAX_COLUMNS]);
void markDouble(int machine[][MAX_COLUMNS]);
void markApostrophe(int machine[][MAX_COLUMNS]);
void markDollarSign(int machine[][MAX_COLUMNS]);
*/
//Print table
void printTable(int machine[][SQL_MAX_COLUMNS]);


#endif // STATE_MACHINE_CONFIG_H
