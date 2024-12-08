//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "MyNewPlayerControllerTest.h"
//#include "Misc/AutomationTest.h"
//#include "Tests/AutomationEditorCommon.h"
//#include "Tests/AutomationCommon.h"
//#include <Kismet/GameplayStatics.h>
//#include <MyNewPlayerController.h>
//
//// Editor context defines that we want to be able to run this test in the editor
//// Product filter defines how long the test will take to run
//
//IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMyNewPlayerControllerTest, "CPPTest.MyNewPlayerController", EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter)
//
//// Your function must be called RunTest and it overrides the RunTest from FAutomationTestBase
//// FDecreaseHealthIncrementTest here must match the one in the macro above
//bool FMyNewPlayerControllerTest::RunTest(const FString& Parameters)
//{
//	AMyNewPlayerController* MyNewPlayerController = NewObject<AMyNewPlayerController>();
//	if (MyNewPlayerController->GetHealth() != MyNewPlayerController->GetMaxHealth())
//	{
//		//If we call AddError, the test fails
//		AddError(TEXT("Newly spawned MyPlayerController should have max health"));
//	}
//	return true;
//}
//
//MyNewPlayerControllerTest::~MyNewPlayerControllerTest()
//{
//}
