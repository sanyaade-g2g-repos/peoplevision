#ifndef DB_H  
#define DB_H 


struct AudStr  // Auditory Memory Array
{
	int pho;  // Phoneme
	int act;  // Activiation Level
	int pov;  // Point of View
	int beg;  // Beginning
	int ctu;  // Continuation
	int psi;  // Tag number to a concept
};

struct EnStr    // English Lexicon Array
{
	int nen;  // Mindcore concept number
	int act;  // Activiation Level
	int fex;  // Mindcor Exit tag
	int pos;  // Part of Speech
	int fin;  // Mindcore In tag
	int aud;  // Auditory Tag
};

struct PsiStr   // Mindcore Concept Array
{
	int psi;  // Mindcore concept number
	int act;  // Activiation Level
	int jux;  // Juxtaposed modifier
	int pre;  // Previous associated
	int pos;  // Part of Speech
	int seq;  // Subsequent tag
	int enx;  // Transfer to English
}; 


//---------------------------------------------------------------
class DB : public ofAI {   // Human input module with AI attention

	public:

		// set functions
   
		static   bool SetEle( PsiStr * StrPtr, const int EleIdx);  // set concept array element, element index
		static   bool SetEle( EnStr * StrPtr,  const int EleIdx);  // set english lexicon element, element index
		static   bool SetEle( AudStr * StrPtr, const int EleIdx);  // set auditory memory element, element index

		// get functions
   
		static   PsiStr * GetPsiEle( const int EleIdx);  // return concept array element
		static   EnStr * GetEnEle( const int EleIdx);    // return english lexicon element
		static   AudStr * GetAudEle( const int EleIdx);   // return auditory memory element

		// delete functions
   
		static   bool DelPsi( const int EleIdx); // delete concept array element, element index
		static   bool DelEn(  const int EleIdx); // delete english lexicon element, element index
		static   bool DelAud( const int EleIdx); // delete auditory memory element, element index
   
		static   PsiStr * GetPsiAryPtr();
		static   EnStr * GetEnAryPtr();
		static   AudStr * GetAudAryPtr();
		static   char * GetWrdPtr(); 
  
		static   void PrintMemErr();
		static   bool SetTime(int t);
		static   int GetTime(); 
		static   void StartupMsg();

		// print functions
   
		bool PrintPsiElements();  // output 
		bool PrintEnElements();   // output 
		bool PrintAudElements();  // output 
                                       
		// Clear data base functions                                    
                                 
		DB();
		~DB(); 
 

	private: 

		void Clr();
		void ClrPsi();            // Clear Psi array
		void ClrEn ();            // Clear En array
		void ClrAud();            // Clear Aud array
		void ClrWrd();            // Clear Word Array
		AudStr * AudStrPtr;
		EnStr  * EnStrPtr;
		PsiStr * PsiStrPtr;
  
};

#endif