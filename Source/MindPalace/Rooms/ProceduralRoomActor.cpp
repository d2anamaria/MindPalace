#include "ProceduralRoomActor.h"
#include "RoomShapes/RoomShapeBase.h"
#include "WindowShapes/SquareWindowStrategy.h"
#include "WindowShapes/CircularWindowStrategy.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"

AProceduralRoomActor::AProceduralRoomActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Months = 4;
	RoomHeightCubes = 3;
	bIsRectangle = false;
	CubeSize = 100.f;
	RandomAnchorsCount = 3;
	AnchorMaxHeight = 150.f;
	RoomColor = FLinearColor(0.2f, 0.4f, 1.0f);
	bEnableWindows = true;
	WindowHeightCenters = {1};

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeAsset.Succeeded())
		CubeMesh = CubeAsset.Object;
}

void AProceduralRoomActor::BeginPlay()
{
	Super::BeginPlay();
	RegenerateRoom();
}

void AProceduralRoomActor::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);
	RegenerateRoom();
}

void AProceduralRoomActor::RegenerateRoom()
{
	ClearPrevious();

	int32 Width = Months;
	int32 Length = bIsRectangle ? Months * 2 : Months;
	int32 Height = RoomHeightCubes;

	// bEnableWindows = (Width >= 4 && Height >= 3);
	if (WindowWidth <= 0)
		WindowWidth = FMath::Clamp(Width / 5, 1, 3);
	if (WindowHeight <= 0)
		WindowHeight = FMath::Clamp(Height / 3, 1, 2);
	if (WindowSpacing <= 0)
		WindowSpacing = FMath::Clamp(Width / 2, 4, 8);

	WindowHeightCenters.Empty();
	if (bEnableWindows)
	{
		// Only middle-level windows
		WindowHeightCenters.Add(Height / 2);
	}

	if (RoomShapeStrategy)
		RoomShapeStrategy->Build(this);

	SpawnAnchorCubes(Width, Length);
}

void AProceduralRoomActor::ClearPrevious()
{
	// Destroy cube components
	for (UStaticMeshComponent *C : SpawnedCubes)
	{
		if (C)
			C->DestroyComponent();
	}
	SpawnedCubes.Empty();

	// Destroy ALL window components (corners etc.)
	for (UActorComponent *C : SpawnedComponents)
	{
		if (C)
		{
			if (USceneComponent *SC = Cast<USceneComponent>(C))
				SC->DestroyComponent();
		}
	}
	SpawnedComponents.Empty();
}

UStaticMeshComponent *AProceduralRoomActor::SpawnCubeAt(const FVector &LocalPos,
														const FRotator &Rot)
{
	if (!CubeMesh)
		return nullptr;

	UStaticMeshComponent *Comp = NewObject<UStaticMeshComponent>(this);
	Comp->SetStaticMesh(CubeMesh);
	Comp->SetupAttachment(RootComponent);
	Comp->RegisterComponent();
	Comp->SetRelativeLocation(LocalPos);
	Comp->SetRelativeRotation(Rot);

	SpawnedCubes.Add(Comp);
	RegisterSpawned(Comp); // <-- NEW

	return Comp;
}

// WINDOW
bool AProceduralRoomActor::ShouldHaveWindowAt(int32 X, int32 Y, int32 H, FVector RoomCenter) const
{
	if (!bEnableWindows)
		return false;

	// --- Horizontal zone ---
	bool bInHorizontalWindow = false;
	int32 Width = Months;
	int32 Length = bIsRectangle ? Months * 2 : Months;

	if (Y == 0 || Y == Length - 1) // front/back walls
	{
		if ((X % WindowSpacing) >= (WindowSpacing / 2 - WindowWidth / 2) &&
			(X % WindowSpacing) < (WindowSpacing / 2 + WindowWidth / 2))
			bInHorizontalWindow = true;
	}
	else if (X == 0 || X == Width - 1) // left/right walls
	{
		if ((Y % WindowSpacing) >= (WindowSpacing / 2 - WindowWidth / 2) &&
			(Y % WindowSpacing) < (WindowSpacing / 2 + WindowWidth / 2))
			bInHorizontalWindow = true;
	}
	// --- Vertical zone ---
	bool bInVerticalWindow = false;

	if (WindowHeightCenters.Num() == 0)
	{
		// Default to middle height if nothing set
		int32 Mid = FMath::Max(1, RoomHeightCubes / 2);
		bInVerticalWindow = (H >= Mid - WindowHeight / 2 && H <= Mid + WindowHeight / 2);
	}
	else
	{
		for (int32 CenterH : WindowHeightCenters)
		{
			if (H >= CenterH - WindowHeight / 2 && H <= CenterH + WindowHeight / 2)
			{
				bInVerticalWindow = true;
				break;
			}
		}
	}

	return (bInHorizontalWindow && bInVerticalWindow);
}
void AProceduralRoomActor::ApplyMaterialTo(UStaticMeshComponent *Comp)
{
	if (!Comp)
		return;

	UMaterialInstanceDynamic *DynMat = nullptr;

	if (RoomMaterial)
	{
		DynMat = UMaterialInstanceDynamic::Create(RoomMaterial, this);
		Comp->SetMaterial(0, DynMat);
	}
	else
	{
		DynMat = Comp->CreateAndSetMaterialInstanceDynamic(0);
	}

	if (!DynMat)
		return;

	// OPTIONAL: tint
	DynMat->SetVectorParameterValue("BaseColor", RoomColor);

	// Get cube index
	FVector Loc = Comp->GetRelativeLocation();
	float Cube = CubeSize > 0.f ? CubeSize : 100.f;

	int32 GridX = FMath::RoundToInt(Loc.X / Cube);
	int32 GridY = FMath::RoundToInt(Loc.Y / Cube);
	int32 GridH = FMath::RoundToInt(Loc.Z / Cube);

	// Send grid coords
	DynMat->SetScalarParameterValue("GridX", GridX);
	DynMat->SetScalarParameterValue("GridY", GridY);
	DynMat->SetScalarParameterValue("GridH", GridH);

	// Room dimensions
	int32 Width = Months;
	int32 Length = bIsRectangle ? Months * 2 : Months;
	int32 Height = RoomHeightCubes;

	// Select atlas width depending on wall
	int32 TotalWidth = (GridY == 0 || GridY == Length - 1) ? Width : Length;

	DynMat->SetScalarParameterValue("TotalWidth", TotalWidth);
	DynMat->SetScalarParameterValue("TotalHeight", Height);

	// ============================================
	// 🔥 Compute the atlas UV cell for this cube
	// ============================================

	float USize = 1.0f / TotalWidth;
	float VSize = 1.0f / Height;
}

