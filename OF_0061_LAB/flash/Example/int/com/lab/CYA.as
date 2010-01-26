//Accepting UDP from Memo's excellent UDP-TCP Bridge: http://www.msavisuals.com/udp_tcp_bridge
package com.lab
{
	import flash.display.Sprite;
	import flash.errors.*;
	import flash.events.*;
	import flash.events.MessageEvent;
	import flash.events.OSCSocket;
	
	public class CYA extends Sprite
	{
		public var stageWidth:Number;
		public var stageHeight:Number;
		public var people:Array;
		
	 	private var socket:OSCSocket;
		private var host:String;
		private var port:uint;
	    private var response:String;
		
		public function CYA( _width:uint = 1024, _height:uint = 768 )
		{
			super();
			stageWidth = _width;
			stageHeight = _height;
			
			//build array of people
			people = [];
			
			//setup socket
			createSocket();		
		}

	/************************************************************************
		SOCKET			
	************************************************************************/	
		
		private function createSocket()
		{
			socket = new OSCSocket ();
			socket.addEventListener(Event.CLOSE, closeHandler);
			socket.addEventListener(Event.CONNECT, connectHandler);
			socket.addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);
			socket.addEventListener(SecurityErrorEvent.SECURITY_ERROR, securityErrorHandler);
		}
		
		public function connect( _host:String = "localhost", _port:uint = 3334 )
		{	
			host = _host;
			port = _port;
			socket.connect(host, port);			
			socket.addEventListener( MessageEvent.MESSAGE_RECEIVED, messageHandler );
		}
		
		public function get connected():Boolean
		{
			if (socket != null) return socket.connected;
			else return false;
		}
		
		//error handlers
				
		private function closeHandler(event:Event):void{
			dispatchEvent(event);
        }
     	private function connectHandler(event:Event):void{	
     		dispatchEvent(event);
        }               
        private function ioErrorHandler(event:IOErrorEvent):void{
        	dispatchEvent(event);
        }
        private function securityErrorHandler(event:SecurityErrorEvent):void{
        	dispatchEvent(event);
        }
	
	/************************************************************************
		CATCH INCOMING MESSAGES			
	************************************************************************/
				
		public function messageHandler ( e:MessageEvent )
		{
			var oscMessage:Array = e.data;
			var messageAddress:String = oscMessage[0];
			
			var person:CYAPerson = new CYAPerson(stageWidth, stageHeight);
			person.id = oscMessage[1];
			person.age = oscMessage[2];
			person.centroid.x = oscMessage[3]*stageWidth;
			person.centroid.y = oscMessage[4]*stageHeight;
			person.velocity.x = oscMessage[5]*stageWidth;
			person.velocity.y = oscMessage[6]*stageHeight;			
			person.x = oscMessage[7]*stageWidth;
			person.y = oscMessage[8]*stageHeight;
			person.realWidth = oscMessage[9]*stageWidth;
			person.realHeight = oscMessage[10]*stageHeight;
			person.opticalFlow.x = oscMessage[11]*stageWidth;
			person.opticalFlow.y = oscMessage[12]*stageHeight;
			
			//get contours
			for (var i:uint = 13; i<oscMessage.length-12; i+=2){
				var point:Object = new Object();
				point.x = oscMessage[i]*stageWidth;
				point.y = oscMessage[i+1]*stageHeight;
				person.contours.push( point );
			}
			
			if ( messageAddress == "cya/personEntered/"){
				personEntered(person);
			} else if ( messageAddress == "cya/personMoved/" ){
				personMoved(person);			
			} else if ( messageAddress == "cya/personWillLeave/" ){
				personLeft(person);				
			}
		}
		
	/************************************************************************
		PEOPLE EVENTS STUFF
		- listen for these in your main file			
	************************************************************************/
		
		public function personEntered( person:CYAPerson ):void {
			people.push(person);
			dispatchEvent( new CYAEvent( CYAEvent.PERSON_ENTERED, person ) );
		}
		
		public function personMoved( person:CYAPerson ):void {
			var found:Boolean = false;
			//double check to see if it's in the array yet
			for (var i:uint = 0; i<people.length; i++){
				if (people[i].id == person.id){
					people[i].update(person);
					found = true;
					break;
				}
			}
			
			//haven't dispatched a personentered yet, so do that and return
			if (!found){
				personEntered ( person );
				return;
			}
			
			dispatchEvent( new CYAEvent( CYAEvent.PERSON_MOVED, person ) );
		}
		
		public function personLeft( person:CYAPerson ):void {			
			for (var i:uint = 0; i<people.length; i++){
				if (people[i].id == person.id){
					people[i].dead = true;
					people.splice(i, 1);
					break;
				}
			}
			dispatchEvent( new CYAEvent( CYAEvent.PERSON_LEFT, person ) );			
		}
	}
}