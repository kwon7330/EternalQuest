// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EQCharacterPlayer.h"
#include "EternalQuest.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "Engine/AssetManager.h"
#include "Player/EQPlayerController.h"
#include "Player/EQPlayerState.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/EQComponentMove.h"
#include "Component/EQComponentInteraction.h"
#include "Component/EQComponentInventory.h"
#include "Component/EQComponentMenuManager.h"
#include "Component/EQComponentAttack.h"
#include "Component/EQComponentQuest.h"
#include "Component/EQComponentStat.h"
#include "Component/EQComponentWidget.h"
#include "Widget/EQWidgetUserName.h"
#include "Widget/EQWidgetHpBar.h"

AEQCharacterPlayer::AEQCharacterPlayer()
{
	// Replication
	bReplicates = true;
	SetReplicateMovement(true);

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetHiddenInGame(true);

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	HairMesh->SetupAttachment(GetMesh(), TEXT("Hair_Socket"));
	HairMesh->SetHiddenInGame(true);

	HatMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hat"));
	HatMesh->SetupAttachment(GetMesh(), TEXT("Hat_Socket"));
	HatMesh->SetHiddenInGame(true);

	WandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Wand"));
	WandMesh->SetupAttachment(GetMesh(), TEXT("Wand_Socket"));
	WandMesh->SetHiddenInGame(true);

	BookMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Book"));
	BookMesh->SetupAttachment(GetMesh(), TEXT("Book_Socket"));
	BookMesh->SetHiddenInGame(true);

	SwordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Sword"));
	SwordMesh->SetupAttachment(GetMesh(), TEXT("Sword_Socket"));
	SwordMesh->SetHiddenInGame(true);

	ShieldMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shield"));
	ShieldMesh->SetupAttachment(GetMesh(), TEXT("Shield_Socket"));
	ShieldMesh->SetHiddenInGame(true);

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Interaction Box
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetupAttachment(RootComponent);
	InteractionBox->SetBoxExtent(FVector(50));
	InteractionBox->SetRelativeLocation(FVector(120, 0, -50));

	// Input
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Blueprints/Input/IMC_Default.IMC_Default'"));
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	// Component
	MoveComp = CreateDefaultSubobject<UEQComponentMove>(TEXT("Move Component"));
	InteractionComp = CreateDefaultSubobject<UEQComponentInteraction>(TEXT("Interaction Component"));
	InventoryComp = CreateDefaultSubobject<UEQComponentInventory>(TEXT("Inventory Component"));
	MenuManagerComp = CreateDefaultSubobject<UEQComponentMenuManager>(TEXT("MenuManager Component"));
	AttackComp = CreateDefaultSubobject<UEQComponentAttack>(TEXT("Attack Component"));
	StatComp = CreateDefaultSubobject<UEQComponentStat>(TEXT("Stat Component"));
	UserNameComp = CreateDefaultSubobject<UEQComponentWidget>(TEXT("UserName Component"));
	HpBarComp = CreateDefaultSubobject<UEQComponentWidget>(TEXT("HpBar Component"));
	QuestComp = CreateDefaultSubobject<UEQComponentQuest>(TEXT("Quest Component"));

	UserNameComp->SetupAttachment(GetMesh());
	UserNameComp->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
	UserNameComp->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> UserNameWidgetRef(TEXT("/Game/Blueprints/Widget/WBP_UserName.WBP_UserName_C"));
	if (UserNameWidgetRef.Succeeded())
	{
		UserNameComp->SetWidgetClass(UserNameWidgetRef.Class);
		UserNameComp->SetWidgetSpace(EWidgetSpace::Screen);
		UserNameComp->SetDrawSize(FVector2D(150.0f, 15.0f));
		UserNameComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	HpBarComp->SetupAttachment(GetMesh());
	HpBarComp->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HpBarComp->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/Blueprints/Widget/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Succeeded())
	{
		HpBarComp->SetWidgetClass(HpBarWidgetRef.Class);
		HpBarComp->SetWidgetSpace(EWidgetSpace::Screen);
		HpBarComp->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBarComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AEQCharacterPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	EQ_LOG(LogEternalQuest, Log, TEXT("%s"), *GetName(), TEXT("Begin"));

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	EQ_LOG(LogEternalQuest, Log, TEXT("%s"), *GetName(), TEXT("End"));
}

void AEQCharacterPlayer::PostInitializeComponents()
{
	EQ_LOG(LogEternalQuest, Log, TEXT("%s %s"), *GetName(), TEXT("Begin"));

	Super::PostInitializeComponents();

	EQ_LOG(LogEternalQuest, Log, TEXT("%s"), TEXT("End"));

	StatComp->OnHpZero.AddUObject(this, &ThisClass::SetDead);
	StatComp->OnStatChanged.AddUObject(this, &ThisClass::ApplyStat);
}

void AEQCharacterPlayer::PossessedBy(AController* NewController)
{
	EQ_LOG(LogEternalQuest, Log, TEXT("%s %s"), *GetName(), TEXT("Begin"));

	Super::PossessedBy(NewController);

	EQ_LOG(LogEternalQuest, Log, TEXT("%s"), TEXT("End"));

	UpdateMesh();
}

void AEQCharacterPlayer::OnRep_Owner()
{
	EQ_LOG(LogEternalQuest, Log, TEXT("%s"), *GetName(), TEXT("Begin"));

	Super::OnRep_Owner();

	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		EQ_LOG(LogEternalQuest, Log, TEXT("Owner : %s"), *Owner->GetName());
	}
	else
	{
		EQ_LOG(LogEternalQuest, Log, TEXT("%s"), TEXT("No Owner"));
	}

	EQ_LOG(LogEternalQuest, Log, TEXT("%s"), TEXT("End"));
}

