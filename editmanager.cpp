//================================================
//
// エディターシーン管理処理 [ editmanager.cpp ]
// Author : Asuma Nishio
//
//================================================

//**************************
// インクルードファイル宣言
//**************************
#include "editmanager.h"
#include "manager.h"
#include "debugproc.h"
#include "camera.h"
#include "mapmanager.h"
#include "mapobject.h"
#include "template.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "json.hpp"
#include "manager.h"
#include "meshfield.h"

//*******************************
// 名前空間
//*******************************
namespace EDITINFO
{
	constexpr float MOVESPEED = 5.0f;
	constexpr float ROTVALUE = 0.03f;
	constexpr float VALUEHEIGHT = 5.0f;
	constexpr float VALUESIZE = 0.1f;
};

//*******************************
// jsonファイル形式を使用する
//*******************************
using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

//=============================
// コンストラクタ
//=============================
CEditManager::CEditManager()
{
	m_pMapManager = nullptr;
	m_nTypeIdx = NULL;
	m_size = CLEARSIZE;
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_nSelectIdx = -1;
	m_FileList.clear();
	m_pSelect = nullptr;
	m_pMeshField = nullptr;
	m_nColType = NULL;
	m_nMassSet = NULL;
	m_isStatic = false;
	m_isMoveVecY = false;
}
//=============================
// デストラクタ
//=============================
CEditManager::~CEditManager()
{
	// 無し
}
//=============================
// 初期化処理
//=============================
HRESULT CEditManager::Init(void)
{
	// モデルリストを読み込み
	CModelList::Load();

	// マネージャー生成
	m_pMapManager = CMapManager::Craete();
	
	// 生成
	m_pMeshField = CMeshField::Create(D3DXVECTOR3(2500.0f,0.0f,0.0f),6000.0f,1000.0f,1,1);

	// 初期化結果を返す
	return S_OK;
}
//=============================
// 終了処理
//=============================
void CEditManager::Uninit(void)
{
	// 破棄
	CModelList::UnLoad();

	// クリア処理
	m_FileList.clear();
}
//=============================
// 更新処理
//=============================
void CEditManager::Update(void)
{
	// Gui作成
	CImguiManager* pImGui = CManager::GetRenderer()->GetImGuiManager();
	if (pImGui == nullptr) return;

	// 開始
	pImGui->StartPos(ImVec2(1000.0f, 0.0f));
	pImGui->StartSize(ImVec2(300.0f, 600.0f));
	pImGui->Start("Inspector", CImguiManager::NONE);

	// モデル選択
	auto modelList = CModelList::GetInfo();

	// TabBarの開始
	if (ImGui::BeginTabBar("MainTab"))
	{
		//=============================
		// 現在オブジェクトの編集
		//=============================
		if (ImGui::BeginTabItem("Current Edit"))
		{
			// 空白を追加
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			// 配置したオブジェクトの表示
			ImGui::Text("AllCreateBlock = %d", m_pMapManager->GetSize());

			// 同じ行にボタンを置く
			ImGui::SameLine();
			
			// フラグ
			ImGui::Checkbox("isLock Object", &m_isStatic);

			// 赤いボタン
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.6f)); 

			// 配置ボタン
			if (ImGui::Button("Create") || CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN))
			{
				// オブジェクトを生成
				CMapObject* pMapObj = CMapObject::Create(m_pos, m_rot, m_size,m_nTypeIdx,m_isStatic);

				// 配列追加
				if (pMapObj) m_pMapManager->PushBack(pMapObj);
			}

			// カラー戻す
			ImGui::PopStyleColor(1);

			// 空白を追加
			ImGui::Dummy(ImVec2(0.0f, 15.0f));

			// 位置編集
			ImGui::DragFloat3("Pos", (float*)&m_pos, 2.0f);

			// 角度
			ImGui::DragFloat3("Rot", (float*)&m_rot, 0.03f);

			// 角度の正規化
			if (m_rot.y > D3DX_PI)
			{// D3DX_PIより大きくなったら
				m_rot.y -= D3DX_PI * 2.0f;
			}
			// 角度の正規化
			if (m_rot.y < -D3DX_PI)
			{// D3DX_PIより小さくなったら
				m_rot.y += D3DX_PI * 2.0f;
			}

			// 角度の正規化
			if (m_rot.x > D3DX_PI)
			{// D3DX_PIより大きくなったら
				m_rot.x -= D3DX_PI * 2.0f;
			}
			// 角度の正規化
			if (m_rot.x < -D3DX_PI)
			{// D3DX_PIより小さくなったら
				m_rot.x += D3DX_PI * 2.0f;
			}

			// 角度の正規化
			if (m_rot.z > D3DX_PI)
			{// D3DX_PIより大きくなったら
				m_rot.z -= D3DX_PI * 2.0f;
			}
			// 角度の正規化
			if (m_rot.z < -D3DX_PI)
			{// D3DX_PIより小さくなったら
				m_rot.z += D3DX_PI * 2.0f;
			}

			// 拡大率
			ImGui::DragFloat3("Scale", (float*)&m_size, 0.1f, 0.5f, 2.0f);

			// 空白を追加
			ImGui::Dummy(ImVec2(0.0f, 20.0f));

			if (!modelList.empty())
			{
				std::vector<const char*> modelNames;
				for (auto& m : modelList) modelNames.push_back(m.FileName);

				ImGui::Combo("FileName", &m_nTypeIdx, modelNames.data(), (int)modelNames.size());
			}

			// 空白を追加
			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			ImGui::EndTabItem();
		}

		//=========================
		// 保存 読み込み
		//=========================
		if (ImGui::BeginTabItem("Save & Load"))
		{
			// 赤いボタン
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.6f)); // 赤色

			if (ImGui::Button("Save"))
				SaveName(GetActiveWindow());

			// カラーリセット
			ImGui::PopStyleColor(1);

			// 緑ボタン
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 0.6f)); // 赤色

			if (ImGui::Button("Reload"))
				Reload();

			// カラーリセット
			ImGui::PopStyleColor(1);

			// タブアイテム終了
			ImGui::EndTabItem();
		}

		//=========================
		// システム
		//=========================
		if (ImGui::BeginTabItem("System"))
		{
			// 現在の背景色を取得
			D3DXCOLOR col = CManager::GetRenderer()->GetBackColor();

			float color[4] = { col.r, col.g, col.b, col.a };

			// カラー調整
			if (ImGui::ColorEdit4("Background Color", color))
			{
				// 変更があった場合に反映
				CManager::GetRenderer()->SetBackColor(color[0], color[1], color[2], color[3]);
			}

			// テキスト表示
			ImGui::Text("FPS = %d", CManager::GetRenderer()->ReturnFps());

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
			if (ImGui::Button("WireFrameOn")) CManager::GetRenderer()->OnWireFrame();

			// 設定数分戻す
			ImGui::PopStyleColor(1);

			// カラー変更
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.6f, 1.0f)); // 通常

			if (ImGui::Button("WireFrameOff"))  CManager::GetRenderer()->OffWireFrame();

			// 設定数分戻す
			ImGui::PopStyleColor(1);

			// 空白を追加
			ImGui::Dummy(ImVec2(0.0f, 15.0f));
			
			// 座標取得
			D3DXVECTOR3 NowPos = m_pMeshField->GetPos();

			if (ImGui::DragFloat3("MeshFieldPos", (float*)&NowPos, 10.0f))
			{
				// 値変更時に反映
				m_pMeshField->SetPos(NowPos);
			}

			// 空白を追加
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			// X横幅取得
			float fRadiusX = m_pMeshField->GetRadiusX();

			if (ImGui::DragFloat("Radius X", (float*)&fRadiusX, 5.0f))
			{
				// 値変更時に反映
				m_pMeshField->SetRaiusX(fRadiusX);
			}

			// 空白を追加
			ImGui::Dummy(ImVec2(0.0f, 5.0f));

			// Z幅取得
			float fRadiusZ = m_pMeshField->GetRadiusZ();

			if (ImGui::DragFloat("Radius Z", (float*)&fRadiusZ, 5.0f))
			{
				// 値変更時に反映
				m_pMeshField->SetRaiusZ(fRadiusZ);
			}

			// タブアイテム終了
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	// レイピッキング
	ColisionRay();

	// 終了
	ImGui::End();

