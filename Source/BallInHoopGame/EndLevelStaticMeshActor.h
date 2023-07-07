// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "EndLevelStaticMeshActor.generated.h"

class ATriggerBox;

/**
 * Static mesh actor for the end of the level
 */
UCLASS()
class BALLINHOOPGAME_API AEndLevelStaticMeshActor : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	ATriggerBox* TriggerBox;

	UPROPERTY(EditAnywhere)
	AStaticMeshActor* PlayerBall = nullptr;

	UPROPERTY(EditAnywhere)
	FString EndLevelName = "";

	void ChangeLevel();

};
