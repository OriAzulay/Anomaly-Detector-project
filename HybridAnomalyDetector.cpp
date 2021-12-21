
#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector()
{
    corrlationMin = 0.5;
}

void HybridAnomalyDetector::CorrelateCircle(float max, string f1, string f2, Point **ps, int N)
{
    correlatedFeatures Cpair;
    Cpair.corrlation = max;
    Cpair.feature1 = f1;
    Cpair.feature2 = f2;
    Line l = linear_reg(ps, N);
    Cpair.lin_reg = l;
    Circle c = findMinCircle(ps, N);
    Cpair.threshold = c.radius * 1.1;

    Point *circleCenter = new Point(c.center.x, c.center.y);
    Cpair.center = circleCenter;
    cf.push_back(Cpair);
}

void HybridAnomalyDetector::learnNormal(const TimeSeries &ts)
{
    TimeSeries t = ts;
    size_t len = t.getVector().at(0).second.size();
    float tempF1[len];
    float tempF2[len];
    correlatedFeatures Cpair;
    int itr = t.getVector().size();
    int N = sizeof(tempF1) / sizeof(tempF1[1]); // size of table's columns
    while (itr != 0)
    {
        // for loop to run with every feature over the whole Table
        //  if it gets the most corrolated, save it in Cpair and push cf
        for (int i = 0; i < t.getVector().size(); i++)
        {
            float maxC = 0;
            string f1;
            string f2;
            Point **ps;
            for (int j = 0; j < t.getVector().size(); j++)
            {
                // skip on equal features check like A-A
                if (t.getVector().at(i).first == t.getVector().at(j).first)
                {
                    continue;
                }
                // turn vector to float
                for (int k = 0; k < len; k++)
                {
                    tempF1[k] = t.getVector().at(i).second[k];
                    tempF2[k] = t.getVector().at(j).second[k];
                }
                if (maxC < abs(pearson(tempF1, tempF2, N)))
                {
                    maxC = abs(pearson(tempF1, tempF2, N));
                    f1 = t.getVector().at(i).first;
                    f2 = t.getVector().at(j).first;
                    ps = toPoint(tempF1, tempF2, len);
                }
            }
            if (maxC >= threshold) // check if the correaltion greater then 0.9
                CorrelatedInit(maxC, f1, f2, ps, N);
            else
            { // probably between 0.5->0.9
                CorrelateCircle(maxC, f1, f2, ps, N);
            }
            itr--;
            // delete the points
            for (size_t k = 0; k < len; k++)
                delete ps[k];
            delete[] ps;
        }
    }
    duplicateRemove();
    // Checker:
    //  for(correlatedFeatures cl: cf){
    //  	cout<<cl.feature1<<"  "<<cl.feature2<<endl<<"threshold: "<<cl.threshold<<endl;
    //  }
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts)
{
    vector<AnomalyReport> report;
    for (correlatedFeatures cl : cf)
    {
        int index1, index2;
        // A loop to save the corrolated index in index1(f1) and index2(f2)
        for (int i = 0; i < ts.getVector().size(); i++)
        {
            if (cl.feature1 == ts.getVector().at(i).first)
            {
                index1 = i;
            }
            else if (cl.feature2 == ts.getVector().at(i).first)
            {
                index2 = i;
            }
        }
        int timeStep = 1;
        // A loop to run over every row with the spesific iteration (Like A-B for instance) and push the detected errors (cl.threshold)
        for (int j = 0; j < ts.getVector().at(0).second.size(); j++)
        {
            Point p1(ts.getVector().at(index1).second[j], ts.getVector().at(index2).second[j]); // the specific row
            if (cl.corrlation < threshold && cl.corrlation > corrlationMin)
            { // if correlation between 0.5->0.9, find by circle
                if (CircleAnomalCheck(p1, cl))
                {
                    report.push_back(AnomalyReport(ts.getVector().at(index1).first + "-" + ts.getVector().at(index2).first, timeStep));
                }
            }
            else
            { // probably correlation above 0.9
                if (SimpleAnomalCheck(p1, cl))
                {
                    report.push_back(AnomalyReport(ts.getVector().at(index1).first + "-" + ts.getVector().at(index2).first, timeStep));
                }
            }
            timeStep++;
        }
    }
    // Checker detect:
    //  for(AnomalyReport rprt: report){
    //  	cout<<"detect_check: "<<rprt.description<<",  timeStep:"<<rprt.timeStep<<endl;
    //  }
    return report;
}

bool HybridAnomalyDetector::CircleAnomalCheck(Point p, correlatedFeatures c)
{
    return (distance(p, *(c.center)) > c.threshold);
}

HybridAnomalyDetector::~HybridAnomalyDetector()
{
}
