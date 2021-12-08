#include "table.h"

//SERIAL NUMBER
int Table::serial = 1000;

//CONSTRUCTORS
//Creates a new table and file
Table::Table(string name, vectorstr field_list)
{
    //Debugger
    const bool DEBUG = false;

    //Initialize private memberts to given parameters
      //Table name
    _table_name = name;
      //Creates file name
    _file_name = name + ".tbl";
      //Initialize table size to 0 (number of records)
    table_size = 0;

    //Initializes the number of fields in the table
    _number_of_fields = actual_size(field_list);

    //Builds indices vector, field map and fields name vector
    set_fields(field_list);

    //Debug
    if (DEBUG)
    {
        cout << "---DEFAULT CONSTRUCTOR---" << endl;
        cout << spaced_left << "File name: " << _file_name << endl;
        cout << spaced_left<< "Number of fields: " << _number_of_fields
             << endl;
        cout << spaced_left << "Number of indices: " << indices.size()
             << endl;
        cout << spaced_left << "Fields Map Size: " << _fields_map.size()
             << endl;
    }

    //Fstream object
    fstream file;
    //Create record of fields
    Record r(field_list);

    //Open file to OVERWRITE field names into field list
    open_fileW(file, _file_name.c_str()); //.c_str converts the string to a
                                            //cstring


    //Write fields into file
    int record_number = r.write(file);
    //Increase table size
    table_size++;

    if (DEBUG)
    {
        //Spaced-left = setw(20) left
        cout << spaced_left << "field list: " << field_list << endl;
        cout << r << "was written in record number: " << record_number << endl;
    }

    file.close();
}

//Opens existing table constructor
Table::Table(const string& name)
{
    const bool DEBUG = false;

    if (DEBUG)
    {
        cout << "-----Table Constructor Open Exisitng------" << endl;
    }

    //Initialize private memberts to given parameters
      //Table name
    _table_name = name;

      //Creates file name
    _file_name = name + ".tbl";


    //Initialize table size to 0
    table_size = 0;

    //Fstream object
    fstream file;
    //Declaration of a record object to read
    Record r;

    //Opens file for reading - will only read the 1st record which contains
        //field names
    open_fileRW(file, _file_name.c_str());

    //Read Field names (Field names are stored in record 0)
    r.read(file, 0);
    //Increase number of records by 1 b/c field names are in record[0]
    table_size = 1;

    //Set Fields names vector to field names obtained from existing file
    _field_names = r.get_record(); //Get record returns a vector of char
                                //max row 20

    //Need to get proper size of field names vector
    _number_of_fields = actual_size(_field_names);

    //Closes file b/c reindex reopens the file
    file.close();

    //Builds Indices multimap, field names map, and field names vector
    set_fields(_field_names); //builds with given info from read_info

    if (DEBUG)
    {
        cout << "-Reindex-" << endl;
    }
    //reBuilds indices
      //Opens file
    reindex(); //This actually updates the table size as well....


}

//Inserts record into new row in the table
void Table::insert_into(vector<string> field_list)
{
    const bool DEBUG = false;

    if (DEBUG)
    {
        cout << "---Table Insert_into---" << endl;
        cout << spaced_left << "Number of fields: " << _number_of_fields
             << endl;
    }

    //Fstream object
    fstream file;
    //Create record object
    Record r(field_list);

    //Opens file for writing to the END
    open_fileRW(file, _file_name.c_str());
    //Write record into file
    int record_number = r.write(file);
    //Set last_record_num to the record number obtained
    last_record_num = record_number;
    //Increase table size
    table_size++;

    //Insert record numbers into corresponding index structure
    for (size_t i = 0; i < _number_of_fields; i++) //Number of fields
    {
        //Insert Multipair of string and record number
        indices[i].insert(field_list[i], record_number);
        if (DEBUG)
        {
            //cout << "INDICE[" << i << "] (" << _field_names[i] << "): "
            //     << endl << indices[i] << endl;
        }
    }

    if (DEBUG)
    {
        cout << "DEBUG " << endl;
        cout << field_list << "was inserted into record number: "
             << record_number << endl;
        cout << spaced_left << "Current table size: " << table_size << endl;
    }

    file.close();
}

/* Select All
 * Pre-Condition : N/A
 * Post-Condition: Returns a table with all the records in it
*/
Table Table::select_all()
{
    serial++;
    //Vector of record numbers
    vector<long> record_numbers;

    //Table that gets returned
      //New table's table name: Table_name and serial number, since its
      //select all, the field names are the same
    Table returnMe(_table_name + to_string(serial), _field_names);

    //Fstream object
    fstream file;
    //Record object to read from file
    Record r;

    //Open files and read
    open_fileRW(file, _file_name.c_str());


    //Push all the record numbers into the vector
    for (int i = 1; i < table_size; i++) //Starts at 1 to avoid reading the
                                            //field names
    {
        //B/c this is select all the record number will start from 0
        //Pushes back record numbers into vector of record numbers
        record_numbers.push_back(i);

        //Read record from file - record_numbers[i - 1] returns a long
          //i - 1 b/c the for loop starts at 1 so record_numbers[0] = 1;
        r.read(file, record_numbers[i - 1]);

        //Vector to hold record info of the selected fields
        vector<string> record_info;

        //Push back into the vector the record info
        for (int j = 0; j < _number_of_fields; j++)
        {
            record_info.push_back(r.get_record()[j]);
        }

        //Inserts the record information into the table
        returnMe.insert_into(record_info);
    }

    file.close();

    return returnMe;

}

