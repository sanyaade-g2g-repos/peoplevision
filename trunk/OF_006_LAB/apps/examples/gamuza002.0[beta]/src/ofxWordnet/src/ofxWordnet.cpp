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
 
#include "ofxWordnet.h"

ofxWordnet::ofxWordnet(){
	
	isDBLoaded = -1;
	
}

ofxWordnet::~ofxWordnet(){
	
}

//----------------------------------------------------------
void ofxWordnet::init(){
	
	isDBLoaded = wninit();
	
	if(isDBLoaded == -1){
		consoleMessage();
		cout << "Error loading Wordnet database files" << endl;
	}else{
		consoleMessage();
		cout << "Wordnet database files and morphology exception lists loaded correctly" << endl;
	}
	
}

//----------------------------------------------------------
void ofxWordnet::reInit(){
	
	isDBLoaded = re_wninit();
	
	if(isDBLoaded == -1){
		consoleMessage();
		cout << "Error loading Wordnet database files" << endl;
	}else{
		consoleMessage();
		cout << "Wordnet database files and morphology exception lists loaded correctly" << endl;
	}
	
}

//----------------------------------------------------------
int ofxWordnet::exists(char * w){
	
	unsigned int def;
	char * morphW;
	
	if(in_wn(w, ALL_POS) == 0){
		morphW = morphstr(w, NOUN);
		if(morphW == NULL){
			printf("%s don't exist\n",w);
			return 0;
		}
		def = in_wn(morphW, ALL_POS);
	}else{
		printf("%s exist\n",w);
		return 1;
	}
	
	if(def == 0){
		printf("%s don't exist\n",w);
		return 0;
		
	}else{
		printf("%s exist\n",w);
		return 1;
	}

}

//----------------------------------------------------------
int ofxWordnet::isNoun(int val){
	int i;
	int cod = 16;

	for (i=0; i<=3; i++) {
		if (val == 0)
			return 0;
		if (val == 2)
			return 1;
		if (cod <= val) {
			val = val - cod;			
		}
		cod = cod / 2;
	}
	return 0;
}

//----------------------------------------------------------
/***********************************************************
getDefinitions
Devuelve las definiciones que puede tener una palabra
Argumentos:
	- w: Palabra de la que se quieren obtener las definiciones.
	- numSense: Indica si se desean todas las significados o solo el + utilizado.
		0 -> Todas las definiciones
		1 -> La primera (la + usada)
************************************************************/
Lexicon * ofxWordnet::getDefinitions(char * w, int numSense){

	char *outbuf = NULL;
	unsigned int def;
	char *morphword;
	char wordsearch[200];
	char *searchword;
	int i;
	static Lexicon meanings;
	int numMeanings;
	char sigSense[50];
	char significado[5000];
	int j;
	char *p;

	strcpy(wordsearch, w);
	searchword = &wordsearch[0];
    strtolower(strsubst(searchword, ' ', '_'));
	strtolower(strsubst(searchword, '-', '_'));
	// Obtiene un numero para determinar la categorÌa sintactica
	// 2  -> Sustantivo
	// 4  -> Verbo
	// 8  -> Adjetivo
	// 16 -> Adverbio
	// Y cualquier combinacion de ellas (6 -> Sustantivo + Verbo)
	def = in_wn(searchword, NOUN);
	
	printf("categoria: %i\n",def);

	if (def == 0) {
		// Obtiene el morfema (houses -> house)
		if ((morphword = morphstr(searchword, NOUN)) == NULL) {
			outbuf = findtheinfo(searchword, NOUN, 5, numSense);
			if (strlen(outbuf) < 1) {
				printf("\nPalabra desconocida.");
				meanings.numLex = 0;
				return &meanings;
			} else {
				//printf("\n%s",outbuf);				
			}
		} else {
			searchword = morphword;
			outbuf = findtheinfo(searchword, NOUN, 5, numSense);
			//printf("\n%s",outbuf);			
		}
	} else {
		outbuf = findtheinfo(searchword, NOUN, 5, numSense);
		//printf("\n%s",outbuf);				
	}

	if (outbuf == NULL) {
		meanings.numLex = 0;
		return &meanings;
	}
	outbuf++;
	if (numSense == 0) {		
		sscanf(outbuf,"%d", &numMeanings);
	} else {
		numMeanings = 1;
	}

	meanings.numLex = numMeanings;
	for (i = 0; i<numMeanings; i++) {
		if (numSense == 0) {
			sprintf(sigSense, "%s %d", "Sense", i+1);
		} else {
			sprintf(sigSense, "%s %d", "Sense", numSense);
		}
		outbuf = strstr(outbuf, sigSense);
		outbuf = outbuf + strlen(sigSense) + 1;
		j = 0;
		while (outbuf[j] != '-' && outbuf[j+1] != '-') {
			significado[j] = outbuf[j];
			j++;
		}
		significado[j] = '\0';
		p = (char *)malloc(strlen(significado) * sizeof(char));
		strcpy(p, significado);
		strcpy(meanings.Data[i], p);

		outbuf = strstr(outbuf, "-- (");
		outbuf = outbuf + 4;
		j = 0;
		while (outbuf[j] != 10) {
			significado[j] = outbuf[j];
			j++;
		}
		significado[j-1] = '\0';
		p = (char *)malloc(strlen(significado) * sizeof(char));
		strcpy(p, significado);
		strcpy(meanings.Descripcion[i], p);
	}

    return &meanings;
}

