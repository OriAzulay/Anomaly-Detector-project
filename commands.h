

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <sstream>
#include <iomanip>
#include <map>
using namespace std;

class
    DefaultIO
{
public:
    virtual string read() = 0;
    virtual void write(string text) = 0;
    virtual void write(float f) = 0;
    virtual void read(float *f) = 0;
    // optinal - func to upload file from path
    void getFileByPath()
    {
        const char *path;
        ofstream file(path);
    }

    virtual ~DefaultIO() {}
};

class Command
{
protected:
    DefaultIO *dio;

public:
    Command(DefaultIO *dio) : dio(dio) {}
    virtual void execute() = 0;
    virtual string getDescription() = 0;
    virtual ~Command() {}
};

class MacroCommand : public Command
{
protected:
    DefaultIO *dioM;

public:
    vector<Command *> commandos;
    float threshold;
    bool menu;
    vector<TimeSeries> timeseries;
    vector<correlatedFeatures> cf; // the correlate features after learning anomalTrain.csv
    vector<AnomalyReport> r;       // the anomal detection after detect the anomalTest.csv
    MacroCommand(DefaultIO *dio) : Command(dio)
    {
        dioM = dio;
        threshold = 0.9;
    }
    virtual void execute()
    {
        string optionN = dioM->read();
        int i;
        menu = true;
        while (menu)
        {
            if (optionN == "1")
            {
                dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
                for (Command *c : commandos)
                {
                    dioM->write(c->getDescription() + "\n");
                }
                commandos.at(0)->execute();
                optionN = dioM->read();
            }
            if (optionN == "2")
            {
                dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
                for (Command *c : commandos)
                {
                    dioM->write(c->getDescription() + "\n");
                }
                commandos.at(1)->execute();
                optionN = dioM->read();
            }
            if (optionN == "3")
            { // WRONG SIZE - (23014 INSTEAD 6 ????)
                dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
                for (Command *c : commandos)
                {
                    dioM->write(c->getDescription() + "\n");
                }
                commandos.at(2)->execute();
                optionN = dioM->read();
            }
            if (optionN == "4")
            {
                dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
                for (Command *c : commandos)
                {
                    dioM->write(c->getDescription() + "\n");
                }
                commandos.at(3)->execute();
                optionN = dioM->read();
            }
            if (optionN == "5")
            {
                dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
                for (Command *c : commandos)
                {
                    dioM->write(c->getDescription() + "\n");
                }
                commandos.at(4)->execute();
                optionN = dioM->read();
            }
            if (optionN == "6")
            {
                dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
                for (Command *c : commandos)
                {
                    dioM->write(c->getDescription() + "\n");
                }
                menu = false;
            }
        }
    }
    virtual string getDescription()
    {
        return "menu";
    }
    void setCommands(Command *a, Command *b, Command *c, Command *d, Command *e, Command *f)
    {
        commandos = {a, b, c, d, e, f};
    }
    virtual ~MacroCommand() {}
};

