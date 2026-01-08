//=====================================
//
// レンダリング処理 [ renderer.cpp ]
// Author: Asuma Nishio
//
//=====================================

//****************************
// インクルードファイル宣言
//****************************
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "manager.h"
#include "input.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

//******************************************
// 静的メンバ変数宣言
//******************************************
CDebugproc* CRenderer::m_pDebug = nullptr;	// デバッグプロセスへのポインタ
int CRenderer::m_fps = 0;					// FPSカウント

//===============================
// コンストラクタ
//===============================
CRenderer::CRenderer()
{
	// 値のクリア
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_pZBuffMT = nullptr;
	m_pRenderDef = nullptr;
	m_pZBuffDef = nullptr;
	m_pImguiManager = nullptr;
	m_isbuller = false;

	for (int nCnt = 0; nCnt < NUM_FEEDBACKPOLYGON; nCnt++)
	{
		m_apRenderMT[nCnt] = nullptr;
		m_apTextureMT[nCnt] = nullptr;
	}

	m_pVtxMT = nullptr;
	m_nBullerTime = NULL;
	m_backCol = COLOR_BLACK;
	m_isActive = false;
}
//===============================
// デストラクタ
//===============================
CRenderer::~CRenderer()
{
	// 無し
}
//===============================
// レンダラー初期化処理
//===============================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	// ディスプレイモード
	D3DDISPLAYMODE d3ddm;			

	// Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションのパラメーターを設定
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));//パラメーターの0クリア

	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;					// ゲーム画面サイズ(高さ)
	m_d3dpp.BackBufferFormat = d3ddm.Format;					// バックバッファの形式
	m_d3dpp.BackBufferCount = 1;								// バックバッファの数
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	m_d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				// デプスバッファとして16bitを使用
	m_d3dpp.Windowed = bWindow;									// ウインドウモード
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_pD3DDevice)))
	{
		// Direct3Dデバイスの生成(描画処理はハードウェア,頂点処理をCPUで行う)
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{
			// Direct3Dデバイスの生成(描画処理,頂点処理をCPUで行う)
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージチャートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// デバッグフォント初期化
	m_pDebug = new CDebugproc;
	m_pDebug->Init();

	// メンバ変数
	m_nBullerTime = NULL;

	// 初期化結果を返す
	return S_OK;
}
//===============================
// レンダラー終了処理
//===============================
void CRenderer::Uninit(void)
{
	if (m_pDebug != nullptr)
	{
		// デバッグプロセスの終了
		m_pDebug->Uninit();
		delete m_pDebug;
		m_pDebug = nullptr;
	}

	// 全オブジェクト破棄
	CObject::ReleaseAll();

	// マルチレンダリングターゲット用Zバッファの破棄
	if (m_pZBuffMT != nullptr)
	{
		m_pZBuffMT->Release();
		m_pZBuffMT = nullptr;
	}

	// 頂点バッファの破棄
	if (m_pVtxMT != nullptr)
	{
		m_pVtxMT->Release();
		m_pVtxMT = nullptr;
	}

	// 作成数分破棄
	for (int nCnt = 0; nCnt < NUM_FEEDBACKPOLYGON; nCnt++)
	{
		// テクスチャ破棄
		if (m_apTextureMT[nCnt] != nullptr)
		{
			m_apTextureMT[nCnt]->Release();
			m_apTextureMT[nCnt] = nullptr;
		}

		// レンダー破棄
		if (m_apRenderMT[nCnt] != nullptr)
		{
			m_apRenderMT[nCnt]->Release();
			m_apRenderMT[nCnt] = nullptr;
		}
	}

	// Direct3Dデバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}
//===============================
// レンダラー更新処理
//===============================
void CRenderer::Update(void)
{
	// キーボードのポインタ
	CInputKeyboard* pInput = CManager::GetInputKeyboard();

	// デバッグ情報の更新処理
	m_pDebug->Update();

	// 全オブジェクト更新処理
	CObject::UpdateAll();

	// ワイヤーフレーム設定
	if (pInput->GetTrigger(DIK_F3))
	{
		// ワイヤーフレームON
		OnWireFrame();
	}
	else if (pInput->GetTrigger(DIK_F4))
	{
		// ワイヤーフレームOFF
		OffWireFrame();
	}

	if (m_pImguiManager == nullptr) return;
}
//===============================
// レンダラー描画処理
//===============================
void CRenderer::Draw(void)
{
	// 画面クリア(バックバッファ&Zバッファ&ステンシルバッファのクリア)
	m_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		m_backCol, 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{// 描画成功時

		// 全オブジェクト描画
		CObject::DrawAll();

		// シーン取得
		CScene * pScene = CManager::Getscene();

		// nullじゃなかったら
		if (pScene != nullptr)
		{
			pScene->Draw();
		}

		// フェード描画
		CManager::GetFade()->Draw();

		// gui関数
		m_pImguiManager->EndFrame();

		// 描画終了
		m_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}
//===============================
// レンダーターゲット変更関数
//===============================
void CRenderer::ChangeTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU)
{
	// マトリックス変数
	D3DXMATRIX mtxprojection, mtxview;

	// アスペクト比計算用
	float fAsepct = NULL;

	// レンダリングターゲットを設定
	m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);

	// Zバッファを設定
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	// ビューポートを設定
	m_pD3DDevice->SetViewport(&m_viewportMT);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxview);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxview,
		&posV,
		&posR,
		&vecU);

	// ビューマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_VIEW, &mtxview);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxprojection);

	// アスペクト比の設定
	fAsepct = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&mtxprojection,
		D3DXToRadian(45.0f),
		fAsepct,
		10.0f,
		3000.0f);

	// プロジェクションマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mtxprojection);
}

