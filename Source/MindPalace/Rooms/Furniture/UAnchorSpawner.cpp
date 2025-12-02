#include "UAnchorSpawner.h"
#include "../ProceduralRoomActor.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

void UAnchorSpawner::Init(AProceduralRoomActor *InOwner)
{
    Owner = InOwner;
    LoadAssets();
}

void UAnchorSpawner::LoadAssets()
{
    UStaticMesh *Mesh = LoadObject<UStaticMesh>(
        nullptr,
        TEXT("/Game/Assets/Anchors/SphereDisplay.SphereDisplay"));

    if (Mesh)
        AnchorAssetPool.Add(Mesh);
}

UStaticMesh *UAnchorSpawner::GetRandomAsset() const
{
    if (AnchorAssetPool.Num() == 0)
        return nullptr;

    int32 RandIdx = FMath::RandRange(0, AnchorAssetPool.Num() - 1);
    return AnchorAssetPool[RandIdx];
}

void UAnchorSpawner::BuildAnchors(int32 WidthCubes, int32 LengthCubes)
{
    if (!Owner)
        return;

    float Cube = Owner->CubeSize;
    float MaxX = (WidthCubes - 1) * Cube;
    float MaxY = (LengthCubes - 1) * Cube;

    for (int32 i = 0; i < Owner->RandomAnchorsCount; i++)
    {
        float Margin = Cube * 0.5f;
        float RandX = FMath::RandRange(Margin, MaxX - Margin);
        float RandY = FMath::RandRange(Margin, MaxY - Margin);
        float RandZ = FMath::RandRange(0.f, Owner->AnchorMaxHeight);

        FVector Pos(RandX, RandY, RandZ);
        FRotator Rot(0.f, FMath::RandRange(0.f, 360.f), 0.f);

        UStaticMesh *MeshToUse = GetRandomAsset();
        if (!MeshToUse)
            MeshToUse = Owner->GetCubeMesh();

        UStaticMeshComponent *Comp = NewObject<UStaticMeshComponent>(Owner);
        Comp->SetStaticMesh(MeshToUse);
        Comp->SetupAttachment(Owner->GetRootComponent());
        Comp->RegisterComponent();
        Comp->SetRelativeLocation(Pos);
        Comp->SetRelativeRotation(Rot);

        Owner->RegisterSpawned(Comp);
    }
}
