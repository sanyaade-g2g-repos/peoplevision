/**
*
*
*	OSC Socket
*	
*	@notice		OSC over TCP Listener
*	@author		Dean North
*	@version	1.0
* 	@date		07/02/2009
* 	@link		http://www.TouchGateway.com
* 
* 
*	Original author :
*	-----------------
*	Dean North aka Pleh
*	dean_north@hotmail.com
*	http://www.TouchGateway.com
*
* 	AUTHOR ******************************************************************************
* 
*	authorName : 	Dean North - www.TouchGateway.com
* 	contribution : 	the original class
* 	date :			07/02/2009
*
*	LICENSE ******************************************************************************
* 
* 	This class is under RECIPROCAL PUBLIC LICENSE 1.5.
* 	http://www.opensource.org/licenses/rpl1.5.txt
* 
* 	Please, keep this header and the list of all authors
* 
*
*/
package flash.events
{
	import flash.net.Socket;
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	public class OSCSocket extends EventDispatcher
	{
		
		private var socket:Socket;	
		
		public function OSCSocket()
		{
			socket = new Socket();	
			socket.addEventListener(Event.CLOSE, closeHandler);
			socket.addEventListener(Event.CONNECT, connectHandler);
			socket.addEventListener(ProgressEvent.SOCKET_DATA, dataHandler);
			socket.addEventListener(IOErrorEvent.IO_ERROR, ioErrorHandler);
			socket.addEventListener(SecurityErrorEvent.SECURITY_ERROR, securityErrorHandler);
		}
		
		public function get connected():Boolean{
			return socket.connected;
		}
		
		private function closeHandler(event:Event):void{
			this.dispatchEvent(event);
        }
     	private function connectHandler(event:Event):void{	
     		this.dispatchEvent(event);			
        }               
        private function ioErrorHandler(event:IOErrorEvent):void{
        	this.dispatchEvent(event);
        }
        private function securityErrorHandler(event:SecurityErrorEvent):void{
        	this.dispatchEvent(event);
        }
        
        private function dataHandler(e:ProgressEvent):void{    
			var bytes:ByteArray = new ByteArray();
			socket.readBytes(bytes);
			
			bytes.endian = Endian.BIG_ENDIAN;
			
			while(bytes.bytesAvailable > 0){
			
				var path:String = readString(bytes);
				if(path != ""){
					if(path == "#bundle"){
						bytes.position+=8;
						var bundlelength:int = bytes.readInt();
						path = readString(bytes);
					}
				
					var datatypes:String = readString(bytes);
					
					var data:Array = new Array();
					data.push(path);
					for(var i:int=1;i<datatypes.length;i++){
						switch (datatypes.charAt(i)){
							case "s" : 
								data.push(readString(bytes));
								break;
							case "i" :
								data.push(bytes.readInt());
								break;
							case "f" :
								data.push(bytes.readFloat());
								break;
						}
					}
					this.dispatchEvent(new MessageEvent(MessageEvent.MESSAGE_RECEIVED,true,true,data));
				}
				
			}
        }
        
        private function readString(byteArray:ByteArray):String{
			var str:String = "";
			while(byteArray.readByte() != 0){
				byteArray.position-=1;
				str += byteArray.readUTFBytes(1);
			}
			byteArray.position += 3-(str.length % 4)
			return str;
		}
		
		public function connect(host:String,port:int):void{
			socket.connect(host, port);
		}
		
		public function close():void{
			socket.close();	
		}

	}
}