// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "TouchPointer.generated.h"


class UTouchPointer;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTouchPointerBeginPinchDelegate, UTouchPointer*, Pointer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTouchPointerEndPinchDelegate, UTouchPointer*, Pointer);

/**
 * Turns an actor into a touch pointer.
 * Touch pointers confer focus to interactables (e.g. buttons) when they overlap with them. 
 * Interactables use the transform of pointers focusing them to drive their interactions.
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MIXEDREALITYUTILS_API UTouchPointer : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTouchPointer();

	UFUNCTION(BlueprintSetter)
	void SetTouchRadius(float radius);

	UFUNCTION(BlueprintGetter)
	float GetTouchRadius() const;

	/** 
	 * Returns the target that is closest to the pointer position or null if there is none. 
	 * The closest point on the target surface is stored in OutPointOnTargetSurface.
	 */
	UFUNCTION(BlueprintCallable)
	UActorComponent* GetClosestPointOnTargets(FVector& OutPointOnTargetSurface) const;

	/// Returns all active pointers.
	UFUNCTION(BlueprintCallable)
	static const TArray<UTouchPointer*>& GetAllPointers();

	UFUNCTION(BlueprintGetter)
	bool GetPinched() const;
	UFUNCTION(BlueprintSetter)
	void SetPinched(bool Enable);

	/** Event raised when the pointer pinch starts. */
	UPROPERTY(BlueprintAssignable)
	FTouchPointerBeginPinchDelegate OnBeginPinch;

	/** Event raised when the pointer pinch ends. */
	UPROPERTY(BlueprintAssignable)
	FTouchPointerEndPinchDelegate OnEndPinch;

protected:

	/// Start touching the component.
	/// Returns false if the component is not a valid touch target.
	bool TryStartTouching(USceneComponent *comp);

	/// Stop touching the component.
	/// Returns false if the component was not touched.
	bool TryStopTouching(USceneComponent *comp);

	/// Stop touching all current components.
	void StopAllTouching();

	UFUNCTION()
	void OnPointerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnPointerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	bool ImplementsTargetInterface(const UObject *obj) const;

protected:

	TSet<UActorComponent*> TouchedTargets;

private:

	UPROPERTY(BlueprintGetter = "GetPinched", BlueprintSetter = "SetPinched")
	bool bIsPinched;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent *TouchSphere;

	UPROPERTY(EditAnywhere, BlueprintSetter = "SetTouchRadius", BlueprintGetter = "GetTouchRadius")
	float TouchRadius;

	/** List with all active pointers. */
	static TArray<UTouchPointer*> Pointers;
};