void AEQCharacterPlayer::OnRep_PlayerState()
{
	EQ_LOG(LogEternalQuest, Log, TEXT("%s"), *GetName(), TEXT("Begin"));

	Super::OnRep_PlayerState();

	EQ_LOG(LogEternalQuest, Log, TEXT("%s"), *GetName(), TEXT("End"));

	UpdateMesh();
}

void AEQCharacterPlayer::PostNetInit()
{
	EQ_LOG(LogEternalQuest, Log, TEXT("%s"), *GetName(), TEXT("Begin"));

	Super::PostNetInit();

	EQ_LOG(LogEternalQuest, Log, TEXT("%s"), TEXT("End"));
}

void AEQCharacterPlayer::BeginPlay()
{
	EQ_LOG(LogEternalQuest, Log, TEXT("%s"), TEXT("Begin"));

	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetPlayerController();
}

void AEQCharacterPlayer::AttackHitCheck()
{
	AttackComp->HitCheck();
}

float AEQCharacterPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	StatComp->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void AEQCharacterPlayer::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	PlayDeadAnimation();
	SetActorEnableCollision(false);
	HpBarComp->SetHiddenInGame(true);
	
	//APlayerController* PlayerController = Cast<APlayerController>(GetController());
	//if (PlayerController)
	//{
	//	DisableInput(PlayerController);
	//}
}

void AEQCharacterPlayer::PlayDeadAnimation()
{
	UAnimInstance* Animinstance = GetMesh()->GetAnimInstance();
	Animinstance->StopAllMontages(0.0f);
	Animinstance->Montage_Play(DeadMontage, 1.0f);
}

int32 AEQCharacterPlayer::GetLevel()
{
	return StatComp->GetCurrentLevel();
}

void AEQCharacterPlayer::SetLevel(int32 InNewLevel)
{
	StatComp->SetLevelStat(InNewLevel);
}

void AEQCharacterPlayer::ApplyStat(const FEQCharacterStat& BaseStat, const FEQCharacterStat& ModifierStat)
{
	StatComp->ApplyStat(BaseStat, ModifierStat);
}

