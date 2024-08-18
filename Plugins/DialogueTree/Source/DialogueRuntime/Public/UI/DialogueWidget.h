
#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SpeechDetails.h"
#include "DialogueController.h"
#include "DialogueInterface.h"
#include "DialogueWidget.generated.h"

class URichTextBlock;
UENUM(BlueprintType)
enum class EDialogueWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogueControllerDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateStart, FSpeechDetails, SpeechDetails);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateEnd);

UCLASS()
class UDialogueWidget : public UCommonActivatableWidget, public IDialogueInterface
{
	GENERATED_BODY()

public:
	UDialogueWidget(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UCommonActivatableWidget interface
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	//~End of UCommonActivatableWidget interface


public:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

	virtual void NativeOnActivated() override;

public:
	virtual void SetController_Implementation(ADialogueController* InController) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget = true), Category = "Dialogue")
	URichTextBlock* DialogueTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = "Dialogue")
	FText OriginText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float DisplayRate = 1.0f;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FDialogueControllerDelegate OnDialogueStart;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FDialogueControllerDelegate OnDialogueEnd;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnStateStart OnStateStart;

	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FOnStateEnd OnCurrentStateEnd;

	

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<ADialogueController> DialogueController;

	FTimerHandle DisplayTextTimerHandle;
	FTimerDelegate DisplayTextTimerDelegate;

protected:
	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category=Input)
	EDialogueWidgetInputMode InputMode = EDialogueWidgetInputMode::Default;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ForwardInputActionData;

	FUIActionBindingHandle ForwardHandle;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;

	FUIActionBindingHandle BackHandle;
	
private:
	// TODO: Press 'X' for Auto Dialogue and Press 'A' to reach end of sentence.

	UFUNCTION()
	void StartNewState(FSpeechDetails InDetails);
	
	UFUNCTION()
	void PreSwitchToNextState();

	void OnSwitchToNextState();

	void DisplayEntireState();

	void DisplayDialogue();
	void EndDialogue();
	void FindFirstIndexAfterRich();
	void SetDisplayRate(float InRate);

	void Forward();
	void Back();
	
private:
	int32 StrIndex = 0;
	FString CurrentStr;
	FString OriginStr;
	bool bHasEntered = false;
	bool bEntireState = false;
};