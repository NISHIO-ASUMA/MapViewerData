//=======================================
//
// ランキングシーン処理 [ ranking.cpp ]
// Author: Asuma Nishio
//
//=======================================

//******************************
// インクルードファイル宣言
//******************************
#include "ranking.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "title.h"
#include "ui.h"

//===================================
// オーバーロードコンストラクタ
//===================================
CRanking::CRanking() : CScene(CScene::MODE_RANKING)
{

}
//===================================
// デストラクタ
//===================================
CRanking::~CRanking()
{
	// 無し
}
//===================================
// 生成処理
//===================================
CRanking* CRanking::Create(void)
{
	// インスタンス生成
	CRanking* pRanking = new CRanking;
	if (pRanking == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pRanking->Init()))
	{
		return nullptr;
	}

	// 生成されたポインタを返す
	return pRanking;
}
//===================================
// 初期化処理
//===================================
HRESULT CRanking::Init(void)
{
	// ui生成
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "ranking.jpg", false);

	// 初期化結果を返す
	return S_OK;
}
//===================================
// 終了処理
//===================================
void CRanking::Uninit(void)
{
	// 無し
}
//===================================
// 更新処理
//===================================
void CRanking::Update(void)
{
	// 決定キー入力
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_A) || CManager::GetJoyPad()->GetTrigger(CJoyPad::JOYKEY_START))
	{
		// フェード取得
		CFade* pFade = CManager::GetFade();
		if (pFade == nullptr) return;

		// 画面遷移
		pFade->SetFade(new CTitle(true));

		return;
	}
}
//===================================
// 描画処理
//===================================
void CRanking::Draw(void)
{
	// 無し
}