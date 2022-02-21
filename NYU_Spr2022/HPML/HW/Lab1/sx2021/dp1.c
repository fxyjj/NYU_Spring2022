#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include<string.h>
#include <time.h>
#include<math.h>

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
    //struct timespec tmid;
    //struct timespec tend;
    struct timespec tbf;
    struct timespec taft;
    // C1
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
            result = dp(n,pA,pB);
            clock_gettime(CLOCK_MONOTONIC,&taft);
            double intv = (double)taft.tv_sec-(double)tbf.tv_sec+((double)taft.tv_nsec-(double)tbf.tv_nsec)/1000000000;
            double bw = 3*sizeof(float)*n/(1000000000*intv);
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
	    result = dp(n,pA,pB);
	}
    }
    //clock_gettime(CLOCK_MONOTONIC,&tmid);
    //clock_gettime(CLOCK_MONOTONIC,&tend);
    printf("%f %lf,%lf\n",result,sec_half_time,first_half_time);
    double overall_time = sec_half_time + first_half_time;
    //printf(" %f %lf,%lf %lf %lf\n",result,sec_half_time,first_half_time,overall_time,((unsigned long)2*n*d)/(1000000000*overall_time));
    //double sec_half_time = ((double)tend.tv_sec-(double)tmid.tv_sec)+((double)tend.tv_nsec-(double)tmid.tv_nsec)/1000000000;
    printf("C1 Full Repitition <N>:%d,<T>:%lf sec,B:%lf Gb/sec,F:%lf GFLOP/sec\n",n,overall_time,(sizeof(float)*3*n*d)/(1000000000*overall_time),((unsigned long)2*n*d)/(1000000000*overall_time));
    printf("C1 Ave Second Half Repitition <N>:%d,<T>:%lf sec,B:%lf Gb/sec,F:%lf GFLOP/sec\n",n/2,sec_half_time*2/d,sec_half_BW*2/d,sec_half_F*2/d);
 
    //printf("overall: %lf,half: %lf\n",overall_time,sec_half_time);
    //printf("time measured %d %lf\n",d,((double)tend.tv_sec-(double)tstart.tv_sec)*1000000+((double)tend.tv_nsec-(double)tstart.tv_nsec)/1000);
    //printf("average time measured %d %lf\n",d,2*(((double)tend.tv_sec-(double)tmid.tv_sec)*1000000+((double)tend.tv_nsec-(double)tmid.tv_nsec)/1000)/d);
    

    return 0;
}

float dp(long N,float *A,float *B){
    float R = 0.0;
    int j;
    for(j=0;j<n;j++){
        //printf(">-------%d--------\n",j);
    	R += A[j]*B[j];
    }
    return R;
}
