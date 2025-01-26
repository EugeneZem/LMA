// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMACharacter.generated.h"

class UCameraComponent;		// ��������� ������
class USpringArmComponent;	//  ���������� ���������� ������

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
	float YRotation = -75.0f;	// ������� ������ �� ��� Y
	float ArmLength = 1400.0f;	// ����� �������
	float FOV = 55.0f;			// ���� ������ ������

	void MoveForward(float Value);	// �������� ��������� �� ��� X
	void MoveRight(float Value);	// �������� ��������� �� ��� Y

	void CameraZoom(void);	// ����������� ������ � ���������
	void CameraAway(void);	// ��������� ������ �� ���������
	
};
