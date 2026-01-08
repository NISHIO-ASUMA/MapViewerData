//================================================
//
// マップに出すオブジェクト処理 [ mapobject.h ]
// Author: Asuma Nishio
//
//================================================

#ifndef _MAPOBJECT_H_ // このマクロ定義がされてなかったら
#define _MAPOBJECT_H_ // 2重インクルード防止のマクロ定義

//******************************
// インクルードファイル
//******************************
#include "object.h"

//*********************************
// マップオブジェクトクラスを定義
//*********************************
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

	//***************************
	// 種類の列挙型
	//***************************
	enum TYPE
	{
		TYPE_NONE,			// すぐ飛ぶ
		TYPE_LIGHTTRUSH,	// 軽い
		TYPE_HEAVYTRUSH,	// 重い
		TYPE_MAX
	};

	// コンストラクタ・デストラクタ
	CMapObject(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CMapObject();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool CollisionMouse(float* Distance);

	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	void SetIdx(int nIdx) { m_nTypeIdx = nIdx; }
	D3DXMATRIX GetMtx(void) { return m_mtxworld; }
	void SetColor(int nColor) { m_nColorType = nColor; }
	void SetMass(int nMass) { MassType = nMass; }
	void SetIsStatic(bool isFlags) { m_isStatic = isFlags; }

	// ゲッター
	D3DXVECTOR3 GetPos() const { return m_pos; }
	D3DXVECTOR3 GetRot() const { return m_rot; }
	D3DXVECTOR3 GetSize()const { return m_size; }
	D3DXMATRIX GetMtx() const { return m_mtxworld;}
	bool getIsStatic() const { return m_isStatic; }

	// インデックス
	int GetIdx() const { return m_nTypeIdx; }
	int GetColorType() const { return m_nColorType; }
	int GetMassType() const { return MassType; }

	// 静的メンバ関数
	static CMapObject* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, int nIdx, bool isflags);

private:
	int m_nTypeIdx; // モデルの種類インデックス
	int m_nColorType; // 色の種類
	int MassType;	  // 重さの種類

	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	D3DXVECTOR3 m_size;
	D3DXMATRIX m_mtxworld;

	bool m_isStatic = false;	// 初期状態は動かせる

};

#endif