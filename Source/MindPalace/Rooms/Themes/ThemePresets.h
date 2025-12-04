#pragma once
#include "ThemeData.h"
#include "EThemeType.h"

struct ThemePresets
{
    static FThemeData GetPreset(EThemeType Style)
    {
        FThemeData T;

        switch (Style)
        {
        case EThemeType::Dark:
            T.TintColor = FLinearColor(0.15f, 0.15f, 0.2f);
            T.Brightness = 0.8f;
            T.Saturation = 0.9f;
            T.RoughnessBias = 0.7f;
            T.LightColor = FLinearColor(1.0f, 0.85f, 0.4f);
            T.LightIntensity = 3000.f;
            T.LightRadius = 3000.f;
            break;

        case EThemeType::Light:
            T.TintColor = FLinearColor(1.0f, 1.0f, 1.0f);
            T.Brightness = 1.2f;
            T.Saturation = 1.0f;
            T.RoughnessBias = -0.1f;
            T.LightColor = FLinearColor::White;
            T.LightIntensity = 20000.f;
            T.LightRadius = 4000.f;
            break;

        case EThemeType::Warm:
            T.TintColor = FLinearColor(1.0f, 0.15f, 0.15f);
            T.Brightness = 1.0f;
            T.Saturation = 1.2f;
            T.MetalnessBias = 0.1f;
            T.LightColor = FLinearColor(1.0f, 0.8f, 0.6f); // warm yellow-orange
            T.LightIntensity = 15000.f;
            T.LightRadius = 3500.f;
            break;

        case EThemeType::Cold:
            T.TintColor = FLinearColor(0.75f, 0.65f, 0.90f);
            T.Brightness = 0.9f;
            T.Saturation = 0.9f;
            T.RoughnessBias = 0.1f;
            T.LightColor = FLinearColor(0.6f, 0.8f, 1.0f); // icy blue
            T.LightIntensity = 12000.f;
            T.LightRadius = 3500.f;
            break;

        case EThemeType::Neon:
            T.TintColor = FLinearColor(0.0f, 1.0f, 0.2f);
            T.Brightness = 1.3f;
            T.Saturation = 1.5f;
            T.RoughnessBias = -0.2f;
            T.EmissionBoost = 2.0f;
            T.LightColor = FLinearColor(0.4f, 0.0f, 1.0f); // deep neon purple
            T.LightIntensity = 30000.f;
            T.LightRadius = 5000.f;
            break;

        case EThemeType::Pastel:
            T.TintColor = FLinearColor(0.80f, 0.70f, 0.95f);
            T.Saturation = 0.5f;
            T.LightColor = FLinearColor(1.0f, 0.8f, 0.9f); // soft white/pink
            T.LightIntensity = 8000.f;
            T.LightRadius = 3000.f;
            break;

        case EThemeType::Cyber:
            T.TintColor = FLinearColor(0.8f, 0.2f, 1.0f);
            T.Brightness = 1.2f;
            T.Saturation = 1.8f;
            T.RoughnessBias = -0.3f;
            T.EmissionBoost = 1.0f;
            T.LightColor = FLinearColor(0.0f, 1.0f, 0.8f); // cyan green
            T.LightIntensity = 40000.f;
            T.LightRadius = 6000.f;
            break;
        }

        return T;
    }
};
