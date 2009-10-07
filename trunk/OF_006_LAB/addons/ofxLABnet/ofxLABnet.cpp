/*
 *  ofxLABnet.cpp
 *  openFrameworks
 *
 *  Created by Brett "the Factor" Renfer on 6/3/09.
 *  Copyright 2009 Rockwell Group. All rights reserved.
 *
 */

#include "ofxLABnet.h"

/***************************************************************
	
	BASIC CONSTRUCTOR
 
	tries to connect to lab default listening port (2000)
	- catches error if it doesn't, but doesn't print anything...
	- if it fails, sets "initialized" to false, which will prompt
	a reconnect when you attempt to add a listener or sender
 
***************************************************************/

	ofxLABnet::ofxLABnet(){
		//create the lab switch osc listener
		try {
			
			//load the last osc variables from XML
			loadXMLVariables();
			
			switchListener = new ofxLABOSCListener(labOSCPort, "LAB_switch_listener", 0);
			initialized = true;
			addOSCListener(switchListener);
		}
		catch (std::runtime_error){
			initialized = false;
		}
	};

/***************************************************************
	
	PORT - BASED CONSTRUCTOR
	sets up the lab listener on a port you pass to it. 
 
 ***************************************************************/

	ofxLABnet::ofxLABnet(int _port){
		cout<<"SETUP "<<_port<<endl;
		labOSCPort	= _port;
		initialized = true;
		//create the lab switch osc listener
		switchListener = new ofxLABOSCListener(_port, "LAB_switch_listener", 0);
		addOSCListener(switchListener);
		
	};

/***************************************************************

	UPDATE
	
	- checks the listeners, which update their variables if they've
	caught any new OSC; also checks for a new port
	- checks the senders, which reroutes them if you've sent them
	a message
 
***************************************************************/

	void ofxLABnet::update(){
		//update all OSC listeners
		for (int i=0; i<OSClisteners.size(); i++){
			OSClisteners[i]->update();
		}
		//update all OSC senders
		for (int i=0; i<OSCSenders.size(); i++){
			OSCSenders[i]->update();
			
			if (OSCSenders[i]->dupPort != 0 && OSCSenders[i]->dupHost != ""){
				duplicateOscSender(OSCSenders[i], OSCSenders[i]->dupHost, OSCSenders[i]->dupPort);
				OSCSenders[i]->dupPort = 0;
				OSCSenders[i]->dupHost = "";
			}
			if (OSCSenders[i]->destroy && OSCSenders[i]->active){
				destroy(OSCSenders[i]);
			}
		}
	};

/***************************************************************
 
	DRAW
	 
	- does nothing
 
 ***************************************************************/

	void ofxLABnet::draw(){
	};


/***************************************************************
 
	SEND
 
	- sends everything
 
 ***************************************************************/

	void ofxLABnet::send(){
		for (int i=0; i<OSCSenders.size(); i++){
			OSCSenders[i]->send();
		}			
	};

	void ofxLABnet::send( ofxOscMessage m ){
		for (int i=0; i<OSCSenders.size(); i++){
			OSCSenders[i]->send(m);
		}			
	};

