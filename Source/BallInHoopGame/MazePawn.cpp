// Fill out your copyright notice in the Description page of Project Settings.


#include "MazePawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Misc/CString.h"
#include "Math/UnrealMathUtility.h"
#include "UObject/ConstructorHelpers.h"



// Sets default values
AMazePawn::AMazePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	DefaultSceneRootComponent->SetMobility(EComponentMobility::Movable);
	DefaultSceneRootComponent->SetupAttachment(RootComponent);
	DefaultSceneRootComponent->RegisterComponent();

	RootComponent = DefaultSceneRootComponent;

	// Created base Maze cube /Script/Engine.StaticMesh'/Game/Meshes/SM_MazeBase.SM_MazeBase'
	static auto MazeBase = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/Meshes/SM_MazeBase.SM_MazeBase'"));

	MazeBaseCubeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MazeBase"));
	if (IsValid(MazeBaseCubeComponent))
	{
		//RootComponent = CubeMeshComponent;

		if (MazeBase.Succeeded())
		{
			MazeBaseCubeComponent->Mobility = EComponentMobility::Movable;
			MazeBaseCubeComponent->CastShadow = false;
			if (MazeBaseCubeComponent->SetStaticMesh(MazeBase.Object)) {
				UE_LOG(LogTemp, Warning, TEXT("Static Mesh Set!"));
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Static Mesh Not Set!"));
			}
			MazeBaseCubeComponent->SetupAttachment(RootComponent);
			MazeBaseCubeComponent->RegisterComponent();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("MazeBase Is Missing!"));
		}
	}

	AddMazeCubes();
	UE_LOG(LogTemp, Log, TEXT("Constructor Ran!"));
	
}

// Called when the game starts or when spawned
void AMazePawn::BeginPlay()
{
	Super::BeginPlay();

	//GenerateMaze();
}

