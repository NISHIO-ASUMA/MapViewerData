//================================================
//
// ブロック管理処理 [ blockmanager.cpp ]
// Author: Asuma Nishio
//
//================================================

//******************************
// インクルードファイル
//******************************
#include "CBlockManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "json.hpp"

// jsonファイル形式を使用する
using json = nlohmann::json;

//******************************
// 静的メンバ変数宣言
//******************************
std::vector<CBlock*>CBlockManager::m_pBlocks = {}; // 配列

//=============================
// コンストラクタ
//=============================
CBlockManager::CBlockManager()
{
}
//=============================
// デストラクタ
//=============================
CBlockManager::~CBlockManager()
{

}
//=============================
// 生成処理
//=============================
CBlock* CBlockManager::CreateManager(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, const char* pModelName)
{
	// インスタンス生成
	CBlock* pblock = CBlock::Create(pModelName, pos, rot, scale);

	if (pblock)
	{
		m_pBlocks.push_back(pblock);
	}

	// ポインタ
	return pblock;
}
//=============================
// 初期化処理
//=============================
HRESULT CBlockManager::Init(void)
{
	// クリア
	m_pBlocks.clear();

	return S_OK;
}
//=============================
// 終了処理
//=============================
void CBlockManager::Uninit(void)
{

	// クリア
	m_pBlocks.clear();
}
//=============================
// 更新処理
//=============================
void CBlockManager::Update(void)
{
	// 無し
}
//=============================
// json読み込み処理
//=============================
void CBlockManager::Load(const char* jsonname)
{
	// 開くファイル
	std::ifstream file(jsonname);

	// 開け無かった
	if (!file.is_open())
	{
		// 例外
		MessageBox(nullptr, "ファイルの読み込みに失敗しました", "エラー", MB_OK | MB_ICONERROR);

		// 終了
		return;
	}

	json j;
	file >> j;

	// ファイルを閉じる
	file.close();

	// 既存のブロックを消す
	for (auto block : m_pBlocks)
	{
		if (block != nullptr)
		{
			// ブロックの終了処理
			block->Uninit();
		}
	}

	// 動的配列を空にする
	m_pBlocks.clear();

	// SetObjectsの配列を回す
	for (const auto& b : j["SetObjects"])
	{
		std::string filepath = b["filepass"];
		int idx = b["idx"];

		D3DXVECTOR3 pos(
			b["pos"][0],
			b["pos"][1],
			b["pos"][2]
		);
		D3DXVECTOR3 rot(
			b["rot"][0],
			b["rot"][1],
			b["rot"][2]
		);

		D3DXVECTOR3 size(
			b["scale"][0],
			b["scale"][1],
			b["scale"][2]
		);

		// ブロック生成
		CBlock* block = CreateManager(pos, rot, size, filepath.c_str());
	}
}