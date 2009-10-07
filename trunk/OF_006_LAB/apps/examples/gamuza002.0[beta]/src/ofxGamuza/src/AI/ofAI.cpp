#include "ofAI.h"

#include "EN.h"
#include "SE.h"
#include "DB.h"
#include "SN.h"



//---------------------------------------------------------------
ofAI::ofAI(){
	

}


//---------------------------------------------------------------
ofAI::~ofAI(){
	

}

//---------------------------------------------------------------
void ofAI::Alife(){
	DB DatBase;   		 // Create Data Base
    EN EngBoot;      	 // Create Initialize Data Base 
    SE Security;   	     // Create Internal Rumination
    SN Sensorium;  		 // Create Human Input Facility
    TH Think;        	 // Create Internal Associative Facility
    MO Motorium;   	     // Create Output Facility
    EngBoot.Ena();        // Initialize Word List
    EngBoot.Enb(); 		 // Initialize Word Data Base
    DatBase.StartupMsg(); // Display Startup Message
    while(true){
		Security.Sec();     // AI Internal Rumination  
		Sensorium.Sen();    // AI Initial procesing of Input
		Think.Thn();        // AI Syntax and vocabulary of natural language
		Motorium.Mot();     // AI Output
	}
}

//---------------------------------------------------------------
//---------------------------------------------------------------
//  Classes below are AI derived classes
   
//---------------------------------------------------------------
MO::MO(){				// Motorium class constructor
   
}              

//---------------------------------------------------------------
MO::~MO(){				// Motorium class destructor
   
}

//---------------------------------------------------------------
void MO::Mot(){			// Motorium function
   
} 

//---------------------------------------------------------------
TH::TH(){				// Think class constructor
   
}					

//---------------------------------------------------------------
TH::~TH(){				// Think class destructor
   
}

//---------------------------------------------------------------
void TH::Thn(){			// Syntax and Vocabulary of Natural Language
   
}
//---------------------------------------------------------------
//---------------------------------------------------------------
