// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MazePawn.generated.h"

class UStaticMesh;

UCLASS()
class BALLINHOOPGAME_API AMazePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMazePawn();
	UFUNCTION(BlueprintCallable)
	void GenerateMaze();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MazeBaseCubeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRootComponent;


	void AddMazeCubes();

	void GenerateMazeTop(int Face);
	void GenerateMazeBottom(int Face);

	void AddStaticMeshToXFace(int Row, int Col, bool bSecondPass);
	void AddStaticMeshToYFace(int Row, int Col, bool bSecondPass);
	void AddStaticMeshToZFace(int Row, int Col, bool bSecondPass);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
