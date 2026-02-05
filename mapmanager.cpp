//=========================================================
//
// マップマネージャー処理 [ mapmanager.cpp ]
// Author : Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "mapmanager.h"
#include "mapobject.h"
#include "modellist.h"
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CMapManager::CMapManager(int nPriority) : CObject(nPriority),
m_MapObj{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CMapManager::~CMapManager()
{
	// 無し
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CMapManager::Init(void)
{
	// 配列クリア
	m_MapObj.clear();

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CMapManager::Uninit(void)
{
	// 値のクリア
	m_MapObj.clear();

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CMapManager::Update(void)
{
	// 無し
}
//=========================================================
// 描画処理
//=========================================================
void CMapManager::Draw(void)
{
	// 無し
}
//=========================================================
// 配列の破棄
//=========================================================
void CMapManager::ClearObj(void)
{
	// オブジェクトのクリア
	for (auto it = m_MapObj.begin(); it != m_MapObj.end(); it++)
	{
		(*it)->Uninit();
	}

	// 値のクリア
	m_MapObj.clear();
}
//=========================================================
// 削除処理
//=========================================================
void CMapManager::DeleteObj(CMapObject* pMapObject)
{
	// 削除処理
	auto DeleteDestObj = std::find(m_MapObj.begin(), m_MapObj.end(),pMapObject);

	// もし最後尾なら
	if (DeleteDestObj == m_MapObj.end())
	{
		return;
	}

	// 削除
	(*DeleteDestObj)->Uninit();

	// 先をnullにする
	pMapObject = nullptr;

	// 配列の要素を消す
	DeleteDestObj = m_MapObj.erase(DeleteDestObj);
}
//=========================================================
// 生成処理
//=========================================================
CMapManager* CMapManager::Craete(void)
{
	// インスタンス生成
	CMapManager* pMapManager = new CMapManager;
	if (pMapManager == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pMapManager->Init())) return nullptr;

	// 生成されたポインタを返す
	return pMapManager;
}