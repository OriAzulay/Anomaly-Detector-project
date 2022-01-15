

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
//#include "timeseries.h"
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

    //function to read CSV file 
	void readFile(string fileName){
		ofstream out(fileName);
		string s="";
		while((s=read())!="done\n"){
			out<<s<<endl;
		}
		out.close();
	}
};

struct fixdReport{
	int begin;
	int end;
	string description;
	bool tp;
};

//structure for saving data information 
// like threshold, report 
struct ReportSum{
	float threshold;
	vector<AnomalyReport> report;
	vector<fixdReport> fixdRports;
	int testFileSize;
	ReportSum(){
		threshold=0.9;
		testFileSize=0;
	}
};


class Command{
protected:
	DefaultIO* dio;

public:
	const string description;
	Command(DefaultIO* dio,const string description):dio(dio),description(description){}
	virtual void execute(ReportSum* sharedState)=0;
	virtual ~Command(){}
};

// This command is for case 1
class CSV_up:public Command{
public:
	CSV_up(DefaultIO* dio):Command(dio,"upload a time series csv file"){}
    //execut and write CSV file into input
	virtual void execute(ReportSum* rs){
		dio->write("Please upload your local train CSV file.\n");
		dio->readFile("anomalyTrain.csv");
		dio->write("Upload complete.\nPlease upload your local test CSV file.\n");
		dio->readFile("anomalyTest.csv");
		dio->write("Upload complete.\n");
	}
};

//This command is for case 2
class algoSettings:public Command{
public:
	algoSettings(DefaultIO* dio):Command(dio,"algorithm settings"){}
	virtual void execute(ReportSum* rs){
		bool isFine=false;
		while(!isFine){
			dio->write("The current correlation threshold is ");
			dio->write(rs->threshold);
			dio->write("\nType a new threshold\n");
			float f;
			dio->read(&f);
            //if user type another threshold
			if(f>0 && f<=1){
				rs->threshold=f;
				isFine=true;
			}
			else
				dio->write("please choose a value between 0 and 1.\n");
		}
	}
};

//This command is for case 3:
class detectAn:public Command{
public:
	detectAn(DefaultIO* dio):Command(dio,"detect anomalies"){}

	virtual void execute(ReportSum* rs){
		TimeSeries train("anomalyTrain.csv");
		TimeSeries test("anomalyTest.csv");
		rs->testFileSize = test.getRowSize();
		HybridAnomalyDetector ad;
		ad.setThreshold(rs->threshold);
		ad.learnNormal(train);
        //detect anomalies
		rs->report = ad.detect(test);
		fixdReport f;
		f.begin=0;
		f.end=0;
		f.description="";
		f.tp=false;
        // for - each loop to push the fixed reports that got from detect
		for_each(rs->report.begin(),rs->report.end(),[&f,rs](AnomalyReport& ar){
            // if there is duplicated, iteration ++
			if(ar.timeStep==f.end+1 && ar.description==f.description)
				f.end++;
			else{
				rs->fixdRports.push_back(f);
				f.begin=ar.timeStep;
				f.end=f.begin;
				f.description=ar.description;
			}
		});
		rs->fixdRports.push_back(f);
		rs->fixdRports.erase(rs->fixdRports.begin());

		dio->write("anomaly detection complete.\n");
	}
};

//This command is for case 4
class dispResult:public Command{
public:
	dispResult(DefaultIO* dio):Command(dio,"display results"){}
	virtual void execute(ReportSum* rs){
        //for-each loop for display results in input
		for_each(rs->report.begin(),rs->report.end(),[this](AnomalyReport& ar){
			dio->write(ar.timeStep);
			dio->write("\t"+ar.description+"\n");
		});
		dio->write("Done.\n");
	}
};


class UploadAnom:public Command{
public:
	UploadAnom(DefaultIO* dio):Command(dio,"upload anomalies and analyze results"){}
    // if there is true positive, before execute to initialize TP
	bool crossS(int as,int ae,int bs, int be){
		return (ae>=bs && be>=as);
	}
	bool isTP(int st, int end,ReportSum* rs){
		for(size_t i=0;i<rs->fixdRports.size();i++){
			fixdReport fr=rs->fixdRports[i];
			if(crossS(st,end,fr.begin,fr.end)){
				rs->fixdRports[i].tp=true;
				return true;
			}
		}
		return false;
	}

	virtual void execute(ReportSum* rs){
		
		for(size_t i=0;i<rs->fixdRports.size();i++){
			rs->fixdRports[i].tp=false;
		}
		
		dio->write("Please upload your local anomalies file.\n");
		string s="";
		float TP=0,sum=0,P=0;
		while((s=dio->read())!="done\n"){
			size_t t=0;
			for(;s[t]!=',';t++);
			string st=s.substr(0,t);
			string en=s.substr(t+1,s.length());
			int start = stoi(st);
			int end = stoi(en);
			if(isTP(start,end,rs))
				TP++;
			sum+=end+1-start;
			P++;
		}
		dio->write("Upload complete.\n");
		float FP=0;
		for(size_t i=0;i<rs->fixdRports.size();i++)
			if(!rs->fixdRports[i].tp)
				FP++;
        //based on TP,FP calculations
		float N=rs->testFileSize - sum;
		float tp_r=((int)(1000.0*TP/P))/1000.0f;
		float fp_r=((int)(1000.0*FP/N))/1000.0f;
		dio->write("True Positive Rate: ");
		dio->write(tp_r);
		dio->write("\nFalse Positive Rate: ");
		dio->write(fp_r);
		dio->write("\n");
	}
};

// for future implemenations
class Exit:public Command{
public:
	Exit(DefaultIO* dio):Command(dio,"exit"){}
	virtual void execute(ReportSum* sharedState){
	}
};


#endif /* COMMANDS_H_ */
