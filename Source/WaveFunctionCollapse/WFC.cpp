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

void FPrototype::Init(TEnumAsByte<ETileType> TileType, TSubclassOf<ATile> TileClass, ERotation Rot)
{
	this->Tile = TileClass;
	this->Rotation = Rot;

	//Form name as "<Tile Type>_<Rotation>"
	this->Name = UEnum::GetValueAsString(TileType) + "_";
	this->Name.AppendInt((int)this->Rotation);
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

	//int x, y, z;

	////Iterate over all dimensions
	//for (z = 0; z < GridDimensions.Z; z++)
	//{
	//	for (y = 0; y < GridDimensions.Y; y++)
	//	{
	//		for (x = 0; x < GridDimensions.X; x++)
	//		{
	//			//Index the 1D array as a 3D array
	//			GridCell cell;
	//			
	//			//Calculate cell location
	//			FVector NewLocation = FVector(x * CellSize, y * CellSize, z * CellSize);

	//			//Spawn Cell
	//			AActor * CellModel = GetWorld()->SpawnActor(TileModels[ETileType::FULL], &NewLocation);
	//			cell.Type = ETileType::FULL;
	//			cell.Tile = StaticCast<ATile*>(CellModel);

	//			//GridCells[x + GridDimensions.X * (y + GridDimensions.Y * z)];
	//			GridCells.Add(cell);
	//		}
	//	}
	//}
}


void AWFC::GeneratePrototypes()
{
	//Reserve and initialize the max amount of space needed, may not use it all
	Prototypes.Init(FPrototype(), TileModels.Num() * 4);

	InitializePrototypes();

	CreateAdjacencies();

	//Release extra reserved memory
	Prototypes.Shrink();


	//////// TEST CODE /////////
	/*for (FPrototype p : Prototypes)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), p.
		UE_LOG(LogTemp, Display, p.AdjacencyLists.Sides)
	}*/
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
			FPrototype p1;
			p1.Init(tile.Key, tile.Value, ERotation::ZERO);

			Prototypes.Add(p1);
		}
		else if (opposite1Symmetric && opposite2Symmetric) //Half symmetric, create 2 prototypes
		{
			FPrototype p1;
			p1.Init(tile.Key, tile.Value, ERotation::ZERO);

			Prototypes.Add(p1);

			FPrototype p2;
			p2.Init(tile.Key, tile.Value, ERotation::NINETY);

			Prototypes.Add(p2);
		}
		else // No Symmetry, create 4 prototypes
		{
			FPrototype p1;
			p1.Init(tile.Key, tile.Value, ERotation::ZERO);

			Prototypes.Add(p1);

			FPrototype p2;
			p2.Init(tile.Key, tile.Value, ERotation::NINETY);

			Prototypes.Add(p2);

			FPrototype p3;
			p3.Init(tile.Key, tile.Value, ERotation::ONE_EIGHTY);

			Prototypes.Add(p3);

			FPrototype p4;
			p4.Init(tile.Key, tile.Value, ERotation::TWO_SEVENTY);

			Prototypes.Add(p4);
		}
	}
}


void AWFC::CreateAdjacencies()
{
	/*TArray<FPrototype> TempPrototypes;
	TempPrototypes.SetNum(Prototypes.Num());*/

	int currentIndex = 0, testIndex = 0;
	
	////Copy Prototypes
	//for (currentIndex = 0; currentIndex < Prototypes.Num(); currentIndex++)
	//{
	//	TempPrototypes[currentIndex] = Prototypes[currentIndex];
	//}

	/*UE_LOG(LogTemp, Display, TEXT("Start %i  Modifier %i  End %i"),
		EDirection::DIR_NORTH, ERotation::ONE_EIGHTY, GetDirRotated(EDirection::DIR_NORTH, ERotation::ONE_EIGHTY));
	UE_LOG(LogTemp, Display, TEXT("Start %i  Modifier %i  End %i"), 
		EDirection::DIR_SOUTH, ERotation::ONE_EIGHTY, GetDirRotated(EDirection::DIR_SOUTH, ERotation::ONE_EIGHTY));*/

	UE_LOG(LogTemp, Display, TEXT("%i"), Prototypes.Num());

	//Iterate over each prototype for each prototype, shrinking the used amount of TempPrototypes each time
	for (currentIndex = 0; currentIndex < Prototypes.Num(); currentIndex++)
	{
		UE_LOG(LogTemp, Display, TEXT("CurrentIndex %i"), currentIndex);

		ATile* ThisTile = Prototypes[currentIndex].Tile.GetDefaultObject();

		//Iterate over remaining prototypes after currentIndex
		for (testIndex = currentIndex + 1; testIndex < Prototypes.Num(); testIndex++)
		{

			UE_LOG(LogTemp, Display, TEXT("Test Index %i"), testIndex);

			//Tile of testing prototype
			ATile* OtherTile = Prototypes[testIndex].Tile.GetDefaultObject();

			//Rotation of the testing prototype
			ERotation rot = Prototypes[testIndex].Rotation;

			if(!ThisTile) { UE_LOG(LogTemp, Error, TEXT("ThisTile is Invalid")); }
			if(!OtherTile) { UE_LOG(LogTemp, Error, TEXT("OtherTiles is Invalid")); }

			UE_LOG(LogTemp, Display, TEXT("TileProfiles %i"), ThisTile->TileProfiles.Num());
			

			//Test adjacent tiles
			if (ThisTile->TileProfiles[DIR_NORTH] == OtherTile->TileProfiles[GetDirRotated(EDirection::DIR_SOUTH, rot)])
			{
				Prototypes[currentIndex].AdjacencyLists.Sides[DIR_NORTH].AdjacencyOptions.Add(testIndex);
				Prototypes[testIndex].AdjacencyLists.Sides[DIR_SOUTH].AdjacencyOptions.Add(currentIndex);
			}

			if (ThisTile->TileProfiles[DIR_EAST] == OtherTile->TileProfiles[GetDirRotated(EDirection::DIR_WEST, rot)])
			{
				Prototypes[currentIndex].AdjacencyLists.Sides[DIR_EAST].AdjacencyOptions.Add(testIndex);
				Prototypes[testIndex].AdjacencyLists.Sides[DIR_WEST].AdjacencyOptions.Add(currentIndex);
			}

			if (ThisTile->TileProfiles[DIR_SOUTH] == OtherTile->TileProfiles[GetDirRotated(EDirection::DIR_NORTH, rot)])
			{
				Prototypes[currentIndex].AdjacencyLists.Sides[DIR_SOUTH].AdjacencyOptions.Add(testIndex);
				Prototypes[testIndex].AdjacencyLists.Sides[DIR_NORTH].AdjacencyOptions.Add(currentIndex);
			}

			if (ThisTile->TileProfiles[DIR_WEST] == OtherTile->TileProfiles[GetDirRotated(EDirection::DIR_EAST, rot)])
			{
				Prototypes[currentIndex].AdjacencyLists.Sides[DIR_WEST].AdjacencyOptions.Add(testIndex);
				Prototypes[testIndex].AdjacencyLists.Sides[DIR_EAST].AdjacencyOptions.Add(currentIndex);
			}
		}
	}
}


void AWFC::PlacePrototype()
{
	UE_LOG(LogTemp, Error, TEXT("Function Not Implemented: PlacePrototype()"));
}


int AWFC::GetDirRotated(EDirection input, ERotation modifier)
{
	return ((input + EDirection::DIR_MAX) - modifier) % EDirection::DIR_MAX;
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

