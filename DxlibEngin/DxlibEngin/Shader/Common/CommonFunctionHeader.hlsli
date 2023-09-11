//インクルードガード
#if !defined(COMMON_FUNCTION_HEADER)
	#define COMMON_FUNCTION_HEADER

	#include "CommonStructuresHeader.hlsli"

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
		return pow(saturate(dot(normal, -normalize(cameraRay + lightRay))), power) * step(0.0f, dot(normal, -lightRay)); //Blinn-Phong(DXライブラリはこちらを採用)
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
		totalDiffuse += CalculateDiffuse(light.direction, normal) * light.diffuse + light.ambient.rgb;
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