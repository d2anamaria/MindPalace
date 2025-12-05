#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

// Required for UNiagaraSystem & UNiagaraComponent
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

// Required for UPrimitiveComponent (OnClicked params)
#include "Components/PrimitiveComponent.h"

// Theme + Concept includes
#include "../Themes/EThemeType.h"
#include "ProceduralMaterials/ConceptTypes.h"
#include "ProceduralMaterials/ConceptFactory.h"
#include "ProceduralMaterials/MaterialParamApplier.h"

#include "UAnchorSpawner.generated.h"

class AProceduralRoomActor;

// object will be asset+concepts pairs
USTRUCT(BlueprintType)
struct FAnchorAssetEntry
{
    GENERATED_BODY()

    UPROPERTY()
    UStaticMesh *Mesh = nullptr;

    UPROPERTY()
    EConceptType LeftConcept = EConceptType::Opaque;

    UPROPERTY()
    EConceptType RightConcept = EConceptType::Transparent;
};

UCLASS()
class MINDPALACE_API UAnchorSpawner : public UObject
{
    GENERATED_BODY()

public:
    void Init(AProceduralRoomActor *InOwner);
    void LoadAssets();
    FAnchorAssetEntry GetRandomAsset();

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
    TArray<FAnchorAssetEntry> AnchorAssetPool;

    UPROPERTY()
    UNiagaraSystem *Remember_PixieDust = nullptr;

    UFUNCTION()
    void RestoreDust(UPrimitiveComponent *AnchorComp);

    void DisableDust(UPrimitiveComponent *AnchorComp); 

private:
    void DisableAnchorAndFX(UPrimitiveComponent *AnchorComp);
};
