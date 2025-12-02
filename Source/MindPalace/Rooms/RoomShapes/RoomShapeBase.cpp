#include "RoomShapeBase.h"
#include "../ProceduralRoomActor.h"
#include "../WindowShapes/URoomOpeningStrategyBase.h"

bool URoomShapeBase::HandleWindowIfAny(AProceduralRoomActor *Owner,
                                       int32 X, int32 Y, int32 H,
                                       float CubeSize,
                                       int32 Width, int32 Length) const
{
    if (!Owner || !Owner->WindowStrategy)
        return false;

    if (!Owner->ShouldHaveWindowAt(X, Y, H, FVector(Width / 2.f, Length / 2.f, 0.f)))
        return false;

    FVector Pos(X * CubeSize, Y * CubeSize, H * CubeSize);
    FRotator Rot = FRotator::ZeroRotator;

    Owner->WindowStrategy->SpawnOpening(Owner, X, Y, H, Pos, Rot);

    return true;
}
