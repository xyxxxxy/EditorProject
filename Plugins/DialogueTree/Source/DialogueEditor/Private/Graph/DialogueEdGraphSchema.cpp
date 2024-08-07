
#include "Graph/DialogueEdGraphSchema.h"
#include "Graph/DialogueEdGraph.h"
#include "Graph/DialogueGraphSchema_Action.h"
#include "Graph/Nodes/GraphNodeDialogueEntry.h"
#include "Graph/Nodes/GraphNodeDialogueSpeech.h"
#include "Transitions/DialogueTransition.h"


#define LOCTEXT_NAMESPACE "DialogueEditorSchema"

int32 UDialogueEdGraphSchema::CurrentCacheRefreshID = 0;

FConnectionDrawingPolicy* UDialogueEdGraphSchema::CreateConnectionDrawingPolicy(int32 InBackLayerID,
	int32 InFrontLayerID, float InZoomFactor, const FSlateRect& InClippingRect, FSlateWindowElementList& InDrawElements,
	UEdGraph* InGraphObj) const
{
	return Super::CreateConnectionDrawingPolicy(InBackLayerID, InFrontLayerID, InZoomFactor, InClippingRect,
	                                            InDrawElements, InGraphObj);
}

void UDialogueEdGraphSchema::OnPinConnectionDoubleCicked(UEdGraphPin* PinA, UEdGraphPin* PinB,
	const FVector2D& GraphPosition) const
{
	Super::OnPinConnectionDoubleCicked(PinA, PinB, GraphPosition);
}

void UDialogueEdGraphSchema::GetGraphContextActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	GetSpeechNodeMenuActions(ContextMenuBuilder);
}

void UDialogueEdGraphSchema::GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const
{
	Super::GetContextMenuActions(Menu, Context);
}

const FPinConnectionResponse UDialogueEdGraphSchema::CanCreateConnection(const UEdGraphPin* A,
	const UEdGraphPin* B) const
{
	return Super::CanCreateConnection(A, B);
}

void UDialogueEdGraphSchema::CreateDefaultNodesForGraph(UEdGraph& Graph) const
{
	FGraphNodeCreator<UGraphNodeDialogueEntry> NodeCreator(Graph);
	UGraphNodeDialogueEntry* TargetNode = NodeCreator.CreateNode();
	NodeCreator.Finalize();
	SetNodeMetaData(TargetNode,FNodeMetadata::DefaultGraphNode);
	
	UDialogueEdGraph* DialogueGraph = Cast<UDialogueEdGraph>(&Graph);
	TargetNode->InitNodeInDialogueGraph(DialogueGraph);
	DialogueGraph->SetGraphRoot(TargetNode);
}

FLinearColor UDialogueEdGraphSchema::GetPinTypeColor(const FEdGraphPinType& PinType) const
{
	return FLinearColor(.5f,.5f,.5f,1.f);
}

bool UDialogueEdGraphSchema::IsCacheVisualizationOutOfDate(int32 InVisualizationCacheID) const
{
	return Super::IsCacheVisualizationOutOfDate(InVisualizationCacheID);
}

int32 UDialogueEdGraphSchema::GetCurrentVisualizationCacheID() const
{
	return Super::GetCurrentVisualizationCacheID();
}

void UDialogueEdGraphSchema::ForceVisualizationCacheClear() const
{
	Super::ForceVisualizationCacheClear();
}

void UDialogueEdGraphSchema::GetSpeechNodeMenuActions(FGraphContextMenuBuilder& ContextMenuBuilder) const
{
	//Fetch the current graph 
	const UDialogueEdGraph* TargetGraph = Cast<UDialogueEdGraph>(ContextMenuBuilder.CurrentGraph);

	if (!TargetGraph)
	{
		return;
	}

	//Loop through the list of speakers 
	for (UDialogueSpeakerSocket* SpeakerSocket : TargetGraph->GetAllSpeakers())
	{
		if (SpeakerSocket->GetSpeakerName().IsNone())
		{
			continue;
		}

		//Add one create node action for each transition type 
		for (TObjectIterator<UClass> TypeIterator; TypeIterator; ++TypeIterator)
		{
			if (TypeIterator->IsChildOf(UDialogueTransition::StaticClass())
				&& !TypeIterator->HasAnyClassFlags(CLASS_Abstract))
			{
				TSubclassOf<UDialogueTransition> CurrentType = *TypeIterator;

				TSharedPtr<FDialogueSchemaAction_NewNode> NewNodeAction =
					MakeCreateSpeechNodeAction(
						SpeakerSocket,
						CurrentType,
						ContextMenuBuilder.OwnerOfTemporaries
					);
				
				//Create action and add to menu 
				ContextMenuBuilder.AddAction(NewNodeAction);
			}
		}
	}

}

TSharedPtr<FDialogueSchemaAction_NewNode> UDialogueEdGraphSchema::MakeCreateSpeechNodeAction(
	UDialogueSpeakerSocket* SpeakerSocket, TSubclassOf<UDialogueTransition> TransitionType, UObject* Outer) const
{
	check(SpeakerSocket && TransitionType && Outer);

	//Get context menu text
	UDialogueTransition* DefaultTransitionObj = TransitionType->GetDefaultObject<UDialogueTransition>();

	FText MenuCategory = LOCTEXT("SpeechNodeMenuCategory", "SpeechNodes");
	FText MenuText = FText::Format(
		LOCTEXT("SpeechNodeCreationText", "{0}, {1}"),
		FText::FromName(SpeakerSocket->GetSpeakerName()),
		DefaultTransitionObj->GetDisplayName()
	);
	FText MenuTooltip = DefaultTransitionObj->GetNodeCreationTooltip();

	//Get template node
	UGraphNodeDialogueSpeech* TemplateObject = UGraphNodeDialogueSpeech::MakeTemplate(Outer, SpeakerSocket, TransitionType);
	check(TemplateObject);

	//Assemble action 
	TSharedPtr<FDialogueSchemaAction_NewNode> NewAction(
		new FDialogueSchemaAction_NewNode(
			MenuCategory,
			MenuText,
			MenuTooltip,
			TemplateObject
		)
	);

	return NewAction;
}

#undef LOCTEXT_NAMESPACE
