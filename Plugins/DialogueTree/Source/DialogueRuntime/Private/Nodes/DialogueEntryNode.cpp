
#include "Nodes/DialogueEntryNode.h"
#include "Dialogue.h"
#include "DialogueRuntimeLogChannels.h"

void UDialogueEntryNode::EnterNode()
{
	check(Dialogue);
	Super::EnterNode();

	if(Children.Num() < 1 || Children[0] == nullptr)
	{
		UE_LOG(LogDialogueRuntime,Warning,TEXT("Exiting dialogue: Entry node has no children..."));
		Dialogue->EndDialogue();
	}
	Dialogue->TraverseNode(Children[0]);
}
