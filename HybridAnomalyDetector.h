#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector
{
public:
    HybridAnomalyDetector();
    // overide the SimpleLearn to check wheter there is Circle problem
    virtual void learnNormal(const TimeSeries &ts);
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);
    // Check the circle correlation
    virtual void CorrelateCircle(float maxC, string f1, string f2, Point **ps, int N);
    // check every row if there is a point outside the circle
    virtual bool CircleAnomalCheck(Point p, correlatedFeatures c);
    virtual ~HybridAnomalyDetector();
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
