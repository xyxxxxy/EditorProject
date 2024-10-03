#pragma once

#include "CoreMinimal.h"

#include "LyraItemEnum.generated.h"

UENUM(BlueprintType)
enum class ELyraItemCategory
{
	None,
	Prop,
	Equipment,
	Document,
};
