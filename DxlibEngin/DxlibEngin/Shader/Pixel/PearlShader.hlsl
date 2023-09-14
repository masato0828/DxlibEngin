#include "PixelShader3DHeader.hlsli"

//エントリーポイント
PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
    //HSVで基準色を設定
    float4 result = float4(0.3f, 0.15f, 1.f, 1.0f);

    //色相を法線と視線の傾きの差分を使って変動させる
    result.r += dot(-normalize(input.viewPos), normalize(input.viewNorm));

    //色相を0～1の間で循環させる
    result.r = fmod(result.r, 1.f);

    //HSVからRGBに変換
    result.rgb = HSVtoRGB(result.rgb);

    //0～1の間に丸める
    return saturate(result);
}
