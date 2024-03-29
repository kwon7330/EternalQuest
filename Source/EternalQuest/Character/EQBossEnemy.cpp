


#include "Character/EQBossEnemy.h"

#include "EQCharacterPlayer.h"
#include "AI/EQMonsterAbility.h"
#include "Player/EQPlayerController.h"
#include "Widget/EQBossMonsterHPUI.h"
#include "Widget/EQReturnTimer.h"

AEQBossEnemy::AEQBossEnemy()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	bReplicates = true;
	SetReplicateMovement(true);
}


void AEQBossEnemy::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	auto Player = Cast<AEQCharacterPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	float Dist = FVector::Dist(Player->GetActorLocation(),this->GetActorLocation());
	
	if(Dist < DetectRange || bCanShowHP == true )
	{
		ShowBossHPBar();
	}
	else
	{
		RemoveBossHPBar();
	}
	
}

bool AEQBossEnemy::GetIsMonsterHit()
{
	return Ability->bIsHit;
}

bool AEQBossEnemy::GetIsMonsterDie()
{
	return Ability->IsDead;
}

void AEQBossEnemy::ShowBossHPBar()
{
	auto UI = CreateWidget<UEQBossMonsterHPUI>(GetWorld(),HPUIFactory);
	UI->AddToViewport();
	UI->SetVisibility(ESlateVisibility::Visible);
	BossHp = Cast<UEQBossMonsterHPUI>(UI);
	UI->UpdateHP(Ability->CurrentHealth,Ability->MaxHealth);
}

void AEQBossEnemy::RemoveBossHPBar()
{
	if(BossHp)
	{
		auto UI = CreateWidget<UEQBossMonsterHPUI>(GetWorld(),HPUIFactory);
		UI->SetVisibility(ESlateVisibility::Hidden);
		BossHp = Cast<UEQBossMonsterHPUI>(UI);
	}
}

void AEQBossEnemy::BackToVillage()
{
	if (HasAuthority())
	{
		UWorld* World = GetWorld();
		if (World)
		{
			FString PathToLevel = FString("/Game/Maps/VillageMap?listen");
			World->ServerTravel(PathToLevel);
			Cast<AEQPlayerController>(World->GetFirstPlayerController())->CreateMainWidget();
		}
	}
}




