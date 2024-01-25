// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM(BlueprintType)
enum ERotation {
	ZERO = 0 UMETA(DisplayName = "ZERO"),
	NINETY = 1     UMETA(DisplayName = "NINETY"),
	ONE_EIGHTY = 2     UMETA(DisplayName = "ONE_EIGHTY"),
	TWO_SEVENTY = 3     UMETA(DisplayName = "TWO_SEVENTY")
};

UENUM()
enum EDirection
{
	DIR_NORTH = 0,
	DIR_EAST = 1,
	DIR_SOUTH = 2,
	DIR_WEST = 3,
	DIR_UP = 4,
	DIR_DOWN = 5,
	DIR_MAX = 6
};

/*
*	List of integers that are indices in GeneratedPrototypes corresponding to possible adjacent prototypes
*/
USTRUCT()
struct FAdjacencySide
{
	GENERATED_BODY()

	FAdjacencySide();

	TArray<int> AdjacencyOptions;
};


/*
*	Set of 4 FAdjaceencySide structs
*/
USTRUCT()
struct FAdjacencySides
{
	GENERATED_BODY()

	//Initialize Sides with 4 elements
	FAdjacencySides();

	TArray<FAdjacencySide> Sides;
};

USTRUCT()
struct FTileProfile
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FString ProfileName;

public:
	bool operator==(const FTileProfile& rhs);
};

UCLASS(BlueprintType, Blueprintable)
class WAVEFUNCTIONCOLLAPSE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UPROPERTY(EditDefaultsOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly)
	TArray<FTileProfile> TileProfiles;

	//Points to the adjacency lists for each side at runtime after the tile is spawned
	//The adjacency lists exist in the AWFC::Prototypes array.  No reason to store duplicate copies of the same array if they exist elsewhere
	const FAdjacencySides* AdjacencyLists;
		 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
