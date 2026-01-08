//============================================
//
// メッシュフィールド処理 [ meshfield.h ]
// Author: Asuma Nishio
// 
// TODO : ここを編集してテキストから配置できるようにする
//  ( 分割数とか大きさをいじれるようにする )
//
//=============================================

#ifndef _MESHFIELD_H_ // このマクロ定義がされてなかったら
#define _MESHFIELD_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "object.h"

//**********************************
// メッシュフィールドクラスを定義
//**********************************
class CMeshField : public CObject
{
public:
	// コンストラクタ・デストラクタ
	CMeshField(int nPrio = static_cast<int>(CObject::PRIORITY::MESH));
	~CMeshField();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// セッター
	void SetTexture(void);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRaiusZ(float fRadius) { m_fRadiusZ = fRadius; }
	void SetRaiusX(float fRadius) { m_fRadius = fRadius; }

	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	float GetRadiusX(void) { return m_fRadius; }
	float GetRadiusZ(void) { return m_fRadiusZ; }

	// 静的メンバ関数
	static CMeshField* Create(D3DXVECTOR3 pos, float fRadiusX,float fRadiusZ, int nNumX, int nNumZ);

private:
	LPDIRECT3DINDEXBUFFER9 m_pIdx; // インデックスバッファ
	LPDIRECT3DVERTEXBUFFER9 m_pVtx;	// 頂点バッファ

	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_rot;		// 角度
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス

	float m_fRadius;
	float m_fRadiusZ;

	int m_nNumPrimitive;	// プリミティブ数
	int m_nNumIdx;			// インデックス数
	int m_nNumAllVtx;		// 全体頂点数


	int m_nNumX;
	int m_nNumZ;

	int m_nTexIdx; // テクスチャインデックス
};
#endif
