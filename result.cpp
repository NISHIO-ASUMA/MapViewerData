//====================================
//
// リザルトシーン処理 [ result.cpp ]
// Author: Asuma Nishio
// 
//=====================================

//**************************
// インクルードファイル宣言
//**************************
#include "result.h"
#include "ui.h"
#include "manager.h"
#include "ranking.h"

//=================================
// オーバーロードコンストラクタ
//=================================
CResult::CResult() : CScene(CScene::MODE_RESULT)
{

}
//=================================
// デストラクタ
//=================================
CResult::~CResult()
{
	// 無し
}
//=================================
// 生成処理
//=================================
CResult* CResult::Create(void)
{
	// インスタンス生成
	CResult* pResult = new CResult;

	// 生成に失敗したら
	if (pResult == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pResult->Init()))
	{
		return nullptr;
	}

	// 生成されたポインタを返す
	return pResult;
}
//=================================
// 初期化処理
//=================================
HRESULT CResult::Init(void)
{
	// ui生成
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "result.jpg", false);

	// 初期化結果を返す
	return S_OK;
}
//=================================
// 終了処理
//=================================
void CResult::Uninit(void)
{
	// 無し
}
//=================================
// 更新処理
//=================================
void CResult::Update(void)
{
	// キー入力
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
	{
		CFade* pFade = CManager::GetFade();

		if (pFade != nullptr)
		{
			pFade->SetFade(new CRanking());

			return;
		}
	}

}
//=================================
// 描画処理
//=================================
void CResult::Draw(void)
{
	// 無し
}
