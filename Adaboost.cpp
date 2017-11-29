/*===============================================================
*   Copyright (C) 2017 All rights reserved.
*   
*   FileName:Adaboost.cpp
*   creator:yuliu1@microsoft.com
*   Time:11/27/2017
*   Description:
*   Notice: 
*   Updates:
*
================================================================*/
#include<limits> 
#include<math.h>
#include<fstream>
#include<iostream>
#include "Adaboost.h"
// please add your code here!
std::vector<std::string> StringSplit(std::string sstr, const char* delim)
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
int Adaboost:: GetIdByLabelName(std::string label)
{
  if (label=="S")
  {
    return 0;
  }
  else if (label=="B")
  {
    return 1;
  }
  else if (label=="M")
  {
    return 2;
  }
  else
  {
    return 3;
  }
}

std::string Adaboost:: GetLabelNameById(int labelId)
{
  if (labelId==0)
  {
    return "S";
  }
  else if (labelId==1)
  {
    return "B";
  }
  else if (labelId==2)
  {
    return "M";
  }
  else
  {
    return "E";
  }
}

std::vector<double>Adaboost::AdaboostInitialWeights(int N)
{
  std::vector<double> weights(N,1.0/N);
  return weights;
}

int Adaboost::DecisionStumpClassifier(Sample &s, int featIndex,int direction)
{
  if(direction==0)
  {
    if (s.featVec[featIndex] == 1)
    {
      return 1;
    }
    else
    {
      return -1;
    }
  }
  else
  {
    if (s.featVec[featIndex] == 1)
    {
      return -1;
    }
    else
    {
      return 1;
    }

  }
}

StumpInfo Adaboost::BuildDecisionStump(std::vector<Sample>&trainingSet,std::vector<double>&weight,int positiveLabelId, int negtiveLabelId)
{
  double minError = std::numeric_limits<double>::max();
  int bestIndex = 0;
  int bestdirection = 0;
  double minTrainError =std::numeric_limits<double>::max();
  for (int i = 0; i < featsNum;i++)
  {
    for (int m=0; m<DIRECTION_NUM;m++)
    {
      std::vector<double> error_vec(trainingSet.size(),0);
      for (int j = 0; j < int(trainingSet.size());j++)
      {
        int classLabel = DecisionStumpClassifier(trainingSet[j],i,m);
        if ( trainingSet[j].label== positiveLabelId )
        {
            if (  classLabel==-1  )
            {
              error_vec[j]=1;
            }
        }
        else
        {
           if ( classLabel == 1 )
           {
             error_vec[j]=1;
           }
        }
    }
    double weightederror = 0.0;
    double error = 0.0;
    for (int j = 0; j <int(trainingSet.size()) ;j++)
    {
      weightederror += weight[j]*error_vec[j];
      error+=error_vec[j];
    }
    error/=trainingSet.size();
    if (error < minTrainError )
    {
      minTrainError = error;
    }
    if (weightederror < minError)
    {
      minError = weightederror;
      bestIndex = i;
      bestdirection=m;
    }

    }
  }
  StumpInfo stumpInfo;
  stumpInfo.minWeightedError = minError;
  stumpInfo.bestSplitFeatIndex = bestIndex;
  stumpInfo.minTrainError = minTrainError;
  stumpInfo.bestDirection = bestdirection;
  return stumpInfo;
}

