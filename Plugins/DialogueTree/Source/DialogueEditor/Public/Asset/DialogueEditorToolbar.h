
#pragma once

#include "CoreMinimal.h"
#include "Asset/DialogueAssetEditor.h"

class UToolMenu;
struct FSlateIcon;

class DIALOGUEEDITOR_API FDialogueEditorToolbar : public TSharedFromThis<FDialogueEditorToolbar>
{
public:
	explicit FDialogueEditorToolbar(const TSharedPtr<FDialogueAssetEditor> InAssetEditor, UToolMenu* ToolbarMenu);

private:
	void BuildAssetToolbar(UToolMenu* ToolbarMenu) const;
	FSlateIcon GetStatusImage() const;
	FText GetLabel() const;
	//TSharedRef<SWidget> MakeDiffMenu() const;
	
	//void BuildDebuggerToolbar(UToolMenu* ToolbarMenu) const;

private:
	TWeakPtr<FDialogueAssetEditor> Editor;
};