#ifndef CSCENEMANAGER_H
#define CSCENEMANAGER_H
#include "CScene.h"

/*
シーンマネージャクラス
*/
class CSceneManager {
public:
	CScene::EScene mScene; //シーンの識別
	CScene *mpScene; //シーンへのポインタ
	//static
	
	//コンストラクタ
	CSceneManager();
	//デストラクタ（削除されるときに実行されます）
	~CSceneManager();
	//初期化処理
	void Init();
	//更新処理
	void Update();
};

extern CSceneManager SceneManager;

#endif