//----------------------------------------------------------
Lexicon * ofxWordnet::getSynonyms(char * w, int numSense){

	char *outbuf;
	unsigned int def;
	int i;
	static Lexicon sinonimos;
	int numSyn;
	char sigSense[50];
	char sinonimo[5000];
	int j;
	char *p;

    // Obtiene un n˙mero para determinar la categorÌa sint·ctica
	// 2  -> Sustantivo
	// 4  -> Verbo
	// 8  -> Adjetivo
	// 16 -> Adverbio
	// Y cualquier combinaciÛn de ellas (6 -> Sustantivo + Verbo)
	def = in_wn(w, ALL_POS);
	
	if (def == 0){
		return NULL;
	}

	if (isNoun(def)) {
		outbuf = findtheinfo(w, NOUN, 2, numSense);
		//printf("\n%s",outbuf);
	} else {
		return NULL;
	}

	outbuf++;
	if (numSense == 0) {
		sscanf(outbuf,"%d", &numSyn);
	} else {
		numSyn = 1;
	}
	sinonimos.numLex = numSyn;
	for (i = 0; i<numSyn; i++) {
		sprintf(sigSense, "%s %d", "Sense", i+1);
		outbuf = strstr(outbuf, sigSense);
		outbuf = strchr(outbuf, 10) + 1;
		j = 0;
		while (outbuf[j] != '-' || outbuf[j + 1] != '-') {
			sinonimo[j] = outbuf[j];
			j++;
		}
		sinonimo[j-1] = '\0';
		p = (char *)malloc(strlen(sinonimo) * sizeof(char));
		strcpy(p, sinonimo);
		strcpy(sinonimos.Data[i], p);
		
		outbuf = strstr(outbuf, "=>");
		outbuf = outbuf + 3;
		j = 0;
		while (outbuf[j] != 45) {
			sinonimo[j] = outbuf[j];
			j++;
		}
		sinonimo[j-1] = '\0';
		p = (char *)malloc(strlen(sinonimo) * sizeof(char));
		strcpy(p, sinonimo);
		
		strcpy(sinonimos.Descripcion[i], p);
	}
	
    return &sinonimos;
}

//----------------------------------------------------------
char *	ofxWordnet::logMessage(char * m){
	return m;
}

//----------------------------------------------------------
void ofxWordnet::consoleMessage(){
	cout << "||||||||||||||||||||||||||||||||||" << endl;
	cout << "|||| OFXWORDNET ADDON MESSAGE ||||" << endl;
	cout << "||||||||||||||||||||||||||||||||||" << endl;
	cout << endl;
}

//----------------------------------------------------------