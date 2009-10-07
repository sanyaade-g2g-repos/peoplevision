#include "DNA.h"


//-----------------------------------------------
DNA::DNA(){
}
//-----------------------------------------------
// Constructor #1
DNA::DNA(int num){
	for(int i=0;i<num;i++){
	    dna.push_back(ofxVec3f(ofRandomf(),ofRandomf(),ofRandomf()));
		dna[i].normalize();
	}

}

//-----------------------------------------------
// Constructor #2
DNA::DNA(vector <ofxVec3f>  newDna){
    dna.clear();
    dna.assign(newDna.begin(),newDna.end());
}

//-----------------------------------------------
DNA::~DNA(){
}

//-----------------------------------------------
ofxVec3f DNA::getGene(int index){
    ofxVec3f gene;
    gene.set(0.0,0.0,0.0);
    if((unsigned int)index < dna.size()){
        return dna[index];
    }else{
        return gene;
    }
}

//-----------------------------------------------
DNA DNA::mate(DNA partner){
    vector <ofxVec3f> child;
    //*pick a midpoint*//
    int crossover = (int)(ofRandom(0,dna.size()));
    //*take "half" from one and "half" from the other*//
    for(unsigned int i=0;i<dna.size();i++){
        if(i<(unsigned int)crossover){
            child.push_back(getGene(i));
        }else{
            child.push_back(partner.getGene(i));
        }
    }

    DNA newdna(child);
    return newdna;
}

//-----------------------------------------------
void DNA::mutate(float m){
    for(unsigned int i=0;i<dna.size();i++){
        if(ofRandomuf() < m){
            dna[i].set(ofRandomf(),ofRandomf(),ofRandomf());
            dna[i].normalize();
        }
    }
}
