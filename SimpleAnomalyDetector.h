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
    Point *center;
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
    virtual void learnNormal(const TimeSeries &ts);
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);
    float getThreshold()
    {
        return threshold;
    }
    vector<correlatedFeatures> getNormalModel()
    {
        return cf;
    }

    Point **toPoint(float *X, float *Y, size_t N);
    void CorrelatedInit(float max, string f1, string f2, Point **p, int N);
    void duplicateRemove();
    bool SimpleAnomalCheck(Point p, correlatedFeatures c);
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
