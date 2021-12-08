#ifndef RECORD_H
#define RECORD_H

#include <cstring>
#include "file_functions.h"
#include "../../Reusables/VectorClass/VectorClass/vector.h"
using namespace std;

typedef vector<string> vectorstr;

class Record
{
public:
    //Default Constructor
    Record()
    {
        //Initializes rows to NULL
        for (int i= 0; i < MAX_ROWS; i++)
        {
            record[i][0] = NULL;
        }
        recno = -1;
        _num_of_fields = 0;
    }

    //Copies records onto a row
    Record(const vectorstr& v)
    {
        //Sets ?cstrings to null
        for (int i= 0; i < MAX_ROWS; i++)
        {
            record[i][0] = NULL;
        }

        //Takes in values from vectorstr
        for (int i= 0; i < v.size(); i++)
        {
            //Copies Record into row i column
            strcpy(record[i], v[i].c_str());
        }
        _num_of_fields = v.size();
    }

    //Returns vector<string> if there are values in the record
    vectorstr get_record()
    {
        vectorstr v;
        //_num_of_fields will be w/e the constructor set it to be
        //cout<<" get_record(): "<< _num_of_fields <<endl;

        for (int i = 0; i < MAX_ROWS; i++)
        {
            v.push_back(string(record[i]));
        }
        return v;
    }

    //Returns record number
    long write(fstream& outs);
    //Returns number of characters read
    long read(fstream& ins, long recno);

    //overloaded << operator
    friend ostream& operator<<(ostream& outs, const Record& r);

private:
    //Maximum number of strings
    static const int MAX_ROWS = 20;
    //Maximum number of characters in a string
    static const int MAX_COLS = 50;
    int recno;
    int _num_of_fields;
    char record[MAX_ROWS][MAX_COLS];
};


#endif // RECORD_H
