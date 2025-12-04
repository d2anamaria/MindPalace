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

// Theme + Concept includes
#include "../Themes/EThemeType.h"
#include "ProceduralMaterials/ConceptTypes.h"
#include "ProceduralMaterials/ConceptFactory.h"
#include "ProceduralMaterials/MaterialParamApplier.h"

void UAnchorSpawner::Init(AProceduralRoomActor *InOwner)
{
    Owner = InOwner;
    LoadAssets();
}

void UAnchorSpawner::LoadAssets()
{
    AnchorAssetPool.Empty();

    // Load the ONE anchor mesh you actually have
    UStaticMesh *BaseMesh = LoadObject<UStaticMesh>(nullptr,
                                                    TEXT("/Game/Assets/Anchors/SphereDisplay.SphereDisplay"));
    UStaticMesh *BaseMeshTranslucent = LoadObject<UStaticMesh>(nullptr,
                                                               TEXT("/Game/Assets/Anchors/SphereDisplayTranslucent.SphereDisplayTranslucent"));

    if (!BaseMesh || !BaseMeshTranslucent)
    {
        UE_LOG(LogTemp, Error, TEXT("FAILED to load SphereDisplay mesh! No anchors will spawn."));
        return;
    }

    // // 1) Opaque vs Transparent
    // {
    //     FAnchorAssetEntry Entry;
    //     Entry.Mesh = BaseMeshTranslucent;
    //     Entry.LeftConcept = EConceptType::Opaque;
    //     Entry.RightConcept = EConceptType::Transparent;
    //     AnchorAssetPool.Add(Entry);
    // }

    // 2) Metallic vs Diffuse
    {
        FAnchorAssetEntry Entry;
        Entry.Mesh = BaseMesh;
        Entry.LeftConcept = EConceptType::Metallic;
        Entry.RightConcept = EConceptType::Diffuse;
        AnchorAssetPool.Add(Entry);
    }

    // 3) Reflective vs Matte
    {
        FAnchorAssetEntry Entry;
        Entry.Mesh = BaseMesh;
        Entry.LeftConcept = EConceptType::Reflective;
        Entry.RightConcept = EConceptType::Matte;
        AnchorAssetPool.Add(Entry);
    }

    // // 4) Phong vs Blinn
    // {
    //     FAnchorAssetEntry Entry;
    //     Entry.Mesh = BaseMesh;
    //     Entry.LeftConcept = EConceptType::Phong;
    //     Entry.RightConcept = EConceptType::Blinn;
    //     AnchorAssetPool.Add(Entry);
    // }

    // // 5) Emissive vs Dark
    // {
    //     FAnchorAssetEntry Entry;
    //     Entry.Mesh = BaseMesh;
    //     Entry.LeftConcept = EConceptType::Emissive;
    //     Entry.RightConcept = EConceptType::Dark;
    //     AnchorAssetPool.Add(Entry);
    // }

    // // 6) NormalBumpy vs NormalFlat
    // {
    //     FAnchorAssetEntry Entry;
    //     Entry.Mesh = BaseMesh;
    //     Entry.LeftConcept = EConceptType::NormalBumpy;
    //     Entry.RightConcept = EConceptType::NormalFlat;
    //     AnchorAssetPool.Add(Entry);
    // }

    // // 7) Glass vs Plastic
    // {
    //     FAnchorAssetEntry Entry;
    //     Entry.Mesh = BaseMeshTranslucent;
    //     Entry.LeftConcept = EConceptType::Glass;
    //     Entry.RightConcept = EConceptType::Plastic;
    //     AnchorAssetPool.Add(Entry);
    // }
}

FAnchorAssetEntry UAnchorSpawner::GetRandomAsset()
{
    FAnchorAssetEntry Empty;

    if (AnchorAssetPool.Num() == 0)
        return Empty;

    int32 RandIdx = FMath::RandRange(0, AnchorAssetPool.Num() - 1);

    FAnchorAssetEntry Chosen = AnchorAssetPool[RandIdx];
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
        float Margin = Cube * 3.0f;
        float RandX = FMath::RandRange(Margin, MaxX - Margin);
        float RandY = FMath::RandRange(Margin, MaxY - Margin);
        float RandZ = FMath::RandRange(0.f, Owner->AnchorMaxHeight);

        FVector Pos(RandX, RandY, RandZ);
        FRotator Rot(0.f, FMath::RandRange(0.f, 360.f), 0.f);

        FAnchorAssetEntry Asset = GetRandomAsset();

        UStaticMesh *MeshToUse = Asset.Mesh;

        // These two determine the concepts for slots 1 and 3
        EConceptType ConceptLeft = Asset.LeftConcept;
        EConceptType ConceptRight = Asset.RightConcept;

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

        Owner->RegisterSpawned(Trigger);

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

        // ===========================================================
        // APPLY THEME + CONCEPTS
        // ===========================================================
        for (int32 Slot = 0; Slot < Comp->GetNumMaterials(); Slot++)
        {
            UMaterialInstanceDynamic *DynMat = Comp->CreateAndSetMaterialInstanceDynamic(Slot);
            if (!DynMat)
                continue;

            // 1) Apply Theme preset from room
            MaterialParamApplier::ApplyThemePreset(DynMat, Owner->ActiveTheme);

            // 2) Apply Concept only to sphere slots (1 and 3)
            if (Slot == 1)
            {
                FConceptData DataLeft = ConceptFactory::Create(ConceptLeft);
                MaterialParamApplier::ApplyConcept(DynMat, DataLeft);
            }
            else if (Slot == 3)
            {
                FConceptData DataRight = ConceptFactory::Create(ConceptRight);
                MaterialParamApplier::ApplyConcept(DynMat, DataRight);
            }
        }
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
