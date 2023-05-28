// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

AGun::AGun()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	
	PrimaryActorTick.bCanEverTick = true;
}

void AGun::BeginPlay()
{
	Super::BeginPlay();	
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (!OwnerController) return false;
	
	FVector ViewPointLocation;
	FRotator ViewPointRotation;
	OwnerController->GetPlayerViewPoint(ViewPointLocation, ViewPointRotation);
	ShotDirection = -ViewPointRotation.Vector();

	FVector EndPoint = ViewPointLocation + ViewPointRotation.Vector() * MaxRange;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(Hit, ViewPointLocation, EndPoint, ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	auto OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return nullptr;

	return OwnerPawn->GetController();
}

void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGun::PullTrigger()
{
	FHitResult HitResult;
	FVector ShotDirection;
	if (GunTrace(HitResult, ShotDirection))
	{		
		UGameplayStatics::SpawnEmitterAtLocation(this, HitParticle, HitResult.ImpactPoint, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, HitResult.ImpactPoint);

		AActor* HitActor = HitResult.GetActor();
		if (HitActor)
		{
			FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			if (!OwnerController) return;
			HitResult.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}

	UGameplayStatics::SpawnEmitterAttached(ShootParticle, MeshComponent, TEXT("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, MeshComponent, TEXT("MuzzleFlashSocket"));
}

