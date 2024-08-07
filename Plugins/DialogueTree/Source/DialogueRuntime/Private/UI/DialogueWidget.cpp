
#include "UI/DialogueWidget.h"

#include "Components/RichTextBlock.h"

static FString Suffix = FString(TEXT("</>"));

void UDialogueWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OriginStr = OriginText.ToString();
	GetWorld()->GetTimerManager().UnPauseTimer(DisplayTextTimerHandle);
}

void UDialogueWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	DisplayTextTimerDelegate.BindUObject(this,&UDialogueWidget::DisplayDialogue);
	
	GetWorld()->GetTimerManager().PauseTimer(DisplayTextTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(DisplayTextTimerHandle,DisplayTextTimerDelegate,DisplayRate,true);
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
		EndDialogue();
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
	bIsEnd = true;
	CurrentStr.Empty();
	OriginStr.Empty();
	StrIndex = 0;
	bHasEntered = false;
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
