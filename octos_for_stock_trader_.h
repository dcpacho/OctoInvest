#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<math.h>
#pragma once

#ifndef PI
    #define PI 3.14159265358979323846f
#endif

#define octo0 \
    (Octo){0,0,0,0,0,0,0,0 \
}

#define OAqum( old , x , X) (\
    (Octo){  x.r*X.r - x.a*X.a - x.b*X.b - x.c*X.c - x.d*X.d - x.e*X.e - x.f*X.f - x.g*X.g + old.r , \
                    x.r*X.a + x.a*X.r + x.b*X.c - x.c*X.b + x.d*X.e - x.e*X.d - x.f*X.g + x.g*X.f + old.a , \
                    x.r*X.b - x.a*X.c + x.b*X.r + x.c*X.a + x.d*X.f + x.e*X.g - x.f*X.d - x.g*X.e + old.b , \
                    x.r*X.c + x.a*X.b - x.b*X.a + x.c*X.r + x.d*X.g - x.e*X.f + x.f*X.e - x.g*X.d + old.c , \
                    x.r*X.d - x.a*X.e - x.b*X.f - x.c*X.g - x.d*X.r + x.e*X.a + x.f*X.b + x.g*X.c + old.d , \
                    x.r*X.e + x.a*X.d + x.b*X.g + x.c*X.f - x.d*X.a + x.e*X.r - x.f*X.c + x.g*X.b + old.e , \
                    x.r*X.f + x.a*X.g + x.b*X.d - x.c*X.e - x.d*X.b + x.e*X.c + x.f*X.r - x.g*X.a + old.f , \
                    x.r*X.g - x.a*X.f + x.b*X.e + x.c*X.d - x.d*X.c - x.e*X.b + x.f*X.a + x.g*X.r + old.g } \
)

#define OADD( x , X ) (Octo){x.r + X.r, x.a + X.a, x.b + X.b,x.c + X.c, x.d + X.d, x.e + X.e, x.f + X.f, x.g + X.g}

#define ODYDX ( Y_P_DX , Y_M_DX , D_X )\
    (Octo){ \
        (Y_P_DX.r - Y_M_DX.r) / (D_X * 2) , \
        (Y_P_DX.a - Y_M_DX.a) / (D_X * 2) , \
        (Y_P_DX.b - Y_M_DX.b) / (D_X * 2) , \
        (Y_P_DX.c - Y_M_DX.c) / (D_X * 2) , \
        (Y_P_DX.d - Y_M_DX.d) / (D_X * 2) , \
        (Y_P_DX.e - Y_M_DX.e) / (D_X * 2) , \
        (Y_P_DX.f - Y_M_DX.f) / (D_X * 2) , \
        (Y_P_DX.g - Y_M_DX.g) / (D_X * 2) \
}

#define ODYDYDXDX ( Y_P_DX , Y_OG , Y_M_DX , D_X )\
    (Octo){ \
        ((Y_P_DX.r + Y_M_DX.r)-(2*Y_OG)) / (D_X * D_X) , \
        ((Y_P_DX.a + Y_M_DX.a)-(2*Y_OG)) / (D_X * D_X) , \
        ((Y_P_DX.b + Y_M_DX.b)-(2*Y_OG)) / (D_X * D_X) , \
        ((Y_P_DX.c + Y_M_DX.c)-(2*Y_OG)) / (D_X * D_X) , \
        ((Y_P_DX.d + Y_M_DX.d)-(2*Y_OG)) / (D_X * D_X) , \
        ((Y_P_DX.e + Y_M_DX.e)-(2*Y_OG)) / (D_X * D_X) , \
        ((Y_P_DX.f + Y_M_DX.f)-(2*Y_OG)) / (D_X * D_X) , \
        ((Y_P_DX.g + Y_M_DX.g)-(2*Y_OG)) / (D_X * D_X) \
}

#define OPartSum( x )\
    ( double ){ \
        x.r + x.a + x.b + x.c + x.d + x.e + x.f + x.g \
} 

#define SHIFTIN( array,  input)\
    array.g=array.f;\
    array.f=array.e;\
    array.e=array.d;\
    array.d=array.c;\
    array.c=array.b;\
    array.b=array.a;\
    array.a=array.r;\
    array.r=input.price;


#define UPDATE_HOLDINGS( term )\
    for( int INVESTOR = 0 ; INVESTOR < total_investors ; INVESTOR++ ){\
        if (( cash[INVESTOR] >= 0.01 ) && (buy_sell[INVESTOR][term] > 0.00001) ) {\
            cash[INVESTOR] -= 100.0*buy_sell[INVESTOR][term];\
            stock[ term ].owners[INVESTOR] +=buy_sell[INVESTOR][term]/stock[ term ].price; }\
        else if ((stock[ term ].owners[INVESTOR] > 0.03 ) && (buy_sell[INVESTOR][term] < -0.00001)){\
            cash[INVESTOR] -= 100.0*buy_sell[INVESTOR][term];\
            stock[ term ].owners[INVESTOR] +=buy_sell[INVESTOR][term]/stock[ term ].price ; }\
    }

#define LIQUIDATE_HOLDINGS( term )\
    for( int INVESTOR = 0 ; INVESTOR < total_investors ; INVESTOR++ ){ \
        while ((stock[ term ].owners[INVESTOR] > 0.03 )){\
            cash[INVESTOR] += 0.001; \
            stock[ term ].owners[INVESTOR] -=0.01/stock[ term ].price ; }\
    }printf("liquidated%d",term);