class Command_1 : public Command
{ // get from the user txt and upload csv files
    DefaultIO *dio1;

public:
    MacroCommand *m;
    string description;
    Command_1(DefaultIO *dio, MacroCommand *macro) : Command(dio)
    {
        description = "1.upload a time series csv file";
        dio1 = dio;
        m = macro;
    }
    virtual string getDescription()
    {
        return description;
    }
    virtual void execute() override
    {
        int itr = 2; // 2 uploads, first train, second test
        string fileName = "Train";
        string name = "train";
        while (itr != 0)
        {
            dio1->write("Please upload your local " + name + " CSV file.\n");
            ofstream myfile;
            myfile.open("anomaly" + fileName + ".csv");
            string lines = dio1->read();
            while (lines != "done")
            {
                myfile << lines + "\n";
                lines = dio1->read();
            }
            itr--;
            myfile.close();
            fileName = "Test";
            dio1->write("Upload complete.\n");
            name = "test";
            myfile.close();
        }
        m->timeseries.push_back(TimeSeries("anomalyTrain.csv")); // insert macroCommand train timeseries to learn
        m->timeseries.push_back(TimeSeries("anomalyTest.csv"));  // insert macroCommand test timeseries to detect
    }
    ~Command_1() {}
};
class Command_2 : public Command
{ // present the threshold that we or the user configure
    DefaultIO *dio2;

public:
    MacroCommand *m;
    string description;
    Command_2(DefaultIO *dio, MacroCommand *macro) : Command(dio)
    {
        description = "2.algorithm settings";
        dio2 = dio;
        m = macro;
    }
    virtual string getDescription()
    {
        return description;
    }
    virtual void execute() override
    {
        bool opt = false;
        float thr;
        while (!opt)
        {
            stringstream stream; // turn float to string
            stream << m->threshold;
            string t = stream.str();
            dio2->write("The current correlation threshold is " + t + "\n"
                                                                      "Type a new threshold\n");
            string g = dio2->read();
            stringstream s(g);
            s >> thr;
            if (thr > 0 && thr < 1)
            { // if the user typed currect threshold, save it
                m->threshold = thr;
                opt = true;
            }
            else
            {
                dio2->write("please choose a value between 0 and 1.\n");
            }
        }
    }
    ~Command_2() {}
};
class Command_3 : public Command
{ // from the given timesetries, find anomal correaltion to cf, and detect testCSV into r
    DefaultIO *dio3;

public:
    MacroCommand *m;
    string description;
    Command_3(DefaultIO *dio, MacroCommand *macro) : Command(dio)
    {
        description = "3.detect anomalies";
        dio3 = dio;
        m = macro;
    }
    virtual string getDescription()
    {
        return description;
    }
    virtual void execute()
    {
        HybridAnomalyDetector ad;
        ad.learnNormal(m->timeseries.at(0));
        m->cf = ad.getNormalModel();
        m->r = ad.detect(m->timeseries.at(1));
        dio3->write("anomaly detection complete.\n");
    }
    ~Command_3() {}
};
class Command_4 : public Command
{ // present the anomal detections by timesteps and descreption
    DefaultIO *dio4;

public:
    MacroCommand *m;
    string description;
    Command_4(DefaultIO *dio, MacroCommand *macro) : Command(dio)
    {
        description = "4.display results";
        dio4 = dio;
        m = macro;
    }
    virtual string getDescription()
    {
        return description;
    }
    virtual void execute()
    {
        for (AnomalyReport a : m->r)
        {
            stringstream stream;
            stream << a.timeStep;
            string t = stream.str();
            dio4->write(t + "	 " + a.description + "\n");
        }
        dio4->write("Done.\n");
    }
    ~Command_4() {}
};
class Command_5 : public Command
{
    DefaultIO *dio5;

public:
    MacroCommand *m;
    string description;
    Command_5(DefaultIO *dio, MacroCommand *macro) : Command(dio)
    {
        description = "5.upload anomalies and analyze results";
        dio5 = dio;
        m = macro;
    }
    virtual string getDescription()
    {
        return description;
    }
    virtual void execute()
    {
        dio5->write("Please upload your local anomalies file.\n");
        map<string, vector<int>> T;
        string temp;
        vector<int> I;
        I.push_back(m->r[0].timeStep);
        for (int k = 1; k < m->r.size(); k++)
        {
            if (m->r[k - 1].description == m->r[k].description && (m->r[k - 1].timeStep == m->r[k].timeStep - 1))
            {
                temp = m->r[k - 1].description;
                I.push_back(m->r[k].timeStep);
                T[temp] = I;
            }
            else
            {
                I.clear();
                I.push_back(m->r[k].timeStep);
                temp = m->r[k].description;
                T[temp] = I;
            }
        }
        bool op = true;
        float TP = 0;
        string report = dio5->read();
        float P = 0;
        int numberOfReports = 0;
        while (op)
        {
            P++;
            vector<int> vect;
            stringstream ss(report);
            for (int i; ss >> i;)
            { // get the min and max by vector
                vect.push_back(i);
                if (ss.peek() == ',')
                    ss.ignore();
            }
            numberOfReports += (vect.at(1) - vect.at(0));
            numberOfReports++;
            int cout = 0;
            int falseCount = 0;
            for (pair<string, vector<int>> x : T)
            {
                for (int j = 0; j < x.second.size(); j++)
                {
                    if (x.second[j] >= vect[0] && x.second[j] <= vect[1])
                    {
                        cout++;
                    }
                }
            }
            if (cout > 0)
            {
                TP++;
            }
            report = dio5->read();
            if (report == "done")
            {
                op = false;
            }
        }
        float TruePositiveRate = (TP / P);
        TruePositiveRate = floor(TruePositiveRate * 1000);
        TruePositiveRate = TruePositiveRate / 1000;
        float FP = (T.size() - TP);
        int N = (m->timeseries.at(0).getVector().at(0).second.size()) - (numberOfReports);
        float FalseAlarmRate = FP / N;
        FalseAlarmRate = floor(FalseAlarmRate * 1000);
        FalseAlarmRate = FalseAlarmRate / 1000;
        stringstream stream;
        stream << setprecision(2) << TruePositiveRate;
        string t = stream.str();
        stringstream sstream;
        sstream << setprecision(2) << FalseAlarmRate;
        string f = sstream.str();
        dio5->write("Upload complete.\n");
        dio5->write("True Positive Rate: " + t + "\n");
        dio5->write("False Positive Rate: " + f + "\n");
        // TODO - HOW TO ROUND 3 DIGITS AFTER THE DOT (0.015 AND NOT 0.01522..)
    }

    ~Command_5() {}
};
class Command_6 : public Command
{
    DefaultIO *dio6;

public:
    MacroCommand *m;
    string description;
    Command_6(DefaultIO *dio, MacroCommand macro) : Command(dio)
    {
        description = "6.exit";
        dio6 = dio;
        m = &macro;
    }
    virtual string getDescription()
    {
        return description;
    }
    virtual void execute()
    {
        m->menu = false;
    }
    ~Command_6() {}
};

#endif /* COMMANDS_H_ */
