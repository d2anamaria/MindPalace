#include "URoomOpeningStrategyBase.h"
#include "../ProceduralRoomActor.h"

// Default: no window
bool URoomOpeningStrategyBase::IsWindowAt_Implementation(int32 X, int32 Y, int32 H, FVector RoomCenter) const
{
    return false;
}

// Default: do nothing
void URoomOpeningStrategyBase::SpawnOpening_Implementation(AProceduralRoomActor *Owner,
                                                           const FVector &LocalPos,
                                                           const FRotator &Rot) const
{
}
