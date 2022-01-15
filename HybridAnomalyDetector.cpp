
#include "HybridAnomalyDetector.h"
#include "minCircle.h"

HybridAnomalyDetector::HybridAnomalyDetector()
{
    corrlationMin = 0.5;
}

void HybridAnomalyDetector::CorrelatedInit(float max, string f1, string f2, Point **p, const TimeSeries& ts){
    SimpleAnomalyDetector::CorrelatedInit(max,f1,f2,p,ts);
    if(max>0.5 && max<threshold){
        correlatedFeatures Cpair;
        Circle c = findMinCircle(p,ts.getRowSize());
        Cpair.threshold=c.radius*1.1;
        Cpair.feature1 = f1;
        Cpair.feature2 = f2;
        Cpair.corrlation = max;
        Point *circleCenter = new Point(c.center.x, c.center.y);
        Cpair.center = circleCenter;
        Cpair.center_x = c.center.x;
        Cpair.center_y = c.center.y;
        Cpair.lin_reg = linear_reg(p,ts.getRowSize());
        cf.push_back(Cpair);
    }
}

bool HybridAnomalyDetector::SimpleAnomalCheck(float x, float y,correlatedFeatures c)
{
    //override anomalCheck for hybrid use
    return (c.corrlation>=threshold && SimpleAnomalyDetector::SimpleAnomalCheck(x,y,c)) ||
			(c.corrlation>corrlationMin && c.corrlation<threshold && distance(Point(c.center_x,c.center_y),Point(x,y))>c.threshold);
}

HybridAnomalyDetector::~HybridAnomalyDetector()
{
}
