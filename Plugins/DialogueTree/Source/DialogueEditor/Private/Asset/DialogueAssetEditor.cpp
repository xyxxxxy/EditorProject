
#include "Asset/DialogueAssetEditor.h"

#include "Dialogue.h"
#include "GraphEditorActions.h"
#include "Asset/DialogueAssetAppMode.h"
#include "Graph/DialogueEdGraph.h"
#include "Graph/DialogueEdGraphSchema.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Framework/Commands/GenericCommands.h"

#define LOCTEXT_NAMESPACE "DialogueAssetEditor"

void FDialogueAssetEditor::RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager)
{
	FWorkflowCentricApplication::RegisterTabSpawners(tabManager);
}

void FDialogueAssetEditor::InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost,
	UObject* InObject)
{
	TArray<UObject*> ObjectsToEdit;
	ObjectsToEdit.Add(InObject);

	Dialogue = Cast<UDialogue>(InObject);
	
	CreateGraph();

	FGenericCommands::Register();
	FGraphEditorCommands::Register();
	
	InitAssetEditor(
		Mode,
		InitToolkitHost,
		TEXT("DialogueAssetEditor"),
		FTabManager::FLayout::NullLayout,
		true,//?
		true,
		ObjectsToEdit
	);

	AddApplicationMode(TEXT("DialogueAssetAppMode"),MakeShareable(new FDialogueAssetAppMode(SharedThis(this))));

	SetCurrentMode(TEXT("DialogueAssetAppMode"));

	RegenerateMenusAndToolbars();
}

UDialogue* FDialogueAssetEditor::GetDialogue() const
{
	return Dialogue;
}

void FDialogueAssetEditor::CreateGraph()
{
	if(!Dialogue->GetGraph())
	{
		Graph = FBlueprintEditorUtils::CreateNewGraph(
			Dialogue,
			NAME_None,
			UDialogueEdGraph::StaticClass(),
			UDialogueEdGraphSchema::StaticClass()
			);
		check(Graph);

		Dialogue->SetGraph(Graph);
		Dialogue->GetGraph()->bAllowDeletion = false;
		
		SpawnInitialNodes();
	}

}

UEdGraph* FDialogueAssetEditor::GetGraph() const
{
	return Graph;
}

TSharedPtr<SGraphEditor> FDialogueAssetEditor::GetGraphEditor() const
{
	return GraphEditor;
}

FName FDialogueAssetEditor::GetToolkitFName() const
{
	return FName(TEXT("FDialogueTreeEditor"));
}

FText FDialogueAssetEditor::GetBaseToolkitName() const
{
	return LOCTEXT("DialogueEditorBaseToolkitName", "Dialogue Tree Editor");
}

FText FDialogueAssetEditor::GetToolkitName() const
{
	if (Dialogue)
	{
		return FText::FromString(Dialogue->GetName());
	}

	return GetBaseToolkitName();
}

FString FDialogueAssetEditor::GetWorldCentricTabPrefix() const
{
	return TEXT("DialogueAssetEditor");
}

FLinearColor FDialogueAssetEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(.3f,.2f,.5f,.5f);
}

FText FDialogueAssetEditor::GetToolkitToolTipText() const
{
	    return GetToolTipTextForObject(Dialogue);
}

FString FDialogueAssetEditor::GetDocumentationLink() const
{
	return TEXT("http://github.com/xyxxxxy");
}

void FDialogueAssetEditor::OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit)
{
}

void FDialogueAssetEditor::OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit)
{
}

bool FDialogueAssetEditor::OnRequestClose()
{
	return FWorkflowCentricApplication::OnRequestClose();
}

void FDialogueAssetEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
	check(Dialogue);
	UEdGraph* TargetGraph = Dialogue->GetGraph();
	check(TargetGraph);

	Collector.AddReferencedObject(Dialogue);
	Collector.AddReferencedObject(TargetGraph);
}

FString FDialogueAssetEditor::GetReferencerName() const
{
	return TEXT("FDialogueTreeEditor");
}

void FDialogueAssetEditor::SetNodeDetailView(TSharedPtr<IDetailsView> InDetailView)
{
	InDetailView = InDetailView.ToSharedRef();
}

TSharedPtr<IDetailsView> FDialogueAssetEditor::GetNodeDetailView() const
{
	return NodeDetailsView;
}

void FDialogueAssetEditor::SpawnInitialNodes()
{
	const UEdGraphSchema* GraphSchema = Graph->GetSchema();
	check(GraphSchema);
	GraphSchema->CreateDefaultNodesForGraph(
		*Graph
	);
}

void FDialogueAssetEditor::OnPasteNodes()
{
}

void FDialogueAssetEditor::PasteNodesAtLocation(const FVector2D& PasteLocation)
{
}

bool FDialogueAssetEditor::CanPasteNodes() const
{
	return true;
}

void FDialogueAssetEditor::OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent)
{
}

void FDialogueAssetEditor::ExtendToolbar()
{
}

void FDialogueAssetEditor::FillToolbarMenu(FToolBarBuilder& ToolbarBuilder)
{
}

FSlateIcon FDialogueAssetEditor::GetStatusImage() const
{
	return FSlateIcon();
}

void FDialogueAssetEditor::OnCompile()
{
}

void FDialogueAssetEditor::RegisterCommands()
{
}

void FDialogueAssetEditor::OnRenameNode()
{
}

bool FDialogueAssetEditor::CanRenameNode() const
{
	return true;
}

void FDialogueAssetEditor::OnDuplicateNodes()
{
}

bool FDialogueAssetEditor::CanDuplicateNodes() const
{
	return true;
}

void FDialogueAssetEditor::OnSelectAll()
{
}

bool FDialogueAssetEditor::CanSelectAll() const
{
	return true;
}

void FDialogueAssetEditor::OnDeleteNodes()
{
}

void FDialogueAssetEditor::OnDeleteDuplicatableNodes()
{
}

bool FDialogueAssetEditor::CanDeleteNodes() const
{
	return true;
}

void FDialogueAssetEditor::OnCopyNodes()
{
}

bool FDialogueAssetEditor::CanCopyNodes() const
{
	return true;
}

void FDialogueAssetEditor::OnCutNodes()
{
}

bool FDialogueAssetEditor::CanCutNodes() const
{
	return true;
}

void FDialogueAssetEditor::OnChangeSelection(const TSet<UObject*>& SelectedObjects)
{
}

#undef LOCTEXT_NAMESPACE
