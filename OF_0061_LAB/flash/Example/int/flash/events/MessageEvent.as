package flash.events {
	
	import flash.display.DisplayObject;	
	
	public class MessageEvent extends Event
	{
		public var data:Array;
		
		public static var MESSAGE_RECEIVED:String = "flash.events.MessageEvent.MessageReceived";
	
		public function MessageEvent(type:String, bubbles:Boolean = false, cancelable:Boolean = false, data:Array=null)
		{
			this.data = data;			
			super(type, bubbles, cancelable);			
		}
	}
}