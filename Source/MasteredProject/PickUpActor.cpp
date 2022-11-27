// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"
#include "Components/SphereComponent.h"
#include "Weapons/WeaponBase.h"
#include "Player/CMPlayer.h"

// Sets default values
APickUpActor::APickUpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f);
	RootComponent = SphereComp;
}

// Called when the game starts or when spawned
void APickUpActor::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnItem();
}

void APickUpActor::SpawnItem()
{
	if (PickUpObject == nullptr)
	{	
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	PickUpInstance = GetWorld()->SpawnActor<AActor>(PickUpObject, GetTransform(), SpawnParams);
}

// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickUpActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (PickUpInstance)
	{
		AWeaponBase* weapon = Cast<AWeaponBase>(PickUpInstance);
		if (weapon)
		{
			ACMPlayer* player = Cast<ACMPlayer>(OtherActor);
			if (player)
			{
				player->SetCurrentWeapon(weapon);
			}
		}
		PickUpInstance = nullptr;
	}
}

