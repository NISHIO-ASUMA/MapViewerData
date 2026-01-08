//====================================
//
// ブロック処理 [ block.cpp ]
// Author: Asuma Nishio
//
//=====================================

//******************************
// インクルードファイル宣言
//******************************
#include "block.h"
#include "template.h"

//===============================
// オーバーロードコンストラクタ
//===============================
CBlock::CBlock(int nPriority) : CObjectX(nPriority)
{
	// 値のクリア
	m_nType = NULL;
}
//===============================
// デストラクタ
//===============================
CBlock::~CBlock()
{
	// 無し
}
//===============================
// 生成処理
//===============================
CBlock* CBlock::Create(const char* pFileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	// インスタンスを生成
	CBlock* pBlock = new CBlock;

	// nullptrだったら
	if (pBlock == nullptr) return nullptr;

	// オブジェクト設定
	pBlock->SetFilePass(pFileName);
	pBlock->SetPos(pos);
	pBlock->SetRot(rot);
	pBlock->SetScale(scale);

	// 初期化に失敗したら
	if (FAILED(pBlock->Init()))
	{
		// nullptrを返す
		return nullptr;
	}

	// 生成ポインタを返す
	return pBlock;
}
//===============================
// 初期化処理
//===============================
HRESULT CBlock::Init(void)
{
	// 親クラスの初期化
	CObjectX::Init();

	// 結果を返す
	return S_OK;
}
//===============================
// 終了処理
//===============================
void CBlock::Uninit(void)
{
	// 親クラスの終了
	CObjectX::Uninit();
}
//===============================
// 更新処理
//===============================
void CBlock::Update(void)
{

}
//===============================
// 描画処理
//===============================
void CBlock::Draw(void)
{
	// 親クラスの描画
	CObjectX::Draw();
}
//===============================
// 矩形の当たり判定
//===============================
bool CBlock::Colliosion(D3DXVECTOR3* pPos, D3DXVECTOR3 *pPosOld,D3DXVECTOR3 * pMove, D3DXVECTOR3 fSize,bool& bLanding)
{
#if 0
	// フラグ
	bool isCheck = false;

	// サイズ取得
	D3DXVECTOR3 Blocksize = CObjectX::GetSize();

	// サイズ判定 ( old )
	float fPosOldMinX = pPosOld->x - fSize.x * BLOCKHALF;
	float fPosOldMinY = pPosOld->y - fSize.y * BLOCKHALF;
	float fPosOldMinZ = pPosOld->z - fSize.z * BLOCKHALF;

	// サイズ判定 ( old )
	float fPosOldMaxX = pPosOld->x + fSize.x * BLOCKHALF;
	float fPosOldMaxY = pPosOld->y + fSize.y * BLOCKHALF;
	float fPosOldMaxZ = pPosOld->z + fSize.z * BLOCKHALF;

	// サイズ判定 ( pos )
	float fPosMinX = pPos->x - fSize.x * BLOCKHALF;
	float fPosMinY = pPos->y - fSize.y * BLOCKHALF;
	float fPosMinZ = pPos->z - fSize.z * BLOCKHALF;

	// サイズ判定 ( pos )
	float fPosMaxX = pPos->x + fSize.x * BLOCKHALF;
	float fPosMaxY = pPos->y + fSize.y * BLOCKHALF;
	float fPosMaxZ = pPos->z + fSize.z * BLOCKHALF;

	// サイズ判定 ( ブロック自身のpos )
	float fBlockPosMaxX = GetPos().x + Blocksize.x * BLOCKHALF;
	float fBlockPosMaxY = GetPos().y + Blocksize.y * BLOCKHALF;
	float fBlockPosMaxZ = GetPos().z + Blocksize.z * BLOCKHALF;

	// サイズ判定 ( ブロック自身のpos )
	float fBlockPosMinX = GetPos().x - Blocksize.x * BLOCKHALF;
	float fBlockPosMinY = GetPos().y - Blocksize.y * BLOCKHALF;
	float fBlockPosMinZ = GetPos().z - Blocksize.z * BLOCKHALF;

	// 引数サイズ判定 ( 引数サイズ )
	float SizeX = fSize.x * BLOCKHALF;
	float SizeY = fSize.y * BLOCKHALF;
	float SizeZ = fSize.z * BLOCKHALF;

	// コリジョンしたら
	if (pPos->y < GetPos().y + Blocksize.y &&
		pPos->y + fSize.y > GetPos().y - Blocksize.y)
	{
		// 左右の当たり判定----------------------
		if (fPosOldMinZ < fBlockPosMaxZ &&
			fPosMaxZ > fBlockPosMinZ)
		{
			// 左からめり込む
			if (fPosOldMaxX < fBlockPosMinX &&
				fPosMaxX > fBlockPosMinX)
			{
				pPos->x = GetPos().x - Blocksize.x * BLOCKHALF - fSize.x * BLOCKHALF - 0.3f;
				pMove->x = 0.0f;
				isCheck = true;

			}
			// 右からめり込む
			else if (fPosOldMinX > fBlockPosMaxX &&
					 fPosMinX < fBlockPosMaxX)
			{
				pPos->x = GetPos().x + Blocksize.x * BLOCKHALF + fSize.x * BLOCKHALF + 0.8f;
				pMove->x = 0.0f;
				isCheck = true;
			}
		}

		// 前後の当たり判定------------------------------
		if (pPosOld->x - SizeZ < fBlockPosMaxX &&
			fPosMaxX > fBlockPosMinX)
		{
			// 手前からめり込む
			if (fPosOldMaxZ < fBlockPosMinZ &&
				fPosMaxZ > fBlockPosMinZ)
			{
				pPos->z = GetPos().z - Blocksize.z * BLOCKHALF - fSize.z * BLOCKHALF - 0.3f;
				isCheck = true;

			}
			// 奥からめり込む
			else if (fPosOldMinZ > fBlockPosMaxZ &&
				fPosMinZ < fBlockPosMaxZ)
			{
				pPos->z = GetPos().z + Blocksize.z * BLOCKHALF + fSize.z * BLOCKHALF + 0.4f;
				isCheck = true;
			}
		}

		// 縦の当たり判定--------------------------
		if (fPosMinX < fBlockPosMaxX &&
			fPosMaxZ > fBlockPosMinX)
		{
			if (fPosOldMinZ < fBlockPosMaxZ &&
				fPosMaxZ > fBlockPosMinZ)
			{
				// 上からめり込んだ時
				if (pPosOld->y >= fBlockPosMaxY &&
					pPos->y < fBlockPosMaxY)
				{
					bLanding = true;
					pPos->y = (GetPos().y + Blocksize.y);
					pMove->y = 0.0f;
					isCheck = true;
				}
				// 下からめり込んだ時
				else if (pPosOld->y + fSize.y <= fBlockPosMinY &&
					pPos->y + fSize.y > fBlockPosMinY)
				{
					pPos->y = (GetPos().y - Blocksize.y) - SizeY;
					pMove->y = 0.0f;
					isCheck = true;
				}
			}
		}
	}

#endif
	return false;
}