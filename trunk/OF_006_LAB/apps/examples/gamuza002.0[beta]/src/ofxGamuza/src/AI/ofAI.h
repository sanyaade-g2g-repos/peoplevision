#ifndef _OF_AI
#define _OF_AI

#ifndef ARY
#define ARY
#define ARYSIZ 100
#endif

#ifndef WRD
#define WRD
#define WRDSIZ 1000
#endif


#include "ofMain.h"

#include <iomanip>
#include <stdlib.h>
#include <ctype.h>
#include <vector>


//---------------------------------------------------------------
class ofAI{

	public:
	
		ofAI();
		~ofAI();
		
		void Alife();  // Artificial Intelligence Program
	
		
};
//---------------------------------------------------------------

class TH : public ofAI{   // Human input module with AI attention

public:

	TH();
	~TH();

    void Thn();

private:

    void Activate();
    void English();
    void NegSvo();   // Negative of Subject-Verb-Object
    void Svo();      // Subject Verb-Object
    
};   

class MO : public ofAI{   // Human input module with AI attention

public:

	MO();
	~MO();

    void Mot();


};

#endif
