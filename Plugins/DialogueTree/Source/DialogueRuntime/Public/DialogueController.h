﻿
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dialogue.h"
#include "SpeechDetails.h"


#include "DialogueController.generated.h"

class UDialogue;
class UDialogueSpeakerComponent;
class UDialogueWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDialogueControllerDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDialogueControllerSpeechDelegate, FSpeechDetails, SpeechDetails);

USTRUCT(BlueprintType)
struct DIALOGUERUNTIME_API FDialogueNodeVisits
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	FName DialogueFName;

	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Dialogue")
	TSet<int32> VisitedNodeIndices;
};

USTRUCT(BlueprintType)
struct DIALOGUERUNTIME_API FDialogueRecords
{
	GENERATED_BODY()
	//..
	UPROPERTY(BlueprintReadOnly, SaveGame, Category = "Dialogue")
	TMap<FName, FDialogueNodeVisits> Records;
};

UCLASS(Abstract, notplaceable, BlueprintType, Blueprintable)
class DIALOGUERUNTIME_API ADialogueController : public AActor
{
	GENERATED_BODY()
	
public:	
	ADialogueController();

public:
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	virtual void SelectOption(int32 InOptionIndex) const;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	virtual TMap<FName, UDialogueSpeakerComponent*> GetSpeakers() const;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogueWithNames(UDialogue* InDialogue, TMap<FName, UDialogueSpeakerComponent*> InSpeakers);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue(UDialogue* InDialogue, TArray<UDialogueSpeakerComponent*> InSpeakers);
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void EndDialogue();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void Skip() const;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ClearNodeVisits();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetSpeaker(FName InName, UDialogueSpeakerComponent* InSpeaker);
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Dialogue")
	FDialogueRecords GetDialogueRecords() const;
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ClearDialogueRecords();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ImportDialogueRecords(FDialogueRecords InRecords);
	
	bool SpeakerInCurrentDialogue(UDialogueSpeakerComponent* TargetSpeaker) const;
	
	void MarkNodeVisited(UDialogue* TargetDialogue, int32 TargetNodeIndex);
	void MarkNodeUnvisited(UDialogue* TargetDialogue, int32 TargetNodeIndex);
	void ClearAllNodeVisitsForDialogue(UDialogue* TargetDialogue);
	bool WasNodeVisited(const UDialogue* TargetDialogue, int32 TargetNodeIndex) const;

	UDialogue* GetDialogue() const;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
	void OpenDisplay();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
	void CloseDisplay();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
	void DisplaySpeech(FSpeechDetails InSpeechDetails, UDialogueSpeakerComponent* InSpeaker);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintNativeEvent, Category = "Dialogue")
	void DisplayOptions(const TArray<FSpeechDetails>& InOptions);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Dialogue")
	bool CanOpenDisplay() const;
	
	UFUNCTION(BlueprintNativeEvent)
	void HandleMissingSpeaker(const FName& MissingName);

	void TransitionOut();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetWidget(UUserWidget* InWidget);

	UFUNCTION(BlueprintPure, Category = "Dialogue")
	UDialogueWidget* GetWidget() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Dialogue")
	TObjectPtr<UDialogue> CurrentDialogue;

private:
	UPROPERTY()
	TObjectPtr<UDialogueWidget> DialogueWidgetInstance;

	FDialogueRecords DialogueRecords;

public:
	UPROPERTY(BlueprintAssignable, Category="Dialogue")
	FDialogueControllerDelegate OnDialogueStarted;
	
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FDialogueControllerDelegate OnDialogueEnded;
	
	UPROPERTY(BlueprintAssignable, Category = "Dialogue")
	FDialogueControllerSpeechDelegate OnDialogueSpeechDisplayed;
};