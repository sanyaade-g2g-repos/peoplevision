#include "ofAI.h"   
#include "SN.h"
#include "DB.h"
#include "EN.h"

#include <string.h>
  
//---------------------------------------------------------------
SN::SN()        // Sensorium class constructor
{
}               // Sensorium class destructor

//---------------------------------------------------------------
SN::~SN()
{
}

//---------------------------------------------------------------
void SN::Sen()  // Human input module with AI attention
{
   RequestMsg();   // Get Human Input
   ClrActTags();   // Clear Active Tags
   Audition();     // Process Human Input
} 

//---------------------------------------------------------------
void SN::RequestMsg()   // Get Human Input
{
   cout << "Enter a brief positive or negative unpunctuated sentence." << endl;
   cout << "The AI listens for an input, and then generates a thought.\n" << endl;
   cout << "Enter an * to exit the AI.\n" << endl;
   cout << "Human: "; 
   ClrBuf();
   cin.getline(Buf,SN_BUFLEN,'\n');
}
   
//---------------------------------------------------------------
void SN::ClrActTags()
{
   AudStr * AudPtr;
   int t = DB::GetTime();
   
}

//---------------------------------------------------------------
void SN::Audition()  // AI attention
{
   int idx = 0;
   int onset; // Beginning of word
   char * tokenptr;  
   if(Buf[0] == '*')    // If first character is * then exit program
      {
      cout << "\nUser Halt.\n" << endl;
      exit(0);
      }
   ConvUpper();         // Convert all characters in message to upper case   
   idx = ClrSpc(idx);   // Skip any leading spaces in messge
   
   // Process words and characters by propagating effect of old
   // words into Psi data base and adding new words and propagating
   // their presence into the Psi data base. 
   
   tokenptr = strtok(Buf," ");
   tokenptr = tokenptr + idx; // Adjust pointer to skip leading spaces
   while(tokenptr != NULL)    // Extract words
   		 {
   		 cout << tokenptr << endl;
         if(AudRecog(tokenptr)) // Test for word in data base list
   				{
   				OldConcept();  // Word is in data base so update concept
   				} 
   		    else
   				{   
   				onset = AddWrdDb(tokenptr); // Add word to data base and return start time
   				AddWrdDbList(tokenptr); // Add word to data base list
   				NewConcept(onset);           // Update concept of new word
   				}
          tokenptr = strtok(NULL, " "); 
          }
}

//---------------------------------------------------------------
// Find the index to the first character of the 
// character string str embedded in the character
// string s.
int SN::Find (const char * str, char * s) const
{
	char * s2 = strstr(s,str);
	if (s2) 
	   return (s2 - s);
	else
	   return(-1);
}

//---------------------------------------------------------------
void SN::ClrBuf()    // Clear the input buffer
{
    int i = 0;
    for(i=0;i<SN_BUFLEN;i++)
        Buf[i] = NULL;
}

//---------------------------------------------------------------
void SN::ConvUpper()    // Convert input to upper case
{
   for(int i =0;i<SN_BUFLEN;i++)
      Buf[i] = (char) toupper(Buf[i]);
}

//---------------------------------------------------------------
int SN::ClrSpc(int i)  // Find index for first non space character
{
   while(Buf[i] == ' ')
       {
       i++;
       }            
   return(i);
}

//---------------------------------------------------------------
BOOL SN::AudRecog(char * tokenptr)  // Test for word in data data base list
{ 
   int i,len;
   char * Wrd;
   Wrd = DB::GetWrdPtr();
   i = SN::Find(tokenptr,Wrd);
   if(i<0) 
       return(false);
   if(*(Wrd + i - 1) != ' ')     
       return(false);
   len = strlen(tokenptr);
   if(*(Wrd + i + len) != ' ')
       return(false);
   return(true);
}

//---------------------------------------------------------------
void SN::OldConcept()   // Update arrays (fibers) for existing word (concept)
{
   SN::Parser();
   SN::Activate();
}

//---------------------------------------------------------------
void SN::NewConcept(int onset)   // Update arrays (fibers) for new  word (concept)
{
   int t;
   t = DB::GetTime();
   EN::SetPsiData(t,1,31,0,0,5,0,1);      
   EN::SetEnData(t,1,31,1,5,1,onset);    
}

//---------------------------------------------------------------
void SN::Parser()
{
}

//---------------------------------------------------------------
void SN::Activate()
{
}              
  
//---------------------------------------------------------------
int SN::AddWrdDb(char * tokenptr)  // Add a word to the Aud array (fiber).
{
	int t,i,wrdlen,onset;
    wrdlen = strlen(tokenptr);  		
    t = DB::GetTime();
   	t = t + 2;  // Leave blank in data base and set time for new word start 
   	onset = t;  // Save the new word start time
   	EN::SetAudData(t,*(tokenptr),0,35,1,0,0);  // Set first letter
    if(wrdlen >= 1)
   		{
   		for(i=1;i<wrdlen;i++)
   			{
   		    EN::SetAudData(t,*(tokenptr+i),0,35,0,1,0);
   		    t++;
   		    }
   		DB::SetTime(t);
   		}
   	return(onset);
}
			
//---------------------------------------------------------------
void SN::AddWrdDbList(char * tokenptr)  // Add a word to the word search list
{
    char * WrdList;
    WrdList = DB::GetWrdPtr();
    strcat(WrdList,tokenptr);
    strcat(WrdList," ");
}