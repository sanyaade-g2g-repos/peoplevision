/*
 *  ofxLABOSCSender.h
 *  openFrameworks
 *
 *  Created by Brett "the Factor" Renfer on 6/4/09.
 *  Copyright 2009 Rockwell Group. All rights reserved.
 *
 */

#ifndef OFX_LABNET_OSCSENDER
#define OFX_LABNET_OSCSENDER
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

#include "ofxLABnet.h"

class ofxLABOSCSender : public ofxOscSender
{	
	public :
			
		string	host, oldhost, dupHost;
		int		port, oldport, dupPort, myId;
		string	name;
		bool	autosend, active, destroy;
	
		vector <ofxLABOSCVariable *> variablePointers;
		ofxXmlSettings XML;

	/***************************************************************
		CONSTRUCTOR
	***************************************************************/
	
		ofxLABOSCSender(string _host, int _port, string _name, int _id){
			myId = _id;
			active = true;
			destroy = false;
			host = oldhost = _host;
			port = oldport = _port;
			name = _name;
			
			loadXMLVariables();
			
			cout<<"SETTING UP SENDER "<<name<<" @ "<<host<<":"<<port<<endl;			
			
			setup(host, port);
			
			autosend = false;
			dupPort = 0;
			dupHost = "";
		};
		
		ofxLABOSCSender(string _host, int _port, string _name, int _id, bool _autosend){
			myId = _id;
			active = true;
			destroy = false;
			host = oldhost = _host;
			port = oldport = _port;
			name = _name;
			
			cout<<"SETTING UP SENDER "<<name<<" @ "<<host<<":"<<port<<endl;	
			
			loadXMLVariables();
			
			setup(host, port);
			autosend = _autosend;
			dupPort = 0;
			dupHost = "";
		};
	
	/***************************************************************
		UPDATE
	***************************************************************/
	
		void update(){
			if (strcmp(oldhost.c_str(), host.c_str()) != 0 || oldport != port){
				oldhost = host;
				oldport = port;
				reroute(host, port);				
			}
			
			/*if (dupPort != 0 && dupHost != ""){
				duplicate(dupHost, dupPort);
				dupPort = 0;
				dupHost = "";
			}*/
		}
	
	/***************************************************************
		SEND
	***************************************************************/
		
		//send all messages
	
		void send(){
			if (active){
				for (int i=0; i<variablePointers.size(); i++){
					if (!variablePointers[i]->sent){
						ofxOscMessage m;
						m.setAddress(variablePointers[i]->messageString);
						
						if (variablePointers[i]->type == "bool"){
							m.addIntArg( *(variablePointers[i]->boolValue) );
						} else if (variablePointers[i]->type == "int"){
							m.addIntArg( *(variablePointers[i]->intValue) );
						} else if (variablePointers[i]->type == "string"){
							m.addStringArg( *(variablePointers[i]->stringValue) );
						} else if (variablePointers[i]->type == "float"){
							m.addFloatArg( *(variablePointers[i]->floatValue) );
						}
						variablePointers[i]->sent = true;
						
						for (int j=0; j<variablePointers.size(); j++){
							if (variablePointers[j]->messageString == variablePointers[i]->messageString && !variablePointers[j]->sent){
								if (variablePointers[j]->type == "bool"){
									m.addIntArg( (int)*(variablePointers[j]->boolValue) );
								} else if (variablePointers[j]->type == "int"){
									m.addIntArg( *(variablePointers[j]->intValue) );
								} else if (variablePointers[j]->type == "string"){
									m.addStringArg( *(variablePointers[j]->stringValue) );
								} else if (variablePointers[j]->type == "float"){
									m.addFloatArg( *(variablePointers[j]->floatValue) );
								}
								variablePointers[j]->sent = true;
							}
						}
						
						sendMessage(m);
					}
				}
				
				for (int j=0; j<variablePointers.size(); j++){
					variablePointers[j]->sent = false;
				}
			}
		}
	
		//send a specific message by address
		void send( string messageString ){
			if (active){
				for (int i=0; i<variablePointers.size(); i++){
					if (variablePointers[i]->messageString == messageString && !variablePointers[i]->sent){
						ofxOscMessage m;
						m.setAddress(variablePointers[i]->messageString);
						
						if (variablePointers[i]->type == "bool"){
							m.addIntArg( (int)*(variablePointers[i]->boolValue) );
						} else if (variablePointers[i]->type == "int"){
							m.addIntArg( *(variablePointers[i]->intValue) );
						} else if (variablePointers[i]->type == "string"){
							m.addStringArg( *(variablePointers[i]->stringValue) );
						} else if (variablePointers[i]->type == "float"){
							m.addFloatArg( *(variablePointers[i]->floatValue) );
						}
						variablePointers[i]->sent = true;
						
						for (int j=0; j<variablePointers.size(); j++){
							if (variablePointers[j]->messageString == variablePointers[i]->messageString && !variablePointers[j]->sent){
								if (variablePointers[j]->type == "bool"){
									m.addIntArg( *(variablePointers[j]->boolValue) );
								} else if (variablePointers[j]->type == "int"){
									m.addIntArg( *(variablePointers[j]->intValue) );
								} else if (variablePointers[j]->type == "string"){
									m.addStringArg( *(variablePointers[j]->stringValue) );
								} else if (variablePointers[j]->type == "float"){
									m.addFloatArg( *(variablePointers[j]->floatValue) );
								}
								variablePointers[j]->sent = true;
							}
						}	
						
						sendMessage(m);
					}
				}
				
				
				for (int j=0; j<variablePointers.size(); j++){
					variablePointers[j]->sent = false;
				}
			}
		}
	
		
		void send ( ofxOscMessage m ){
			if (active) {
				sendMessage(m);
			}
		}
	
