#include "RoomShapeBase.h"
#include "../ProceduralRoomActor.h"

bool URoomShapeBase::ShouldSkipForWindow(AProceduralRoomActor *Owner, int32 X, int32 Y, int32 H, int32 Width, int32 Length) const
{
    if (!Owner)
        return false;

    // Calculate the approximate center of the room grid
    FVector RoomCenter(Width / 2.0f, Length / 2.0f, 0.0f);

    // Delegate decision to the actor’s window logic
    return Owner->ShouldHaveWindowAt(X, Y, H, RoomCenter);
}
