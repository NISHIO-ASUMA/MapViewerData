//=====================================
//
// レンダリング処理 [ renderer.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _RENDERER_H_ // このマクロ定義がされてなかったら
#define _RENDERER_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "debugproc.h"
#include "imguimanager.h"

//*************************
// レンダラークラスを定義
//*************************
class CRenderer
{
public:
	// コンストラクタ・デストラクタ
	CRenderer();
	~CRenderer();

	// メンバ関数
	HRESULT Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void OnWireFrame();
	void OffWireFrame();
	void UpdateGui(void);
	void ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	bool NeedReset(void);

	// セッター
	void SetSize(UINT uWidth, UINT uHeight) { m_Width = uWidth, m_Height = uHeight; }
	void SetBuller(bool isBuller, const int nMaxbullerTime);
	void ResetDevice(void);
	void SetActiveFlag(bool isActive) { m_isActive = isActive; }

	void SetBackColor(float r, float g, float b, float a)
	{
		m_backCol.r = r,
		m_backCol.g = g,
		m_backCol.b = b,
		m_backCol.a = a;
	}

	void SetGuiManager(CImguiManager* pGui,HWND hwnd)
	{
		// ポインタ取得
		m_pImguiManager = pGui;

		// 初期化
		m_pImguiManager->Init(hwnd, m_pD3DDevice);
	}

	// ゲッター
	LPDIRECT3DDEVICE9 GetDevice(void);
	UINT GetWidth(void) { return m_Width; }
	UINT GetHeight(void) { return m_Height; }
	LPDIRECT3DTEXTURE9 GetTextureFeedMT(void) { return m_apTextureMT[0]; }
	bool GetBuller(void) { return m_isbuller; }
	CImguiManager* GetImGuiManager(void) { return m_pImguiManager; }
	D3DXCOLOR GetBackColor(void) { return m_backCol; }
	int ReturnFps(void) { return m_fps; }

	// 静的メンバ関数
	static CDebugproc* GetDebug(void) { return m_pDebug; }
	static void GetFps(int nFps);

private:
	static inline constexpr int NUM_FEEDBACKPOLYGON = 2; // フィードバック用ポリゴン

	LPDIRECT3D9 m_pD3D;					// Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Direct3Dデバイスへのポインタ
	static CDebugproc* m_pDebug;		// デバッグ
	static int m_fps;					// fps
	int m_nBullerTime;					// 継続時間
	bool m_isbuller;					// ブラーかどうか

	D3DPRESENT_PARAMETERS m_d3dpp;
	UINT m_Width;
	UINT m_Height;
	bool m_isDeviceLost;				// デバイスロスト対処

	LPDIRECT3DSURFACE9 m_pRenderDef; // マルチターゲットレンダリング用インターフェース保存用
	LPDIRECT3DSURFACE9 m_pZBuffDef;	 // マルチターゲットレンダリング用Zバッファ保存用

	LPDIRECT3DTEXTURE9 m_apTextureMT[NUM_FEEDBACKPOLYGON]; // レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9 m_apRenderMT[NUM_FEEDBACKPOLYGON];	 // テクスチャレンダリング用インターフェース
	LPDIRECT3DSURFACE9 m_pZBuffMT;	 // テクスチャレンダリング用Zバッファ
	D3DVIEWPORT9 m_viewportMT;		 // テクスチャレンダリング用ビューポート

	LPDIRECT3DVERTEXBUFFER9 m_pVtxMT;  // ポリゴン用頂点バッファ

	CImguiManager* m_pImguiManager; // クラスポインタ
	D3DXCOLOR m_backCol;  // バックバッファのカラー
	bool m_isActive;

};

#endif