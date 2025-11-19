#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RoomShapeBase.generated.h"

class AProceduralRoomActor;

UCLASS(Abstract, Blueprintable, EditInlineNew)
class MINDPALACE_API URoomShapeBase : public UObject
{
    GENERATED_BODY()

public:
    virtual void Build(AProceduralRoomActor *Owner) PURE_VIRTUAL(URoomShapeBase::Build, );

protected:
    bool HandleWindowIfAny(AProceduralRoomActor *Owner,
                           int32 X, int32 Y, int32 H,
                           float CubeSize,
                           int32 Width, int32 Length) const;
};
