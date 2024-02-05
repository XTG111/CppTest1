// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework\CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetMathLibrary.h"
#include "JCharacter.h"
#include "WhiteManAnimInstance.h"

// Sets default values
AJCharacter::AJCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//将相机和弹簧臂组件绑定在人物角色上
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	GetCharacterMovement()->MaxWalkSpeedCrouched = 100.f;
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	GetCharacterMovement()->JumpZVelocity = 300.0f;
}

//Delay函数绑定
void AJCharacter::ResetbIsJump()
{
	bIsJump = false;
	bInJump = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void AJCharacter::ResetInput()
{
	EnableInput(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

//当在障碍物下按下蹲时无法站立
bool AJCharacter::CheckCouldCrouch2Stand()
{
	//设置检测点
	FVector HeadLoc = UKismetMathLibrary::MakeVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 300.f);
	//检测碰撞
	FHitResult Hit;
	//设置检测哪些物体
	FCollisionQueryParams Params;
	//不要检测自己
	Params.AddIgnoredActor(this);
	//配置碰撞检测条件
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	bool IsCheck = GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), HeadLoc, ECC_Visibility, Params);
	return IsCheck;
}

// Called when the game starts or when spawned
void AJCharacter::BeginPlay()
{
	Super::BeginPlay();
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UWhiteManAnimInstance* AnimIns = Cast<UWhiteManAnimInstance>(AnimInstance);
		if (AnimIns)
		{
			AnimIns->StopFlag = 0.f;
		}
	}
}

//左右前后移动
void AJCharacter::MoveForward(float value)
{
	FRotator ControlRota = GetControlRotation();
	ControlRota.Pitch = 0.0f;
	ControlRota.Roll = 0.0f;
	ForwardInput = value;
	//对应蓝图中的make Rotator + GetForwardVector
	FVector ForwardVector = FRotationMatrix(ControlRota).GetScaledAxis(EAxis::X);

	AddMovementInput(ForwardVector, value);
}

void AJCharacter::MoveRight(float value)
{
	FRotator ControlRota = GetControlRotation();
	ControlRota.Pitch = 0.0f;
	ControlRota.Roll = 0.0f;
	RightInput = value;
	//对应蓝图中的make Rotator + GetRightVector
	FVector RightVector = FRotationMatrix(ControlRota).GetScaledAxis(EAxis::Y);

	AddMovementInput(RightVector, value);
}

void AJCharacter::RunControl()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->JumpZVelocity = 400.0f;
	bInRun = true;
}

void AJCharacter::JogControl()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	GetCharacterMovement()->JumpZVelocity = 300.0f;
	bInRun = false;
}

void AJCharacter::ReJump()
{
	Super::StopJumping();

	bCanJump = false;
}

void AJCharacter::PrJump()
{
	Super::Jump();
	bCanJump = true;
}

void AJCharacter::PreJCrouch()
{
	bool check = CheckCouldCrouch2Stand();
	if (GetCharacterMovement()->IsCrouching() && !check) 
	{
		UnCrouch();
		bIsCrouch = false;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			UWhiteManAnimInstance* AnimIns = Cast<UWhiteManAnimInstance>(AnimInstance);
			if (AnimIns)
			{
				AnimIns->StopFlag = 0.f;
			}
		}
	}
	else
	{
		Crouch();
		bIsCrouch = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			UWhiteManAnimInstance* AnimIns = Cast<UWhiteManAnimInstance>(AnimInstance);
			if (AnimIns)
			{
				AnimIns->StopFlag = 1.f;
			}
		}
	}
}

void AJCharacter::SetWeapon()
{
	if (!bSetWeapon) {
		bSetWeapon = true;
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}

	else {
		bSetWeapon = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

// Called every frame
void AJCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AJCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//绑定行为
	//PlayerInputComponent->BindAction("WalkControl", IE_Pressed, this, &AJCharacter::WalkControl);
	PlayerInputComponent->BindAction("RunControl", IE_Pressed, this, &AJCharacter::RunControl);
	//PlayerInputComponent->BindAction("WalkControl", IE_Released, this, &AJCharacter::JogControl);
	PlayerInputComponent->BindAction("RunControl", IE_Released, this, &AJCharacter::JogControl);
	PlayerInputComponent->BindAction("JumpControl", IE_Pressed, this, &AJCharacter::PrJump);
	PlayerInputComponent->BindAction("JumpControl", IE_Released, this, &AJCharacter::ReJump);
	PlayerInputComponent->BindAction("CrouchControl", IE_Pressed, this, &AJCharacter::PreJCrouch);
	PlayerInputComponent->BindAction("SetWeapon", IE_Pressed, this, &AJCharacter::SetWeapon);

	//绑定键盘映射
	//Yaw对应Z轴，Pitch对应Y轴，Roll对应X轴
	PlayerInputComponent->BindAxis("MoveForward", this, &AJCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

}

void AJCharacter::SetMoveStartDirection()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UWhiteManAnimInstance* AnimIns = Cast<UWhiteManAnimInstance>(AnimInstance);
		if (AnimIns)
		{
			AnimIns->MoveStartDirection = AnimIns->MoveLoopDirection;
		}
	}
}

void AJCharacter::SetStartUpFoot()
{

}

void AJCharacter::SetLoopUpFoot()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UWhiteManAnimInstance* AnimIns = Cast<UWhiteManAnimInstance>(AnimInstance);
		if (AnimIns)
		{
			float NowFoot = AnimIns->GetCurveValue(FName("UpFoot"));
			if (NowFoot > 0.f)
			{
				AnimIns->UpFoot = 1.f;
			}
			else
			{
				AnimIns->UpFoot = -1.f;
			}
		}
	}
}

void AJCharacter::SetJumpState()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UWhiteManAnimInstance* AnimIns = Cast<UWhiteManAnimInstance>(AnimInstance);
		if (AnimIns)
		{
			if (!AnimIns->bCanMove)
			{
				AnimIns->JumpState = 1.f;
			}
			else
			{
				if (AnimIns->bIsCrouch)
				{
					AnimIns->JumpState = 1.f;
				}
				else
				{
					AnimIns->JumpState = AnimIns->CurrentMoveState;
				}
			}
		}
	}
}

void AJCharacter::JumpStartToLand()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UWhiteManAnimInstance* AnimIns = Cast<UWhiteManAnimInstance>(AnimInstance);
		if (AnimIns)
		{
			AnimIns->bLand = true;
		}
	}
	GetWorldTimerManager().SetTimer(LandTime, this, &AJCharacter::LandTimeFinished, LandDelayTime);
}

void AJCharacter::LandTimeFinished()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UWhiteManAnimInstance* AnimIns = Cast<UWhiteManAnimInstance>(AnimInstance);
		if (AnimIns)
		{
			AnimIns->bLand = false;
		}
	}
	GetWorldTimerManager().ClearTimer(LandTime);
}