#define APPLY_CHANGE_U(WEIGHT, DY_DX , DYDY_DXDX , PRAM_POS_DYDY , PRAM_NEG_DYDY, STEP_SIZE )\
    (DYDY_DXDX.r<0)?(WEIGHT.r-=DY_DX.r*STEP_SIZE/(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.r))):(WEIGHT.r+=DY_DX.r*STEP_SIZE*(1.0+(PRAM_POS_DYDY*DYDY_DXDX.r)));\
    (DYDY_DXDX.a<0)?(WEIGHT.a-=DY_DX.a*STEP_SIZE/(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.a))):(WEIGHT.a+=DY_DX.a*STEP_SIZE*(1.0+(PRAM_POS_DYDY*DYDY_DXDX.a)));\
    (DYDY_DXDX.b<0)?(WEIGHT.b-=DY_DX.b*STEP_SIZE/(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.b))):(WEIGHT.b+=DY_DX.b*STEP_SIZE*(1.0+(PRAM_POS_DYDY*DYDY_DXDX.b)));\
    (DYDY_DXDX.c<0)?(WEIGHT.c-=DY_DX.c*STEP_SIZE/(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.c))):(WEIGHT.c+=DY_DX.c*STEP_SIZE*(1.0+(PRAM_POS_DYDY*DYDY_DXDX.c)));\
    (DYDY_DXDX.d<0)?(WEIGHT.d-=DY_DX.d*STEP_SIZE/(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.d))):(WEIGHT.d+=DY_DX.d*STEP_SIZE*(1.0+(PRAM_POS_DYDY*DYDY_DXDX.d)));\
    (DYDY_DXDX.e<0)?(WEIGHT.e-=DY_DX.e*STEP_SIZE/(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.e))):(WEIGHT.e+=DY_DX.e*STEP_SIZE*(1.0+(PRAM_POS_DYDY*DYDY_DXDX.e)));\
    (DYDY_DXDX.f<0)?(WEIGHT.f-=DY_DX.f*STEP_SIZE/(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.f))):(WEIGHT.f+=DY_DX.f*STEP_SIZE*(1.0+(PRAM_POS_DYDY*DYDY_DXDX.f)));\
    (DYDY_DXDX.g<0)?(WEIGHT.g-=DY_DX.g*STEP_SIZE/(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.g))):(WEIGHT.g+=DY_DX.g*STEP_SIZE*(1.0+(PRAM_POS_DYDY*DYDY_DXDX.g)));

#define APPLY_CHANGE_n(WEIGHT, DY_DX , DYDY_DXDX , PRAM_POS_DYDY , PRAM_NEG_DYDY , STEP_SIZE )\
    (DYDY_DXDX.r<0)?(WEIGHT.r-=DY_DX.r*STEP_SIZE*(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.r))):(WEIGHT.r+=DY_DX.r*STEP_SIZE/(1.0+(PRAM_POS_DYDY*DYDY_DXDX.r)));\
    (DYDY_DXDX.a<0)?(WEIGHT.a-=DY_DX.a*STEP_SIZE*(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.a))):(WEIGHT.a+=DY_DX.a*STEP_SIZE/(1.0+(PRAM_POS_DYDY*DYDY_DXDX.a)));\
    (DYDY_DXDX.b<0)?(WEIGHT.b-=DY_DX.b*STEP_SIZE*(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.b))):(WEIGHT.b+=DY_DX.b*STEP_SIZE/(1.0+(PRAM_POS_DYDY*DYDY_DXDX.b)));\
    (DYDY_DXDX.c<0)?(WEIGHT.c-=DY_DX.c*STEP_SIZE*(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.c))):(WEIGHT.c+=DY_DX.c*STEP_SIZE/(1.0+(PRAM_POS_DYDY*DYDY_DXDX.c)));\
    (DYDY_DXDX.d<0)?(WEIGHT.d-=DY_DX.d*STEP_SIZE*(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.d))):(WEIGHT.d+=DY_DX.d*STEP_SIZE/(1.0+(PRAM_POS_DYDY*DYDY_DXDX.d)));\
    (DYDY_DXDX.e<0)?(WEIGHT.e-=DY_DX.e*STEP_SIZE*(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.e))):(WEIGHT.e+=DY_DX.e*STEP_SIZE/(1.0+(PRAM_POS_DYDY*DYDY_DXDX.e)));\
    (DYDY_DXDX.f<0)?(WEIGHT.f-=DY_DX.f*STEP_SIZE*(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.f))):(WEIGHT.f+=DY_DX.f*STEP_SIZE/(1.0+(PRAM_POS_DYDY*DYDY_DXDX.f)));\
    (DYDY_DXDX.g<0)?(WEIGHT.g-=DY_DX.g*STEP_SIZE*(-1.0+(PRAM_NEG_DYDY * DYDY_DXDX.g))):(WEIGHT.g+=DY_DX.g*STEP_SIZE/(1.0+(PRAM_POS_DYDY*DYDY_DXDX.g)));


typedef struct Octo {
    double r;
    double a;
    double b;
    double c;
    double d;
    double e;
    double f;
    double g;} Octo;

void OSaveNet(  int* depthAndShape , double Range , Octo* weights );
Octo * ONewWeights(int * depthAndShape);
Octo * OLoadNet(  int* depthAndShape , double Range );
Octo ** ONeroNet(double Range , int * depthAndShape , Octo ** InOut , Octo * Weights );
int * ONewDepthAndShape(int depth, ...);
Octo ** ONewInOutArray(int * DepthAndShape);
Octo OSquash(Octo x , double maxSize);
void softMaxO( Octo* outputarray , double* probArray , int outCount );
