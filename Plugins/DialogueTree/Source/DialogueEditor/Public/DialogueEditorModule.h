
#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FDialogueNodeFactory;

class FDialogueEditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
	void RegisterAssets();
	void UnRegisterAssets();

	void RegisterNodeFactory();
	void UnRegisterNodeFactory();

	void RegisterDetailCustomizations();
	void UnRegisterDetailCustomizations();

	void RegisterCustomClassLayout(const TSubclassOf<UObject> Class, const FOnGetDetailCustomizationInstance DetailLayout);

private:
	TSet<FName> CustomClassLayouts;
	TSharedPtr<FDialogueNodeFactory> NodeFactory;
};