//When given specified fields
Table Table::select_fields(const vectorstr &fields)
{
    serial++;
    const bool DEBUG = false;

    //fstream object
    fstream file;
    //record object
    Record r;

    //Create new table with designated fields
    Table temp(_table_name + to_string(serial), fields);

    //Opens file
    open_fileRW(file, _file_name.c_str());

    // - 1 because field names in record 0 count as a record
    //Insert filtered record numbers into the new table that's being returned
    for (size_t i = 1; i < table_size; i++)
    {
        //Reads record number from vector of record numbers extracted
        r.read(file, i);

        vectorstr record_w_col;

        for (size_t j = 0; j < fields.size(); j++)
        {
            record_w_col.push_back(r.get_record()[_fields_map[fields[j]]]);
        }
        if (DEBUG)
        {
            //The record thatjust got pushed back into the vectorstr
            cout << "Record #" << i << ": " << record_w_col
                    << endl;
        }
        temp.insert_into(record_w_col);
    }
    file.close();
    return temp;
}

//When given a condition and no fields
Table Table::select_conditional(const vectorstr &condition)
{
    const bool DEBUG = false;
    serial++;

    /* Pulling out record numbers from table
     * Utilizes Tokenizer, Shunting Yard, and RPN
     * Then
     * Creates new tables and inserts the records associated with the record#s
    */

    //Vector to hold record numbers
    vector<long> record_numbers;

    //Record object
    Record r;

    //fstream object
    fstream file;

    //Fstream object for new table class
    fstream other_file;

    //---Triple Threat---
    //Tokenize vector of strings condition
    //Instantiate tokenizer obj
    Tokenizer tokenizer(condition);
    //Tokenizes the string
    queue<r_Token*> infix = tokenizer.convert();

    //Instantiate Shunting yard object
    Shunting_Yard shunting_yard(infix);
    //Converts infix expression to a postfix expression
    queue<r_Token*> postfix = shunting_yard.postfix();

    //Instantiate rpn object
    rpn _rpn(postfix, indices, _fields_map);
    //RPN filters record numbers based on given condition and returns vector
    record_numbers = _rpn.filter();
    //-------------------

    //DEBUG
    if (DEBUG)
    {

        cout << " -----TABLE SELECT----- " << endl;
        cout << "Indices: " << endl;
        cout << indices << endl;
        cout << "Fields Map: " << _fields_map << endl;
        cout << spaced_left << "Condition: " << condition << endl;
        cout << spaced_left << "Filtered Rec#s: " << record_numbers << endl;
    }

    //Create table object to return
                    //Table_name + serial number, since all fields are selected
                    //then pass it all the field names
    Table returnThis(_table_name + to_string(serial), _field_names);

    //Opens this table file
    open_fileRW(file, _file_name.c_str());

    //Record number.size is the size of the vector of the fields holding the
        //record numbers from indices
    //Print selected table
    for (int i = 0; i < record_numbers.size(); i++)
    {
        //Should read the record numbers returned from indices
        r.read(file, record_numbers[i]);

        //Insert records read from this table into the table being returned
        returnThis.insert_into(r.get_record());

        /*
        //Opens file in new table
        open_fileRW(other_file, returnThis._file_name.c_str());
        //Writes record into the new file
        r.write(other_file);
        //Close the file
        other_file.close();
        */
    }

    file.close();
    //returnThis.reindex();
    //Returns table of selected fields
    return returnThis;

}

