#ifdef  _WIN32
#include <windows.h>
#include <stdio.h>
#include <winnt.h>
#include <winreg.h>
#define PIPE_NAME "\\\\.\\pipe\\ -<{( Hit This Pipe )}>-"
typedef struct Stock { double price; double * owners; } Stock ;    
HANDLE  Attach_Python_Pipe(void);
int Get_Prices_From_Python( Stock * received_price, int total_stocks);
int Get_Total_Stocks( int total_stocks);
#endif
