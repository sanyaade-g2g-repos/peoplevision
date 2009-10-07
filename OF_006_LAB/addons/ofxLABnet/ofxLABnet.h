/*
 *  ofxLABnet.h
 *  openFrameworks
 *
 *  Created by Brett "the Factor" Renfer on 6/3/09.
 *  Copyright 2009 Rockwell Group. All rights reserved.
 *
 
 This file is part of the ofxLABnet openFrameworks addon. It's some
 pretty scary business.
 
 ofxLABnet is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version. However, if you chose to do so, it
 may self replicate and take over your home. Thus, you will become
 an accomplice in the overthrowing of the human race. 
 
 ofxLABnet is distributed without the hope that it will be useful,
 and WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details, if you dare.
 
 You should have received a copy of the GNU Lesser General Public License
 along with ofxLABnet.  If not, see <http://www.gnu.org/licenses/>.
 
 */

#ifndef OFX_LABNET
#define OFX_LABNET

#include "ofxLABnetConstants.h"
#include "ofxOsc.h"
#include "ofxLABOSCVariable.h"
#include "ofxLABOSCListener.h"
#include "ofxLABOSCSender.h"
#include "ofxXmlSettings.h"

class ofxLABnet
{
	
	public :
		
		vector <ofxLABOSCListener *> OSClisteners;
		vector <ofxLABOSCSender *> OSCSenders;
		
		ofxLABOSCListener * switchListener;
		bool initialized;
	
		ofxLABnet();
		ofxLABnet(int port);
		
		void update();
		void draw();
		void send();
		void send( ofxOscMessage m );
		
		//xml things
		ofxXmlSettings XML;
		void loadXMLVariables();
		void saveXMLVariables();
		
		void addOSCListener(ofxLABOSCListener * listener);
		ofxLABOSCListener * addOSCListener(int _port, string _name);
	
		void addOscSender(ofxLABOSCSender * sender);
		ofxLABOSCSender * addOscSender(string _host, int _port, string _name);
	
		//void duplicateOscSender(ofxLABOSCSender * sender, string host, int port);
		ofxLABOSCSender *  duplicateOscSender(ofxLABOSCSender * sender, string host, int port);
		void destroy (ofxLABOSCSender * sender);
		void destroy (string sender);
};


#endif