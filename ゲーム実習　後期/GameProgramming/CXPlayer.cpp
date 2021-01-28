#include"CXPlayer.h"
#include"CKey.h"
#include"CInput.h"
#include"CWaterGun.h"
#include"CXEnemy.h"
#include"CSceneGame.h"
#include"CBullet.h"
#include"CCollisionManager.h"
#include"CEye.h"
CXPlayer *CXPlayer::mpxPlayer = 0;
float CXPlayer::mHPMax = 100;
float CXPlayer::mHPNow = mHPMax;

#define G (9.8f/60.0f)//重力加速度
#define JUMPV0 (1.5f)//ジャンプ初速
#define MOUSE_SENSE 4
#define TURN (-360.0f/60*3)//回転
CXPlayer::CXPlayer()
:mColSphereBody(this, CVector(0.0f, 3.0f, 0.0f), CVector(), CVector(1.0f, 1.0f, 1.0f), 1.8f)
, mColSphereHead(this, CVector(0.0f, 0.5f, -3.0f), CVector(), CVector(1.0f, 1.0f, 1.0f), 0.5f)
, mColSphereSword(this, CVector(-10.0f, 10.0f, 50.0f), CVector(), CVector(1.0f, 1.0f, 1.0f), 0.3f)
, mColSphereLeg0(this, CVector(0.0f, 1.5f, 0.0f), CVector(), CVector(5.0f, 5.0f, 5.0f), 0.5f)
, mColSphereLeg1(this, CVector(0.0f, 1.5f, 0.0f), CVector(), CVector(5.0f, 5.0f, 5.0f), 0.5f)
, mVelovcityJump(0), mBulletCount(0), mBulletCountMax(60), mWaterCount(0), mWaterCountMax(3)
, mInvincibleCountMax(150), mInvincibleCount(150), mTunRotation(0.0f,2.5f,0.0f)
, mMudCount(0), mDrawCount(0)
, mSpeed(0)
{
	Damege = false;

	jflag = false;

	waterflag = false;

	mpxPlayer = this;


	mScale = CVector(1.0f, 1.0f, 1.0f);
	//タグにプレイヤーを設定します
	mTag = EPLAYER;

	mstate = ENORMAL;
	//mColSphereSword.mTag = CCollider::ESWORD;
	mColSphereBody.mTag = CCollider::EPLAYEREBODY;


}
void CXPlayer::Init(CModelX*model){


	CXCharacter::Init(model);
	//合成行列
	mColSphereBody.mpCombinedMatrix =
		&mpCombinedMatrix[8];
	//合成行列
	mColSphereHead.mpCombinedMatrix =
		&mpCombinedMatrix[11];
	//合成行列
	mColSphereSword.mpCombinedMatrix =
		&mpCombinedMatrix[21];

	mColSphereLeg0.mpCombinedMatrix =
		&mpCombinedMatrix[25];

	mColSphereLeg1.mpCombinedMatrix =
		&mpCombinedMatrix[30];

}

void CXPlayer::TaskCollision()
{
	mColSphereBody.ChangePriority();
	mColSphereHead.ChangePriority();
	mColSphereLeg0.ChangePriority();
	CollisionManager.Collision(&mColSphereBody);
	CollisionManager.Collision(&mColSphereHead);
	CollisionManager.Collision(&mColSphereLeg0);

}


