
#include "UI/DialogueWidget.h"
#include "Components/RichTextBlock.h"
#include "DialogueRuntimeLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(DialogueWidget)

const static FString Suffix = FString(TEXT("</>"));

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnStateStart.AddDynamic(this, &UDialogueWidget::StartNewState);
	OnCurrentStateEnd.AddDynamic(this, &UDialogueWidget::PreSwitchToNextState);

	OriginStr = OriginText.ToString();
}

void UDialogueWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	DisplayTextTimerDelegate.BindUObject(this,&UDialogueWidget::DisplayDialogue);
}

void UDialogueWidget::NativeDestruct()
{
	Super::NativeDestruct();
	
	GetWorld()->GetTimerManager().ClearTimer(DisplayTextTimerHandle);
	DisplayTextTimerDelegate.Unbind();
}


void UDialogueWidget::DisplayDialogue()
{
	if(StrIndex >= OriginStr.Len())
	{
		UE_LOG(LogDialogueRuntime, Warning , TEXT("Widget : StrIndex >= OriginStr.Len()!"));
		OnCurrentStateEnd.Broadcast();
		//PreSwitchToNextState();
		//EndDialogue();
		return;
	}

	if(OriginStr[StrIndex] == '<')
	{
		// start to enter rich block
		if(!bHasEntered)
		{
			FindFirstIndexAfterRich();
			if(StrIndex < OriginStr.Len() && OriginStr[StrIndex] == '<')
			{
				FindFirstIndexAfterRich();
				StrIndex--;
				CurrentStr = OriginStr.Left(StrIndex+1);
				DialogueTextBlock->SetText(FText::FromString(CurrentStr));
			}
			else
			{
				bHasEntered = true;
				CurrentStr = OriginStr.Left(StrIndex+1).Append(Suffix);
				StrIndex++;
				DialogueTextBlock->SetText(FText::FromString(CurrentStr));
			}
		}
		// start to exit rich block
		else
		{
			StrIndex++;
			FindFirstIndexAfterRich();
			bHasEntered = false;
			
			CurrentStr = OriginStr.Left(StrIndex+1);
			StrIndex++;
			DialogueTextBlock->SetText(FText::FromString(CurrentStr));
			
			return;
		}
	}
	// in rich block
	if(bHasEntered)
	{
		CurrentStr = OriginStr.Left(StrIndex+1).Append(Suffix);
		StrIndex++;
		DialogueTextBlock->SetText(FText::FromString(CurrentStr));
	}
	// out of rich block
	else
	{
		CurrentStr = OriginStr.Left(StrIndex+1);
		StrIndex++;
		DialogueTextBlock->SetText(FText::FromString(CurrentStr));
	}
}

void UDialogueWidget::EndDialogue()
{
	GetWorld()->GetTimerManager().PauseTimer(DisplayTextTimerHandle);
}

void UDialogueWidget::FindFirstIndexAfterRich()
{
	while(StrIndex < OriginStr.Len() && OriginStr[StrIndex] != '>')
	{
		StrIndex++;
	}
	StrIndex++;
}

void UDialogueWidget::SetDisplayRate(float InRate)
{
	DisplayRate = InRate;
}

void UDialogueWidget::StartNewState(FSpeechDetails InDetails)
{
	OriginText = InDetails.SpeechText;
	OriginStr = OriginText.ToString();
	//DisplayRate = InDetails.
	GetWorld()->GetTimerManager().SetTimer(DisplayTextTimerHandle, DisplayTextTimerDelegate, DisplayRate, true);
}

void UDialogueWidget::PreSwitchToNextState()
{
	CurrentStr.Empty();
	OriginStr.Empty();
	StrIndex = 0;
	bHasEntered = false;
	bEntireState = true;
	GetWorld()->GetTimerManager().ClearTimer(DisplayTextTimerHandle);
}

void UDialogueWidget::DisplayEntireState()
{
	GetWorld()->GetTimerManager().PauseTimer(DisplayTextTimerHandle);
	DialogueTextBlock->SetText(OriginText);
	PreSwitchToNextState();
}

void UDialogueWidget::SetController(ADialogueController* InDialogueController)
{
	check(InDialogueController);
	DialogueController = InDialogueController;
}

void UDialogueWidget::OnSwitchToNextState()
{
	check(DialogueController);
	DialogueController->TransitionOut();
	bEntireState = false;
}

void UDialogueWidget::OnButtonClicked()
{
	if(bEntireState)
	{
		OnSwitchToNextState();
	}
	else
	{
		DisplayEntireState();
	}
	
}