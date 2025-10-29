#include "ProceduralRoomActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AProceduralRoomActor::AProceduralRoomActor()
{
	PrimaryActorTick.bCanEverTick = true;

	OuterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OuterMesh"));
	RootComponent = OuterMesh;

	InnerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InnerMesh"));
	InnerMesh->SetupAttachment(OuterMesh);

	// --- Load once here (safe)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereFinder(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderFinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));

	CubeMesh = CubeFinder.Succeeded() ? CubeFinder.Object : nullptr;
	SphereMesh = SphereFinder.Succeeded() ? SphereFinder.Object : nullptr;
	CylinderMesh = CylinderFinder.Succeeded() ? CylinderFinder.Object : nullptr;

	// Default mesh
	if (CubeMesh)
	{
		OuterMesh->SetStaticMesh(CubeMesh);
		InnerMesh->SetStaticMesh(CubeMesh);
	}

	Months = 1;
	RoomColor = FLinearColor::Red;
	RoomShape = ERoomShape::Cube;
}

void AProceduralRoomActor::BeginPlay()
{
	Super::BeginPlay();
	UpdateRoom();
}

void AProceduralRoomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateRoom();
}

void AProceduralRoomActor::UpdateRoom()
{
	// --- Pick mesh based on RoomShape
	UStaticMesh *ChosenMesh = nullptr;
	switch (RoomShape)
	{
	case ERoomShape::Cube:
		ChosenMesh = CubeMesh;
		break;
	case ERoomShape::Sphere:
		ChosenMesh = SphereMesh;
		break;
	case ERoomShape::Cylinder:
		ChosenMesh = CylinderMesh;
		break;
	case ERoomShape::HalfSphere:
		// Placeholder for your imported dome mesh
		ChosenMesh = SphereMesh;
		break;
	default:
		ChosenMesh = CubeMesh;
		break;
	}

	if (ChosenMesh)
	{
		OuterMesh->SetStaticMesh(ChosenMesh);
		InnerMesh->SetStaticMesh(ChosenMesh);
	}

	// --- Scale and color
	int32 SafeMonths = FMath::Max(1, Months);
	FVector NewScale(SafeMonths, SafeMonths, 1.0f);
	OuterMesh->SetWorldScale3D(NewScale);
	InnerMesh->SetWorldScale3D(NewScale);

	UMaterialInstanceDynamic *OuterDyn = OuterMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (OuterDyn)
		OuterDyn->SetVectorParameterValue("BaseColor", RoomColor);

	UMaterialInstanceDynamic *InnerDyn = InnerMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (InnerDyn)
		InnerDyn->SetVectorParameterValue("BaseColor", RoomColor);
}

#if WITH_EDITOR
void AProceduralRoomActor::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UpdateRoom();
}
#endif
