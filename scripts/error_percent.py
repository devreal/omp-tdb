#!/usr/bin/python2

import sys
import os

def main():
    errormap = dict();
    for path in sys.argv[1:]:
        filename = os.path.basename(path)
        compiler = filename.split('_')[1]
        ntasks   = filename.split('_')[3]

        if compiler not in errormap: 
            errormap[compiler] = dict()
        with open(path) as f:
            for line in f:
                #print line
                if '=' in line:
                    s1 = line.split('=')
                    benchmark = s1[0]
                    if "NEIGHBORS MASTER" in benchmark or " OUT CHAIN" in benchmark or "IN LINKED" in benchmark:
                        if benchmark not in errormap[compiler]: 
                            errormap[compiler][benchmark] = 0.0
                        s2   = s1[1].split()
                        time = float(s2[0])
                        sd   = float(s2[3])
                        print s1[0], ": error: ", (sd/time)*100
                        if errormap[compiler][benchmark] < (sd/time)*100:
                            errormap[compiler][benchmark] = (sd/time)*100

    for compiler, errors in errormap.iteritems():
        for benchmark, error in errors.iteritems():
            print "{0} | {1}: {2}".format(compiler, benchmark, error)
                    




if __name__ == "__main__":
   main()
