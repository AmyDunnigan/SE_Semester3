// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero.h"

// Sets default values
AHero::AHero()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MaxHealth = 10;
	Health = MaxHealth;
	Gold = 0;
}

// Called when the game starts or when spawned
void AHero::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AHero::DecreaseHealth,
		1.0f, true, 1.0f);
	
}

// Called every frame
void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AHero::GetHealth() const
{
	return Health;
}

float AHero::GetMaxHealth() const
{
	return MaxHealth;
}

void AHero::Hurt(float Damage, bool bPoison)
{
	if(Damage < 0)
	{
		Damage = 0;
	}
	Health = FMath::Clamp<float>(Health - Damage, 0, MaxHealth);

	if (bPoison)
	{
		IsPoisoned = true;
	}
}

void AHero::Heal(float Amount)
{
}

void AHero::CurePoison()
{
}

void AHero::Respawn()
{
}

bool AHero::GetIsPoisoned() const
{
	return IsPoisoned;
}

bool AHero::GetIsDead() const
{
	return false;
}

int AHero::GetGold() const
{
	return 0;
}

void AHero::DecreaseHealth()
{
	if(IsPoisoned)
	{
		Hurt(1, true);
	}
}

void AHero::Debug_SetHealth(float InHealth)
{
	Health = InHealth;
}

void AHero::Debug_SetMaxHealth(float InMaxHealth)
{
	MaxHealth = InMaxHealth;
}
