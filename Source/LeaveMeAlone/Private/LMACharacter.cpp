// Fill out your copyright notice in the Description page of Project Settings.


#include "LMACharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LMAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

// Sets default values
ALMACharacter::ALMACharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// ���������, ��������������� ����� ����
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

	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");
}

// Called when the game starts or when spawned
void ALMACharacter::BeginPlay()
{
	Super::BeginPlay();

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnDeath.AddUObject(this, &ALMACharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMACharacter::OnHealthChanged);
}

void ALMACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(HealthComponent->IsDead()))
	{
		RotationPlayerOnCursor();
	}

	DurabilityControl();
}

// Called to bind functionality to input
void ALMACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMACharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMACharacter::MoveRight);
	PlayerInputComponent->BindAction("CameraZoom", IE_Pressed, this, &ALMACharacter::CameraZoom);
	PlayerInputComponent->BindAction("CameraAway", IE_Pressed, this, &ALMACharacter::CameraAway);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMACharacter::Sprint);

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

void ALMACharacter::OnDeath()
{
	CurrentCursor->DestroyRenderState_Concurrent();
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ALMACharacter::OnHealthChanged(float NewHealth)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));
}

void ALMACharacter::RotationPlayerOnCursor()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),
			ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}

//	��������� ����
void ALMACharacter::Sprint(void)
{
	if (Durability > 0)
	{
		IsSprint = true;

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Durability = %f"), Durability));
	}
}

void ALMACharacter::DurabilityChange(void)
{
	//if (IsSprint) Durability = FMath::Clamp(Durability - 5, 0.0f, 100.0f);
	//else Durability = FMath::Clamp(Durability + 2.5f, 0.0f, 100.0f);

//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Durability = %f"), Durability));
}

void ALMACharacter::DurabilityControl(void)
{
	//	�������� ����� ���������� ������������
//	GetWorldTimerManager().SetTimer(DelayChangeDurability, this, &ALMACharacter::DurabilityChange, 1);

	if (IsSprint) Durability = FMath::Clamp(Durability - 5, 0.0f, 100.0f);
	else Durability = FMath::Clamp(Durability + 2.5f, 0.0f, 100.0f);
		
//	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Durability = %f"), Durability));
//	IsSprint = false;
}