#include "table.h"
#include "parse_tree.h"
#include "sql.h"
#include "rpn.h"
#include "shunting_yard.h"
#include "tokenizer.h"

void simple_table_test();
void simple_ptree_test();
void simple_token_test();
void bare_minimum();

void select_test();

//Parse Tree -> Tokenizer -> Shunting Yard -> RPN = Vector<Record Numbers>
void parser_test();

//Tokenizer
void simple_tokenize_test();

//Shunting Yard
void quick_shunting_yard_test();

//RPN
void rpn_filter_test();
//Private member function
//void rpn_process_test(); //Commented out


int main()
{
    char user_input;
    cout << "[B] for batch \t [I] for interactive" << endl;
    cin >> user_input;
    if (toupper(user_input) == 'I')
    {
        cout << "----------- SQL ----------- " << endl;
        SQL sql;
        sql.run();
        cout << " -----------END---------- " << endl;
    }
    else if (toupper(user_input) == 'B')
    {
        cout << " ------ SQL w/ BATCH file ----- " << endl;
        SQL sql("_!select-1.txt");
        sql.run_batch();
        cout << "----------- END ---------- " << endl;
    }



    //select_test();
    //bare_minimum();
    //parser_test();
    //rpn_filter_test();
    //rpn_process_test();
    //quick_shunting_yard_test();
    //simple_tokenize_test();
    //simple_ptree_test();
    //simple_token_test();
    //simple_table_test();

    return 0;
}

void simple_table_test()
{
    //Field List
    vector<string> field_names({"First", "Last", "Age", "Major", "Gender"});

    //Create and initialize table with fields
    Table People("Students", field_names);

    //Insert Anthony

    vector<string> Anthony({"Anthony", "Wu", "19", "CS", "Male"});
    People.insert_into(Anthony);

    //Insert person 1
    vector<string> person({"Person" , "Yeet", "24", "CS", "Attack Helicopter"});
    People.insert_into(person);

    People.insert_into({"John", "Doe", "23", "Biochemistry", "Male"});

    vector<string> person2({"Person", "Lee", "81", "Finance", "Female"});
    People.insert_into(person2);

    People.insert_into({"Person", "Smith", "16", "Neuroscience", "Male"});

    People.insert_into({"Andrew", "Wu", "27", "Nursing", "Male"});
    People.insert_into({"Rina", "Wu", "26", "Biology", "Female"});
    People.insert_into({"Tina", "Wu", "24", "Biology", "Female"});


    cout << "-----Table-----" << endl;
    cout << People.select_all() << endl << endl;


    cout << "-----Selecting First = Person-----" << endl;

    cout << "Selected Table: " << endl
         << People.select_conditional({"First", "=", "Person"}) << endl << endl;

    cout << "------Selecting Last = Wu-----" << endl;
    cout << People.select_conditional({"Last", "=", "Wu"}) << endl << endl;

    cout << "------Selecting All * --------" << endl;
    cout << People.select_all() << endl;

    cout << "------Selecting Gender = Female-----" << endl;
    cout << People.select_conditional({"Gender", "=", "Female"}) << endl << endl;

    cout << endl;

    cout << "-------COPY (Open existing) CTOR---------" << endl;
    Table copy("Students");
    cout << copy.select_all() << endl;


    cout << "-----END-----" << endl;

}

void simple_ptree_test()
{
    cout << "-----SIMPLE PTREE TEST-----" << endl;
    cout << "P1: " << endl;
    parse_tree p1("INSERT INTO employees VALUES \"Anthony Wu\", 19, Chef, "
                  "\"Michellin Star\"");
    //Need to drop the quotation marks when processing the strings

    p1.PTree().print_map();
    cout << endl;

    cout << "P2: " << endl;
    parse_tree p2("SeLect last from Students");


    p2.PTree().print_map();
    cout << endl;

    cout << "P3: " << endl;
    parse_tree p3("Select Last, First, age from People Where first < Anthony "
                  "aND City = \"Monterey Park\"");
    p3.PTree().print_map();

    cout << "P4: " << endl;
    parse_tree p4("Select * from Houses");
    p4.PTree().print_map();
    cout << endl;

    cout << "-----END-----" << endl;

}

