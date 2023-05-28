// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	AGun();

	virtual void Tick(float DeltaTime) override;

	void PullTrigger();
	
protected:
	virtual void BeginPlay() override;

private:

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController() const;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ShootParticle;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* MuzzleSound;

	UPROPERTY(EditDefaultsOnly)
	USoundBase* ImpactSound;

	UPROPERTY(EditDefaultsOnly)
	float MaxRange = 1000.f;

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.f;
};
