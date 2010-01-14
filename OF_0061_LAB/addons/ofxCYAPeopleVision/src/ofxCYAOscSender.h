/***********************************************************************
 
 ofxCYAOSCSender.h
 CYAPeopleVision

 Copyright (c) 2009, LAB at Rockwell Group
 http://lab.rockwellgroup.com
 Created by Brett Renfer on 1/14/10.
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 ***********************************************************************/

#pragma once

#include "ofxOSC.h"
#include "ofxCYAPerson.h"

class ofxCYAOscSender : public ofxOscSender
{
	public :
	
	//"old" variables are to check for reroute
	string	host, oldhost, dupHost;
	int		port, oldport, dupPort, myId;
	
	//vector <ofxECSVariable *> variablePointers;
	
	/***************************************************************
		CONSTRUCTOR + SETUP
	***************************************************************/
	
	ofxCYAOscSender(string _host, int _port){
		setupSender(_host, _port);
	};
	
	void setupSender(string _host, int _port){
		host = oldhost = _host;
		port = oldport = _port;
		
		stringstream message;
		message<<"SETTING UP SENDER @ "<<host<<":"<<port;
		ofLog(OF_LOG_VERBOSE, message.str());
				
		setup(host, port);
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
	}
	
	/***************************************************************
		SEND
	***************************************************************/
		
	void send ( ofxCYAPerson * p ){
		ofxOscBundle b;
		
		ofxOscMessage m;
		m.setAddress("cya/person");
	};
	
	void send ( ofxOscMessage m ){
		sendMessage(m);
	};
	
	/***************************************************************
		REROUTE
	***************************************************************/	
	
	void reroute(string _host, int _port){
		stringstream portstream;
		portstream<<_port;
		ofLog(OF_LOG_VERBOSE, "ofxCYAOscSender: REROUTING TO "+host+", "+portstream.str());
		host = _host;
		port = _port;
		setup(host, port);
	};

};