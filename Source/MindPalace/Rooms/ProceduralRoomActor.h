#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralRoomActor.generated.h"

class URoomShapeBase;

UCLASS()
class MINDPALACE_API AProceduralRoomActor : public AActor
{
	GENERATED_BODY()

public:
	AProceduralRoomActor();

	virtual void OnConstruction(const FTransform &Transform) override;
	virtual void BeginPlay() override;

	// ---------------- Parameters exposed to the editor ----------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	int32 Months;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	int32 RoomHeightCubes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	bool bIsRectangle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	UStaticMesh *CubeMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	float CubeSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	int32 RandomAnchorsCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	float AnchorMaxHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room")
	FLinearColor RoomColor;

	// The shape strategy you pick in the editor (Cube / Dome / Hemisphere)
	UPROPERTY(EditAnywhere, Instanced, Category = "Proc Room|Shape")
	URoomShapeBase *RoomShapeStrategy;

	// ---------------- Helpers reused by shapes ----------------
	UStaticMeshComponent *SpawnCubeAt(const FVector &LocalPos, const FRotator &Rot);
	void ApplyMaterialTo(UStaticMeshComponent *Comp);
	float GetCubeSize() const { return CubeSize; }
	UStaticMesh *GetCubeMesh() const { return CubeMesh; }

	// WINDOWS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room|Windows")
	bool bEnableWindows = true;

	// Window size
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room|Windows")
	int32 WindowWidth = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room|Windows")
	int32 WindowHeight = 2;

	// Distance between window centers along X axis
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room|Windows")
	int32 WindowSpacing = 5;

	// Vertical center positions (in cube indices)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Proc Room|Windows")
	TArray<int32> WindowHeightCenters;

	//  Defines where windows are placed, given room grid coordinates
	UFUNCTION(BlueprintCallable, Category = "Proc Room|Windows")
	bool ShouldHaveWindowAt(int32 X, int32 Y, int32 H, FVector RoomCenter) const;

	// ---------------- Core generation ----------------
	void RegenerateRoom();

protected:
	void ClearPrevious();
	void SpawnAnchorCubes(int32 WidthCubes, int32 LengthCubes);

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent *Root;

	UPROPERTY(Transient)
	TArray<UStaticMeshComponent *> SpawnedCubes;
};
