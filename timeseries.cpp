#include "timeseries.h"

TimeSeries::TimeSeries(const char* CSVfileName){
		ifstream in(CSVfileName); //open csv file
		string head; 
		in>>head; //read title
		string feat;
		stringstream hss(head);
        // every title splited by ',' , push to features
		while(getline(hss,feat,',')){ 
			ts.emplace(feat,vector<float>());
		    features.push_back(feat);
		}
        // while the title is not empty, push data to rows
		while(!in.eof()){
			string line;
			in>>line;
			string val;
			stringstream lss(line);
			int i=0;
			while(getline(lss,val,',')){
				ts[features[i]].push_back(stof(val));
			     i++;
			}
		}
		in.close();

		dataRowSize = ts[features[0]].size();

	}

    const vector<float>& TimeSeries::getFeaturesData(string name)const{
		return ts.at(name);
	}

    const vector<string>& TimeSeries::getFeatures()const{
		return features;
	}
    size_t TimeSeries::getRowSize()const{
		return dataRowSize;
	}