//=========================================================
//
// メイン関数 [ main.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include <crtdbg.h>
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "imguimanager.h"
#include "imgui.h"
#include "renderer.h"
#include "editmanager.h"
#include "edit.h"

//*********************************************************
// ウィンドウプロシージャを定義
//*********************************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=========================================================
// メインエントリーポイント関数
//=========================================================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE /*hInstancePrev*/, _In_ LPSTR /*lpCmdLine*/, _In_ int nCmdShow)
{
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// メモリリーク検知用のフラグ
#endif // _DEBUG

	// マネージャーのインスタンスを生成
	CManager* pManager = NULL;

	// FPSカウント
	int nCountFPS = 0;

	DWORD dwCurrentTime;			// 現在時刻
	DWORD dwExecLastTime;			// 終了時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				 // WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						 // ウインドウのスタイル
		WindowProc,						 // ウインドウプロシージャ
		0,								 // 0にする
		0,								 // 0にする
		hInstance,						 // インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),  // タスクバーアイコン
		LoadCursor(NULL,IDC_ARROW),		// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),		// クライアント領域の背景色
		NULL,							// メニューバー
		CLASS_NAME,						// ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),	// ファイルアイコン
	};

	HWND hWnd;	// ウインドウハンドル(識別子)
	MSG msg;	// メッセージを格納する変数

	// 画面サイズの構造
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx(0,			// 拡張ウインドウスタイル
		CLASS_NAME,						// ウインドウクラスの名前
		WINDOW_NAME,					// ウインドウの名前
		WS_OVERLAPPEDWINDOW,			// ウインドウスタイル
		CW_USEDEFAULT,					// ウインドウの左上のX座標
		CW_USEDEFAULT,					// ウインドウの左上のY座標
		(rect.right - rect.left),       // ウインドウ幅
		(rect.bottom - rect.top),       // ウインドウの高さ
		NULL,
		NULL,
		hInstance,						// インスタンスハンドル
		NULL);							// ウインドウ作成データ

	// メモリ確保
	pManager = new CManager;

	// マネージャーの初期化処理
	pManager->Init(hInstance, hWnd, TRUE);

	// guiマネージャー生成
	CImguiManager* pImguiManager = new CImguiManager;
	
	// レンダラー取得
	CRenderer* pRenderer = CManager::GetRenderer();

	// ポインタを格納する
	pRenderer->SetGuiManager(pImguiManager, hWnd);

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// 初期化
	dwExecLastTime = timeGetTime();		// 現在時刻を保存

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);	// ウインドウの表示状態の設定
	UpdateWindow(hWnd);			// クライアント領域の更新

	// ファイルドロップを許可
	DragAcceptFiles(hWnd, TRUE);

	// 初期化
	DWORD dwFrameCount = 0;					// フレームカウント
	DWORD dwFPSLastTime = timeGetTime();	// 最後にFPSを計測した時刻

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			// Windowsの処理
			if (msg.message == WM_QUIT)
			{
				// WM_QUITメッセージを受け取ったらループ抜ける
				break;
			}
			else
			{
				// メッセージ設定
				TranslateMessage(&msg);	// 仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);	// ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{// DirectXの処理

			dwCurrentTime = timeGetTime();		// 現在時刻の取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過

				nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime); 	// FPSを計測

				dwFPSLastTime = dwCurrentTime;	// 保存

				dwFrameCount = 0;				// 0に戻す

				CRenderer::GetFps(nCountFPS);	// FPS取得
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60/1経過

				// 処理開始の時刻
				dwExecLastTime = dwCurrentTime;	

				// フレームカウントを加算
				dwFrameCount++;

				// リセットが必要かどうか
				if (pRenderer->NeedReset())
				{
					// リセットデバイス
					pRenderer->ResetDevice();
				}

				// フレーム開始
				pImguiManager->NewFrame();

				// マネージャー更新処理
				pManager->Update();

				// フレーム終了
				ImGui::EndFrame();

				// マネージャー描画処理
				pManager->Draw();
			}
		}
	}

	// nullptrじゃなかったら
	if (pManager != nullptr)
	{
		// マネージャー終了処理
		pManager->Uninit();

		// ポインタの破棄
		delete pManager;

		// nullptrにする
		pManager = nullptr;
	}

	// nullptrじゃなかったら
	if (pImguiManager != nullptr)
	{
		// 終了処理
		pImguiManager->Uninit();

		// 破棄
		delete pImguiManager;

		// null初期化
		pImguiManager = nullptr;
	}

	// 分解能を戻す
	timeEndPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// メッセージを返す
	return(int)msg.wParam;
}

