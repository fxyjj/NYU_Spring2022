#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include<string.h>
#include <time.h>
int n;
int d;
//check the format of the argument, only digit char allowed
int checkDigit(char *str){
    int res = 1;
    for(int i = 0;i<strlen(str);i++){
        res = res && isdigit(str[i]);
    }
    return res;
} 
float dp(long N,float *A,float *B);
int main(int argc, char *argv[]){
    if(argc>3 || argc < 3){
        printf("only two parameters allowed!\n");
	return 0;
    }
    if(argc == 3){
		if(checkDigit(argv[1]) && checkDigit(argv[2])){
		    n = atoi(argv[1]);
	        d = atoi(argv[2]);
		}else{   
		    printf("integer parameters only!\n");
		    return 0;
	    }
    }
    float pA[n];
    float pB[n];
    //initialize
    for(int i=0;i<n;i++){
    	pA[i] = 1.0;
    	pB[i] = 1.0;
    }
    struct timespec tstart;
    struct timespec tend;
    clock_gettime(CLOCK_MONOTONIC,&tstart);
    for(int i=0;i<d;i++){
        printf("------%d-------\n",i);
         dp(n,pA,pB);
    }
    clock_gettime(CLOCK_MONOTONIC,&tend);
    printf("time measured %d %lf\n",d,(tend.tv_sec-tstart.tv_sec)*1000000+(tend.tv_nsec-tstart.tv_nsec)/1000);
    return 0;
}

float dp(long N,float *A,float *B){
    float R = 0.0;
    int j;
    for(j=0;j<n;j++){
        printf(">-------%d--------\n",j);
    	R += A[j]+B[j];
    }
    return R;
}
