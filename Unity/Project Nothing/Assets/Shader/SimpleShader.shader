﻿Shader "Nothing/SimpleShader"
{
	Properties
	{
		_Color ("Color Tint", Color) = (1.0, 1.0, 1.0, 1.0)
	}
	SubShader
	{
		Pass
		{
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag

			fixed4 _Color;

			struct appdata
			{
				float4 vertex : POSITION;
				float3 normal : NORMAL;
				float2 uv : TEXCOORD0;
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
				o.color = v.normal * 0.5 + fixed3 (0.5, 0.5, 0.5);

				return o;
			}

			fixed4 frag (v2f i) : SV_Target
			{
				fixed3 c = i.color;
				c *= _Color.rgb;
				return fixed4 (c, 1.0);
			}
			ENDCG
		}
	}
	FallBack "Diffuse"
}
