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
 
 OSC Message is structured like this:
 
 address: cya/person/ + ordered id (i.e. cya/person/0)
 
 argument 0: pid; 
 argument 1: age; 
 argument 2: centroid.x; 
 argument 3: centroid.y; 
 argument 4: velocity.x; 
 argument 5: velocity.y; 
 argument 6: boundingRect.x; 
 argument 7: boundingRect.y; 
 argument 8: boundingRect.width; 
 argument 9: boundingRect.height; 
 argument 10: opticalFlowVectorAccumulation.x; 
 argument 11: opticalFlowVectorAccumulation.y;
 argument 12+ : contours (if enabled)
 
 
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
	void personEntered ( ofxCYAPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
	void personMoved ( ofxCYAPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
	void personUpdated ( ofxCYAPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );	
	void personWillLeave ( ofxCYAPerson * p, ofPoint centroid, int cameraWidth, int cameraHeight, bool sendContours = false );
	
	void send ( ofxOscMessage m );
	void reroute(string _ip, int _port);

};