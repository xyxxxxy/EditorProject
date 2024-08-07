
#include "DialogueController.h"
#include "Dialogue.h"
#include "DialogueSpeakerComponent.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectIterator.h"

ADialogueController::ADialogueController()
{
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	bIsSpatiallyLoaded = false;
#endif
}

void ADialogueController::SelectOption(int32 InOptionIndex) const
{
	
}

TMap<FName, UDialogueSpeakerComponent*> ADialogueController::GetSpeakers() const
{
	
	return TMap<FName, UDialogueSpeakerComponent*>();
}

void ADialogueController::StartDialogueWithNames(UDialogue* InDialogue, 
	TMap<FName, UDialogueSpeakerComponent*> InSpeakers)
{
	if (!InDialogue)
	{

		return;
	}

	if (!CanOpenDisplay())
	{
		return;
	}

	CurrentDialogue = InDialogue;

	OpenDisplay();

	OnDialogueStarted.Broadcast();
}

void ADialogueController::StartDialogue(UDialogue* InDialogue, 
	TArray<UDialogueSpeakerComponent*> InSpeakers)
{
	if (InSpeakers.IsEmpty())
	{

		return;
	}

	TMap<FName, UDialogueSpeakerComponent*> TargetSpeakers;
	for (UDialogueSpeakerComponent* Speaker : InSpeakers)
	{
		if (Speaker == nullptr)
		{

			return;
		}

		if (Speaker->GetDialogueName().IsNone())
		{

			return;
		}

		
		TargetSpeakers.Add(Speaker->GetDialogueName(), Speaker);
	}

	StartDialogueWithNames(InDialogue, TargetSpeakers);
}

void ADialogueController::EndDialogue()
{
	CloseDisplay();
	OnDialogueEnded.Broadcast();

	if (CurrentDialogue)
	{
		//Clear any behavior flags from the speakers and stop speaking



		CurrentDialogue = nullptr;
	}
}

void ADialogueController::Skip() const
{
	if (CurrentDialogue)
	{

	}
}

void ADialogueController::ClearNodeVisits()
{
	if (CurrentDialogue)
	{
		ClearAllNodeVisitsForDialogue(CurrentDialogue);
	}
}

void ADialogueController::SetSpeaker(FName InName, 
	UDialogueSpeakerComponent* InSpeaker)
{
	if (CurrentDialogue && InSpeaker)
	{

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

	TArray<FName> SpeakerRoles;


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