//=========================================================
//
// Gui管理処理 [ imguimanager.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "imguimanager.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"
#include "imgui_internal.h"

//=========================================================
// コンストラクタ
//=========================================================
CImguiManager::CImguiManager()
{
	// 無し
}
//=========================================================
// デストラクタ
//=========================================================
CImguiManager::~CImguiManager()
{
	// 無し
}
//=========================================================
// 初期化処理
//=========================================================
void CImguiManager::Init(HWND hwnd, LPDIRECT3DDEVICE9 pDevice)
{
	IMGUI_CHECKVERSION(); // ImGui のバージョンチェック
	ImGui::CreateContext(); // ImGui コンテキスト作成
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // キーボード入力有効化
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // ゲームパッド入力有効化
	ImGui::StyleColorsClassic(); // テーマ適用

	// カスタムフォントの追加
	ImFont* myFont = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/meiryo.ttc", 22.0f);

	if (myFont == nullptr)
	{// フォントがNULLなら
		MessageBox(hwnd, "初期化失敗", "例外スロー", MB_OK);
	}

	// フォントをリビルド
	ImGui::GetIO().Fonts->Build();

	// ImGui のバックエンド初期化（Win32 & DirectX9）
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(pDevice);
}
//=========================================================
// 終了処理
//=========================================================
void CImguiManager::Uninit(void)
{
	// Imguiの破棄
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
//=========================================================
// フレーム開始
//=========================================================
void CImguiManager::NewFrame(void)
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}
//=========================================================
// フレーム終了
//=========================================================
void CImguiManager::EndFrame(void)
{
	// 終了関数
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}
//=========================================================
// 描画開始関数
//=========================================================
void CImguiManager::Start(const char* pTypename, TYPE Type)
{
	// 種類に応じて変更
	if (pTypename != NULL)
	{
		switch (Type)
		{
		case NONE:
			// シンプルスタート
			ImGui::Begin(pTypename);
			break;
		case NOMOVE:
			// 移動させない時
			ImGui::Begin(pTypename, NULL, ImGuiWindowFlags_NoMove);
			break;
		case NORESIZE:
			// 拡大させない時
			ImGui::Begin(pTypename, NULL, ImGuiWindowFlags_NoResize);
			break;
		case NOMOVE_AND_RESIZE:
			// 移動も拡大もさせない
			ImGui::Begin(pTypename, NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
			break;
		default:
			break;
		}
	}
}
//=========================================================
// 開始座標関数
//=========================================================
void CImguiManager::StartPos(ImVec2 Pos)
{
	// ウィンドウの出現場所設定
	ImGui::SetNextWindowPos(Pos, ImGuiCond_Once);
}
//=========================================================
// 開始サイズ関数
//=========================================================
void CImguiManager::StartSize(ImVec2 Size)
{
	// ウィンドウの出現場所設定
	ImGui::SetNextWindowSize(Size, ImGuiCond_Once);
}
//=========================================================
// アクティブウィンドウ
//=========================================================
bool CImguiManager::GetActive(void)
{
	// Imguiから取得
	ImGuiIO& io = ImGui::GetIO();

	if (io.WantCaptureMouse && ImGui::IsMouseClicked(0))
	{
		return true;
	}

	return false;
}