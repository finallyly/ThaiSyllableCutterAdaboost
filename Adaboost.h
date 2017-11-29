/*===============================================================
*   Copyright (C) 2017 All rights reserved.
*   
*   FileName:Adaboost.h
*   creator:yuliu1@microsoft.com
*   Time:11/27/2017
*   Description:
*   Notice: 
*   Updates:
*
================================================================*/
 
#ifndef _ADABOOST_H
#define _ADABOOST_H
// please add your code here!
#include<vector>
#include<map>
#include<string>
#define CLASS_NUM 4
#define DIRECTION_NUM 2
struct Sample
{
  Sample(int featSize)
  {
    featVec.resize(featSize);
  }
  std::vector<int> featVec;
  int label;
};
struct StumpInfo
{
  int bestSplitFeatIndex;
  int bestDirection;
  double minWeightedError;
  double minTrainError;
};

class Adaboost
{
  public:
  int GetIdByLabelName(std::string label);
  std::string GetLabelNameById(int labelId);
    std::vector<double> AdaboostInitialWeights(int N);
    //notice for as for basic decision Stump the direction is a very important option, if direction is fixed, the whole algorithm failed. here for thai syllable cutter , direction has 0, 1 two directions
    int DecisionStumpClassifier(Sample& s, int featIndex, int direction);
    StumpInfo BuildDecisionStump(std::vector<Sample>&trainingSet,std::vector<double>&weight,int positiveLabelId,int negtiveLabelId=-1);
    void AdaboostTrainDS(std::vector<Sample>&trainingSet, std::vector<double>&alphas,std::vector<int>&bestSplits,std::vector<int> &bestDirections,int positiveLabelId,int negtiveLabelId,double stoprate,int maxIter);  
    int AdaboostClassify(Sample &s, std::vector<double>&alphas,std::vector<int>&bestSplits,std::vector<int>&bestDirections,int positveLabelId,int negtiveLabelId);
    void LoadingTrainingSet(const char* inputfile);
    void GeneratingTrainingSetOneVsOther(int targetLabel,std::vector<Sample>& trainingSet);
    void GeneratingTrainingSetOneVsOne(int targetLabel1,int targetLabel2,std::vector<Sample>&trainingSet);
    void MultiClassAdaboostTrainOneVsOne(double stoprate, int maxIter);
    void MultiClassAdaboostTrainOneVsOther(double stoprate, int maxIter);
    int ClassifyByVoting(std::vector<int> classifyResults);
    int MultiClassAdaboostClassifyOneVsOne(Sample& s); 
    int MultiClassAdaboostClassifyOneVsOther(Sample& s);
    void Train(int method,double stoprate,int maxIter);
    int Predict(Sample &s, int method);
    void Print(); 
  private:
   // for multiclass version
    int trainingSize;
    int featsNum;
    std::vector<Sample> AlltrainingSet;
    // store the the Label and count information of AlltrainingSet
    std::map<int,int> trainingSta;
    // data structures used for oneVSone or oneVSother version
    // notice you can not use 1vs1, and 1vsO at the same time, because they share the following data members. 
    std::vector<int>positiveLabelIds;
    std::vector<std::pair<int,int> > LabelIdPair;
    std::vector<std::vector<int> > bestSplits;
    std::vector<std::vector<int> > directions;
    std::vector<std::vector<double> > alphas;
};
#endif
