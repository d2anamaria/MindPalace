#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

// Required for UNiagaraSystem & UNiagaraComponent
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

// Required for UPrimitiveComponent (OnClicked params)
#include "Components/PrimitiveComponent.h"

#include "UAnchorSpawner.generated.h"

class AProceduralRoomActor;

UCLASS()
class MINDPALACE_API UAnchorSpawner : public UObject
{
    GENERATED_BODY()

public:
    void Init(AProceduralRoomActor *InOwner);
    void LoadAssets();
    UStaticMesh *GetRandomAsset();
    void BuildAnchors(int32 WidthCubes, int32 LengthCubes);

    UFUNCTION()
    void OnAnchorClicked(UPrimitiveComponent *ClickedComp, FKey ButtonPressed);

    UFUNCTION()
    void OnAnchorOverlap(
        UPrimitiveComponent *OverlappedComp,
        AActor *OtherActor,
        UPrimitiveComponent *OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult &SweepResult);

private:
    UPROPERTY()
    AProceduralRoomActor *Owner = nullptr;

    UPROPERTY()
    TArray<UStaticMesh *> AnchorAssetPool;

    UPROPERTY()
    UNiagaraSystem *Remember_PixieDust = nullptr;

    UFUNCTION()
    void RestoreDust(UPrimitiveComponent *AnchorComp); // called after 5s

    void DisableDust(UPrimitiveComponent *AnchorComp); // only stop FX

private:
    void DisableAnchorAndFX(UPrimitiveComponent *AnchorComp);
};
