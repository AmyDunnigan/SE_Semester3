// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Hero.generated.h"

UCLASS()
class SE_SEMESTER3_API AHero : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHero();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float GetHealth() const;
	float GetMaxHealth() const;

	//Dummy functionality
	void Hurt(float Damage, bool bPoison);
	void Heal(float Amount);
	void CurePoison();
	void Respawn();

	bool GetIsPoisoned() const;
	bool GetIsDead() const;
	int GetGold() const;

	void DecreaseHealth();
	
	void Debug_SetHealth(float InHealth);
	void Debug_SetMaxHealth(float InMaxHealth);

private:
	float Health;
	float MaxHealth;
	int RespawnCount;
	bool IsPoisoned;
	bool IsDead;
	FTimerHandle MemberTimerHandle;
	int Gold;

};
