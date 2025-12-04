#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "../Themes/ThemeData.h"
#include "UThemePointLightComponent.generated.h"

class UPointLightComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINDPALACE_API UThemePointLightComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    UThemePointLightComponent();

    // The actual UE point light
    UPROPERTY(VisibleAnywhere)
    UPointLightComponent* Light = nullptr;

    // Initialization of the light
    void InitLight(const FVector& RelativePos, float Radius = 800.f);

    // Apply FThemeData parameters
    void ApplyTheme(const FThemeData& Theme);
};
