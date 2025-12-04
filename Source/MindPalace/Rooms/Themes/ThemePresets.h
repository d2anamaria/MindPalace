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
            break;

        case EThemeType::Light:
            T.TintColor = FLinearColor(1.0f, 1.0f, 1.0f);
            T.Brightness = 1.2f;
            T.Saturation = 1.0f;
            T.RoughnessBias = -0.1f;
            break;

        case EThemeType::Warm:
            T.TintColor = FLinearColor(1.0f, 0.75f, 0.45f);
            T.Brightness = 1.0f;
            T.Saturation = 1.2f;
            T.MetalnessBias = 0.1f;
            break;

        case EThemeType::Cold:
            T.TintColor = FLinearColor(0.4f, 0.6f, 1.0f);
            T.Brightness = 0.9f;
            T.Saturation = 0.9f;
            T.RoughnessBias = 0.1f;
            break;

        case EThemeType::Neon:
            T.TintColor = FLinearColor(0.7f, 0.2f, 1.0f);
            T.Brightness = 1.3f;
            T.Saturation = 1.5f;
            T.RoughnessBias = -0.2f;
            T.EmissionBoost = 2.0f;
            break;

        case EThemeType::Pastel:
            T.TintColor = FLinearColor(1.0f, 0.7f, 0.8f);
            T.Saturation = 0.5f;
            break;

        case EThemeType::Cyber:
            T.TintColor = FLinearColor(0.2f, 1.0f, 1.0f);
            T.Brightness = 1.2f;
            T.Saturation = 1.8f;
            T.RoughnessBias = -0.3f;
            T.EmissionBoost = 3.0f;
            break;
        }

        return T;
    }
};
