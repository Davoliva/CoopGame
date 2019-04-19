// Fill out your copyright notice in the Description page of Project Settings.

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SCharacterMovementComponent.h"
#include "SWeapon.h"

// Sets default values
ASCharacter::ASCharacter(const class FObjectInitializer& ObjectInitializer)
	/* Override the movement class from the base class to our own to support multiple speeds (eg. sprinting) */
	: Super(ObjectInitializer.SetDefaultSubobjectClass<USCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	
	SprintingSpeedModifier = 2.0f;

	ZoomedFOV = 65.0f;
	ZoomInterpSpeed = 20;

	WeaponAttachSocketName = "WeaponSocket";
}

bool ASCharacter::IsSprinting() const
{
	if (!GetCharacterMovement())
	{
		return false;
	}

	return bWantsToRun && !GetVelocity().IsZero()
		// Don't allow sprint while strafing sideways or standing still (1.0 is straight forward, -1.0 is backward while near 0 is sideways or standing still)
		&& (FVector::DotProduct(GetVelocity().GetSafeNormal2D(), GetActorRotation().Vector()) > 0.8); // Changing this value to 0.1 allows for diagonal sprinting. (holding W+A or W+D keys)
}




// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Enfoque por defecto
	DefaultFOV = CameraComp->FieldOfView;

	//Generar un arma por defecto
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StartWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwner(this);
		CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponAttachSocketName);
	}
	
}
//funcion para caminar adelante
void ASCharacter::MoveForward(float Value)
{
	if (IsProne != true && IsProneComplete == false) {
		
			AddMovementInput(GetActorForwardVector()* Value);
	}
	
}
//funcion para caminar a los lados
void ASCharacter::MoveRight(float Value)
{
	if (IsProne != true && IsProneComplete == false) {
		AddMovementInput(GetActorRightVector()* Value);
	}
	
}
//funcion para comenzar a agacharse
void ASCharacter::BeginCrouch()
{
	if (IsProne == true) {
		//event set is prone
		if (IsProne == true) {
			IsProne = false;
		}
		else
		{
			IsProne = true;
		}
			//
			if (IsCrouching == true) {
				UnCrouch();
				//event set is crouching
				if (IsCrouching == true) {
					IsCrouching = false;
				}
				else {
					IsCrouching = true;
				}
			}
			else
			{
				
				Crouch();
				//event set is crounching
				if (IsCrouching == true) {
					IsCrouching = false;
				}
				else {
					IsCrouching = true;
				}
			}
	}
	else
	{
		if (IsCrouching == true) {
			UnCrouch();
			//event set is crouching
			if (IsCrouching == true) {
				IsCrouching = false;
			}
			else {
				IsCrouching = true;
			}
		}
		else
		{
			Crouch();
			//event set is crouching
			if (IsCrouching == true) {
				IsCrouching = false;
			}
			else {
				IsCrouching = true;
			}
		}
	}
}


void ASCharacter::BeginProne()
{
	if (IsCrouching == true)
	{
		//event set is crouching
		if (IsCrouching == true) {
			IsCrouching = false;
		}
		else {
			IsCrouching = true;
		}
			//Prone
			if (IsProne == true)
			{
				UnCrouch();
				//event is prone
				if (IsProne == true) {
					IsProne = false;
				}
				else
				{
					IsProne = true;
				}
			}
			else
			{
				Crouch();
				//event is prone
				if (IsProne == true) {
					IsProne = false;
				}
				else
				{
					IsProne = true;
				}
			}

	}
	else
	{
		//Prone
		if (IsProne == true)
		{
			
			UnCrouch();
			//Event is prone
			if (IsProne == true) {
				IsProne = false;
			}
			else
			{
				IsProne = true;
			}
		}
		else
		{
			
			Crouch();
			//event is prone
			if (IsProne == true) {
				IsProne = false;
			}
			else
			{
				IsProne = true;
			}
		}
	}
}


//funcion para comenzar a saltar
void ASCharacter::BeginJump()
{
	Jump();
	
}
//funcion para termianar el salto
void ASCharacter::EndJump()
{
	StopJumping();
	
}

void ASCharacter::SprintEnd()
{
	
	/*GetCharacterMovement()->MaxWalkSpeed /= SpritingSpeedMultiplier;*/
	SetSprinting(false);
}



void ASCharacter::ServerSetSprinting_Implementation(bool NewSprinting)
{
	SetSprinting(NewSprinting);
}

bool ASCharacter::ServerSetSprinting_Validate(bool NewSprinting)
{
	return true;
}

void ASCharacter::BeginZoom()
{
	bWantsToZoom = true;
}

void ASCharacter::EndZoom()
{
	bWantsToZoom = false;
}

void ASCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	}
}

void ASCharacter::StopFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StopFire();
	}
}

void ASCharacter::SetSprinting(bool NewSprinting)
{
	if (bWantsToRun)
	{
		//StopWeaponFire();
	}

	bWantsToRun = NewSprinting;

	if (bIsCrouched)
	{
		UnCrouch();
	}

	if (Role < ROLE_Authority)
	{
		ServerSetSprinting(NewSprinting);
	}
}

float ASCharacter::GetSprintingSpeedModifier() const
{
	return SprintingSpeedModifier;
}

void ASCharacter::BeginSprint()
{
	/*GetCharacterMovement()->MaxWalkSpeed *= SpritingSpeedMultiplier;*/

	SetSprinting(true);
	 
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetFOV;
	if (bWantsToZoom == true)
	{
		TargetFOV = ZoomedFOV;
	}
	else 
	{
		TargetFOV = DefaultFOV;
	}

	float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, DeltaTime, ZoomInterpSpeed);
	CameraComp->SetFieldOfView(NewFOV);

	if (bWantsToRun && !IsSprinting())
	{
		SetSprinting(true);
	}

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//entrada para caminar
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	//entradas para mover mouse
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);
	//entrada para agacharse
	PlayerInputComponent->BindAction("Crouch", IE_Pressed,this, &ASCharacter::BeginCrouch);
	//Entrada para boca abajo
	PlayerInputComponent->BindAction("Prone", IE_Pressed, this, &ASCharacter::BeginProne);

	//entrada de salto
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::BeginJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ASCharacter::EndJump);

	//Entrada de correr
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::SprintEnd);

	//Entrada de enfocar
	PlayerInputComponent->BindAction("Zoom", IE_Pressed, this, &ASCharacter::BeginZoom);
	PlayerInputComponent->BindAction("Zoom", IE_Released, this, &ASCharacter::EndZoom);

	//Entrada de disparar
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASCharacter::StopFire);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if(CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}


