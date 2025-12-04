#pragma once

UENUM(BlueprintType)
enum class EConceptType : uint8
{
    Opaque,
    Transparent,
    Metallic,
    Diffuse,
    Smooth,
    Rough,
    Reflective,
    Matte,
    Emissive,
    Dark,
    Glass,
    Plastic,
    Phong,
    Blinn,
    NormalBumpy,
    NormalFlat
};
