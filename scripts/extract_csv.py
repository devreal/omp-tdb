



def main(argv):
    import subprocess, os, re
    import csv
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('file', type=argparse.FileType('r'), nargs='+')
    #parser.add_argument('file')
    args = parser.parse_args()

    data={}

    for f in args.file:
        #print f.name
        file = f.name


        filename=file.split("/")[-1]
        typelessFilename= filename.replace("."+filename.split(".")[-1],"")
        reader = csv.DictReader(f)
        #print reader.fieldnames
        times=[]
        times_A=[]
        BENCHS=[]
        for row in reader:
            times.append(row['time'])
            times_A.append(row['time_A'])

            BENCHS.append(row['BENCH'])
            #print row['time']

            #data[typelessFilename].append(row['time'])
            #data.append(row['time'])
        data[typelessFilename]=times
        data[typelessFilename+"_error"]=times_A
        #print data["BENCH"]
        #print BENCHS
        #if len(data["BENCH"])  len(BENCHS):
        if "BENCH" in  data:
            print len(data["BENCH"])
            print len(BENCHS)
            if len(data["BENCH"]) < len(BENCHS):
                data["BENCH"]=BENCHS
        else:
            data["BENCH"]=BENCHS


    #print data

    print  data.keys()
    s=""
    s= "bench,thread,task"+','.join(data["BENCH"]) +"\n"


    for k in data.keys():
        if k is not "BENCH" and "_error" not in k:
            thread = k.split("_")[2].replace("t","")
            task_num = k.split("_")[3].replace("n","")
            s+= k+","+thread+","+task_num+','.join(data[k])+"\n"

    s+="\n\n"
    for k in data.keys():
        if k is not "BENCH" and "_error" in k:
            thread = k.split("_")[2].replace("t","")
            task_num = k.split("_")[3].replace("n","")
            s+= k+","+thread+","+task_num+','.join(data[k])+"\n"

    text_file = open("sample.csv", "w")

    text_file.write(s)

    text_file.close()

    # print data
    # print data.keys()
    # keys = data.keys()
    # print keys
    # with open('sample.csv', 'wb') as output_file:
    #     dict_writer = csv.DictWriter(output_file, keys)
    #     dict_writer.writeheader()
    #     dict_writer.writerows(data)#

if __name__ == "__main__":
    import sys
    main(sys.argv)
