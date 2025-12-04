#pragma once
#include "EThemeType.generated.h"

UENUM(BlueprintType)
enum class EThemeType : uint8
{
    Dark UMETA(DisplayName = "Dark"),
    Light UMETA(DisplayName = "Light"),
    Warm UMETA(DisplayName = "Warm"),
    Cold UMETA(DisplayName = "Cold"),
    Neon UMETA(DisplayName = "Neon"),
    Pastel UMETA(DisplayName = "Pastel"),
    Cyber UMETA(DisplayName = "Cyber")
};
