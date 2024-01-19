// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tile.h"

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

};