void simple_token_test()
{
    cout << "-----SIMPLE TOKEN TEST-----" << endl;
    r_Token* tok = new r_Token(")");
    cout << "--MAIN--" << endl;
    cout << "Token: " << tok << endl;
    cout << "Type: " << tok->getType() << endl;
    cout << "Precedence: " << tok->getPrecedence() << endl;

    r_Token token("Yes");
    cout << tok->is_Valid() << endl;
}

//Parse Tree -> Tokenizer -> Shunting Yard -> RPN = Vector<Record Numbers>
void parser_test() //Hardcoded indices
{
    cout << "-----------------------------------------------------------"
         << endl;

    cout << " ----- INSTANTIATING DUMMY TABLE ----- " << endl;
    //Instantiate indices
    string record[3] = {"Anthony", "Wu", "19"};
    vector<MMap<string, long>> indices;
    //Indices
    for (int i = 0; i < 3; i++)
    {
        //Instantiate indices
        indices.push_back(MMap<string, long>());
        indices[i].insert(record[i], 1);
    }

    vector<string> Kobe = {"Kobe", "Bryant", "MAMBA"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Kobe[i], 2);
    }

    vector<string> Jordan = {"Michael", "Jordan", "GOAT"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Jordan[i], 3);
    }

    vector<string> Michael = {"Michael", "Bay", "EXPLOSION"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Michael[i], 4);
    }

    vector<string> Buble = {"Michael", "Buble", "45"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Buble[i], 5);
    }

    vector<string> Rina = {"Rina", "Wu", "26"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Rina[i], 6);
    }

    vector<string> AJ = {"Anthony", "Jordan", "23"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(AJ[i], 7);
    }

    vectorstr MB = {"Michael", "Bay", "55"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(MB[i], 8);
    }

    cout << ">>>Indices initialized: " << endl;
    string fields[3] = {"First", "Last", "Age"};
    for (int i = 0; i < 3; i++)
    {
        cout << ">>[" << fields[i] << "]<<" << endl;
        indices[i].print_map();
        cout << endl;
    }

    Map<string, size_t> fields_map;
    for (int i = 0; i < 3; i++)
    {
        fields_map.insert(fields[i], i);
    }

    cout << ">>>Field Maps initialized: " << endl << fields_map << endl;

    cout << "--------------------------------------------------------" << endl;

    char* command = "Select Last, First, age from People Where "
                    //Condition
                    "First = Michael or Last >= Jordan";

    cout << ">>> " << command << endl;

    cout << "--------------------------------------------------------" << endl;
    cout << " ----- Passing SQL Statement through Parser ----- " << endl;
    parse_tree p1(command);

    cout << "Parse Table: " << endl;
    MMap<string, string> ptree = p1.PTree();

    if (p1.success())
    {
        p1.PTree().print_map();

        cout << " ----- Tokenizer ----- " << endl;
        Tokenizer tokenizer(ptree["Condition"]);
        queue<r_Token*> infix = tokenizer.convert();
        cout << "Infix: " << endl;
        cout << infix << endl;

        cout << " ----- Shunting Yard ----- " << endl;

        //SHUNTING YARD TO CONVERT INTO POSTFIX EXPRESSION
        Shunting_Yard shunting_yard(infix);
        queue<r_Token *> postfix = shunting_yard.postfix();
        cout << "Postfix: " << endl;
        cout << postfix << endl;

        cout << " ----- RPN ------ " << endl;
        rpn r(postfix, indices, fields_map);
        vector<long> records = r.filter();

        cout << " ----- RECORD NUMBERS ----- " << endl;
        cout << spaced_left << "Record numbers: " << records << endl;
    }
    else
    {
        cout << "Command line failed" << endl;
    }
    cout << endl;

    cout << "------------------------------------------------" << endl;
}

