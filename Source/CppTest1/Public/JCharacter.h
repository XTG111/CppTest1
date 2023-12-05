// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TimelineComponent.h"
#include "JCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;
class UTimelineComponen;

UCLASS()
class CPPTEST1_API AJCharacter : public ACharacter
{
	GENERATED_BODY()

//动画蓝图需要获取的参数
public:
	//获取前后左右移动的值,用于动画蓝图
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float ForwardInput;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float RightInput;

	//跳跃状态
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool InJump = false;

	//奔跑状态
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool InRun = false;

	//判断是否在下蹲
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsCrouch = false;

	//判断是否装备武器
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bSetWeapon = false;

public:
	// Sets default values for this character's properties
	AJCharacter();

	////定义一个时间轴，实现下蹲和起立的相机视角切换
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//	UTimelineComponent*  CWTimeline;

	////定义一个采样曲线
	//UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	//	 UCurveFloat* CWFloatCurve;

	////定义曲线更新事件
	//FOnTimelineFloat OnCWTimelineTickCallBack;
	////定义曲线播发完成事件
	//FOnTimelineEvent OnCWTimelineFinishedCallBack;

	//UFUNCTION()
	//	void CWTimelineTickCallBack(float value);
	//UFUNCTION()
	//	void CWTimelineFinishedCallBack();

	//实现Delay
	FTimerHandle TimerHandle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsJump = false;
	//当在Jump动画时，更改，然后延时动画时长之后修改回来
	void ResetbIsJump();
	//重新恢复输入
	void ResetInput();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//相机+弹簧臂组件
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComp;

	//动画蒙太奇
	UPROPERTY(EditAnywhere, Category = "Jump")
		UAnimMontage* WalkJump;
	UPROPERTY(EditAnywhere, Category = "Jump")
		UAnimMontage* JogJump;
	UPROPERTY(EditAnywhere, Category = "Jump")
		UAnimMontage* RunJump;

	//左右前后移动函数
	void MoveForward(float value);
	void MoveRight(float value);
	
	//定义速度切换
	//void WalkControl();
	void RunControl();
	void JogControl();

	////重写Jump,以实现播放蒙太奇
	void PrJump();
	void ReJump();
	//播放完成之后才能继续跳跃
	bool bInJump = false;

	//实现蹲伏功能
	void PreJCrouch();

	//实现装备武器功能
	
	void SetWeapon();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
