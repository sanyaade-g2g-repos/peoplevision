CYA cyaReceiver;
PFont helvetica;

void setup(){
	size(1280,960);
	background(0);
	cyaReceiver= new CYA(12000);
	
	hint(ENABLE_NATIVE_FONTS);
	helvetica = createFont("Helvetica", 11);
	textFont(helvetica);
	smooth();
	noStroke();
};

void draw(){
	cyaReceiver.update();
	background(0);
	//this should be an event or something more useful... in the future
	//following the testApp event example would be ideal
	//we'll need to separate out events into separate messages for that
	
	//println(cyaReceiver.people.size());
		
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
