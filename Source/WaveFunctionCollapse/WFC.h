// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "Grid.h"

#include "Containers/Map.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WFC.generated.h"

//UENUM(BlueprintType)
//enum ETileType {
//	EMPTY = 0 UMETA(DisplayName = "EMPTY"),
//	QUARTER = 1     UMETA(DisplayName = "QUARTER"),
//	HALF = 2     UMETA(DisplayName = "HALF"),
//	THREE_QUARTER = 3     UMETA(DisplayName = "THREE_QUARTER"),
//	DIAGONAL = 4  UMETA(DisplayName = "DIAGONAL"),
//	FULL = 5
//};



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


/*
*
*	Reference to the a tile subclass along with its possible adjacent prototypes
*	This struct is used in the actual algorithm in lieu of the tiles themselves so that rotation can be accounted for
* 
*/
USTRUCT()
struct FPrototype
{
	GENERATED_BODY()

	FPrototype();

	/// <summary>
	/// Initialize prototype values
	/// </summary>
	/// <param name="TileType">Type of tile taken as the enum key from the map of TileModels</param>
	/// <param name="TileClass">TSubclassOf an ATile, allows us to spawn it later</param>
	/// <param name="Rot">The rotation of the tile</param>
	void Init(TSubclassOf<ATile> TileClass, ERotation Rot);

	FString Name;

	//Referenced tile
	TSubclassOf<ATile> Tile;

	// Used during generation to determine how the Tile's profiles should be offset to reflect this tile's rotation in WFC
	// Also used during tile placement, identifies which direction the tile should be rotated
	TEnumAsByte<ERotation> Rotation;

	//Adjacency list of possible tiles
	FAdjacencySides AdjacencyLists;
};


/*
*	Current State of a cell in the grid
*/
struct GridCell
{
	ATile* Tile;
};



UCLASS()
class WAVEFUNCTIONCOLLAPSE_API AWFC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWFC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GeneratePrototypes();

	void InitializePrototypes();

	void CreateAdjacencies();

	void PlacePrototype();

	int GetDirRotated(EDirection input, ERotation modifier);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//The models corresponding to each cell
	UPROPERTY(EditDefaultsOnly)
	TSet<TSubclassOf<ATile>> TileModels;

	//Stores the prototypes generated at runtime
	TArray<FPrototype> Prototypes;

	//The possibly adjacent tiles for each cell
	//Each is an array with 4 elements (one for each side) with an array of indices that correspond to the GeneratedPrototypes array
	//Key - Name of prototype
	//Value - An array of 4 arrays each containing an adjacency list
	//TMap<FString, FAdjacencySides> PrototypeAdjacency;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UGrid* TileGridComponent;*/

	/*
	*
	* Grid
	* 
	*/

	//Length of each side of a cell (cube)
	UPROPERTY(EditDefaultsOnly)
	float CellSize = 100;

	//How many cells lie in each direction
	UPROPERTY(EditDefaultsOnly)
	FIntVector GridDimensions = FIntVector(10, 10, 1);

	//Array storing current state of each cell
	TArray<GridCell> GridCells;

	/// <summary>
	/// Get grid cell by reference
	/// </summary>
	/// <param name="Index">The 3D coordinates of the cell</param>
	/// <returns>Grid Cell Reference</returns>
	GridCell* GetCell(FIntVector Index);
};
