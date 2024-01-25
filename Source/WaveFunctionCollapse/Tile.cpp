// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"


FAdjacencySide::FAdjacencySide()
{

}

FAdjacencySides::FAdjacencySides()
{
	Sides.SetNum(6);	//Must have 6 entries, one for each side
}

bool FTileProfile::operator==(const FTileProfile& rhs)
{
	return ProfileName == rhs.ProfileName;
}

// Sets default values
ATile::ATile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileProfiles.SetNum(4);
	VerticalTileProfiles.SetNum(2);
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

