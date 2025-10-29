#include "ProceduralRoomActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/World.h"
#include "UObject/ConstructorHelpers.h"

AProceduralRoomActor::AProceduralRoomActor()
{
	PrimaryActorTick.bCanEverTick = false;

	// Root scene so we can attach everything under this actor
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	// Default values
	Months = 4;
	RoomHeightCubes = 3;
	bIsRectangle = false;
	CubeSize = 100.f; // Unreal default cube is 100x100x100
	RandomAnchorsCount = 3;
	AnchorMaxHeight = 150.f;
	RoomColor = FLinearColor(0.2f, 0.4f, 1.0f, 1.0f); // bluish

	// Try to auto-load Engine basic cube if user forgets to set CubeMesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
	{
		CubeMesh = CubeAsset.Object;
	}
}

void AProceduralRoomActor::BeginPlay()
{
	Super::BeginPlay();
	// Room is already generated in OnConstruction for editor preview.
	// If you ALSO want it guaranteed at runtime (e.g. in packaged build), call again:
	RegenerateRoom();
}

void AProceduralRoomActor::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
	RegenerateRoom();
}

// ------------------------------------------------------
// Core: wipe old cubes, then rebuild
// ------------------------------------------------------
void AProceduralRoomActor::RegenerateRoom()
{
	ClearPrevious();

	if (Months < 1)
		Months = 1;
	if (RoomHeightCubes < 1)
		RoomHeightCubes = 1;

	// Decide horizontal size in cubes
	int32 WidthCubes = Months;
	int32 LengthCubes = bIsRectangle ? Months * 2 : Months;

	// 1. build perimeter walls
	BuildWalls(WidthCubes, LengthCubes);

	// 2. spawn interior random anchors
	SpawnAnchorCubes(WidthCubes, LengthCubes);
}

// ------------------------------------------------------
// Remove previously spawned cube components
// ------------------------------------------------------
void AProceduralRoomActor::ClearPrevious()
{
	for (UStaticMeshComponent *Comp : SpawnedCubes)
	{
		if (Comp)
		{
			Comp->DestroyComponent();
		}
	}
	SpawnedCubes.Empty();
}

// ------------------------------------------------------
// Build all 4 walls around the rectangle
// ------------------------------------------------------
void AProceduralRoomActor::BuildWalls(int32 WidthCubes, int32 LengthCubes)
{
	// We'll build a hollow rectangle of walls.
	// Coordinate system in local space:
	// X goes along width, Y goes along length, Z is up.

	// Bottom + Top edges (along X)
	for (int32 x = 0; x < WidthCubes; x++)
	{
		// bottom edge, y = 0
		SpawnWallColumn(x, 0, RoomHeightCubes);

		// top edge, y = LengthCubes-1 (only if different from 0)
		if (LengthCubes > 1)
		{
			SpawnWallColumn(x, LengthCubes - 1, RoomHeightCubes);
		}
	}

	// Left + Right edges (along Y)
	for (int32 y = 0; y < LengthCubes; y++)
	{
		// left edge, x = 0
		SpawnWallColumn(0, y, RoomHeightCubes);

		// right edge, x = WidthCubes-1 (only if different from 0)
		if (WidthCubes > 1)
		{
			SpawnWallColumn(WidthCubes - 1, y, RoomHeightCubes);
		}
	}
}

// ------------------------------------------------------
// Spawn one column of cubes stacked up (a wall pillar).
// Here we can add door/window logic later.
// ------------------------------------------------------
void AProceduralRoomActor::SpawnWallColumn(int32 GridX, int32 GridY, int32 HeightCubes)
{
	// Decide if this column should be a "door gap" at the bottom row, etc.
	// For now: solid walls everywhere.

	for (int32 h = 0; h < HeightCubes; h++)
	{
		FVector LocalPos(
			GridX * CubeSize,
			GridY * CubeSize,
			h * CubeSize);

		// Spawn cube
		UStaticMeshComponent *CubeComp = SpawnCubeAt(LocalPos, FRotator::ZeroRotator);

		// OPTIONAL FUTURE:
		// if this GridX/GridY is "door", skip h == 0 to leave a hole.
		// if this GridX/GridY is "window", skip some middle h to make a window gap.
	}
}

// ------------------------------------------------------
// Spawn some knowledge cubes randomly inside floor area
// ------------------------------------------------------
void AProceduralRoomActor::SpawnAnchorCubes(int32 WidthCubes, int32 LengthCubes)
{
	// interior bounds in local space
	float MaxX = (WidthCubes - 1) * CubeSize;
	float MaxY = (LengthCubes - 1) * CubeSize;

	for (int32 i = 0; i < RandomAnchorsCount; i++)
	{
		// pick random position NOT too close to walls
		float Margin = CubeSize * 0.5f;

		float RandX = FMath::RandRange(Margin, MaxX - Margin);
		float RandY = FMath::RandRange(Margin, MaxY - Margin);
		float RandZ = FMath::RandRange(0.f, AnchorMaxHeight);

		FVector LocalPos(RandX, RandY, RandZ);

		// Give them a tiny rotation so they don't look copy/paste
		FRotator Rot(0.f, FMath::RandRange(0.f, 360.f), 0.f);

		UStaticMeshComponent *AnchorCube = SpawnCubeAt(LocalPos, Rot);

		// You could mark these anchors differently by pushing a brighter color, etc.
		// Example: more emissive-looking color tint.
		if (AnchorCube)
		{
			UMaterialInstanceDynamic *DynMat =
				AnchorCube->CreateAndSetMaterialInstanceDynamic(0);

			if (DynMat)
			{
				// Slightly different tint for "memory anchors"
				FLinearColor Accent = RoomColor * 1.5f;
				DynMat->SetVectorParameterValue("BaseColor", Accent);
			}
		}
	}
}

// ------------------------------------------------------
// Low-level helper: create a cube component, attach to root, color it
// ------------------------------------------------------
UStaticMeshComponent *AProceduralRoomActor::SpawnCubeAt(const FVector &LocalPos, const FRotator &Rot)
{
	if (!CubeMesh)
		return nullptr;

	UStaticMeshComponent *Comp = NewObject<UStaticMeshComponent>(this);
	Comp->SetStaticMesh(CubeMesh);

	// Attach for hierarchy, so moving the actor moves the whole room
	Comp->SetupAttachment(RootComponent);

	// Register so it shows up in the world
	Comp->RegisterComponent();

	// Position relative to the actor's root
	Comp->SetRelativeLocation(LocalPos);
	Comp->SetRelativeRotation(Rot);

	// Apply material tint
	ApplyMaterialTo(Comp);

	// Keep track so we can delete them on regen
	SpawnedCubes.Add(Comp);

	return Comp;
}

void AProceduralRoomActor::ApplyMaterialTo(UStaticMeshComponent *Comp)
{
	if (!Comp)
		return;

	// Make a dynamic material for color control
	UMaterialInstanceDynamic *DynMat = Comp->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMat)
	{
		// Your master material in Unreal must expose a VectorParameter "BaseColor"
		DynMat->SetVectorParameterValue("BaseColor", RoomColor);
	}
}
