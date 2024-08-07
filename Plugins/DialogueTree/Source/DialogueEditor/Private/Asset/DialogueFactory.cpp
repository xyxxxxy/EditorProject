
#include "Asset/DialogueFactory.h"
#include "Dialogue.h"

UDialogueFactory::UDialogueFactory(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer)
{
	SupportedClass = UDialogue::StaticClass();
}

bool UDialogueFactory::CanCreateNew() const
{
	return true;
}

UObject* UDialogueFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags,
	UObject* Context, FFeedbackContext* Warn)
{
	UDialogue* Dialogue = NewObject<UDialogue>(InParent,InName,Flags);
	return Dialogue;
}
