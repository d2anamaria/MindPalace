#include "SquareWindowStrategy.h"
#include "../ProceduralRoomActor.h"

bool USquareWindowStrategy::IsWindowAt_Implementation(
    int32 X, int32 Y, int32 H, FVector RoomCenter) const
{
    const AProceduralRoomActor *Owner = Cast<AProceduralRoomActor>(GetOuter());
    return Owner && Owner->ShouldHaveWindowAt(X, Y, H, RoomCenter);
}

void USquareWindowStrategy::SpawnOpening_Implementation(
    AProceduralRoomActor *Owner,
    int32 X, int32 Y, int32 H,
    const FVector &LocalPos,
    const FRotator &Rot) const
{
    // still hole-only, do nothing
}
