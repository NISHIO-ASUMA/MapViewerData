//=====================================
//
// テクスチャ処理 [ texture.h ]
// Author: Asuma Nishio
//
//=====================================

#ifndef _TEXTURE_H_ // このマクロ定義がされてなかったら
#define _TEXTURE_H_ // 2重インクルード防止のマクロ定義

//*****************************
// インクルードファイル
//*****************************
#include <string>

//*****************************
// テクスチャ管理クラスを定義
//*****************************
class CTexture
{
public:
	// コンストラクタ・デストラクタ
	CTexture();
	~CTexture();

	// メンバ関数
	HRESULT Load(void);
	void UnLoad(void);
	int Register(const char* pFileName);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);
	HRESULT LoadFile(void);

private:

	static inline constexpr int NUM_TEXTURE = 256;	// 最大テクスチャ数

	std::string TEXINFO[NUM_TEXTURE]; // テクスチャ配列
	LPDIRECT3DTEXTURE9 m_apTexture[NUM_TEXTURE];	// テクスチャポインタ
	static int m_nNumAll;							// テクスチャカウント変数
};

#endif