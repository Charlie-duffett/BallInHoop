// Fill out your copyright notice in the Description page of Project Settings.


#include "MazePawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Misc/CString.h"
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

	for (int Face = 0; Face < 6; Face++) { // Loop over each face of the cube
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
}

void AMazePawn::AddMazeCubesTop(ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset, bool bSecondPass) {
	for (int Row = 0; Row < 12; Row++) {
		for (int Col = 0; Col < 12; Col++) {
			FVector RelativePos = GetMazeCubeZVector(bSecondPass, Row, Col);
			FTransform Transform(RelativePos);
			AddMazeCubeComponent(CubeMeshAsset, Transform);
		}
	}
}

void AMazePawn::AddMazeCubesSide(ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset, bool bSecondPass, int Face) {
	FVector RelativePos;
	for (int Row = 0; Row < 10; Row++) {
		for (int Col = 0; Col < 11; Col++) {
			if (Face % 3 == 1) {
				RelativePos = GetMazeCubeXVector(bSecondPass, Row, Col);
			}
			else {
				RelativePos = GetMazeCubeYVector(bSecondPass, Row, Col);
				UE_LOG(LogTemp, Log, TEXT("Value of bSecondPass %d"), (int)bSecondPass);
			}
			
			FTransform Transform(RelativePos);
			AddMazeCubeComponent(CubeMeshAsset, Transform);
		}
	}
}

void AMazePawn::AddMazeCubeComponent(ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset, FTransform Transform) {
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
	}
}

FVector AMazePawn::GetMazeCubeXVector(bool bSecondPass, int Row, int Col) {
	// Work out the relative position of the static mesh.
	int x, y, z;
	x = -550 + ((int)bSecondPass * 1100); 
	y = -450 + (Col * 100) + (-100 * (int)bSecondPass);// We want alternate which side has the extra col
	z = -450 + (Row * 100);

	return FVector(x, y, z);
}

FVector AMazePawn::GetMazeCubeYVector(bool bSecondPass, int Row, int Col) {
	// Work out the relative position of the static mesh.
	int x, y, z;
	x = -450 + (Col * 100 + (-100 * (int)!bSecondPass)); // We do the opposite here as we dont want to add the extra col on the same space
	y = -550 + ((int)bSecondPass * 1100);// If it is the 2nd pass we want to move the position to the other side
	z = -450 + (Row * 100); 

	return FVector(x, y, z);
}

FVector AMazePawn::GetMazeCubeZVector(bool bSecondPass, int Row, int Col) {
	// Work out the relative position of the static mesh.
	int x, y, z;
	x = -550 + (Row * 100);
	y = -550 + (Col * 100); // Here we will always have an extra col so we dont need any logic
	z = -550 + ((int)bSecondPass * 1100);// If it is the 2nd pass we want to move the position to the other side

	return FVector(x, y, z);
}