
#include "Asset/DialogueEditorToolbar.h"
#include "DialogueEditorCommands.h"
#include "Asset/DialogueAssetEditor.h"
#include "Textures/SlateIcon.h"

#define LOCTEXT_NAMESPACE "DialogueEditorToolbar"

FDialogueEditorToolbar::FDialogueEditorToolbar(const TSharedPtr<FDialogueAssetEditor> InAssetEditor, UToolMenu* ToolbarMenu)
{
	Editor = InAssetEditor;
	BuildAssetToolbar(ToolbarMenu);
}

void FDialogueEditorToolbar::BuildAssetToolbar(UToolMenu* ToolbarMenu) const
{
	FToolMenuSection& Section = ToolbarMenu->AddSection("DialogueTreeAsset");

	Section.InsertPosition = FToolMenuInsert("Asset",EToolMenuInsertType::After);

	Section.AddEntry(FToolMenuEntry::InitToolBarButton(
		FDialogueEditorCommands::Get().CompileAsset,
		TAttribute<FText>(this,&FDialogueEditorToolbar::GetLabel)
		));
}

FSlateIcon FDialogueEditorToolbar::GetStatusImage() const
{
	return FSlateIcon();
}

FText FDialogueEditorToolbar::GetLabel() const
{
	return FText::FromString(TEXT("Compile Dialogue Tree."));
}

#undef LOCTEXT_NAMESPACE