void AEQCharacterPlayer::SetupCharacterWidget(UEQWidgetBase* InWidgetBase)
{
	/*UEQWidgetUserName* UserNameWidget = Cast<UEQWidgetUserName>(Cast<UEQWidgetBase>(UserNameComp->GetWidget()));
	if (UserNameWidget)
	{
		const APlayerController* PlayerController = UserNameWidget->GetOwningPlayer();
		const APlayerState* EQPlayerState = PlayerController->GetPlayerState<APlayerState>();
		const FString UserName = EQPlayerState->GetPlayerName();
		UserNameWidget->SetUserName(FText::FromString(UserName));
	}*/

	UEQWidgetHpBar* HpBarWidget = Cast<UEQWidgetHpBar>(InWidgetBase);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateHpBar(StatComp->GetCurrentHp(), StatComp->GetMaxHp());
		StatComp->OnHpChanged.AddUObject(HpBarWidget, &UEQWidgetHpBar::UpdateHpBar);
	}
}

void AEQCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputSignature.Broadcast(PlayerInputComponent);
}

void AEQCharacterPlayer::Jump()
{
	Super::Jump();
}

void AEQCharacterPlayer::StopJumping()
{
	Super::StopJumping();
}

void AEQCharacterPlayer::SetPlayerController()
{
	if (!IsLocallyControlled())
	{
		return;
	}

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AEQCharacterPlayer::UpdateMesh()
{
	ClassType = Cast<UEQGameInstance>(GetGameInstance())->GetClassType();

	if (IsLocallyControlled())
	{
		SwitchClassType(ClassType, this);
	}

	if (!HasAuthority())
	{
		Server_UpdateMesh(ClassType);
	}
}

void AEQCharacterPlayer::Server_UpdateMesh_Implementation(EClassType InClassType)
{
	for (AEQCharacterPlayer* CharacterPlayer : TActorRange<AEQCharacterPlayer>(GetWorld()))
	{
		if (CharacterPlayer->IsLocallyControlled())
		{
			continue;
		}
		else
		{
			CharacterPlayer->ClassType = InClassType;
			SwitchClassType(CharacterPlayer->ClassType, CharacterPlayer);
		}
	}

	EClassType CClassType = GetWorld()->GetFirstPlayerController()->GetPlayerState<AEQPlayerState>()->GetClassType();
	Client_UpdateMesh(CClassType);
}

void AEQCharacterPlayer::Client_UpdateMesh_Implementation(EClassType InClassType)
{
	for (AEQCharacterPlayer* CharacterPlayer : TActorRange<AEQCharacterPlayer>(GetWorld()))
	{
		if (CharacterPlayer->IsLocallyControlled())
		{
			continue;
		}
		else
		{
			CharacterPlayer->ClassType = InClassType;
			SwitchClassType(CharacterPlayer->ClassType, CharacterPlayer);
		}
	}
}

void AEQCharacterPlayer::SwitchClassType(EClassType InClassType, AEQCharacterPlayer* CharacterPlayer)
{
	switch (InClassType)
	{
	case EClassType::ECT_Mage:
		CharacterPlayer->BodyMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(BodyMeshes[9], FStreamableDelegate::CreateUObject(CharacterPlayer, &ThisClass::BodyMeshLoadCompleted));
		CharacterPlayer->HairMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(HairMeshes[7], FStreamableDelegate::CreateUObject(CharacterPlayer, &ThisClass::HairMeshLoadCompleted));
		CharacterPlayer->HatMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(HatMeshes[0], FStreamableDelegate::CreateUObject(CharacterPlayer, &ThisClass::HatMeshLoadCompleted));
		CharacterPlayer->WandMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(WandMeshes[0], FStreamableDelegate::CreateUObject(CharacterPlayer, &ThisClass::WandMeshLoadCompleted));
		CharacterPlayer->BookMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(BookMeshes[0], FStreamableDelegate::CreateUObject(CharacterPlayer, &ThisClass::BookMeshLoadCompleted));
		break;
	case EClassType::ECT_Paladin:
		break;
	case EClassType::ECT_Priest:
		break;
	case EClassType::ECT_Rogue:
		break;
	case EClassType::ECT_Warrior:
		CharacterPlayer->BodyMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(BodyMeshes[4], FStreamableDelegate::CreateUObject(CharacterPlayer, &ThisClass::BodyMeshLoadCompleted));
		CharacterPlayer->HairMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(HairMeshes[0], FStreamableDelegate::CreateUObject(CharacterPlayer, &ThisClass::HairMeshLoadCompleted));
		CharacterPlayer->SwordMeshHandle = UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(SwordMeshes[0], FStreamableDelegate::CreateUObject(CharacterPlayer, &ThisClass::SwordMeshLoadCompleted));
		break;
	}
}


void AEQCharacterPlayer::BodyMeshLoadCompleted()
{
	if (BodyMeshHandle.IsValid())
	{
		USkeletalMesh* BodyMeshRef = Cast<USkeletalMesh>(BodyMeshHandle->GetLoadedAsset());
		if (BodyMeshRef)
		{
			GetMesh()->SetSkeletalMesh(BodyMeshRef);
			GetMesh()->SetHiddenInGame(false);
		}
	}

	BodyMeshHandle->ReleaseHandle();
}

void AEQCharacterPlayer::HairMeshLoadCompleted()
{
	if (HairMeshHandle.IsValid())
	{
		USkeletalMesh* HairMeshRef = Cast<USkeletalMesh>(HairMeshHandle->GetLoadedAsset());
		if (HairMeshRef)
		{
			HairMesh->SetSkeletalMesh(HairMeshRef);
			HairMesh->SetHiddenInGame(false);
		}
	}

	HairMeshHandle->ReleaseHandle();
}

void AEQCharacterPlayer::HatMeshLoadCompleted()
{
	if (HatMeshHandle.IsValid())
	{
		USkeletalMesh* HatMeshRef = Cast<USkeletalMesh>(HatMeshHandle->GetLoadedAsset());
		if (HatMeshRef)
		{
			HatMesh->SetSkeletalMesh(HatMeshRef);
			HatMesh->SetHiddenInGame(false);
		}
	}

	HatMeshHandle->ReleaseHandle();
}

void AEQCharacterPlayer::WandMeshLoadCompleted()
{
	if (WandMeshHandle.IsValid())
	{
		USkeletalMesh* WandMeshRef = Cast<USkeletalMesh>(WandMeshHandle->GetLoadedAsset());
		if (WandMeshRef)
		{
			WandMesh->SetSkeletalMesh(WandMeshRef);
			WandMesh->SetHiddenInGame(false);
		}
	}

	WandMeshHandle->ReleaseHandle();
}

void AEQCharacterPlayer::BookMeshLoadCompleted()
{
	if (BookMeshHandle.IsValid())
	{
		USkeletalMesh* BookMeshRef = Cast<USkeletalMesh>(BookMeshHandle->GetLoadedAsset());
		if (BookMeshRef)
		{
			BookMesh->SetSkeletalMesh(BookMeshRef);
			BookMesh->SetHiddenInGame(false);
		}
	}

	BookMeshHandle->ReleaseHandle();
}

void AEQCharacterPlayer::SwordMeshLoadCompleted()
{
	if (SwordMeshHandle.IsValid())
	{
		USkeletalMesh* SwordMeshRef = Cast<USkeletalMesh>(SwordMeshHandle->GetLoadedAsset());
		if (SwordMeshRef)
		{
			SwordMesh->SetSkeletalMesh(SwordMeshRef);
			SwordMesh->SetHiddenInGame(false);
		}
	}

	SwordMeshHandle->ReleaseHandle();
}

void AEQCharacterPlayer::ShieldMeshLoadCompleted()
{
	if (ShieldMeshHandle.IsValid())
	{
		USkeletalMesh* ShieldMeshRef = Cast<USkeletalMesh>(ShieldMeshHandle->GetLoadedAsset());
		if (ShieldMeshRef)
		{
			ShieldMesh->SetSkeletalMesh(ShieldMeshRef);
			ShieldMesh->SetHiddenInGame(false);
		}
	}

	ShieldMeshHandle->ReleaseHandle();
}
