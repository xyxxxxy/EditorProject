
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IndicatorWidget.generated.h"

class UIndicatorManagerSubsystem;
class UTextBlock;
class UOverlay;

UCLASS()
class UIndicatorWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    UIndicatorWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

public:

    UPROPERTY(EditAnyWhere, meta = (BindWidget = true), Category = "Indicator")
    TObjectPtr<UOverlay> TargetIndicator;

    UPROPERTY(EditAnyWhere, meta = (BindWidget = true), Category = "Indicator")
    TObjectPtr<UTextBlock> DistanceTextBlock;

    UPROPERTY(BlueprintReadOnly, Category = "Indicator")
    TObjectPtr<AActor> TargetActor;

protected:

    UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Get Overlay"),Category = "Indicator")
    UOverlay* BP_GetOverlay() const;

    void UpdateIndicator();
    void UpdateDistance();
    void UpdatePosition();
    void SetRelativePosition();
    void SetPrimitivePosition();
    void SetClippingPosition();

    void Test();

    UPROPERTY()
    TObjectPtr<UIndicatorManagerSubsystem> IndicatorManagerSubsystem;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Indicator")
    float UpdateFrequency = 0.2f;

    UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Indicator")
    FVector TargetOffset;   

    UPROPERTY(BlueprintReadWrite,Category = "Indicator")
    float Distance = 0.0f;

    FVector2D RelativePosition = FVector2D::ZeroVector;
    FVector2D PrimitivePosition = FVector2D::ZeroVector;
    FVector2D ClippingPosition = FVector2D::ZeroVector;

    FVector2D DefaultWindowsSize = FVector2D::ZeroVector;

private:
    FTimerHandle IndicatorTimerHandle;
    FTimerDelegate IndicatorTimerDelegate;
};