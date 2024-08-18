// Copyright Epic Games, Inc. All Rights Reserved.

#include "DonkeyKong_L01Character.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"//LIBREARIA PARA GENERAR UNA  VELOCIDAD ALEATORIA
#include "Obstaculo.h" //INCLUYENDO EL ARCHIVO DE ENCABEZADO (2)
#include "Enemigo.h"//(2)
#include "Barril.h"
 
ADonkeyKong_L01Character::ADonkeyKong_L01Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SocketOffset = FVector(0.f,0.f,75.f);
	CameraBoom->SetRelativeRotation(FRotator(0.f,180.f,0.f));

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1500.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADonkeyKong_L01Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAxis("MoveRight", this, &ADonkeyKong_L01Character::MoveRight);

	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADonkeyKong_L01Character::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADonkeyKong_L01Character::TouchStopped);
}

void ADonkeyKong_L01Character::BeginPlay()
{
	Super::BeginPlay();

	setRandomSpeed();

	FVector SpawnLocation = GetActorLocation();//(2)
	GetWorld()->SpawnActor<AObstaculo>(AObstaculo::StaticClass(), FVector(1207, -516, 104), FRotator(0, 0, 0));//Obstaculo(3)

	GetWorld()->SpawnActor<AEnemigo>(AEnemigo::StaticClass(), FVector(1207, -516, 800), FRotator(0, 0, 0));//Enemigo(3)

	GetWorld()->SpawnActor<ABarril>(ABarril::StaticClass(), FVector(1207, -200, 900),FRotator(0, 0, 0));//Barril(3)
}

void ADonkeyKong_L01Character::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f,-1.f,0.f), Value);
}

void ADonkeyKong_L01Character::TouchStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	// jump on any touch
	Jump();
}

void ADonkeyKong_L01Character::TouchStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}

void ADonkeyKong_L01Character::setRandomSpeed()
{
	//DEFINIR EL RANGO DE VELOCIDADES POSIBLES 
	float MinSpeed = 200.0f; //velocidad minima
	float MaxSpeed = 1500.0f; //velocidad maxima

	//generar una velocidad aleatoria dentro del rango
	float RandomSpeed = FMath::RandRange(MinSpeed, MaxSpeed);
	GetCharacterMovement()->MaxWalkSpeed = RandomSpeed;

	//Imprimir la velocidad para verificarla en el log
	//UE_LOG(LogTemp, Warning, TEXT("Nueva velocidad del personaje: %f"), RandomSpeed);

	GEngine->AddOnScreenDebugMessage(-1, 50.f, FColor::Cyan, FString::Printf(TEXT("La velocidad es: %f"), RandomSpeed));// ESTA LINEA DE CODIGO MUESTRA POR UN MOMENTO LA VELOCIDAD A LA QUE SE MOVERA EL PERSONAJE
}