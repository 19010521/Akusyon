#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include"CCollider.h"
#include"CPoint.h"


class CXEnemy :public CXCharacter{
public:
	enum Estate
	{
		ENORMAL,//通常

		ENEAR,//近ずく

		EATTACK1,//突進

		EJUNP,//ジャンプ

		ELANDING,//着地

		EBACKFLIP,//後転


	

	};
	Estate mstate;

	static int mPointSize;//ポイントの数
	CPoint*mpPoint;//目指すポイント
	int mPointCnt;//ポイントのカウンタ

	//誘導ポイント
	static CPoint *mPoint;
	int Randam;
	//CEnemy(モデル,位置,回転,拡縮)
	
	CXEnemy();
	void Update();
	CCollider mColSphereBody;//体
	
	CCollider mColSphereHead;   //頭
	CCollider mColSphereSword0;  //剣
	CCollider mColSphereSword1;  //剣1
	CCollider mColSphereSword2;  //剣2
	CCollider mSearch;
	CCollider mSearch2;

	//CCollider mSearch2;
	float mVelovcityJump;
	int mnearCount;
	int mnearCountMax;
	int mCount;
	int mCountMax;
	bool jflag;//ジャンプフラグ
	bool lflag;//ジャンプフラグ
	static bool Attackflag;
	void Init(CModelX*model);
	//衝突処理
	void Collision(CCollider*m, CCollider*y);
};
#endif
