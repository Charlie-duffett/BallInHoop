// Fill out your copyright notice in the Description page of Project Settings.


#include "MazePawn.h"

// Sets default values
AMazePawn::AMazePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMazePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMazePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMazePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called to Generate the maze will also store an array of the different face objects
void AMazePawn::GenerateMaze() {
	UE_LOG(LogTemp, Warning, TEXT("AMazePawn::GenerateMaze() Called!"));
	for (int Face = 0; Face < 6; Face++) { // Loop over each face of the cube
		if (Face % 3 == 0) {
			// Loop over the columns and rows for the face. 
			// We do it different here as 1 set of faces will need to be 1 cube wider on all edges
			for (int Row = 0; Row < 12; Row++) {
				for (int Col = 0; Col < 12; Col++) {

				}
			}
		}
		else {

		}
	}
}

void AMazePawn::GenerateMazeTop() {

}

void AMazePawn::GenerateMazeBottom() {

}