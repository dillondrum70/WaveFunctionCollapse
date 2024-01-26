// Fill out your copyright notice in the Description page of Project Settings.


#include "WFC.h"


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

	/*FString text = "";

	FTileProfile profile;
	profile.ProfileName = "v0s";

	text += profile.ProfileName + " Rotated 0: " + GetRotatedVerticalProfile(profile, ERotation::ZERO) + "\n";
	profile.ProfileName = "v0s";
	text += profile.ProfileName + " Rotated 90: " + GetRotatedVerticalProfile(profile, ERotation::NINETY) + "\n";
	profile.ProfileName = "v0s";
	text += profile.ProfileName + " Rotated 180: " + GetRotatedVerticalProfile(profile, ERotation::ONE_EIGHTY) + "\n";
	profile.ProfileName = "v0s";
	text += profile.ProfileName + " Rotated 270: " + GetRotatedVerticalProfile(profile, ERotation::TWO_SEVENTY) + "\n\n";

	profile.ProfileName = "v1_0";
	text += profile.ProfileName + " Rotated 0: " + GetRotatedVerticalProfile(profile, ERotation::ZERO) + "\n";
	profile.ProfileName = "v1_0";
	text += profile.ProfileName + " Rotated 90: " + GetRotatedVerticalProfile(profile, ERotation::NINETY) + "\n";
	profile.ProfileName = "v1_0";
	text += profile.ProfileName + " Rotated 180: " + GetRotatedVerticalProfile(profile, ERotation::ONE_EIGHTY) + "\n";
	profile.ProfileName = "v1_0";
	text += profile.ProfileName + " Rotated 270: " + GetRotatedVerticalProfile(profile, ERotation::TWO_SEVENTY) + "\n\n";

	profile.ProfileName = "v1_1";
	text += profile.ProfileName + " Rotated 0: " + GetRotatedVerticalProfile(profile, ERotation::ZERO) + "\n";
	profile.ProfileName = "v1_1";
	text += profile.ProfileName + " Rotated 90: " + GetRotatedVerticalProfile(profile, ERotation::NINETY) + "\n";
	profile.ProfileName = "v1_1";
	text += profile.ProfileName + " Rotated 180: " + GetRotatedVerticalProfile(profile, ERotation::ONE_EIGHTY) + "\n";
	profile.ProfileName = "v1_1";
	text += profile.ProfileName + " Rotated 270: " + GetRotatedVerticalProfile(profile, ERotation::TWO_SEVENTY) + "\n\n";

	profile.ProfileName = "v1_2";
	text += profile.ProfileName + " Rotated 0: " + GetRotatedVerticalProfile(profile, ERotation::ZERO) + "\n";
	profile.ProfileName = "v1_2";
	text += profile.ProfileName + " Rotated 90: " + GetRotatedVerticalProfile(profile, ERotation::NINETY) + "\n";
	profile.ProfileName = "v1_2";
	text += profile.ProfileName + " Rotated 180: " + GetRotatedVerticalProfile(profile, ERotation::ONE_EIGHTY) + "\n";
	profile.ProfileName = "v1_2";
	text += profile.ProfileName + " Rotated 270: " + GetRotatedVerticalProfile(profile, ERotation::TWO_SEVENTY) + "\n\n";

	profile.ProfileName = "v1_3";
	text += profile.ProfileName + " Rotated 0: " + GetRotatedVerticalProfile(profile, ERotation::ZERO) + "\n";
	profile.ProfileName = "v1_3";
	text += profile.ProfileName + " Rotated 90: " + GetRotatedVerticalProfile(profile, ERotation::NINETY) + "\n";
	profile.ProfileName = "v1_3";
	text += profile.ProfileName + " Rotated 180: " + GetRotatedVerticalProfile(profile, ERotation::ONE_EIGHTY) + "\n";
	profile.ProfileName = "v1_3";
	text += profile.ProfileName + " Rotated 270: " + GetRotatedVerticalProfile(profile, ERotation::TWO_SEVENTY) + "\n\n";

	profile.ProfileName = "v1_0p";
	text += profile.ProfileName + " Rotated 0: " + GetRotatedVerticalProfile(profile, ERotation::ZERO) + "\n";
	profile.ProfileName = "v1_0p";
	text += profile.ProfileName + " Rotated 90: " + GetRotatedVerticalProfile(profile, ERotation::NINETY) + "\n";
	profile.ProfileName = "v1_0p";
	text += profile.ProfileName + " Rotated 180: " + GetRotatedVerticalProfile(profile, ERotation::ONE_EIGHTY) + "\n";
	profile.ProfileName = "v1_0p";
	text += profile.ProfileName + " Rotated 270: " + GetRotatedVerticalProfile(profile, ERotation::TWO_SEVENTY) + "\n\n";

	profile.ProfileName = "v1_1p";
	text += profile.ProfileName + " Rotated 0: " + GetRotatedVerticalProfile(profile, ERotation::ZERO) + "\n";
	profile.ProfileName = "v1_1p";
	text += profile.ProfileName + " Rotated 90: " + GetRotatedVerticalProfile(profile, ERotation::NINETY) + "\n";
	profile.ProfileName = "v1_1p";
	text += profile.ProfileName + " Rotated 180: " + GetRotatedVerticalProfile(profile, ERotation::ONE_EIGHTY) + "\n";
	profile.ProfileName = "v1_1p";
	text += profile.ProfileName + " Rotated 270: " + GetRotatedVerticalProfile(profile, ERotation::TWO_SEVENTY) + "\n\n";

	UE_LOG(LogTemp, Display, TEXT("%s"), *text);*/

	
	//Preprocessing step, setting up for algorithm
	GeneratePrototypes();

	GridCell DefaultCell;	//Default state of a cell before WFC
	DefaultCell.Tile = nullptr;
	DefaultCell.Possibilities.SetNum(Prototypes.Num());

	//Fill Possibilities with all possible indices in Prototypes
	for(int i = 0; i < Prototypes.Num(); i++)
	{
		DefaultCell.Possibilities[i] = i;
	}

	//Initialize grid with default values, all cells can be any prototype
	GridCells.Init(DefaultCell, GridDimensions.X * GridDimensions.Y * GridDimensions.Z);
	
	//Run the Wave Function Collapse algorithm and place tiles
	RunAlgorithm();
	

	////////// TEST CODE ////////////

	/*UE_LOG(LogTemp, Display, TEXT("Possibilities - %i"), Prototypes.Num());
	TArray<int> n = GetPossibleNeighbors(15, EDirection::DIR_NORTH);
	for (int t : n)
	{
		UE_LOG(LogTemp, Display, TEXT("%i"), t);
	}*/

	UE_LOG(LogTemp, Display, TEXT("Collapsed: %i   Total: %i"), CollapsedTiles, GridCells.Num());

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
	FString text;
	for (FPrototype p : Prototypes)
	{
		text = "\nPrototype Name: " + p.Name + "\n";

		//UE_LOG(LogTemp, Display, TEXT("%s Adjacency Lists"), *p.Name);
		for (int i = 0; i < p.AdjacencyLists.Sides.Num(); i++)
		{
			//UE_LOG(LogTemp, Display, TEXT("%s Side"), *UEnum::GetValueAsString((EDirection)i));
			text += "\n" + UEnum::GetValueAsString((EDirection)i) + " -\n";
			for (int index : p.AdjacencyLists.Sides[i].AdjacencyOptions)
			{
				//UE_LOG(LogTemp, Display, TEXT("%s"), *Prototypes[index].Name);
				text += "\t" + Prototypes[index].Name + "\n";
			}
			text += "\n";
		}
		text += "\n\n";
		UE_LOG(LogTemp, Display, TEXT("%s"), *text);
	}
	
}


