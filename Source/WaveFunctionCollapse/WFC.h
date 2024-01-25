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
	GridCell();

	ATile* Tile;

	TArray<int> Possibilities;	//Each index is the index possible neighbor in WFC::Prototypes 
};



UCLASS()
class WAVEFUNCTIONCOLLAPSE_API AWFC : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWFC();

protected:

	//How many tiles have been collapsed by WFC
	int CollapsedTiles;

	TArray<int> PropogationIndices;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/*
	*	Preprocessing Functions
	*/

	/// <summary>
	/// Create prototypes for all possible rotations of each tile
	/// </summary>
	void GeneratePrototypes();

	/// <summary>
	/// Create the adjacency list for each prototype
	/// </summary>
	void CreateAdjacencies();

	/*
	*	Wave Function Collapse Algorithm Functions
	*/

	/// <summary>
	/// Run the WFC algorithm and place tiles
	/// </summary>
	void RunAlgorithm();

	/// <summary>
	/// Run at each iteration of the WFC algorithm
	/// </summary>
	void IterateAlgorithm();

	/// <summary>
	/// Find the lowest entropy cells
	/// </summary>
	/// <param name="Index">Out parameter of the integer format of an index in grid that have lowest entropy</param>
	int FindLowestEntropy();

	/// <summary>
	/// Collapse the cell in GridCells at the given index.
	/// This chooses a tile from its possibilities and spawns that tile.
	/// </summary>
	/// <param name="index">Index to collapse at</param>
	void CollapseCell(int Index);

	/// <summary>
	/// Updates other cells to reflect the changes of the current cell
	/// </summary>
	/// <param name="Index">Current cell index</param>
	void PropogateCellChanges(int Index);

	/*
	*	Helper Functions
	*/

	/// <summary>
	/// Create the initial prototypes for each rotation
	/// </summary>
	void InitializePrototypes();

	/// <summary>
	/// Get the rotated direction based on some ERotation
	/// </summary>
	/// <param name="input">The direction to rotate</param>
	/// <param name="modifier">How much to rotate by</param>
	/// <returns>The new rotated direction expressed as an integer</returns>
	int GetDirRotated(EDirection input, int modifier);

	/// <summary>
	/// Similar to GetDirRotated but specifically for rotations instead
	/// </summary>
	/// <param name="input"></param>
	/// <param name="modifier"></param>
	/// <param name="RotMax">Max number of rotations, typically 0, 90, 180, or 270 but can be 0 and 90 if diagonally symmetric or just 0 if fully symmetric</param>
	/// <returns></returns>
	int GetRotatedRotation(ERotation input, ERotation modifier, int RotMax = 4);

	/// <summary>
	/// Convert an ERotation to an FRotator
	/// </summary>
	/// <param name="Rotation">ERotation to convert</param>
	/// <returns>FRotator conversion</returns>
	FRotator GetRotation(ERotation Rotation);

	/// <summary>
	/// Place a tile at a grid index and rotate it
	/// </summary>
	/// <param name="Prototype">Prototype with rotation and tile info</param>
	/// <param name="GridIndex">Index to place tile at</param>
	void PlacePrototype(int PrototypeIndex, FIntVector GridIndex);

	/// <summary>
	/// Check if the wave function collapse has completed
	/// </summary>
	/// <returns>Whether or not WFC is done</returns>
	inline bool IsCollapsed() { return CollapsedTiles >= GridCells.Num(); }

	/// <summary>
	/// Returns possible neighboring tiles based on the profiles of the indexed tile's Possibilities in that direction
	/// </summary>
	/// <param name="Index">Index of the tile to check</param>
	/// <param name="Direction">Which side of tile to pull profiles from</param>
	/// <returns>Array if indices in Prototypes that represents all possible neighbors</returns>
	TArray<int> GetPossibleNeighbors(int Index, EDirection Direction);

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

	/// <summary>
	/// Get the cell in the given direction from the given index
	/// </summary>
	/// <param name="Index">Current cell</param>
	/// <param name="Direction">Direction of the adjacent cell</param>
	/// <returns>Adjacent cell, returns nullptr if on the edge of the grid</returns>
	GridCell* GetAdjacentCell(int Index, EDirection Direction, int& OutAdjacentIndex);

	/// <summary>
	/// Convert a 3D index in the grid into a flattened integer for a 1D array representing three dimensions
	/// </summary>
	/// <param name="GridIndex">Index to convert</param>
	/// <returns>Integer conversion</returns>
	inline int GridIndexToInt(FIntVector GridIndex);

	/// <summary>
	/// Convert an integer index into a 3D index in the grid, the opposite of GridIndexToInt
	/// </summary>
	/// <param name="Index">Index to convert</param>
	/// <returns>FIntVector conversion</returns>
	FIntVector IntToGridIndex(int Index);

	/// <summary>
	/// Checks if profiles can be adjacent, uses specific rules such as the fact that certain tiles must be mirrored profiles of each other to be adjacent.
	/// </summary>
	/// <param name="lhs"></param>
	/// <param name="rhs"></param>
	/// <returns>Whether or not the profiles can be adjacent</returns>
	bool CompareProfiles(const FString& lhs, const FString& rhs, bool Vertical) const;

	/// <summary>
	/// Return the profile name that corresponds to the passed profile rotated by Rotation degrees
	/// </summary>
	/// <param name="Profile"></param>
	/// <param name="Rotation"></param>
	/// <returns></returns>
	FString GetRotatedVerticalProfile(const FTileProfile& Profile, ERotation Rotation);
};
