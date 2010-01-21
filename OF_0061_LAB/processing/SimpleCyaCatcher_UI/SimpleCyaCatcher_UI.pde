import cya.*;

CYA cyaReceiver;
PFont timesBold18;

//vars for status bar
PImage statusBar;
PImage numberOfPeople;
PImage personEnteredNotice;
PImage personUpdatedNotice;
PImage personLeftNotice;

void setup(){
	size(1024,768);
	cyaReceiver= new CYA(this, 12000);
	
	hint(ENABLE_NATIVE_FONTS);
	timesBold18 = loadFont("TimesNewRomanPS-BoldItalicMT-18.vlw");
	textFont(timesBold18, 18);
	smooth();
	noStroke();
	loadImages();
	drawBackground();
};

void draw(){
	cyaReceiver.update();
	drawBackground();
	
	//this should be an event or something more useful... in the future
	//following the testApp event example would be ideal
	//we'll need to separate out events into separate messages for that
	
	for (int i=0; i<cyaReceiver.people.size(); i++)
	{
		CYAPerson person = (CYAPerson) cyaReceiver.people.get(i);
		
		fill(120,120,0);
		rect(person.boundingRect.x*width, person.boundingRect.y*height, person.boundingRect.width*width, person.boundingRect.height*height);
		
		fill(255,255,255);
		ellipse(person.centroid.x*width, person.centroid.y*height, 10, 10);
		text("id: "+person.id+" age: "+person.age, person.boundingRect.x*width, (person.boundingRect.y*height + person.boundingRect.height*height) + 2);	
	};
};

void personEntered( CYAPerson p ){
	println("ENTERED");
}

void personUpdated( CYAPerson p ){
	println("UPDATED");
}

void personLeft( CYAPerson p ){
	println("LEFT");
}

void loadImages(){
	statusBar = loadImage("bottomBar.png");
}

void drawBackground(){
	background(148,129,85);	
	//image (statusBar, 0, 700);
}


