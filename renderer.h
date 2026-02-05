//=========================================================
//
// レンダリング処理 [ renderer.h ]
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
#include "debugproc.h"
#include "imguimanager.h"

//*********************************************************
// レンダラークラスを定義
//*********************************************************
class CRenderer
{
public:

	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void OnWireFrame();
	void OffWireFrame();
	void UpdateGui(void);
	bool NeedReset(void);

	void SetSize(UINT uWidth, UINT uHeight) { m_Width = uWidth, m_Height = uHeight; }
	void ResetDevice(void);
	void SetActiveFlag(bool isActive) { m_isActive = isActive; }

	void SetBackColor(float r, float g, float b, float a);
	void SetGuiManager(CImguiManager* pGui, HWND hwnd);

	LPDIRECT3DDEVICE9 GetDevice(void);
	UINT GetWidth(void) { return m_Width; }
	UINT GetHeight(void) { return m_Height; }
	CImguiManager* GetImGuiManager(void) { return m_pImguiManager; }
	D3DXCOLOR GetBackColor(void) { return m_backCol; }
	int ReturnFps(void) { return m_fps; }

	static CDebugproc* GetDebug(void) { return m_pDebug; }
	static void GetFps(int nFps);

private:

	static inline constexpr int NUM_FEEDBACKPOLYGON = 2; // フィードバック用ポリゴン

	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Direct3Dデバイスへのポインタ
	static CDebugproc* m_pDebug;		// デバッグ
	static int m_fps;					// fps

	D3DPRESENT_PARAMETERS m_d3dpp;
	UINT m_Width;
	UINT m_Height;
	bool m_isDeviceLost;				// デバイスロスト対処
	bool m_isActive;					// アクティブフラグ
	CImguiManager* m_pImguiManager; // クラスポインタ
	D3DXCOLOR m_backCol;			// バックバッファのカラー
};