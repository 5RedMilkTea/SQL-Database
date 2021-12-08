#include "record.h"

//Returns record number
long Record::write(fstream &outs)
{
    //write to the end of the file.
    outs.seekg(0, outs.end);
    long pos = outs.tellp();

    //outs.write(&record[0], sizeof(record));
    outs.write(&record[0][0], sizeof(record)); //I'm assuming its [0][0] b/c
                                          //the value is removed from the arr
                                          //after its written right?
    return pos/sizeof(record);
}

//Returns number of characters read
long Record::read(fstream &ins, long recno)
{
    long pos = recno * sizeof(record);
    ins.seekg(pos, ios_base::beg);

    ins.read(&record[0][0], sizeof(record));
    //Does not update num_of_fields... hmmm
    return ins.gcount();

}

// << operator
ostream& operator << (ostream& outs, const Record& r)
{
    for (int i= 0; i < r._num_of_fields; i++)
    {
        outs << r.record[i] << "|";
    }
    return outs;
}

