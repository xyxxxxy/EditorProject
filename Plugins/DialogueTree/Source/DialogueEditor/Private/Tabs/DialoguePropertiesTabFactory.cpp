﻿
#include "Tabs/DialoguePropertiesTabFactory.h"
#include "Asset/DialogueAssetEditor.h"
#include "Dialogue.h"
#include "Tabs/DialogueEditorTabs.h"

#define LOCTEXT_NAMESPACE "DialoguePropertiesTabFactory"

FDialoguePropertiesTabFactory::FDialoguePropertiesTabFactory(TSharedPtr<FDialogueAssetEditor> InEditor)
:FWorkflowTabFactory(FDialogueEditorTabs::GraphPropertiesTabID,InEditor)
{
	Editor = InEditor;
	TabLabel = LOCTEXT("DialoguePropertiesTabLabel","Properties");
	ViewMenuDescription = LOCTEXT("DialoguePropertiesViewMenuDescription","Display the Properties view for current dialogue asset.");
	ViewMenuTooltip = LOCTEXT("DialoguePropertiesViewMenuTooltip","Show the Properties view.");
}

TSharedRef<SWidget> FDialoguePropertiesTabFactory::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	TSharedPtr<FDialogueAssetEditor> EditorPtr = Editor.Pin();
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>(TEXT("PropertyEditor"));

	FDetailsViewArgs DetailsViewArgs;
	{
		DetailsViewArgs.bAllowSearch = false;
		DetailsViewArgs.bHideSelectionTip = true;
		DetailsViewArgs.bLockable = false;
		DetailsViewArgs.bSearchInitialKeyFocus = true;
		DetailsViewArgs.bUpdatesFromSelection = false;
		DetailsViewArgs.NotifyHook = nullptr;
		DetailsViewArgs.bShowOptions = true;
		DetailsViewArgs.bShowModifiedPropertiesOption = false;
		DetailsViewArgs.bShowScrollBar = false;
	}

	TSharedPtr<IDetailsView> DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);
	
	DetailsView->SetObject(EditorPtr->GetGraph());

	return SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.HAlign(HAlign_Fill)
				[
					DetailsView.ToSharedRef()
				];
}

FText FDialoguePropertiesTabFactory::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return LOCTEXT("DialoguePropertiesTabToolTipText","The Properties view for doing properties thing.");
}

#undef LOCTEXT_NAMESPACE