//Generalized select - when given specified conditions and fields
Table Table::select(const vectorstr& fields, const vectorstr& condition)
{
    const bool DEBUG = false;
    serial++;

    /* Pulling out record numbers from table
     * Utilizes Tokenizer, Shunting Yard, and RPN
     * Then
     * Creates new tables and inserts the records associated with the record#s
    */

    //Vector to hold record numbers
    vector<long> record_numbers;

    //Record object
    Record r;

    //fstream object
    fstream file;

    //---Triple Threat---
    //Tokenize vector of strings condition
    //Instantiate tokenizer obj
    Tokenizer tokenizer(condition);
    //Tokenizes the string
    queue<r_Token*> infix = tokenizer.convert();

    //Instantiate Shunting yard object
    Shunting_Yard shunting_yard(infix);
    //Converts infix expression to a postfix expression
    queue<r_Token*> postfix = shunting_yard.postfix();

    //Instantiate rpn object
    rpn _rpn(postfix, indices, _fields_map);
    //RPN filters record numbers based on given condition and returns vector
    record_numbers = _rpn.filter();
    //-------------------

    //DEBUG
    if (DEBUG)
    {
        cout << " -----TABLE SELECT----- " << endl;
        cout << spaced_left << "Condition: " << condition << endl;
        cout << spaced_left << "Filtered Rec#s: " << record_numbers << endl;
    }

    //Create new table with designated fields
    Table temp(_table_name + to_string(serial), fields);

    //Opens file
    open_fileRW(file, _file_name.c_str());

    //Insert filtered record numbers into the new table that's being returned
    for (size_t i = 0; i < record_numbers.size(); i++)
    {
        //Reads record number from vector of record numbers extracted
        r.read(file, record_numbers[i]);

        vectorstr record_w_col;

        for (size_t j = 0; j < fields.size(); j++)
        {
            record_w_col.push_back(r.get_record()[_fields_map[fields[j]]]);
        }
        if (DEBUG)
        {
            //The record thatjust got pushed back into the vectorstr
            cout << "Record #" << record_numbers[i] << ": " << record_w_col
                    << endl;
        }
        temp.insert_into(record_w_col);
    }
    file.close();
    return temp;
}


bool Table::empty()
{
    //If not counting field names then -1
    return (table_size - 1 == 0);
}

//Returns vector of field names
vector<string> Table::get_fields()
{
    return _field_names;
}


//-----Private Member Function Definitions-----

/* set_fields
 * PRE-CONDITION : Vector that contains the name of the fields
 * POST-CONDITION: Initializes private member _fields_names to the given vector
 *                 of field names. Also initializes the keys for the field
 *                 names map. Additionally creates the pairs for the indices
 *                 vector.
*/
void Table::set_fields(vectorstr field_list)
{
    const bool DEBUG = false;
    //Indices is a vector<MMap<string, long>> The multipair is the field and
                            //record number
    //Field list
    _field_names = field_list;

    //Initialize indices and fields map
    for (size_t i = 0; i < _number_of_fields; i++)
    {
        //Build indices vector
        indices.push_back(MMap<string, long>());
        //Build field map
            //Pushes name and associated number
        _fields_map.insert(_field_names[i], i); //Kinda like enumerating the
                                            //field names
    }

    if (DEBUG)
    {
        cout << "--SET FIELDS--" << endl;
        cout << "Field Map: " << endl;
        cout << _fields_map << endl;
    }

}

//Assumes that there is an existing fields_file and file name
void Table::reindex()
{
    //Fstream object
    fstream file;
    //Opens file
    open_fileRW(file, _file_name.c_str());

    //Declaration of a record object
    Record r;

    //Initialize record number to 1 to read from record 1
        //(record 0 holds field names)
    int record_num = 1;

    //Read record
    r.read(file, record_num); //Reads record from file

    //While open file is not in an erroenous state
    while (!file.eof())
    {
        //Increase table size
        table_size++;

        //Number of fields
        for (size_t i = 0; i < _number_of_fields; i++)
        {
            indices[i].insert(r.get_record()[i], record_num);
        }

        //Increment record number for while loop to read the next record
        record_num++;

        //Read record
        r.read(file, record_num); //Reads record from file
    }

    file.close();
}

//Takes a vectorstr from a record and returns the actual size of the vector
//B/c of its from a record the size will always be whatever it is set to in
// the record class
size_t Table::actual_size(vectorstr fields)
{
    const bool DEBUG = false;
    if (DEBUG)
    {
        cout << "-----ACTUAL SIZE-----" << endl;
        cout << "Fields initial size: " << fields.size() << endl;
        cout << "Fields: " << fields << endl;
    }
    //Initializes size to 0
    size_t size = 0;

    //For size is 0, less than vector size
    for (size = 0; size < fields.size(); size++)
    {
        if (fields[size] == "\0")
        {
            if (DEBUG)
                cout << "New size: " << size << endl;
            return size;
        }
    }
    return size;
}


// << operator
ostream& operator << (ostream& outs, Table print_me)
{
    //Prints table name and number of records excluding field names, hence
    //the - 1
    outs << "TABLE NAME: <" << print_me._table_name << "> - " << "Records: "
         << print_me.table_size - 1 << endl << endl;;
    //Record object
    Record r;
    //Fstream object
    fstream f;
    //Prints records
    open_fileRW(f, print_me._file_name.c_str());

    //Prints the table
    for (size_t i = 0; i < print_me.table_size; i++)
    {
        //Reads from file into record
        r.read(f, i);

        if (i != 0)
        {
            outs << "[" << setw(3) << right << i << "] ";
        }
        else
        {
            outs << setw(6) << left << "Rec#";
        }

        for (int j = 0; j < print_me._number_of_fields; j++)
        {
            //Get record value
            outs << setw(20) << left << r.get_record()[j] << "|";
        }
        outs << endl;

    }
    f.close();

    return outs;
}
