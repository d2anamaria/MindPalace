#include "CubeRoomShape.h"
#include "../ProceduralRoomActor.h"
#include "Components/StaticMeshComponent.h"

void UCubeRoomShape::Build(AProceduralRoomActor *Owner)
{
    if (!Owner)
        return;

    float CubeSize = Owner->GetCubeSize();
    int32 Width = Owner->Months;
    int32 Length = Owner->bIsRectangle ? Owner->Months * 2 : Owner->Months;
    int32 Height = Owner->RoomHeightCubes;
    UStaticMesh *Mesh = Owner->GetCubeMesh();
    if (!Mesh)
        return;

    // bottom & top edges
    for (int32 x = 0; x < Width; x++)
    {
        for (int32 h = 0; h < Height; h++)
        {
            FVector Bottom(x * CubeSize, 0.f, h * CubeSize);
            UStaticMeshComponent *C1 = Owner->SpawnCubeAt(Bottom, FRotator::ZeroRotator);
            if (C1)
                Owner->ApplyMaterialTo(C1);

            if (Length > 1)
            {
                FVector Top(x * CubeSize, (Length - 1) * CubeSize, h * CubeSize);
                UStaticMeshComponent *C2 = Owner->SpawnCubeAt(Top, FRotator::ZeroRotator);
                if (C2)
                    Owner->ApplyMaterialTo(C2);
            }
        }
    }

    // left & right edges
    for (int32 y = 0; y < Length; y++)
    {
        for (int32 h = 0; h < Height; h++)
        {
            FVector Left(0.f, y * CubeSize, h * CubeSize);
            UStaticMeshComponent *L = Owner->SpawnCubeAt(Left, FRotator::ZeroRotator);
            if (L)
                Owner->ApplyMaterialTo(L);

            if (Width > 1)
            {
                FVector Right((Width - 1) * CubeSize, y * CubeSize, h * CubeSize);
                UStaticMeshComponent *R = Owner->SpawnCubeAt(Right, FRotator::ZeroRotator);
                if (R)
                    Owner->ApplyMaterialTo(R);
            }
        }
    }
}
