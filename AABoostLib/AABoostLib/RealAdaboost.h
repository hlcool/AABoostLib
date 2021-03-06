#ifndef REALADABOOST_H
#define REALADABOOST_H

#include "Common.h"

class EXPORT_CLASS OneSample
{
	//行为
public:
	OneSample();
	~OneSample();

	//初始化样本概率分布
	void InitProb(unsigned int samplesNum);

	//属性
public:
	enum LABELTYPE{NEGATIVE=-1,UNKNOWN=0,POSITIVE=1};

	LABELTYPE m_label;
	int m_partition;
	double m_probability;
	vector<double> m_features;
};

typedef vector<OneSample> Samples;
typedef vector<OneSample* > PSamples;
typedef Samples PosSamples;
typedef Samples NegSamples;

class EXPORT_CLASS LUT
{
	//行为
public:
	LUT();
	~LUT();

	//获取特征集合中的最小值最大值
	void GetMinMaxFeat(Samples &allsamples);

	//设置获取binscount
	void SetBinsCount(int binscount);
	int GetBinsCount();

	//获取特征所属BINS
	int FindFeatBin(CLASSIFIER classifier,double feature);

	//设置获取特征类型数量
	void SetFeatTypesnum(unsigned int value);
	unsigned int GetFeatTypesnum();

	//属性
protected:
	int m_binscount;
	unsigned int m_feattypesnum;
	vector<double> m_min;
	vector<double> m_max;
};

class EXPORT_CLASS Classifier:public LUT
{
public:
	Classifier(){}
	~Classifier(){}

	//强分类器级联
	double m_bestb;
	vector<CLASSIFIER> m_strongbestclassifier;
	vector<vector<double> > m_strongbesth;
};

class EXPORT_CLASS DividedManagement
{
	//行为
public:
	DividedManagement();
	~DividedManagement();

	//计算ProbW
	void CalcProbW();

	//获得ProbW
	double GetProbPosW();
	double GetProbNegW();

	//计算弱分类器输出
	void CalcH();

	//获得弱分类器输出
	double GetH();

	//设置平滑因子
	void SetSmoothingFactor(double value);

	//属性
public:
	PSamples m_samples;

	//属性
private:
	double m_probposw;
	double m_probnegw;
	double m_h;
	double m_smoothingfactor;
};

typedef vector<DividedManagement> DividedManagements;

class EXPORT_CLASS AABoost
{
	//行为
public:
	AABoost();
	~AABoost();

	//连续Adaboost算法
	void RunRealAdaboost(double maxfalsepositivesf,double minpassd,unsigned int maxweakclassifiernum);

	//释放空间
	void Release();

	//使用当前分类器预测分类，输入为单个样本，返回值为分类结果
	OneSample::LABELTYPE Predict(const OneSample &onesample);

	//插入样本接口
	void InsertOneSample(OneSample::LABELTYPE label,vector <double> &features,bool isFirst=false);

	//行为
private:
	//初始化
	void Init();
	void InitWeak();

	//划分样本
	void Samples2Managements(CLASSIFIER classifier);

	//样本概率分布变更后更新样本
	//void Managements2Samples();

	//计算当前划分归一化因子并选取最优结果
	void SelectBestNormalizationFactorAndH();

	//更新概率分布
	void UpdateProbabilityDistribution();

	//计算强分类器误报率及通过率
	void CalcFalseAndPass(double &falsepositivesf,double &passd,double minpassd);

	//属性
public:
	double m_falsepositivesf;
	double m_passd;

	Samples m_allsamples;

	Classifier m_finalclassifier;

	//属性
private:
	CLASSIFIER m_bestclassifier;
	CLASSIFIER m_currentclassifier;
	unsigned int m_t;
	double m_bestnormalizationfactor;
	vector<double> m_besth;

	DividedManagements m_dividedmanagements;

#ifdef DEBUG_OUTPUT
	double m_totalprob;
	double m_totalnum;
#endif
};

#endif