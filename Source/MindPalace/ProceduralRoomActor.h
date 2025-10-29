#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralRoomActor.generated.h"

UCLASS()
class MINDPALACE_API AProceduralRoomActor : public AActor
{
	GENERATED_BODY()

public:
	AProceduralRoomActor();

	// Called in editor whenever you move sliders, and also at runtime start.
	virtual void OnConstruction(const FTransform &Transform) override;

protected:
	virtual void BeginPlay() override;

public:
	// ================ PARAMETERS YOU TWEAK IN DETAILS PANEL ================

	// How "big" the room is. We'll use this as number of cubes on each wall.
	// If bIsRectangle = false -> room is Months x Months
	// If bIsRectangle = true  -> room is Months x (Months * 2)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	int32 Months;

	// Height of the wall in cubes (vertical stack)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	int32 RoomHeightCubes;

	// Rectangle mode toggle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	bool bIsRectangle;

	// Mesh for each cube block. You assign /Engine/BasicShapes/Cube in editor.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	UStaticMesh *CubeMesh;

	// Size of one cube in world units. 100 uu is default UE cube.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	float CubeSize;

	// How many random knowledge cubes to drop inside the room
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	int32 RandomAnchorsCount;

	// How high (max Z) the random cubes are allowed to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	float AnchorMaxHeight;

	// Color theme for cubes (we’ll apply per-instance dynamic material)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	FLinearColor RoomColor;

private:
	// We create components at runtime and keep references so we can clean them up/regenerate
	UPROPERTY(VisibleAnywhere, Category = "Proc Room")
	USceneComponent *Root;

	// Store spawned mesh components so we can delete/regenerate cleanly
	UPROPERTY(Transient)
	TArray<UStaticMeshComponent *> SpawnedCubes;

private:
	void RegenerateRoom();
	void ClearPrevious();

	void BuildWalls(int32 WidthCubes, int32 LengthCubes);
	void SpawnWallColumn(int32 GridX, int32 GridY, int32 HeightCubes);

	void SpawnAnchorCubes(int32 WidthCubes, int32 LengthCubes);

	UStaticMeshComponent *SpawnCubeAt(const FVector &LocalPos, const FRotator &Rot);
	void ApplyMaterialTo(UStaticMeshComponent *Comp);
};
