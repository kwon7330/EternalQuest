

#pragma once

#include "CoreMinimal.h"
#include "AI/EQBaseFSM.h"
#include "EQRangerFSM.generated.h"


/**
 * 
 */
UCLASS()
class ETERNALQUEST_API UEQRangerFSM : public UEQBaseFSM
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void TickMove() override;
	virtual void TickAttack() override;

	UPROPERTY(EditAnywhere,Category="MonsterAnim")
	UAnimMontage* AnimMontage;

	
};
