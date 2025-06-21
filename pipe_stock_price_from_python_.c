#include "pipe_stock_price_from_python_.h"

#ifdef  _WIN32

extern  HANDLE pipe;
extern DWORD dwRead;
    
HANDLE  Attach_Python_Pipe(void) {
    pipe= CreateFile(
        PIPE_NAME,
        GENERIC_READ,
        /* sharing*/0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );
    if (pipe== INVALID_HANDLE_VALUE){
        printf("Error connecting pipe. Error code: %ld\n",
             GetLastError());
        exit(1);
    }
    printf("Connected to pipe!\n");
    return pipe;
}

int Get_Prices_From_Python(
    Stock * received_price,
     int total_stocks
    ) {
    for (int stock=0; stock<total_stocks; stock++) {
        if(
            ReadFile(
                pipe,
                &received_price[stock].price,
                sizeof(double),
                &dwRead,
                NULL)
                && 
                dwRead == sizeof(double)
            ){
            printf("Received: %.8f\n",
                        received_price[stock].price);
        } else {
            printf("Error reading pipe. Error code: %ld\n",
                GetLastError());
            return 0;
        }
    }
    return 1;
}

int Get_Total_Stocks(
     int total_stocks
    ) {
        if(
            ReadFile(
                pipe,
                &total_stocks,
                sizeof(int),
                &dwRead,
                NULL)
                && 
                dwRead == sizeof(int)
            ){
            printf("Received: %d\n",
                        total_stocks);
        } else {
            printf("Error reading pipe. Error code: %ld\n",
                GetLastError());
            exit(1);
        }
    return total_stocks;}

#endif
