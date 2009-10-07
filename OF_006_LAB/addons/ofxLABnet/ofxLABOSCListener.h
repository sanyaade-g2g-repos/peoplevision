/*
 *  ofxLABOSClstener.h
 *  openFrameworks
 *
 *  Created by Brett "the Factor" Renfer on 6/3/09.
 *  Copyright 2009 Rockwell Group. All rights reserved.
 *
 
 ADD THIS BONUS FUNCtiON to ofxOscReciever, if you don't mind
 
 void closeSocket(){
	releaseMutex();
	listen_socket->AsynchronousBreak();
	messages.clear();
	delete thread;
	delete listen_socket;
 }
 
 
 */

#ifndef OFX_LABNET_LISTENER
#define OFX_LABNET_LISTENER
#define NUM_MSG_STRINGS 10

#include "ofMain.h"
#include "ofxLABnet.h"
#include "ofxLABnetConstants.h"
#include "ofxOsc.h"
#include "ofxXmlSettings.h"

class ofxLABOSCListener
{
	
	public :
		int		port, oldport, myId;
		string	name;
		
		bool	listening, destroy;
		vector <ofxLABOSCVariable *> variablePointers;
		
		int				current_msg_string;
		std::string		msg_strings[NUM_MSG_STRINGS];
		float			timers[NUM_MSG_STRINGS];
		
		//had to make this separate instead of extending the class bc of
		//re-routing crashes ;(
		ofxOscReceiver * receiver;
		ofxXmlSettings XML;
	
	/***************************************************************
	 
		CONSTRUCTOR
	 
	***************************************************************/
	
			ofxLABOSCListener(int _port){
				port = oldport = _port;
				listening = true;
				receiver = new ofxOscReceiver();
				receiver->setup( port );
				name = "";
				myId = 0;
			};
	
			ofxLABOSCListener(int _port, string _name, int _id){
				port = oldport = _port;
				name = _name;
				myId   = _id;
				
				loadXMLVariables();
				
				listening = true;
				receiver = new ofxOscReceiver();
				
				try {
					receiver->setup( port );
					cout<<"LISTENING @ "<<port<<":"<<name<<endl;
				}
				catch (std::runtime_error){
					cout<<"ERROR SETTING UP PORT! :::"<<port<<":"<<name<<endl;
				}								
			};
		
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
		
		void addVariable (bool * boolPointer, string messageString, int index){
			
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, index, "bool", variablePointers.size() );
			variable->setPointer( boolPointer );
			variable->setValue( *boolPointer );
			