	/***************************************************************
		ADD VARIABLES OF VARIOUS TYPES
	***************************************************************/
	
		//bool
		
		void addVariable (bool * boolPointer, string messageString){
			
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "bool", variablePointers.size() );
			variable->setPointer( boolPointer );
			variable->setValue( *boolPointer );
			
			variablePointers.push_back(variable);
		}
		
		//string
		
		void addVariable (string * stringPointer, string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "string", variablePointers.size() );
			variable->setPointer( stringPointer );
			variable->setValue( *stringPointer );
			
			variablePointers.push_back(variable);
		}
		
		//int
		
		void addVariable (int * intPointer, string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "int", variablePointers.size() );
			variable->setPointer( intPointer );
			variable->setValue( *intPointer );			
			variablePointers.push_back(variable);
		}
		
		
		//float
		
		void addVariable (float * floatPointer, string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "float", variablePointers.size() );
			variable->setPointer( floatPointer );
			variable->setValue( *floatPointer );
			
			variablePointers.push_back(variable);
		}
	
		//add a variable pointer
		void addVariable (ofxLABOSCVariable * variable){			
			variablePointers.push_back(variable);
		}
		
	/***************************************************************
		REROUTE
	***************************************************************/	
	
		void reroute(string _host, int _port){
			active = true;
			destroy = false;
			cout<<"REROUTE TO "<<_host<<":"<<_port<<endl;
			host = _host;
			port = _port;
			setup(host, port);
			saveXMLVariables();
		};
	
	/***************************************************************
	 
		REROUTE

	***************************************************************/	
		
		void duplicate(string _host, int _port){
			//labNet->duplicateOscSender(this, _host, _port);
		};
	
	/***************************************************************
	 
		CHECK XML FOR SAVED PORT
	 
	***************************************************************/
		
		void loadXMLVariables(){
			XML.loadFile("config.xml");
			bool senders = XML.pushTag("senders", 0);
			if (senders){
				int	numSenders = XML.getNumTags("sender");
				if (numSenders > 0){
					for (int i=0; i<numSenders; i++){
						XML.pushTag("sender", i);
						string curName = XML.getValue("name", "", 0);
						string curHost = XML.getValue("host", "", 0);
						int curId = XML.getValue("id", 0, 0);
						int curPort = XML.getValue("port", 0, 0);
						if (strcmp(curName.c_str(),name.c_str()) == 0 && curId == myId){
							port = oldport = curPort;
							host =  oldhost = curHost;
						};
						XML.popTag();
					}
				}
				XML.popTag();
			}
			saveXMLVariables();
		};
	
	/***************************************************************
	 
		SAVE XML VARIABLES
	 
	***************************************************************/
		
		void saveXMLVariables(){
			XML.loadFile("config.xml");
			bool senders  = XML.pushTag("senders", 0);
			if (senders){
				int	numSenders = XML.getNumTags("sender");
				if (numSenders > 0){
					
					bool found = false;
					
					for (int i=0; i<numSenders; i++){
						XML.pushTag("sender", i);
						string curName = XML.getValue("name", "", 0);
						string curHost = XML.getValue("host", "", 0);
						int curId = XML.getValue("id", 0, 0);
						int curPort = XML.getValue("port", 0, 0);
						if (strcmp(curName.c_str(),name.c_str()) == 0 && curId == myId){
							found = true;
							XML.setValue("port", port);
							XML.setValue("host", host);
						};
						XML.popTag();
					}
					
					if (!found){
						int sender = XML.addTag("sender");
						cout<<"CREATING TAG at "<<sender<<":"<<endl;
						XML.pushTag("sender", sender);
							XML.addValue("id", myId);
							XML.addValue("name", name);
							XML.addValue("port", port);
							XML.addValue("host", host);
						XML.popTag();
					}
					
					XML.popTag();
				} else {				
					int sender = XML.addTag("sender");
					cout<<"CREATING TAG at "<<sender<<endl;
					XML.pushTag("sender", sender);
						XML.addValue("id", myId);
						XML.addValue("name", name);
						XML.addValue("port", port);
						XML.addValue("host", host);
					XML.popTag();
				}
							
				XML.popTag();
			} else {
				cout<<"ADDING TAG"<<endl;
				XML.addTag("senders");
				XML.pushTag("senders", 0);
				int senderId = XML.addTag("sender");
					cout<<"CREATING TAG at "<<senderId<<endl;
					XML.pushTag("sender", senderId);
						XML.addValue("id", myId);
						XML.addValue("name", name);
						XML.addValue("port", port);
						XML.addValue("host", host);
					XML.popTag();
				
				XML.popTag();
				
			}
			XML.saveFile("config.xml");
		};
		
	/***************************************************************
	 
		USEFUL OPERATORS
	 
	***************************************************************/
	
		bool operator==( ofxLABOSCSender * testSender){
			return (testSender->name == name) && (testSender->port == port) && (testSender->host == host) ;
		};
	
};

#endif