from pylab import plotfile, show, gca
import matplotlib.cbook as cbook
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import matplotlib.cm as cm

import string


import numpy
import csv
from itertools import izip

filled_markers = ('o', 'v', '^', '<', '>', '8', 's', 'p', '*', 'h', 'H', 'D', 'd', 'P', 'X')


def rgb2gray(rgb):
    return numpy.dot(rgb[...,:3], [0.299, 0.587, 0.144])

def main(argv):
    file=argv[1]
    col_names_list=[]
    row_names_list=[]

    #with open(file) as f:
    #    with open(file+"tans", 'w') as fw:
    #        csv.writer(fw, delimiter=',').writerows(izip(*csv.reader(f, delimiter=',')))

    import pandas as pd
    pd.read_csv(file).T.to_csv('output.csv',header=False)
    file ="output.csv"

    #return
    with open(file) as csv_file:
        for row in csv_file:
            col_names_list =[x.strip() for x in row.split(",")]
            break
        for row in csv_file:
            row_names_list.append(row.split(",")[0])


    col_names= tuple(col_names_list)
    #col_names = '.'join(col_names)


    #filter_x = lambda x,y: [xi for (xi, yi) in zip(x,y) if not numpy.isnan(yi)]
    #filter_y = lambda y: [yi for yi in y if not numpy.isnan(yi)]
    filter_x = lambda x,y : x[numpy.isfinite(y)]
    filter_y = lambda y : y[numpy.isfinite(y)]

    csv_data = matplotlib.mlab.csv2rec(file,delimiter=u',', skiprows=2, missing='--',names=col_names)
    print row_names_list
    print col_names

    xx=[1,2,4,8,16,24,32]
    print "-------------------------"
    #print csv_data
    #print len(xx)
    #print len(csv_data['taskbench_gnu_t2_n1000'])
    print "-------------------------"

    #print filter_x(lbc_768['cores'],lbc_768['lbc_pure_MPI'])
    #print filter_y(lbc_768['lbc_pure_MPI'])





    size=7


    compilers_list=["gnu","intel", "intel-17.0.1.132", "mcc.openmp","mcc.ompss","clang"]
    tasks_list=["1000", "5000"]
#    seperate(compilers_list,tasks_list,size,csv_data,xx)
    thread=["1","2","12"]
    seperate2(compilers_list,tasks_list,size,csv_data,xx,thread)
    #seperateTrans(compilers_list,tasks_list,size,csv_data,xx,thread)

    seperateByBench(compilers_list,tasks_list,size,csv_data,xx,thread,2)
    seperateByBench(compilers_list,tasks_list,size,csv_data,xx,thread,9)
    seperateByBench(compilers_list,tasks_list,size,csv_data,xx,thread,16)
    seperateByBench(compilers_list,tasks_list,size,csv_data,xx,thread,23)
    seperateByBench(compilers_list,tasks_list,size,csv_data,xx,thread,30)
    seperateByBench(compilers_list,tasks_list,size,csv_data,xx,thread,37)

def nameFilter(input, compiler, thread):
#    out = input.translate(None, string.digits)+" "+compiler+ " threads="+ thread
#    out = input.translate(None, string.digits)+" "+compiler+ " threads="+ thread
#    out = out.replace("TASK DEPENDENCY","")
#    out = out.replace("MASTER","")
    out = compiler+ " threads="+ thread
    return out



def seperateByBench(compilers_list,tasks_list,size,csv_data,xx,threads,start):

    for tasks in tasks_list:
        plt.figure()
        for thread in threads:
            for compiler in compilers_list:
                line=":"
                color="red"
                marker="o"
                if compiler == "intel":
                    color="green"
                    marker="<"
                if compiler == "intel-17.0.1.132":
                    color="yellow"
                    marker=">"
                if compiler == "mcc.openmp":
                    color = "blue"
                    marker="_"
                if compiler == "mcc.ompss":
                    color = "magenta"
                    marker="|"
                if compiler == "clang":
                    color = "orange"
                    marker="s"

                if thread  is "2":
                    line="--"
                if thread  is "12":
                    line="-."


                plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                                        label=nameFilter(csv_data["bench"][start],compiler,thread), marker=marker, markersize=8, fillstyle="none",color=color,linestyle=line)



        #plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=2, mode="expand", borderaxespad=0.)
        #plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.12),ncol=2, fancybox=True, shadow=True,fontsize=10)
        plt.legend(loc='upper left',bbox_to_anchor=(-0.15, 1.13),ncol=2, fancybox=True, shadow=True,fontsize=8,markerscale=0.5)
        plt.grid()
        #gca().set_xlabel('threads/per core')
        #gca().set_ylabel('walltime in seconds')
        gca().set_xscale('log',basex=2)
        gca().set_yscale('log',basex=2)
        gca().set_xticks(xx)
        #gca().set_xticks([1,2,4,8,12,16,24])
        #gca().set_yticks([10,20,30,50,100,130,180])
        gca().get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
        gca().get_yaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
        plt.xlim(0,36)
        #plt.ylim(7,260)
        plt.plot()
        plt.ylim([0,5000])
        name =nameFilter(csv_data["bench"][start],"",thread)
        filename = csv_data["bench"][start].translate(None, string.digits)+"_"+str(threads)+"_"+tasks+".pdf"
        plt.savefig(filename.translate(None, "[],'").replace(" ", "_"),format='pdf',dpi=600)


        #plt.show()





