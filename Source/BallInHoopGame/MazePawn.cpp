// Fill out your copyright notice in the Description page of Project Settings.


#include "MazePawn.h"
#include "CoreFwd.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"



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
	GenerateMaze();
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
		// We need to do 2 slightly different loops depending if the face is a "top piece" or "side piece" :D
		// See GenerateMazeTop or GenerateMazeBottom for more info
		if (Face % 3 == 0) {
			GenerateMazeTop(Face);
		}
		else {
			GenerateMazeBottom(Face);
		}
	}
}

// GenerateMazeTop will loop 2 extra times on row and once extra on column to fill or the edges.
// This stops one corner piece being left over if all sides try to fill in 2 edges.
void AMazePawn::GenerateMazeTop(int Face) {
	UE_LOG(LogTemp, Warning, TEXT("AMazePawn::GenerateMazeTop() Called!"));
	for (int Row = 0; Row < 12; Row++) {
		for (int Col = 0; Col < 12; Col++) {
			AddStaticMeshToXFace(Row, Col, Face > 2);
		}
	}
}

void AMazePawn::GenerateMazeBottom(int Face) {
	UE_LOG(LogTemp, Warning, TEXT("AMazePawn::GenerateMazeTop() Called!"));
}

void AMazePawn::AddStaticMeshToXFace(int Row, int Col, bool bSecondPass) {
	UE_LOG(LogTemp, Warning, TEXT("AMazePawn::AddStaticMeshToXFace() Called!"));
	// Work out the relative position of the static mesh.
	int x, y, z;
	x = -550 + ((int)bSecondPass * 1100); // If it is the 2nd pass we want to move the position to the other side
	y = -550 + (Row * 100);
	z = -550 + (Col * 100);
	
	FVector RelativePos(x, y, z);

	FTransform Transform(RelativePos);
	
	//Create new static mesh object
	UStaticMeshComponent* CubeMeshComponent = NewObject<UStaticMeshComponent>(this);
	if (CubeMeshComponent)
	{
		if (CubeMeshAsset)
		{
			UE_LOG(LogTemp, Warning, TEXT("CubeMeshAsset Exists!"));
			CubeMeshComponent->RegisterComponent();
			// Set the static mesh of the cube component
			CubeMeshComponent->SetStaticMesh(CubeMeshAsset);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("CubeMeshAsset Is Missing!"));
		}

		// Attach the cube component to the root component or another desired parent component
		if (RootComponent) {
			UE_LOG(LogTemp, Warning, TEXT("RootComponent Exists!"));
			CubeMeshComponent->SetupAttachment(RootComponent);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("RootComponent is Missing!!"));
		}
		
		CubeMeshComponent->SetRelativeTransform(Transform);
		CubeMeshComponent->Mobility = EComponentMobility::Movable;
		CubeMeshComponent->CastShadow = false;
	}
}

void AMazePawn::AddStaticMeshToYFace(int Row, int Col, bool bSecondPass) {

}

void AMazePawn::AddStaticMeshToZFace(int Row, int Col, bool bSecondPass) {

}

/*
LoginId:81a3663d4136d1255a27a7a2d6063660
EpicAccountId : 0e6f49645a6c47ae9039300760d1c431

Fatal error : [File:D:\build\++UE5\Sync\Engine\Source\Runtime\CoreUObject\Private\UObject\Obj.cpp] [Line:121] No object initializer found during construction.

UnrealEditor_CoreUObject
UnrealEditor_BallInHoopGame_patch_1!AMazePawn::GenerateMaze()[C:\Users\charl\Documents\Unreal Projects\BallInHoopGame\Source\BallInHoopGame\MazePawn.cpp:45]
UnrealEditor_CoreUObject
UnrealEditor_CoreUObject
UnrealEditor_CoreUObject
UnrealEditor_CoreUObject
UnrealEditor_CoreUObject
UnrealEditor_CoreUObject
UnrealEditor_CoreUObject
UnrealEditor_CoreUObject
UnrealEditor_CoreUObject
UnrealEditor_Engine
UnrealEditor_Engine
UnrealEditor_Engine
UnrealEditor_Engine
UnrealEditor_Engine
UnrealEditor_Engine
UnrealEditor_Engine
UnrealEditor_Engine
UnrealEditor_UnrealEd
UnrealEditor_UnrealEd
UnrealEditor_UnrealEd
UnrealEditor_UnrealEd
UnrealEditor_UnrealEd
UnrealEditor_UnrealEd
UnrealEditor_UnrealEd
UnrealEditor_UnrealEd
UnrealEditor
UnrealEditor
UnrealEditor
UnrealEditor
UnrealEditor
UnrealEditor
kernel32
ntdll*/