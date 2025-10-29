#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralRoomActor.generated.h"

UENUM(BlueprintType)
enum class ERoomShape : uint8
{
	Cube UMETA(DisplayName = "Cube"),
	HalfSphere UMETA(DisplayName = "Half Sphere"),
	Cylinder UMETA(DisplayName = "Cylinder")
};

UCLASS()
class MINDPALACE_API AProceduralRoomActor : public AActor
{
	GENERATED_BODY()

public:
	AProceduralRoomActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/** Controls the shape of the room */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings")
	ERoomShape RoomShape;

	/** Controls the overall scale (related to months) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings")
	int32 Months;

	/** Color of the room walls */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings")
	FLinearColor RoomColor;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *OuterMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *InnerMesh;

	void UpdateRoom();
	void ApplyShape();
};
