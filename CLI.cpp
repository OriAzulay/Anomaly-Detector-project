#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
		this->dio=dio;
        // use components design pattern & bridge DP
		commandos.push_back(new CSV_up(dio));
		commandos.push_back(new algoSettings(dio));
		commandos.push_back(new detectAn(dio));
		commandos.push_back(new dispResult(dio));
		commandos.push_back(new UploadAnom(dio));
		commandos.push_back(new Exit(dio));
}

void CLI::start(){
	ReportSum rs;
	int idx=-1;
	while(idx!=5){
		dio->write("Welcome to the Anomaly Detection Server.\n");
		dio->write("Please choose an option:\n");
		for(size_t i=0;i<commandos.size();i++){
			string s("1.");
			s[0]=((char)(i+1+'0'));
			dio->write(s);
			dio->write(commandos[i]->description+"\n");
		}
		string input = dio->read();
		idx=input[0]-'0'-1;
        //execute commands as the index showing while client pick index (case)
		if(idx>=0 && idx<=6)
			commandos[idx]->execute(&rs);
	}
}

CLI::~CLI() {
	for(size_t i=0;i<commandos.size();i++){
		delete commandos[i];
	}
}