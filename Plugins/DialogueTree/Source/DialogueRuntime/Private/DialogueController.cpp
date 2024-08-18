
#include "DialogueController.h"
#include "Dialogue.h"
#include "DialogueSpeakerComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectIterator.h"
#include "DialogueRuntimeLogChannels.h"
#include "Nodes/DialogueSpeechNode.h"
#include "Transitions/DialogueTransition.h"
#include "UI/DialogueWidget.h"
#include "CommonUIExtensions.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayTagContainer.h"
#include "DialogueInterface.h"

ADialogueController::ADialogueController()
{
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	bIsSpatiallyLoaded = false;
#endif
	//check(DialogueWidgetInstance.Get());
	OnAfterWidgetPush.BindUObject(this,&ADialogueController::StartState);
}

void ADialogueController::SelectOption(int32 InOptionIndex) const
{	
}

TMap<FName, UDialogueSpeakerComponent*> ADialogueController::GetSpeakers() const
{
	if(CurrentDialogue)
	{
		return CurrentDialogue->GetAllSpeakers();
	}
	return TMap<FName, UDialogueSpeakerComponent*>();
}

void ADialogueController::StartDialogueWithNames(UDialogue* InDialogue, const TMap<FName, UDialogueSpeakerComponent*>& InSpeakers)
{
	if (!InDialogue)
	{
		UE_LOG(LogDialogueRuntime, Error, TEXT("Controller : Could not start dialogue. Provided dialogue null."));
		return;
	}

	if (!CanOpenDisplay())
	{
		UE_LOG(LogDialogueRuntime, Warning, TEXT("Controller : Can not open Dialogue Widget."));
		return;
	}

	CurrentDialogue = InDialogue;
	UE_LOG(LogDialogueRuntime, Error, TEXT("Controller : StartDialogueWithNames."));
	

	OpenDisplay(InSpeakers);
	// Open Widget
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UCommonUIExtensions::SuspendInputForPlayer(PC->GetLocalPlayer(), FName(TEXT("Push Content To Layer")));
		DialogueWidgetInstance = UCommonUIExtensions::PushContentToLayer_ForPlayer(
			PC->GetLocalPlayer(), 
			FGameplayTag::RequestGameplayTag(FName(TEXT("UI.Layer.Game"))), 
			DialogueWidgetClass
		);
		if(DialogueWidgetInstance.Get() && DialogueWidgetInstance.Get()->Implements<UDialogueInterface>())
		{
			IDialogueInterface::Execute_SetController(DialogueWidgetInstance.Get(), this);
		}
	}
	

	
	//OpenDisplay(InSpeakers);
	OnAfterWidgetPush.ExecuteIfBound(InSpeakers);
	//Traverse Node
	// ???
	//CurrentDialogue->OpenDialogue(this, InSpeakers);
	//OnDialogueStarted.Broadcast();
}

void ADialogueController::StartDialogue(UDialogue* InDialogue, TArray<UDialogueSpeakerComponent*> InSpeakers)
{
	if (InSpeakers.IsEmpty())
	{
		UE_LOG(LogDialogueRuntime, Warning, TEXT("Controller : No speakers provided on dialogue start."));		
		return;
	}

	TMap<FName, UDialogueSpeakerComponent*> TargetSpeakers;
	for (UDialogueSpeakerComponent* Speaker : InSpeakers)
	{
		if (Speaker == nullptr)
		{
			UE_LOG(LogDialogueRuntime, Error, TEXT("Controller : Could not start dialogue. Invalid speaker provided."));
			return;
		}

		if (Speaker->GetDialogueName().IsNone())
		{
			UE_LOG(LogDialogueRuntime, Error, TEXT("Controller : Could not start dialogue. A provided speaker has an unfilled DialogueName."));
			return;
		}

		if (TargetSpeakers.Contains(Speaker->GetDialogueName()))
		{
			UE_LOG(LogDialogueRuntime, Error, TEXT("Could not start dialogue. Multiple provided speakers share a DialogueName."));
			return;
		}

		TargetSpeakers.Add(Speaker->GetDialogueName(), Speaker);
	}
	UE_LOG(LogDialogueRuntime, Warning, TEXT("Controller : StartDialogue."));
	StartDialogueWithNames(InDialogue, TargetSpeakers);
}

void ADialogueController::EndDialogue()
{
	CloseDisplay();
	//OnDialogueEnded.Broadcast();

	if (CurrentDialogue)
	{
		//Clear any behavior flags from the speakers and stop speaking
		for (auto& Entry : CurrentDialogue->GetAllSpeakers())
		{
			if (Entry.Value)
			{
				Entry.Value->Stop();
				Entry.Value->ClearBehaviorFlags();
			}
		}

		CurrentDialogue->ClearController();
		CurrentDialogue = nullptr;
	}
}

void ADialogueController::Skip() const
{
	if (CurrentDialogue)
	{
		CurrentDialogue->Skip();
	}
}

void ADialogueController::ClearNodeVisits()
{
	if (CurrentDialogue)
	{
		ClearAllNodeVisitsForDialogue(CurrentDialogue);
	}
}

void ADialogueController::SetSpeaker(FName InName, UDialogueSpeakerComponent* InSpeaker)
{
	if (CurrentDialogue && InSpeaker)
	{
		CurrentDialogue->SetSpeaker(InName, InSpeaker);
	}
}

