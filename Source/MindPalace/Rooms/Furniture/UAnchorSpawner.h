#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "UAnchorSpawner.generated.h"

class AProceduralRoomActor;

UCLASS()
class UAnchorSpawner : public UObject
{
    GENERATED_BODY()

public:
    void Init(AProceduralRoomActor *InOwner);
    void BuildAnchors(int32 WidthCubes, int32 LengthCubes);

private:
    UPROPERTY()
    AProceduralRoomActor *Owner = nullptr;

    UPROPERTY()
    TArray<UStaticMesh *> AnchorAssetPool;

    void LoadAssets();
    UStaticMesh *GetRandomAsset() const;
};
