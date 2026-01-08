//====================================
//
// プレイヤー処理 [ player.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _PLAYER_H_ // このマクロ定義がされてなかったら
#define _PLAYER_H_ // 2重インクルード防止のマクロ定義

//**********************
// インクルードファイル
//**********************
#include "object.h"
#include "model.h"
#include "motion.h"

//**********************
// 前方宣言
//**********************
class CInputKeyboard;
class CJoyPad;

//*************************
// プレイヤークラスを定義
//*************************
class CPlayer : public CObject
{
public:
	//***********************************
	// プレイヤーが行うモーション列挙型
	//***********************************
	enum PLAYERMOTION
	{
		PLAYERMOTION_NEUTRAL,		// ニュートラル
		PLAYERMOTION_MOVE,			// 移動
		PLAYERMOTION_ACTION,		// アクション
		PLAYERMOTION_JUMP,			// ジャンプ
		PLAYERMOTION_LANDING,		// 着地
		PLAYERMOTION_JUMPATTACK,	// ジャンプ攻撃
		PLAYERMOTION_DAMAGE,		// ダメージ
		PLAYERMOTION_AVOID,			// 回避
		PLAYERMOTION_MAX
	};

	// コンストラクタ・デストラクタ
	CPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CPlayer();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// セッター
	void SetRotDest(D3DXVECTOR3 rotDest) { m_rotDest = rotDest; }
	void SetJump(bool isJump) { m_isJump = isJump; }
	void SetLanding(bool isLanding) { m_isLanding = isLanding; }
	void AddMove(void) { m_pos += m_move; }
	void SetValue(float fValue) { m_fValue = fValue; }
	void JumpMove(void) { m_move.y = m_fValue; }
	void SetIsDamege(bool isFlags) { m_isDecHp = isFlags; }
	void SetActive(bool isActiveFlags) { m_isControll = isActiveFlags; }
	void SetLeft(bool isLeft) { m_isLeft = isLeft; }
	void SetRight(bool isRight) { m_isRight = isRight; }
	void SetAvoid(bool isAvoid) { m_isAvoid = isAvoid; }

	// ゲッター
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// 現在の座標を取得
	D3DXVECTOR3 GetOldPos(void) { return m_posOld; }	// 過去の座標を取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// 現在の角度を取得
	D3DXVECTOR3 GetRotDest(void) { return m_rotDest; }	// 目的角を取得

	CMotion* GetMotion(void) { return m_pMotion; }
	CModel** GetModel(void) { return m_apModel; }

	int GetType(void) { return m_type; }

	float GetCoolTime(void) { return m_fAvoidTime; }

	// フラグメント関数
	bool IsJumping() { return m_isJump; } 	// ジャンプ状態の確認
	bool isLanding(void) { return m_isJump; }
	bool GetLanding(void) { return m_isLanding; }
	bool GetIsDamege(void) { return m_isDecHp; }
	bool GetIsActive(void) { return m_isControll; }
	bool GetLeft(void) {return m_isLeft; }
	bool GetRight(void) { return m_isRight; }

	// 静的メンバ関数
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife,const char* pFilename); // 生成処理
	static bool IsDeath(void) { return m_isDeath; }

private:
	static inline constexpr int MAX_MODEL = 19; // プレイヤーで使うモデルの数

	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rotDest;  // 目的角

	D3DXVECTOR3 m_pos;		// 階層構造設定座標
	D3DXVECTOR3 m_posOld;	// 過去の座標情報
	D3DXVECTOR3 m_rot;		// 角度
	D3DXVECTOR3 m_Scal;
	D3DXVECTOR3 m_fSize;
	D3DXMATRIX m_mtxworld;	// ワールドマトリックス

	CModel* m_apModel[MAX_MODEL]; // 使うモデルのポインタ
	CMotion* m_pMotion;		// モーションのポインタ

	int m_type;				// モーションの種類変数
	int m_nNumAll;			// モデル総数
	int m_State;			// 状態管理カウンター

	bool m_isLanding;		// 着地判定
	bool m_isJump;			// ジャンプ判定
	bool m_isMoving;		// 移動キー判定
	bool m_isAttack;		// 攻撃判定
	bool m_isShadow;
	bool m_isStateSynchro;
	bool m_isConectPad;
	bool m_isGuard;
	bool m_isDecHp;
	bool m_isControll;
	bool m_isCollisionBlock;
	bool m_isJumpAttack;
	bool m_isLeft;
	bool m_isRight;
	bool m_isAvoid;

	float m_fAngle;			// 現在の角度
	float m_fValue;
	float m_fAvoidTime;
	const char* m_pFilename; // 読み込むファイル名

	static bool m_isDeath; // 静的
};

#endif