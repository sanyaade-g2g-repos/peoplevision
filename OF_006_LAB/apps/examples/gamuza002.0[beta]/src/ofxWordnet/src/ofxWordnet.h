/*
 *  ofxWordnet.h
 *  openFrameworks Wordnet Addon.
 *
 *  Created by Emanuele Mazza | n3m3da@d3cod3.org
 *	http://www.d3cod3.org
 *	http://code.d3cod3.org
 *
 *	Provides simple access to the Wordnet [http://wordnet.princeton.edu/] ontology:
 *
 *	Synonyms, Antonyms, Hypernyms, & Hyponyms
 *	Holonyms, Meronyms, Coordinates, & Similars
 *	Nominalizations, Verb-Groups, & Derived-terms
 *	Glosses, See-Alsos, Examples, & Descriptions
 *	Support for pattern-matching, soundex, anagrams, etc.
 *
 */
 
#ifndef _OFXWORDNET_H
#define _OFXWORDNET_H


#include <iostream>
using namespace std;

#include <vector>

#include "wn.h" // WORDNET Library

struct Lexicon
{
	int numLex;
	char Data[20][500];
	char Descripcion[20][500];
};

class ofxWordnet {

	public:
	
		ofxWordnet();
		virtual ~ofxWordnet();
		
		//----------------------------------------------------------			
		void init();
		void reInit();
		
		//----------------------------------------------------------
		int					exists(char * w);
		int					isNoun(int val); // sustantivo
		
		Lexicon *			getDefinitions(char * w, int numSense); // definiciones
		Lexicon *			getSynonyms(char * w, int numSense); // similares
		
		//----------------------------------------------------------
		char *	logMessage(char * m);
		void	consoleMessage();

	protected:
		int	isDBLoaded;
	
};

#endif
