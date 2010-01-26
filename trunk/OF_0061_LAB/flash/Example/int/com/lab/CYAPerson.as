package com.lab
{
	import flash.display.MovieClip;
	import flash.display.Sprite;
	
	public class CYAPerson extends Sprite
	{
		public var id:uint; 
		public var age:uint; 
		public var stageWidth:uint;
		public var stageHeight:uint;
	 	public var centroid:Object;  
		public var velocity:Object;  
		public var opticalFlow:Object;
		public var realWidth:Number;
		public var realHeight:Number; 
		public var dead:Boolean;
		public var contours:Array;
		
		public function CYAPerson( _stageWidth:uint, _stageHeight:uint, _id:uint = 0, _age:uint = 0, _centroid:Object = null, _velocity:Object = null, _x:Number = 0, _y:Number = 0, _width:Number = 0, _height:Number = 0, _opticalFlow:Object = null, _dead:Boolean = false )
		{
			super();
			
			stageWidth = _stageWidth;
			stageHeight = _stageHeight;
			
			id = _id;
			age = _age;
			
			if (_centroid == null){
				centroid = new Object();
				centroid.x = centroid.y = 0;
			} else {
				centroid = _centroid;
			}
			
			if (_velocity == null){
				velocity = new Object();
				velocity.x = velocity.y = 0;
			} else {
				velocity = _velocity;
			}
			
			if (_opticalFlow == null)
			{
				opticalFlow = new Object();
				opticalFlow.x = opticalFlow.y = 0;
			} else {
				opticalFlow = _opticalFlow;
			}
			
			x = _x;
			y = _y;
			realWidth = _width;
			realHeight = _height;
			dead = _dead;
			contours = new Array();
		}
				
		public function update ( _person:CYAPerson )
		{
			age 		= _person.age;
			centroid 	= _person.centroid;
			velocity 	= _person.velocity;
			opticalFlow = _person.opticalFlow;
			x 			= _person.x;
			y 			= _person.y;
			realWidth	= _person.realWidth;
			realHeight	= _person.realHeight;
			contours	= _person.contours;	
			dispatchEvent ( new CYAEvent(CYAEvent.PERSON_UPDATED, this));
		}
	}
}