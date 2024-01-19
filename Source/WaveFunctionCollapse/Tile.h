// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS(BlueprintType, Blueprintable)
class WAVEFUNCTIONCOLLAPSE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int id;	//Unique to each tile

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
