///////////////////////////////////////////////////////////
//  LineTrace.cpp
//  Implementation of the Class LineTrace
//  Created on:      10-9-2010 19:43:08
//  Original author: skawasak
///////////////////////////////////////////////////////////

#include "LineTrace.h"
//extern INIT_FORWARD;
//extern FORWARD;
//extern LEFT (-1)
//extern RIGHT (1)
//extern TRACE_EDGE (1)
#define INIT_FORWARD (50.0)
#define FORWARD (100.0)
#define LEFT (-1)
#define RIGHT (1)
//#define TRACE_EDGE (1)


int TRACE_EDGE[3] = {LEFT, RIGHT, 0};														//�e�X�g�p

LineTrace::LineTrace(float black, float white, float lightThresh, LightSensor &light):
	mBlack(black), mWhite(white),mThreshold(lightThresh),
	mLightSensor(light)
{
	/*this -> mBlack = black;
	this -> mWhite = white;
	this -> mThreshold = lightThresh;
	this -> mLightSensor = light;
	*/
}

LineTrace::~LineTrace(){

}
	
float LineTrace::calcVector(){

int i;																						//�e�X�g�p

	//���K���������Z���T�l��P�Ɋi�[����
	float P = this->normalizeLightValue();
	
	//���Z���T�l��PID����ɂ�����
//	float Y = mPid.control(P);
	float Y = 5;																			//�e�X�g�p

for(i=0; i<3; i++){																			//�e�X�g�p
	
	
	if (TRACE_EDGE[i] == LEFT) {
		Y *= -1;
    	assert(TRACE_EDGE[i] == LEFT);														//�e�X�g�p
    	cout << "Test No.1 OK" << endl;														//�e�X�g�p
    }
	else if(TRACE_EDGE[i] == RIGHT) {
		Y *= 1;
		assert(TRACE_EDGE[i] == RIGHT);														//�e�X�g�p
    	cout << "Test No.2 OK" << endl;														//�e�X�g�p
	}
	else {
		/* ��O */
		Y *= 1;
		assert(TRACE_EDGE[i] != LEFT && TRACE_EDGE[i] != RIGHT);							//�e�X�g�p
    	cout << "Test No.3 OK" << endl;														//�e�X�g�p
	}

}																							//�e�X�g�p
	
	return Y;
}


float LineTrace::normalizeLightValue(){
//	float L = 0.0;
//	L = mLightSensor.get();

//	L = mLightSensor.getBrightness();														//�e�X�g�p
	int i;																					//�e�X�g�p
	float L[12] = {800.0, 800.0, 800.0, 500.0, 700.0, 400.0, 800.0, 800.0, 800.0, NULL , NAN, INFINITY};//�e�X�g�p

	float P = 0.0; // �΍�

	for(i=0; i<12; i++){																	//�e�X�g�p
	
	//L�l�`�F�b�N
	if(L[i] == NULL)
	{
		assert(L[i] == NULL);																//�e�X�g�p
    	cout << "Test No.10 OK" << endl;													//�e�X�g�p
		P = 0.0;
	}
	else if(isnan(L[i]))
	{
		assert(isnan(L[i]));																//�e�X�g�p
    	cout << "Test No.11 OK" << endl;													//�e�X�g�p
		P = 0.0;
	}
	else if(isinf(L[i]))
	{
		assert(isinf(L[i]));																//�e�X�g�p
    	cout << "Test No.12 OK" << endl;													//�e�X�g�p
		P = 0.0;
	}
	else
	{
		P = L[i] - mThreshold;
	}
    
    if(L[i] < mThreshold){ // ��
    	assert(L[i] < mThreshold);															//�e�X�g�p
    	cout << "Test No.4 OK" << endl;														//�e�X�g�p
    	P = P / (mThreshold - mWhite); // [-1.0, 1.0] �̒l�ɐ��K��
    	
    }
	else if(L[i] >= mThreshold){ // ��
    	assert(L[i] >= mThreshold);															//�e�X�g�p
    	cout << "Test No.5 OK" << endl;														//�e�X�g�p
    	P = P / (mBlack - mThreshold); // [-1.0, 1.0] �̒l�ɐ��K��
    	
    }
	else{}
		
	int j;																					//�e�X�g�p
	float tP[3] = {P, NAN, INFINITY};				//�e�X�g�p
	for(j=0; j<3; j++){																	//�e�X�g�p
	
	if(isinf(tP[j]))
	{
		assert(isinf(tP[j]));																	//�e�X�g�p
    	cout << "Test No.9 OK" << endl;														//�e�X�g�p
		P = 0.0;
	}
	else if(isnan(tP[j]))
	{
		assert(isnan(tP[j]));																	//�e�X�g�p
    	cout << "Test No.8 OK" << endl;														//�e�X�g�p
		P = 0.0;
	}
	else if(tP[j] < -1)
	{
		assert(tP[j] < -1);																		//�e�X�g�p
    	cout << "Test No.6 OK" << endl;														//�e�X�g�p
		P = -1;
	}
	else if(tP[j] > 1)
	{
		assert(tP[j] > 1);																		//�e�X�g�p
    	cout << "Test No.7 OK" << endl;														//�e�X�g�p
		P = 1;
	}
}																							//�e�X�g�p
		
}																							//�e�X�g�p
		
    return P;
}

