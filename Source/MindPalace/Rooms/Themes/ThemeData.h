#pragma once
#include "CoreMinimal.h"
#include "ThemeData.generated.h"

USTRUCT(BlueprintType)
struct FThemeData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor TintColor = FLinearColor::Red;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Brightness = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Saturation = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RoughnessBias = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MetalnessBias = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EmissionBoost = 0.0f;
};
