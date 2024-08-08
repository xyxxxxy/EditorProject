
#include "Graph/Slate/SDialogueGraphEditor.h"

#include "Asset/DialogueAssetEditor.h"
#include "Dialogue.h"
#include "Graph/Nodes/GraphNodeDialogue.h"

void SDialogueGraphEditor::Construct(const FArguments& InArgs, const TSharedPtr<FDialogueAssetEditor> InEditor)
{
	Editor = InEditor;
	Dialogue = Editor.Pin()->GetDialogue();
	NodeDetailsView = InArgs._DetailsView;

	SGraphEditor::FArguments Arguments;
	Arguments._AdditionalCommands = CommandList;
	Arguments._GraphToEdit = Dialogue->GetGraph();
	Arguments._GraphEvents = InArgs._GraphEvents;
	Arguments._AutoExpandActionMenu = true;

	Arguments._GraphEvents.OnSelectionChanged = FOnSelectionChanged::CreateSP(this,&SDialogueGraphEditor::OnSelectedNodesChanged);

	SGraphEditor::Construct(Arguments);
}

void SDialogueGraphEditor::OnSelectedNodesChanged(const TSet<UObject*>& SelectedObjects)
{
	TSharedPtr<IDetailsView> DetailsView = NodeDetailsView.Pin();
	if(SelectedObjects.Num() < 1)
	{
		DetailsView->SetObject(nullptr);
		return;
	}

	TArray<UObject*> SelectedNodes;
	for(auto& CurrentObject : SelectedObjects)
	{
		if(UGraphNodeDialogue* DialogueNode = Cast<UGraphNodeDialogue>(CurrentObject))
		{
			SelectedNodes.Add(CurrentObject);
		}
	}
	check(DetailsView);
	DetailsView->SetObjects(SelectedNodes);
	
}
