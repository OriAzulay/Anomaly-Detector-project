#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <sstream>
#include "string.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <map>
#include <vector>
#include <string.h>
#include <bits/stdc++.h>
#include <algorithm>
using namespace std;

class TimeSeries
{
protected:
    // vector -> fet.at(index).first\second..
    //time series table presentaion
    map<string,vector<float>> ts;
	vector<string> features;
	size_t dataRowSize;
public:
    TimeSeries(const char *CSVfileName); // construction
    //get features 
    const vector<float>& getFeaturesData(string name)const;
    //get features title
    const vector<string>& getFeatures()const;
    //row size
    size_t getRowSize()const;
};

#endif /* TIMESERIES_H_ */