void CXPlayer::Update(){


	mVelovcityJump -= G;

	//if (CKey::Once('Z')){
	//	for (int s = 0; s < 359; s++){
	//		CBullet*bullet = new CBullet();
	//		bullet->Set(0.1f, 1.5f);
	//		bullet->mPosition = CVector(0.0f, 0.0f, 10.0f)*CMatrix().RotateY(s)*mMatrix;
	//		bullet->mRotation = CVector(0.0f, s, 0.0f);
	//		//mFireCount = 60;
	//	}
	//}
	
		if (mAnimationIndex != 11 && mstate == EMUD || mstate == EINVINCIBLE){
			if (CKey::Push('W') || CKey::Push('A') || CKey::Push('D') || CKey::Push('S')){

				mRotation.mY = CEye::mpthis->mRotation.mY;

				ChangeAnimation(1, true, 60);

				mPosition = CVector(0.0f, 0.0f, 0.1f)*mMatrix;

				if (CKey::Push('S')){

					mRotation.mY += 180;

					mPosition = CVector(0.0f, 0.0f, 0.1f)*mMatrix;
				}

				if (CKey::Push('A')){
					mRotation.mY += 90;
					mPosition = CVector(0.0f, 0.0f, 0.1f)*mMatrix;

					if (CKey::Push('S')){
						mRotation.mY += 225;
						mPosition = CVector(0.0f, 0.0f, 0.1f)*mMatrix;
					}

					if (CKey::Push('W')){
						mRotation.mY += 315;
						mPosition = CVector(0.0f, 0.0f, 0.1f)*mMatrix;
					}
				}

				if (CKey::Push('D')){
					mRotation.mY += 270;
					mPosition = CVector(0.0f, 0.0f, 0.1f)*mMatrix;
					if (CKey::Push('S')){
						mRotation.mY += 135;
						mPosition = CVector(0.0f, 0.0f, 0.1f)*mMatrix;
					}

					if (CKey::Push('W')){
						mRotation.mY += 45;
						mPosition = CVector(0.0f, 0.0f, 0.1f)*mMatrix;
					}
				}

			}
			else if (mAnimationIndex == 1){
				ChangeAnimation(0, true, 60);
			}



			if (mAnimationIndex == 3){
				if (mAnimationFrame >= mAnimationFrameSize){
					ChangeAnimation(4, false, 30);
				}
			}

			if (mAnimationIndex == 4){
				if (mAnimationFrame >= mAnimationFrameSize){
					ChangeAnimation(0, true, 60);
				}
			}
		}

		if (mAnimationIndex != 11 && mstate == ENORMAL){
			if (CKey::Push('W') || CKey::Push('A') || CKey::Push('D') || CKey::Push('S')){

				mRotation.mY = CEye::mpthis->mRotation.mY;

				ChangeAnimation(1, true, 60);

				mPosition = CVector(0.0f, 0.0f, 0.2f - mSpeed)*mMatrix;

				if (CKey::Push('S')){
					mRotation.mY += 180;

					mPosition = CVector(0.0f, 0.0f, 0.2f - mSpeed)*mMatrix;
				}

				if (CKey::Push('A')){
					mRotation.mY += 90;
					mPosition = CVector(0.0f, 0.0f, 0.2f - mSpeed)*mMatrix;
					if (CKey::Push('S')){
						mRotation.mY += 225;
						mPosition = CVector(0.0f, 0.0f, 0.2f - mSpeed)*mMatrix;
					}
					if (CKey::Push('W')){
						mRotation.mY += 315;
						mPosition = CVector(0.0f, 0.0f, 0.2f - mSpeed)*mMatrix;
					}
				}

				if (CKey::Push('D')){
					mRotation.mY += 270;
					mPosition = CVector(0.0f, 0.0f, 0.2f - mSpeed)*mMatrix;
					if (CKey::Push('S')){
						mRotation.mY += 135;
						mPosition = CVector(0.0f, 0.0f, 0.2f - mSpeed)*mMatrix;
					}
					if (CKey::Push('W')){
						mRotation.mY += 45;
						mPosition = CVector(0.0f, 0.0f, 0.2f - mSpeed)*mMatrix;
					}
				}

			}
			else if (mAnimationIndex == 1){
				ChangeAnimation(0, true, 60);
			}

			if (mAnimationIndex == 3){
				if (mAnimationFrame >= mAnimationFrameSize){
					ChangeAnimation(4, false, 30);
				}
			}
			if (mAnimationIndex == 4){
				if (mAnimationFrame >= mAnimationFrameSize){
					ChangeAnimation(0, true, 60);
				}
			}

			/*if (CInput::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)){

			if (mAnimationIndex == 0 || mAnimationIndex == 1){
			ChangeAnimation(3, false, 30);
			}
			}*/



			if (waterflag == true && mWaterCount > 0){

				if (mBulletCount > 0 && jflag == false){

					mBulletCount--;
				}

				//マウスの右入力で弾発射
				else if (CInput::GetMouseButton(GLFW_MOUSE_BUTTON_RIGHT)){

					CWaterGun*bullet = new CWaterGun();
					bullet->Set(0.1f, 1.5f);
					bullet->mPosition = CVector(0.0f, 5.0f, 1.0f)*mMatrix;
					//bullet->mRotation = mRotation;
					bullet->mForward = bullet->mForward*mMatrixRotate;
					mBulletCount = mBulletCountMax;
					mWaterCount -= 1;
					if (mWaterCount <= 0){
						waterflag = false;
					}
					mSpeed -= 0.03f;

				}
				//マウスの左入力で全弾発射
				else if (CInput::GetMouseButton(GLFW_MOUSE_BUTTON_LEFT)){
					if (mWaterCount == 3){

						CWaterGun*bullet = new CWaterGun();
						bullet->Set(0.1f, 1.5f);
						bullet->mPosition = CVector(0.0f, 5.0f, 1.0f)*mMatrix;
						//bullet->mRotation = mRotation;
						bullet->mForward = bullet->mForward*mMatrixRotate;
						mBulletCount = mBulletCountMax;
						waterflag = false;
						mWaterCount = 0;
						mSpeed -= 0.09;
						//bullet->mTag = CCharacter::;


						/*if (mAIM == true){
						mRotation.mX -= 20;
						mRotation.mY -= 20;
						}*/

					}
				}
			}

		}



		//ジャンプ
		if (jflag == false){
			mPosition.mY += mVelovcityJump;
			mVelovcityJump -= G;
		}
		if (jflag == true){

			mPosition.mY += mVelovcityJump;
			mTurn -= TURN;


		}
		else if (CKey::Once(VK_SPACE)){

			if (jflag == false && mstate == ENORMAL){
				mVelovcityJump = JUMPV0;
				mPosition.mY += mVelovcityJump;
				//mPosition = CVector(0.0f, 0.0f, 0.5f)*mMatrix;
				jflag = true;
				mTurn = TURN;
			}
		}
	

		//無敵状態
		if (mstate == EINVINCIBLE){
			if (mInvincibleCount>0){
				mInvincibleCount--;

			}
			else if (mMudCount < 3)
			{
				mstate = ENORMAL;
				mInvincibleCount = mInvincibleCountMax;

			}
			else
			{
				mstate = EMUD;
				mInvincibleCount = mInvincibleCountMax;
			}

		}
		//泥状態になる
		if (mMudCount > 3){
			mstate = EMUD;
		}
		//ダメージ1
		if (Damege == true){

			mHPNow -= 20;
			mMudCount += 1;
			Damege = false;

			mstate = EINVINCIBLE;
		}
		//ダメージ2
		if (Damege2 == true){

			mHPNow -= 5;
			Damege2 = false;

			mstate = EINVINCIBLE;
		}
		//死んだ--
		if (mAnimationIndex == 11){
			if (mAnimationFrame >= mAnimationFrameSize){
				mstate = EDESU;
			}
		}


		if (mHPNow < 0){
			ChangeAnimation(11, false, 30);
			mHPNow = 0;

		}
		CCharacter::Update();

		if (jflag == true){
			CMatrix a, b, c;
			a.Translate(-mTunRotation.mX, -mTunRotation.mY, -mTunRotation.mZ);
			b.RotateX(mTurn);
			c.Translate(mTunRotation.mX, mTunRotation.mY, mTunRotation.mZ);
			mMatrix = a*b*c*mMatrix;
		}

		CXCharacter::Update(mMatrix);

	}


