// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;

UCLASS()
class MASTEREDPROJECT_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* MeshComp;

	UFUNCTION(BlueprintCallable, Category = "weapon")
	void Fire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "weapon")
	UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "weapon")
	UParticleSystem* ImpactEffect;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