// ImGuiのウィンドウプロシージャを取得
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//=========================================================
// ウインドウプロシージャ
//=========================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// ImGuiのウィンドウプロシージャを使用する
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	// ウインドウの領域
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }; 

	// メッセージハンドルを返す変数
	int nID;

	// レンダラーの取得
	CRenderer* pRenderer = CManager::GetRenderer();

	// メインプロセス変数
	CMainProc pProc = {};

	switch (uMsg)
	{
	case WM_DESTROY:// ウインドウ破棄メッセージ

		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_DROPFILES:
	{
		HDROP hDrop = (HDROP)wParam;
		UINT nFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);

		if (nFiles > 0)
		{
			// ファイルパス
			char szPath[MAX_PATH] = { NULL };

			if (DragQueryFile(hDrop, 0, szPath, MAX_PATH) > 0)
			{
				std::string path = szPath;

				// 拡張子チェック
				std::string ext = path.substr(path.find_last_of('.') + 1);
				for (auto& c : ext) c = (char)tolower(c);

				if (ext == "json")
				{
					// 確認メッセージ
					int result = MessageBox(
						hWnd,
						"ファイルを開きますか?",
						path.c_str(),
						MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2
					);

					if (result == IDYES)
					{
						// CEditManagerを取得
						CEditManager* pEdit = CEdit::GetPoint();

						if (pEdit)
						{
							pEdit->ReloadFromPath(path); // ファイル読み込み
						}
					}
				}
				else
				{
					MessageBox(hWnd, "JSONファイルをドロップしてください。", "ファイル形式エラー", MB_OK | MB_ICONEXCLAMATION);
				}
			}
		}

		DragFinish(hDrop);
		return 0;
	}

	case WM_SIZE:

		if (wParam == SIZE_MINIMIZED)
			return 0;

		pRenderer->SetSize(LOWORD(lParam), HIWORD(lParam));

		return 0;

	case WM_MOUSEWHEEL: // マウスのホイール判定
	{
		// ローカル変数
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		// ホイール情報
		CManager::GetCamera()->WheelMouse(zDelta);
	}
		break;

	case WM_KEYDOWN:				// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:				// [ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);// ウインドウ破棄メッセージ
			}
			else
			{
				return 0;			// 返す
			}
			break;

		case VK_F11:
			pProc.ToggleFullScreen(hWnd);	// F11でフルスクリーン
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);// 既定の処理を繰り返す
}

//=========================================================
// コンストラクタ
//=========================================================
CMainProc::CMainProc() : m_Windowrect{}
{

}
//=========================================================
// デストラクタ
//=========================================================
CMainProc::~CMainProc()
{
	// 無し
}
//=========================================================
// ウィンドウフルスクリーン
//=========================================================
void CMainProc::ToggleFullScreen(HWND hWnd)
{
	// 切り替えフラグを宣言
	static bool isFullscreen = false;

	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (isFullscreen)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, m_Windowrect.left, m_Windowrect.top,
			m_Windowrect.right - m_Windowrect.left, m_Windowrect.bottom - m_Windowrect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &m_Windowrect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	// フラグを変更
	isFullscreen = !isFullscreen;
}