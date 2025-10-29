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

void AProceduralRoomActor::ApplyShape()
{
	// Load different assets based on shape
	UStaticMesh *SelectedMesh = nullptr;

	switch (RoomShape)
	{
	case ERoomShape::Cube:
		SelectedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube.Cube"));
		break;
	case ERoomShape::HalfSphere:
		SelectedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Sphere.Sphere"));
		break;
	case ERoomShape::Cylinder:
		SelectedMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
		break;
	default:
		break;
	}

	if (SelectedMesh)
	{
		OuterMesh->SetStaticMesh(SelectedMesh);
		InnerMesh->SetStaticMesh(SelectedMesh);
	}
}

void AProceduralRoomActor::UpdateRoom()
{
	ApplyShape();

	int32 SafeMonths = FMath::Max(1, Months);
	float OuterScale = SafeMonths * 10.0f;
	float InnerScale = OuterScale * 0.85f;

	OuterMesh->SetWorldScale3D(FVector(OuterScale));

	if (RoomShape == ERoomShape::HalfSphere)
	{
		// Cut half by lowering the mesh halfway down (visual)
		OuterMesh->SetRelativeScale3D(FVector(1, 1, 0.5f));
	}

	InnerMesh->SetWorldScale3D(FVector(InnerScale));
	InnerMesh->SetRelativeLocation(FVector(0, 0, 0.5f));

	// Hide inner mesh to simulate subtraction
	InnerMesh->SetRenderInMainPass(false);

	if (UMaterialInstanceDynamic *DynMat = OuterMesh->CreateAndSetMaterialInstanceDynamic(0))
		DynMat->SetVectorParameterValue("BaseColor", RoomColor);
}

#if WITH_EDITOR
void AProceduralRoomActor::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UpdateRoom();
}
#endif
