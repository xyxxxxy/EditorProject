
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogueInterface.generated.h"

class ADialogueController;
// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI)
class UDialogueInterface : public UInterface
{
	GENERATED_BODY()
};

class DIALOGUERUNTIME_API IDialogueInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Dialogue")
	void SetController(ADialogueController* InController);
};