//=========================================================
//
// オブジェクト処理 [ object.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// オブジェクトクラスを定義
//*********************************************************
class CObject
{
public:

	//*************************
	// オブジェクトの種類定義
	//*************************
	enum TYPE
	{
		TYPE_NONE,
		TYPE_PLAYER,
		TYPE_MESH,
		TYPE_BULLET,
		TYPE_BOSS,
		TYPE_ENEMY,
		TYPE_BLOCK,
		TYPE_PAUSE,
		TYPE_BARRIER,
		TYPE_ITEM,
		TYPE_RUBBLE,
		TYPE_PILER,
		TYPE_POINT,
		TYPE_MAX
	};

	//*************************
	// 描画番号定義
	//*************************
	enum PRIORITY
	{
		NONE,		// 初期状態
		MESH,		// メッシュ
		BASENUMBER, // 基準番号
		MODELOBJECT,// モデル
		BULLET,		// 弾
		SHADOW,		// 影
		IMPACT,		// 衝撃波
		CHARACTOR,	// プレイヤー
		BILLBOARD,	// ビルボード
		EFFECT,		// エフェクト
		UI,			// 2DUI
		PAUSE,		// ポーズ
		PRIORITY_MAX // 最大数
	};

	CObject(int nPriority = NONE);
	virtual ~CObject();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static CObject* GetTop(int nPriority) { return m_pTop[nPriority]; }
	static int GetNumAll(void) { return m_nNumAll; }

	void SetObjType(TYPE type) { m_Type = type; }

	TYPE GetObjType(void) { return m_Type; }
	CObject* GetNext(void) { return m_pNext; }

protected:
	void Release(void);			// 解放処理
private:

	static int m_nNumAll;		// 総数管理
	int m_nID;					// 自分自身のID
	TYPE m_Type;				// オブジェクトの種類変数
	int m_nPriority;			// 優先順位番号

	static CObject* m_pTop[PRIORITY_MAX];	// 先頭オブジェクト
	static CObject* m_pCur[PRIORITY_MAX];	// 最後尾オブジェクト

	CObject* m_pNext;		// 次のオブジェクトのポインタ
	CObject* m_pPrev;		// 前のオブジェクトのポインタ

	bool m_isDeath;			// 死亡フラグ
};