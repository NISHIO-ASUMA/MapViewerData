//=========================================
//
// プレイヤー処理 [ player.cpp ]
// Author: Asuma Nishio
//
//=========================================

//**********************
// インクルードファイル
//**********************
#include "player.h"
#include "manager.h"
#include "texture.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"

//**********************
// 名前空間
//**********************
namespace PLAYERINFO
{
	constexpr float MOVE = 0.015f;		 // 1フレームの移動量
	constexpr float JUMPVALUE = 18.3f;	 // ジャンプ量
	constexpr float GRAVITY = 1.5f;		 // 重力値
	constexpr float HITRADIUS = 25.0f;	 // 当たり判定の半径
	constexpr float AVOID_COOLTIME = 5.0f;	// 回避のクールタイム
	constexpr float RADIUS = 550.0f;		// 半径
	constexpr int   NUMBER_MAIN = 0;	 // メイン操作プレイヤー番号
	constexpr int   NUMBER_SUB = 1;		 // 分身操作プレイヤー番号
	constexpr int   KeyRepeatCount = 15; // キーのリピート最大カウント
};

//**********************
// 静的メンバ変数宣言
//**********************
bool CPlayer::m_isDeath = false;  // 死亡フラグ

//===============================
// オーバーロードコンストラクタ
//===============================
CPlayer::CPlayer(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_move = VECTOR3_NULL;
	m_Scal = VECTOR3_NULL;
	m_State = NULL;
	m_rotDest = VECTOR3_NULL;
	m_nNumAll = NULL;
	m_type = NULL;
	m_posOld = VECTOR3_NULL;
	m_pFilename = {};
	m_fAngle = NULL;
	m_fAvoidTime = NULL;

	// モデルのポインタのクリア
	for (int nCnt = 0; nCnt < MAX_MODEL; nCnt++)
	{
		m_apModel[nCnt] = nullptr;
	}

	// クラスポインタ
	m_pMotion = nullptr;

	// フラグメント
	m_isLanding = false;
	m_isJump = false;
	m_isAttack = false;
	m_isMoving = false;
	m_isShadow = false;
	m_isStateSynchro = false;
	m_isConectPad = false;
	m_isGuard = false;
	m_isDecHp = false;
	m_isControll = false;
	m_isCollisionBlock = false;
	m_isJumpAttack = false;
	m_isLeft = false;
	m_isRight = false;
	m_isAvoid = false;
}
//===============================
// デストラクタ
//===============================
CPlayer::~CPlayer()
{
	// 無し
}
//===============================
// プレイヤー生成処理
//===============================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,int nLife,const char* pFilename)
{
	// プレイヤーのインスタンス生成
	CPlayer* pPlayer = new CPlayer;

	// インスタンスがnullptrだったら
	if (pPlayer == nullptr) return nullptr;

	// オブジェクト設定
	pPlayer->m_pos = pos;
	pPlayer->m_rot = rot;
	pPlayer->m_pFilename = pFilename;

	// プレイヤー初期化処理
	if (FAILED(pPlayer->Init()))
	{
		// nullptrを返す
		return nullptr;
	}

	// プレイヤーのポインタを返す
	return pPlayer;
}
//===============================
// プレイヤー初期化処理
//===============================
HRESULT CPlayer::Init(void)
{
	// オブジェクトの種類をセット
	SetObjType(TYPE_PLAYER);

	// モデル総数を代入
	m_nNumAll = MAX_MODEL;

	// モーション種類数を代入
	m_type = PLAYERMOTION_MAX;

	// 角度初期化
	m_fAngle = NULL;

	// スケールサイズを設定
	m_Scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// フラグを設定
	m_isDeath = false;
	m_isJump = false;
	m_isLanding = false;
	m_isMoving = false;

	// モーションの読み込み
	m_pMotion = CMotion::Load(m_pFilename, MAX_MODEL, m_apModel,PLAYERMOTION_MAX);

	// モーション数を設定
	m_pMotion->SetMotionNum(PLAYERMOTION_MAX);

	m_fSize = { 50.0f,50.0f,50.0f };

	// クールタイム
	m_fAvoidTime = PLAYERINFO::AVOID_COOLTIME;

	// 結果を返す
	return S_OK;
}
//===============================
// プレイヤー終了処理
//===============================
void CPlayer::Uninit(void)
{
	// モデル数分の破棄
	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		// nullptrチェック
		if (m_apModel[nCnt] != nullptr)
		{
			// 終了処理
			m_apModel[nCnt]->Uninit();

			// ポインタの破棄
			delete m_apModel[nCnt];

			// nullptrにする
			m_apModel[nCnt] = nullptr;
		}
	}

	// nullptrチェック
	if (m_pMotion != nullptr)
	{
		// ポインタの破棄
		delete m_pMotion;

		// nullptrにする
		m_pMotion = nullptr;
	}

	// オブジェクト自身の破棄
	CObject::Release();
}
//============================================================
// プレイヤー更新処理
//============================================================
void CPlayer::Update(void)
{
	// 入力デバイスのポインタ取得
	CInputKeyboard* pInput = nullptr;
	CJoyPad* pJoyPad = nullptr;

	pInput = CManager::GetInputKeyboard();
	pJoyPad = CManager::GetJoyPad();

	// カメラ取得
	CCamera* pCamera = CManager::GetCamera();

	// 移動処理
	if (pInput->GetPress(DIK_A))
	{// Aキーを押した

		if (pInput->GetPress(DIK_W))
		{// Wキーを押した

			m_move.x += sinf(pCamera->GetRot().y - D3DX_PI * 0.25f) * 1.5f;
			m_move.z += cosf(pCamera->GetRot().y - D3DX_PI * 0.25f) * 1.5f;
			m_rotDest.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);

			// モーションセット
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);

		}
		else if (pInput->GetPress(DIK_S))
		{// Sキーを押した
			m_move.x -= sinf(pCamera->GetRot().y + D3DX_PI * 0.25f) * 1.5f;
			m_move.z -= cosf(pCamera->GetRot().y + D3DX_PI * 0.25f) * 1.5f;
			m_rotDest.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);

			// モーションセット
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}
		else
		{// Aキーのみを押した
			m_move.x -= sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * 1.5f;
			m_move.z -= cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * 1.5f;
			m_rotDest.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);

			// モーションセット
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}

		// 角度の正規化
		if (m_rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			m_rot.y -= D3DX_PI * 2.0f;
		}

	}
	else if (pInput->GetPress(DIK_D))
	{// Dキーを押した

		if (pInput->GetPress(DIK_W))
		{// Wキーを押した
			m_move.x += sinf(pCamera->GetRot().y + D3DX_PI * 0.25f) * 1.5f;
			m_move.z += cosf(pCamera->GetRot().y + D3DX_PI * 0.25f) * 1.5f;
			m_rotDest.y = pCamera->GetRot().y - (D3DX_PI * 0.75f);

			// モーションセット
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);

		}

		else if (pInput->GetPress(DIK_S))
		{// Sキーを押した
			m_move.x -= sinf(pCamera->GetRot().y - D3DX_PI * 0.25f) * 1.5f;
			m_move.z -= cosf(pCamera->GetRot().y - D3DX_PI * 0.25f) * 1.5f;
			m_rotDest.y = pCamera->GetRot().y - (D3DX_PI * 0.25f);

			// モーションセット
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}

		else
		{// Dキーのみ押した
			m_move.x += sinf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * 1.5f;
			m_move.z += cosf(pCamera->GetRot().y + (D3DX_PI * 0.5f)) * 1.5f;
			m_rotDest.y = pCamera->GetRot().y - (D3DX_PI * 0.5f);

			// モーションセット
			m_pMotion->SetMotion(PLAYERMOTION_MOVE);
		}

		// 角度の正規化
		if (m_rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			m_rot.y -= D3DX_PI * 2.0f;
		}

	}
	else if (pInput->GetPress(DIK_W))
	{// Wキーを押した

			// モーションセット
		m_pMotion->SetMotion(PLAYERMOTION_MOVE);

		m_move.x += sinf(pCamera->GetRot().y) * 1.5f;
		m_move.z += cosf(pCamera->GetRot().y) * 1.5f;
		m_rotDest.y = pCamera->GetRot().y - (D3DX_PI);

		// 角度を正規化
		if (m_rot.y < -D3DX_PI)
		{// D3DX_PIより小さくなったら
			m_rot.y += D3DX_PI * 2.0f;
		}

		if (!m_isJump) 	m_pMotion->SetMotion(PLAYERMOTION_MOVE, false, 0, false); // 移動モーションに変更

	}
	else if (pInput->GetPress(DIK_S))
	{// Sキーを押した
		// モーションセット
		m_pMotion->SetMotion(PLAYERMOTION_MOVE);


		m_move.x -= sinf(pCamera->GetRot().y) * 1.5f;
		m_move.z -= cosf(pCamera->GetRot().y) * 1.5f;
		m_rotDest.y = pCamera->GetRot().y;

		// 角度の正規化
		if (m_rot.y > D3DX_PI)
		{// D3DX_PIより大きくなったら
			m_rot.y -= D3DX_PI * 2.0f;
		}


	}
	else
	{
		// 移動モーション時,ニュートラルモーションに変更
		if (m_pMotion->GetMotionType() == PLAYERMOTION_MOVE) m_pMotion->SetMotion(PLAYERMOTION_NEUTRAL, false, 0, false);
	}

	// 重力
	m_move.y -= 0.7f;

	// 前回の位置を保存
	m_posOld = m_pos;

	// 移動更新
	m_pos += m_move;

	// 移動量の減衰
	m_move.x += (0.0f -m_move.x) * 0.5f;
	m_move.z += (0.0f -m_move.z) * 0.5f;

	// 現在のy座標が0.0f以下の時
	if (m_pos.y <= 0.0f)
	{
		// y座標を地面の座標に設定
		m_pos.y = 0.0f;

		// 着地判定を有効化
		m_isLanding = true;

		// 高さへの移動量を0.0fに設定
		m_move.y = 0.0f;
	}
	
	// モーションの全体更新
	m_pMotion->Update(m_apModel, MAX_MODEL); 
}
//===============================
// プレイヤー描画処理
//===============================
void CPlayer::Draw(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxworld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxScal, m_Scal.x, m_Scal.y, m_Scal.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxScal);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// 全モデルパーツの描画
	for (int nCnt = 0; nCnt < m_nNumAll; nCnt++)
	{
		m_apModel[nCnt]->Draw();
	}
}