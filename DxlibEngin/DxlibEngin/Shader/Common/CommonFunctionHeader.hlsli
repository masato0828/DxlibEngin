//インクルードガード
#if !defined(COMMON_FUNCTION_HEADER)
	#define COMMON_FUNCTION_HEADER

	#include "CommonStructuresHeader.hlsli"

	/// @brief 各要素の内、最大値の要素を返す
	/// @param vec 任意のベクトル
	/// @return 最大値の要素
	float VecMax(in float3 vec)
	{
	    return max(vec.r, max(vec.g, vec.b));
	}

	/// @brief 各要素の内、最小値の要素を返す
	/// @param vec 任意のベクトル
	/// @return 最小値の要素
	float VecMin(in float3 vec)
	{
	    return min(vec.r, min(vec.g, vec.b));
	}

	/// @brief RGBからHSVに変換
	/// @param color RGB
	/// @return HSV
	float3 RGBtoHSV(in float3 color)
	{
		const float3 clampedColor = saturate(color);

	    float3 result = float3(0.f, 0.f, 0.f);

	    const float maxValue = VecMax(clampedColor);
	    const float minValue = VecMin(clampedColor);

	    const float delta = maxValue - minValue;

		//H(色相)の設定
		if(delta > 0.0f)
	    {
	        if (maxValue == clampedColor.r)
			{
	            result.r = (clampedColor.g - clampedColor.b) / delta;
	        }
	        else if (maxValue == clampedColor.g)
			{
	            result.r = (clampedColor.b - clampedColor.r) / delta + 2.f;
	        }
			else
			{
	            result.r = (clampedColor.r - clampedColor.g) / delta + 4.f;
	        }

	        result.r /= 6.f;
			if(result.r < 0.0f)
			{
	            result.r += 1.f;
	        }
	    }

		//S(彩度)の設定
		if(maxValue != 0.0f)
	    {
	        result.g = delta / maxValue;
	    }

		//V(明度)の設定
		result.b = maxValue;

	    return result;
	}

	/// @brief HSVからRGBに変換
	/// @param color HSV
	/// @return RGB
	float3 HSVtoRGB(in float3 color)
	{
		float3 clampedColor = saturate(color);
	    float3 result = float3(clampedColor.b, clampedColor.b, clampedColor.b);

		if(clampedColor.g != 0.0f)
		{
	        clampedColor.r *= 6.f;
	        const float f = clampedColor.r - floor(clampedColor.r);
	        const float a = clampedColor.b * (1 - clampedColor.g);
	        const float b = clampedColor.b * (1 - clampedColor.g * f);
	        const float c = clampedColor.b * (1 - clampedColor.g * (1 - f));

			if(clampedColor.r < 1)
			{
	            result.r = clampedColor.b;
	            result.g = c;
	            result.b = a;
	        }
			else if(clampedColor.r < 2)
			{
	            result.r = b;
	            result.g = clampedColor.b;
	            result.b = a;
	        }
	        else if (clampedColor.r < 3)
	        {
	            result.r = a;
	            result.g = clampedColor.b;
	            result.b = c;
	        }
	        else if (clampedColor.r < 4)
	        {
	            result.r = a;
	            result.g = b;
	            result.b = clampedColor.b;
	        }
	        else if (clampedColor.r < 5)
	        {
	            result.r = c;
	            result.g = a;
	            result.b = clampedColor.b;
	        }
	        else
	        {
	            result.r = clampedColor.b;
	            result.g = a;
	            result.b = b;
	        }
	    }

	    return result;
	}

	/// @brief XorShift法での疑似乱数生成
	/// @param seed シード値
	/// @return 疑似乱数
	uint XorShift(in uint seed)
	{
	    uint ret = seed;
	    ret ^= ret << 13;
	    ret ^= ret >> 17;
	    ret ^= ret << 5;
	    return ret;
	}

	/// @brief ベクトル空間変換
	/// @param vec ベクトル
	/// @param tan 元の空間のX方向単位ベクトルを変換先空間に変換したもの
	/// @param bin 元の空間のY方向単位ベクトルを変換先空間に変換したもの
	/// @param norm 元の空間のZ方向単位ベクトルを変換先空間に変換したもの
	/// @return 変換後のベクトル
	float3 ConvertCoordinateSpace(in float3 vec, in float3 tan, in float3 bin, in float3 norm)
	{
		//ビュー座標系に変換する逆行列を取得
	    const float3x3 tangentViewMat = transpose(float3x3(normalize(tan), normalize(bin), normalize(norm)));

		//ベクトルをビュー座標系に変換
	    return normalize(mul(tangentViewMat, vec));
	}

	/// @brief 拡散反射の強さを計算
	/// @param lightRay ライトのレイ
	/// @param normal 法線
	/// @return 
	float CalculateDiffuse(in float3 lightRay, in float3 normal)
	{
	    return saturate(dot(normal, -lightRay));
	}

	/// @brief 鏡面反射の強さを計算
	/// @param lightRay ライトのレイ
	/// @param normal 法線
	/// @param cameraRay レイ
	/// @param power 強さ
	/// @return 
	float CalculateSpecular(in float3 lightRay, in float3 normal, in float3 cameraRay, in float power)
	{
		//最後のstep関数で貫通を防ぐ
		return pow(saturate(dot(normal, -normalize(cameraRay + lightRay))), power) * step(0.0f, dot(normal, -lightRay));	//Blinn-Phong(DXライブラリはこちらを採用)
	    //return pow(saturate(dot(-cameraRay, reflect(lightRay, normal))), power);											//Phong
	}

	/// @brief ポイントライトの処理
	/// @param pos 位置(ビュー)
	/// @param cameraRay レイ(ビュー)
	/// @param normal 法線(ビュー)
	/// @param light ライト構造体
	/// @param power 鏡面反射の強さ
	/// @param totalDiffuse 拡散反射入出力
	/// @param totalSpecular 鏡面反射入出力
	void ProcessPointLight(in float3 pos, in float3 cameraRay, in float3 normal, in Light light, in float power, inout float3 totalDiffuse, inout float3 totalSpecular)
	{
		//ライトから位置へのベクトルを算出
	    const float3 lightRay = normalize(pos - light.position);

	    //ライトとの距離
	    const float lightDistance = distance(pos, light.position);

		//ライトの減衰を計算
	    float lightDecay = 1.0f / (light.attenuation0 + light.attenuation1 * lightDistance + light.attenuation2 * pow(lightDistance, 2.0f));

		//有効距離外なら減衰率を最大
		lightDecay *= step(pow(lightDistance, 2.0f), light.rangePow2);

		//最終的な光量に加算
	    totalDiffuse += (CalculateDiffuse(lightRay, normal) * light.diffuse + light.ambient.rgb) * lightDecay;
	    totalSpecular += CalculateSpecular(lightRay, normal, cameraRay, power) * light.specular * lightDecay;
	}

	/// @brief スポットライトの処理
	/// @param pos 位置(ビュー)
	/// @param ray レイ(ビュー)
	/// @param normal 法線(ビュー)
	/// @param light ライト構造体
	/// @param power 鏡面反射の強さ
	/// @param totalDiffuse 拡散反射入出力
	/// @param totalSpecular 鏡面反射入出力
	void ProcessSpotLight(in float3 pos, in float3 ray, in float3 normal, in Light light, in float power, inout float3 totalDiffuse, inout float3 totalSpecular)
	{
	    //ライトから位置へのベクトルを算出
	    const float3 lightRay = normalize(pos - light.position);
	    
	    //ライトとの距離を算出
	    const float lightDistance = abs(distance(pos, light.position));
	    
	    //距離による減衰率の計算
	    float lightDecay = 1.0f / (light.attenuation0 + light.attenuation1 * lightDistance + light.attenuation2 * pow(lightDistance, 2.0f));
	    
	    //ライトベクトルとライトから頂点へのベクトルの内積を算出
	    const float lightDirectionCosA = dot(lightRay, light.direction);
	    
	    //スポットライトとしての減衰率の計算
	    lightDecay *= saturate(pow(abs(max(lightDirectionCosA - light.spotParam0, 0.0f) * light.spotParam1), light.fallOff));
	    
	    //有効距離外なら減衰率を最大
	    lightDecay *= step(pow(lightDistance, 2.0f), light.rangePow2);
	    
	    //最終的な光量に加算  
		totalDiffuse += (CalculateDiffuse(lightRay, normal) * light.diffuse + light.ambient.rgb) * lightDecay;
	    totalSpecular += CalculateSpecular(lightRay, normal, ray, power) * light.specular * lightDecay;
	}

	/// @brief ディレクショナルライトの処理
	/// @param pos 位置(ビュー)
	/// @param cameraRay レイ(ビュー)
	/// @param normal 法線(ビュー)
	/// @param light ライト構造体
	/// @param power 鏡面反射の強さ
	/// @param totalDiffuse 拡散反射入出力
	/// @param totalSpecular 鏡面反射入出力
	void ProcessDirectionalLight(in float3 pos, in float3 cameraRay, in float3 normal, in Light light, in float power, inout float3 totalDiffuse, inout float3 totalSpecular)
	{
		//最終的な光量に加算
	    totalDiffuse += CalculateDiffuse(light.direction, normal) * light.diffuse  + light.ambient.rgb;
	    totalSpecular += CalculateSpecular(light.direction, normal, cameraRay, power) * light.specular;
	}

	/// @brief 種類を問わないライトの処理
	/// @param pos 位置(ビュー)
	/// @param cameraRay レイ(ビュー)
	/// @param normal 法線(ビュー)
	/// @param light ライト構造体
	/// @param power 鏡面反射の強さ
	/// @param totalDiffuse 拡散反射入出力
	/// @param totalSpecular 鏡面反射入出力
	void ProcessLight(in float3 pos, in float3 cameraRay, in float3 normal, in Light light, in float power, inout float3 totalDiffuse, inout float3 totalSpecular)
	{
	    if (light.type == DX_LIGHTTYPE_POINT)
	    {
	        //ポイントライト
	        ProcessPointLight(pos, cameraRay, normal, light, power, totalDiffuse, totalSpecular);
	    }
	    else if (light.type == DX_LIGHTTYPE_SPOT)
	    {
	        //スポットライト
	        ProcessSpotLight(pos, cameraRay, normal, light, power, totalDiffuse, totalSpecular);
	    }
	    else if (light.type == DX_LIGHTTYPE_DIRECTIONAL)
	    {
	        //ディレクショナルライト
	        ProcessDirectionalLight(pos, cameraRay, normal, light, power, totalDiffuse, totalSpecular);
	    }
	}

#endif
