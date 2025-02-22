#include "Components/LMAWeaponComponent.h"
#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Weapon/LMABaseWeapon.h"
#include "TimerManager.h"

ULMAWeaponComponent::ULMAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();

	InitAnimNotify();

}

void ULMAWeaponComponent::SpawnWeapon()
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
	if (Weapon)
	{
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character)
		{
			FAttachmentTransformRules
				AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");
		}

		Weapon->OnClipsEmpty.AddUObject(this, &ULMAWeaponComponent::AutoReload);
	}
}

void ULMAWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage)return;
	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
		if (ReloadFinish)
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
	}
}

bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading;
}

void ULMAWeaponComponent::AutoReload()
{
	Reload();
}

void ULMAWeaponComponent::Reload()
{
	if (CanReload())
	{
		AnimReloading = true;
		if (LongFireActivated)
		{
			Weapon->FireDeactivate();
		}
		ACharacter* Character = Cast<ACharacter>(GetOwner());
		Character->PlayAnimMontage(ReloadMontage);
	}

}

void ULMAWeaponComponent::Fire()
{
	if (IsValid(Weapon) && !AnimReloading && !FireProhibited)
	{
		Weapon->Fire();
	}
}

void ULMAWeaponComponent::StartLongFire()
{
	if (IsValid(Weapon))
	{
		LongFireActivated = true;

		if (!AnimReloading && !FireProhibited)
		{
			Weapon->FireActivate();
		}
	}
}

void ULMAWeaponComponent::StopLongFire()
{
	if (IsValid(Weapon))
	{
		Weapon->FireDeactivate();
		LongFireActivated = false;
	}
}

void ULMAWeaponComponent::ManualReload()
{
	Reload();
}

void ULMAWeaponComponent::FireProhibition()
{
	FireProhibited = true;
	if (LongFireActivated)
	{
		Weapon->FireDeactivate();
	}
}

void ULMAWeaponComponent::FirePermission()
{
	FireProhibited = false;
	if (LongFireActivated)
	{
		StartLongFire();
	}
}

void ULMAWeaponComponent::ReloadProhibition()
{
	ReloadProhibited = true;
}

void ULMAWeaponComponent::ReloadPermission()
{
	ReloadProhibited = false;
}

bool ULMAWeaponComponent::IsReloading()
{
	return AnimReloading;
}