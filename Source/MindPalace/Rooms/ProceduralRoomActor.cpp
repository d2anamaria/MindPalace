#include "ProceduralRoomActor.h"
#include "RoomShapes/RoomShapeBase.h"
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

	if (RoomShapeStrategy)
		RoomShapeStrategy->Build(this); // <- Strategy pattern!

	int32 Width = Months;
	int32 Length = bIsRectangle ? Months * 2 : Months;
	SpawnAnchorCubes(Width, Length);
}

void AProceduralRoomActor::ClearPrevious()
{
	for (UStaticMeshComponent *Comp : SpawnedCubes)
		if (Comp)
			Comp->DestroyComponent();
	SpawnedCubes.Empty();
}

UStaticMeshComponent *AProceduralRoomActor::SpawnCubeAt(const FVector &LocalPos, const FRotator &Rot)
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
	return Comp;
}

void AProceduralRoomActor::ApplyMaterialTo(UStaticMeshComponent *Comp)
{
	if (!Comp)
		return;

	UMaterialInstanceDynamic *DynMat = Comp->CreateAndSetMaterialInstanceDynamic(0);
	if (DynMat)
		DynMat->SetVectorParameterValue("BaseColor", RoomColor);
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
