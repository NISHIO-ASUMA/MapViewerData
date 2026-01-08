//=====================================
//
// カメラ処理 [ camera.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _CAMERA_H_ // このマクロ定義がされてなかったら
#define _CAMERA_H_ // 2重インクルード防止のマクロ定義

//**********************
// 前方宣言
//**********************
class CInputMouse;
class CInputKeyboard;

//**********************
// カメラクラスを定義
//**********************
class CCamera
{
public:
	//**********************
	// カメラモード列挙型
	//**********************
	enum MODE
	{
		MODE_NONE,
		MODE_PLAYER,
		MODE_LOCKON,
		MODE_MOUSE,
		MODE_SHAKE,
		MODE_EVENT,
		MODE_ANIM,
		MODE_MAIN,
		MODE_SUB,
		MODE_MAX
	
	};

	// コンストラクタ・デストラクタ
	CCamera();
	~CCamera();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);
	void MouseView(CInputMouse* pMouse, CInputKeyboard* pInput);
	void WheelMouse(int nDelta);

	// セッター
	void SetCameraMode(int nMode) { m_pCamera.nMode = nMode; }

	// ゲッター
	D3DXVECTOR3 GetRot(void) { return m_pCamera.rot; }
	D3DXVECTOR3 GetPos(void) { return m_pCamera.posV; }
	D3DXVECTOR3 GetPosR(void) { return m_pCamera.posR; }
	D3DXMATRIX GetMtxProjection(void) { return m_pCamera.mtxprojection; }
	D3DXMATRIX GetViewMtx(void) { return m_pCamera.mtxView; }
	int GetMode(void) { return m_pCamera.nMode; }
	float GetDistance(void) { return m_pCamera.fDistance; }

private:

	//*************************
	// カメラ構造体を定義
	//*************************
	struct Camera
	{
		D3DXMATRIX mtxprojection;	// プロジェクションマトリックス
		D3DXMATRIX mtxView;			// ビューマトリックス
		D3DXVECTOR3 posV, posR;		// 視点,注視点座標
		D3DXVECTOR3 rot;			// 角度
		D3DXVECTOR3 vecU;			// 法線ベクトル
		D3DXVECTOR3 posRDest;		// 目的座標
		float fDistance;			// カメラの距離
		int nMode;					// カメラのモード
		int nUseKey;				// アニメーション使用時に読み取るキー数
		int nCntAnim;				// カウント
	};

	Camera m_pCamera;		// カメラ構造体変数
	D3DXVECTOR3 m_Zoom;		// ズーム
};

#endif