float4x4 g_mtxWVP; // ワールドビュー射影行列
float4x4 g_mtxWorld; // ワールド行列
float4 g_vOutlineColor = float4(0, 0, 0, 1);
float g_fOutlineSize = 0.02f;

//======================
// 頂点シェーダ入力
//======================
struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
};

//======================
// 頂点シェーダ出力
//======================
struct VS_OUTPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR0;
};

VS_OUTPUT VS_Main(VS_INPUT In)
{
    VS_OUTPUT Out;

    // 法線をワールド空間に変換
    float3 normalWorld = mul(float4(In.Normal, 0.0f), g_mtxWorld).xyz;
    normalWorld = normalize(normalWorld);

    // 頂点をワールド座標へ
    float4 posWorld = mul(In.Pos, g_mtxWorld);

    // ワールド空間で法線方向に拡大
    posWorld.xyz += normalWorld * g_fOutlineSize;

    // 最終変換
    Out.Pos = mul(posWorld, g_mtxWVP);

    // アウトライン色
    Out.Color = g_vOutlineColor;

    return Out;
}

float4 PS_Main(VS_OUTPUT In) : COLOR0
{
    return In.Color;
}

technique Outline
{
    pass P0
    {
        // 裏面を描画（アウトライン用）
        CullMode = CW;

        // 使用するシェーダーを指定
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}
