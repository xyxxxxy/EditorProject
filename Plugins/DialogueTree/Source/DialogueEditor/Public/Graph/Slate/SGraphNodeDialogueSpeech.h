
#pragma once

#include "SGraphNodeDialogueBase.h"

class UGraphNodeDialogueSpeech;

class DIALOGUEEDITOR_API SGraphNodeDialogueSpeech : public SGraphNodeDialogueBase
{
public:
	SLATE_BEGIN_ARGS(SGraphNodeDialogueSpeech){}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphNode* InNode);

protected:
	virtual TSharedRef<SWidget> CreateNodeContentArea() override;
	virtual TSharedRef<SWidget> CreateNodeTitleWidget() override;

private:
	const FSlateBrush* GetTransitionIcon() const;
	FText GetSpeakerNameText() const;
	FText GetNodeSubtitleText() const;
	TSharedRef<SWidget> GetSpeakerNameWidget() const;
	TSharedRef<SWidget> GetNodeSubtitleWidget() const;
	TSharedRef<SWidget> GetTitleTextBox() const;

private:
	TObjectPtr<UGraphNodeDialogueSpeech> SpeechNode;
	
	const float TITLE_BOX_PADDING = 10.f;
	const FMargin ICON_PADDING = FMargin(20.f, 0.f, 10.f, 0.f);
	const FMargin SUBTITLE_PADDING = FMargin(0.f, 3.f, 0.f, 0.f);
	const float SUBTITLE_WRAP_DIFFERENCE = 60.f;
};