void AMazePawn::EndPlay(EEndPlayReason::Type EndReason)
{
	Super::EndPlay(EndReason);
	// Iterate through the array and destroy each component
	for (FMazeCell MeshComponent : MazeCubeComponents)
	{
		if (MeshComponent.CellComponent != nullptr) { // Clean up none deleted elements
			MeshComponent.CellComponent->DestroyComponent();
		}
	}

	// Clear the array
	MazeCubeComponents.Empty();
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

void AMazePawn::AddMazeCubes() { 
	static auto CubeMeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	for (int Face = 0; Face < MaxFaces; Face++) { // Loop over each face of the cube
		// We need to do 2 slightly different loops depending if the face is a "top piece" or "side piece" :D
		// See GenerateMazeTop or GenerateMazeBottom for more info
		if (Face % 3 == 0) {
			AddMazeCubesTop(CubeMeshAsset, Face > 2);
		}
		else {
			AddMazeCubesSide(CubeMeshAsset, Face > 2, Face);
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Counter Total: %d"), MazeCubeCounter);
	UE_LOG(LogTemp, Log, TEXT("Max Index Total: %d"), GetCubeIndex(5, 9, 10));
}

void AMazePawn::AddMazeCubesTop(ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset, bool bSecondPass) {
	for (int Row = 0; Row < MazeTopHeight; Row++) {
		for (int Col = 0; Col < MazeTopWidth; Col++) {
			FVector RelativePos = GetMazeCubeZVector(bSecondPass, Row, Col);
			FTransform Transform(RelativePos);
			AddMazeCubeComponent(CubeMeshAsset, Transform, (int)bSecondPass * 3, Row, Col);
		}
	}
}

void AMazePawn::AddMazeCubesSide(ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset, bool bSecondPass, int Face) {
	FVector RelativePos;
	for (int Row = 0; Row < MazeSideHeight; Row++) {
		for (int Col = 0; Col < MazeSideWidth; Col++) {
			if (Face % 3 == 1) {
				RelativePos = GetMazeCubeXVector(bSecondPass, Row, Col);
			}
			else {
				RelativePos = GetMazeCubeYVector(bSecondPass, Row, Col);
				UE_LOG(LogTemp, Log, TEXT("Value of bSecondPass %d"), (int)bSecondPass);
			}
			
			FTransform Transform(RelativePos);
			AddMazeCubeComponent(CubeMeshAsset, Transform, Face, Row, Col);
		}
	}
}

FVector AMazePawn::GetMazeCubeXVector(bool bSecondPass, int Row, int Col) const {
	// Work out the relative position of the static mesh.
	int x, y, z;
	x = -550 + ((int)bSecondPass * 1100); 
	if (bSecondPass) {
		y = 450 - (Col * 100);// We want alternate which side has the extra col
	}
	else {
		y = -450 + (Col * 100);// We want alternate which side has the extra col
	}
	
	z = -550 + (Row * 100);

	return FVector(x, y, z);
}

FVector AMazePawn::GetMazeCubeYVector(bool bSecondPass, int Row, int Col) const  {
	// Work out the relative position of the static mesh.
	int x, y, z;
	if (bSecondPass) {
		x = (-450 - (Col * -100));
	}
	else {
		x = (450 + (Col * -100));
	}
	y = -550 + ((int)bSecondPass * 1100);// If it is the 2nd pass we want to move the position to the other side
	z = -550 + (Row * 100); 

	return FVector(x, y, z);
}

FVector AMazePawn::GetMazeCubeZVector(bool bSecondPass, int Row, int Col) const  {
	// Work out the relative position of the static mesh.
	int x, y, z;
	x = -450 + (Row * 100);
	y = -450 + (Col * 100); // Here we will always have an extra col so we dont need any logic
	z = 550 + ((int)bSecondPass * -1100);// If it is the 2nd pass we want to move the position to the other side

	return FVector(x, y, z);
}

int AMazePawn::GetCubeIndex(int Face, int Row, int Col) {
	int Index = 0;
	for (int i = 0; i < Face; i++) { // Loop over all the previous faces to workout the index before the current face.
		if (i % 3 == 0) { // If its a "top" part of the maze then it will cover more indexes
			Index += MazeTopWidth * MazeTopHeight;
		}
		else {
			Index += MazeSideWidth * MazeSideHeight;
		}
	}

	if (Face % 3 == 0) { // If its a "top" part of the maze then it will cover more indexes
		Index += (Row * MazeTopWidth) + Col; // add the index of where it would be on the current face.
	}
	else {
		Index += (Row * MazeSideWidth) + Col; // add the index of where it would be on the current face.
	}
	
	return Index;
}

void AMazePawn::AddMazeCubeComponent(ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset, FTransform Transform, int Face, int Row, int Col) {
	//Create new static mesh object
	UStaticMeshComponent* CubeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("MazeCube %s"), *FString::FromInt(MazeCubeCounter++)));
	if (CubeMeshComponent)
	{
		if (CubeMeshAsset.Succeeded())
		{
			//UE_LOG(LogTemp, Log, TEXT("CubeMeshAssetNew Exists!"));
			// Set the static mesh of the cube component
			CubeMeshComponent->Mobility = EComponentMobility::Movable;
			CubeMeshComponent->CastShadow = false;
			CubeMeshComponent->SetStaticMesh(CubeMeshAsset.Object);
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("CubeMeshAssetNew Is Missing!"));
		}

		// Attach the cube component to the root component or another desired parent component
		if (MazeBaseCubeComponent) {
			//UE_LOG(LogTemp, Log, TEXT("MazeBaseCubeComponent Exists!"));
			CubeMeshComponent->SetupAttachment(MazeBaseCubeComponent);
			MazeBaseCubeComponent->RegisterComponent();
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("MazeBaseCubeComponent is Missing!!"));
		}

		CubeMeshComponent->SetRelativeTransform(Transform);
		
		MazeCubeComponents.Push(FMazeCell(CubeMeshComponent, Face, Row, Col)); // We dont care about a return value so we call push instead of add
	}
}

