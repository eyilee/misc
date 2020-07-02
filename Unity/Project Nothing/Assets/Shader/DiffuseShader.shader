Shader "Nothing/DiffuseShader"
{
	Properties
	{
		_Diffuse ("Diffuse", Color) = (1.0, 1.0, 1.0, 1.0)
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
				fixed halfLambert = dot (worldNormal, worldLightDir) * 0.5 + 0.5;
				fixed3 diffuse = _LightColor0.rgb * _Diffuse.rgb * halfLambert;
				o.color = ambient + diffuse;

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
