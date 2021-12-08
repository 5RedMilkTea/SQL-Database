TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    ../../Reusables/StringTokenizer/String_Tokenizer/stokenizer.cpp \
    ../../Reusables/StringTokenizer/String_Tokenizer/table_config.cpp \
    ../../Reusables/StringTokenizer/String_Tokenizer/token.cpp \
    r_token.cpp \
    debug_format.cpp \
    file_functions.cpp \
    parse_tree.cpp \
    state_machine_config.cpp \
    record.cpp \
    rpn.cpp \
    shunting_yard.cpp \
    sql.cpp \
    table.cpp \
    tokenizer.cpp \
    ../../Reusables/FileTokenizer/file_Tokenizer/ftokenizer.cpp


HEADERS += \
    ../../Reusables/VectorClass/VectorClass/vector.h \
    ../../B Tree/bplus_multimap/mmap.h \
    ../../B Tree/bplusmap/map.h \
    ../../Reusables/StringTokenizer/String_Tokenizer/stokenizer.h \
    ../../Reusables/StringTokenizer/String_Tokenizer/table_config.h \
    ../../Reusables/StringTokenizer/String_Tokenizer/token.h \
    r_token.h \
    debug_format.h \
    file_functions.h \
    parse_tree.h \
    state_machine_config.h \
    record.h \
    ../../Reusables/IteratedQueue/queue/queue.h \
    ../../Reusables/IteratedStack/stack/stack.h \
    rpn.h \
    shunting_yard.h \
    sql.h \
    table.h \
    tokenizer.h \
    ../../Reusables/FileTokenizer/file_Tokenizer/ftokenizer.h \
    z_workreport.h
