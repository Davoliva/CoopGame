// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"


// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	

	// ...
	Health = 100;
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
	}

	Health = DefaultHealth;
}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f)
	{
		return;
	}
	//actualizacion de vida sujeta
	Health = FMath::Clamp(Health - Damage, 0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s"), *FString::SanitizeFloat(Health));
}