// random anchors (same as before)
void AProceduralRoomActor::SpawnAnchorCubes(int32 WidthCubes, int32 LengthCubes)
{
	float MaxX = (WidthCubes - 1) * CubeSize;
	float MaxY = (LengthCubes - 1) * CubeSize;

	for (int32 i = 0; i < RandomAnchorsCount; i++)
	{
		float Margin = CubeSize * 0.5f;
		float RandX = FMath::RandRange(Margin, MaxX - Margin);
		float RandY = FMath::RandRange(Margin, MaxY - Margin);
		float RandZ = FMath::RandRange(0.f, AnchorMaxHeight);

		FVector LocalPos(RandX, RandY, RandZ);
		FRotator Rot(0.f, FMath::RandRange(0.f, 360.f), 0.f);

		UStaticMeshComponent *Anchor = SpawnCubeAt(LocalPos, Rot);
		if (Anchor)
		{
			UMaterialInstanceDynamic *DynMat = Anchor->CreateAndSetMaterialInstanceDynamic(0);
			if (DynMat)
			{
				FLinearColor Accent = RoomColor * 1.5f;
				DynMat->SetVectorParameterValue("BaseColor", Accent);
			}
		}
	}
}
void AProceduralRoomActor::RegisterSpawned(UActorComponent *Comp)
{
	if (Comp)
		SpawnedComponents.Add(Comp);
}

void AProceduralRoomActor::ApplyUVParams(
	UStaticMeshComponent *Comp,
	int32 X, int32 H,
	int32 Width, int32 Height)
{
	if (!Comp)
		return;

	int32 MaterialWidth = 5;
	int32 MaterialHeight = 5;

	float USize = 1.f / MaterialWidth;
	float VSize = 1.f / MaterialHeight;

	// --- Determine which wall this cube belongs to ---
	FVector Loc = Comp->GetRelativeLocation();
	int32 GridX = FMath::RoundToInt(Loc.X / CubeSize);
	int32 GridY = FMath::RoundToInt(Loc.Y / CubeSize);

	// We know the actual room sizes
	int32 RoomWidth = Months;
	int32 RoomLength = bIsRectangle ? Months * 2 : Months;

	bool bIsFront = (GridY == 0);
	bool bIsBack = (GridY == RoomLength - 1);
	bool bIsLeft = (GridX == 0);
	bool bIsRight = (GridX == RoomWidth - 1);

	// --- UV flipping rules ---
	bool bFlipHorizontal = false;

	if (bIsLeft)
		bFlipHorizontal = true; // back wall needs flipping
	if (bIsRight)
		bFlipHorizontal = true; // front wall needs flipping

	float UStart;
	if (bFlipHorizontal)
		UStart = float((MaterialWidth - 1) - (X % MaterialWidth)) * USize;
	else
		UStart = float(X % MaterialWidth) * USize;

	// Vertical flip only (texture layout)
	float VStart = float((MaterialHeight - 1) - (H % MaterialHeight)) * VSize;

	Comp->SetScalarParameterValueOnMaterials(TEXT("UStart"), UStart);
	Comp->SetScalarParameterValueOnMaterials(TEXT("VStart"), VStart);
	Comp->SetScalarParameterValueOnMaterials(TEXT("USize"), USize);
	Comp->SetScalarParameterValueOnMaterials(TEXT("VSize"), VSize);
}
