// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EQCharacterBase.generated.h"

UCLASS()
class ETERNALQUEST_API AEQCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEQCharacterBase();

protected:
	virtual void OnRep_Owner() override;
	virtual void PossessedBy(AController* NewController) override;
};
