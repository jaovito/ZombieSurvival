#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ZOMBIESURVIVAL_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category="Inventory")
	void AddItem(FName ItemID, AActor* Item);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	void RemoveItem(FName ItemID);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	AActor* GetItem(FName ItemID) const;

	UFUNCTION(BlueprintCallable, Category="Inventory")
	TMap<FName, AActor*> GetItems() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	UUserWidget* InventoryWidget;

private:
	UPROPERTY()
	TMap<FName, AActor*> Items;
};