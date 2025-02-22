#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/LMABaseWeapon.h"
#include "LMAWeaponComponent.generated.h"

// Оставить

class ALMABaseWeapon;
class UAnimMontage;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class LEAVEMEALONE_API ULMAWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULMAWeaponComponent();

	void Fire();

	void StartLongFire();

	void StopLongFire();

	void ManualReload();

	void FireProhibition();

	void FirePermission();

	void ReloadProhibition();

	void ReloadPermission();

	bool IsReloading();

	void Reload();

	UPROPERTY()
	ALMABaseWeapon* Weapon = nullptr;


	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ALMABaseWeapon> WeaponClass;
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* ReloadMontage;
	
	virtual void BeginPlay() override;

private:
	void AutoReload();

	void InitAnimNotify();

	void SpawnWeapon();

	bool CanReload() const;

	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh);

	bool AnimReloading = false;
	bool LongFireActivated = false;
	bool FireProhibited = false;
	bool ReloadProhibited = false;

};
