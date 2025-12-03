#include "UAnchorSpawner.h"
#include "../ProceduralRoomActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

// REQUIRED for FX
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

// REQUIRED for OnClicked signature
#include "Components/PrimitiveComponent.h"

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

    Remember_PixieDust = LoadObject<UNiagaraSystem>(
        nullptr,
        TEXT("/Game/FX/Remember.Remember"));
}

UStaticMesh *UAnchorSpawner::GetRandomAsset()
{
    if (AnchorAssetPool.Num() == 0)
        return nullptr;

    int32 RandIdx = FMath::RandRange(0, AnchorAssetPool.Num() - 1);
    UStaticMesh *Chosen = AnchorAssetPool[RandIdx];

    // Remove from pool so it can't be chosen again
    AnchorAssetPool.RemoveAt(RandIdx);

    return Chosen;
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

        // ==============================================
        // Create Anchor Mesh Component
        // ==============================================
        UStaticMeshComponent *Comp = NewObject<UStaticMeshComponent>(Owner);
        Comp->SetStaticMesh(MeshToUse);
        Comp->SetupAttachment(Owner->GetRootComponent());
        Comp->RegisterComponent();
        Comp->SetRelativeLocation(Pos);
        Comp->SetRelativeRotation(Rot);

        // Enable clicking
        Comp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        Comp->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
        Comp->OnClicked.AddDynamic(this, &UAnchorSpawner::OnAnchorClicked);

        // ------------------------------
        // Create an overlap trigger around the anchor
        // ------------------------------
        USphereComponent *Trigger = NewObject<USphereComponent>(Owner);
        Trigger->InitSphereRadius(Comp->Bounds.SphereRadius * 1.2f);
        Trigger->SetupAttachment(Comp);
        Trigger->SetRelativeLocation(FVector::ZeroVector);
        Trigger->RegisterComponent();

        // Only overlap with Pawn
        Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
        Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
        Trigger->SetGenerateOverlapEvents(true);

        // Bind overlap
        Trigger->OnComponentBeginOverlap.AddDynamic(this, &UAnchorSpawner::OnAnchorOverlap);

        // ==============================================
        // Add Remember_PixieDust FX
        // ==============================================
        if (Remember_PixieDust)
        {
            UNiagaraComponent *FXComp = NewObject<UNiagaraComponent>(Owner);
            FXComp->SetAsset(Remember_PixieDust);
            FXComp->SetupAttachment(Comp);
            FXComp->RegisterComponent();
            FXComp->SetRelativeLocation(FVector::ZeroVector);

            //---- Make FX take the shape of the anchor ---- can't make it work properly now
            // FXComp->SetVariableStaticMesh(TEXT("User_Mesh"), MeshToUse);

            const float BaseRadiusInNiagara = 100.f;
            float MeshRadius = Comp->Bounds.SphereRadius;

            float ScaleFactor = MeshRadius / BaseRadiusInNiagara;
            FXComp->SetRelativeScale3D(FVector(ScaleFactor));

            // // ---- Adjust spawn rate based on mesh size ---- can't make it work properly now
            // float BaseRate = 50.f; // float, not int
            // float AreaFactor = ScaleFactor * ScaleFactor;

            // float SpawnRate = FMath::Clamp(
            //     BaseRate * AreaFactor,
            //     10.f,
            //     500.f);

            // // Niagara param: User.SpawnRate (float)
            // FXComp->SetVariableFloat(TEXT("User.SpawnRate"), SpawnRate);

            Owner->RegisterSpawned(FXComp);
        }
        Owner->RegisterSpawned(Comp);
    }
}

void UAnchorSpawner::OnAnchorClicked(UPrimitiveComponent *ClickedComp, FKey ButtonPressed)
{
    DisableDust(ClickedComp);

    if (Owner)
    {
        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, FName("RestoreDust"), ClickedComp);

        FTimerHandle Handle;
        Owner->GetWorldTimerManager().SetTimer(Handle, Delegate, 5.f, false);
    }
}

void UAnchorSpawner::OnAnchorOverlap(
    UPrimitiveComponent *OverlappedComp,
    AActor *OtherActor,
    UPrimitiveComponent *OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult &SweepResult)
{
    // Only react to the player
    if (!OtherActor || !OtherActor->IsA(APawn::StaticClass()))
        return;

    // OverlappedComp is the sphere trigger ... get its parent (the mesh anchor)
    UPrimitiveComponent *AnchorComp =
        Cast<UPrimitiveComponent>(OverlappedComp->GetAttachParent());

    if (!AnchorComp)
        return;

    DisableDust(AnchorComp);

    if (Owner)
    {
        FTimerDelegate Delegate;
        Delegate.BindUFunction(this, FName("RestoreDust"), AnchorComp);

        FTimerHandle Handle;
        Owner->GetWorldTimerManager().SetTimer(Handle, Delegate, 5.f, false);
    }
}

void UAnchorSpawner::DisableAnchorAndFX(UPrimitiveComponent *AnchorComp)
{
    if (!AnchorComp)
        return;

    AnchorComp->SetVisibility(false, true);
    AnchorComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    TArray<USceneComponent *> Children;
    AnchorComp->GetChildrenComponents(true, Children);

    for (USceneComponent *Child : Children)
    {
        if (UNiagaraComponent *FX = Cast<UNiagaraComponent>(Child))
        {
            FX->DeactivateImmediate();
            FX->DestroyComponent();
        }
    }
}

void UAnchorSpawner::DisableDust(UPrimitiveComponent *AnchorComp)
{
    if (!AnchorComp)
        return;

    TArray<USceneComponent *> Children;
    AnchorComp->GetChildrenComponents(true, Children);

    for (USceneComponent *Child : Children)
    {
        if (UNiagaraComponent *FX = Cast<UNiagaraComponent>(Child))
        {
            FX->DeactivateImmediate();
        }
    }
}

void UAnchorSpawner::RestoreDust(UPrimitiveComponent *AnchorComp)
{
    if (!AnchorComp)
        return;

    TArray<USceneComponent *> Children;
    AnchorComp->GetChildrenComponents(true, Children);

    for (USceneComponent *Child : Children)
    {
        if (UNiagaraComponent *FX = Cast<UNiagaraComponent>(Child))
        {
            FX->Activate(true);
        }
    }
}