void AMazePawn::GenerateMaze() {
	// Current plan for Maze Generation is to use the erasedLoopWalk Method IE (Wilsons Algorithm)
	// Start in the same place of the top face of the cube (saves issues later)
	// First pass will randomly pick a direction to go and move 2 spaces each time untill it hits itself.
	// Once it has hit itself remove all the cubes in that "trail" and start the actual maze process.
	// Now we want to start at index 0 in our array and just go from there. This will ensure all places are done.
	// First check that this place isnt already a part of the maze by verifying CellComponent isnt null
	// Then double check that this cube isnt directly touching any completed maze piece (to ensure we keep walls inplace)
	// Then randomly pick a direction and move 2 places. Each time it moves it sets that point to bInCurrentPath=true
	// If it hits itself on that journey it should pop out all the elements of the trail until it goes
	// back to the point it has hit. Note it must set bInCurrentPath=false again
	// then start from that point
	// When it hits a part that is already in the maze loop through all of the trail
	// and set bInCurrentPath aswell as destory all the components leaving the pointer as a nullptr.
	// repeat until there are no places left

	// First we remove one cube to set something to be part of the maze.
	int Index = GetCubeIndex(0, 5, 5);
	MazeCubeComponents[Index].CellComponent->DestroyComponent();
	MazeCubeComponents[Index].CellComponent = nullptr;

	// Now we just loop through the whole array to randomly generate the maze
	for (int Face = 0; Face < MaxFaces; Face++) { // Loop over each face of the cube
		// We need to do 2 slightly different loops depending if the face is a "top piece" or "side piece" :D
		// See GenerateMazeTop or GenerateMazeBottom for more info
		if (Face % 3 == 0) {
			
		}
		else {
			
		}
	}
}

void AMazePawn::GenerateMazeTop(int Face) {
	for (int Row = 0; Row < MazeTopHeight; Row++) {
		for (int Col = 0; Col < MazeTopWidth; Col++) {

		}
	}
}

void AMazePawn::GenerateMazeSide(int Face) {
	for (int Row = 0; Row < MazeSideHeight; Row++) {
		for (int Col = 0; Col < MazeSideWidth; Col++) {

		}
	}
}

void AMazePawn::LoopErasedWalk(int Face, int Row, int Col) {
	int CurrentFace = Face;
	int CurrentRow = Row;
	int CurrentCol = Col;
	int CurrentIndex = GetCubeIndex(CurrentFace, CurrentRow, CurrentCol);
	TArray<FMazeCell*> MazeCurrentTrail;

	// First Verify that this cell isnt already touching a part of the maze
	// We must check in every direction
	if (IsNextToMazeCell(CurrentFace, CurrentRow, CurrentCol)) {
		// if this is next to a maze cell we want to move to the next cell
		return;
	}

	//Push starting point into trail
	MazeCurrentTrail.Push(&MazeCubeComponents[CurrentIndex]);
	MazeCubeComponents[CurrentIndex].bInCurrentPath = true;

	bool bSearchingForMazeCell = true;

	while (bSearchingForMazeCell) {
		// Decide which way to move 0=Left 1=Up 2=Right 3=Down
		int Direction = FMath::RandRange(0, 3);
		for (int i = 0; i < 2; i++) {
			if (Direction == 0) {
				--CurrentCol;
			}
			else if (Direction == 1) {
				--CurrentRow;
			}
			else if (Direction == 2) {
				++CurrentCol;
			}
			else if (Direction == 3) {
				++CurrentRow;
			}
		}
	}
}

bool AMazePawn::IsNextToMazeCell(int Face, int Row, int Col) {
	


	return false;
}

int AMazePawn::GetLeftMazeCellIndex(int Face, int Row, int Col) {
	int NewFace = 0;
	int NewRow = 0;
	int NewCol = Col - 1;
	
	if (NewCol < 0) { // This means we have switched face!
		if (Face % 3 == 0) {
			if (Face == 0) {
				NewFace = 2;
				NewRow = (MazeSideHeight-1); /// Removing one from side height as we start counting from 0
				NewCol = MazeSideWidth - Row - 2; // adjusting for offset here
			}
			else {
				NewFace = 2;
				NewRow = Col; /// Removing one from side height as we start counting from 0
				NewCol = MazeSideWidth - Row - 2;
			}
		}
		else {
			if (Face % 3 == 1) {
				NewFace = Face+1;
				NewRow = Row;
				NewCol = MazeSideWidth - 1;
			}
			else {
				NewFace = Face + 2;
				NewRow = Row;
				NewCol = MazeSideWidth - 1;
				if (NewFace > 5) {
					NewFace = 1;
				}
			}
		}
	}
	return GetCubeIndex(NewFace, NewRow, NewCol); // No new face here so just return the index with the updated col
}