FDialogueRecords ADialogueController::GetDialogueRecords() const
{
	return DialogueRecords;
}

void ADialogueController::ClearDialogueRecords()
{
	DialogueRecords.Records.Empty();
}

void ADialogueController::ImportDialogueRecords(FDialogueRecords InRecords)
{
	DialogueRecords = InRecords;
}

bool ADialogueController::SpeakerInCurrentDialogue(UDialogueSpeakerComponent* TargetSpeaker) const
{
	//If no active dialogue, then automatically false
	if (!CurrentDialogue)
	{
		return false;
	}

	//Retrieve speakers
	TMap<FName, UDialogueSpeakerComponent*> Speakers = CurrentDialogue->GetAllSpeakers();
	TArray<FName> SpeakerRoles;
	Speakers.GetKeys(SpeakerRoles);

	//If any one speaker matches the target, true
	for(FName NextRole : SpeakerRoles)
	{
		if (Speakers[NextRole] == TargetSpeaker)
		{
			return true;
		}
	}

	//No speakers matched the target, false
	return false;
}

void ADialogueController::MarkNodeVisited(UDialogue* TargetDialogue, int32 TargetNodeIndex)
{
	if (!TargetDialogue)
	{
		return;
	}

	FName TargetDialogueName = TargetDialogue->GetFName();
	if (TargetDialogueName.IsEqual(NAME_None))
	{
		return;
	}

	//Create a new record if the target record does not exist
	if (!DialogueRecords.Records.Contains(TargetDialogueName))
	{
		FDialogueNodeVisits NewRecord;
		NewRecord.DialogueFName = TargetDialogueName;

		DialogueRecords.Records.Add(TargetDialogueName, NewRecord);
	}

	//Mark the node visited in the record 
	DialogueRecords.Records[TargetDialogueName].VisitedNodeIndices.Add(TargetNodeIndex);
}

void ADialogueController::MarkNodeUnvisited(UDialogue* TargetDialogue, int32 TargetNodeIndex)
{
	if (!TargetDialogue)
	{
		return;
	}

	FName TargetDialogueName = TargetDialogue->GetFName();
	if (!DialogueRecords.Records.Contains(TargetDialogueName))
	{
		return;
	}
	
	DialogueRecords.Records[TargetDialogueName].VisitedNodeIndices.Remove(TargetNodeIndex);
}

void ADialogueController::ClearAllNodeVisitsForDialogue(UDialogue* TargetDialogue)
{
	if (!TargetDialogue)
	{
		return;
	}

	FName TargetDialogueName = TargetDialogue->GetFName();
	if (!DialogueRecords.Records.Contains(TargetDialogueName))
	{
		return;
	}

	DialogueRecords.Records[TargetDialogueName].VisitedNodeIndices.Empty();
}

bool ADialogueController::WasNodeVisited(const UDialogue* TargetDialogue, int32 TargetNodeIndex) const
{
	if (!TargetDialogue)
	{
		return false;
	}

	FName TargetDialogueName = TargetDialogue->GetFName();

	if (!DialogueRecords.Records.Contains(TargetDialogueName))
	{
		return false;
	}

	FDialogueNodeVisits TargetRecord = DialogueRecords.Records[TargetDialogueName];

	return TargetRecord.VisitedNodeIndices.Contains(TargetNodeIndex);
}

bool ADialogueController::CanOpenDisplay_Implementation() const
{
	return true;
}

UDialogue* ADialogueController::GetDialogue() const
{
	return CurrentDialogue;
}

void ADialogueController::TransitionOut()
{
	UDialogueNode* DialogueNode = CurrentDialogue->GetActiveNode();
	check(DialogueNode);
	if(UDialogueSpeechNode* SpeechNode = Cast<UDialogueSpeechNode>(DialogueNode))
	{
		if(UDialogueTransition* Transition = SpeechNode->GetTransition())
		{
			Transition->TryTransitionOut();
		}
	}
}

void ADialogueController::DisplaySpeech_Implementation(FSpeechDetails InSpeechDetails, UDialogueSpeakerComponent* InSpeaker)
{
	GetWidget()->OnStateStart.Broadcast(InSpeechDetails);
}

void ADialogueController::SetWidget(UUserWidget* InWidget)
{
	if(UDialogueWidget* Widget = Cast<UDialogueWidget>(InWidget))
	{
		DialogueWidgetInstance = Widget;
	}
}

UDialogueWidget* ADialogueController::GetWidget() const
{
	return Cast<UDialogueWidget>(DialogueWidgetInstance.Get());
}

void ADialogueController::OpenWidget_Implementation(const TMap<FName, UDialogueSpeakerComponent*>& InSpeakers){}

void ADialogueController::CloseDisplay_Implementation(){}

void ADialogueController::DisplayOptions_Implementation(const TArray<FSpeechDetails>& InOptions){}

void ADialogueController::HandleMissingSpeaker_Implementation(const FName& MissingName){}

void ADialogueController::StartState(const TMap<FName, UDialogueSpeakerComponent*>& InSpeakers)
{
	//GetWidget()->OnStateStart.Broadcast()
	CurrentDialogue->OpenDialogue(this, InSpeakers);
}

void ADialogueController::BeforePush(){}
void ADialogueController::AfterPush(const TMap<FName, UDialogueSpeakerComponent*>& InSpeakers)
{
	OnAfterWidgetPush.ExecuteIfBound(InSpeakers);
}