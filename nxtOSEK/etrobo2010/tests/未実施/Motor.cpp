//
// Motor.cpp
//
// Tsubasa
//

#include "Motor.h"

//=============================================================================
// Constructor
Motor::Motor(char* filename){

	/* �t�@�C���ǂݍ��� */
	ifstream infile( filename );

	/* ������  */
	index = -1;
	for(int i = 0; i < sizeof(mTable) / sizeof(int); i++){
		mTable[i] = 0;
	}
	
	/* �e�[�u���Ƀt�@�C�����i�[ */
	for(int i = 0; !infile.eof(); i++){
		infile >> mTable[i];
	}
}
//=============================================================================

void Motor::setCount(){
	index ++;
}

S32 Motor::getCount(){
	return mTable[index];
}
