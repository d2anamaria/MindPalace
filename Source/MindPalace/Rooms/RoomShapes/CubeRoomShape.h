#pragma once

#include "CoreMinimal.h"
#include "RoomShapeBase.h"
#include "CubeRoomShape.generated.h" // must be the LAST include

UCLASS()
class UCubeRoomShape : public URoomShapeBase
{
    GENERATED_BODY()

public:
    virtual void Build(AProceduralRoomActor *Owner) override;
};
