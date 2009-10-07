#ifndef SE_H  
#define SE_H 


//---------------------------------------------------------------
class SE : public ofAI {   // Security = Internal rumination tasks

public:

	SE();
	~SE();

    void Sec();

private:

    void Rejuvinate();
    void PsiDecay();
    void Ego();
    
};

#endif