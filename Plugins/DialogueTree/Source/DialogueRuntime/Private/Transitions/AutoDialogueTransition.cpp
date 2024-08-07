
#include "Transitions/AutoDialogueTransition.h"
#include "Nodes/DialogueSpeechNode.h"
#include "Dialogue.h"

FText UAutoDialogueTransition::GetDisplayName() const
{
	return FText::FromString("AutoTransition");
}

FText UAutoDialogueTransition::GetNodeCreationTooltip() const
{
	return FText::FromString("Create speech node that automatically transitions to the first viable option");
}

void UAutoDialogueTransition::TransitionOut()
{
	TArray<UDialogueNode*> Children = OwningNode->GetChildren();
	if (!Children.IsEmpty() && Children[0])
	{
		OwningNode->GetDialogue()->TraverseNode(Children[0]);
	}
	else
	{
		OwningNode->GetDialogue()->EndDialogue();
	}
}
