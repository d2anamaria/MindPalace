#pragma once
#include "ConceptTypes.h"
#include "ConceptData.h"

class ConceptFactory
{
public:
    static FConceptData Create(EConceptType Type)
    {
        FConceptData Data;

        switch (Type)
        {
        // ----------------------------------------------------
        //  OPACITY (Opaque vs Transparent)
        // ----------------------------------------------------
        case EConceptType::Opaque:
            Data.OpacityLevel = 1.0f;
            break;

        case EConceptType::Transparent:
            Data.OpacityLevel = 0.2f;
            break;

        // ----------------------------------------------------
        //  METAL / DIFFUSE
        // ----------------------------------------------------
        case EConceptType::Metallic:
            Data.MetallicLevel = 1.0f;
            Data.RoughnessLevel = 0.2f;
            break;

        case EConceptType::Diffuse:
            Data.MetallicLevel = 0.0f;
            Data.RoughnessLevel = 0.8f;
            break;

        // ----------------------------------------------------
        //  SMOOTH / ROUGH TEXTURE
        // ----------------------------------------------------
        case EConceptType::Smooth:
            Data.RoughnessLevel = 0.1f;
            break;

        case EConceptType::Rough:
            Data.RoughnessLevel = 1.0f;
            break;

        // ----------------------------------------------------
        //  REFLECTIVE / MATTE
        // ----------------------------------------------------
        case EConceptType::Reflective:
            Data.ReflectionStrength = 1.0f;
            Data.RoughnessLevel = 0.1f;
            break;

        case EConceptType::Matte:
            Data.ReflectionStrength = 0.0f;
            Data.RoughnessLevel = 1.0f;
            break;

        // ----------------------------------------------------
        //  EMISSIVE / DARK
        // ----------------------------------------------------
        case EConceptType::Emissive:
            Data.EmissionStrength = 3.0f;
            Data.Brightness = 2.0f; // optional boost
            break;

        case EConceptType::Dark:
            Data.EmissionStrength = 0.0f;
            Data.Brightness = 0.5f;
            break;

        // ----------------------------------------------------
        //  GLASS / PLASTIC
        // ----------------------------------------------------
        case EConceptType::Glass:
            Data.OpacityLevel = 0.3f;
            Data.RefractionAmount = 0.2f;
            Data.ReflectionStrength = 0.6f;
            Data.RoughnessLevel = 0.05f;
            break;

        case EConceptType::Plastic:
            Data.MetallicLevel = 0.0f;
            Data.RoughnessLevel = 0.5f;
            Data.ReflectionStrength = 0.2f;
            break;

        // ----------------------------------------------------
        //  SPECULAR MODELS
        // ----------------------------------------------------
        case EConceptType::Phong:
            Data.ReflectionStrength = 0.7f;
            Data.RoughnessLevel = 0.2f;
            break;

        case EConceptType::Blinn:
            Data.ReflectionStrength = 1.0f;
            Data.RoughnessLevel = 0.05f;
            break;

        // ----------------------------------------------------
        //  NORMALS
        // ----------------------------------------------------
        case EConceptType::NormalBumpy:
            Data.NormalStrength = 1.0f;
            break;

        case EConceptType::NormalFlat:
            Data.NormalStrength = 0.0f;
            break;
        }

        return Data;
    }
};
