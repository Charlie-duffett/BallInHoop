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
	bool bInCurrentPath = false;
	UPROPERTY(VisibleInstanceOnly)
	bool bInMaze = false;
	UPROPERTY(VisibleInstanceOnly)
	int Face = 0;
	UPROPERTY(VisibleInstanceOnly)
	int Row = 0;
	UPROPERTY(VisibleInstanceOnly)
	int Col = 0;
	UPROPERTY(VisibleInstanceOnly)
	TWeakObjectPtr<UStaticMeshComponent> CellComponent = nullptr;

	// Constructors
	FMazeCell(){}
	FMazeCell(UStaticMeshComponent* CellComponent, int Face, int Row, int Col): 
		CellComponent(CellComponent),
		Face(Face),
		Row(Row),
		Col(Col),
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
	const int MazeTopWidth = 10;
	const int MazeTopHeight = 10;
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

	void AddMazeCubeComponent(ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset, FTransform Transform, int Face, int Row, int Col);
	
	void GenerateMaze();
	void GenerateMazeTop(int Face);
	void GenerateMazeSide(int Face);
	void LoopErasedWalk(int Face, int Row, int Col);

	bool IsNextToMazeCell(int Face, int Row, int Col);
	
	int GetLeftMazeCellIndex(int Face, int Row, int Col);
	int GetUpMazeCellIndex(int Face, int Row, int Col);
	int GetRightMazeCellIndex(int Face, int Row, int Col);
	int GetDownMazeCellIndex(int Face, int Row, int Col);

	bool IsInLoop(int Face, int Row, int Col, int Direction, int& index);

	void RemoveMazeCells();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
