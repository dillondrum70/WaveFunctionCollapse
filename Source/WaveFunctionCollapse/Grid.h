// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Grid.generated.h"


/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class WAVEFUNCTIONCOLLAPSE_API UGrid : public USceneComponent
{

	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