void CXPlayer::Collision(CCollider*mc, CCollider*yc){

	//自身コライダの判定タイプ
	switch (mc->mType){
	case CCollider::ESPHERE://球コライダ
		//相手のコライダが三角コライダの時
		if (yc->mType == CCollider::ETRIANGLE){
			CVector adjust;//調整用ベクトル		
			if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
				jflag = false;
				mVelovcityJump = 0;
				//位置の更新
				mPosition = mPosition - adjust*-1;
				//行列の更新
				CXCharacter::Update();
			}
		}
		break;
	}
	//共に球コライダの時
	if (mc->mType == CCollider::ESPHERE && yc->mType == CCollider::ESPHERE){
		//コライダのｍとｙが衝突しているか判定
		if (CCollider::Collision(mc, yc)){



			if (mstate != EMUD){
				if (waterflag == false){
					if (yc->mTag == CCollider::EPUDDLE0){
						//水をくむ
						if (CKey::Once('Q')){
							if (CPuddle::mPuddle->UseCount > 0){

								waterflag = true;
								mWaterCount = mWaterCountMax;
								CPuddle::mPuddle->UseCount--;
								mSpeed += 0.09;
							}
						}
					}
					else if (yc->mTag == CCollider::EPUDDLE1){

						//水をくむ
						if (CKey::Once('Q')){
							if (CPuddle1::mPuddle01->UseCount > 0){
								waterflag = true;
								mWaterCount = mWaterCountMax;
								CPuddle1::mPuddle01->UseCount--;
								mSpeed += 0.09;
							}
						}
					}
					else if (yc->mTag == CCollider::EPUDDLE2){

						//水をくむ
						if (CKey::Once('Q')){
							if (CPuddle2::mPuddle02->UseCount > 0){
								waterflag = true;
								mWaterCount = mWaterCountMax;
								CPuddle2::mPuddle02->UseCount--;
								mSpeed += 0.09;
							}
						}
					}
				}
				if (yc->mTag == CCollider::EINPACT){
					if (mAnimationIndex != 11){
						Damege2 = true;
					}
				}
			}

			//衝突したコライダの親の種類を判定
			switch (yc->mpParent->mTag){
			case EENEMY:
				if (yc->mTag == CCollider::EENEMYBODY){
					if (mAnimationIndex != 11){
						if (mstate != EINVINCIBLE){
							if (CXEnemy::Attackflag == false){
								Damege = true;

							}
						}
					}
					break;
				}
			}
		}
	}
}

void CXPlayer::Render(){
	//頂点をアニメーションを適応する
	//mpModel->AnimateVertex(mpCombinedMatrix);
	//mpModel->Render();

	if (mstate == EINVINCIBLE){
		if (mDrawCount % 8 == 0){
			mpModel->RenderShader(mpCombinedMatrix);
		}
		++mDrawCount;
	}
	else
	{
		mpModel->RenderShader(mpCombinedMatrix);
	}
}
