package
{
	import flash.events.*;
	import flash.display.MovieClip;
	import flash.display.Stage;
	import flash.text.TextField;
	import flash.utils.setTimeout;
	
	//import the lab CYA files
	
	import com.lab.*;
	
	public class CYAExample extends MovieClip
	{
		public var cyaServer:CYA;		
		public var stageWidth:Number;
		public var stageHeight:Number;		
		public var customPeople:Array;
		
		public var currentColor:uint = 0;
		
		public function CYAExample()
		{
			super();
			
			// store variables of stage width + height
			// CYA sends out coordinates from 0 - 1, so you need to scale them to
			// the size that you want (usually the stage width + height)
			// - for this app, there is a MC on the stage called activeArea_mc that
			// 	 we're using for the active area
			
			stageWidth = activeArea_mc.width; //stage.stageWidth;
			stageHeight = activeArea_mc.height; //stage.stageHeight;
			
			// build array of your custom people objects
			customPeople = [];
			
			// start cya server
			cyaServer = new CYA(stageWidth, stageHeight);
			
			//listen for three key CYA events: person entered, person moved, person left
			// - person entered is only called the first time a person appears
			// - person moved gets called each time a person gets updated
			// - person left gets called when a person dissapears (when they are no longer tracked)
			
			cyaServer.addEventListener( CYAEvent.PERSON_ENTERED, personEntered);
			cyaServer.addEventListener( CYAEvent.PERSON_MOVED, personMoved);
			cyaServer.addEventListener( CYAEvent.PERSON_LEFT, personLeft);
			
			//listen for connection information from CYA
			// - standard events from Socket class
			// - most useful are connect, close, and ioerror (ioerror is dispatched when there is an
			// 	 error connecting the socket)
			cyaServer.addEventListener(Event.CONNECT, connectHandler);
			cyaServer.addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);
			cyaServer.addEventListener(Event.CLOSE, closeHandler);
			
			//connect CYA
			// - this defaults to host as "localhost" and port as 3334, so you can
			// 	 just call connect() if all is default; otherwise, you can call
			//	 connect (host, port)
			
			cyaServer.connect();
												
			numberOfPeople_mc.number_txt.text = String(customPeople.length);
		}
		
		/************************************************************************
			SOCKET EVENTS			
		************************************************************************/
		
			public function connectHandler ( e:Event ){
				//do something when connected
				trace ("connected");
				connected_mc.visible = false;
			}
			
			public function ioErrorHandler ( e:IOErrorEvent ){
				trace ("io error! "+e.text);
				//is this error that we weren't able to connect?
				// if so, try again in 1 second
				if (cyaServer.connected == false) setTimeout(cyaServer.connect, 1500);
			}
			
			public function closeHandler ( e:Event ){
				trace ("closed!");
				connected_mc.visible = true;
				if (cyaServer.connected == false) setTimeout(cyaServer.connect, 1500);
			}
		
		
		/************************************************************************
			PEOPLE EVENT CATCHERS
			- add these to listen for specific things from CYA			
		************************************************************************/
			
			public function personEntered( e:CYAEvent ):void {
				// create a new custom person + pass it the person parameter of the CYAEvent
				var newCustomPerson:CustomPerson = new CustomPerson( e.person );
				newCustomPerson.setBackground(currentColor);
				currentColor++;
				if (currentColor > 20) currentColor = 0;
				
				addChild(newCustomPerson);
				customPeople.push( newCustomPerson );
				
				personEntered_mc.gotoAndPlay("active");
				numberOfPeople_mc.number_txt.text = customPeople.length;
			};

			public function personMoved( e:CYAEvent ):void {
				//don't do any updating; we are listening for the person's update in the CustomPerson class
				personUpdated_mc.gotoAndPlay("active");
				numberOfPeople_mc.number_txt.text = customPeople.length;
			};

			public function personLeft( e:CYAEvent ):void {
				// find the person that left by id + remove from array
				for (var i:uint = 0; i<customPeople.length; i++){
					if (customPeople[i].person.id == e.person.id){ 
						var oldCustomPerson = customPeople[i];
						removeChild(oldCustomPerson);
						customPeople.splice( i, 1 );
						break;
					}
				}
				personLeft_mc.gotoAndPlay("active");
				numberOfPeople_mc.number_txt.text = customPeople.length;
			};
	}
}