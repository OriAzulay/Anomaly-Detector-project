#include "timeseries.h"
#include "string.h"

TimeSeries::TimeSeries(const char *CSVfileName)
{
    // ReadFile(CSVfileName);
    ifstream fin;
    fin.open(CSVfileName);
    ifstream Cfile(CSVfileName);
    if (!Cfile.is_open())
        throw runtime_error("Could not open file");
    string line, colname;
    float val;
    int size = 0;
    // keep the features name
    if (Cfile.good())
    {
        getline(Cfile, line);
        stringstream ss(line);
        while (getline(ss, colname, ','))
        {
            features.push_back({colname, vector<float>{}});
            size++;
        }
    }
    // loop for initilize the rows by every feature
    while (getline(Cfile, line))
    {
        stringstream ss(line);
        int colIdx = 0;
        // while every value is float, push it into the vector values by the index
        while (ss >> val)
        {
            features.at(colIdx).second.push_back(val);
            // If the next token is a comma, ignore it and move on
            if (ss.peek() == ',')
                ss.ignore();
            colIdx++;
        }
    }
    Cfile.close();
}
