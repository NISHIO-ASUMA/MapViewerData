//=========================================================
//
// マップマネージャー処理 [ mapmanager.h ]
// Author : Asuma Nishio
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
#include <vector>

//*********************************************************
// 前方宣言
//*********************************************************
class CMapObject;

//*********************************************************
// マップオブジェクトを管理するクラスを定義
//*********************************************************
class CMapManager : CObject
{
public:

	CMapManager(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CMapManager();


	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void PushBack(CMapObject* pMapOjbect) { m_MapObj.push_back(pMapOjbect); };
	void ClearObj(void);
	void DeleteObj(CMapObject* pMapObject);

	CMapObject* GetInfo(const int nIdx) { return m_MapObj[nIdx]; }
	std::vector<CMapObject*>& GetList(void) { return m_MapObj; }

	int GetSize() const { return static_cast<int>(m_MapObj.size()); }

	static CMapManager* Craete(void);

private:

	std::vector<CMapObject*>m_MapObj;	// マップオブジェクトの動的配列
};