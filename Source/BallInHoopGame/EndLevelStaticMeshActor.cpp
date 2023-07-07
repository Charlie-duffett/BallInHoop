// Fill out your copyright notice in the Description page of Project Settings.


#include "EndLevelStaticMeshActor.h"
#include "Engine/Triggerbox.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"



void AEndLevelStaticMeshActor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	// Check to see if the trigger has overlaped with the supplied Component!
	if (PlayerBall) { // Make sure we have both
		if (TriggerBox->IsOverlappingActor(PlayerBall)) { // If the supplied player has hit the end trigger change level!
            ChangeLevel();
		}
	}
}

void AEndLevelStaticMeshActor::ChangeLevel()
{ // Taken from ChatGPT
    UWorld* World = GEngine->GetWorld();
    if (World && EndLevelName != "")
    {
        FString CurrentLevelName = World->GetMapName(); // Get current level name 

        if (CurrentLevelName != EndLevelName)
        {
            UGameplayStatics::OpenLevel(World, FName(*EndLevelName));
        }
    }
}
