
#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowCentricApplication.h"

class UDialogue;

class FDialogueAssetEditor : public FWorkflowCentricApplication, public FEditorUndoClient, public FNotifyHook, public FGCObject
{
public:
	virtual void RegisterTabSpawners(const TSharedRef<class FTabManager>& tabManager) override;
	void InitEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, UObject* InObject);
	
	UDialogue* GetDialogue() const;

	void CreateGraph();
	UEdGraph* GetGraph() const;

	TSharedPtr<SGraphEditor> GetGraphEditor() const;

public:
	// FAssetEditorToolkit Interface
	virtual FName GetToolkitFName() const override;
	virtual FText GetBaseToolkitName() const override;
	virtual FText GetToolkitName() const override;
	virtual FString GetWorldCentricTabPrefix() const override;
	virtual FLinearColor GetWorldCentricTabColorScale() const override;
	virtual FText GetToolkitToolTipText() const override;
	virtual FString GetDocumentationLink() const override;
	virtual void OnToolkitHostingStarted(const TSharedRef<IToolkit>& Toolkit) override;
	virtual void OnToolkitHostingFinished(const TSharedRef<IToolkit>& Toolkit) override;
	virtual bool OnRequestClose() override;
	// ~ Interface

	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	virtual FString GetReferencerName() const override;

	void SetNodeDetailView(TSharedPtr<IDetailsView> InDetailView);
	TSharedPtr<IDetailsView> GetNodeDetailView() const;

	void SpawnInitialNodes();
	
	void OnPasteNodes();
	void PasteNodesAtLocation(const FVector2D& PasteLocation);
	bool CanPasteNodes() const;
	
	void OnFinishedChangingProperties(const FPropertyChangedEvent& PropertyChangedEvent);

	void ExtendToolbar();
	void FillToolbarMenu(FToolBarBuilder& ToolbarBuilder);

	FSlateIcon GetStatusImage() const;
	
	void OnCompile();
	
	void RegisterCommands();
	
	void OnRenameNode();
	bool CanRenameNode() const;

	void OnDuplicateNodes();
	bool CanDuplicateNodes() const;
	
	void OnSelectAll();
	bool CanSelectAll() const;
	
	void OnDeleteNodes(); 
	void OnDeleteDuplicatableNodes();
	bool CanDeleteNodes() const;
	
	void OnCopyNodes();
	bool CanCopyNodes() const;
	
	void OnCutNodes();
	bool CanCutNodes() const;
	
	void OnChangeSelection(const TSet<UObject*>& SelectedObjects);

private:
	TObjectPtr<UDialogue> Dialogue;
	TObjectPtr<UEdGraph> Graph;

	TSharedPtr<IDetailsView> NodeDetailsView;

	TSharedPtr<SGraphEditor> GraphEditor;

	TSharedPtr<FUICommandList> EditorCommands;
};
