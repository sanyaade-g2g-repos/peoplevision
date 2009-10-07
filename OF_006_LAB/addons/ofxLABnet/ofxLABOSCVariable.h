/*
 *  ofxLABOSCVariable.h
 *  openFrameworks
 *
 *  Created by Brett "the Factor" Renfer on 6/3/09.
 *  Copyright 2009 Rockwell Group. All rights reserved.
 *
 */

#pragma once

/***************************************************************
	CONVENIENT STORAGE DOGGIE
***************************************************************/

class ofxLABOSCVariable
{
	
public:
	
	bool *				boolValue;
	vector <bool> *		boolValues;
	string *			stringValue;
	vector <string> *	stringValues;
	int *				intValue;
	vector <int> *		intValues;
	float *				floatValue;
	vector <float> *	floatValues;
	
	void (*boolFun)(bool);
	void (*stringFun)(string);
	void (*stringVectFun)(vector<string>);
	void (*floatFun)(float);
	void (*floatVectFun)(vector<float>);
	void (*intFun)(int);
	void (*intVectFun)(vector<int>);
	
	bool		active, sent;
	
	string messageString;
	string type;
	int myId;
	int index;
	
	ofxLABOSCVariable( string _messageString, string _type, int _id){
		messageString = _messageString;
		type		= _type;
		myId			= _id;
		index		= 0;
		active		= true;
		sent		= false;
	}
	
	ofxLABOSCVariable( string _messageString, int _index, string _type, int _id){
		messageString = _messageString;
		type		= _type;
		myId			= _id;
		index		= _index;
		active		= true;
		sent		= false;
	}
	
	//set pointer
	
	void setPointer( bool * val ){
		boolValue = val;
	}
	
	void setPointer( vector <bool> * val ){
		boolValues = val;
	}
	
	void setPointer( string * val ){
		stringValue = val;
	}
	
	void setPointer( vector <string> * val ){
		stringValues = val;
	}
	
	void setPointer( int * val ){
		intValue = val;
	}
	
	void setPointer( vector <int> * val ){
		intValues = val;
	}
	
	void setPointer( float * val ){
		floatValue = val;
	}
	
	void setPointer( vector <float> * val ){
		floatValues = val;
	}
	
	//functions
	
	void setPointer( void (*_boolFun)(bool) ){
		boolFun = *_boolFun;
	}
	
	void setPointer( void (*_stringFun)(string) ){
		stringFun = _stringFun;
	}
	
	void setPointer( void (*_stringFun)(vector<string>) ){
		stringVectFun = _stringFun;
	}
	
	void setPointer( void (*_floatFun)(float) ){
		floatFun = _floatFun;
	}
	
	void setPointer( void (*_floatFun)(vector<float>) ){
		floatVectFun = _floatFun;
	}
	
	void setPointer( void (*_intFun)(int) ){
		intFun = _intFun;
	}
	
	void setPointer( void (*_intFun)(vector<int>) ){
		intVectFun = _intFun;
	}
	
	//set value
	
	void setValue( bool val ){
		*boolValue = val;
	}
	
	void setValue( vector <bool> val ){
		*boolValues = val;
	}
	
	void setValue( string val ){
		*stringValue = val;
	}
	
	void setValue( vector <string> val ){
		stringValues->clear();
		*stringValues = val;
	}
	
	void setValue( int val ){
		*intValue = val;
	}
	
	void setValue( vector <int>  val ){
		intValues->clear();
		*intValues = val;
	}
	
	void setValue( float val ){
		*floatValue = val;
	}
	
	void setValue( vector <float> val ){
		floatValues->clear();
		*floatValues = val;
	}
	
	//call callback
	
	void executeCallback( bool _bool){
		boolFun (_bool);
	}
	
	void executeCallback( string _string){
		cout<<"ABOUT TO CALL "<<_string<<endl;
		stringFun(_string);
	}
	
	void executeCallback( vector <string> stringVect){
		//cout<<"ABOUT TO CALL "<<_string<<endl;
		stringVectFun(stringVect);
	}
	
	void executeCallback( float _float){
		floatFun(_float);
	}
	
	void executeCallback( vector <float> floatVect){
		//cout<<"ABOUT TO CALL "<<_string<<endl;
		floatVectFun(floatVect);
	}
	
	void executeCallback( int _int){
		intFun(_int);
	}
	
	void executeCallback( vector <int> intVect){
		//cout<<"ABOUT TO CALL "<<_string<<endl;
		intVectFun(intVect);
	}
};