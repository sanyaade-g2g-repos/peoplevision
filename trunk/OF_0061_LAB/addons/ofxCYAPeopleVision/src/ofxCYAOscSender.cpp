/***********************************************************************
 
 ofxCYAOSCSender.cpp
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

#include "ofxCYAOSCSender.h"

/***************************************************************
 CONSTRUCTOR + SETUP
 ***************************************************************/

ofxCYAOscSender::ofxCYAOscSender(){};

ofxCYAOscSender::ofxCYAOscSender(string _ip, int _port){
	setupSender(_ip, _port);
};

void ofxCYAOscSender::setupSender(string _ip, int _port){
	ip = oldip = _ip;
	port = oldport = _port;
	
	stringstream message;
	message<<"SETTING UP SENDER @ "<<ip<<":"<<port;
	ofLog(OF_LOG_VERBOSE, message.str());
	
	setup(ip, port);
};

/***************************************************************
 UPDATE
 ***************************************************************/

void ofxCYAOscSender::update(){
	if (strcmp(oldip.c_str(), ip.c_str()) != 0 || oldport != port){
		oldip = ip;
		oldport = port;
		reroute(ip, port);				
	}
}

/***************************************************************
 SEND
 ***************************************************************/

void ofxCYAOscSender::send ( ofxCYAPerson * p, ofPoint centroid ){
	ofxOscBundle b;
	
	stringstream address;
	address<<"cya/person/"<<p->oid;
	
	ofxOscMessage m;
	m.setAddress(address.str());
	m.addIntArg(p->pid);
	m.addIntArg(p->age);
	m.addFloatArg(centroid.x);
	m.addFloatArg(centroid.y);
	m.addFloatArg(p->velocity.x);
	m.addFloatArg(p->velocity.y);
	
	ofRectangle boundingRect = p->getBoundingRectNormalized(640,480);
	
	m.addFloatArg(boundingRect.x);
	m.addFloatArg(boundingRect.y);
	m.addFloatArg(boundingRect.width);
	m.addFloatArg(boundingRect.height);
	
	send(m);
};

void ofxCYAOscSender::send ( ofxOscMessage m ){
	sendMessage(m);
};

void ofxCYAOscSender::kill ( ofxCYAPerson * p )
{
	ofxOscBundle b;
	
	stringstream address;
	address<<"cya/kill/"<<p->oid;
	
	ofxOscMessage m;
	m.setAddress(address.str());
	m.addIntArg(p->pid);
	
	send(m);
	
};

/***************************************************************
 REROUTE
 ***************************************************************/	

void ofxCYAOscSender::reroute(string _ip, int _port){
	stringstream portstream;
	portstream<<_port;
	ofLog(OF_LOG_VERBOSE, "ofxCYAOscSender: REROUTING TO "+ip+", "+portstream.str());
	ip = _ip;
	port = _port;
	setup(ip, port);
};