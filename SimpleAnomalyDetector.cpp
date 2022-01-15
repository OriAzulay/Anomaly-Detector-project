//ID : 206336794
#include "SimpleAnomalyDetector.h"
#include <iostream>
#include <algorithm>

SimpleAnomalyDetector::SimpleAnomalyDetector()
{
    threshold = 0.9; // configure the threshold
}

SimpleAnomalyDetector::~SimpleAnomalyDetector()
{
}

// ------helping methods----------
// create Point from list of floats
Point** SimpleAnomalyDetector::toPoints(vector<float> x, vector<float> y){
	Point** ps=new Point*[x.size()];
	for(size_t i=0;i<x.size();i++){
		ps[i]=new Point(x[i],y[i]);
	}
	return ps;
}

// Method to save the correlated feature and push to the cf
void SimpleAnomalyDetector::CorrelatedInit(float max, string f1, string f2, Point **p, const TimeSeries& ts)
{
    correlatedFeatures Cpair;
    Cpair.corrlation = max;
    Cpair.feature1 = f1;
    Cpair.feature2 = f2;
    Line l = linear_reg(p, ts.getFeaturesData(f1).size());
    Cpair.lin_reg = l;
    float maxDev = 0;
    //find threshhold
    for (int s = 0; s < ts.getFeaturesData(f1).size(); s++)
    {
        if (maxDev < dev(*(p[s]), l))
        {
            maxDev = dev(*(p[s]), l);
        }
        Cpair.threshold = maxDev * 1.1;
    }
    cf.push_back(Cpair);
}

//--------------------------------
/**
 * @brief
 * Check for every fearture1, by pearson who is the most corrolate
 * to him, and update vector<correlatedFeatures> that keep
 * inside him astruct with feature1 that checked and feature 2 that corrolate.
 * @param ts
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts)
{
    // get the rows name and store in vector
    vector<string> atts=ts.getFeatures();
	size_t len=ts.getRowSize();
	float vals[atts.size()][len];
    //create 2d vector for table ts as vals
    for(size_t i=0;i<atts.size();i++){
		vector<float> x=ts.getFeaturesData(atts[i]);
		for(size_t j=0;j<len;j++){
			vals[i][j]=x[j];
		}
	}
    // check for max correlation every column
    for(size_t i=0;i<atts.size();i++){
		string f1=atts[i];
		float max=0;
		size_t index_max=0;
		for(size_t j=i+1;j<atts.size();j++){
			float p=abs(pearson(vals[i],vals[j],len));
            //check by pearson correlation
			if(p>max){
				max=p;
				index_max=j;
			}
		}
        string f2=atts[index_max];
        Point** ps=toPoints(ts.getFeaturesData(f1),ts.getFeaturesData(f2));
        //assamption that f1 & f2 have the size column's size
        CorrelatedInit(max,f1, f2, ps, ts);
        
        //delete points space:
        for(size_t po=0;po<len;po++)
			delete ps[po];
		delete[] ps;
    }
}

/**
 * @brief on every report we want to return description and timestep.
 *  The method in the end, return a vector of "AnomalyReport" for every single row
 *  that count the timestep (start from 1) and have the descripsion (for the most corrolated\uncorrolated?) like "A-C"
 * @param ts
 * @return vector<AnomalyReport>
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts){
    vector<AnomalyReport> report;
    for (correlatedFeatures cl : cf)
    {
        vector<float> x=ts.getFeaturesData(cl.feature1);
		vector<float> y=ts.getFeaturesData(cl.feature2);
		for(size_t i=0;i<x.size();i++){
            //check if the linear regrese is greater then threshhold
			if(SimpleAnomalCheck(x[i],y[i],cl)){
				string des=cl.feature1 + "-" + cl.feature2;
				report.push_back(AnomalyReport(des,(i+1))); //timestep from 1
			}
		}
    }
    return report;
}

// check if there is anomal detection
bool SimpleAnomalyDetector::SimpleAnomalCheck(float x, float y,correlatedFeatures c)
{
    return (abs(y - c.lin_reg.f(x))>c.threshold);
}