			variablePointers.push_back(variable);
		}
	
		//bool function
	
		void addVariable (void (*boolPointer)(bool) , string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "boolfunction", variablePointers.size() );
			variable->setPointer( boolPointer );
			variablePointers.push_back(variable);
		}
		
		//bool vector
		
		void addVariable (vector <bool> * boolPointer, string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "boolvector", variablePointers.size() );
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
	
		void addVariable (string * stringPointer, string messageString, int index){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, index, "string", variablePointers.size() );
			variable->setPointer( stringPointer );
			variable->setValue( *stringPointer );
			
			variablePointers.push_back(variable);
		}
	
		//string function
		
		void addVariable (void (*stringPointer)(string) , string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "stringfunction", variablePointers.size() );
			variable->setPointer( stringPointer );
			variablePointers.push_back(variable);
		}
	
		void addVariable (void (*stringPointer)(vector<string>) , string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "stringvectorfunction", variablePointers.size() );
			variable->setPointer( stringPointer );
			variablePointers.push_back(variable);
		}
	
		//string vector
		
		void addVariable (vector <string> * boolPointer, string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "stringvector", variablePointers.size() );
			variable->setPointer( boolPointer );
			variable->setValue( *boolPointer );
			variablePointers.push_back(variable);
		}
	
		//int
	
		void addVariable (int * intPointer, string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "int", variablePointers.size() );
			variable->setPointer( intPointer );
			variable->setValue( *intPointer );			
			variablePointers.push_back(variable);
		}
	
		void addVariable (int * intPointer, string messageString, int index){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, index, "int", variablePointers.size() );
			variable->setPointer( intPointer );
			variable->setValue( *intPointer );
			variablePointers.push_back(variable);
		}
		
		//int function
		
		void addVariable (void (*intPointer)(int) , string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "intfunction", variablePointers.size() );
			variable->setPointer( intPointer );
			variablePointers.push_back(variable);
		}
	
		
		void addVariable (void (*intPointer)(vector<int>) , string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "intvectorfunction", variablePointers.size() );
			variable->setPointer( intPointer );
			variablePointers.push_back(variable);
		}
		
		//int vector
		
		void addVariable (vector <int> * intPointer, string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "intvector", variablePointers.size() );
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

		void addVariable (float * floatPointer, string messageString, int index){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, index, "float", variablePointers.size() );
			variable->setPointer( floatPointer );
			variable->setValue( *floatPointer );
			
			variablePointers.push_back(variable);
		}

		//float function
		
		void addVariable (void (*floatPointer)(float) , string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "floatfunction", variablePointers.size() );
			variable->setPointer( floatPointer );
			variablePointers.push_back(variable);
		}
		
		
		void addVariable (void (*floatPointer)(vector<float>) , string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "floatvectorfunction", variablePointers.size() );
			variable->setPointer( floatPointer );
			variablePointers.push_back(variable);
		}
	
		//float vector
		
		void addVariable (vector <float> * boolPointer, string messageString){
			ofxLABOSCVariable * variable = new ofxLABOSCVariable ( messageString, "intvector", variablePointers.size() );
			variable->setPointer( boolPointer );
			variable->setValue( *boolPointer );
			variablePointers.push_back(variable);
		}

	/***************************************************************
	 
		UPDATE
		
		- get all new osc messages + update all the pointers
	 
	***************************************************************/
	
		void update(){
			if (listening){
				
				// hide old messages
				for ( int i=0; i<NUM_MSG_STRINGS; i++ )
				{
					if ( timers[i] < ofGetElapsedTimef() )
						msg_strings[i] = "";
				}
				
				// check for waiting messages
				while( receiver->hasWaitingMessages() )
				{
					// get the next message
					ofxOscMessage m;
					receiver->getNextMessage( &m );
					
					bool found = false;
					
					string msg_string;
					msg_string = m.getAddress();
					msg_string += ": ";
					msg_string += m.getArgType( 0 );
					msg_string += ": ";
					for ( int i=0; i<m.getNumArgs(); i++ )
					{
						// get the argument type
						msg_string += m.getArgTypeName( i );
						msg_string += ":";
						// display the argument - make sure we get the right type
						if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
							msg_string += ofToString( m.getArgAsInt32( i ) );
						else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
							msg_string += ofToString( m.getArgAsFloat( i ) );
						else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
							msg_string += m.getArgAsString( i );
						else
							msg_string += "unknown";
					}
					for (int i=0; i < variablePointers.size(); i++){
						if (m.getAddress() == variablePointers[i]->messageString){
							found = true;
							if ( variablePointers[i]->active ){
								if( variablePointers[i]->type == "int" ){
									if (m.getArgType(variablePointers[i]->index) == OFXOSC_TYPE_INT32){
										variablePointers[i]->setValue( m.getArgAsInt32( variablePointers[i]->index )) ;
									} else if( m.getArgType( variablePointers[i]->index ) == OFXOSC_TYPE_FLOAT ){
										variablePointers[i]->setValue( (int) m.getArgAsFloat( variablePointers[i]->index )) ;
									} else if ( m.getArgType( variablePointers[i]->index ) == OFXOSC_TYPE_STRING ){
										variablePointers[i]->setValue( atoi( m.getArgAsString( variablePointers[i]->index ).c_str()) ) ;
									}
								} else if( variablePointers[i]->type == "string" ){
									variablePointers[i]->setValue( (string) m.getArgAsString( variablePointers[i]->index ));
								} else if( variablePointers[i]->type == "float" ){
									variablePointers[i]->setValue( m.getArgAsFloat( variablePointers[i]->index ));
								} else if( variablePointers[i]->type == "bool" ){
									
									if (m.getArgType(variablePointers[i]->index) == OFXOSC_TYPE_INT32){
										variablePointers[i]->setValue( (bool) m.getArgAsInt32( variablePointers[i]->index )) ;
									} else if( m.getArgType( variablePointers[i]->index ) == OFXOSC_TYPE_FLOAT ){
										variablePointers[i]->setValue( (bool) m.getArgAsFloat( variablePointers[i]->index ));
									} else if ( m.getArgType( variablePointers[i]->index ) == OFXOSC_TYPE_STRING ){
										string boolString = m.getArgAsString( variablePointers[i]->index);
										
										if (strcmp(boolString.c_str(), "true")){
											variablePointers[i]->setValue( true ) ;
										} else {
											variablePointers[i]->setValue( false ) ;
										}
									}
									
								} else if( variablePointers[i]->type == "intvector" ){
									vector <int> tempVector;
									
									for ( int j=0; j<m.getNumArgs(); j++ ){
										tempVector.push_back(m.getArgAsInt32( j ));
									}
									variablePointers[i]->setValue( tempVector );
								} else if( variablePointers[i]->type == "stringvector" ){
									vector <string> tempVector;
									
									for ( int j=0; j<m.getNumArgs(); j++ ){
										tempVector.push_back(m.getArgAsString( j ));
									}
									variablePointers[i]->setValue( tempVector );
								} else if( variablePointers[i]->type == "floatvector" ){
									vector <float> tempVector;
									
									for ( int j=0; j<m.getNumArgs(); j++ ){
										tempVector.push_back(m.getArgAsFloat( j ));
									}
									variablePointers[i]->setValue( tempVector );
								} else if( variablePointers[i]->type == "boolvector" ){
									vector <bool> tempVector;
									
									for ( int j=0; j<m.getNumArgs(); j++ ){
										if (m.getArgType(j) == OFXOSC_TYPE_INT32){
											tempVector.push_back( (bool) m.getArgAsInt32( j )) ;
										} else if( m.getArgType( j ) == OFXOSC_TYPE_FLOAT ){
											tempVector.push_back( (bool) m.getArgAsFloat( j )) ;
										} else if ( m.getArgType( j ) == OFXOSC_TYPE_STRING ){
											string boolString = m.getArgAsString( j );
											
											if (strcmp(boolString.c_str(), "true")){
												tempVector.push_back( true ) ;
											} else {
												tempVector.push_back( false ) ;
											}
										}
									}
									variablePointers[i]->setValue( tempVector );
								} else if( variablePointers[i]->type == "intfunction" ){
									if (m.getArgType(variablePointers[i]->index) == OFXOSC_TYPE_INT32){
										variablePointers[i]->executeCallback( m.getArgAsInt32( variablePointers[i]->index )) ;
									} else if( m.getArgType( variablePointers[i]->index ) == OFXOSC_TYPE_FLOAT ){
										variablePointers[i]->executeCallback( (int) m.getArgAsFloat( variablePointers[i]->index )) ;
									} else if ( m.getArgType( variablePointers[i]->index ) == OFXOSC_TYPE_STRING ){
										variablePointers[i]->executeCallback( atoi( m.getArgAsString( variablePointers[i]->index ).c_str()) ) ;
									}
								}  else if ( variablePointers[i]->type == "intvectorfunction" ){
									
									vector <int> tempVector;
									
									for ( int j=0; j<m.getNumArgs(); j++ ){
										if (m.getArgType(variablePointers[i]->index) == OFXOSC_TYPE_INT32){
											tempVector.push_back( m.getArgAsInt32( variablePointers[i]->index )) ;
										} else if( m.getArgType( variablePointers[i]->index ) == OFXOSC_TYPE_FLOAT ){
											tempVector.push_back( (int) m.getArgAsFloat( variablePointers[i]->index )) ;
										} else if ( m.getArgType( variablePointers[i]->index ) == OFXOSC_TYPE_STRING ){
											tempVector.push_back( atoi( m.getArgAsString( variablePointers[i]->index ).c_str()) ) ;
										}
									}
									
									variablePointers[i]->executeCallback( tempVector );							
								} else if( variablePointers[i]->type == "stringfunction" ){
									variablePointers[i]->executeCallback( (string) m.getArgAsString( variablePointers[i]->index ));
								} else if ( variablePointers[i]->type == "stringvectorfunction" ){
									
									vector <string> tempVector;
									
									for ( int j=0; j<m.getNumArgs(); j++ ){
										tempVector.push_back( m.getArgAsString( j )) ;
									}
									
									variablePointers[i]->executeCallback( tempVector );							
								} else if( variablePointers[i]->type == "floatfunction" ){
									variablePointers[i]->executeCallback( m.getArgAsFloat( variablePointers[i]->index ));
								} else if ( variablePointers[i]->type == "floatvectorfunction" ){
									
									vector <float> tempVector;
									
									for ( int j=0; j<m.getNumArgs(); j++ ){
										tempVector.push_back( m.getArgAsFloat( j )) ;
									}
									
									variablePointers[i]->executeCallback( tempVector );							
								} else if( variablePointers[i]->type == "boolfunction" ){
									
									if (m.getArgType(variablePointers[i]->index) == OFXOSC_TYPE_INT32){
										variablePointers[i]->executeCallback( (bool) m.getArgAsInt32( variablePointers[i]->index )) ;
									} else if( m.getArgType( variablePointers[i]->index ) == OFXOSC_TYPE_FLOAT ){
										variablePointers[i]->executeCallback( (bool) m.getArgAsFloat( variablePointers[i]->index ));
									} else if ( m.getArgType( variablePointers[i]->index ) == OFXOSC_TYPE_STRING ){
										string boolString = m.getArgAsString( variablePointers[i]->index);
										
										if (strcmp(boolString.c_str(), "true")){
											variablePointers[i]->executeCallback( true ) ;
										} else {
											variablePointers[i]->executeCallback( false ) ;
										}
									}
									
								} 
							} else {
								cout<<"VAR IS INACTIVE"<<endl;
							}
						}
					}
										
					if (!found){
						cout<<m.getAddress()<<" NOT FOUND!"<<endl;
						/*
						// unrecognized message: display on the bottom of the screen
						string msg_string;
						msg_string = m.getAddress();
						msg_string += ": ";
						for ( int i=0; i<m.getNumArgs(); i++ )
						{
							// get the argument type
							msg_string += m.getArgTypeName( i );
							msg_string += ":";
							// display the argument - make sure we get the right type
							if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
								msg_string += ofToString( m.getArgAsInt32( i ) );
							else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
								msg_string += ofToString( m.getArgAsFloat( i ) );
							else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
								msg_string += m.getArgAsString( i );
							else
								msg_string += "unknown";
						}
						// add to the list of strings to display
						msg_strings[current_msg_string] = msg_string;
						timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
						current_msg_string = ( current_msg_string + 1 ) % NUM_MSG_STRINGS;
						// clear the next line
						msg_strings[current_msg_string] = "";
						 */
					}
				}
				
			}
			
			if (oldport != port){
				port = max (port, 2000);
				reroute(port);
				oldport = port;
			}
		}
		
	/***************************************************************
	 
		RE-ROUTE (AKA SEND TO A DIFF PORT)
	 
	***************************************************************/
	
		void reroute(int _port){
			cout<<"REROUTE TO "<<_port<<endl;
			port = _port;
			receiver->closeSocket();
			receiver = new ofxOscReceiver();
			receiver->setup( port );
			saveXMLVariables();
		};
		
	
	/***************************************************************
	 
		DEACTIVATE / ACTIVATE A VAR
	 
	***************************************************************/
	
		bool deactivate ( string messageString){
			for (int i=0; i < variablePointers.size(); i++){
				if (variablePointers[i]->messageString == messageString){
					variablePointers[i]->active = false;
				}
			}			
		}
	
		bool activate ( string messageString){
			for (int i=0; i < variablePointers.size(); i++){
				if (variablePointers[i]->messageString == messageString){
					variablePointers[i]->active = true;
				}
			}			
		};
	
	/***************************************************************
	 
		CHECK XML FOR SAVED PORT
	 
	***************************************************************/
	
		void loadXMLVariables(){
			XML.loadFile("config.xml");
			cout<<"loading variables for "<<name<<endl;
			bool listeners = XML.pushTag("listeners", 0);
			if (listeners){
				int	numListeners = XML.getNumTags("listener");
				if (numListeners > 0){
					for (int i=0; i<numListeners; i++){
						XML.pushTag("listener", i);
						string curName = XML.getValue("name", "", 0);
						int curId = XML.getValue("id", 0, 0);
						int curPort = XML.getValue("port", 0, 0);
						if (strcmp(curName.c_str(),name.c_str()) == 0 && curId == myId){
							port = oldport = curPort;
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
			bool listeners = XML.pushTag("listeners", 0);
			if (listeners){
				int	numListeners = XML.getNumTags("listener");
				if (numListeners > 0){
					
					bool found = false;
					
					for (int i=0; i<numListeners; i++){
						XML.pushTag("listener", i);
						string curName = XML.getValue("name", "", 0);
						int curId = XML.getValue("id", 0, 0);
						int curPort = XML.getValue("port", 0, 0);
						if (strcmp(curName.c_str(),name.c_str()) == 0 && curId == myId){
							found = true;
							XML.setValue("port", port, 0);
						}
						XML.popTag();
					} 
					
					if (!found){
						cout<<"LISTENER "<<name<<" NOT FOUND"<<endl;
						XML.addTag("listener");
						XML.pushTag("listener", numListeners);
							XML.setValue("id", myId, 0);
							XML.setValue("name", name, 0);
							XML.setValue("port", port, 0);
						XML.popTag();
					}
				} else {					
					cout<<"LISTENER "<<name<<" NOT FOUND"<<endl;
					XML.addTag("listener");
					XML.pushTag("listener", numListeners);
						XML.setValue("id", myId, 0);
						XML.setValue("name", name, 0);
						XML.setValue("port", port, 0);
					XML.popTag();
				}
				
				XML.popTag();
			} else {
				XML.addTag("listeners");
				XML.pushTag("listeners", 0);
					XML.addTag("listener");
					XML.pushTag("listener", 0);
						XML.setValue("id", myId, 0);
						XML.setValue("name", name, 0);
						XML.setValue("port", port, 0);
					XML.popTag();			
				XML.popTag();							
			}
			XML.saveFile("config.xml");
		};
	
	/***************************************************************
	 
		USEFUL OPERATORS
	 
	***************************************************************/
	
		bool operator==( ofxLABOSCListener * testListener){
			return testListener->name == name;
		};
	
};


#endif
