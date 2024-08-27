
#include "IndicatorManagerSubsystem.h"
#include "TargetIndicatorLogChannels.h"
#include "IndicatorWidget.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(IndicatorManagerSubsystem)

bool UIndicatorManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
    return true;
}

void UIndicatorManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTargetIndicator, Warning, TEXT("%s is Initialized."), *GetNameSafe(this));
}

void UIndicatorManagerSubsystem::Deinitialize()
{
    Super::Deinitialize();
    UE_LOG(LogTargetIndicator, Warning, TEXT("%s is Deinitialized."), *GetNameSafe(this));
    RemoveAllIndicator();
}

void UIndicatorManagerSubsystem::StartIndicator(const TArray<AActor*>& TargetActors, TSubclassOf<UIndicatorWidget> DefaultWidgetClass)
{
    check(DefaultWidgetClass);
    if(TargetActors.IsEmpty())
    {
        return;
    }

    if(APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        for(AActor* Actor : TargetActors)
        {
            if(Widgets.Contains(Actor))continue;
            UIndicatorWidget* WidgetInstance = CreateWidget<UIndicatorWidget>(PC, DefaultWidgetClass);
            WidgetInstance->TargetActor = Actor;
            Widgets.Emplace(Actor, WidgetInstance);
            if(!WidgetInstance->IsInViewport())
            {
                WidgetInstance->AddToViewport();
                UE_LOG(LogTargetIndicator, Warning, TEXT("%s and %s is Pushed."), *GetNameSafe(Actor), *GetNameSafe(WidgetInstance));
            }

        }        
    }
}

void UIndicatorManagerSubsystem::RemoveIndicator(AActor* TargetActor)
{
    if(Widgets.Contains(TargetActor))
    {
        if(UUserWidget* IndicatorWidget = Widgets[TargetActor])
        {
            if(IndicatorWidget->IsInViewport())
            {
                IndicatorWidget->RemoveFromParent();
                Widgets[TargetActor] = nullptr;
                Widgets.Remove(TargetActor);
                UE_LOG(LogTargetIndicator, Warning, TEXT("%s is Removed."), *GetNameSafe(TargetActor));
            }
        }
    }
}
void UIndicatorManagerSubsystem::RemoveAllIndicator()
{
    for(auto& [TargetActor, Widget] : Widgets)
    {
        if(Widget->IsInViewport())
        {
            Widget->RemoveFromParent();
        }
    }
    Widgets.Reset();
}
