//=====================================
//
// ブロック処理 [ block.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _BLOCK_H_ // このマクロ定義がされてなかったら
#define _BLOCK_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "objectX.h"

//************************************
// ブロッククラスを定義
//************************************
class CBlock : public CObjectX
{
public:
	// コンストラクタ・デストラクタ
	CBlock(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CBlock();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadJson(const char* pJsonName);

	bool Colliosion(D3DXVECTOR3 * pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 fSize, bool& bLanding);

	// 静的メンバ関数
	static CBlock* Create(const char* pFileName, D3DXVECTOR3 pos,D3DXVECTOR3 rot, D3DXVECTOR3 scale);

private:
	static constexpr float BLOCKHALF = 0.5f;	// サイズ
	int m_nType;	// 種類
};

#endif
