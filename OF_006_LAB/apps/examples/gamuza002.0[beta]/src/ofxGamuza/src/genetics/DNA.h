#ifndef 	_DNA_H
#define 	_DNA_H

#include "ofMain.h"
#include "ofxVectorMath.h"			// vectorial math structures library
#include <vector>

// DNA is an array of vectors

class DNA{

	public:
			DNA();
			DNA(int num); // Constructor(makes a DNA of random vectors)
			DNA(vector <ofxVec3f> newDna); // Constructor #2, creates the instance based on an existing array
			virtual ~DNA();

			ofxVec3f getGene(int index); // return one element from the DNA array

			//**CROSSOVER**//
			//creates new DNA sequence from two(this & a partner)
			DNA mate(DNA partner);

			//based on a mutation probability, picks a new random Vector
			void mutate(float m);



	private:

            vector <ofxVec3f>    dna; // the genetic sequence

};

#endif
