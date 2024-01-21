// Fill out your copyright notice in the Description page of Project Settings.


#include "WFC.h"

FAdjacencySide::FAdjacencySide()
{
	
}

FAdjacencySides::FAdjacencySides()
{
	Sides.SetNum(4);	//Must have 6 entries, one for each side
}


FPrototype::FPrototype()
{

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

	GeneratePrototypes();


	////////// TEST CODE ////////////

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
				AActor * CellModel = GetWorld()->SpawnActor(TileModels[ETileType::FULL], &NewLocation);
				cell.Type = ETileType::FULL;
				cell.Tile = StaticCast<ATile*>(CellModel);

				//GridCells[x + GridDimensions.X * (y + GridDimensions.Y * z)];
				GridCells.Add(cell);
			}
		}
	}
}


void AWFC::GeneratePrototypes()
{
	//Reserve and initialize the max amount of space needed, may not use it all
	Prototypes.Init(FPrototype(), TileModels.Num() * 4);

	InitializePrototypes();

	CreateAdjacencies();

	//Release extra reserved memory
	Prototypes.Shrink();
}


void AWFC::InitializePrototypes()
{
	for (TPair<TEnumAsByte<ETileType>, TSubclassOf<ATile>> tile : TileModels)
	{
		ATile* DefaultTile = tile.Value.GetDefaultObject();

		bool opposite1Symmetric = (DefaultTile->TileProfiles[0].Name == DefaultTile->TileProfiles[2].Name);	// Check opposite profiles match
		bool opposite2Symmetric = (DefaultTile->TileProfiles[1].Name == DefaultTile->TileProfiles[3].Name); // Check other opposite profiles match

		bool adjacentSymmetric = (DefaultTile->TileProfiles[0].Name == DefaultTile->TileProfiles[1].Name); // Check if 

		//Fully symmetric, create 1 prototype
		if (opposite1Symmetric && opposite2Symmetric && adjacentSymmetric)
		{
			UE_LOG(LogTemp, Display, TEXT("Symmetric"));
			FPrototype p1;
			p1.Tile = tile.Value;
			p1.Rotation = ERotation::ZERO;

			Prototypes.Add(p1);
		}
		else if (opposite1Symmetric && opposite2Symmetric) //Half symmetric, create 2 prototypes
		{
			UE_LOG(LogTemp, Display, TEXT("Half Symmetric"));
			FPrototype p1;
			p1.Tile = tile.Value;
			p1.Rotation = ERotation::ZERO;

			Prototypes.Add(p1);

			FPrototype p2;
			p1.Tile = tile.Value;
			p1.Rotation = ERotation::NINETY;

			Prototypes.Add(p1);
		}
		else // No Symmetry, create 4 prototypes
		{
			UE_LOG(LogTemp, Display, TEXT("No Symmetry"));
			FPrototype p1;
			p1.Tile = tile.Value;
			p1.Rotation = ERotation::ZERO;

			Prototypes.Add(p1);

			FPrototype p2;
			p1.Tile = tile.Value;
			p1.Rotation = ERotation::NINETY;

			Prototypes.Add(p1);

			FPrototype p3;
			p3.Tile = tile.Value;
			p3.Rotation = ERotation::ONE_EIGHTY;

			Prototypes.Add(p3);

			FPrototype p4;
			p4.Tile = tile.Value;
			p4.Rotation = ERotation::TWO_SEVENTY;

			Prototypes.Add(p4);
		}
	}
}


void AWFC::CreateAdjacencies()
{

}


void AWFC::PlacePrototype()
{
	UE_LOG(LogTemp, Error, TEXT("Function Not Implemented: PlacePrototype()"));
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

