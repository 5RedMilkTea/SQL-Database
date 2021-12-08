#include "file_functions.h"

//-------------------------------------------------
bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
        std::fstream::in | std::fstream::binary );
    if (ff.fail())
    {
        if (debug)
            cout << "file_exists(): File does NOT exist: "
                      << filename << endl;
        return false;
    }
    if (debug) cout<<"file_exists(): File DOES exist: "<<filename<<endl;
    ff.close();
    return true;
}

//OPENS FILE FOR READING AND TO WRITE TO THE END
void open_fileRW(fstream& f, const char filename[]) throw(char*)
{
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug)
                cout<<"open_fileRW: file created successfully: "
                          <<filename<<endl;
        }
    }
    else{
        f.open (filename,
            std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}

//OPENS FULE FOR OVERWRITING
void open_fileW(fstream& f, const char filename[]){
    f.open (filename,
            std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}


/*
#include "../../Reusables/VectorClass/VectorClass/vector.h"
#include "record.h"
void fun_with_binary_file()
{
    string Bob[3] = {"BOB", "jones", "CS"};
    vector<string> bob;
    for (int i = 0; i < 3; i++)
    {
        bob.push_back(Bob[i]);
    }

    string Jane[3] = {"Jane","Doe","Math"};
    vector<string> jane;
    for (int i = 0; i < 3; i++)
    {
        jane.push_back(Jane[i]);
    }

    string Steve[3] = {"Steve","Booker","CS"};
    vector<string> steve;
    for (int i = 0; i < 3; i++)
    {
        steve.push_back(Steve[i]);
    }


    string Jack[3] = {"Jack","Thompson","Phys"};
    vector<string> jack;
    for (int i = 0; i < 3; i++)
    {
        jack.push_back(Jack[i]);
    }

    //ARRAY of vector<string>s
    vector<string> records[4];
    records[0] = bob;
    records[1] = jane;
    records[2] = steve;
    records[3] = jack;

    fstream f;
    //Open file
    open_fileW(f, "two_d_record_list.bin");
    //Loop through writing info into file
    for (int i= 0; i<4; i++)
    {
        //Create record of the vectorstr
        Record rec(records[i]);
        int record_num = rec.write(f);
        cout << rec << " was written in record number: " << record_num << endl;

    }
    f.close();

    cout <<endl<<endl<<endl<< "================================" << endl;


    Record r;
    open_fileRW(f, "two_d_record_list.bin" );
    int recno = 0;
    //While not in erroneous state
    while (!f.eof())
    {
        r.read(f, recno);   //Read stuff from record number ____ from file
        recno++;            //Move to next record
        cout<<"["<<recno<<"] "<< r.get_record() << endl;
    }
    cout<<"-------- E N D -------------"<<endl;
}

*/
