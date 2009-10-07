// Data Base - Creates and manipulates AI
// memory arrays. Currently 1 dimensional
// fixed length. Should be variable length
// and a linked list to allow convenient
// insertion and deletion. Deletion 
// currently just clears the location.
// Should also be asociative memory.
// Hashing would help.

#include "ofAI.h"   
#include "DB.h"


int Time;              // Time index to words

PsiStr * PsiAryPtr;    // Pointer to start of Psi array (fiber).
EnStr  * EnAryPtr;     // Pointer to start of En array (fiber).
AudStr * AudAryPtr;    // Pointer to start of Aud array (fiber).
char   * WrdAryPtr;    // Pointer to start of Word list.

//---------------------------------------------------------------
DB::DB()
{
   // Use extensible Array class here
   // rather than fixed size array
   if((PsiAryPtr = new PsiStr[ARYSIZ]) == (PsiStr *)0)
       PrintMemErr();
   if((EnAryPtr  = new EnStr[ARYSIZ]) == (EnStr *)0)
       PrintMemErr();
   if((AudAryPtr = new AudStr[ARYSIZ]) == (AudStr *)0)
       PrintMemErr(); 
   ClrPsi();   // Clears Psi array (Tabulrasa equivalent)                     
   ClrEn();    // Clears En  array (Tabulrasa equivalent)                    
   ClrAud();   // Clears Aud array (Tabulrasa equivalent)                     
   if((WrdAryPtr = new char[WRDSIZ]) == (char *)0)
       PrintMemErr();
   ClrWrd(); // Clears Word List
}

//---------------------------------------------------------------
DB::~DB()
{
   delete [] PsiAryPtr;
   delete [] EnAryPtr;
   delete [] AudAryPtr;
   delete [] WrdAryPtr;
}

//---------------------------------------------------------------
PsiStr * DB::GetPsiAryPtr()   // Get Psi beginning pointer
   {
   return(PsiAryPtr);
   }

//---------------------------------------------------------------
   EnStr * DB::GetEnAryPtr()     // Get En beginning pointer
   {
   return(EnAryPtr);
   }

//---------------------------------------------------------------
   AudStr * DB::GetAudAryPtr()   // Get Aud beginning pointer
   {
   return(AudAryPtr);
   }

//---------------------------------------------------------------
   char * DB::GetWrdPtr()        // Get word list beginning pointer
   {
   return(WrdAryPtr);
   }
   
//---------------------------------------------------------------
   void DB::PrintMemErr()        // Memory error message
   {
   cout << "\nArray creation error\n";
   }
   
   // Set functions
   // Templates to be used for data base functions

//---------------------------------------------------------------
    bool DB::SetEle(PsiStr * StrPtr, const int EleIdx)    // Set concept array element
   	{                                                     // This is the Instantiate module
   	(PsiAryPtr+EleIdx)->psi = StrPtr->psi;
   	(PsiAryPtr+EleIdx)->act = StrPtr->act;
   	(PsiAryPtr+EleIdx)->jux = StrPtr->jux;
  	(PsiAryPtr+EleIdx)->pre = StrPtr->pre;
   	(PsiAryPtr+EleIdx)->pos = StrPtr->pos;
   	(PsiAryPtr+EleIdx)->seq = StrPtr->seq;
   	(PsiAryPtr+EleIdx)->enx = StrPtr->enx; 
    return(true);
   	}

//---------------------------------------------------------------
   bool DB::SetEle(EnStr  * StrPtr, const int EleIdx)   // set english lexicon element, element index
   	{                                                   // This is the EnVocab module
   	(EnAryPtr+EleIdx)->nen = StrPtr->nen;              
   	(EnAryPtr+EleIdx)->act = StrPtr->act;
   	(EnAryPtr+EleIdx)->fex = StrPtr->fex;
  	(EnAryPtr+EleIdx)->pos = StrPtr->pos;
   	(EnAryPtr+EleIdx)->fin = StrPtr->fin;
   	(EnAryPtr+EleIdx)->aud = StrPtr->aud;
    return(true);
   	}

