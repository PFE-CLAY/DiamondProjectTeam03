#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SubtitleTableRow.generated.h"

UENUM(BlueprintType)
enum class Language : uint8
{
	eng,
	fr,
}; 

USTRUCT(BlueprintType)
struct FSubtitleTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SubTitle)
	TMap<Language, FString> LocSubtitle;
};
