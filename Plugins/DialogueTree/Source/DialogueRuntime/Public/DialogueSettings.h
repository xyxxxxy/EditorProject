
#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DialogueSettings.generated.h"

class ADialogueController;

UCLASS(Config = Engine,DefaultConfig,meta = (DisplayName = "Dialogue Tree"))
class DIALOGUERUNTIME_API UDialogueSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	const FString DefaultControllerCoords = "/Script/Engine.Blueprint'/DialogueTree/Blueprints/Controllers/BP_BasicDialogueController.BP_BasicDialogueController_C'";
	
public:
	UPROPERTY(Config,EditAnywhere,BlueprintReadOnly,Category = "Dialogue")
	TSubclassOf<ADialogueController> DialogueControllerType = LoadClass<ADialogueController>(nullptr,*DefaultControllerCoords);
	
};
