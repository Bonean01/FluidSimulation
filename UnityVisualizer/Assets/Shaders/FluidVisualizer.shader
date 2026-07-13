Shader "Custom/FluidVisualizer" {
    Properties {
        [MainTexture] _MainTex ("Texture", 2D) = "white" {}
        [MainColor] _BaseColor("Base Color", Color) = (1, 1, 1, 1)
        _VelocityTexture("Velocity Texture", 2D) = "white" {}
        _PressureTexture("Pressure Texture", 2D) = "white" {}
        _SolidCellMapTexture("Solid Cell Map Texture", 2D) = "white" {}
        _GridWidth("Grid Width", int) = 10
        _GridHeight("Grid Height", int) = 10
    }

    SubShader {
        Tags { "RenderType" = "Opaque" "RenderPipeline" = "UniversalPipeline" }

        Pass {
            HLSLPROGRAM

            #pragma vertex vert
            #pragma fragment frag

            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"

            int _GridWidth, _GridHeight;
            float _Offset;

            TEXTURE2D(_VelocityTexture);
            SAMPLER(sampler_VelocityTexture);

            TEXTURE2D(_PressureTexture);
            SAMPLER(sampler_PressureTexture);

            TEXTURE2D(_SolidCellMapTexture);
            SAMPLER(sampler_SolidCellMapTexture);

            struct Attributes {
                float4 positionOS : POSITION;
                float2 uv : TEXCOORD0;
            };

            struct Varyings {
                float4 positionHCS : SV_POSITION;
                float2 uv : TEXCOORD0;
            };

            Varyings vert(Attributes IN) {
                Varyings OUT;
                OUT.positionHCS = TransformObjectToHClip(IN.positionOS.xyz);
                OUT.uv = IN.uv;
                return OUT;
            }

            float4 frag(Varyings IN) : SV_Target {
                float2 uv = IN.uv;
                //float4 color = float4(floor(uv.x * _GridWidth) / _GridWidth, floor(uv.y * _GridHeight) / _GridHeight, 0, 1);

                //float grid = min(frac(uv.x * _GridWidth), frac(uv.y * _GridHeight)) < 0.1f ? 0 : 1;
                //color *= grid;

                float4 color = SAMPLE_TEXTURE2D(_VelocityTexture, sampler_VelocityTexture, uv);

                return color;
            }
            ENDHLSL 
        }
    }
}