void select_test()
{
    //Field List
    vector<string> field_names({"First", "Last", "Age", "Major", "Gender"});

    //Create and initialize table with fields
    Table People("Students", field_names);

    //Insert Anthony

    vector<string> Anthony({"Anthony", "Wu", "19", "CS", "Male"});
    People.insert_into(Anthony);

    //Insert person 1
    vector<string> person({"Person" , "Yeet", "24", "CS", "Attack Helicopter"});
    People.insert_into(person);

    People.insert_into({"John", "Doe", "23", "Biochemistry", "Male"});

    vector<string> person2({"Person", "Lee", "81", "Finance", "Female"});
    People.insert_into(person2);

    People.insert_into({"Person", "Smith", "16", "Neuroscience", "Male"});

    People.insert_into({"Andrew", "Wu", "27", "Nursing", "Male"});
    People.insert_into({"Rina", "Wu", "26", "Biology", "Female"});
    People.insert_into({"Tina", "Wu", "24", "Biology", "Female"});


    cout << "-----Table-----" << endl;
    cout << People.select_all() << endl << endl;

    cout << "---------------" << endl;

    char* command = "Select First, Last from People Where "
                    //Condition
                    "First = John or Last = Wu";

    cout << ">>> " << command << endl;

    cout << "--------------------------------------------------------" << endl;
    cout << " ----- Passing SQL Statement through Parser ----- " << endl;
    parse_tree p1(command);

    cout << "Parse Table: " << endl;
    MMap<string, string> ptree = p1.PTree();

    if (p1.success())
    {
       cout << "--Command is valid--" << endl;
       ptree.print_map();
       cout << "Select : " << endl;
       cout << People.select(ptree["Fields"], ptree["Condition"]) << endl;
    }
    else
    {
        cout << "Command line failed" << endl;
    }

    cout << People.indices << endl;

}

//Tokenizer
void simple_tokenize_test()
{
    cout << "------SIMPLE TOKENIZE-----" << endl;
    char* command = "Select Last, First, age from People Where (first <="
                    " Anthony) aND (City = \"Monterey Park\")";

            //= "Select Last, First, age from People where first < Anthony and "
            //  "city = \"Monterey Park\"";
    cout << ">>> " << command << endl;

    cout << "------------------------" << endl;
    parse_tree p1(command);


    cout << "Parse Table: " << endl;
    MMap<string, string> ptree = p1.PTree();
    if (p1.success())
    {
        p1.PTree().print_map();

        cout << "ptree[Command]: " << ptree["Command"] << endl;

        Tokenizer tokenizer(ptree["Condition"]);
        cout << tokenizer.convert() << endl;
    }
    else
    {
        cout << "Command line failed" << endl;
    }
    cout << endl;

    cout << "------------------------------------------------" << endl;
}
//Shunting Yard
void quick_shunting_yard_test()
{
    char* command = "Select Last, First, age from People Where last = jones "
                    "and first = bob or age = 20";

            //= "Select Last, First, age from People where first < Anthony and "
            //  "city = \"Monterey Park\"";
    cout << ">>> " << command << endl;

    cout << "------------------------" << endl;
    parse_tree p1(command);


    cout << "Parse Table: " << endl;
    MMap<string, string> ptree = p1.PTree();
    if (p1.success())
    {
        p1.PTree().print_map();
        cout << "------------------------" << endl;

        Tokenizer tokenizer(ptree["Condition"]);
        queue<r_Token*> infix = tokenizer.convert();
        cout << "Infix: " << endl;
        cout << infix << endl;

        //SHUNTING YARD TO CONVERT INTO POSTFIX EXPRESSION
        Shunting_Yard shunting_yard(infix);
        cout << "Postfix: " << endl;
        cout << shunting_yard.postfix() << endl;
    }
    else
    {
        cout << "Command line failed" << endl;
    }
    cout << endl;
}
//RPN
void rpn_filter_test()
{
    cout << "-----RPN HARD CODED PROCESS TEST-----" << endl;

    //Instantiate indices
    string record[3] = {"Anthony", "Wu", "19"};
    vector<MMap<string, long>> indices;
    //Indices
    for (int i = 0; i < 3; i++)
    {
        //Instantiate indices
        indices.push_back(MMap<string, long>());
        indices[i].insert(record[i], 1);
    }

    vector<string> Kobe = {"Kobe", "Bryant", "MAMBA"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Kobe[i], 2);
    }

    vector<string> Jordan = {"Michael", "Jordan", "GOAT"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Jordan[i], 3);
    }

    vector<string> Michael = {"Michael", "Bay", "EXPLOSION"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Michael[i], 4);
    }

    vector<string> Buble = {"Michael", "Buble", "45"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Buble[i], 5);
    }

    vector<string> Rina = {"Rina", "Wu", "26"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Rina[i], 6);
    }

    vector<string> AJ = {"Anthony", "Jordan", "23"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(AJ[i], 7);
    }

    vectorstr MB = {"Michael", "Bay", "55"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(MB[i], 8);
    }

    cout << ">>>Indices initialized: " << endl;
    for (int i = 0; i < 3; i++)
    {
        indices[i].print_map();
        cout << endl;
    }

    string fields[3] = {"First", "Last", "Age"};
    Map<string, size_t> fields_map;
    for (int i = 0; i < 3; i++)
    {
        fields_map.insert(fields[i], i);
    }


    cout << ">>>Field Maps initialized: " << endl << fields_map << endl;

    //Comands
    queue<r_Token*> q;
    q.push(new r_Token("First")); //Field
    q.push(new r_Token("Michael"));     //Value
    q.push(new r_Token("="));
    q.push(new r_Token("Last"));  //Field
    q.push(new r_Token("Bay"));
    q.push(new r_Token("="));
    q.push(new r_Token("and"));

    cout << ">>>Queue of tokens initialized: " << q << endl;

    cout << "---Initializing rpn object---" << endl;
    rpn r(q, indices, fields_map);


    vector<long> records = r.filter();

    cout << spaced_left << "Record numbers: " << records << endl;
}

