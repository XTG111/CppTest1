// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "JCharacter.h"
#include "WhiteManAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CPPTEST1_API UWhiteManAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	//动画蓝图初始化
	virtual void NativeInitializeAnimation() override;

	//event tick: log in main Func 
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	friend class AJCharacter;

private:
	UPROPERTY(BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
		 AJCharacter* JCharacter_Ins;

	//动画中需要用到的参数
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float ForwardInput;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float RightInput;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		bool bIsCrouch;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		bool bIsRun;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		bool bCanJump;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		bool bCanMove;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		bool bIsFalling;
	UPROPERTY(BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
		bool bHaveWeapon;
	//控制蹲，走，跑切换
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float DesireState;
	//控制是否停止时的Idle
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float StopFlag;
	//最终切换
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float CurrentMoveState;
	//移动方向向量
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		FVector InputVelocityVec;

	//Yaw 和 Pitch
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float Yaw;
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float Pitch;
	//移动时的方向-180~180
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float MoveLoopDirection;
	//移动速度
	UPROPERTY(BlueprintReadOnly, Category = MoveInput, meta = (AllowPrivateAccess = "true"))
		float Speed;

	//蓝图中可修改的变量
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		float UpFoot;
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		float JumpState;
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		bool bLand;
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		float MoveStartDirection;

	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		FRotator YawRotator;
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		FVector ForwardVec;
	UPROPERTY(BlueprintReadWrite, Category = MoveStop, meta = (AllowPrivateAccess = "true"))
		FVector RightVec;


public:
	//获取角色中的参数
	UFUNCTION()
		void GetVariable();

	//设置移动状态
	UFUNCTION()
		void SetState(float DeltaTime);
	void SetMoveVector();
};
