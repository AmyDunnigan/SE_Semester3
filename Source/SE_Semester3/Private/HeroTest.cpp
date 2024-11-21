// Fill out your copyright notice in the Description page of Project Settings.

#include "Misc/AutomationTest.h"
#include "Tests/AutomationEditorCommon.h"
#include "Tests/AutomationCommon.h"
#include "Hero.h"
#include <Kismet/GameplayStatics.h>

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHeroTestSpawn, "CPPTests.HeroSpawn", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHeroTestSpawn::RunTest(const FString& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	{
		AHero* Hero = World->SpawnActor<AHero>();
		if (Hero->GetHealth() != Hero->GetMaxHealth())
		{
			AddError(TEXT("Newly spawned heros should have max health"));
		}
		Hero->Destroy();
	}
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHeroTestDamage, "CPPTests.HeroDamage",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

bool FHeroTestDamage::RunTest(const FString& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	{
		AHero* Hero = World->SpawnActor<AHero>();
		const float Health = 100;
		Hero->Debug_SetMaxHealth(Health);
		Hero->Debug_SetMaxHealth(Health);
		const float Damage = 10;
		Hero->Hurt(Damage, false);
		if (Hero->GetHealth() != Health - Damage)
		{
			AddError(TEXT("Hurt is not subtracting health"));
		}
		Hero->Destroy();
	}
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHeroTestPoisoned, "CPPTests.HeroPoisoned",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)

	bool FHeroTestPoisoned::RunTest(const FString& Parameters)
{
	UWorld* World = FAutomationEditorCommonUtils::CreateNewMap();
	{
		AHero* Hero = World->SpawnActor<AHero>();
		Hero->Hurt(0, true);
		if (!Hero->GetIsPoisoned())
		{
			AddError(TEXT("Hurt with second parameter true is not poisoning the player"));
		}
		Hero->Destroy();
	}
	return true;
}

/*
Before we could do all of our testing in one function, but now we need parts of
it to wait for gameplay code.
AutomationTest.h has a macro ADD_LATENT_AUTOMATION_COMMAND() that creates a new
instance of the class that is passed in in parentheses and it adds it to a list
of commands to be performed in order.
*/

//Hero's health decreases every second when poisoned

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHeroBrokenTest, "CPPTests.BrokenTest", 
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
	bool FHeroBrokenTest::RunTest(const FString& Parameters)
{
	AHero* Hero = NewObject<AHero>();
	Hero->Hurt(0, true);
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(10.f));
	TestNotEqual("Hero should have decreased health", Hero->GetHealth(),
		Hero->GetMaxHealth());
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FSetupHero, AHero*, Hero);
bool FSetupHero::Update()
{
	Hero = NewObject<AHero>();
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FHeroGoldTest, FAutomationTestBase*, Test, AHero*, Hero);
bool FHeroGoldTest::Update()
{
	Test->TestEqual("Check that gold is zero", Hero->GetGold(), 0);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHeroLatentGoldTest, "CPPTests.LatentGoldTest",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
	bool FHeroLatentGoldTest::RunTest(const FString& Parameters)
{
	AHero* Hero = nullptr;
	ADD_LATENT_AUTOMATION_COMMAND(FSetupHero(Hero));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FHeroGoldTest(this, Hero));
	return true;
}

//Hero's health decreases every second when poisoned

DEFINE_LATENT_AUTOMATION_COMMAND(FSpawnHero);
bool FSpawnHero::Update()
{
	if (GEditor->IsPlayingSessionInEditor())
	{
		GEditor->GetPIEWorldContext()->World()->SpawnActor(AHero::StaticClass());
		return true;
	}
	return false;
}

DEFINE_LATENT_AUTOMATION_COMMAND(FSetHeroHealth);
bool FSetHeroHealth::Update()
{
	if (GEditor->IsPlayingSessionInEditor())
	{
		UWorld* testWorld = GEditor->GetPIEWorldContext()->World();
		AHero* testHero = Cast<AHero>(UGameplayStatics::GetActorOfClass(testWorld,
			AHero::StaticClass()));
		if (testHero)
		{
			testHero->Debug_SetHealth(5);
			return true;
		}
	}
	return false;
}

