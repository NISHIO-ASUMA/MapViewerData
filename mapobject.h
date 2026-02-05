//=========================================================
//
// マップに出すオブジェクト処理 [ mapobject.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object.h"

//*********************************************************
// マップオブジェクトクラスを定義
//*********************************************************
class CMapObject : public CObject
{
public:

	//****************************
	// 色の種類
	//****************************
	enum COLORTYPE
	{
		COLORTYPE_RED = 0,
		COLORTYPE_BLUE,
		COLORTYPE_MAX
	};

	CMapObject(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CMapObject();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool CollisionMouse(float* Distance);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	void SetIdx(int nIdx) { m_nTypeIdx = nIdx; }
	void SetIsStatic(bool isFlags) { m_isStatic = isFlags; }
	D3DXMATRIX GetMtx(void) { return m_mtxworld; }

	D3DXVECTOR3 GetPos() const { return m_pos; }
	D3DXVECTOR3 GetRot() const { return m_rot; }
	D3DXVECTOR3 GetSize()const { return m_size; }
	D3DXMATRIX GetMtx() const { return m_mtxworld;}
	bool getIsStatic() const { return m_isStatic; }
	int GetIdx() const { return m_nTypeIdx; }

	static CMapObject* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nIdx, bool isflags);

private:

	int m_nTypeIdx;		// モデルの種類インデックス
	bool m_isStatic;	// 固定化オブジェクトフラグ

	D3DXVECTOR3 m_pos; // 座標
	D3DXVECTOR3 m_rot; // 角度
	D3DXVECTOR3 m_size;// 拡大率
	D3DXMATRIX m_mtxworld; // ワールドマトリックス
};