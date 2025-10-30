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
};