def seperate2(compilers_list,tasks_list,size,csv_data,xx,threads):

    for tasks in tasks_list:
        plt.figure()
        for thread in threads:
            for compiler in compilers_list:
                line=":"
                color="red"
                if compiler == "intel":

                    color="green"
                if compiler == "mcc.openmp":
                    color = "blue"
                if compiler == "mcc.ompss":
                    color = "magenta"
                if compiler == "clang":
                    color = "orange"
                if thread  is "2":
                    line="--"


                start=2
                plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                                        label=nameFilter(csv_data["bench"][start],compiler,thread), marker='o', markersize=8, fillstyle="none",color=color,linestyle=line)
                start=9
                #plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                #                        label=nameFilter(csv_data["bench"][start],compiler,thread), marker='v', markersize=10,color=color,linestyle=line)
                start=16
                #plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                #                        label=nameFilter(csv_data["bench"][start],compiler,thread), marker='^', markersize=10,color=color,linestyle=line)
                start=23
                plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                                        label=nameFilter(csv_data["bench"][start],compiler,thread), marker='<', markersize=8, fillstyle="none",color=color,linestyle=line)
                start=30
                plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                                        label=nameFilter(csv_data["bench"][start],compiler,thread), marker='>', markersize=8, fillstyle="none",color=color,linestyle=line)
                start=37
                plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                                        label=nameFilter(csv_data["bench"][start],compiler,thread), marker='s', markersize=8, fillstyle="none",color=color,linestyle=line)



        #plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=2, mode="expand", borderaxespad=0.)
        #plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.12),ncol=2, fancybox=True, shadow=True,fontsize=10)
        plt.legend(loc='upper left',ncol=2, fancybox=True, shadow=True,fontsize=8,markerscale=0.5)
        plt.grid()
        #gca().set_xlabel('threads/per core')
        #gca().set_ylabel('walltime in seconds')
        gca().set_xscale('log',basex=2)
        gca().set_yscale('log',basex=2)
        gca().set_xticks(xx)
        #gca().set_xticks([1,2,4,8,12,16,24])
        #gca().set_yticks([10,20,30,50,100,130,180])
        gca().get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
        gca().get_yaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
        plt.xlim(0,36)
        #plt.ylim(7,260)
        plt.plot()
        plt.ylim([0,1000])
        plt.savefig("_".join(compilers_list)+"_"+tasks+".pdf",format='pdf',dpi=600)


        #plt.show()

