#pragma once
#include "CoreMinimal.h"
#include "ConceptTypes.h"
#include "ConceptData.generated.h"

USTRUCT(BlueprintType)
struct FConceptData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor TintColor = FLinearColor::Blue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Brightness = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Saturation = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MetalnessBias = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MetallicLevel = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RoughnessBias = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RoughnessLevel = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReflectionStrength = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EmissionBoost = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EmissionStrength = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OpacityLevel = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RefractionAmount = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NormalStrength = 0.0f;
};
