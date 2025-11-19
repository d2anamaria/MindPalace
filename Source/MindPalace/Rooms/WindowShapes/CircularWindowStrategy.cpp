#include "CircularWindowStrategy.h"
#include "../ProceduralRoomActor.h"
#include "Components/StaticMeshComponent.h"

static UStaticMesh *LoadMesh()
{
    static UStaticMesh *Mesh = nullptr;

    if (!Mesh)
    {
        Mesh = LoadObject<UStaticMesh>(
            nullptr,
            TEXT("/Game/Assets/Windows/Corner.Corner"));
    }

    return Mesh;
}

bool UCircularWindowStrategy::IsWindowAt_Implementation(
    int32 X, int32 Y, int32 H,
    FVector RoomCenter) const
{
    const AProceduralRoomActor *Owner = Cast<AProceduralRoomActor>(GetOuter());
    return Owner && Owner->ShouldHaveWindowAt(X, Y, H, RoomCenter);
}

void UCircularWindowStrategy::SpawnOpening_Implementation(
    AProceduralRoomActor *Owner,
    const FVector &LocalPos,
    const FRotator &Rot) const
{
    if (!Owner)
        return;

    UStaticMesh *Mesh = LoadMesh();
    if (!Mesh)
        return;

    float Cube = Owner->GetCubeSize();
    int32 X = LocalPos.X / Cube;
    int32 Y = LocalPos.Y / Cube;
    int32 H = LocalPos.Z / Cube;

    auto IsSolid = [&](int32 NX, int32 NY, int32 NH)
    {
        return !Owner->ShouldHaveWindowAt(NX, NY, NH, FVector(0, 0, 0));
    };

    bool UL = IsSolid(X - 1, Y, H) && IsSolid(X, Y, H + 1);
    bool UR = IsSolid(X + 1, Y, H) && IsSolid(X, Y, H + 1);
    bool LL = IsSolid(X - 1, Y, H) && IsSolid(X, Y, H - 1);
    bool LR = IsSolid(X + 1, Y, H) && IsSolid(X, Y, H - 1);

    if (!UL && !UR && !LL && !LR)
        return;

    float Yaw = 0.f;
    if (UL)
        Yaw = 90.f;
    if (UR)
        Yaw = 0.f;
    if (LL)
        Yaw = 180.f;
    if (LR)
        Yaw = 270.f;

    UStaticMeshComponent *C = NewObject<UStaticMeshComponent>(Owner);
    C->SetStaticMesh(Mesh);
    C->SetupAttachment(Owner->GetRootComponent());
    C->RegisterComponent();
    Owner->RegisterSpawned(C);
    C->SetRelativeLocation(LocalPos);
    C->SetRelativeRotation(FRotator(Yaw, 0.f, -90.f));
}
