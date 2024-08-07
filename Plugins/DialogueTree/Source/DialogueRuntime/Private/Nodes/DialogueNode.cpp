
#include "Nodes/DialogueNode.h"

UDialogue* UDialogueNode::GetDialogue() const
{
	return Dialogue;
}

void UDialogueNode::SetDialogue(UDialogue* InDialogue)
{
	Dialogue = InDialogue;
}

void UDialogueNode::AddChild(UDialogueNode* InNode)
{
	if(!Children.Contains(InNode))
	{
		Children.Add(InNode);
	}
}

void UDialogueNode::AddParent(UDialogueNode* InNode)
{
	if(!Parent.Contains(InNode))
	{
		Parent.Add(InNode);
	}
}

TArray<UDialogueNode*> UDialogueNode::GetChildren() const
{
	return Children;
}

TArray<UDialogueNode*> UDialogueNode::GetParent() const
{
	return Parent;
}

int32 UDialogueNode::GetNodeIndex() const
{
	return NodeIndex;
}

void UDialogueNode::SetNodeIndex(int32 Index)
{
	NodeIndex = Index;
}

FDialogueOption UDialogueNode::GetAsOption()
{
	return FDialogueOption();
}

void UDialogueNode::SelectOption(int32 InOptionIndex)
{
}
