#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>

struct correlatedFeatures
{
    string feature1, feature2; // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    //for Circle porpose
    Point *center;
    float center_x;
    float center_y;
};

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector
{
protected:
    vector<correlatedFeatures> cf;
    float threshold;
    float corrlationMin;

public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    //learn anomalies
    virtual void learnNormal(const TimeSeries &ts);
    //detect anomalies from CSV file
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);
    float getThreshold()
    {
        return threshold;
    }
    vector<correlatedFeatures> getNormalModel()
    {
        return cf;
    }
    void setThreshold(float t){
        this->threshold = t;
    }
// helpers:
    //turn vectors of data fo points
    Point **toPoints(vector<float> x, vector<float> y);
    // initialize correlated features
    virtual void CorrelatedInit(float max, string f1, string f2, Point **p, const TimeSeries& ts);
    // check if data x & y are correlated
    virtual bool SimpleAnomalCheck(float x, float y,correlatedFeatures c);
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