//===============================
// リセットが必要なら
//===============================
bool CRenderer::NeedReset(void)
{
	// 両方0なら
	if (m_Width != NULL && m_Height != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//===============================
// ブラーの設定
//===============================
void CRenderer::SetBuller(bool isBuller, const int nMaxbullerTime)
{
	// 値を設定
	m_isbuller = isBuller;
	m_nBullerTime = nMaxbullerTime;
}

//===============================
// デバイス生成処理
//===============================
void CRenderer::ResetDevice(void)
{
	// 例外処理
	if (!m_pD3DDevice || m_Height == 0 || m_Width == 0)
	{
		return;
	}

	// デバッグフォントの破棄
	m_pDebug->Uninit();

	m_d3dpp.BackBufferWidth = m_Width;
	m_d3dpp.BackBufferHeight = m_Height;
	m_Height = m_Width = 0;

	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT hr = m_pD3DDevice->Reset(&m_d3dpp);

	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();

	//レンダーステートの再設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの再設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの再設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// 初期化
	m_pDebug->Init();

	// ライトの再設定
	CLight* pLight = CManager::GetLight();
	pLight->Init();
}

//===============================
// デバイス取得処理
//===============================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	// デバイスを返す
	return m_pD3DDevice;
}
//===============================
// FPS表示
//===============================
void CRenderer::GetFps(int nFps)
{
	// 代入
	m_fps = nFps;
}
//===============================
// ワイヤーフレーム起動
//===============================
void CRenderer::OnWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
//===============================
// ワイヤーフレーム終了
//===============================
void CRenderer::OffWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
//===============================
// システムウィンドウ
//===============================
void CRenderer::UpdateGui(void)
{
	// アクティブなら
	if (m_isActive)
	{
		// 座標セット
		m_pImguiManager->StartPos(ImVec2(0.0f, 0.0f));

		// サイズセット
		m_pImguiManager->StartSize(ImVec2(300.0f, 350.0f));

		// 開始
		m_pImguiManager->Start("System_Info", CImguiManager::NONE);

		// カラー調整
		ImGui::ColorEdit4("colorInfo", m_backCol);

		// テキスト表示
		ImGui::Text("FPS = %d", m_fps);

		// カメラ情報表示
		ImGui::Text("CameraPosV = %.2f,%.2f,%.2f", CManager::GetCamera()->GetPos().x, CManager::GetCamera()->GetPos().y, CManager::GetCamera()->GetPos().z);
		ImGui::Text("CameraPosR = %.2f,%.2f,%.2f", CManager::GetCamera()->GetPosR().x, CManager::GetCamera()->GetPosR().y, CManager::GetCamera()->GetPosR().z);
		ImGui::Text("CameraDistance = %.2f", CManager::GetCamera()->GetDistance());

		// ボタンセット
		if (ImGui::Button("CameraReset"))
		{
			// カメラの初期値に設定
			CManager::GetCamera()->Init();
		}

		// カラー変更
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.0f, 0.0f, 1.0f)); // 通常

		// ワイヤーフレーム
		if (ImGui::Button("WireFrameOn"))OnWireFrame();

		// 設定数分戻す
		ImGui::PopStyleColor(1);

		// カラー変更
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.6f, 1.0f)); // 通常

		if (ImGui::Button("WireFrameOff")) OffWireFrame();

		// 設定数分戻す
		ImGui::PopStyleColor(1);

		// 終了
		ImGui::End();
	}
}
