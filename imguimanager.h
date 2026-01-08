//=====================================
//
// Gui管理処理 [ imguimanager.h ]
// Author: Asuma Nishio
//
//=====================================

//*********************
// インクルードガード
//*********************
#pragma once

//****************************
// 使用インクルードファイル
//****************************
#include "imgui.h"

//*********************
// 管理クラスを定義
//*********************
class CImguiManager
{
public:
	//*************************
	// 描画の種類
	//*************************
	enum TYPE
	{
		NONE,
		NOMOVE,
		NORESIZE,
		NOMOVE_AND_RESIZE
	};

	// コンストラクタ・デストラクタ
	CImguiManager();
	~CImguiManager();

	// メンバ関数
	void Init(HWND hwnd, LPDIRECT3DDEVICE9 pDevice);
	void Uninit(void);
	void NewFrame(void);
	void EndFrame(void);
	void Start(const char* pTypename, TYPE Type);
	void StartPos(ImVec2 Pos);
	void StartSize(ImVec2 Size);
	bool GetActive(void);

private:
};

