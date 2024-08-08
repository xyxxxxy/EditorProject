
#include "Graph/Slate/SGraphNodeDialogueSpeech.h"

#include "DialogueEditorStyle.h"
#include "Graph/Nodes/GraphNodeDialogueSpeech.h"
#include "DialogueSpeakerSocket.h"


#define LOCTEXT_NAMESPACE "SGraphNodeDialogueSpeech"

void SGraphNodeDialogueSpeech::Construct(const FArguments& InArgs, UEdGraphNode* InNode)
{
	SpeechNode = Cast<UGraphNodeDialogueSpeech>(InNode);
	check(SpeechNode);

	SGraphNodeDialogueBase::Construct(SGraphNodeDialogueBase::FArguments(), InNode);
}

TSharedRef<SWidget> SGraphNodeDialogueSpeech::CreateNodeContentArea()
{
	check(SpeechNode);
	
	return SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.Padding(SUBTITLE_PADDING)
			[
				GetTitleTextBox()
			]
			+ SVerticalBox::Slot()
			.Padding(SUBTITLE_PADDING)
			[
				CreateNodeTitleWidget()
			];
}

TSharedRef<SWidget> SGraphNodeDialogueSpeech::CreateNodeTitleWidget()
{
	check(SpeechNode);
	return SNew(STextBlock)
		.Justification(TEXT_JUSTIFY)
		.Text(SpeechNode->GetSpeechText())
		.WrapTextAt(20.f)
		.TextStyle(&FDialogueEditorStyle::GetTextStyle(TEXT("NodeSubtitleStyle")));
}

TSharedRef<SWidget> SGraphNodeDialogueSpeech::GetTitleTextBox() const
{
	return SNew(SVerticalBox)
	+ SVerticalBox::Slot()
	[
		GetSpeakerNameWidget()
	]
	+ SVerticalBox::Slot()
	.Padding(SUBTITLE_PADDING)
	.AutoHeight()
	[
		GetNodeSubtitleWidget()
	];	
}

TSharedRef<SWidget> SGraphNodeDialogueSpeech::GetSpeakerNameWidget() const
{
	return SNew(STextBlock)
		.Justification(TEXT_JUSTIFY)
		.Text(GetSpeakerNameText());
}

FText SGraphNodeDialogueSpeech::GetSpeakerNameText() const
{
	check(SpeechNode);
	UDialogueSpeakerSocket* SpeakerSocket = SpeechNode->GetSpeakerSocket();
	
	FText SpeakerNameText = LOCTEXT("ErrorNameText", "Error: Speaker Removed");

	if (SpeakerSocket && !SpeakerSocket->GetSpeakerName().IsNone())
	{
		SpeakerNameText = FText::FromName(SpeakerSocket->GetSpeakerName());
	}

	return SpeakerNameText;
}

TSharedRef<SWidget> SGraphNodeDialogueSpeech::GetNodeSubtitleWidget() const
{
	return SNew(STextBlock)
		.Justification(TEXT_JUSTIFY)
		.Text(GetNodeSubtitleText())
		.WrapTextAt(WRAP_TEXT_AT - SUBTITLE_WRAP_DIFFERENCE);
}

FText SGraphNodeDialogueSpeech::GetNodeSubtitleText() const
{
	check(SpeechNode);
	return FText::FromName(SpeechNode->GetID());
}

const FSlateBrush* SGraphNodeDialogueSpeech::GetTransitionIcon() const
{
	return FDialogueEditorStyle::GetBrush("InputTransitionIcon");
}


#undef LOCTEXT_NAMESPACE

