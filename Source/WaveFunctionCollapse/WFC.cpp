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

void FPrototype::Init(TSubclassOf<ATile> TileClass, ERotation Rot)
{
	this->Tile = TileClass;
	this->Rotation = Rot;

	//Form name as "<Tile Type>_<Rotation>"
	this->Name = this->Tile.GetDefaultObject()->Name + "_";
	this->Name.AppendInt((int)this->Rotation);
}


GridCell::GridCell()
{
	Tile = nullptr;
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
	GridCells.Init(GridCell(), GridDimensions.X * GridDimensions.Y * GridDimensions.Z);

	//Preprocessing step, setting up for algorithm
	GeneratePrototypes();

	//Run the Wave Function Collapse algorithm and place tiles
	RunAlgorithm();


	////////// TEST CODE ////////////

	//int x, y, z;

	////Iterate over all dimensions
	//for (z = 0; z < GridDimensions.Z; z++)
	//{
	//	for (y = 0; y < GridDimensions.Y; y++)
	//	{
	//		for (x = 0; x < GridDimensions.X; x++)
	//		{
	//			PlacePrototype(Prototypes[0], FIntVector(x, y, z));
	//		}
	//	}
	//}
}


void AWFC::GeneratePrototypes()
{
	//Reserve the max amount of space needed, may not use it all
	Prototypes.Reserve(TileModels.Num() * 4);

	InitializePrototypes();

	//Release extra reserved memory
	Prototypes.Shrink();

	CreateAdjacencies();

	//////// TEST CODE /////////
	//Print adjacency lists
	/*for (FPrototype p : Prototypes)
	{
		UE_LOG(LogTemp, Display, TEXT("%s Adjacency Lists"), *p.Name);
		for (int i = 0; i < p.AdjacencyLists.Sides.Num(); i++)
		{
			UE_LOG(LogTemp, Display, TEXT("%s Side"), *UEnum::GetValueAsString((EDirection)i));
			for (int index : p.AdjacencyLists.Sides[i].AdjacencyOptions)
			{
				UE_LOG(LogTemp, Display, TEXT("%s"), *Prototypes[index].Name);
			}
		}
	}*/
}


void AWFC::InitializePrototypes()
{
	for (const TSubclassOf<ATile>& tile : TileModels)
	{
		ATile* DefaultTile = tile.GetDefaultObject();

		if (!DefaultTile) { UE_LOG(LogTemp, Error, TEXT("DefaultTile is Invalid.  WFC::InitializePrototypes()")); }

		bool opposite1Symmetric = (DefaultTile->TileProfiles[0].ProfileName == DefaultTile->TileProfiles[2].ProfileName);	// Check opposite profiles match
		bool opposite2Symmetric = (DefaultTile->TileProfiles[1].ProfileName == DefaultTile->TileProfiles[3].ProfileName); // Check other opposite profiles match

		bool adjacentSymmetric = (DefaultTile->TileProfiles[0].ProfileName == DefaultTile->TileProfiles[1].ProfileName); // Check if 

		//Fully symmetric, create 1 prototype
		if (opposite1Symmetric && opposite2Symmetric && adjacentSymmetric)
		{
			FPrototype p1;
			p1.Init(tile, ERotation::ZERO);

			Prototypes.Add(p1);
		}
		else if (opposite1Symmetric && opposite2Symmetric) //Half symmetric, create 2 prototypes
		{
			FPrototype p1;
			p1.Init(tile, ERotation::ZERO);

			Prototypes.Add(p1);

			FPrototype p2;
			p2.Init(tile, ERotation::NINETY);

			Prototypes.Add(p2);
		}
		else // No Symmetry, create 4 prototypes
		{
			FPrototype p1;
			p1.Init(tile, ERotation::ZERO);

			Prototypes.Add(p1);

			FPrototype p2;
			p2.Init(tile, ERotation::NINETY);

			Prototypes.Add(p2);

			FPrototype p3;
			p3.Init(tile, ERotation::ONE_EIGHTY);

			Prototypes.Add(p3);

			FPrototype p4;
			p4.Init(tile, ERotation::TWO_SEVENTY);

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

	UE_LOG(LogTemp, Display, TEXT("Start CreateAdjacencyList"));

	//Iterate over each prototype for each prototype, shrinking the used amount of TempPrototypes each time
	for (currentIndex = 0; currentIndex < Prototypes.Num(); currentIndex++)
	{
		ATile* ThisTile = Prototypes[currentIndex].Tile.GetDefaultObject();

		//Iterate over remaining prototypes after currentIndex
		for (testIndex = currentIndex + 1; testIndex < Prototypes.Num(); testIndex++)
		{
			//Tile of testing prototype
			ATile* OtherTile = Prototypes[testIndex].Tile.GetDefaultObject();

			//Rotation of the testing prototype
			ERotation rot = Prototypes[testIndex].Rotation;

			if (!ThisTile) { UE_LOG(LogTemp, Error, TEXT("ThisTile is Invalid")); return; }
			if (!OtherTile) { UE_LOG(LogTemp, Error, TEXT("OtherTiles is Invalid")); return; }
			
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

	UE_LOG(LogTemp, Warning, TEXT("CreateAdjacencyList Succeeded"));
}


void AWFC::RunAlgorithm()
{

}


void AWFC::PlacePrototype(const FPrototype& Prototype, FIntVector GridIndex)
{
	//Index the 1D array as a 3D array
	GridCell* cell = GetCell(GridIndex);

	//Calculate cell location
	FVector NewLocation = FVector(GridIndex.X * CellSize, GridIndex.Y * CellSize, GridIndex.Z * CellSize);

	//Spawn Cell
	AActor* CellModel = GetWorld()->SpawnActor(Prototype.Tile, &NewLocation);
	CellModel->SetActorRotation(GetRotation(Prototype.Rotation).Quaternion());	//Get FRotator from prototype rotation then rotate actor

	//Store spawned actor in grid
	cell->Tile = StaticCast<ATile*>(CellModel);
}


inline int AWFC::GetDirRotated(EDirection input, ERotation modifier)
{
	return ((input + EDirection::DIR_MAX) - modifier) % EDirection::DIR_MAX;
}


FRotator AWFC::GetRotation(ERotation Rotation)
{
	switch (Rotation)
	{
	case ERotation::ZERO:
		return FRotator(0, 0, 0);
		break;
	case ERotation::NINETY:
		return FRotator(0, 90, 0);
		break;
	case ERotation::ONE_EIGHTY:
		return FRotator(0, 180, 0);
		break;
	case ERotation::TWO_SEVENTY:
		return FRotator(0, 270, 0);
		break;
	default:
		return FRotator(0, 0, 0);
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

