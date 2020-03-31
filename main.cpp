#include <iostream>
#include <signal.h>
#include <string>
#include <time.h>
#include <vector>
#include <thread>
#include "RtMidi.h"
#include "screen.hpp"
#include "main_screen.hpp"
using namespace std;

void choosePortDevice(RtMidi *RtMidiPort){
	unsigned int selectedPort;
	bool choosen=false;
	cout<<"choose a port"<<endl;
	for(unsigned int i=0;i<RtMidiPort->getPortCount();i++){
		cout<<i<<" : "<<RtMidiPort->getPortName(i)<<endl;
	}
	cout<<endl;
	while(!choosen){
		cin>>selectedPort;
		if(selectedPort>0&&selectedPort<RtMidiPort->getPortCount()){
			RtMidiPort->openPort(selectedPort);
			cout<<"selected device : "<<RtMidiPort->getPortName(selectedPort)<<endl;
			choosen=true;
		}else{
			cout<<"not available choice"<<endl;
		}
	}
}

vector<unsigned char> displayLed(unsigned char row,unsigned char col,Color c){
	unsigned char ledAdress=16*row+col;
	vector<unsigned char> message;
	message.push_back(0x90);
	message.push_back(ledAdress);
	message.push_back(c);
	return message;
}

bool done;
static void finish(int ignore){ done = true; }

int main(){
	RtMidiIn *in=0;
	RtMidiIn *controller=0;
	RtMidiOut *out=0;
	vector<unsigned char> allLedsOn = {0xB0,0x00,0x7E};
	vector<unsigned char> allLedsOff = {0xB0,0x00,0x00};
	vector<unsigned char> message;
	try{
		in=new RtMidiIn();
		controller=new RtMidiIn();
		out=new RtMidiOut();
	}catch(RtMidiError &e){
		e.printMessage();
	}
	cout<<"select input port"<<endl;
	choosePortDevice(in);
	cout<<"select pad controller"<<endl;
	choosePortDevice(controller);
	cout<<"select output port"<<endl;
	choosePortDevice(out);
	  
	done = false;
  	(void) signal(SIGINT, finish);
	thread inputThread([in,&message,out](){
		/*double stamp;
		int nBytes;*/
		vector<unsigned char> outMessage;
		while (!done)
		{
			/*stamp=*/in->getMessage(&message);
			//nBytes=message.size();
			outMessage={};
			for(unsigned char event : message){
				out->sendMessage(&event,1);
			}
			
		}
		cout<<"exit programm";
	});

		
	thread controllerThread([controller](){
		vector<unsigned char> controllMessage;
		//double stamp;
		bool runs=false;
		while(!done){
			/*stamp=*/controller->getMessage(&controllMessage);
			if(controllMessage.size()>0&&controllMessage[2]>0){
				switch(controllMessage[1]){
					case 0x78:
						cout<<(runs?"stop":"play")<<endl;
						runs=!runs;
						break;
					case 0x68:
						cout<<"rec"<<endl;
						runs=true;
						break;
					case 0x58:
						cout<<"edit track"<<endl;
						break;
					case 0x48:
						cout<<"patter mode"<<endl;
						break;
					default :
						break;
			}
			}
		}
	});

	
	inputThread.join();
	controllerThread.join();
	
	delete in;
	delete controller;
	delete out;
	return 0;
}