void Adaboost::AdaboostTrainDS(std::vector<Sample>&trainingSet,std::vector<double>&alphas,std::vector<int>&bestSplits,std::vector<int> &bestDirections,int positiveLabelId,int negtiveLabelId,double stoprate,int maxIter)
{
  std::ofstream fout("dump.txt");
  std::vector<double> weights = AdaboostInitialWeights(trainingSet.size());
  for (int i = 0; i < maxIter;i++)
  {
    StumpInfo info = BuildDecisionStump(trainingSet,weights,positiveLabelId, negtiveLabelId);
    double alpha = 0.0; 
    
    if ( info.minWeightedError==0)
    {
      alpha = 16; 
    }

    if ( info.minWeightedError==1)
    {
      alpha = 0.01;
    }
    
    alpha = 0.5*((log(1-info.minWeightedError)-log(info.minWeightedError))/log(2.0));
    
    if (alpha >16)
    {
      alpha = 16;
    }
    
    if (alpha < 0)
    {
      alpha = 0.01;
    }
    
    std::cerr <<"positiveLabelId="<<positiveLabelId<<"\tnegtiveLabelId="<<negtiveLabelId<< "\titeration="<<i<<"\tMin Train Error\t" <<info.minTrainError<<"\tMin weightedError\t"<<info.minWeightedError<<"\ta\t"<<alpha<<"\tbestSplit\t"<<info.bestSplitFeatIndex<<"\tbestDirection\t"<<info.bestDirection<<std::endl;
    fout<<"***************************"<<i<<"******************************"<<std::endl;
    for (int k = 0;k<weights.size();k++)
    {
      fout << weights[k]<<std::endl;
    }
    alphas.push_back(alpha);
    bestSplits.push_back(info.bestSplitFeatIndex);
    bestDirections.push_back(info.bestDirection);
    double Z = 0.0; 
    for (int j= 0; j < int (trainingSet.size( )); j++)
    {
        int t = (trainingSet[j].label==positiveLabelId?1:-1);
        int y = DecisionStumpClassifier(trainingSet[j],info.bestSplitFeatIndex,info.bestDirection);
        double factor = exp(-alpha*t*y);
        weights[j] = weights[j]*factor; 
        Z+=weights[j];
    }
    for (int j= 0; j < int(trainingSet.size());j++)
    {
      weights[j]=weights[j]/Z;
    }
    if (info.minWeightedError<stoprate)
    {
      break;
    }

  }
  fout.close();
}


int Adaboost::AdaboostClassify(Sample &s, std::vector<double>&alphas, std::vector<int>&bestSplits, std::vector<int>&bestDirections,int positiveLabelId, int negtiveLabelId)
{
  double weightedLabel = 0;
  for (int i = 0; i<bestSplits.size();i++)
  {
    weightedLabel += alphas[i]*DecisionStumpClassifier(s,bestSplits[i], bestDirections[i]);
  }
  if (weightedLabel>=0)
  {
    return positiveLabelId;
  }
  else
  {
    return negtiveLabelId;
  }
}

void Adaboost::LoadingTrainingSet(const char * inputfilename)
{
  std::ifstream fin(inputfilename);
  std::string line = "";
  int len = 0;
  while (std::getline(fin,line))
  {
    if ( line=="" )
    {
      continue;
    }
    std::vector<std::string> lines = StringSplit(line,"\t");
    len = int(lines.size());
    Sample s(len-2);
    s.label = atoi(lines[len-2].c_str());
    if (trainingSta.count(s.label))
    {
        trainingSta[s.label] += 1;
    }
    else
    {
      trainingSta[s.label] = 1;
    }

    for (int i = 0; i < len -2; i++)
    {
      s.featVec[i] = atoi(lines[i].c_str());
    }
    AlltrainingSet.push_back(s);
  }
  fin.close();
  trainingSize = AlltrainingSet.size();
  featsNum = len-2;
  std::cerr<<"trainingSize "<<trainingSize<<std::endl;
  std::cerr<<"featsNum "<<featsNum<<std::endl;
}

void Adaboost::GeneratingTrainingSetOneVsOne(int targetLabel1, int targetLabel2, std::vector<Sample>&NewtrainingSet)
{
   for (std::vector<Sample>::iterator it = AlltrainingSet.begin(); it != AlltrainingSet.end();it++)
   {
      if (it->label == targetLabel1||it->label==targetLabel2)
      {
        NewtrainingSet.push_back(*it);
      }
   }
}

