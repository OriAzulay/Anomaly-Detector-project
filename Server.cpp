
#include "Server.h"
//based on Eli's zoom explanation about Client-Server
string socketIO::read(){
    char c = 0;
    size_t i = 0;
    string s = "";
    while (c != '\n')
    {
        recv(clientID, &c, sizeof(char), 0);
        s += c;
    }
    return s;
}
void socketIO::write(string text){
    const char *txt = text.c_str();
    send(clientID, txt, strlen(txt), 0);
}

void socketIO::write(float f){
    ostringstream ss;
    ss << f;
    string s(ss.str());
    write(s);
}

void socketIO::read(float *f){
    // recv(clientID,f,sizeof(float),0);
}

Server::Server(int port) throw(const char *){
    stopped = false;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
        throw "socket failed";

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0)
        throw "bind failure";

    if (listen(fd, 3) < 0)
        throw "listen failure";
}

void Server::start(ClientHandler& ch)throw(const char*){	
	  t=new thread([&ch,this](){
		    while(!stopped){
				socklen_t clientSize=sizeof(client);
				int aClient = accept(fd,(struct sockaddr*)&client,&clientSize);
                //from the thread make executaions for accepted command
				if(aClient>0){
                        ch.handle(aClient);
						close(aClient);
				}
		    }
		    close(fd);
	  });
}

void Server::stop(){
    stopped = true;
    t->join(); // do not delete this!
}

Server::~Server(){
    // TODO Auto-generated destructor stub
}