void bare_minimum()
{
    //BARE MINIMUM MAIN
    Table students("student", {"First", "last", "Age", "Year"});

    students.insert_into({"Anthony", "Wu", "19", "2000"});


    parse_tree p("select last from student"); //need to implement this select
                                                //function

    MMap<string, string> ptree = p.PTree();

    if (ptree["Command"][0] == "select")
    {
        string table_name = ptree["Table"][0];
        Table t(table_name);

        ptree.print_map();
        cout << "PTree[Fields]: " << ptree["Fields"] << endl;
        //ptree["Fields"] returns last
        //So this is equivalent to t.select("last")
        //cout << t.select(ptree["Fields"]) << endl;
    }
}


//Private member function - was public for this test
/*
void rpn_process_test()
{
    cout << "-----RPN HARD CODED PROCESS TEST-----" << endl;

    //Instantiate indices
    string record[3] = {"Anthony", "Wu", "19"};
    vector<MMap<string, long>> indices;
    for (int i = 0; i < 3; i++)
    {
        indices.push_back(MMap<string, long>());
        indices[i].insert(record[i], 1);
    }

    vector<string> Kobe = {"Kobe", "Bryant", "MAMBA"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Kobe[i], 2);
    }

    vector<string> Jordan = {"Michael", "Jordan", "GOAT"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Jordan[i], 3);
    }

    vector<string> Michael = {"Michael", "Bay", "EXPLOSION"};
    for (int i = 0; i < 3; i++)
    {
        indices[i].insert(Michael[i], 4);
    }

    cout << ">>>Indices initialized: " << endl;
    for (int i = 0; i < 3; i++)
    {
        indices[i].print_map();
        cout << endl;
    }

    string fields[3] = {"First", "Last", "Age"};
    Map<string, size_t> fields_map;
    for (int i = 0; i < 3; i++)
    {
        fields_map.insert(fields[i], i);
    }

    cout << ">>>Field Maps initialized: " << endl << fields_map << endl;

    queue<r_Token*> q;
    q.push(new r_Token("Last"));
    q.push(new r_Token("Wu"));
    q.push(new r_Token("<="));

    cout << ">>>Queue of tokens initialized: " << q << endl;

    cout << "---Initializing rpn object---" << endl;
    rpn r(q, indices, fields_map);

    cout << "---Processing---" << endl;
    r_Token* a = q.pop(); //Last
    r_Token* b = q.pop(); //Wu
    r_Token* c = q.pop(); //=

    vector<long> records = r._process(a, b, c);

    cout << "Record numbers: " << records << endl;
}
*/
