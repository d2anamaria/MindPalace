#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralRoomActor.generated.h"

UENUM(BlueprintType)
enum class ERoomShape : uint8
{
	Cube UMETA(DisplayName = "Cube"),
	Sphere UMETA(DisplayName = "Sphere"),
	HalfSphere UMETA(DisplayName = "HalfSphere"),
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings")
	int32 Months;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings")
	FLinearColor RoomColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room Settings")
	ERoomShape RoomShape;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *OuterMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *InnerMesh;

	// Cached static meshes (loaded once)
	UPROPERTY()
	UStaticMesh *CubeMesh;

	UPROPERTY()
	UStaticMesh *SphereMesh;

	UPROPERTY()
	UStaticMesh *CylinderMesh;

	void UpdateRoom();
};
