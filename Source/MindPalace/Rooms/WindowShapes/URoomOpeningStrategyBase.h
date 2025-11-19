#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "URoomOpeningStrategyBase.generated.h" // MUST BE LAST INCLUDE

class AProceduralRoomActor; // ✔ REQUIRED FORWARD DECLARATION

UCLASS(Abstract, Blueprintable, EditInlineNew)
class MINDPALACE_API URoomOpeningStrategyBase : public UObject
{
    GENERATED_BODY()
public:
    // Does this coordinate contain a window?
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    bool IsWindowAt(int32 X, int32 Y, int32 H, FVector RoomCenter) const;

    // Spawn the opening mesh (or nothing for hole-only windows)
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
    void SpawnOpening(AProceduralRoomActor *Owner,
                      const FVector &LocalPos,
                      const FRotator &Rot) const;
};
