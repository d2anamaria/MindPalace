#pragma once

#include "CoreMinimal.h"
#include "URoomOpeningStrategyBase.h"
#include "SquareWindowStrategy.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class USquareWindowStrategy : public URoomOpeningStrategyBase
{
    GENERATED_BODY()
public:
    virtual bool IsWindowAt_Implementation(int32 X, int32 Y, int32 H,
                                           FVector RoomCenter) const override;

    virtual void SpawnOpening_Implementation(AProceduralRoomActor *Owner,
                                             const FVector &LocalPos,
                                             const FRotator &Rot) const override;
};
