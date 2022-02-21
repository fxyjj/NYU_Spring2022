#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include<string.h>
#include <time.h>
#include<math.h>
#include <mkl_cblas.h>

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
float bdp(long N,float *A,float *B);
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
    struct timespec tbf;
    struct timespec taft;
    clock_gettime(CLOCK_MONOTONIC,&tstart);

    float result=0.0;
    int check = 0;
    double first_half_time = 0.0;
    double sec_half_time = 0.0;
    double sec_half_BW = 0.0;
    double sec_half_F = 0.0;
    for(int i=0;i<d;i++){
        if(i>=(int)floor(d/2)){
            
            clock_gettime(CLOCK_MONOTONIC,&tbf);
            result = bdp(n,pA,pB);
            clock_gettime(CLOCK_MONOTONIC,&taft);
            double intv = (double)taft.tv_sec-(double)tbf.tv_sec+((double)taft.tv_nsec-(double)tbf.tv_nsec)/1000000000;
            double bw = 3*n*sizeof(float)/(1000000000*intv);
            double flop = 2*n/(1000000000*intv);
            sec_half_time += intv;
            sec_half_BW += bw;
            sec_half_F += flop;
            if(check == 0){
		first_half_time = (double)tbf.tv_sec-(double)tstart.tv_sec+((double)tbf.tv_nsec-(double)tstart.tv_nsec)/1000000000;
	        check=1;
	    }
           // printf("iter:%d,  R:%f, T:%lfs, B:%lfGb/s, F:%lfGFLOP/s\n",i, result,intv,bw,flop);
        }else{
	      result = bdp(n,pA,pB);
	}
    }
    printf("%f\n",result);
    double overall_time = sec_half_time + first_half_time;
    printf("C3 Full Repitition <N>:%d,<T>:%lf sec,B:%lf Gb/sec,F:%lf GFLOP/sec\n",n,overall_time,(sizeof(float)*3*n*d)/(1000000000*overall_time),((unsigned long)2*n*d)/(1000000000*overall_time));
    printf("C3 Ave Second Half Repitition <N>:%d,<T>:%lf sec,B:%lf Gb/sec,F:%lf GFLOP/sec\n",n/2,sec_half_time*2/d,sec_half_BW*2/d,sec_half_F*2/d);

    return 0;
}

float bdp(long N,float *A,float *B){
    float R = cblas_sdot(N, A, 1, B, 1);
    return R;
}
