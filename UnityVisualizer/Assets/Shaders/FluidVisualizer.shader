Shader "Custom/FluidVisualizer" {
    Properties {
        [MainTexture] _MainTex ("Texture", 2D) = "white" {}
        [MainColor] _BaseColor("Base Color", Color) = (1, 1, 1, 1)

        _VelocityTexture("Velocity Texture", 2D) = "white" {}
        _PressureTexture("Pressure Texture", 2D) = "white" {}
        _SolidCellMapTexture("Solid Cell Map Texture", 2D) = "white" {}

        _DisplayedField("Displayed Field", int) = 0

        _SpeedGradientTexture("Speed Gradient Texture", 2D) = "white" {}
        _MaxSpeedSqrd("Max Speed Squared", float) = 10

        _MaxPressureColor("Max Pressure Color", Color) = (1, 0, 0, 1)
        _MinPressureColor("Min Pressure Color", Color) = (0, 0, 1, 1)

        _MaxPressureValueAbs("Max Pressure Value Abs", float) = 0
    }

    SubShader {
        Tags { "RenderType" = "Opaque" "RenderPipeline" = "UniversalPipeline" }

        Pass {
            HLSLPROGRAM

            #pragma vertex vert
            #pragma fragment frag

            #include "Packages/com.unity.render-pipelines.universal/ShaderLibrary/Core.hlsl"

            int _DisplayedField;
            float _MaxSpeedSqrd, _MaxPressureValueAbs;

            float4 _MaxPressureColor, _MinPressureColor;

            TEXTURE2D(_VelocityTexture);
            SAMPLER(sampler_VelocityTexture);

            TEXTURE2D(_PressureTexture);
            SAMPLER(sampler_PressureTexture);

            TEXTURE2D(_SolidCellMapTexture);
            SAMPLER(sampler_SolidCellMapTexture);

            TEXTURE2D(_SpeedGradientTexture);
            SAMPLER(sampler_SpeedGradientTexture);

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

                float4 color = float4(1.0f, 0.0f, 1.0f, 1.0f);
                // This is probably not very efficient for a shader, would be better having
                // separate shaders for displaying each property
                switch (_DisplayedField) {
                    case 0:  // VELOCITY
                        color = float4(SAMPLE_TEXTURE2D(_VelocityTexture, sampler_VelocityTexture, uv).xy / sqrt(_MaxSpeedSqrd), 0.0f, 1.0f);
                        break;

                    case 1:  // SPEED
                        float2 vel = SAMPLE_TEXTURE2D(_VelocityTexture, sampler_VelocityTexture, uv).xy;
                        float speedSqrd = dot(vel, vel);
                        float t = clamp(speedSqrd / _MaxSpeedSqrd, 0, 1);
                        color = SAMPLE_TEXTURE2D(_SpeedGradientTexture, sampler_SpeedGradientTexture, float2(t, 0));
                        break;

                    case 2:  // PRESSURE
                        float pressure = SAMPLE_TEXTURE2D(_PressureTexture, sampler_PressureTexture, uv).x;
                        color = lerp(_MinPressureColor, _MaxPressureColor, (pressure + _MaxPressureValueAbs) / (2 * _MaxPressureValueAbs));
                        //color = pressure < _MaxPressureValueAbs;
                        break;

                    case 3:  // SOLID CELLS
                        color = float4(SAMPLE_TEXTURE2D(_SolidCellMapTexture, sampler_SolidCellMapTexture, uv).xxx, 1.0f);
                        break;
                }
                return color;
            }
            ENDHLSL
        }
    }
}
