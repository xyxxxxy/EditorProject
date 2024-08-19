
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
	virtual void NativeOnInitialized() override;

	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;

public:
	virtual void SetController_Implementation(ADialogueController* InController) override;

private:
	// TODO: Press 'X' for Auto Dialogue and Press 'A' to reach end of sentence.

	UFUNCTION()
	void StartNewStatement(FSpeechDetails InDetails);
	
	UFUNCTION()
	void PreSwitchToNextStatement();

	void OnSwitchToNextStatement();
	void DisplayEntireStatement();

	void DisplayDialogue();
	void FindFirstIndexAfterRich();

	void InitialValuables();

	void Forward();
	void Back();

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	TObjectPtr<ADialogueController> DialogueController;

	FTimerHandle DisplayTextTimerHandle;
	FTimerDelegate DisplayTextTimerDelegate;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget = true), Category = "Dialogue")
	URichTextBlock* DialogueTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	FText OriginText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	float DisplayRate = 0.2f;

protected:
	/** The desired input mode to use while this UI is activated, for example do you want key presses to still reach the game/player controller? */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EDialogueWidgetInputMode InputMode = EDialogueWidgetInputMode::Default;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle ForwardInputActionData;

	FUIActionBindingHandle ForwardHandle;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle BackInputActionData;

	FUIActionBindingHandle BackHandle;
	
private:
	int32 StrIndex = 0;
	FString CurrentStr;
	FString OriginStr;
	bool bHasEntered = false;
	bool bEntireStatement = false;
};