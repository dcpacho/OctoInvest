#include "octos_for_stock_trader_.h"
#include "pipe_stock_price_from_python_.h"

#define LAYERS 4
#define PRAM_POS_DYDY 0.0001
#define PRAM_NEG_DYDY 0.0001
#define DT 0.1
#define STEP_SIZE 0.00007
#define START_CASH 1000.0
#define START_STOCK 1.0
HANDLE pipe;
DWORD dwRead=0;

int main() {
    
    int total_stocks=0;
    
    pipe=Attach_Python_Pipe();
    
    total_stocks=Get_Total_Stocks(total_stocks);
    int total_investors = 16*total_stocks*total_stocks*(LAYERS+1)+1;
    printf("Total Investors: %d\n", total_investors);
    Stock  * stock = (Stock *)malloc(sizeof(Stock) * total_stocks );
    for (int i=0; i<total_stocks; i++) {
        stock[i].owners= (double *)malloc(sizeof(double) * total_investors);
        for (int j=0; j<total_investors; j++) {stock[i].owners[j]=START_STOCK;}
    }
    
    double  * *  buy_sell=(double * * )malloc(sizeof(double * )*total_investors);
    
    double * cash=malloc(sizeof(double)*total_investors);
    
    for(int i=0;i<total_investors;i++){ 
        cash[i]=START_CASH; 
        buy_sell[i]= (double *)malloc(sizeof(double) * total_stocks );
        for(int j=0;j<total_stocks;j++){ 
            buy_sell[i][j]=0.0; 
        }

    }

    int wc=0;

    int * DnS = ONewDepthAndShape(LAYERS,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks,total_stocks);
    
    Octo * wts=OLoadNet(DnS, 100.0);
    
    Octo ** ioarray=ONewInOutArray(DnS);
    
    Octo * input = malloc(sizeof(Octo)*total_stocks);
    
    Octo first_dif = octo0, second_dif = octo0, applied_change = octo0;
    for(int j=0;j<8;j++){
        if(Get_Prices_From_Python(stock,total_stocks)){
            for (int i=0; i<total_stocks; i++){
                SHIFTIN(input[i],stock[i])
            }
        
        }
    
    }

    while(Get_Prices_From_Python(stock, total_stocks)){
        for(int i=0;i<total_stocks;i++){
            SHIFTIN(input[i], stock[i])
        }

        for(int i=0;i<total_stocks;i++){
            ioarray[0][i]=input[i];
        }

        ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
        
        softMaxO(ioarray[1], buy_sell[total_investors-1], total_stocks);
        
        for(wc=0;wc<(total_investors-1)/16;wc++){
            wts[wc].r+=DT;
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[wc*16], total_stocks);
            wts[wc].r-=DT*2.0;//       r
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[1+(wc*16)], total_stocks);
            wts[wc].r+=DT; wts[wc].a+=DT; 
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[2+(wc*16)], total_stocks);
            wts[wc].a-=DT*2.0;//     a
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[3+(wc*16)], total_stocks);
            wts[wc].a+=DT; wts[wc].b+=DT; 
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[4+(wc*16)], total_stocks);
            wts[wc].b-=DT*2.0;//       b
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[5+(wc*16)], total_stocks);
            wts[wc].b+=DT; wts[wc].c+=DT; 
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[6+(wc*16)], total_stocks);
            wts[wc].c-=DT*2.0;//       c
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[7+(wc*16)], total_stocks);
            wts[wc].c+=DT; wts[wc].d+=DT; 
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[8+(wc*16)], total_stocks);
            wts[wc].d-=DT*2.0;//       d
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[9+(wc*16)], total_stocks);
            wts[wc].d+=DT; wts[wc].e+=DT; 
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[10+(wc*16)], total_stocks);
            wts[wc].e-=DT*2.0;//       e
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[11+(wc*16)], total_stocks);
            wts[wc].e+=DT; wts[wc].f+=DT; 
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[12+(wc*16)], total_stocks);
            wts[wc].f-=DT*2.0;//       f
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[13+(wc*16)], total_stocks);
            wts[wc].f+=DT; wts[wc].g+=DT; 
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[14+(wc*16)], total_stocks);
            wts[wc].g-=DT*2.0;//       g
            for(int i=0;i<total_stocks;i++){ioarray[0][i]=input[i];}       
            ioarray=ONeroNet( 100.0,DnS,ioarray,wts);
            softMaxO(ioarray[1], buy_sell[15+(wc*16)], total_stocks);
            wts[wc].g+=DT; 
        }

        for(int i=0;i<total_stocks;i++){ 
            //printf("buy_sell[INVESTOR%d][STOCK%d]=%lf  %lf\n",(total_investors-1),i,buy_sell[(total_investors-1)][i], cash[(total_investors-1)]);
             UPDATE_HOLDINGS(i)
        }
    
    }
    free(input);free(ioarray[0]);free(ioarray[1]); free(ioarray);
    for(int i=0;i<total_investors;i++){free(buy_sell[i]);}free(buy_sell);
    CloseHandle(pipe); printf("Pipe disconnected.\n"); 

    for (int j=0; j<total_stocks; j++) {LIQUIDATE_HOLDINGS(j)}
    for (int i=0; i<total_stocks; i++) {free(stock[i].owners);}free(stock);
    for (int i=0; i<total_investors; i++) {printf("Investor %d has cash: %lf\n", i, cash[i]);}


    for (wc=0; wc<(total_investors-1)/16; wc++) {
        first_dif=(Octo){(cash[wc*16] - cash[(wc*16)+1]) / ( DT * 2) ,
                            (cash[(wc*16)+2] - cash[(wc*16)+3]) / ( DT * 2) ,
                            (cash[(wc*16)+4] - cash[(wc*16)+5]) / ( DT * 2) ,
                            (cash[(wc*16)+6] - cash[(wc*16)+7]) / ( DT * 2) ,
                            (cash[(wc*16)+8] - cash[(wc*16)+9]) / ( DT * 2) ,
                            (cash[(wc*16)+10] - cash[(wc*16)+11]) / ( DT * 2) ,
                            (cash[(wc*16)+12] - cash[(wc*16)+13]) / ( DT * 2) ,
                            (cash[(wc*16)+14] - cash[(wc*16)+15]) / ( DT * 2)
        };
        
        second_dif=(Octo){((cash[wc*16]+ cash[(wc*16)+1])-(2*cash[total_investors-1])) / ( DT * DT ),
                                ((cash[(wc*16)+2]+ cash[(wc*16)+3])-(2*cash[total_investors-1])) / ( DT * DT ),
                                ((cash[(wc*16)+4]+ cash[(wc*16)+5])-(2*cash[total_investors-1])) / ( DT * DT ),
                                ((cash[(wc*16)+6]+ cash[(wc*16)+7])-(2*cash[total_investors-1])) / ( DT * DT ),
                                ((cash[(wc*16)+8]+ cash[(wc*16)+9])-(2*cash[total_investors-1])) / ( DT * DT ),
                                ((cash[(wc*16)+10]+ cash[(wc*16)+11])-(2*cash[total_investors-1])) / ( DT * DT ),
                                ((cash[(wc*16)+12]+ cash[(wc*16)+13])-(2*cash[total_investors-1])) / ( DT * DT ),
                                ((cash[(wc*16)+14]+ cash[(wc*16)+15])-(2*cash[total_investors-1])) / ( DT * DT )
        };
        
        applied_change = octo0;
        APPLY_CHANGE_U(applied_change, first_dif, second_dif, PRAM_POS_DYDY, PRAM_NEG_DYDY , STEP_SIZE )
        printf("\
Investor %d\nfirst dif: %.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\nsecond dif: %.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n\
Old weight: %.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\nApplyed change: %lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n",
            wc,
            first_dif.r, first_dif.a, first_dif.b, first_dif.c, first_dif.d, first_dif.e, first_dif.f, first_dif.g,
            second_dif.r, second_dif.a, second_dif.b, second_dif.c, second_dif.d, second_dif.e, second_dif.f, second_dif.g,
            wts[wc].r, wts[wc].a, wts[wc].b, wts[wc].c, wts[wc].d, wts[wc].e, wts[wc].f, wts[wc].g,
            applied_change.r, applied_change.a, applied_change.b, applied_change.c, applied_change.d, applied_change.e, applied_change.f, applied_change.g
        );

        wts[wc]=OADD(wts[wc], applied_change);
        printf("NeW weight: %.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",wts[wc].r, wts[wc].a, wts[wc].b, wts[wc].c, wts[wc].d, wts[wc].e, wts[wc].f, wts[wc].g);

    }

    free(cash);   
    
    OSaveNet(DnS, 100.0, wts);   

    free(wts); 
    free(DnS);
    return 0;
}