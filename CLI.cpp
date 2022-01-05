#include "CLI.h"


CLI::CLI(DefaultIO* di) {
    dio = di;
}

void CLI::start(){
    
     MacroCommand macro(dio);
    Command* one = new Command_1(dio,&macro);
    Command* two = new Command_2(dio, &macro);
    Command* three = new Command_3(dio, &macro);
    Command* four = new Command_4(dio, &macro);
    Command* five = new Command_5(dio, &macro);
    Command* six = new Command_6(dio, macro);
    macro.setCommands(one,two,three,four,five,six);
    macro.execute();
    
}   

CLI::~CLI(){
}

