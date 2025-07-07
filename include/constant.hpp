#if defined(_WIN32) || defined(_WIN64)
#define PATH_SEP "\\"
#define BACK_DIR "..\\"
#else
#define BACK_DIR "../"
#define PATH_SEP "/"
#endif


#define DUMMY BACK_DIR "data" PATH_SEP "dummy.xml"
#define JMDICT_XML BACK_DIR "data" PATH_SEP "JMdict"
#define JMDICT_E_XML BACK_DIR "data" PATH_SEP "JMdict_e_examp"
#define DATA_KANKEN_1 BACK_DIR "data" PATH_SEP "kanken_1.txt"
#define DATA_KANKEN_1_5 BACK_DIR "data" PATH_SEP "kanken_1.5.txt"
#define DATA_KANKEN_2 BACK_DIR "data" PATH_SEP "kanken_2.txt"
#define DATA_KANKEN_2_5 BACK_DIR "data" PATH_SEP "kanken_2.5.txt"
#define DATA_KANKEN_3 BACK_DIR "data" PATH_SEP "kanken_3.txt"
#define DATA_KANKEN_4 BACK_DIR "data" PATH_SEP "kanken_4.txt"
#define DATA_KANKEN_5 BACK_DIR "data" PATH_SEP "kanken_5.txt"
#define DATA_KANKEN_6 BACK_DIR "data" PATH_SEP "kanken_6.txt"
#define DATA_KANKEN_7 BACK_DIR "data" PATH_SEP "kanken_7.txt"
#define DATA_KANKEN_8 BACK_DIR "data" PATH_SEP "kanken_8.txt"
#define DATA_KANKEN_9 BACK_DIR "data" PATH_SEP "kanken_9.txt"
#define DATA_KANKEN_10 BACK_DIR "data" PATH_SEP "kanken_10.txt"


#define MODE_JMDICT_CREATE 0
#define MODE_EXAMPLE_EXTRACT 1