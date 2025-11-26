#include "CircularWindowStrategy.h"
#include "../ProceduralRoomActor.h"
#include "Components/StaticMeshComponent.h"

// Load mesh once
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

    int32 X = FMath::RoundToInt(LocalPos.X / Cube);
    int32 Y = FMath::RoundToInt(LocalPos.Y / Cube);
    int32 H = FMath::RoundToInt(LocalPos.Z / Cube);

    int32 Width = Owner->Months;
    int32 Length = Owner->bIsRectangle ? Owner->Months * 2 : Owner->Months;

    // Identify wall
    bool IsFront = (Y == 0);
    bool IsBack = (Y == Length - 1);
    bool IsLeft = (X == 0);
    bool IsRight = (X == Width - 1);

    // Check solid neighbor cubes
    auto IsSolid = [&](int32 NX, int32 NY, int32 NH)
    {
        return !Owner->ShouldHaveWindowAt(
            NX, NY, NH,
            FVector(Width / 2.f, Length / 2.f, 0.f));
    };

    bool UL = false, UR = false, LL = false, LR = false;

    if (IsFront || IsBack)
    {
        // FRONT/BACK walls → horizontal neighbors along X-axis
        UL = IsSolid(X - 1, Y, H) && IsSolid(X, Y, H + 1);
        UR = IsSolid(X + 1, Y, H) && IsSolid(X, Y, H + 1);
        LL = IsSolid(X - 1, Y, H) && IsSolid(X, Y, H - 1);
        LR = IsSolid(X + 1, Y, H) && IsSolid(X, Y, H - 1);
    }
    else if (IsLeft || IsRight)
    {
        // LEFT/RIGHT walls → horizontal neighbors along Y-axis
        UL = IsSolid(X, Y - 1, H) && IsSolid(X, Y, H + 1);
        UR = IsSolid(X, Y + 1, H) && IsSolid(X, Y, H + 1);
        LL = IsSolid(X, Y - 1, H) && IsSolid(X, Y, H - 1);
        LR = IsSolid(X, Y + 1, H) && IsSolid(X, Y, H - 1);
    }
    // else
    // {
    //     return; // Not a wall
    // }

    if (!UL && !UR && !LL && !LR)
        return;

    float Pitch = 0.f;

    if (UL)
        Pitch = 90.f;
    if (UR)
        Pitch = 0.f;
    if (LL)
        Pitch = 180.f;
    if (LR)
        Pitch = 270.f;

    float WallYaw = 0.f;

    if (IsFront)
        WallYaw = 0.f; // +Y
    if (IsBack)
        WallYaw = 180.f; // -Y
    if (IsLeft)
        WallYaw = 90.f; // -X
    if (IsRight)
        WallYaw = -90.f; // +X

    UStaticMeshComponent *C = NewObject<UStaticMeshComponent>(Owner);
    C->SetStaticMesh(Mesh);
    C->SetupAttachment(Owner->GetRootComponent());
    C->RegisterComponent();
    Owner->RegisterSpawned(C);

    C->SetRelativeLocation(LocalPos);

    C->SetRelativeRotation(FRotator(Pitch, WallYaw, -90.f));
}
