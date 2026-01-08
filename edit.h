//================================================
//
// マップエディターシーン処理 [ edit.h ]
// Author: Asuma Nishio
//
//================================================

//************************************
// インクルードガード
//************************************
#pragma once

//************************************
// インクルードファイル
//************************************
#include "scene.h"

//**********************************
// 前方宣言
//**********************************
class CEditManager;

//************************************
// オブジェクト編集シーンクラスを定義
//************************************
class CEdit : public CScene
{
public:
	// コンストラクタ・デストラクタ
	CEdit();
	~CEdit();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ゲッター
	static CEditManager* GetPoint(void) { return m_pEditManager; }

private:
	static CEditManager* m_pEditManager;

};

