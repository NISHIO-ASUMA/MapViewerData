//=====================================
//
// テクスチャ処理 [ texture.cpp ]
// Author: Asuma Nishio
//
//=====================================

//**********************
// インクルードファイル
//**********************
#include "texture.h"
#include "manager.h"
#include <sstream>
#include <iostream>
#include <fstream>

//**********************
// 静的メンバ変数
//**********************
int CTexture::m_nNumAll = NULL;	// 総数管理

//===============================
// コンストラクタ
//===============================
CTexture::CTexture()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		m_apTexture[nCnt] = nullptr;
	}
}
//===============================
// デストラクタ
//===============================
CTexture::~CTexture()
{
	// 無し
}
//===============================
// テクスチャ読み込み
//===============================
HRESULT CTexture::Load(void)
{
	// 読み込み処理
	LoadFile();

#if 0
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// すべてのテクスチャ分回す
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		// 文字列があったら
		if (!TEXINFO[nCnt].empty())
		{
			// テクスチャ生成
			HRESULT hr = D3DXCreateTextureFromFile(
				pDevice,
				TEXINFO[nCnt].c_str(),
				&m_apTexture[nCnt]);

			if (FAILED(hr))
			{
				// nullにする
				m_apTexture[nCnt] = nullptr;
			}
		}
		else
		{
			// nullにする
			m_apTexture[nCnt] = nullptr;
		}
	}
#endif
	// 結果を返す
	return S_OK;
}
//===============================
// テクスチャ破棄
//===============================
void CTexture::UnLoad(void)
{
	// すべてのテクスチャの破棄
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		// 配列の連結クリア
		TEXINFO[nCnt].clear();

		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}
}
//===============================
// テクスチャ番号を登録する
//===============================
int CTexture::Register(const char* pFileName)
{
	// テクスチャがあったら
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		if (!TEXINFO[nCnt].empty() && TEXINFO[nCnt] == pFileName)
		{
			return nCnt; // インデックスを返す
		}
	}

	// 無かったら新規作成
	if (m_nNumAll < NUM_TEXTURE)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		// 制作時
		if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, pFileName, &m_apTexture[m_nNumAll])))
		{
			// ファイル名を登録
			TEXINFO[m_nNumAll] = pFileName;

			// 総数を返す
			return m_nNumAll++;
		}
	}

	// テクスチャがない場合
	return -1;
}
//===============================
// テクスチャ番号取得
//===============================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	// 例外処理
	if (nIdx < 0 || nIdx >= NUM_TEXTURE) return nullptr;

	// テクスチャ番号を取得
	return m_apTexture[nIdx];
}
//===============================
// テクスチャ読み込み処理
//===============================
HRESULT CTexture::LoadFile(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 開くファイルを設定
	std::ifstream openfile ("data\\Loader\\TextureFile.txt");

	if (!openfile)
	{
		MessageBox(NULL, "ファイルオープン失敗", "data\\Loader\\TextureFile.txt", MB_OK);

		return E_FAIL;
	}

	// ラインを設定
	std::string Line;

	// 全部クリア
	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{
		TEXINFO[nCnt].clear();

		if (m_apTexture[nCnt] != nullptr)
		{
			m_apTexture[nCnt]->Release();
			m_apTexture[nCnt] = nullptr;
		}
	}

	while (std::getline(openfile, Line))
	{
		// 空行やコメントは無視
		if (Line.empty() || Line[0] == '/' || Line[0] == '#')
			continue;

		// "TEXNAME" を探す
		size_t pos = Line.find("TEXNAME");

		if (pos != std::string::npos)
		{
			size_t eqPos = Line.find('=');

			if (eqPos != std::string::npos)
			{
				// "=" の後のファイル名を取得
				std::string texFile = Line.substr(eqPos + 1);

				// 前後の空白削除
				texFile.erase(0, texFile.find_first_not_of(" \t"));
				texFile.erase(texFile.find_last_not_of(" \t\r\n") + 1);

				if (!texFile.empty() && m_nNumAll < NUM_TEXTURE)
				{
					// ファイルパスを設定
					std::string fullPath = texFile;

					// 読み込み
					if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice, fullPath.c_str(), &m_apTexture[m_nNumAll])))
					{
						TEXINFO[m_nNumAll] = texFile; // ファイル名だけを保持
						m_nNumAll++;
					}
				}
			}
		}
	}

	// ファイルを閉じる
	openfile.close();

	// 結果を返す
	return S_OK;
}