//===============================
// ショートカットキー
//===============================

	// カメラ
	CCamera* pCamera = CManager::GetCamera();

	// 移動処理
	if (CManager::GetInputKeyboard()->GetPress(DIK_A))
	{// Aキーを押した

		m_pos.x -= sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * EDITINFO::MOVESPEED;
		m_pos.z -= cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * EDITINFO::MOVESPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_D))
	{// Dキーを押した

		m_pos.x += sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * EDITINFO::MOVESPEED;
		m_pos.z += cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * EDITINFO::MOVESPEED;
	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_W))
	{// Wキーを押した

		m_pos.x += sinf(pCamera->GetRot().y) * EDITINFO::MOVESPEED;
		m_pos.z += cosf(pCamera->GetRot().y) * EDITINFO::MOVESPEED;

	}
	else if (CManager::GetInputKeyboard()->GetPress(DIK_S))
	{// Sキーを押した

		m_pos.x -= sinf(pCamera->GetRot().y) * EDITINFO::MOVESPEED;
		m_pos.z -= cosf(pCamera->GetRot().y) * EDITINFO::MOVESPEED;
	}

	// 種類変更
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RIGHT) && m_nTypeIdx < CModelList::GetSize())
	{// 右矢印キーを押した
		m_nTypeIdx++;
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_LEFT) && m_nTypeIdx > NULL)
	{// 左矢印キーを押した
		m_nTypeIdx--;
	}
}
//=============================
// 描画処理
//=============================
void CEditManager::Draw(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans,mtxScale;

	// 現在のマテリアルを保存
	D3DMATERIAL9 matDef;

	// マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxworld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxScale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxRot);
	
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// 現在のマトリックスの取得
	pDevice->GetMaterial(&matDef);

	// モデル情報
	CModelList::MODELINFO Info = CModelList::GetInfo(m_nTypeIdx);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)Info.pBuffMat->GetBufferPointer();

	// マテリアル数だけ回す
	for (int nCntMat = 0; nCntMat < (int)Info.dwNumMat; nCntMat++)
	{
		// カラー
		D3DXMATERIAL Col = pMat[nCntMat];

		// 透明度
		Col.MatD3D.Diffuse.a = 0.5f;

		// マテリアル設定
		pDevice->SetMaterial(&Col.MatD3D);

		// インデックスに応じて変更する
		if (Info.pTexture[nCntMat] != -1)
		{
			// テクスチャ取得
			CTexture* pTexture = CManager::GetTexture();

			// テクスチャセット
			pDevice->SetTexture(0, pTexture->GetAddress(Info.pTexture[nCntMat]));
		}
		else
		{
			pDevice->SetTexture(0, NULL); // テクスチャなし
		}

		
		// モデル( パーツ )の描画
		Info.pMesh->DrawSubset(nCntMat);
	}

	// マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//=============================
