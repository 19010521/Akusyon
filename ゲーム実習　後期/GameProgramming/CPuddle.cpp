#include"CPuddle.h"
#include"CXPlayer.h"
#include"CKey.h"
#include"CRes.h"

#define WATERUSE (0.001f) //水の異動量
#define EPUDDLE0 (0)
#define EPUDDLE1 (1)
#define EPUDDLE2 (2)
#define EPUDDLE3 (3)



int CPuddle::mclearcount = 0;
bool CPuddle::rock;
bool CPuddle::Enemy;
bool CPuddle::Enemy1;
CPuddle::CPuddle(CModel*model,CModel*model2, CVector position, CVector rotation, CVector scale)
:mPuddle(this, CVector(0.0f, 1.0f, 0.0f), CVector(), CVector(5.0f, 3.0f, 5.0f), 2.0f)

, x(6.0f), y(0.5f), z(6.0f), mx(0.0f), mz(0.0f), mxMax(6.0f), mzMax(6.0f), frame(0), frame1(0), mCount(120), mCountMax(120)

{

	
	mTag = ENORMALPUDDLE;
	mPuddle.mTag = CCollider::EPUDDLE;
	mpModel = model;  //モデルの設定
	mPosition = position;//位置の設定
	mRotation = rotation;//回転の設定
	mScale = scale;   //拡縮の設定
	
	Puddle = model;
	MudPuddle = model2;
	usefrag = false;
	Enemy = false;
	Enemy1 = false;
	mScale = CVector(x, y, z);

}

void CPuddle::Set(const CVector &pos, float r){

	mPosition = pos;
	mPuddle.mRadius = r;

}
 
void CPuddle::Update(){


	if (rock == false){
	mclearcount ++ ;
	rock = true;
	}
	if (mpModel == Puddle){

		mTag = ENORMALPUDDLE;
		mPuddle.mTag = CCollider::EPUDDLE;

		
	}



	if (mpModel == MudPuddle){
		mTag = EMUDPUDDLE;
		mPuddle.mTag = CCollider::EMUDPUDDLE;
	
	}
	
	if (x > 0 && z > 0){
		if (usefrag == true){
			mScale = CVector(x -= mx, y, z -= mz);
		}
	}
	if (x < mxMax&&z < mzMax){
		if (usefrag == false){

			if (mCount > 0){
				mCount--;
			}
			else
			{
				mScale = CVector(x += mx, y, z += mz);
				mCount = mCountMax;
			}

		}
	}


	if (Enemy == true){
		if (mPosition.mZ < 0){
			if (mTag == EMUDPUDDLE){

				frame++;//フレーム数に1加算
				if (frame < 1000 && frame % 150 == 0){
					//敵機の生成
					mEnemy = new CXEnemy();
					//敵の初期設定
					mEnemy->Init(&CRes::sKnight);

					//敵の配置
					mEnemy->mAnimationFrameSize = 1024;
					mEnemy->mPosition = mPosition;
					mEnemy->ChangeAnimation(2, true, 200);
					if (Enemy == true){
						Enemy = false;
					}
				}
			}
		}
	}
	if (Enemy1 == true){
	if (mPosition.mZ > 0){
		if (mTag == EMUDPUDDLE){

			frame1++;//フレーム数に1加算
			if (frame1 < 1000 && frame1 % 150 == 0){
				//敵機の生成
				mEnemy = new CXEnemy();
				//敵の初期設定
				mEnemy->Init(&CRes::sKnight);

				//敵の配置
				mEnemy->mAnimationFrameSize = 1024;
				mEnemy->mPosition = mPosition;
				mEnemy->ChangeAnimation(2, true, 200);
				if (Enemy1 == true){
					Enemy1 = false;
				}
			}
		}
	}
}
	
	
	
		

	CCharacter::Update();
	
}

void CPuddle::Collision(CCollider*m, CCollider*y){

	//共に球コライダの時
	if (m->mType == CCollider::ESPHERE && y->mType == CCollider::ESPHERE){
		//コライダのｍとｙが衝突しているか判定
		if (CCollider::Collision(m, y)){
			//泥の水たまり
			if (m->mTag == CCollider::EMUDPUDDLE){
				//アイテム
				if (CXPlayer::mpxPlayer->mClean_up > 0){
					if (CKey::Push('Q')){
						rock = false;
						if (mpModel == MudPuddle){
							CXPlayer::mpxPlayer->mClean_up--;
						}
						mpModel = Puddle;
					}
				}

			}
			//普通の水たまり
				if (m->mTag == CCollider::EPUDDLE){
					
					if (y->mTag == CCollider::EPLAYEREBODY){
						
						if (CKey::Push('Q') && CXPlayer::mpxPlayer->mWaterCount < CXPlayer::mpxPlayer->mWaterCountMax){

							if (mx <= 1.0f && mz <= 1.0f){
								mx += WATERUSE;
								mz += WATERUSE;

							}
							else{
								mx = 0;
								mz = 0;

							}
							CXPlayer::mpxPlayer->mWaterCount++;
							usefrag = true;

						}
						else if (usefrag == true)
						{
							usefrag = false;
						}


						if (CXPlayer::mpxPlayer->mstate == CXPlayer::mpxPlayer->EMUD ){
							
								CXPlayer::mpxPlayer->mstate = CXPlayer::mpxPlayer->ENORMAL;
								mpModel = MudPuddle;
							
						}
						

						}
					if (y->mTag == CCollider::EENEMYBODY){
						
						mclearcount--;
						mpModel = MudPuddle;

					}
				}
			}
		}
	}



