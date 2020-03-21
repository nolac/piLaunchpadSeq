#include <iostream>
#include <string>
#include <time.h>
#include <vector>
#include "RtMidi.h"
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

int main(){
	RtMidiIn *in=0;
	RtMidiOut *out=0;

	try{
		in=new RtMidiIn();
		out=new RtMidiOut();
	}catch(RtMidiError e){
		e.printMessage();
	}
	cout<<"select input port"<<endl;
	choosePortDevice(in);
	cout<<"select output port"<<endl;
	choosePortDevice(out);
	bool isRunning=true;
	int currentPosition=0;
	time_t  oldTime=clock(),currentTime;
	vector<unsigned char> message;
	message.push_back(0xB0);
	message.push_back(0x00);
	message.push_back(0x00);
	while(isRunning){
		currentTime=clock();
			if((currentTime-oldTime)/CLOCKS_PER_SEC>=1){
				cout<<++currentPosition<<endl;
				oldTime=currentTime;
				out->sendMessage(&message);
				message[1]++;

			}
			if(currentPosition>=10){
				message[2]=0;
				out->sendMessage(&message);
				isRunning=false;
			}

	}
	delete in;
	delete out;
	return 0;
}
