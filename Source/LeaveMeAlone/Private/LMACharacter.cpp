// Fill out your copyright notice in the Description page of Project Settings.


#include "LMACharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALMACharacter::ALMACharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Параметры, соответствующие жанру игры
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void ALMACharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}
	
}

// Called every frame
void ALMACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}

// Called to bind functionality to input
void ALMACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMACharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMACharacter::MoveRight);
	PlayerInputComponent->BindAction("CameraZoom", IE_Pressed,  this, &ALMACharacter::CameraZoom);
	PlayerInputComponent->BindAction("CameraAway", IE_Pressed, this, &ALMACharacter::CameraAway);
}

void ALMACharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMACharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMACharacter::CameraZoom(void)
{
	if (ArmLength > ArmLengthMin)
	{
		ArmLength -= SmoothZoom;
		if (ArmLength < ArmLengthMin)
		{
			ArmLength = ArmLengthMin;
		}
		SpringArmComponent->TargetArmLength = ArmLength;
	}
}

void ALMACharacter::CameraAway(void)
{
	if (ArmLength < ArmLengthMax)
	{
		ArmLength += SmoothZoom;
		if (ArmLength > ArmLengthMax)
		{
			ArmLength = ArmLengthMax;
		}
		SpringArmComponent->TargetArmLength = ArmLength;
	}
}

