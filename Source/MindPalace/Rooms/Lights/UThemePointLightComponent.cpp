#include "UThemePointLightComponent.h"
#include "Components/PointLightComponent.h"

UThemePointLightComponent::UThemePointLightComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UThemePointLightComponent::InitLight(const FVector &RelativePos, float Radius)
{
    // Create sub-light
    Light = NewObject<UPointLightComponent>(this);
    Light->SetupAttachment(this);
    Light->RegisterComponent();

    SetRelativeLocation(RelativePos);

    Light->SetAttenuationRadius(Radius * 100.0f);
    Light->SetCastShadows(true);
}

void UThemePointLightComponent::ApplyTheme(const FThemeData &Theme)
{
    if (!Light)
        return;

    // Real UE lighting values
    Light->SetLightColor(Theme.LightColor);
    Light->SetIntensity(Theme.LightIntensity * 10.0f);
}
