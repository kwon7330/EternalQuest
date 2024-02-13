


#include "AI/EQMonsterAbility.h"

#include "AIController.h"
#include "EQBaseFSM.h"
#include "Character/EQCharacterBase.h"
#include "Character/EQCharacterNonPlayer.h"
#include "Character/EQNormalEnemy.h"


UEQMonsterAbility::UEQMonsterAbility()
{
	
	PrimaryComponentTick.bCanEverTick = false;

	MaxHealth = 100;
	CurrentHealth = MaxHealth;
	
}



void UEQMonsterAbility::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if(Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this,&UEQMonsterAbility::TakeDamage);
	}
	
}

void UEQMonsterAbility::UpdateHP(float UpdateHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("Current Health222: %f"), CurrentHealth);
	CurrentHealth = FMath::Max(0,CurrentHealth+UpdateHealth);
	UE_LOG(LogTemp,Warning,TEXT("damage: %.1f, curent Health: %.1f"), UpdateHealth, CurrentHealth);
}


void UEQMonsterAbility::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UEQMonsterAbility::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	AController* InstigatedBy, AActor* DamageCauser)
{
	if(Damage <= 0)
	{
		return;
	}

	UpdateHP(-Damage);
	
	if(DamagedActor->IsA<AEQNormalEnemy>())
	{
		auto Monster = Cast<AEQNormalEnemy>(DamagedActor);
		if(CurrentHealth > 0)
		{
			Monster->BaseFsm->AI->StopMovement();
			Monster->BaseFsm->SetState(EMonsterState::Hit);
		}
		else
		{
			Monster->BaseFsm->SetState(EMonsterState::Die);
		}
	}
}



