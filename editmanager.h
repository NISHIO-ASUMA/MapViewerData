//============================================
//
// エディターシーン管理処理 [ editmanager.h ]
// Author : Asuma Nishio
//
//============================================

#ifndef _EDITMANAGER_H_ // このマクロ定義がされてなかったら
#define _EDITMANAGER_H_ // 2重インクルード防止のマクロ定義

//**********************************
// インクルードファイル
//**********************************
#include "modellist.h"
#include <string>

//**********************************
// 前方宣言
//**********************************
class CMapManager;
class CMapObject;
class CMeshField;

//**********************************
// エディター管理クラスを定義
//**********************************
class CEditManager
{
public:
	// コンストラクタ・デストラクタ
	CEditManager();
	~CEditManager();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Save(const char* pSaveName);
	void SaveName(HWND hWnd);
	void Reload(void);
	void ColisionRay(void);
	void ReloadFromPath(const std::string& path);

	// セッター
	D3DXVECTOR3 SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 SetSize(D3DXVECTOR3 size) { m_size = size; }
	// ゲッター
	D3DXVECTOR3 GetPos() const { return m_pos; }
	D3DXVECTOR3 GetRot() const { return m_rot; }
	D3DXVECTOR3 GetSIze() const { return m_size; }

private:

	// 格納関数
	std::string ShowSaveFileDialog(HWND hWnd); // ダイアログ保存関数
	std::string ShowReloadFileDialog(HWND hWnd); // ダイアログ再読み込み関数
	float Round(float fValue);

	//*******************************
	// ファイルパスを格納するリスト
	//*******************************
	struct FileEntry
	{
		std::string fullPath;	// フルパス
		std::string fileName;	// 表示用のファイル名
	};

	std::vector<FileEntry> m_FileList;	// 動的に管理

	D3DXVECTOR3 m_pos; // 座標
	D3DXVECTOR3 m_rot; // 角度
	D3DXVECTOR3 m_size;// サイズ
	D3DXMATRIX m_mtxworld; // ワールドマトリックス

	CMapManager* m_pMapManager; // マネージャーポインタ
	CMapObject* m_pSelect;		// 選択中オブジェクト
	CMeshField* m_pMeshField;	// メッシュフィールド

	int m_nTypeIdx;		// 種類インデックス
	int m_nSelectIndex; // 選択中オブジェクトのインデックス
	int m_nSavePassIdx; // セーブファイルパスインデックス
	int m_nSelectIdx;	// 選択中のオブジェクト番号

	int m_nColType;		// 識別色の種類
	int m_nMassSet;		// 重さの識別番号
	bool m_isStatic;	// 動かせるかどうか
};

#endif
