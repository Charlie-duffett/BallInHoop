// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MazePawn.generated.h"

class UStaticMesh;

//Create Struct Datatype to store cells in the maze (Cubes)
USTRUCT()
struct FMazeCell {
	GENERATED_BODY()
	UPROPERTY(VisibleInstanceOnly)
	bool bInCurrentPath;
	UPROPERTY(VisibleInstanceOnly)
	TWeakObjectPtr<UStaticMeshComponent> CellComponent;

	// Constructors
	FMazeCell(): CellComponent(nullptr),
		bInCurrentPath(false) 
	{}
	FMazeCell(UStaticMeshComponent* CellComponent): 
		CellComponent(CellComponent),
		bInCurrentPath(false)
	{}
};

UCLASS()
class BALLINHOOPGAME_API AMazePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMazePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;

	int MazeCubeCounter = 0;
	const int MaxFaces = 6;
	const int MazeTopWidth = 12;
	const int MazeTopHeight = 12;
	const int MazeSideWidth = 11;
	const int MazeSideHeight = 12;

	TArray<FMazeCell> MazeCubeComponents;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MazeBaseCubeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* DefaultSceneRootComponent;


	void AddMazeCubes();
	void AddMazeCubesTop(ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset, bool bSecondPass);
	void AddMazeCubesSide(ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset, bool bSecondPass, int Face);
	
	FVector GetMazeCubeXVector(bool bSecondPass, int Row, int Col) const;
	FVector GetMazeCubeYVector(bool bSecondPass, int Row, int Col) const;
	FVector GetMazeCubeZVector(bool bSecondPass, int Row, int Col) const;

	int GetCubeIndex(int Face, int Row, int Col);

	void AddMazeCubeComponent(ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset, FTransform Transform);
	
	void GenerateMaze();
	void GenerateMazeTop(int Face);
	void GenerateMazeSide(int Face);
	void LoopErasedWalk(int Face, int Row, int Col);

	bool IsNextToMazeCell(int Face, int Row, int Col);
	bool IsLeftMazeCell(int Face, int Row, int Col);
	bool IsUpMazeCell(int Face, int Row, int Col);
	bool IsRightMazeCell(int Face, int Row, int Col);
	bool IsDownMazeCell(int Face, int Row, int Col);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