void AWFC::InitializePrototypes()
{
	for (const TSubclassOf<ATile>& tile : TileModels)
	{
		ATile* DefaultTile = tile.GetDefaultObject();

		if (!DefaultTile) { UE_LOG(LogTemp, Error, TEXT("DefaultTile is Invalid.  WFC::InitializePrototypes()")); }

		bool opposite1Match = (DefaultTile->TileProfiles[0].ProfileName == DefaultTile->TileProfiles[2].ProfileName);	// Check opposite profiles match
		bool opposite2Match = (DefaultTile->TileProfiles[1].ProfileName == DefaultTile->TileProfiles[3].ProfileName); // Check other opposite profiles match

		bool adjacentMatch = (DefaultTile->TileProfiles[0].ProfileName == DefaultTile->TileProfiles[1].ProfileName); // Check if 

		//Fully symmetric, create 1 prototype
		if (opposite1Match && opposite2Match && adjacentMatch)
		{
			FPrototype p1;
			p1.Init(tile, ERotation::ZERO);

			Prototypes.Add(p1);
		}
		else if (opposite1Match && opposite2Match) //Half symmetric, create 2 prototypes
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
		ERotation ThisRot = Prototypes[currentIndex].Rotation;

		//Iterate over remaining prototypes after currentIndex
		for (testIndex = currentIndex; testIndex < Prototypes.Num(); testIndex++)
		{
			//Tile of testing prototype
			ATile* OtherTile = Prototypes[testIndex].Tile.GetDefaultObject();

			//Rotation of the testing prototype
			ERotation TestRot = Prototypes[testIndex].Rotation;

			if (!ThisTile) { UE_LOG(LogTemp, Error, TEXT("ThisTile is Invalid")); return; }
			if (!OtherTile) { UE_LOG(LogTemp, Error, TEXT("OtherTiles is Invalid")); return; }
			
			//Compare adjacent profiles and add them to each others adjacency list
			int ThisDir = GetDirRotated(EDirection::DIR_NORTH, ThisRot);	// Side of current prototype
			int TestDir = GetDirRotated(EDirection::DIR_SOUTH, TestRot);	// Adjacent side of other prototype on the previous side (i.e. the north side of this prototype is adjacent to the south side of the other)
			if (CompareProfiles(ThisTile->TileProfiles[ThisDir].ProfileName, OtherTile->TileProfiles[TestDir].ProfileName, false) &&
				!InvalidEmptyProfile(currentIndex, OtherTile->TileProfiles[ThisDir].ProfileName))
			{
				Prototypes[currentIndex].AdjacencyLists.Sides[DIR_NORTH].AdjacencyOptions.Add(testIndex);

				//Do not add twice if index is the same, don't want to have same entry twice in one prototype
				if (currentIndex != testIndex)
				{
					Prototypes[testIndex].AdjacencyLists.Sides[DIR_SOUTH].AdjacencyOptions.Add(currentIndex);
				}
			}

			ThisDir = GetDirRotated(EDirection::DIR_EAST, ThisRot);
			TestDir = GetDirRotated(EDirection::DIR_WEST, TestRot);
			if (CompareProfiles(ThisTile->TileProfiles[ThisDir].ProfileName, OtherTile->TileProfiles[TestDir].ProfileName, false) &&
				!InvalidEmptyProfile(testIndex, OtherTile->TileProfiles[ThisDir].ProfileName))
			{
				Prototypes[currentIndex].AdjacencyLists.Sides[DIR_EAST].AdjacencyOptions.Add(testIndex);

				if (currentIndex != testIndex)
				{
					Prototypes[testIndex].AdjacencyLists.Sides[DIR_WEST].AdjacencyOptions.Add(currentIndex);
				}
			}

			ThisDir = GetDirRotated(EDirection::DIR_SOUTH, ThisRot);
			TestDir = GetDirRotated(EDirection::DIR_NORTH, TestRot);
			if (CompareProfiles(ThisTile->TileProfiles[ThisDir].ProfileName, OtherTile->TileProfiles[TestDir].ProfileName, false) &&
				!InvalidEmptyProfile(testIndex, OtherTile->TileProfiles[ThisDir].ProfileName))
			{
				Prototypes[currentIndex].AdjacencyLists.Sides[DIR_SOUTH].AdjacencyOptions.Add(testIndex);

				if (currentIndex != testIndex)
				{
					Prototypes[testIndex].AdjacencyLists.Sides[DIR_NORTH].AdjacencyOptions.Add(currentIndex);
				}
			}

			ThisDir = GetDirRotated(EDirection::DIR_WEST, ThisRot);
			TestDir = GetDirRotated(EDirection::DIR_EAST, TestRot);
			if (CompareProfiles(ThisTile->TileProfiles[ThisDir].ProfileName, OtherTile->TileProfiles[TestDir].ProfileName, false) &&
				!InvalidEmptyProfile(testIndex, OtherTile->TileProfiles[ThisDir].ProfileName))
			{
				Prototypes[currentIndex].AdjacencyLists.Sides[DIR_WEST].AdjacencyOptions.Add(testIndex);

				if (currentIndex != testIndex)
				{
					Prototypes[testIndex].AdjacencyLists.Sides[DIR_EAST].AdjacencyOptions.Add(currentIndex);
				}
			}

			FString ThisProfile = GetRotatedVerticalProfile(ThisTile->TileProfiles[DIR_UP], ThisRot);
			FString TestProfile = GetRotatedVerticalProfile(OtherTile->TileProfiles[DIR_DOWN], TestRot);
			if (CompareProfiles(ThisProfile, TestProfile, true))
			{
				Prototypes[currentIndex].AdjacencyLists.Sides[DIR_UP].AdjacencyOptions.Add(testIndex);

				if (currentIndex != testIndex)
				{
					Prototypes[testIndex].AdjacencyLists.Sides[DIR_DOWN].AdjacencyOptions.Add(currentIndex);
				}
			}

			ThisProfile = GetRotatedVerticalProfile(ThisTile->TileProfiles[DIR_DOWN], ThisRot);
			TestProfile = GetRotatedVerticalProfile(OtherTile->TileProfiles[DIR_UP], TestRot);
			if (CompareProfiles(ThisProfile, TestProfile, true))
			{
				Prototypes[currentIndex].AdjacencyLists.Sides[DIR_DOWN].AdjacencyOptions.Add(testIndex);

				if (currentIndex != testIndex)
				{
					Prototypes[testIndex].AdjacencyLists.Sides[DIR_UP].AdjacencyOptions.Add(currentIndex);
				}
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("CreateAdjacencyList Succeeded"));
}


void AWFC::RunAlgorithm()
{
	//Reset collapsed tiles to 0
	CollapsedTiles = 0;

	//Clear list
	PropogationIndices = TArray<int>();

	//While not collapsed
	while (!IsCollapsed())
	{
		//Continue collapsing tiles
		IterateAlgorithm();
	}
}


void AWFC::IterateAlgorithm()
{
	/*
	*	1. Find min entropy cell, consider priority queue
	*	2. Collapse that cell by choosing and placing random tile (or the last remaining tile/ error if no remaining possibilities)
	*	3. Propogate those changes to other cells
	*/

	//Index in GridCells of chosen lowest entropy cell
	int index = FindLowestEntropy();

	//Failed to find lowest entropy, exit early
	if (index < 0 || index >= GridCells.Num()) { 
		UE_LOG(LogTemp, Error, TEXT("Failed to find lowest entropy cell"));
		return; 
	}

	//Collapse the grid cell at the lowest entropy index
	CollapseCell(index);
	
	//Propogate changes from collapsed cell
	PropogateCellChanges(index);
}


int AWFC::FindLowestEntropy()
{
	TArray<int> Indices;
	int LowestPossible = Prototypes.Num();

	for (int i = 0; i < GridCells.Num(); i++)
	{
		//If there is already something in this tile, continue
		if (GridCells[i].Tile) { continue; }

		//Continue if higher entropy
		if (GridCells[i].Possibilities.Num() > LowestPossible) { continue; }

		//Clear array and update new lowest entropy if lower possibilities
		if (GridCells[i].Possibilities.Num() < LowestPossible)
		{
			Indices.Empty();
			LowestPossible = GridCells[i].Possibilities.Num();
		}

		//If equal or lower, add this index to OutIndices
		Indices.Add(i);
	}

	//Sanity check, there should be at least one lowest entropy cell
	//Technically this could happen if a tile was set before the algorithm started, may want that functionality later
	if (Indices.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("WFC quit early because there were no more uncollapsed cells.  AWFC::FindLowestEntropy"));
		CollapsedTiles = GridCells.Num();
		return -1;
	}

	//Chosen index is 0 by default
	int RandIndex = 0;

	//Randomize if more options
	if (Indices.Num() > 1)
	{
		RandIndex = FMath::RandRange(0, Indices.Num() - 1);
	}

	UE_LOG(LogTemp, Display, TEXT("Lowest Entropy (%i / %i) with %i possibilities"), RandIndex, Indices.Num(), LowestPossible);

	return Indices[RandIndex];
}


void AWFC::CollapseCell(int Index)
{
	GridCell* Cell = &GridCells[Index];

	//Ensure Cell is not null
	if (!Cell)
	{
		UE_LOG(LogTemp, Error, TEXT("Cell is not valid at GridCells index %i.  AWFC::CollapseCell"), Index);
		return;
	}

	//Sanity check, ensure there is at least one possibility
	 //check(Cell->Possibilities.Num() > 0)

	//Non-fatal erroring for sanity check, easier debugging
	if (Cell->Possibilities.Num() <= 0)
	{
		FIntVector GridInd = IntToGridIndex(Index);
		UE_LOG(LogTemp, Error, TEXT("No Possibilities in cell (%i, %i, %i)"), GridInd.X, GridInd.Y, GridInd.Z);
		PlacePrototype(0, GridInd);
		return;
	}

	//Default to 0
	int TileIndex = 0;

	//If more than one possibility, choose randomly
	if (Cell->Possibilities.Num() > 1)
	{
		TileIndex = FMath::RandRange(0, Cell->Possibilities.Num() - 1);
	}

	//Place the chosen tile
	int ProIndex = Cell->Possibilities[TileIndex];
	PlacePrototype(ProIndex, IntToGridIndex(Index));
}


void AWFC::PropogateCellChanges(int Index)
{
	//Start the stack
	PropogationIndices.Add(Index);

	int dir, oth;

	//Until all effects are realized
	while (PropogationIndices.Num() > 0)
	{
		//Store and pop the top index
		int CurrentIndex = PropogationIndices.Pop();

		//Iterate over adjacent cells
		for (dir = 0; dir < EDirection::DIR_MAX; dir++)
		{
			int OtherIndex = -1;
			//Get adjacent cell in current direction
			GridCell* OtherCell = GetAdjacentCell(CurrentIndex, (EDirection)dir, OtherIndex);
			//(EDirection)GetDirRotated((EDirection)dir, ERotation::ONE_EIGHTY)

			//Not a valid direction, could be off the map
			if (!OtherCell) { continue; }
			//Ignore if already chosen tile
			if (OtherCell->Tile) { continue; }

			//Copy possibilities to temp array
			TArray<int> OtherPossibilities = TArray<int>(OtherCell->Possibilities);

			//Use the adjacency list for this direction to decide which possible prototypes to remove
			//const FAdjacencySide* AdjList = &GridCells[Index].Tile->AdjacencyLists->Sides[dir];
			TArray<int> PossibleNeighbors = GetPossibleNeighbors(CurrentIndex, (EDirection)dir);

			//Iterate over prototype options
			for (oth = 0; oth < OtherPossibilities.Num(); oth++)
			{
				//If list of possible adjacent prototypes for current cell doesn't contain a possible prototype in an adjacent cell
				if (!PossibleNeighbors.Contains(OtherPossibilities[oth]))
				{
					//Remove possibility from adjacent cell
					OtherCell->Possibilities.Remove(OtherPossibilities[oth]);

					//Add to stack if not already in it
					if (!PropogationIndices.Contains(OtherIndex))
					{
						PropogationIndices.Add(OtherIndex);
					}
				}
			}

			//////////////////////////////
			// Print name of each possibility
			//FIntVector temp = IntToGridIndex(OtherIndex);
			//UE_LOG(LogTemp, Display, TEXT("Adjacent (Other) Cell: (%i, %i, %i)   "), temp.X, temp.Y, temp.Z);
			//for (int i = 0; i < OtherCell->Possibilities.Num(); i++)
			//{
			//	UE_LOG(LogTemp, Display, TEXT("Possibility: %i - %s"), OtherCell->Possibilities[i],
			//		*Prototypes[OtherCell->Possibilities[i]].Name);
			//}
			//temp = IntToGridIndex(CurrentIndex);
			//UE_LOG(LogTemp, Display, TEXT("Current Cell: (%i, %i, %i)   "), temp.X, temp.Y, temp.Z);
			//for (int i = 0; i < GridCells[CurrentIndex].Possibilities.Num(); i++)
			//{
			//	UE_LOG(LogTemp, Display, TEXT("Possibility: %i - %s"), GridCells[CurrentIndex].Possibilities[i],
			//		*Prototypes[GridCells[CurrentIndex].Possibilities[i]].Name);
			//}
			/////////////////////////////////
		}
	}
}


void AWFC::PlacePrototype(int PrototypeIndex, FIntVector GridIndex)
{
	//Index the 1D array as a 3D array
	GridCell* cell = GetCell(GridIndex);

	//Calculate cell location
	FVector NewLocation = GetActorLocation() + FVector(GridIndex.X * CellSize, GridIndex.Y * CellSize, GridIndex.Z * CellSize);

	//Spawn Cell
	AActor* CellModel = GetWorld()->SpawnActor(Prototypes[PrototypeIndex].Tile, &NewLocation);
	CellModel->SetActorRotation(GetRotation(Prototypes[PrototypeIndex].Rotation).Quaternion());	//Get FRotator from prototype rotation then rotate actor

	//Store spawned actor in grid
	cell->Tile = StaticCast<ATile*>(CellModel);

	//Setup const pointer to adjacency list in prototype to save space
	cell->Tile->AdjacencyLists = &Prototypes[PrototypeIndex].AdjacencyLists;

	//Update possibilities to match the index for the chosen tile
	cell->Possibilities.Empty();
	cell->Possibilities.Add(PrototypeIndex);

	//Increment to track collapsed tiles
	CollapsedTiles++;

	//////// Log Code /////////
	//Print adjacent collapsed tiles
	/*for (int i = 0; i < EDirection::DIR_MAX; i++)
	{
		int otherIndex = -1;
		GridCell* otherCell = GetAdjacentCell(GridIndexToInt(GridIndex), (EDirection)i, otherIndex);

		if (!otherCell || !otherCell->Tile) { continue; }

		FIntVector otherGrid = IntToGridIndex(otherIndex);

		UE_LOG(LogTemp, Display, TEXT("Direction: %s   Adjacent Name: %s   Grid Location: (%i, %i, %i)   "),
			*UEnum::GetValueAsString((EDirection)i),
			*Prototypes[otherCell->Possibilities[0]].Name,
			otherGrid.X, otherGrid.Y, otherGrid.Z);
	}*/

	UE_LOG(LogTemp, Display, TEXT("Collapsed: %i   Index: (%i, %i, %i)   Prototype Name: %s"), 
		CollapsedTiles, GridIndex.X, GridIndex.Y, GridIndex.Z, *Prototypes[cell->Possibilities[0]].Name)
}


TArray<int> AWFC::GetPossibleNeighbors(int Index, EDirection Direction)
{
	TSet<int> PossibleNeighbors;

	//For each possibility in the current cell
	for (int p : GridCells[Index].Possibilities)
	{
		//Add each of its possible neighbors in the given direction to the set
		for (int neighbor : Prototypes[p].AdjacencyLists.Sides[Direction].AdjacencyOptions)
		{
			PossibleNeighbors.Add(neighbor);
		}
	}

	/*UE_LOG(LogTemp, Display, TEXT("Possibilities - %i"), PossibleNeighbors.Num());*/

	//Return the set as an array
	return PossibleNeighbors.Array();
}


int AWFC::GetDirRotated(EDirection input, int modifier)
{
	if (input == EDirection::DIR_UP || input == EDirection::DIR_DOWN)
	{
		return input;
	}

	return ((input + 4) - modifier) % 4;
}


int AWFC::GetRotatedRotation(ERotation input, ERotation modifier, int RotMax)
{
	return ((input + RotMax) - modifier) % RotMax;
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
		UE_LOG(LogTemp, Error, TEXT("Invalid ERotation passed to AWFC::GetRotation: %i"), Rotation);
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
	int SingleIndex = GridIndexToInt(Index);

	//Out of range
	if (SingleIndex > GridCells.Num()) { return nullptr; }

	//Index the 1D array using 3D coordinates
	return &GridCells[SingleIndex];
}


//Get the adjacent grid cell
GridCell* AWFC::GetAdjacentCell(int Index, EDirection Direction, int& OutAdjacentIndex)
{
	FIntVector GridIndex = IntToGridIndex(Index);
	
	switch (Direction)
	{
	case EDirection::DIR_NORTH:
		//Calculate adjacent index
		GridIndex.X += 1;

		//Return cell at that index if in range
		if (GridIndex.X < GridDimensions.X)
		{
			OutAdjacentIndex = GridIndexToInt(GridIndex);
			return &GridCells[OutAdjacentIndex];
		}
		break;

	case EDirection::DIR_SOUTH:
		GridIndex.X -= 1;
		if (GridIndex.X >= 0)
		{
			OutAdjacentIndex = GridIndexToInt(GridIndex);
			return &GridCells[OutAdjacentIndex];
		}
		break;

	case EDirection::DIR_EAST:
		GridIndex.Y += 1;
		if (GridIndex.Y < GridDimensions.Y)
		{
			OutAdjacentIndex = GridIndexToInt(GridIndex);
			return &GridCells[OutAdjacentIndex];
		}
		break;

	case EDirection::DIR_WEST:
		GridIndex.Y -= 1;
		if (GridIndex.Y >= 0)
		{
			OutAdjacentIndex = GridIndexToInt(GridIndex);
			return &GridCells[OutAdjacentIndex];
		}
		break;

	case EDirection::DIR_UP:
		GridIndex.Z += 1;
		if (GridIndex.Z < GridDimensions.Z)
		{
			OutAdjacentIndex = GridIndexToInt(GridIndex);
			return &GridCells[OutAdjacentIndex];
		}
		break;

	case EDirection::DIR_DOWN:
		GridIndex.Z -= 1;
		if (GridIndex.Z >= 0)
		{
			OutAdjacentIndex = GridIndexToInt(GridIndex);
			return &GridCells[OutAdjacentIndex];
		}
		break;

	default:
		UE_LOG(LogTemp, Error, TEXT("Invalid direction passed to AWFC::GetAdjacentCell"));
	}

	//Return null if invalid index
	return nullptr;
}


inline int AWFC::GridIndexToInt(FIntVector GridIndex)
{
	return GridIndex.X + GridDimensions.X * (GridIndex.Y + GridDimensions.Y * GridIndex.Z);
}

FIntVector AWFC::IntToGridIndex(int Index)
{
	FIntVector GridIndex = FIntVector(0, 0, 0);

	GridIndex.Z = Index / (GridDimensions.X * GridDimensions.Y);
	Index -= GridIndex.Z * (GridDimensions.X * GridDimensions.Y);

	GridIndex.Y = Index / GridDimensions.X;
	Index -= GridIndex.Y * GridDimensions.X;

	GridIndex.X = Index;

	return GridIndex;
}

bool AWFC::CompareProfiles(const FString& lhs, const FString& rhs, bool Vertical) const
{
	//If vertical, the profiles must match
	if (Vertical)
	{
		return lhs == rhs;
	}

	//Otherwise handle logic if they are horizontal

	bool Symmetric = lhs.EndsWith("s");

	//Return true if symmetric and profiles match
	if (Symmetric)
	{
		return lhs == rhs;
	}

	//Profiles are mirrored if they end with f
	bool LeftMirror = lhs.EndsWith("f");
	bool RightMirror = rhs.EndsWith("f");

	//If both profiles are mirrored or not, they can not match
	if (LeftMirror == RightMirror)
	{
		return false;
	}

	//Remove the 'f' character and check if profiles match beyond that
	return (LeftMirror ? lhs.LeftChop(1) : lhs) ==
		(RightMirror ? rhs.LeftChop(1) : rhs);
}


FString AWFC::GetRotatedVerticalProfile(const FTileProfile& Profile, ERotation Rotation)
{
	//Symmetry on all sides
	bool Symmetric = Profile.ProfileName.EndsWith("s");	

	if (Symmetric)
	{
		return Profile.ProfileName;
	}

	/*
	*		i.e. rotatign a diagonally symmetric tile
	*
				0		90		180		270
	*
	*			#.		.#		#.		.#
	*			.#		#.		.#		#.
	*
	*			0 = 180			90 = 270
	*/

	//Max unique rotated profiles
	int RotMax = 4; //Typically different at 0, 90, 180, and 270 unless there is symmetry like a diagonal tile

	//Store the rotation of the current profile
	int CurrentRotation = 0;

	//Diagonal Symmetry
	bool PartiallySymmetric = Profile.ProfileName.EndsWith("p");
	FString BaseProfile = "";

	if (PartiallySymmetric)
	{
		RotMax = 2;

		//Skip "p" and read second to last character
		CurrentRotation = FCString::Atoi(&Profile.ProfileName[Profile.ProfileName.Len() - 2]);

		//Remove "_#p"
		BaseProfile = Profile.ProfileName.LeftChop(3);
	}
	else
	{
		//Read last character
		CurrentRotation = FCString::Atoi(&Profile.ProfileName[Profile.ProfileName.Len() - 1]);

		//Remove "_#"
		BaseProfile = Profile.ProfileName.LeftChop(2);

		//UE_LOG(LogTemp, Warning, TEXT("Original: %s    Current Rotation: %i     Base: %s"), *Profile.ProfileName, CurrentRotation, *BaseProfile);
	}

	//Calculate new rotation
	int NewRotation = GetRotatedRotation((ERotation)CurrentRotation, (ERotation)(-(int)Rotation), RotMax);

	//Append to end of profile to create rotated profile name
	return BaseProfile + "_" + FString::FromInt(NewRotation) + (PartiallySymmetric ? "p" : "");
}

inline bool AWFC::InvalidEmptyProfile(int ThisIndex, FString ProfileName)
{
	return (EmptyProfileIsAlwaysEmpty && ThisIndex > 0 && ProfileName == EmptyProfile);
}