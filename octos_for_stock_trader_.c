#include"octos_for_stock_trader_.h"

Octo OSquash(Octo x , double maxSize){
    maxSize=maxSize*2.0;
    return(Octo){(maxSize/PI)*atan((PI/maxSize)*x.r) , (maxSize/PI)*atan((PI/maxSize)*x.a) ,
                            (maxSize/PI)*atan((PI/maxSize)*x.b) , (maxSize/PI)*atan((PI/maxSize)*x.c) ,
                            (maxSize/PI)*atan((PI/maxSize)*x.d) , (maxSize/PI)*atan((PI/maxSize)*x.e) ,
                            (maxSize/PI)*atan((PI/maxSize)*x.f) , (maxSize/PI)*atan((PI/maxSize)*x.g)};
}

Octo ** ONewInOutArray(int * DepthAndShape){
    int max=0;
    for (int i = 1; i < (DepthAndShape[0]+3); i++){if(max<DepthAndShape[i]){max=DepthAndShape[i];}}
    Octo **InOut=malloc(sizeof(Octo*)*2);
    InOut[0]=malloc(sizeof(Octo)*max);
    InOut[1]=malloc(sizeof(Octo)*max);
    return InOut;
}

int* ONewDepthAndShape(int depth, ...){
    va_list layer;
    va_start(layer, depth);
    int *depthAndShape=malloc(sizeof(int)*(depth+3)); 
    depthAndShape[0]=depth;
    for (int i = 1; i < depth+3; i++){ depthAndShape[i]=va_arg(layer , int); }
    va_end(layer);
    return depthAndShape;
}

Octo * ONewWeights(int * depthAndShape){
    srand(time(NULL));
    int totalWeigths=0;
    for (int i = 0; i < depthAndShape[0]+1; i++){ totalWeigths = totalWeigths + depthAndShape[i+1] * depthAndShape[i+2];
    printf("\nweights in L%d = %d\n",i,depthAndShape[i+1] * depthAndShape[i+2]); 
    }
    printf("\n\ntotal weights %d\n\n",totalWeigths);
    Octo * weightsArray=malloc(sizeof(Octo)*(totalWeigths));
    for (int i = 0; i < totalWeigths; i++){
        weightsArray[i]=(Octo){
                                    (double)rand()/RAND_MAX*1.5-0.75 , (double)rand()/RAND_MAX*1.5-0.75 ,
                                    (double)rand()/RAND_MAX*1.5-0.75 , (double)rand()/RAND_MAX*1.5-0.75 ,
                                    (double)rand()/RAND_MAX*1.5-0.75 , (double)rand()/RAND_MAX*1.5-0.75 ,
                                    (double)rand()/RAND_MAX*1.5-0.75 , (double)rand()/RAND_MAX*1.5-0.75 };
    }
    return weightsArray;
}

Octo ** ONeroNet(double Range , int * depthAndShape , Octo ** InOut , Octo * Weights ){
    int used = 0; int a=0; int b=1;
    //printf("\nstill working");
    for (int i = 0; i < depthAndShape[0]+1; i++){ 
        for (int j = 0; j < depthAndShape[i+2]; j++){ InOut[b][j] = octo0;
            for (int k = 0; k < depthAndShape[i+1]; k++){ InOut[b][j] = OAqum( InOut[b][j] , InOut[a][k] , Weights[used] ); used++; } 
            InOut[b][j] = OSquash( InOut[b][j] , Range );     
        }
        b=a; a=(a+1)%2; 
    }
    if ((depthAndShape[0]%2)){
        Octo * buf=InOut[0];
        InOut[0]=InOut[1];
        InOut[1]=buf;}
    return InOut;
}

void OSaveNet(  int* depthAndShape , double Range , Octo* weights ){
    char file_name_buff[100];
    sprintf(file_name_buff,"investor_%lf_%d_%d_.oct\0", Range, depthAndShape[0], depthAndShape[1]);
    FILE * f=fopen(file_name_buff,"wb");
    int totalWeigths=0;
    for (int i = 0; i < depthAndShape[0]+1; i++){ totalWeigths = totalWeigths + depthAndShape[i+1] * depthAndShape[i+2]; }
    fwrite(weights,sizeof(Octo),totalWeigths,f);
    fclose(f);
}

Octo * OLoadNet(  int* depthAndShape , double Range ){
    char file_name_buff[100];
    sprintf(file_name_buff,"investor_%lf_%d_%d_.oct\0", Range, depthAndShape[0], depthAndShape[1]);
    FILE * f=fopen(file_name_buff,"rb");
    if ( f != NULL ) {
        int totalWeigths=0;
        for (int i = 0; i < depthAndShape[0]+1; i++){ totalWeigths = totalWeigths + depthAndShape[i+1] * depthAndShape[i+2]; }
        Octo * weights=(Octo * )malloc(sizeof(Octo)*(totalWeigths));
        fread(weights,sizeof(Octo),totalWeigths,f);
        fclose(f);
        return weights;
    }else {printf("\n\nError opening file %s\n\n",file_name_buff);return ONewWeights(depthAndShape);}
}

void softMaxO( Octo* outputarray , double* probArray , int outCount ){
    double total_neg=0,total_pos=0; int i;
    for ( i = 0; i < outCount; i++) {if((probArray[i]=OPartSum(outputarray[i]))>0){total_pos=total_pos+probArray[i];}else{total_neg=total_neg+probArray[i];} }
    for ( i = 0; i < outCount; i++) {if (probArray[i]>0){probArray[i]=probArray[i]/total_pos;} else {probArray[i]=-probArray[i]/total_neg;}}
}

