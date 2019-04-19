// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class ASWeapon;
UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	//para correr el const class FObjectInitializer& ObjectInitializer
	ASCharacter(const class FObjectInitializer& ObjectInitializer);

	//correr
	UFUNCTION(BlueprintCallable, Category = "Movement")
	virtual bool IsSprinting() const;

	/* Client/local call to update sprint state  */
	virtual void SetSprinting(bool NewSprinting);

	float GetSprintingSpeedModifier() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//funciones
	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void BeginProne();

	void BeginJump();
	
	void EndJump();

	void BeginSprint();

	void SprintEnd();

	//Para Correr
	/* Server side call to update actual sprint state */
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetSprinting(bool NewSprinting);

	void ServerSetSprinting_Implementation(bool NewSprinting);

	bool ServerSetSprinting_Validate(bool NewSprinting);

	//funciones para apuntar
	void BeginZoom();

	void EndZoom();

	//Weapon
	ASWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ASWeapon>StartWeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon" )
	FName WeaponAttachSocketName;

	void StartFire();

	void StopFire();
	//componentes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	//variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool IsCrouching;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool IsProne;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	bool IsProneComplete;

	//variable para correr
	UPROPERTY(EditDefaultsOnly, Category = "Movement")
		float SprintingSpeedModifier;

	/* Character wants to run, checked during Tick to see if allowed */
	//UPROPERTY(Transient, Replicated)
		bool bWantsToRun;

	//variables para enfocar
	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom")
	float ZoomInterpSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Zoom", meta = (ClampMin = 0.1, ClampMax = 100))
	float ZoomedFOV;
		
	/* Default FOV set during begin play*/
	float DefaultFOV;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

// 	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement")
	float SpritingSpeedMultiplier;
	
	
};