def seperate(compilers_list,tasks_list,size,csv_data,xx):
    for compiler in compilers_list:
        for tasks in tasks_list:
            plt.figure()
            start=2
            plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t1_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t1_n'+tasks+'_error'][start:start+size],
                                    label=nameFilter(csv_data["bench"][start],"","1"), marker='o', markersize=8, fillstyle="none",color='red',linestyle=':')
            plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t2_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t2_n'+tasks+'_error'][start:start+size],
                                    label=nameFilter(csv_data["bench"][start],"","2"), marker='o', markersize=8, fillstyle="none",color='green',linestyle='--')


            start=9
            #plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t1_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t1_n'+tasks+'_error'][start:start+size],
            #                        label=nameFilter(csv_data["bench"][start],"","1"), marker='v', markersize=10,color='red',linestyle=':')
            #plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t2_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t2_n'+tasks+'_error'][start:start+size],
            #                        label=nameFilter(csv_data["bench"][start],"","2"), marker='v', markersize=10,color='green',linestyle='--')

            start=16
            #plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t1_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t1_n'+tasks+'_error'][start:start+size],
            #                        label=nameFilter(csv_data["bench"][start],"","1"), marker='^', markersize=10,color='red',linestyle=':')
            #plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t2_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t2_n'+tasks+'_error'][start:start+size],
            #                        label=nameFilter(csv_data["bench"][start],"","2"), marker='^', markersize=10,color='green',linestyle='--')


            start=23
            plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t1_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t1_n'+tasks+'_error'][start:start+size],
                                    label=nameFilter(csv_data["bench"][start],"","1"), marker='<', markersize=8, fillstyle="none",color='red',linestyle=':')
            plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t2_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t2_n'+tasks+'_error'][start:start+size],
                                    label=nameFilter(csv_data["bench"][start],"","2"), marker='<', markersize=8, fillstyle="none",color='green',linestyle='--')


            start=30
            plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t1_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t1_n'+tasks+'_error'][start:start+size],
                                    label=nameFilter(csv_data["bench"][start],"","1"), marker='>', markersize=8, fillstyle="none",color='red',linestyle=':')
            plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t2_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t2_n'+tasks+'_error'][start:start+size],
                                    label=nameFilter(csv_data["bench"][start],"","2"), marker='>', markersize=8, fillstyle="none",color='green',linestyle='--')

            start=37
            plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t1_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t1_n'+tasks+'_error'][start:start+size],
                                    label=nameFilter(csv_data["bench"][start],"","1"), marker='s', markersize=8, fillstyle="none",color='red',linestyle=':')
            plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t2_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t2_n'+tasks+'_error'][start:start+size],
                                    label=nameFilter(csv_data["bench"][start],"","2"), marker='s', markersize=8, fillstyle="none",color='green',linestyle='--')


            #plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=2, mode="expand", borderaxespad=0.)
            #plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.12),ncol=1, fancybox=True, shadow=True,fontsize=10)
            plt.legend(loc='upper left',ncol=1, fancybox=True, shadow=True,fontsize=10,markerscale=0.5)
            plt.grid()
            gca().set_title(compiler)
            #gca().set_xlabel('threads/per core')
            #gca().set_ylabel('walltime in seconds')
            gca().set_xscale('log',basex=2)
            gca().set_yscale('log',basex=2)
            gca().set_xticks(xx)
            #gca().set_xticks([1,2,4,8,12,16,24])
            #gca().set_yticks([10,20,30,50,100,130,180])
            gca().get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
            gca().get_yaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
            plt.xlim(0,36)
            #plt.ylim(7,260)
            plt.plot()
            plt.savefig(compiler+"_"+tasks+".pdf",format='pdf',dpi=600)


            #plt.show()


def seperateTrans(compilers_list,tasks_list,size,csv_data,xx,threads):
    alpha =0.7
    for tasks in tasks_list:
        plt.figure()
        for thread in threads:
            for compiler in compilers_list:
                line="-"
                color="red"
                if compiler == "intel":

                    color="green"
                if compiler == "mcc.openmp":
                    color = "blue"
                if compiler == "mcc.ompss":
                    color = "magenta"
                if thread  is "2":
                    line="--"


                start=2
                plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                                        label=nameFilter(csv_data["bench"][start],compiler,thread),
                                        color=color,linestyle=line,linewidth=20.0,alpha=alpha)
                start=9
                #plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                #                        label=nameFilter(csv_data["bench"][start],compiler,thread), marker='v', markersize=10,color=color,linestyle=line)
                start=16
                #plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                #                        label=nameFilter(csv_data["bench"][start],compiler,thread), marker='^', markersize=10,color=color,linestyle=line)
                start=23
                plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                                        label=nameFilter(csv_data["bench"][start],compiler,thread),
                                        color=color,linestyle=line,linewidth=20.0,alpha=alpha)
                start=30
                plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                                        label=nameFilter(csv_data["bench"][start],compiler,thread),
                                        color=color,linestyle=line,linewidth=20.0,alpha=alpha)
                start=37
                plt.errorbar(xx,csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks][start:start+size], yerr= csv_data['taskbench_'+compiler+'_t'+thread+'_n'+tasks+'_error'][start:start+size],
                                        label=nameFilter(csv_data["bench"][start],compiler,thread),
                                        color=color,linestyle=line,linewidth=20.0,alpha=alpha)



        #plt.legend(bbox_to_anchor=(0., 1.02, 1., .102), loc=3, ncol=2, mode="expand", borderaxespad=0.)
        #plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.12),ncol=2, fancybox=True, shadow=True,fontsize=10)
        #plt.legend(loc='upper left',ncol=2, fancybox=True, shadow=True,fontsize=10)
        plt.grid()
        #gca().set_xlabel('threads/per core')
        #gca().set_ylabel('walltime in seconds')
        gca().set_xscale('log',basex=2)
        gca().set_yscale('log',basex=2)
        gca().set_xticks(xx)
        #gca().set_xticks([1,2,4,8,12,16,24])
        #gca().set_yticks([10,20,30,50,100,130,180])
        gca().get_xaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
        gca().get_yaxis().set_major_formatter(matplotlib.ticker.ScalarFormatter())
        plt.xlim(0,36)
        #plt.ylim(7,260)
        plt.plot()
        plt.savefig("_".join(compilers_list)+"_"+tasks+".pdf",format='pdf',dpi=600)


        plt.show()



if __name__ == "__main__":
    import sys
    main(sys.argv)
