

#pragma once

#include "CoreMinimal.h"
#include "Character/EQCharacterNonPlayer.h"
#include "EQNormalEnemy.generated.h"


class UWidgetComponent;
class UEQMonsterAbility;
class UEQBaseFSM;
/**
 * 
 */
UCLASS()
class ETERNALQUEST_API AEQNormalEnemy : public AEQCharacterNonPlayer
{
	GENERATED_BODY()
public:
	AEQNormalEnemy();

	UPROPERTY(EditAnywhere,Category="FSM")
	UEQBaseFSM* BaseFsm;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Ability")
	UEQMonsterAbility* Ability;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Ability")
	UWidgetComponent* HPComp;

	UPROPERTY(EditAnywhere,Category="Ability")
	float DetectRange = 1500.f;
	bool bCanShowHP = false;

	void ActivateEnemy(const FVector& SpawnLocation);
	bool IsActive();
	void Deactivate();
private:
	bool bIsActive;

protected:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)override;


	
	UPROPERTY()
	UDamageType* Type;
	
	void Tick(float DeltaSeconds) override;
	
};
