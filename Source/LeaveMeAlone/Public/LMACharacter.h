// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMACharacter.generated.h"

class UCameraComponent;		// Компонент камеры
class USpringArmComponent;	//  Управление поведением камеры

UCLASS()
class LEAVEMEALONE_API ALMACharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMACharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ArmLengthMin = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ArmLengthMax = 1400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float SmoothZoom = 100.f;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float YRotation = -75.0f;	// Поворот камеры по оси Y
	float ArmLength = 1400.0f;	// Длина штатива
	float FOV = 55.0f;			// Угол зрения камеры

	void MoveForward(float Value);	// Движение персонажа по оси X
	void MoveRight(float Value);	// Движение персонажа по оси Y

	void CameraZoom(void);	// Приближение камеры к персонажу
	void CameraAway(void);	// Отдаление камеры от персонажа
	
};
