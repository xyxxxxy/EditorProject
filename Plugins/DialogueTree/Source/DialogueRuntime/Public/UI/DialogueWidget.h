
#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SpeechDetails.h"
#include "DialogueController.h"
#include "DialogueWidget.generated.h"

class URichTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateStart, FSpeechDetails, SpeechDetails);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateEnd);

UCLASS()
class UDialogueWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget = true), Category = "Dialogue")
	URichTextBlock* DialogueTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Dialogue")
	FText OriginText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float DisplayRate = 0.2f;
	
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnStateStart OnStateStart;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnStateEnd OnCurrentStateEnd;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetController(ADialogueController* InDialogueController);

	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();

protected:
	UPROPERTY()
	TObjectPtr<ADialogueController> DialogueController;

	FTimerHandle DisplayTextTimerHandle;
	FTimerDelegate DisplayTextTimerDelegate;
	
private:
	// TODO: Press 'X' for Auto Dialogue and Press 'A' to reach end of sentence.

	UFUNCTION()
	void StartNewState(FSpeechDetails InDetails);
	
	void PreSwitchToNextState();
	void OnSwitchToNextState();
	void DisplayEntireState();
	void DisplayDialogue();
	void EndDialogue();
	void FindFirstIndexAfterRich();
	void SetDisplayRate(float InRate);
	
	int32 StrIndex = 0;
	FString CurrentStr;
	FString OriginStr;
	bool bHasEntered = false;
	bool bEntireState = false;
};