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
    const FVector &Pos,
    const FRotator &Rot) const
{
    // Square windows = hole-only
}
