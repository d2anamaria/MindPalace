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

    // ------------------------------
    // BOTTOM & TOP EDGES
    // ------------------------------
    for (int32 x = 0; x < Width; x++)
    {
        for (int32 h = 0; h < Height; h++)
        {
            // Skip cubes at window positions (bottom/front wall)
            if (HandleWindowIfAny(Owner, x, 0, h, CubeSize, Width, Length))
                continue;
            ;

            FVector Bottom(x * CubeSize, 0.f, h * CubeSize);
            UStaticMeshComponent *C1 = Owner->SpawnCubeAt(Bottom, FRotator::ZeroRotator);
            if (C1)
            {
                Owner->ApplyMaterialTo(C1);
                int32 UIndex = x; // FRONT: no flip
                Owner->ApplyUVParams(C1, UIndex, h, Width, Height);
            }

            // Opposite wall (top edge)
            if (Length > 1)
            {
                if (HandleWindowIfAny(Owner, x, Length - 1, h, CubeSize, Width, Length))
                    continue;
                FVector Top(x * CubeSize, (Length - 1) * CubeSize, h * CubeSize);
                UStaticMeshComponent *C2 = Owner->SpawnCubeAt(Top, FRotator::ZeroRotator);
                if (C2)
                {
                    Owner->ApplyMaterialTo(C2);
                    int32 UIndex = (Width - 1) - x; // BACK: flip
                    Owner->ApplyUVParams(C2, UIndex, h, Width, Height);
                }
            }
        }
    }

    // ------------------------------
    // LEFT & RIGHT EDGES
    // ------------------------------
    for (int32 y = 0; y < Length; y++)
    {
        for (int32 h = 0; h < Height; h++)
        {

            // LEFT WALL  (X = 0)
            if (HandleWindowIfAny(Owner, 0, y, h, CubeSize, Width, Length))
                continue;

            FVector Left(0.f, y * CubeSize, h * CubeSize);
            UStaticMeshComponent *L = Owner->SpawnCubeAt(Left, FRotator::ZeroRotator);
            if (L)
            {
                Owner->ApplyMaterialTo(L);
                int32 UIndex = y;                                   // LEFT: horizontal is y
                Owner->ApplyUVParams(L, UIndex, h, Length, Height); // NOTE: use Length
            }

            // Opposite wall (right edge)
            if (Width > 1)
            {
                if (HandleWindowIfAny(Owner, Width - 1, y, h, CubeSize, Width, Length))
                    continue;

                FVector Right((Width - 1) * CubeSize, y * CubeSize, h * CubeSize);
                UStaticMeshComponent *R = Owner->SpawnCubeAt(Right, FRotator::ZeroRotator);
                if (R)
                {
                    Owner->ApplyMaterialTo(R);
                    int32 UIndex = (Length - 1) - y; // RIGHT: flip
                    Owner->ApplyUVParams(R, UIndex, h, Length, Height);
                }
            }
        }
    }
}
