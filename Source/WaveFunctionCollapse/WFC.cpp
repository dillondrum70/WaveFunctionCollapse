// Fill out your copyright notice in the Description page of Project Settings.


#include "WFC.h"

FAdjacencySides::FAdjacencySides()
{
	Sides.SetNum(6);	//Must have 6 entries, one for each side
}


// Sets default values
AWFC::AWFC()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*TileGridComponent = CreateDefaultSubobject<UGrid>(TEXT("Grid"));
	TileGridComponent->SetupAttachment(GetRootComponent());*/

}

// Called when the game starts or when spawned
void AWFC::BeginPlay()
{
	Super::BeginPlay();
	
	//Reserve memory now to avoid heavy allocations later
	GridCells.Reserve(GridDimensions.X * GridDimensions.Y * GridDimensions.Z);

	int x, y, z;

	//Iterate over all dimensions
	for (z = 0; z < GridDimensions.Z; z++)
	{
		for (y = 0; y < GridDimensions.Y; y++)
		{
			for (x = 0; x < GridDimensions.X; x++)
			{
				//Index the 1D array as a 3D array
				GridCell cell;
				
				//Calculate cell location
				FVector NewLocation = FVector(x * CellSize, y * CellSize, z * CellSize);

				//Spawn Cell
				AActor * CellModel = GetWorld()->SpawnActor(TileModels[TileType::FULL], &NewLocation);
				cell.Type = TileType::FULL;
				cell.Tile = StaticCast<ATile*>(CellModel);

				//GridCells[x + GridDimensions.X * (y + GridDimensions.Y * z)];
				GridCells.Add(cell);
			}
		}
	}
}

// Called every frame
void AWFC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// Get grid cell by reference
GridCell* AWFC::GetCell(FIntVector Index)
{
	int SingleIndex = Index.X + GridDimensions.X * (Index.Y + GridDimensions.Y * Index.Z);

	//Out of range
	if (SingleIndex > GridCells.Num()) { return nullptr; }

	//Index the 1D array using 3D coordinates
	return &GridCells[SingleIndex];
}

