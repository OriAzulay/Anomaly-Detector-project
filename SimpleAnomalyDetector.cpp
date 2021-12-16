
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
Point **SimpleAnomalyDetector::toPoint(float *X, float *Y, size_t N)
{
    Point **ps = new Point *[N];
    for (int t = 0; t < N; t++)
    {
        ps[t] = new Point(X[t], Y[t]); // point (x,y)
    }
    return ps;
}

// Method to save the correlated feature and push to the cf
void SimpleAnomalyDetector::CorrelatedInit(float max, string f1, string f2, Point **p, int N)
{
    correlatedFeatures Cpair;
    Cpair.corrlation = max;
    Cpair.feature1 = f1;
    Cpair.feature2 = f2;
    Line l = linear_reg(p, N);
    Cpair.lin_reg = l;
    float maxDev = 0;
    for (int s = 0; s < N; s++)
    {
        if (maxDev < dev(*(p[s]), l))
        {
            maxDev = dev(*(p[s]), l);
        }
        Cpair.threshold = maxDev * 1.1;
    }
    cf.push_back(Cpair);
}

// A method to delete all the duplicated corrolation features check.
void SimpleAnomalyDetector::duplicateRemove()
{
    int count = 0;
    for (correlatedFeatures cl : cf)
    {
        for (int k = count; k < cf.size(); k++)
        {
            if ((cl.feature1 == cf.at(k).feature2) && (cl.feature2 == cf.at(k).feature1))
            {
                cf.erase(cf.begin() + k);
                continue;
            }
        }
        count++;
    }
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
    TimeSeries t = ts;
    size_t len = t.getVector().at(0).second.size();
    float tempF1[len];
    float tempF2[len];
    correlatedFeatures Cpair;
    int itr = t.getVector().size();             // iterator
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
                // initial the maximun correlation at cf -> a correlation vector
                if (maxC < abs(pearson(tempF1, tempF2, N)))
                {
                    maxC = abs(pearson(tempF1, tempF2, N));
                    f1 = t.getVector().at(i).first;
                    f2 = t.getVector().at(j).first;
                    ps = toPoint(tempF1, tempF2, len);
                }
            }
            // The initial part
            CorrelatedInit(maxC, f1, f2, ps, N);
            itr--;
            // delete the points
            for (size_t k = 0; k < len; k++)
                delete ps[k];
            delete[] ps;
        }
    }
    duplicateRemove();
}

/**
 * @brief on every report we want to return description and timestep.
 *  The method in the end, return a vector of "AnomalyReport" for every single row
 *  that count the timestep (start from 1) and have the descripsion (for the most corrolated\uncorrolated?) like "A-C"
 * @param ts
 * @return vector<AnomalyReport>
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts)
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
            Point p1(ts.getVector().at(index1).second[j], ts.getVector().at(index2).second[j]);
            if (SimpleAnomalCheck(p1, cl))
            {
                report.push_back(AnomalyReport(ts.getVector().at(index1).first + "-" + ts.getVector().at(index2).first, timeStep));
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

// check if there is anomal detection
bool SimpleAnomalyDetector::SimpleAnomalCheck(Point p, correlatedFeatures c)
{
    return (dev(p, c.lin_reg) > c.threshold);
}
