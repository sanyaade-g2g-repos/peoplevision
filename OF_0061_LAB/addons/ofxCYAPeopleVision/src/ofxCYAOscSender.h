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
	string	ip, oldip;
	int		port, oldport;
	
	ofxCYAOscSender();
	ofxCYAOscSender(string _ip, int _port);
	void setupSender(string _ip, int _port);	
	void update();		
	void send ( ofxCYAPerson * p, ofPoint centroid );	
	void send ( ofxOscMessage m );
	void reroute(string _ip, int _port);

};