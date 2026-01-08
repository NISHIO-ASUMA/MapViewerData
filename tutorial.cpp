//=====================================
//
// チュートリアル処理 [ tutorial.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "tutorial.h"
#include "manager.h"
#include "sound.h"
#include "game.h"
#include "ui.h"

//===============================
// オーバーロードコンストラクタ
//===============================
CTutorial::CTutorial() : CScene(CScene::MODE_TUTORIAL)
{

}
//===============================
// デストラクタ
//===============================
CTutorial::~CTutorial()
{
	// 無し
}
//===============================
// 初期化処理
//===============================
HRESULT CTutorial::Init(void)
{
	// サウンド取得
	CSound* pSound = CManager::GetSound();

	// nullだったら
	if (pSound == nullptr) return E_FAIL;

	// サウンド再生
	// pSound->PlaySound(CSound::SOUND_LABEL_TUTORIALBGM);

	// ui生成
	CUi::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), 0, SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, "tutorial.jpg", false);

	// 初期化結果を返す
	return S_OK;
}
//===============================
// 終了処理
//===============================
void CTutorial::Uninit(void)
{

}
//===============================
// 更新処理
//===============================
void CTutorial::Update(void)
{
	// キー入力
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
	{
		CFade* pFade = CManager::GetFade();

		if (pFade != nullptr)
		{
			pFade->SetFade(new CGame());

			return;
		}
	}
}
//===============================
// 描画処理
//===============================
void CTutorial::Draw(void)
{
	// 無し
}
//===============================
// 生成処理
//===============================
CTutorial* CTutorial::Create(void)
{
	// インスタンス生成
	CTutorial* pTutorial = new CTutorial;

	// 生成失敗時
	if (pTutorial == nullptr) return nullptr;

	// もし初期化に失敗したら
	if (FAILED(pTutorial->Init()))
	{
		return nullptr;
	}

	// 生成されたポインタを返す
	return pTutorial;
}