import numpy as np
import sys,getopt
import time
# chack the parameters format and validity.
def digitCheck(argv):
    n = 16
    k = 4
    try:
       opts, args = getopt.getopt(argv,"hn:k:",["help","pageNo=","pageSz="])
    except getopt.GetoptError:
        print('test.py -n <ArrSize> -k <Iteration> : parameter needed')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ('-h','--help'):
            print('test.py -n <ArrSize> -k <Iteration>')
            sys.exit()
        elif opt in ("-n", "--ArrSize"):
            if int(arg) <= 0 or any(i not in '0123456789' for i in arg):
                print("invalid size!")
                sys.exit(2)
            n = int(arg)
        elif opt in ("-k", "--iter"):
            if int(arg) < 0 or any(i not in '0123456789' for i in arg):
                print('at least 1 iteraton or must be number!')
                sys.exit(2)
            k = int(arg)
    return n,k

def dp(n:int,A:np.ndarray,B:np.ndarray):
    R = 0.0
    for i in range(n):
        R += A[i]*B[i]
    return R

# main routine
if __name__ == "__main__":
    n,k = digitCheck(sys.argv[1:])
    res = 0.0
    print(n,k)
    A = np.ones(n,dtype=np.float32)
    B = np.ones(n,dtype=np.float32)
    first_half = 0.0
    second_half = 0.0
    check = 0
    bw = 0.0
    flop = 0.0
    start = time.monotonic()
    for j in range(k):
        if j >= k//2:
            bf = time.monotonic()
            res = dp(n,A,B)
            aft = time.monotonic()
            intv = aft - bf
            i_bw = 3*4*n/(1000000000*intv)
            i_flop = 2*n/(1000000000*intv)
            if check == 0:
                first_half = bf - start
                check = 1

            #print("iter",j,"R:",res,"<T>:",intv,"<B>:",i_bw,"<F>:",i_flop)
            bw+=i_bw
            flop+=i_flop
            second_half += intv
        else:
            res = dp(n,A,B)
    print(res,first_half,second_half)
    overall_time = first_half + second_half
    print("C4 <N>:",n," ,<T>: ",(overall_time),"sec, <B>: ",3*4*n*k/(overall_time*1000000000),"Gb/s,F: ",2*n*k/(1000000000*overall_time),"GFLOP/s")
    print("C4 <N>:",n,", <T>: ",second_half*2/k,"sec, <B>: ",bw*2/k,"Gb/s, <F>:",flop*2/k,"GFLOP/s")
