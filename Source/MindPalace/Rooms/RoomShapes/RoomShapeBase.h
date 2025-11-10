#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RoomShapeBase.generated.h"

class AProceduralRoomActor;

UCLASS(Abstract, Blueprintable, EditInlineNew)
class URoomShapeBase : public UObject
{
    GENERATED_BODY()
public:
    virtual void Build(AProceduralRoomActor *Owner) PURE_VIRTUAL(URoomShapeBase::Build, );

protected:
    // Helper used by all shapes to decide if a cube should be skipped for a window
    bool ShouldSkipForWindow(AProceduralRoomActor *Owner, int32 X, int32 Y, int32 H, int32 Width, int32 Length) const;
};
