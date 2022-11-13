// Fill out your copyright notice in the Description page of Project Settings.


#include "CMPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACMPlayer::ACMPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ACMPlayer::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = walkingSpeed;
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

