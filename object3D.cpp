//=========================================================
//
// 3Dオブジェクト処理 [ object3D.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object3D.h"
#include "texture.h"
#include "manager.h"

//=========================================================
// コンストラクタ
//=========================================================
CObject3D::CObject3D(int nPriority) : CObject(nPriority),
m_pVtxBuff(nullptr),
m_mtxWorld{},
m_rot(VECTOR3_NULL),
m_pos(VECTOR3_NULL),
m_col(COLOR_WHITE),
m_nIdxTexture(-1),
m_fHeight(NULL),
m_fWidth(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CObject3D::~CObject3D()
{
	// 無し
}
//=========================================================
// 生成処理
//=========================================================
CObject3D* CObject3D::Create(D3DXVECTOR3 pos)
{
	// インスタンス生成
	CObject3D* pObj3D = new CObject3D;
	if (pObj3D == nullptr) return nullptr;

	// オブジェクト設定
	pObj3D->SetPos(pos);
	pObj3D->SetTexture();

	// 初期化処理失敗時
	if (FAILED(pObj3D->Init()))
	{
		// nullptr代入
		pObj3D = nullptr;
	}

	// 生成されたポインタを返す
	return pObj3D;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CObject3D::Init(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, 0.0f, m_fWidth);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fWidth);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fWidth);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, -m_fWidth);

	// 各頂点の法線(ベクトル)の設定
	pVtx[0].nor = 
	pVtx[1].nor = 
	pVtx[2].nor = 
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = 
	pVtx[1].col = 
	pVtx[2].col = 
	pVtx[3].col = COLOR_WHITE;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CObject3D::Uninit(void)
{
	// バッファ解放
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CObject3D::Update(void)
{
	// 頂点情報のポインタ
	VERTEX_3D* pVtx = nullptr;

	// 頂点バッファをロックし,頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth, 5.0f, m_fWidth);
	pVtx[1].pos = D3DXVECTOR3(m_fWidth, 5.0f, m_fWidth);
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 5.0f, -m_fWidth);
	pVtx[3].pos = D3DXVECTOR3(m_fWidth, 5.0f, -m_fWidth);

	// 各頂点の法線(ベクトル)の設定
	pVtx[0].nor =
	pVtx[1].nor =
	pVtx[2].nor =
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 頂点カラーの設定
	pVtx[0].col = 
	pVtx[1].col = 
	pVtx[2].col = 
	pVtx[3].col = m_col;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// アンロック
	m_pVtxBuff->Unlock();
}
//=========================================================
// 描画処理
//=========================================================
void CObject3D::Draw(void)
{
	// デバイスポインタを宣言
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y,m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// テクスチャNULLにする
	pDevice->SetTexture(0, NULL);
}
//=========================================================
// テクスチャ割り当て
//=========================================================
void CObject3D::SetTexture(void)
{
	// テクスチャポインタ取得
	CTexture* pTexture = CManager::GetTexture();

	// 割り当て
	m_nIdxTexture = pTexture->Register("data\\TEXTURE\\number.png");
}