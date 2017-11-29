#-*-coding:utf8-*-
#########################################################################
#   Copyright (C) 2017 All rights reserved.
# 
#   FileName:AdjustId.py
#   Creator: yuliu1@microsoft.com
#   Time:11/28/2017
#   Description:
#
#   Updates:
#
#########################################################################
#!/usr/bin/python
# please add your code here!
import sys;
import re;
mydict = {};
def LoadRefFile(reffile):
    with open(reffile,"r") as f:
        for line in f:
            line = line.strip();
            col = line.split("\t");
            mydict[int(col[0])]=int(col[1]);
def Process(ifilename,ofilename):
    fout = open(ofilename,"w");
    with open(ifilename,"r") as f:
        for line in f:
            line = line.strip();
            if (line==""):
                fout.write("\n");
                continue;
            col = line.split("\t");
            newcol=[];
            for i in range(0,100):
                newcol.append(0);
            for m in col[:len(col)-2]:
                mykey=int(m);
                newcol[mydict[mykey]]=1;
            newline="";
            for i in range(0,len(newcol)):
                if (i!=0):
                    newline+="\t";
                newline += str(newcol[i]);
            newline+="\t"+col[-2];
            newline+="\t"+col[-1];
            fout.write("%s\n"%newline);
    fout.close();


if __name__=="__main__":
    if (len(sys.argv)!=4):
        sys.stderr.write("no enough params\n");
        sys.exit(1);
    LoadRefFile(sys.argv[1]);
    Process(sys.argv[2],sys.argv[3]);

