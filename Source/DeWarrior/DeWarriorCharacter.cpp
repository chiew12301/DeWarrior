// Copyright Epic Games, Inc. All Rights Reserved.

#include "DeWarriorCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "OpenAttackNotify.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "CustomCameraShake.h"
#include "LightAttackCameraShake.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ADeWarriorCharacter

ADeWarriorCharacter::ADeWarriorCharacter()
{
	this->m_curAttackCount = 0;
	this->m_MaxComboIndex = ComboMontages.Num();
	this->m_bIsComboWindowOpen = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ADeWarriorCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


void ADeWarriorCharacter::OnAnimationNotify()
{
	UE_LOG(LogTemp, Warning, TEXT("Notify:Open Attack!"));
	this->m_bIsComboWindowOpen = true;
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADeWarriorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ADeWarriorCharacter::Attack);
		//EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ADeWarriorCharacter::ResetCombo);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADeWarriorCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADeWarriorCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ADeWarriorCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ADeWarriorCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADeWarriorCharacter::Attack()
{
	UE_LOG(LogTemp, Warning, TEXT("Triggered: Attack"));

	if (!this->m_bIsComboWindowOpen) return;
	UE_LOG(LogTemp, Warning, TEXT("Register: Attack"));

	this->ProceedAttackAnimation();

	if (!this->IsFinalComboStep())
	{
		this->PlayLightCameraShake();
		this->m_curAttackCount++;
	}
	else
	{
		this->PlayCameraShake();
		this->ResetCombo();
	}
	UE_LOG(LogTemp, Warning, TEXT("Current Count: %d"), this->m_curAttackCount);
}

void ADeWarriorCharacter::ProceedAttackAnimation()
{
	if (ComboMontages.IsValidIndex(this->m_curAttackCount) && ComboMontages[this->m_curAttackCount])
	{
		UE_LOG(LogTemp, Warning, TEXT("Animation Play: %d"), this->m_curAttackCount);
		// Play the montage corresponding to the current combo step
		UAnimMontage* CurrentMontage = ComboMontages[this->m_curAttackCount];

		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &ADeWarriorCharacter::OnComboMontageEnded);

			// Play the montage
			AnimInstance->Montage_Play(CurrentMontage);

			const auto NotifyEvents = CurrentMontage->Notifies;
			for (FAnimNotifyEvent EventNotify : NotifyEvents)
			{
				if (const auto curNotify = Cast<UOpenAttackNotify>(EventNotify.Notify))
				{
					curNotify->OnNotified.RemoveAll(this);
					curNotify->OnNotified.AddUObject(this, &ADeWarriorCharacter::OnAnimationNotify);
				}
			}
			// Bind to the delegate that is called when any montage ends
			AnimInstance->OnMontageEnded.AddDynamic(this, &ADeWarriorCharacter::OnComboMontageEnded);

			if (ComboSounds.IsValidIndex(this->m_curAttackCount) && ComboSounds[this->m_curAttackCount])
			{
				// Play the sound at the character's location
				UGameplayStatics::PlaySoundAtLocation(this, ComboSounds[this->m_curAttackCount], GetActorLocation());
			}

			this->m_bIsComboWindowOpen = false;
		}
	}
}

void ADeWarriorCharacter::OnComboMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	UE_LOG(LogTemp, Warning, TEXT("Montage Ended"));
	int checkCount = 0;

	if (this->m_curAttackCount > 0)
	{
		checkCount = this->m_curAttackCount - 1;
	}
	else
	{
		checkCount = ComboMontages.Num() - 1;
	}

	if (Montage == ComboMontages[checkCount])
	{
		this->m_bIsComboWindowOpen = true;
		if (!bInterrupted)
		{
			UE_LOG(LogTemp, Warning, TEXT("Montage Play Completed, Reset Combo"));
			this->ResetCombo();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Montage Interupted do attack"));
		}

		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->OnMontageEnded.RemoveDynamic(this, &ADeWarriorCharacter::OnComboMontageEnded);
		}
	}
}

bool ADeWarriorCharacter::IsFinalComboStep() const
{
	return this->m_curAttackCount >= this->m_MaxComboIndex - 1;
}

void ADeWarriorCharacter::PlayCameraShake()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController != nullptr)
	{
		APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;

		if (CameraManager)
		{
			// Play the camera shake (UMyCameraShake is your custom camera shake class)
			CameraManager->StartCameraShake(UCustomCameraShake::StaticClass());
		}
	}
}

void ADeWarriorCharacter::PlayLightCameraShake()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController != nullptr)
	{
		APlayerCameraManager* CameraManager = PlayerController->PlayerCameraManager;

		if (CameraManager)
		{
			// Play the camera shake (UMyCameraShake is your custom camera shake class)
			CameraManager->StartCameraShake(ULightAttackCameraShake::StaticClass());
		}
	}
}

void ADeWarriorCharacter::ResetCombo()
{
	this->m_curAttackCount = 0;
	GetWorldTimerManager().ClearTimer(ComboResetTimer);
}