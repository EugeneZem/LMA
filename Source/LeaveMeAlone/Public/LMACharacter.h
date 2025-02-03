// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMACharacter.generated.h"

class UCameraComponent;		//	��������� ������
class USpringArmComponent;	//	���������� ���������� ������
class ULMAHealthComponent;	//	�������� 
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMACharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMACharacter();

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sprint")
	bool IsSprint = false;		// �����

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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Durability")
	float Durability = 100.0f;			// ������������

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

	void CameraZoom(void);	// ����������� ������ � ��������� (������� 05)
	void CameraAway(void);	// ��������� ������ �� ��������� (������� 05)
	
	void SprintActivate(void);
	void SprintDeActivate(void);



	FTimerHandle TimerDurability;	// ������ ��� ��������� ������������
	float TimerDurabilityRate = 0.5f;
	void DurabilityControl();

	void OnDeath();
	void OnHealthChanged(float NewHealth);

	void RotationPlayerOnCursor();


	
};
