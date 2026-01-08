//================================================
//
// ブロック管理処理 [ blockmanager.h ]
// Author: Asuma Nishio
//
//================================================

//******************************
// インクルードガード
//******************************
#pragma once

//******************************
// インクルードファイル
//******************************
#include "block.h"
#include <vector>

//******************************
// ブロック管理クラスを定義
//******************************
class CBlockManager
{
public:
	// コンストラクタ・デストラクタ
	CBlockManager();
	~CBlockManager();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Load(const char* jsonname);

	// 生成処理
	static CBlock* CreateManager(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, const char* pModelName);

private:
	static std::vector<CBlock*>m_pBlocks; // 動的配列

};

