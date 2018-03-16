#include <rpc/rpc.h>
#define DATE_PROG 0x31234567
#define DATE_VERS 1
#define BIN_DATE 1
extern  long * bin_date_1();
extern  long * bin_date_1_svc();
#define STR_DATE 2
extern  char ** str_date_1();
extern  char ** str_date_1_svc();
extern int date_prog_1_freeresult ();


