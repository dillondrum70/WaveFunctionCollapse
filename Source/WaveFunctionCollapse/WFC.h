// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"
#include "Grid.h"


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WFC.generated.h"

UENUM(BlueprintType)
enum TileType {
	EMPTY = 0 UMETA(DisplayName = "EMPTY"),
	QUARTER = 1     UMETA(DisplayName = "QUARTER"),
	HALF = 2     UMETA(DisplayName = "HALF"),
	THREE_QUARTER = 3     UMETA(DisplayName = "THREE_QUARTER"),
	DIAGONAL = 4  UMETA(DisplayName = "DIAGONAL"),
	FULL = 5
};

struct GridCell
{
	TileType Type;
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	TMap<TEnumAsByte<TileType>, TSubclassOf<ATile>> TileModels;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UGrid* TileGridComponent;

	/*
	*
	* Grid
	* 
	*/

	UPROPERTY(EditDefaultsOnly)
	float CellSize = 100;

	UPROPERTY(EditDefaultsOnly)
	FIntVector GridDimensions = FIntVector(10, 10, 1);

	TArray<GridCell> GridCells;

	/// <summary>
	/// Get grid cell by reference
	/// </summary>
	/// <param name="Index">The 3D coordinates of the cell</param>
	/// <returns>Grid Cell Reference</returns>
	GridCell* GetCell(FIntVector Index);
};