// 保存処理
//=============================
void CEditManager::Save(const char * pSaveName)
{
	// JSONオブジェクト
	ordered_json j;

	// 総数
	j["AllObject"] = m_pMapManager->GetSize();

	// サイズ分で回す
	for (int nCnt = 0; nCnt < m_pMapManager->GetSize(); nCnt++)
	{
		// オブジェクトの情報取得
		CMapObject* pObj = m_pMapManager->GetInfo(nCnt);

		// 無かったらスキップ
		if (!pObj) continue;

		// 座標,角度を取得
		D3DXVECTOR3 pos = pObj->GetPos();
		D3DXVECTOR3 rot = pObj->GetRot();
		D3DXVECTOR3 scale = pObj->GetSize();
		int nType = m_pMapManager->GetInfo(nCnt)->GetIdx();
		bool isMove = m_pMapManager->GetInfo(nCnt)->getIsStatic();

		// モデルファイルパスを取得
		CModelList::MODELINFO info = CModelList::GetInfo(pObj->GetIdx());

		// JSONに追加
		ordered_json obj;

		// 配置インデックス
		obj["idx"] = nCnt;

		// 各種オブジェクト情報
		obj["pos"] = { Round(pos.x), Round(pos.y), Round(pos.z) };
		obj["rot"] = { Round(rot.x), Round(rot.y), Round(rot.z) };
		obj["scale"] = { Round(scale.x), Round(scale.y), Round(scale.z) };
		obj["Flags"] = isMove;

		// モデルのXファイル
		obj["filepath"] = info.FileName;

		// 追加
		j["SetObjects"].push_back(obj);
	}

	// ファイル書き出し
	std::ofstream ofs(pSaveName);

	if (!ofs.is_open())
	{
		MessageBox(NULL, "保存ファイルを開けませんでした", "エラー", MB_OK);
		return;
	}

	// 出力
	ofs << std::setw(3) << j.dump(3) << std::endl;
	
	// 終了
	ofs.close();
}
//=============================
// 名前をつけて保存
//=============================
void CEditManager::SaveName(HWND hWnd)
{
	// クリア
	m_FileList.clear();

	// 新規のパス
	std::string newPath = ShowSaveFileDialog(hWnd);

	if (!newPath.empty())
	{
		// jsonに保存処理
		Save(newPath.c_str());

		// リストに追加
		FileEntry entry;
		entry.fullPath = newPath;
		entry.fileName = std::filesystem::path(newPath).filename().string();
		m_FileList.push_back(entry);

		// 選択中を更新
		m_nSavePassIdx = static_cast<int>(m_FileList.size()) - 1;
	}
}
//=============================
// ダイアログ保存
//=============================
std::string CEditManager::ShowSaveFileDialog(HWND hWnd)
{
	char szFile[MAX_PATH] = { NULL };

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);

	// JSONだけに限定
	ofn.lpstrFilter = "JSON Files\0*.json\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrDefExt = "json";  // デフォルト拡張子をjsonに
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		std::string path(ofn.lpstrFile);

		// .json を付ける
		if (path.find('.') == std::string::npos)
		{
			path += ".json";
		}

		// パスを返す
		return path;
	}

	return "";
}
//=============================
// ダイアログ再読み込み処理
//=============================
std::string CEditManager::ShowReloadFileDialog(HWND hWnd)
{
	char szFile[MAX_PATH] = { NULL };

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);

	// JSON再読み込み
	ofn.lpstrFilter = "JSON Files\0*.json\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrDefExt = "json";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		return std::string(ofn.lpstrFile);
	}

	return "";
}
//=============================
// 変換処理
//=============================
float CEditManager::Round(float fValue)
{
	return std::round(fValue * 100.0f) / 100.0f;
}
//=============================
// ファイル再読み込み処理
//=============================
void CEditManager::Reload(void)
{
	// ロードされたパス
	std::string path = ShowReloadFileDialog(GetActiveWindow());
	if (path.empty()) return;

	// 該当パスを開く
	std::ifstream ifs(path);
	if (!ifs.is_open()) return;

	// jsonクラス
	ordered_json j;
	ifs >> j;
	ifs.close();

	// 一度クリア
	m_pMapManager->ClearObj();

	// JSON から復元
	for (auto& obj : j["SetObjects"])
	{
		// 座標,角度,サイズ,パスを設定
		auto type = obj["idx"];
		auto pos = obj["pos"];
		auto rot = obj["rot"];
		auto scale = obj["scale"];
		auto isstatic = obj["Flags"];
		std::string file = obj["filepath"];

		int modelIdx = -1;
		auto modelList = CModelList::GetInfo();

		for (int nCnt = 0; nCnt < (int)modelList.size(); nCnt++)
		{
			if (file == modelList[nCnt].FileName)
			{
				modelIdx = nCnt;
				break;
			}
		}

		if (modelIdx == -1) continue;

		// 新しく生成
		CMapObject* pObj = CMapObject::Create
		(
			D3DXVECTOR3(pos[0], pos[1], pos[2]),
			D3DXVECTOR3(rot[0], rot[1], rot[2]),
			D3DXVECTOR3(scale[0], scale[1], scale[2]),
			modelIdx,
			isstatic
		);

		if (pObj)
		{
			// 配列に追加
			m_pMapManager->PushBack(pObj);
		}
	}	
}
//=============================
// レイピッキング処理
//=============================
void CEditManager::ColisionRay(void)
{
	// 入れ物
	float fDistance = 0.0f;

	// マネージャー取得
	CImguiManager* pImGui = CManager::GetRenderer()->GetImGuiManager();

	//============================
	// マウスクリック時に選択確定
	//============================
	if (CManager::GetMouse()->GetTriggerDown(CInputMouse::MOUSE_LEFT))
	{
		// ヒットフラグ
		bool isNoHit = false;
		CMapObject* closest = nullptr;
		float closestDist = FLT_MAX;

		for (auto& ite : m_pMapManager->GetList())
		{
			if (ite->CollisionMouse(&fDistance))
			{
				if (fDistance < closestDist)
				{
					closestDist = fDistance;
					closest = ite;
					break;
				}
				//// 当たったらポインタセット
				//m_pSelect = ite;
				//isNoHit = true;
				//break;
			}
		}

		bool isWindow = pImGui->GetActive();

		if (closest && !isWindow)
		{
			m_pSelect = closest;
		}
		else if (!closest && !isWindow)
		{
			m_pSelect = nullptr;
		}
	}

	//=========================
	// 既存オブジェクト編集
	//=========================
	pImGui->StartPos(ImVec2(0.0f, 0.0f));
	pImGui->StartSize(ImVec2(320.0f, 230.0f));
	pImGui->Start("SelectModelList", CImguiManager::NONE);

	// 選択されたら
	if (m_pSelect != nullptr)
	{
		// 縦軸移動フラグ情報
		if (ImGui::Button("VecMoveY_Set"))
		{
			m_isMoveVecY = m_isMoveVecY ? false : true;
		}


		// 固定化されているかどうか
		bool isStatic = m_pSelect->getIsStatic();

		if (ImGui::Button("Unlock"))
		{
			m_pSelect->SetIsStatic(false); // 動かせる
		}
		else
		{
			// Editable のときは固定ボタンを表示
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.6f, 0.2f, 0.7f));

			if (ImGui::Button("Lock Object!"))
			{
				m_pSelect->SetIsStatic(true);	// 固定化
			}

			ImGui::PopStyleColor();

			// 分ける
			ImGui::Separator();

			// 現在の値を取得
			D3DXVECTOR3 pos = m_pSelect->GetPos();
			D3DXVECTOR3 rot = m_pSelect->GetRot();
			D3DXVECTOR3 size = m_pSelect->GetSize();
			int idx = m_pSelect->GetIdx();

			// 編集できる場合のみUIを有効化
			ImGui::BeginDisabled(isStatic);

			// 空白
			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			// 位置
			if (ImGui::DragFloat3("Change Pos", (float*)&pos, 2.0f))
				m_pSelect->SetPos(pos);

			// 角度
			if (ImGui::DragFloat3("Change Rot", (float*)&rot, 0.03f))

			// 角度の正規化
			if (rot.y > D3DX_PI)
			{// D3DX_PIより大きくなったら
				rot.y -= D3DX_PI * 2.0f;
			}
			// 角度の正規化
			if (rot.y < -D3DX_PI)
			{// D3DX_PIより小さくなったら
				rot.y += D3DX_PI * 2.0f;
			}

			// 角度の正規化
			if (rot.x > D3DX_PI)
			{// D3DX_PIより大きくなったら
				rot.x -= D3DX_PI * 2.0f;
			}
			// 角度の正規化
			if (rot.x < -D3DX_PI)
			{// D3DX_PIより小さくなったら
				rot.x += D3DX_PI * 2.0f;
			}

			// 角度の正規化
			if (rot.z > D3DX_PI)
			{// D3DX_PIより大きくなったら
				rot.z -= D3DX_PI * 2.0f;
			}
			// 角度の正規化
			if (rot.z < -D3DX_PI)
			{// D3DX_PIより小さくなったら
				rot.z += D3DX_PI * 2.0f;
			}

			m_pSelect->SetRot(rot);

			// サイズ
			if (ImGui::DragFloat3("Change Scale", (float*)&size, 0.1f, 0.5f, 2.0f))
				m_pSelect->SetSize(size);

			// モデル選択
			auto modelList = CModelList::GetInfo();

			// モデル変更用コンボ
			if (!modelList.empty())
			{
				std::vector<const char*> modelNames;
				for (auto& m : modelList) modelNames.push_back(m.FileName);

				if (ImGui::Combo("Change XFile", &idx, modelNames.data(), (int)modelNames.size()))
				{
					// 種類変更
					m_pSelect->SetIdx(idx);
				}
			}

			// 空白を追加
			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			ImGui::EndDisabled();

			// 空白を追加
			ImGui::Dummy(ImVec2(0.0f, 10.0f));

			// 削除ボタン
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.6f));

			if (ImGui::Button("Delete Selected") && !m_pSelect->getIsStatic())
			{
				// 選択オブジェクト消去
				m_pMapManager->DeleteObj(m_pSelect);

				// 削除したので選択解除
				m_pSelect = nullptr;
			}

			ImGui::PopStyleColor();
		}
	}
	else
	{
		ImGui::Text("No Object Selected!!");
	}

	//===============================
	// マウス操作で移動
	//===============================
	if (m_pSelect != nullptr && !m_pSelect->getIsStatic())
	{
		// もしImGuiがマウスを使っているならスキップ
		if (ImGui::GetIO().WantCaptureMouse)
		{
			ImGui::End();
			return;
		}

		// マウスに追従移動
		D3DXVECTOR2 mouseMove = CManager::GetMouse()->GetMov();
		CCamera* pCamera = CManager::GetCamera();

		D3DXMATRIX matView = pCamera->GetViewMtx();

		// カメラ
		D3DXVECTOR3 camRight(matView._11, matView._21, matView._31);
		D3DXVECTOR3 camForward(matView._13, matView._23, matView._33);

		// 移動方向をセット
		camRight.y = 0.0f;
		camForward.y = 0.0f;
		D3DXVec3Normalize(&camRight, &camRight);
		D3DXVec3Normalize(&camForward, &camForward);

		// 座標取得
		D3DXVECTOR3 pos = m_pSelect->GetPos();

		// 移動速度
		const float moveSpeed = 2.0f;

		// 一定条件下で縦軸(Y軸の移動)に移動する
		if (CManager::GetMouse()->GetPress(CInputMouse::MOUSE_LEFT) && m_isMoveVecY)
		{
			pos.y += -mouseMove.y * moveSpeed;
		}

		// 左ドラッグでXZ平面移動
		if (CManager::GetMouse()->GetPress(CInputMouse::MOUSE_LEFT) && !m_isMoveVecY)
		{
			pos += camRight * (mouseMove.x * moveSpeed);
			pos += -camForward * (mouseMove.y * moveSpeed);
		}

		// 座標セット
		m_pSelect->SetPos(pos);
		
	}

	// 終了
	ImGui::End();
}
//============================
// D&D
//============================
void CEditManager::ReloadFromPath(const std::string& path)
{
	if (path.empty()) return;

	std::ifstream ifs(path);
	if (!ifs.is_open())
	{
		MessageBox(NULL, "ファイルを開けませんでした", "エラー", MB_OK);
		return;
	}

	// jsonクラス
	json j;
	ifs >> j;
	ifs.close();

	// 一度クリア
	m_pMapManager->ClearObj();

	// JSON から復元
	for (auto& obj : j["SetObjects"])
	{
		// 座標,角度,サイズ,パスを設定
		auto idx = obj["idx"];
		auto pos = obj["pos"];
		auto rot = obj["rot"];
		auto scale = obj["scale"];
		auto isstatic = obj["Flags"];

		std::string file = obj["filepath"];

		int modelIdx = -1;
		auto modelList = CModelList::GetInfo();

		for (int nCnt = 0; nCnt < (int)modelList.size(); nCnt++)
		{
			if (file == modelList[nCnt].FileName)
			{
				modelIdx = nCnt;
				break;
			}
		}

		if (modelIdx == -1) continue;

		// 新しく生成
		CMapObject* pObj = CMapObject::Create
		(
			D3DXVECTOR3(pos[0], pos[1], pos[2]),
			D3DXVECTOR3(rot[0], rot[1], rot[2]),
			D3DXVECTOR3(scale[0], scale[1], scale[2]),
			modelIdx,
			isstatic
		);

		if (pObj)
		{
			// 配列に追加
			m_pMapManager->PushBack(pObj);
		}
	}
}