/***************************************************************
 
	ADD OSC LISTENER
	
	- after you create a listener somewhere, pass a pointer to it
	to this function
	- this adds it to the switch listener, allowing it to be
	rerouted based on its name
	- adds it to the listeners vector, so it updates every time
	you call update()
 
***************************************************************/

	void ofxLABnet::addOSCListener(ofxLABOSCListener * listener)
	{
		if (!initialized){
			ofxLABnet(LAB_DEFAULT_PORT);
		}
				
		//create message string to listen to
		stringstream reroute;
		reroute<<"/"<<listener->name<<"/reroute";
		
		stringstream destroy;
		destroy<<"/"<<listener->name<<"/destroy";
		
		//add to switch + add to listener vector
		if (listener != switchListener){
			switchListener->addVariable(&(listener->port), reroute.str());
			switchListener->addVariable(&(listener->port), destroy.str());
			cout<<"LISTENING FOR SWITCH ON "<<reroute.str()<<endl;
		} else {
			cout<<"ADDING MAIN LISTENER"<<endl;
		}
		
		OSClisteners.push_back(listener);
	};

	ofxLABOSCListener * ofxLABnet::addOSCListener(int _port, string _name)
	{
		if (!initialized){
			ofxLABnet(LAB_DEFAULT_PORT);
		}
		
		ofxLABOSCListener * listener = new ofxLABOSCListener(_port, _name, OSClisteners.size());
		
		//create message string to listen to
		stringstream reroute;
		reroute<<"/"<<listener->name<<"/reroute";
		
		stringstream destroy;
		destroy<<"/"<<listener->name<<"/destroy";
		
		//add to switch + add to listener vector
		if (listener != switchListener){
			switchListener->addVariable(&(listener->port), reroute.str());
			switchListener->addVariable(&(listener->port), destroy.str());
			cout<<"LISTENING FOR SWITCH ON "<<reroute.str()<<endl;
		} else {
			cout<<"ADDING MAIN LISTENER"<<endl;
		}
		
		OSClisteners.push_back(listener);
		return listener;
	};


/***************************************************************
 
	ADD OSC SENDER
 
	- after you create a sender somewhere, pass a pointer to it
	to this function
	- this adds it to the switch listener, allowing it to be
	rerouted based on its name
	- adds it to the senders vector, so it updates every time
	you call update()
 
 ***************************************************************/

	void ofxLABnet::addOscSender(ofxLABOSCSender * sender)
	{
		if (!initialized) ofxLABnet(LAB_DEFAULT_PORT);
		
		//create message string to listen to
		stringstream ssHost;
		ssHost<<"/"<<sender->name<<"/reroute/host";
			
		stringstream ssPort;
		ssPort<<"/"<<sender->name<<"/reroute/port";
		
		stringstream duplicateHost;
		duplicateHost<<"/"<<sender->name<<"/duplicate/host";
		
		stringstream duplicatePort;
		duplicatePort<<"/"<<sender->name<<"/duplicate/port";
		
		stringstream destroy;
		destroy<<"/"<<sender->name<<"/destroy";
		
		cout<<"LISTENING FOR SWITCH ON "<<ssHost.str()<<":"<<ssPort.str()<<endl;
		
		//add host, port + duplicate vars
		switchListener->addVariable(&(sender->host), ssHost.str());
		switchListener->addVariable(&(sender->port), ssPort.str());
		switchListener->addVariable(&(sender->dupHost), duplicateHost.str());
		switchListener->addVariable(&(sender->dupPort), duplicatePort.str());
		switchListener->addVariable(&(sender->destroy), destroy.str());
		
		OSCSenders.push_back(sender);
	};

	ofxLABOSCSender * ofxLABnet::addOscSender(string _host, int _port, string _name)
	{
		if (!initialized) ofxLABnet(LAB_DEFAULT_PORT);
		
		ofxLABOSCSender * sender = new ofxLABOSCSender(_host, _port,_name, OSCSenders.size());
		
		//create message string to listen to
		stringstream ssHost;
		ssHost<<"/"<<sender->name<<"/reroute/host";
		
		stringstream ssPort;
		ssPort<<"/"<<sender->name<<"/reroute/port";
		
		stringstream duplicateHost;
		duplicateHost<<"/"<<sender->name<<"/duplicate/host";
		
		stringstream duplicatePort;
		duplicatePort<<"/"<<sender->name<<"/duplicate/port";
		
		stringstream destroy;
		destroy<<"/"<<sender->name<<"/destroy";
		
		cout<<"LISTENING FOR SWITCH ON "<<ssHost.str()<<":"<<ssPort.str()<<endl;
		
		//add host, port + duplicate vars
		switchListener->addVariable(&(sender->host), ssHost.str());
		switchListener->addVariable(&(sender->port), ssPort.str());
		switchListener->addVariable(&(sender->dupHost), duplicateHost.str());
		switchListener->addVariable(&(sender->dupPort), duplicatePort.str());
		switchListener->addVariable(&(sender->destroy), destroy.str());
		
		OSCSenders.push_back(sender);
		return sender;
	};