//---------------------------------------------------------------
   bool DB::SetEle(AudStr * StrPtr, const int EleIdx)   // set auditory memory element, element index
   	{
   	(AudAryPtr+EleIdx)->pho = StrPtr->pho;
   	(AudAryPtr+EleIdx)->act = StrPtr->act;
   	(AudAryPtr+EleIdx)->pov = StrPtr->pov;
  	(AudAryPtr+EleIdx)->beg = StrPtr->beg;
   	(AudAryPtr+EleIdx)->ctu = StrPtr->ctu;
   	(AudAryPtr+EleIdx)->psi = StrPtr->psi;
    return(true);
   	}

   // get functions 
   
//---------------------------------------------------------------
   PsiStr * DB::GetPsiEle( const int EleIdx)   // return concept array element 
   	{
   	return(GetPsiAryPtr() + EleIdx);
   	}

//---------------------------------------------------------------
   EnStr * DB::GetEnEle( const int EleIdx)   // return english lexicon element
   	{
   	return(GetEnAryPtr() + EleIdx);
   	} 

//---------------------------------------------------------------
   AudStr * DB::GetAudEle( const int EleIdx)   // return auditory memory element
   	{
   	return(GetAudAryPtr() + EleIdx);
   	}


// delete functions
//---------------------------------------------------------------
// delete concept array element, element index
bool DB::DelPsi( const int EleIdx){
}

//---------------------------------------------------------------
// delete english lexicon element, element index
bool DB::DelEn(  const int EleIdx){
}
 
//---------------------------------------------------------------
// delete auditory memory element, element index
bool DB::DelAud( const int EleIdx){
}
   
// print functions 
//---------------------------------------------------------------
bool DB::PrintPsiElements(){
}

//---------------------------------------------------------------
bool DB::PrintEnElements(){
}

//---------------------------------------------------------------
bool DB::PrintAudElements(){
}
   
//---------------------------------------------------------------
   bool DB::SetTime(int t)       // Set Time
   {
   if((t < 65000) && (t >= 0))
       {
       Time = t;
       return(true);
       }
   else
   	   return(false);
   }

//---------------------------------------------------------------
   int DB::GetTime()             // Get Time
   {
   return(Time);
   }

//---------------------------------------------------------------
   void DB::StartupMsg()
   {
   cout << "\nThis software is not prepared to perform any function.\n" << endl;
   }


//---------------------------------------------------------------
   void DB::ClrPsi()    // Clears Psi array (Tabulrasa equivalent)  
   {
   int i;
   PsiStr PsiZer;
   PsiStr * PsiZerPtr = & PsiZer; 
   PsiZer.psi = 0;
   PsiZer.act = 0;
   PsiZer.jux = 0;
   PsiZer.pre = 0;
   PsiZer.pos = 0;
   PsiZer.seq = 0;
   PsiZer.enx = 0;
   for( i=0;i<ARYSIZ;i++)
   		DB::SetEle(PsiZerPtr,i);		    ; 
   }

//---------------------------------------------------------------
   void DB::ClrEn()     // Clears En array (Tabulrasa equivalent)  
   {
   int i;
   EnStr EnZer;
   EnStr * EnZerPtr = & EnZer; 
   EnZer.nen = 0;
   EnZer.act = 0;
   EnZer.fex = 0;
   EnZer.pos = 0;
   EnZer.fin = 0;
   EnZer.aud = 0; 
   for( i=0;i<ARYSIZ;i++)
   		DB::SetEle(EnZerPtr,i);		    
   }

//---------------------------------------------------------------
   void DB::ClrAud()   // Clears Aud array (Tabulrasa equivalent)  
   {
   int i;
   AudStr AudZer;
   AudStr * AudZerPtr = & AudZer; 
   AudZer.pho = ' ';
   AudZer.act = 0;
   AudZer.pov = 0;
   AudZer.beg = 0;
   AudZer.ctu = 0;
   AudZer.psi = 0; 
   for( i=0;i<ARYSIZ;i++)
   		DB::SetEle(AudZerPtr,i);		    ; 
   } 

//---------------------------------------------------------------
   void DB::ClrWrd()     // Clears word list  
   {
   int i;
   for(i=0;i<WRDSIZ;i++)
   		*(WrdAryPtr + i) = ' ';
   }
