//=========================================================
//
// マネージャー処理 [ manager.h ]
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
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "scene.h"
#include "fade.h"

//*********************************************************
// マネージャークラスを定義
//*********************************************************
class CManager
{
public:

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer* GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; }
	static CJoyPad* GetJoyPad(void) { return m_pJoyPad; }
	static CInputMouse* GetMouse(void) { return m_pInputMouse; }
	static CTexture* GetTexture(void) { return m_pTexture; }
	static CCamera* GetCamera(void) { return m_pCamera; }
	static CLight* GetLight(void) { return m_pLight; }
	static CScene* Getscene(void) { return m_pScene; }
	static CFade* GetFade(void) { return m_pFade; }

	static void SetScene(CScene * pNewScene);
	static CScene::MODE GetScene(void);

private:
	static CRenderer* m_pRenderer;			// レンダラークラスのポインタ
	static CInputKeyboard* m_pInputKeyboard;// キーボードクラスのポインタ
	static CJoyPad* m_pJoyPad;				// ジョイパッドクラスのポインタ
	static CInputMouse* m_pInputMouse;		// マウスクラスのポインタ
	static CTexture* m_pTexture;			// テクスチャクラスのポインタ
	static CCamera* m_pCamera;				// カメラクラスのポインタ
	static CLight* m_pLight;				// ライトクラスのポインタ
	static CScene* m_pScene;				// シーン管理クラスのポインタ
	static CFade* m_pFade;					// フェードクラスのポインタ
};