int AMazePawn::GetUpMazeCellIndex(int Face, int Row, int Col) {
	int NewFace = Face;
	int NewRow = Row + 1;
	int NewCol = Col;
	if (NewRow == MazeTopHeight && NewFace % 3 == 0) {
		if (Face == 0) {
			NewFace = 4;
			NewRow = MazeSideHeight -1; /// Removing one from side height as we start counting from 0
			NewCol = MazeSideWidth - Col - 2; // Adding one here for our offset
			return GetCubeIndex(NewFace, NewRow, NewCol);
		}
		else {
			NewFace = 4;
			NewRow = 0; /// Removing one from side height as we start counting from 0
			NewCol = MazeSideWidth - Col - 2; // Adding one here for our offset
			return GetCubeIndex(NewFace, NewRow, NewCol);
		}
	}
	else if (NewRow == MazeSideHeight) {
		// If we are on either edge we are actually just going to end up going left/right
		if (NewCol == 0) {
			return GetLeftMazeCellIndex(NewFace, Row, NewCol);
		}
		else if (NewCol == MazeSideWidth - 1) {
			return GetRightMazeCellIndex(NewFace, Row, NewCol);
		}
		// Now we can worry about going onto Face 0 which will always be above the side faces
		NewFace = 0;
		// Now Set the Row/Col I dont like it this way but i think its best to work out per face
		if (Face == 1) {
			NewRow = 0;
			NewCol = Col;
		}
		else if (Face == 2) {
			NewRow = Col - 1;
			NewCol = 0;
		}
		else if (Face == 4) {
			NewRow = MazeTopHeight - 1;
			NewCol = Col - 1;
		}
		else {
			NewRow = Col;
			NewCol = MazeTopWidth - 1;
		}
	}
	return GetCubeIndex(NewFace, NewRow, NewCol);
}

int AMazePawn::GetRightMazeCellIndex(int Face, int Row, int Col) {
	int NewFace = Face;
	int NewRow = Row;
	int NewCol = Col + 1;
	
	if (NewCol == MazeTopWidth && NewFace % 3 == 0) {
		if (Face == 0) {
			NewFace = 5;
			NewRow = MazeSideHeight -1; /// Removing one from side height as we start counting from 0
			NewCol = MazeSideWidth - (MazeTopWidth - Row) - 1;		
		}
		else {
			NewFace = 5;
			NewRow = 0; /// Removing one from side height as we start counting from 0
			NewCol = MazeSideWidth - (MazeTopWidth - Row) - 1;
		}
	}
	else if (NewCol == MazeSideWidth) {
		if (Face % 3 == 2) {
			NewFace = Face - 1;
			NewRow = Row;
			NewCol = 0;
		}
		else {
			NewFace = Face - 2;
			NewRow = Row;
			NewCol = 0;
			if (NewFace < 0) {
				NewFace = 5;
			}
		}
	}

	return GetCubeIndex(NewFace, NewRow, NewCol);
}

int AMazePawn::GetDownMazeCellIndex(int Face, int Row, int Col) {
	int NewFace = Face;
	int NewRow = Row - 1;
	int NewCol = Col;

	if (NewRow < 0) { // This means we have switched face!
		if (Face % 3 == 0) {
			// Face 4/2 has an offset of 1 col
			if (Face == 0) {
				NewFace = 1;
				NewRow = MazeSideHeight - 1; /// Removing one from side height as we start counting from 0
				NewCol = Col;
				return GetCubeIndex(NewFace, NewRow, NewCol);
			}
			else {
				NewFace = 1;
				NewRow = 0; /// Removing one from side height as we start counting from 0
				NewCol = Col;
				return GetCubeIndex(NewFace, NewRow, NewCol);
			}
		}
		else {
			// If we are on either edge we are actually just going to end up going left/right
			if (NewCol == 0) {
				return GetLeftMazeCellIndex(NewFace, Row, NewCol);
			}
			else if (NewCol == MazeSideWidth - 1) {
				return GetRightMazeCellIndex(NewFace, Row, NewCol);
			}
			// Now we can worry about going onto Face 0 which will always be above the side faces
			NewFace = 3;
			// Now Set the Row/Col I dont like it this way but i think its best to work out per face
			if (Face == 1) {
				NewRow = 0;
				NewCol = Col;
			}
			else if (Face == 2) {
				NewRow = Col - 1;
				NewCol = 0;
			}
			else if (Face == 4) {
				NewRow = MazeTopHeight - 1;
				NewCol = Col - 1;
			}
			else {
				NewRow = Col;
				NewCol = MazeTopWidth - 1;
			}
		}
	}

	return GetCubeIndex(NewFace, NewRow, NewCol);
}