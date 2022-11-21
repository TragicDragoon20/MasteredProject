// Fill out your copyright notice in the Description page of Project Settings.


#include "CMPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "HealthComponent.h"
#include "../Weapons/WeaponBase.h"

// Sets default values
ACMPlayer::ACMPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	WeaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ACMPlayer::BeginPlay()
{
	Super::BeginPlay();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(StarterWeapon, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponAttachSocketName);
	}

	GetCharacterMovement()->MaxWalkSpeed = walkingSpeed;

	HealthComp->OnHealthChanged.AddDynamic(this, &ACMPlayer::OnHealthChanged);
}

void ACMPlayer::StartFire()
{
	if (CurrentWeapon)
	{
		bShooting = true;
		CurrentWeapon->Fire();
	}
}

void ACMPlayer::EndFire()
{
	if (CurrentWeapon)
	{
		bShooting = false;
	}
}

void ACMPlayer::OnHealthChanged(UHealthComponent* HealthCompo, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0.0f && !bDied)
	{
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DetachFromControllerPendingDestroy();

		SetLifeSpan(10.0f);
	}
}

// Called every frame
void ACMPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACMPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ACMPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACMPlayer::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ACMPlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRight", this, &ACMPlayer::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ACMPlayer::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ACMPlayer::EndCrouch);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ACMPlayer::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ACMPlayer::EndSprint);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ACMPlayer::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ACMPlayer::EndFire);

}

FVector ACMPlayer::GetPawnViewLocation() const
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

void ACMPlayer::MoveForward(float value)
{
	AddMovementInput(GetActorForwardVector() * value);
}

void ACMPlayer::MoveRight(float value)
{
	AddMovementInput(GetActorRightVector() * value);
}

void ACMPlayer::BeginCrouch()
{
	Crouch();
}

void ACMPlayer::EndCrouch()
{
	UnCrouch();
}

void ACMPlayer::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = runningSpeed;
}

void ACMPlayer::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = walkingSpeed;
}

