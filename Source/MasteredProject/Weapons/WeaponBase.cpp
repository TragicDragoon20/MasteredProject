// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "particles/ParticleSystem.h"
// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Comp"));
	RootComponent = MeshComp;

	MuzzleSocketName = "MuzzleSocket";
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector ShotDirection = EyeRotation.Vector();

		FVector TraceEnd = EyeLocation + (ShotDirection * 10000);

		FCollisionQueryParams QuerryParams;
		QuerryParams.AddIgnoredActor(MyOwner);
		QuerryParams.AddIgnoredActor(this);	
		QuerryParams.bTraceComplex = true;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QuerryParams))
		{
			AActor* HitActor = Hit.GetActor();

			UGameplayStatics::ApplyPointDamage(HitActor, 20.0f, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);

			if (ImpactEffect)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			}
		}

		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);

		if (MuzzleEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
		}
	}
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

