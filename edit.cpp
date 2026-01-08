//================================================
//
// マップエディターシーン処理 [ edit.cpp ]
// Author: Asuma Nishio
//
//================================================

//********************************
// インクルードファイル
//********************************
#include "edit.h"
#include "editmanager.h"
#include "meshfield.h"
#include "imguimanager.h"
#include "manager.h"
#include "ui.h"
#include "game.h"

//********************************
// 静的変数宣言
//********************************
CEditManager* CEdit::m_pEditManager = nullptr;

//==============================
// オーバーロードコンストラクタ
//==============================
CEdit::CEdit() : CScene(CScene::MODE_EDIT)
{
}
//==============================
// デストラクタ
//==============================
CEdit::~CEdit()
{
	// 無し
}
//==============================
// 初期化処理
//==============================
HRESULT CEdit::Init(void)
{
	// メッシュフィールド
	// CMeshField::Create(VECTOR3_NULL, 2000.0f,2000.0f,1,1);

	// ポインタの生成
	m_pEditManager = new CEditManager;
	m_pEditManager->Init();

	return S_OK;
}
//==============================
// 終了処理
//==============================
void CEdit::Uninit(void)
{
	// nullチェック
	if (m_pEditManager != nullptr)
	{
		// 終了処理
		m_pEditManager->Uninit();

		// ポインタの破棄
		delete m_pEditManager;

		// null初期化
		m_pEditManager = nullptr;
	}
}
//==============================
// 更新処理
//==============================
void CEdit::Update(void)
{
	// マネージャーの更新処理
	if (m_pEditManager != nullptr)
	{
		m_pEditManager->Update();
	}
}
//==============================
// 描画処理
//==============================
void CEdit::Draw(void)
{
	// マネージャーの描画
	if (m_pEditManager != nullptr)
	{
		m_pEditManager->Draw();
	}
}