/***************************************************************
 
	 DESTROY OSC SENDER
	 
	- either destroys a sender completely or, if there's only
	one left, turns it off
	 
 ***************************************************************/

	void ofxLABnet::destroy (ofxLABOSCSender * sender){
		cout<<"DESTROYING: "<<sender->name<<endl;
		if (OSCSenders.size() > 1){
			for (int i=0; i<OSCSenders.size(); i++){
				if (OSCSenders[i] == sender){
					OSCSenders.erase(OSCSenders.begin() + i);
					break;
				}
			}
		} else {
			sender->active = false;
		}
	};

	void ofxLABnet::destroy (string sender){
		for (int i=0; i<OSCSenders.size(); i++){
			if (OSCSenders[i]->name == sender){
				if (OSCSenders.size() > 1){
					OSCSenders.erase(OSCSenders.begin() + i);
					break;
				} else {
					OSCSenders[i]->active = false;
				}
			}
		} 
	};


/***************************************************************
 
	DUPLICATE OSC SENDER
 
	- basically to make it possible to listen via other things
	in real time
 
***************************************************************/

//	void ofxLABnet::duplicateOscSender(ofxLABOSCSender * sender, string host, int port)
//	{
//		bool newSender = true;
//		
//		for (int i=0; i<OSCSenders.size(); i++){
//			
//			if (OSCSenders[i]->host == host && OSCSenders[i]->port == port) newSender = false;
//		}
//		
//		if (newSender){			
//			cout<<"DUPLICATING SENDER "<<sender->name<<" TO "<<host<<":"<<port<<endl;
//			ofxLABOSCSender * newSender = new ofxLABOSCSender(host, port, sender->name, OSCSenders.size());
//			for (int i=0; i<sender->variablePointers.size(); i++){
//				newSender->addVariable(sender->variablePointers[i]);
//			};
//			
//			addOscSender(newSender);
//		} else {
//			cout<<"THIS SENDER ALREADY EXISTS, DUMMY!"<<endl;
//		}
//	};

	//this is a little bit unsafe, as it might just return you the old sender

	ofxLABOSCSender* ofxLABnet::duplicateOscSender(ofxLABOSCSender * sender, string host, int port)
	{
		ofxLABOSCSender * newOSCSender;
		bool newSender = true;
		
		for (int i=0; i<OSCSenders.size(); i++){			
			if (strcmp(OSCSenders[i]->host.c_str(), host.c_str()) == 0 && OSCSenders[i]->port == port && strcmp(OSCSenders[i]->name.c_str(), sender->name.c_str()) == 0) newSender = false;
			newOSCSender = sender;
		}
		
		if (newSender){			
			cout<<"DUPLICATING SENDER "<<sender->name<<" TO "<<host<<":"<<port<<endl;
			newOSCSender = new ofxLABOSCSender(host, port, sender->name, OSCSenders.size());
			for (int i=0; i<sender->variablePointers.size(); i++){
				newOSCSender->addVariable(sender->variablePointers[i]);
			};
			
			addOscSender(newOSCSender);
		} else {
			cout<<"SENDER "<<sender->name<<" AT "<<host<<":"<<" ALREADY EXISTS, DUMMY!"<<endl;
		}
		return newOSCSender;
	};

/***************************************************************
 
	 LOAD XML VARIABLES
	 
***************************************************************/

	void ofxLABnet::loadXMLVariables(){
		if(XML.loadFile("config.xml")){
			labOSCPort	= XML.getValue("labOSCPort", labOSCPort, 0);
		}
		saveXMLVariables();
	};

/***************************************************************
 
	LOAD XML VARIABLES
 
***************************************************************/

	void ofxLABnet::saveXMLVariables(){
		if (XML.tagExists("labOSCPort", 0)){
			XML.setValue("labOSCPort", labOSCPort, 0);
		} else {
			XML.loadFile("config.xml");
			XML.addTag("labOSCPort");
			XML.setValue("labOSCPort", labOSCPort, 0);
		}
	};

