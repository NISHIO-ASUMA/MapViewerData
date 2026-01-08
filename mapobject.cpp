//================================================
//
// マップに出すオブジェクト処理 [ mapobject.cpp ]
// Author: Asuma Nishio
//
//================================================

//******************************
// インクルードファイル
//******************************
#include "mapobject.h"
#include "manager.h"
#include "modellist.h"
#include "editmanager.h"

//=========================
// コンストラクタ
//=========================
CMapObject::CMapObject(int nPriority) : CObject(nPriority)
{
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_mtxworld = {};
	m_nTypeIdx = NULL;
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_nColorType = NULL;
	MassType  = NULL;
	m_isStatic = false;
}
//=========================
// デストラクタ
//=========================
CMapObject::~CMapObject()
{
	// 無し
}
//=========================
// 生成
//=========================
CMapObject* CMapObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 scale, int nIdx,bool isflags)
{
	// インスタンス生成
	CMapObject* pMapObject = new CMapObject;

	// nullなら
	if (pMapObject == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pMapObject->Init()))
	{
		return nullptr;
	}

	// セット
	pMapObject->m_pos = pos;
	pMapObject->m_rot = rot;
	pMapObject->m_size = scale;
	pMapObject->m_nTypeIdx = nIdx;
	pMapObject->m_isStatic = isflags;

	// 生成されたポインタを返す
	return pMapObject;
}
//=========================
// 初期化
//=========================
HRESULT CMapObject::Init(void)
{
	// 値の初期化
	m_pos = VECTOR3_NULL;
	m_rot = VECTOR3_NULL;
	m_mtxworld = {};
	m_nTypeIdx = -1;
	m_nColorType = NULL;
	MassType = NULL;
	m_isStatic = false;

	return S_OK;
}
//=========================
// 終了
//=========================
void CMapObject::Uninit(void)
{
	// 自身の破棄
	CObject::Release();
}
//=========================
// 更新
//=========================
void CMapObject::Update(void)
{
	// 無し
}
//=========================
// 描画
//=========================
void CMapObject::Draw(void)
{
	// 番号がnull
	if (m_nTypeIdx == -1) return;

	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// リスト取得 ( 種類 )
	CModelList::MODELINFO Info = CModelList::GetInfo(m_nTypeIdx);

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

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

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)Info.pBuffMat->GetBufferPointer();

	// マテリアル数だけ回す
	for (int nCntMat = 0; nCntMat < (int)Info.dwNumMat; nCntMat++)
	{
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
			pDevice->SetTexture(0, nullptr); // テクスチャなし
		}

		// マテリアル設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// モデル(パーツ)の描画
		Info.pMesh->DrawSubset(nCntMat);
	}

	// マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}
//===========================
// マウスヒット
//===========================
bool CMapObject::CollisionMouse(float* Distance)
{
	// 距離初期化
	*Distance = FLT_MAX;

	// デバイス
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	if (!pDevice) return false;

	// マウス取得
	D3DXVECTOR3 rayOriginW, rayDirW;
	CManager::GetMouse()->GetRay(&rayOriginW, &rayDirW);

	// ヒット
	BOOL hit = false;

	// モデル情報
	CModelList::MODELINFO info = CModelList::GetInfo(m_nTypeIdx);
	if (!info.pMesh) return false;

	// ワールド行列を取得
	D3DXMATRIX world = m_mtxworld;

	// レイをローカル座標系へ変換
	D3DXMATRIX invWorld;
	D3DXMatrixInverse(&invWorld, nullptr, &world);

	D3DXVECTOR3 rayOriginL, rayDirL;
	D3DXVec3TransformCoord(&rayOriginL, &rayOriginW, &invWorld);
	D3DXVec3TransformNormal(&rayDirL, &rayDirW, &invWorld);

	// ヒットかどうか
	DWORD faceIndex;

	// メッシュとの当たり判定
	D3DXIntersect(info.pMesh,
		&rayOriginL,
		&rayDirL,
		&hit,
		&faceIndex,
		NULL,
		NULL,
		Distance,
		nullptr,
		nullptr);

	// 当たったら
	if (hit)
	{
		return true;
	}

	return false;
}

