#include "Weapon/LMABaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ALMABaseWeapon::ALMABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	SetRootComponent(WeaponComponent);
}

void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	CurrentAmmoWeapon = AmmoWeapon;
}

void ALMABaseWeapon::Fire()
{
	if (CurrentAmmoWeapon.Bullets > 0)
	{
		Shoot();
	}
}

void ALMABaseWeapon::Shoot()
{
	const FTransform SocketTransform = WeaponComponent->GetSocketTransform("Muzzle");
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + ShootDirection * TraceDistance;
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility);
	if (HitResult.bBlockingHit)
	{
		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
	}
	DecrementBullets();
}

void ALMABaseWeapon::ChangeClip()
{
	if (AmmoWeapon.Infinite)
	{
		CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
	}
	else if (CurrentAmmoWeapon.Clips > 0)
	{
		CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;;
		--CurrentAmmoWeapon.Clips;
	}
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}

bool ALMABaseWeapon::IsCurrentClipFool() const
{
	return CurrentAmmoWeapon.Bullets == AmmoWeapon.Bullets;
}

void ALMABaseWeapon::DecrementBullets()
{
	CurrentAmmoWeapon.Bullets--;

	UE_LOG(LogWeapon, Display, TEXT("Bullets = %s"), *FString::FromInt(CurrentAmmoWeapon.Bullets));

	if (IsCurrentClipEmpty())
	{
		OnClipsEmpty.Broadcast();
	}
}

FAmmoWeapon ALMABaseWeapon::getAmmoWeapon()
{
	return CurrentAmmoWeapon;
}

void ALMABaseWeapon::FireActivate()
{
	Fire();
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ALMABaseWeapon::Fire, 60 / FrenquencyFire, true);
}

void ALMABaseWeapon::FireDeactivate()
{
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}