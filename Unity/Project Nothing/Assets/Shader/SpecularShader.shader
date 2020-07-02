Shader "Nothing/SpecularShader"
{
	Properties
	{
		_Diffuse ("Diffuse", Color) = (1.0, 1.0, 1.0, 1.0)
		_Specular ("Specular", Color) = (1.0, 1.0, 1.0, 1.0)
		_Gloss ("Gloss", Range (8.0, 256.0)) = 20.0
	}
	SubShader
	{
		Pass
		{
			Tags { "LightMode" = "ForwardBase" }

			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#include "Lighting.cginc"

			fixed4 _Diffuse;
			fixed4 _Specular;
			fixed _Gloss;

			struct appdata
			{
				float4 vertex : POSITION;
				float3 normal : NORMAL;
			};

			struct v2f
			{
				float4 vertex : SV_POSITION;
				fixed3 color : COLOR0;
			};

			v2f vert (appdata v)
			{
				v2f o;
				o.vertex = UnityObjectToClipPos (v.vertex);
				fixed3 ambient = UNITY_LIGHTMODEL_AMBIENT.xyz;
				// Half Lambert lighting model.
				fixed3 worldNormal = UnityObjectToWorldNormal (v.normal);
				fixed3 worldLightDir = normalize (_WorldSpaceLightPos0.xyz);
				fixed3 diffuse = _LightColor0.rgb * _Diffuse.rgb * saturate (dot (worldNormal, worldLightDir));
				// Blinn lighting model.
				fixed3 reflectDir = normalize (reflect (-worldLightDir, worldNormal));
				fixed3 viewDir = normalize (UnityWorldSpaceViewDir (mul (unity_ObjectToWorld, v.vertex)));
				fixed3 halfDir = normalize (worldLightDir + viewDir);
				fixed3 specular = _LightColor0.rgb * _Specular.rgb * pow (max (0, dot (worldNormal, halfDir)), _Gloss);
				o.color = ambient + diffuse + specular;

				return o;
			}

			fixed4 frag (v2f i) : SV_Target
			{
				return fixed4 (i.color, 1.0);
			}
			ENDCG
		}
	}
	FallBack "Diffuse"
}
