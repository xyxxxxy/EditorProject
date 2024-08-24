
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IndicatorWidget.generated.h"

UCLASS()
class UIndicatorWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UIndicatorWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};