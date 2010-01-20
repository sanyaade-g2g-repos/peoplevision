package com.lab
{
	import flash.events.Event;
	import com.lab.CYAPerson;
	
	public class CYAEvent extends Event
	{
		public static var PERSON_ENTERED:String = "person_entered";
		public static var PERSON_UPDATED:String = "person_updated";
		public static var PERSON_MOVED:String = "person_moved";
		public static var PERSON_LEFT:String = "person_left";
		
		public var person:CYAPerson;
		
		public function CYAEvent( type:String, _person:CYAPerson, bubbles:Boolean = false, cancelable:Boolean = false )
		{
			person = _person;
			super (type, bubbles, cancelable);			
		}

		public override function clone():Event
		{
			return new CYAEvent ( type, person, bubbles, cancelable );
		}
	}
}