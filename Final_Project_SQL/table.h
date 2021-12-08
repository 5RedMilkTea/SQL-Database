#ifndef TABLE_H
#define TABLE_H

#include "../../B Tree/bplus_multimap/mmap.h"
#include "../../B Tree/bplusmap/map.h"

#include "record.h"
#include "debug_format.h"

//Triple Threat
#include "tokenizer.h"
#include "shunting_yard.h"
#include "rpn.h"


class Table
{
public:
    //Default CTor
    Table() : table_size(0), _table_name(""), _file_name(""),
        last_record_num(NULL), _field_names({}), _number_of_fields(0){}

    //Creates new table
    Table(string name, vectorstr field_list);
    //Open an existing table
    Table (const string& name);


    //Insert records into the table
    void insert_into(vectorstr field_list);

    //Accursed --- SELECT FUNCTIONS ---
    //Selects and returns the whole table
    Table select_all();

    //Returns table of designated fields
    //Specified condition but select all fields
    Table select_conditional(const vectorstr& condition); //Filter rows

    //Specified fields but no specific conditions
    Table select_fields(const vectorstr& fields); //Filter columns

    //Returns table of designated fields and conditions
    //Filter columnds and rows
    Table select(const vectorstr& fields, const vectorstr& conditions);

    //----------------------------------

    //Returns true if there are no records
    bool empty();

    //Returns the vector of field names
    vector<string> get_fields();

    //Prints table
    friend ostream& operator << (ostream& outs, Table print_me);


    vector<MMap<string, long>> indices;

private:
    //Total number of records in the table
    size_t table_size;

    //Name of the table
    string _table_name;

    //Name of the file .tbl
    string _file_name;

    //Global Serial Number
    static int serial;

    //Last record number
    long last_record_num;

    //Returns vector of names of fields
    vectorstr _field_names;

    //Number of fields (columns)
    size_t _number_of_fields;

    //Initializes field names vector, fields map MAP, and indices MULTIMAP
    void set_fields(vectorstr field_list);

    //Field names are the map keys and the value are their index number
    Map<string, size_t> _fields_map; //LIKE ENUMERATORS
    //ex: _fields_map["First Name"] returns 0

    //Vector of multimaps (Indices) ex: Indices[0]["Wu"]
                                   //Goes into MMap at vector[0] and looks for
                                    //key: "Wu" and returns value list
                                    //(record numbers of everyone with "Wu")
    //vector<MMap<string, long>> indices; //holds vector of record numbers

    //updates indices to each record's record number
    void reindex();

    //Takes in a record and returns the number of objects in the record
    size_t actual_size(vectorstr fields);
};

#endif // TABLE_H
