#pragma once

#include "CoreMinimal.h"
#include "URoomOpeningStrategyBase.h"
#include "OvalWindowStrategy.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class MINDPALACE_API UOvalWindowStrategy : public URoomOpeningStrategyBase
{
    GENERATED_BODY()

public:
    virtual bool IsWindowAt_Implementation(int32 X, int32 Y, int32 H,
                                           FVector RoomCenter) const override;

    virtual void SpawnOpening_Implementation(
        AProceduralRoomActor *Owner,
        int32 X, int32 Y, int32 H,
        const FVector &LocalPos,
        const FRotator &Rot) const override;
};
