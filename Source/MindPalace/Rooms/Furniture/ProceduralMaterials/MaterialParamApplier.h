#pragma once
#include "CoreMinimal.h"
#include "../../Themes/ThemeData.h"
#include "ConceptData.h"

class MaterialParamApplier
{
public:
    static void ApplyThemePreset(UMaterialInstanceDynamic *Mat, const FThemeData &T)
    {
        if (!Mat)
            return;

        Mat->SetVectorParameterValue("TintColor", T.TintColor);
        Mat->SetScalarParameterValue("Brightness", T.Brightness);
        Mat->SetScalarParameterValue("Saturation", T.Saturation);
        Mat->SetScalarParameterValue("RoughnessBias", T.RoughnessBias);
        Mat->SetScalarParameterValue("MetalnessBias", T.MetalnessBias);
        Mat->SetScalarParameterValue("EmissionBoost", T.EmissionBoost);
    }

    // -----------------------------------------------------
    // Apply concept values (USED FOR SPHERE ANCHORS)
    // -----------------------------------------------------
    static void ApplyConcept(UMaterialInstanceDynamic *Mat, const FConceptData &Data)
    {
        if (!Mat)
            return;

        Mat->SetScalarParameterValue("OpacityLevel", Data.OpacityLevel);
        Mat->SetScalarParameterValue("MetallicLevel", Data.MetallicLevel);
        Mat->SetScalarParameterValue("RoughnessLevel", Data.RoughnessLevel);
        Mat->SetScalarParameterValue("ReflectionStrength", Data.ReflectionStrength);
        Mat->SetScalarParameterValue("EmissionStrength", Data.EmissionStrength);
        Mat->SetScalarParameterValue("RefractionAmount", Data.RefractionAmount);
        Mat->SetScalarParameterValue("NormalStrength", Data.NormalStrength);
    }
};
