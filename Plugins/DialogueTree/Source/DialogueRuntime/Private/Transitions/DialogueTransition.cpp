
#include "Transitions/DialogueTransition.h"
#include "DialogueRuntimeLogChannels.h"
#include "Nodes/DialogueSpeechNode.h"
#include "Dialogue.h"
#include "DialogueSpeakerComponent.h"

UDialogueTransition::UDialogueTransition()
{
	OnContentEnd.BindUFunction(this, "OnDonePlayingContent");
	OnTimerEnd.BindUFunction(this, "OnMinPlayTimeElapsed");
}

void UDialogueTransition::SetOwningNode(UDialogueSpeechNode* InNode)
{
	OwningNode = InNode;
}

void UDialogueTransition::StartTransition()
{
	bMinPlayTimeElapsed = false;
	bAudioFinished = false;
	
	if (!OwningNode)
	{
		UE_LOG(LogDialogueRuntime,Error,TEXT("Transition failed to find owning node. Ending dialogue early."));
		OwningNode->GetDialogue()->EndDialogue();
		return;
	}
	
	UDialogueSpeakerComponent* SpeakerComp = OwningNode->GetSpeaker();
	if (!SpeakerComp)
	{
		UE_LOG(LogDialogueRuntime,Error, TEXT("Transition failed to find speaker component. Ending dialogue early."));
		OwningNode->GetDialogue()->EndDialogue();
		return;
	}
	
	float MinPlayTime = OwningNode->GetDetails().MinimumPlayTime;
	if (MinPlayTime > 0.01f)
	{
		SpeakerComp->GetWorld()->GetTimerManager().SetTimer(MinPlayTimeHandle, OnTimerEnd, MinPlayTime, false);
	}
	else
	{
		bMinPlayTimeElapsed = true;
	}
	
	if (SpeakerComp->IsPlaying())
	{
		SpeakerComp->OnAudioFinished.AddUnique(OnContentEnd);
	}
	else
	{
		bAudioFinished = true;
	}

	//If no minimum time or audio content, just transition out 
	TryTransitionOut();
}

void UDialogueTransition::TransitionOut()
{
}

void UDialogueTransition::SelectOption(int32 InOptionIndex)
{
}

void UDialogueTransition::Skip()
{
	if (!bAudioFinished)
	{
		OnDonePlayingContent();
	}

	if (!bMinPlayTimeElapsed)
	{
		OnMinPlayTimeElapsed();
	}
}

FText UDialogueTransition::GetDisplayName() const
{
	return FText::FromString(StaticClass()->GetName());
}

FText UDialogueTransition::GetNodeCreationTooltip() const
{
	return FText::GetEmpty();
}

EDialogueConnectionLimit UDialogueTransition::GetConnectionLimit() const
{
	return EDialogueConnectionLimit::Single;
}

void UDialogueTransition::TryTransitionOut()
{
	if (bAudioFinished && bMinPlayTimeElapsed)
	{
		TransitionOut();
	}
}

void UDialogueTransition::OnDonePlayingContent()
{
	//Unbind from audio event 
	// UDialogueSpeakerComponent* Speaker = OwningNode->GetSpeaker();
	//
	// if (Speaker)
	// {
	// 	Speaker->Stop();
	// 	Speaker->OnAudioFinished.Remove(OnContentEnd);
	// }

	//Mark audio complete
	bAudioFinished = true;

	//See if we should transition out
	TryTransitionOut();
}

void UDialogueTransition::OnMinPlayTimeElapsed()
{
	//Mark min play time elapsed
	bMinPlayTimeElapsed = true;

	//Check if we should transition out
	TryTransitionOut();
}