DEFINE_LATENT_AUTOMATION_COMMAND(FPoisonHero);
bool FPoisonHero::Update()
{
	if (GEditor->IsPlayingSessionInEditor())
	{
		UWorld* testWorld = GEditor->GetPIEWorldContext()->World();
		AHero* testHero = Cast<AHero>(UGameplayStatics::GetActorOfClass(testWorld,
			AHero::StaticClass()));
		if (testHero)
		{
			testHero->Hurt(0, true);
			return true;
		}
	}
	return false;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FHeroDecreaseHealthTest, FAutomationTestBase*,
	Test);
bool FHeroDecreaseHealthTest::Update()
{
	if (GEditor->IsPlayingSessionInEditor())
	{
		UWorld* testWorld = GEditor->GetPIEWorldContext()->World();
		AHero* testHero = Cast<AHero>(UGameplayStatics::GetActorOfClass
		(testWorld, AHero::StaticClass()));
		if (testHero)
		{
			Test->TestNotEqual("Hero should have decreased HP", testHero->GetHealth(),
				testHero->GetMaxHealth());
			return true;
		}
	}
	return false;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FHeroLatentIsPoisonedTest, FAutomationTestBase*,
	Test);
bool FHeroLatentIsPoisonedTest::Update()
{
	if (GEditor->IsPlayingSessionInEditor())
	{
		UWorld* testWorld = GEditor->GetPIEWorldContext()->World();
		AHero* testHero = Cast<AHero>(UGameplayStatics::GetActorOfClass
		(testWorld, AHero::StaticClass()));
		if (testHero)
		{
			Test->TestEqual("Hero should be poisoned", testHero->GetIsPoisoned(),
				true);
			return true;
		}
	}
	return false;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FHeroLatentDeathTest, FAutomationTestBase*,
	Test);
bool FHeroLatentDeathTest::Update()
{
	if (GEditor->IsPlayingSessionInEditor())
	{
		UWorld* testWorld = GEditor->GetPIEWorldContext()->World();
		AHero* testHero = Cast<AHero>(UGameplayStatics::GetActorOfClass
		(testWorld, AHero::StaticClass()));
		if (testHero)
		{
			Test->TestNotEqual("Hero should be dead", testHero->GetIsDead(),
				true);
			return true;
		}
	}
	return false;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHeroTestLatentPoison, "CPPTests.HeroLatentPoison", EAutomationTestFlags::
	EditorContext | EAutomationTestFlags::ProductFilter)
	bool FHeroTestLatentPoison::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(FString("/Game/Tests/CPPTestMap")));
	ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true));
	ADD_LATENT_AUTOMATION_COMMAND(FSpawnHero);
	ADD_LATENT_AUTOMATION_COMMAND(FPoisonHero);
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0));
	ADD_LATENT_AUTOMATION_COMMAND(FHeroDecreaseHealthTest(this));
	ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FHeroTestPoisonToDeath, "CPPTests.HeroPoisonToDeath", EAutomationTestFlags::
	EditorContext | EAutomationTestFlags::ProductFilter)
	bool FHeroTestPoisonToDeath::RunTest(const FString& Parameters)
{
	ADD_LATENT_AUTOMATION_COMMAND(FEditorLoadMap(FString("/Game/Tests/CPPTestMap")));
	ADD_LATENT_AUTOMATION_COMMAND(FStartPIECommand(true));
	ADD_LATENT_AUTOMATION_COMMAND(FSpawnHero);
	ADD_LATENT_AUTOMATION_COMMAND(FSetHeroHealth);
	ADD_LATENT_AUTOMATION_COMMAND(FPoisonHero);
	ADD_LATENT_AUTOMATION_COMMAND(FHeroLatentIsPoisonedTest(this));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.0));
	ADD_LATENT_AUTOMATION_COMMAND(FHeroDecreaseHealthTest(this));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(5.0));
	ADD_LATENT_AUTOMATION_COMMAND(FHeroLatentDeathTest(this));
	ADD_LATENT_AUTOMATION_COMMAND(FEndPlayMapCommand);
	return true;
}