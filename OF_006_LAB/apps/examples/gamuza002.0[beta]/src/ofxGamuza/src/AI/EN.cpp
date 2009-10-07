// Enboot - Initialize the memory threads
// with English words  

// Definitions: 

// Pho - Phoneme 
// Act - Flag for previous character hit in search
// Pov - Point of View (# Internal, * External)
// Beg - Beginning of word flag
// Ctu - Continuation of word flag
// Psi - Assoctiaon tag to En Array
// Jux - Juxtaposed word tag
// Pre - Previous concept asssociated with another concept
// Pos - Part of Speech  1=adj 2=adv 3=conj 4=interj 5=noun 6=prep 7=pron 8=verb
// Seq - Subsequent concept associated with another concept
// Enx - Transfer to English
// Nen - English lexical concept number
// Fex - Fiber exit flag
// Fin - Fiber in flag
// Aud - Auditory recall tag 

// Psi Concept Numbers 

// Articles 

// 1 = a,an		2 = all		3 = any		4 = false
// 5 = no		6 = one		7 = the		8 = true

// Adverbs

// 9 = also		10 = else 	11 = how 	12 = not
// 13 = then	14 = when	15 = where 	16 = why

// Conjunctions

// 17 = and		18 = because	19 = either 	20 = if
// 21 = or		22 = that 		23 = when		24 = whether

// Interjections

// 25 = goodbye	26 = hello		27 = no			28 = no
// 29 = ouch	30 = please		31 = thanks		32 = yes

// Nouns

// 33 = Andru	34 = bugs		35 = cats		36 = fish
// 37 = people	38 = persons	39 = robots 	40 = things	

//  Prepositions

// 41 = at		42 = for		43 = from		44 = in
// 45 = of		46 = on			47 = to			48 = with

// Pronouns

// 49 = he,she	50 = nothing	51 = nothing	52 = they
// 53 = we		54 = what		55 = who		56 = you

// Verbs

// 57 = am,is,are	58 = be		59 = do			60 = hear
// 61 = know 	62 = see		63 = think		64 = understand

#include "ofAI.h"   
#include "EN.h"
#include "DB.h" 

#include <string.h>  
   
//---------------------------------------------------------------
EN::EN(){
   
}
   
//---------------------------------------------------------------
EN::~EN(){
   
}
                                                          

    
//---------------------------------------------------------------
// Set data into Aud array (fiber).
void EN::SetAudData( int t, int pho, int act, int pov, int beg, int ctu, int psi)
{
   AudStr AudVal;
   AudStr * AudValPtr = &AudVal;
   AudVal.pho = pho;
   AudVal.act = act;
   AudVal.pov = pov;
   AudVal.beg = beg;
   AudVal.ctu = ctu;
   AudVal.psi = psi;
   DB::SetEle(AudValPtr,t);
}

//---------------------------------------------------------------
// Set data into Psi array (fiber).
void EN::SetPsiData(int t, int psi, int act, int jux, int pre, int pos, int seq, int enx)
{
   PsiStr PsiVal;
   PsiStr * PsiValPtr = & PsiVal;
   PsiVal.psi = psi;
   PsiVal.act = act;
   PsiVal.jux = jux;
   PsiVal.pre = pre;
   PsiVal.pos = pos;
   PsiVal.seq = seq;
   PsiVal.enx = enx;
   DB::SetEle(PsiValPtr,t);
}

//---------------------------------------------------------------
// Set data into En array (fiber).
void EN::SetEnData(int t, int nen, int act, int fex, int pos, int fin, int aud)
{
   EnStr EnVal;
   EnStr * EnValPtr = & EnVal; 
   EnVal.nen = nen;
   EnVal.act = act;
   EnVal.fex = fex;
   EnVal.pos = pos;
   EnVal.fin = fin;
   EnVal.aud = aud;
   DB::SetEle(EnValPtr,t);
}

//---------------------------------------------------------------
// Initialize arrays (fibers)
void EN::Enb()
{ 
   SetAudData(1,'Y',0,35,1,0,0);
   SetAudData(2,'E',0,35,0,1,0);
   SetAudData(3,'S',0,35,0,1,0);
   SetPsiData(3,32,0,0,0,4,0,32);
   SetEnData(3,32,0,32,4,32,1);

   SetAudData(5,'I',0,35,1,0,0);
   SetAudData(6,'F',0,35,0,1,20);
   SetPsiData(6,20,0,0,0,3,0,20);
   SetEnData(6,20,0,20,3,20,5);

   SetAudData(8,'T',0,35,1,0,0);
   SetAudData(9,'H',0,35,0,1,0);
   SetAudData(10,'E',0,35,0,1,7);
   SetPsiData(10,7,0,0,0,1,0,7);
   SetEnData(10,7,0,7,1,7,8);

   SetAudData(12,'T',0,35,1,0,0);
   SetAudData(13,'R',0,35,0,1,0);
   SetAudData(14,'U',0,35,0,1,0);
   SetAudData(15,'T',0,35,0,1,0);
   SetAudData(16,'H',0,35,0,1,68);
   SetPsiData(16,68,0,0,0,5,66,68);
   SetEnData(16,68,0,68,5,68,12);

   SetAudData(18,'I',0,35,1,0,0);
   SetAudData(19,'S',0,35,0,1,66);
   SetPsiData(19,66,0,0,68,8,1,66);
   SetEnData(19,66,0,66,8,66,18);

   SetAudData(21,'T',0,35,1,0,0);
   SetAudData(22,'H',0,35,0,1,0);
   SetAudData(23,'A',0,35,0,1,0);
   SetAudData(24,'T',0,35,0,1,22);
   SetPsiData(24,22,0,0,0,3,0,22);
   SetEnData(24,22,0,22,3,22,21);

   SetAudData(26,'A',0,35,1,0,0);
   SetAudData(27,'L',0,35,0,1,0);
   SetAudData(28,'L',0,35,0,1,2);
   SetPsiData(28,2,0,0,0,1,0,2);
   SetEnData(28,2,0,2,1,2,26);

   SetAudData(30,'R',0,35,1,0,0);
   SetAudData(31,'O',0,35,0,1,0);
   SetAudData(32,'B',0,35,0,1,0);
   SetAudData(33,'O',0,35,0,1,0);
   SetAudData(34,'T',0,35,0,1,0);
   SetAudData(35,'S',0,35,0,1,39);
   SetPsiData(35,39,0,0,0,5,0,39);
   SetEnData(35,39,0,39,5,39,30);

   SetAudData(37,'A',0,35,1,0,0);
   SetAudData(38,'R',0,35,0,1,0);
   SetAudData(39,'E',0,35,0,0,67);
   SetPsiData(39,67,0,0,39,8,38,67);
   SetEnData(39,67,0,67,8,67,37);

   SetAudData(41,'P',0,35,1,0,0);
   SetAudData(42,'E',0,35,0,1,0);
   SetAudData(43,'R',0,35,0,1,0);
   SetAudData(44,'S',0,35,0,1,0);
   SetAudData(45,'O',0,35,0,1,0);
   SetAudData(46,'N',0,35,0,1,0);
   SetAudData(47,'S',0,35,0,0,38);
   SetPsiData(47,38,0,0,0,5,0,39);
   SetEnData(47,38,0,38,5,38,41);
   DB::SetTime(47);
}

//---------------------------------------------------------------
// Initialize the word search list
void EN::Ena()
{
   char * WrdPtr;
   char string[] = " YES IF THE TRUTH IS THAT ROBOTS ";
   WrdPtr = DB::GetWrdPtr();
   strcpy(WrdPtr,string);         
}

