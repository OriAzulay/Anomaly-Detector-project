#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector : public SimpleAnomalyDetector
{
public:
    HybridAnomalyDetector();
    // overide the SimpleLearn to check wheter there is Circle problem
    //initilize correlated features for both circle & csv data
    virtual void CorrelatedInit(float max, string f1, string f2, Point **p, const TimeSeries& ts);
    virtual bool SimpleAnomalCheck(float x, float y,correlatedFeatures c);
    virtual ~HybridAnomalyDetector();
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
