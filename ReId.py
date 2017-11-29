#-*-coding:utf8-*-
#########################################################################
#   Copyright (C) 2017 All rights reserved.
# 
#   FileName:ReId.py
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
def Process(infilename, ofilename):
    id=0;
    fout = open(ofilename,"w");
    with open(infilename,"r") as f:
        for line in f:
            line  = line.strip();
            col = line.split("\t");
            fout.write("%s\t%d\n"%(col[0],id));
            id+=1;
    fout.close();
if __name__=="__main__":
    if (len(sys.argv)!=3):
        sys.stderr.write("no enough params\n");
        sys.exit(1);
    Process(sys.argv[1],sys.argv[2]);

