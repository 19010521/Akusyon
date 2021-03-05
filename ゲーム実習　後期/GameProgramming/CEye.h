#ifndef CEYE_H
#define CEYE_H
#include "CCharacter.h"
#include"CInput.h"
class CEye :public CCharacter{
private:

	CEye(CModel*model, CVector position, CVector rotation, CVector scale);

	int mMouseX;
	int mMouseY;

	void Update();
	
public:
	
	static CEye *mpthis;
	
	CEye(){
		mMouseX = 1920 / 2;
		mMouseY = 1080 / 2;
		mpthis = this;
		CInput::SetMousePos(mMouseX, mMouseY);
		CInput::GetMousePos(&mMouseX, &mMouseY);
	};

};
#endif