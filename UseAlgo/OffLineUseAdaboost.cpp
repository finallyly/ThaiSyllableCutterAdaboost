/*===============================================================
*   Copyright (C) 2017 All rights reserved.
*   
*   FileName:OffLineUseDecisionTree.cpp
*   creator:yuliu1@microsoft.com
*   Time:11/25/2017
*   Description:
*   Notice: 
*   Updates:
*
================================================================*/
 
#include "Adaboost.h"
#include<iostream>
#include<fstream>
// please add your code here!
// notice StringSplit2 is the same as StringSplit,  I just get lazy here
std::vector<std::string> StringSplit2(std::string sstr, const char* delim)
{
  std::vector<std::string> results;
  char *src = new char [sstr.length() + 1];
  strncpy(src,sstr.c_str(),sstr.length());
  src[sstr.length()] = 0;
  char *p = strtok(src,delim);
  if ( p!= NULL)
  {
    results.push_back(p);
  }
  while ( (p=strtok(NULL,delim)) != NULL )
  {
    results.push_back(p);
  }
  if (src != NULL )
  {
    delete [] src;
    src = NULL;
  }
  return results;
}
void TestLoading(const char *trainfilename,const char* testfilename,const char* ofilename)
{
   Adaboost ada;
   //0 for 1vs1 1 for 1vsOther
  int method = 1;
  int maxIter = 50; 
  std::cerr << "begin loading trainingSet" << std::endl;
  ada.LoadingTrainingSet(trainfilename);
  std::cerr <<"begin training"<<std::endl;
  ada.Train(method,0,maxIter);
  std::cerr <<"end training"<<std::endl;
  ada.Print();
  std::cerr <<"begin predicting"<<std::endl;
  int linecount = 0;
  std::ifstream fin(testfilename);
  std::ofstream fout(ofilename);
  std::string line="";
  while (std::getline(fin,line))
  {
    if (line == "")
    {
      fout<<std::endl;
      continue;
    }
    linecount++;
    if (linecount%1000==0)
    {
      std::cerr <<"linecount="<<linecount<<std::endl;
    }
    std::vector<std::string> lines = StringSplit2(line,"\t");
    int len = int(lines.size());
    Sample s(len-2);
    std::vector<std::string> cols = StringSplit2(lines[len-1],"#");
    s.label = atoi(lines[len-2].c_str());
    for (int i = 0; i <len-2; i++)
    {
      s.featVec[i] = atoi(lines[i].c_str());
    }
    int labelId = ada.Predict(s,method);
    std::string label = ada.GetLabelNameById(labelId);
    fout<<cols[0]<<"\t"<<cols[1]<<"\t"<<ada.GetLabelNameById(s.label)<<"\t"<<label<<std::endl;
  }
  fin.close();
  fout.close();
  
}
int main(int argc, char **argv)
{
  if (argc <4)
  {
    std::cerr << "no enough params"<<std::endl;
    exit(1);
  }
  TestLoading(argv[1],argv[2],argv[3]);
  return 0;
}
