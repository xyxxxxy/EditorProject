
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueWidget.generated.h"

class URichTextBlock;

UCLASS()
class UDialogueWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget = true), Category = "Dialogue")
	URichTextBlock* DialogueTextBlock;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ExposeOnSpawn = true),Category = "Dialogue")
	FText OriginText;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Dialogue")
	float DisplayRate = 0.2f;
	
protected:
	FTimerHandle DisplayTextTimerHandle;
	FTimerDelegate DisplayTextTimerDelegate;
	
private:
	// TODO: Press 'X' for Auto Dialogue and Press 'A' to reach end of sentence.
	void DisplayDialogue();
	void EndDialogue();
	void FindFirstIndexAfterRich();
	void SetDisplayRate(float InRate);
	
	int32 StrIndex = 0;
	FString CurrentStr;
	FString OriginStr;
	bool bHasEntered = false;
	bool bIsEnd = false;
};