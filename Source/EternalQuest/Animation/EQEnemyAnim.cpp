


#include "Animation/EQEnemyAnim.h"

#include "AI/EQBaseFSM.h"
#include "AI/EQMonsterAbility.h"
#include "Character/EQAmbushOrc.h"
#include "Character/EQArcherOrc.h"
#include "Character/EQBerserkerOrc.h"
#include "Character/EQMush.h"
#include "Character/EQScorpion.h"
#include "Character/EQSpider.h"
#include "Character/EQWarlockOrc.h"
#include "Character/EQWarriorOrc.h"
#include "Net/UnrealNetwork.h"



void UEQEnemyAnim::AnimNotify_ShootWeb()
{
	AEQSpider* Spider = Cast<AEQSpider>(TryGetPawnOwner());
	if(Spider != nullptr)
	{
		Spider->BaseFsm->ShootWeb();
	}
}

void UEQEnemyAnim::AnimNotify_ScorSkill()
{
	AEQScorpion* Scorpion = Cast<AEQScorpion>(TryGetPawnOwner());
	if(Scorpion != nullptr)
	{
		Scorpion->BaseFsm->ScorpionPrj();
	}
}

void UEQEnemyAnim::AnimNotify_ScorAttack()
{
	AEQScorpion* Scorpion = Cast<AEQScorpion>(TryGetPawnOwner());
	if(Scorpion != nullptr)
	{
		Scorpion->BaseFsm->MeleeAttackCheck();
	}
}

void UEQEnemyAnim::AnimNotify_SuperAmorEnd()
{
	AEQScorpion* Scorpion = Cast<AEQScorpion>(TryGetPawnOwner());
	if(Scorpion != nullptr)
	{
		Scorpion->BaseFsm->SuperAmor = false;
		Scorpion->Ability->HitCount = 0;
	}
}

void UEQEnemyAnim::AnimNotify_DieEnd()
{
	AEQNormalEnemy* NormalEnemy = Cast<AEQNormalEnemy>(TryGetPawnOwner());
	if(NormalEnemy != nullptr)
	{
		NormalEnemy->DieEffect();
		NormalEnemy->BaseFsm->IsDieDone = true;
		//IsDieDone = true;
	}
}

void UEQEnemyAnim::AnimNotify_MushAttack()
{
	AEQMush* Mush = Cast<AEQMush>(TryGetPawnOwner());
	if(Mush != nullptr)
	{
		Mush->BaseFsm->MeleeAttackCheck();
	}
}

void UEQEnemyAnim::AnimNotify_BK_Dodge()
{
	AEQBerserkerOrc* BerserkerOrc = Cast<AEQBerserkerOrc>(TryGetPawnOwner());
	if(BerserkerOrc != nullptr)
	{
		FVector SelfPos = BerserkerOrc->GetActorLocation();
		FVector DodgePos = SelfPos + FVector(-0.000221,0,0);
		BerserkerOrc->SetActorLocation(DodgePos);
	}
}

void UEQEnemyAnim::AnimNotify_ShootArrow()
{
	
	AEQArcherOrc* ArcherOrc = Cast<AEQArcherOrc>(TryGetPawnOwner());
	if(ArcherOrc != nullptr)
	{
		
		ArcherOrc->BaseFsm->ShootArrow();
	}
}

void UEQEnemyAnim::AnimNotify_WarlockSkill()
{
	AEQWarlockOrc* WarlockOrc = Cast<AEQWarlockOrc>(TryGetPawnOwner());
	if(WarlockOrc != nullptr)
	{
		WarlockOrc->BaseFsm->WarlockPrj();
	}
}

void UEQEnemyAnim::AnimNotify_Teleport()
{
	AEQWarlockOrc* WarlockOrc = Cast<AEQWarlockOrc>(TryGetPawnOwner());
	if(WarlockOrc != nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("Teleport!!!!!!!!!!!!!!!!!!!!"));
		WarlockOrc->BaseFsm->WarlockTeleport();
	}
}

void UEQEnemyAnim::AnimNotify_AbAttack()
{
	AEQAmbushOrc* AmbushOrc = Cast<AEQAmbushOrc>(TryGetPawnOwner());
	if( AmbushOrc != nullptr)
	{
		AmbushOrc->BaseFsm->MeleeAttackCheck();
	}
}

void UEQEnemyAnim::AnimNotify_WarriorAttack()
{
	auto* Warrior = Cast<AEQWarriorOrc>(TryGetPawnOwner());
	if( Warrior != nullptr)
	{
		Warrior->BaseFsm->MeleeAttackCheck();
	}
}

void UEQEnemyAnim::AnimNotify_DamageEnd()
{
	AEQBerserkerOrc* BerserkerOrc = Cast<AEQBerserkerOrc>(TryGetPawnOwner());
	if(BerserkerOrc != nullptr)
	{
		BerserkerOrc->EndAttack();
	}
}

void UEQEnemyAnim::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(UEQEnemyAnim, State);
	//DOREPLIFETIME(UEQEnemyAnim, IsDieDone);
}

