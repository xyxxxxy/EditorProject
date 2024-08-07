
#include "DialogueEditorModule.h"

#include "Asset/DialogueAssetAction.h"
#include "DialogueEditorStyle.h"
#include "IAssetTools.h"
#include "DetailCustomizations/DialogueGraphCustomization.h"
#include "Graph/DialogueEdGraph.h"
#include "Graph/Nodes/DialogueNodeFactory.h"

#define LOCTEXT_NAMESPACE "FDialogueEditorModule"

void FDialogueEditorModule::StartupModule()
{
	FDialogueEditorStyle::Initialize();

	RegisterNodeFactory();
	RegisterAssets();
	RegisterDetailCustomizations();

}

void FDialogueEditorModule::ShutdownModule()
{
	FDialogueEditorStyle::ShutDown();

	UnRegisterNodeFactory();
}

void FDialogueEditorModule::RegisterAssets()
{
	IAssetTools& AssetToolsModule = IAssetTools::Get();
	EAssetTypeCategories::Type AssetType = AssetToolsModule.RegisterAdvancedAssetCategory(
		FName("DialogueTree"),
		LOCTEXT("AdvancedAssetCategory","Dialogue Tree"));

	TSharedPtr<FDialogueAssetAction> DialogueAssetAction = MakeShareable(new FDialogueAssetAction(AssetType));
	AssetToolsModule.RegisterAssetTypeActions(DialogueAssetAction.ToSharedRef());
}

void FDialogueEditorModule::UnRegisterAssets()
{
}

void FDialogueEditorModule::RegisterNodeFactory()
{
	NodeFactory = MakeShareable(new FDialogueNodeFactory());
	FEdGraphUtilities::RegisterVisualNodeFactory(NodeFactory);
}

void FDialogueEditorModule::UnRegisterNodeFactory()
{
	if (NodeFactory.IsValid())
	{
		FEdGraphUtilities::UnregisterVisualNodeFactory(NodeFactory);
		NodeFactory.Reset();
	}
}

void FDialogueEditorModule::RegisterDetailCustomizations()
{
	if(FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

		RegisterCustomClassLayout(UDialogueEdGraph::StaticClass(),FOnGetDetailCustomizationInstance::CreateStatic(FDialogueGraphCustomization::MakeInstance));
		
	}
}

void FDialogueEditorModule::UnRegisterDetailCustomizations()
{
}

void FDialogueEditorModule::RegisterCustomClassLayout(const TSubclassOf<UObject> Class,
	const FOnGetDetailCustomizationInstance DetailLayout)
{
	if(Class)
	{
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.RegisterCustomClassLayout(Class->GetFName(),DetailLayout);

		CustomClassLayouts.Add(Class->GetFName());
	}
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDialogueEditorModule, DialogueEditor)