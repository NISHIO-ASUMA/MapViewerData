//=========================================================
//
// モデルファイルリスト [ modellist.h ]
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
#include <vector>

//*********************************************************
// 読み込みクラスを定義
//*********************************************************
class CModelList
{
public:

	//*************************
	// 情報保存用構造体
	//************************* 
	struct MODELINFO
	{
		LPD3DXMESH pMesh;			// メッシュのポインタ
		LPD3DXBUFFER pBuffMat;		// マテリアルのポインタ
		DWORD dwNumMat;				// マテリアル数
		std::vector<int>pTexture;	// テクスチャの動的ポインタ
		char FileName[MAX_WORD];	// 文字列
	};

	CModelList();
	~CModelList();

	static void Load(void);
	static void UnLoad(void);
	static int GetSize() { return static_cast<int>(m_ModelInfo.size() -1); }
	static MODELINFO GetInfo(const int Indx) { return m_ModelInfo[Indx]; };
	static std::vector <MODELINFO> GetInfo() { return m_ModelInfo; }

private:
	static std::vector<MODELINFO>m_ModelInfo;								// 動的配列
	static constexpr const char* MODELLIST = "data\\Loader\\ModelFile.txt"; // ファイルパス
};