int Adaboost::ClassifyByVoting(std::vector<int> classifyResults)
{
  std::vector<int> results(CLASS_NUM,0);
  for (std::vector<int>::iterator it = classifyResults.begin(); it != classifyResults.end(); it++)
  {
    results[*it]+=1;
  }
  int maxIndex = 0;
  int maxCount = 0;
  for (int i = 0; i < results.size();i++)
  {
    if (results[i]>maxCount)
    {
      maxCount = results[i];
      maxIndex = i;
    }
  }
  return maxIndex;
}

void Adaboost::MultiClassAdaboostTrainOneVsOne(double stoprate,int maxIter)
{
  std::vector<int> labels;
  for (std::map<int,int>::iterator it = trainingSta.begin(); it != trainingSta.end();it++)
  {
    labels.push_back(it->first);
  }
  for (int i=0; i < int(labels.size())-1; i++)
  {
    for (int j = i+1; j < int(labels.size());j++)
    {
      std::vector<Sample> newtrainingSet;
      GeneratingTrainingSetOneVsOne(labels[i],labels[j],newtrainingSet);
      LabelIdPair.push_back(std::make_pair(labels[i],labels[j]));
      std::vector<double> alpha;
      std::vector<int> bestSplit;
      std::vector<int> bestDirection;
      AdaboostTrainDS(newtrainingSet,alpha,bestSplit,bestDirection,labels[i],labels[j],stoprate,maxIter);
      alphas.push_back(alpha);
      bestSplits.push_back(bestSplit);
      directions.push_back(bestDirection);
    }
  }
}

int Adaboost::MultiClassAdaboostClassifyOneVsOne(Sample &s)
{
    std::vector<int> classifyResults(alphas.size(),0);
    for(int i=0; i < alphas.size();i++)
    {
      classifyResults[i] = AdaboostClassify(s,alphas[i],bestSplits[i],directions[i],LabelIdPair[i].first, LabelIdPair[i].second);
    }
    return ClassifyByVoting(classifyResults); 
}

void Adaboost:: MultiClassAdaboostTrainOneVsOther(double stoprate, int maxIter)
{
  for (std::map<int,int>::iterator it = trainingSta.begin(); it != trainingSta.end();it++)
  {
    positiveLabelIds.push_back(it->first);
  }
  for (int i=0; i < positiveLabelIds.size();i++)
  {
    std::vector<double> alpha;
    std::vector<int> bestSplit;
    std::vector<int> bestDirection;
    AdaboostTrainDS(AlltrainingSet,alpha,bestSplit,bestDirection,positiveLabelIds[i],-1,stoprate,maxIter);
    alphas.push_back(alpha);
    bestSplits.push_back(bestSplit);
    directions.push_back(bestDirection);
  }
}

int Adaboost::MultiClassAdaboostClassifyOneVsOther(Sample& s)
{
  for (int i = 0; i < positiveLabelIds.size();i++)
  {
    int label = AdaboostClassify(s,alphas[i],bestSplits[i],directions[i],positiveLabelIds[i],-1);
    if (label==positiveLabelIds[i])
    {
      return label;
    }
  }
  return -1;
}

void Adaboost::Train(int method,double stoprate,int maxIter)
{
  //one VS one
  if (method==0)
  {
    MultiClassAdaboostTrainOneVsOne(stoprate,maxIter);
  }
  else // one vs other
  {
    MultiClassAdaboostTrainOneVsOther(stoprate,maxIter);
  }
}

int Adaboost::Predict(Sample& s,int method)
{
  if (method==0)
  {
    return MultiClassAdaboostClassifyOneVsOne(s);
  }
  else
  {
    return MultiClassAdaboostClassifyOneVsOther(s);
  }
}
void Adaboost::Print()
{
  for (int i = 0; i <alphas.size();i++)
  {
    for (int j=0; j <alphas[i].size();j++)
    {
      std::cout <<"alphas[i][j]="<< alphas[i][j]<<" ";
    }
    std::cout << std::endl;
  }
}

