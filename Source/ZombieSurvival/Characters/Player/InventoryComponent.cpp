#include "InventoryComponent.h"

#include "GameFramework/Character.h"
#include "Interfaces/InventoryItemInterface.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInventoryComponent::AddItem(FName ItemID, AActor* Item)
{
	if (Item)
	{
		if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
		{
			IInventoryItemInterface::Execute_Pickup(Item, Character);
		}
		
		Items.Add(ItemID, Item);
	}
}

void UInventoryComponent::RemoveItem(FName ItemID)
{
	if (AActor* Item = GetItem(ItemID))
	{
		IInventoryItemInterface::Execute_Drop(Item);
	}

	Items.Remove(ItemID);

}

AActor* UInventoryComponent::GetItem(const FName ItemID) const
{
	if (Items.Contains(ItemID))
	{
		return *Items.Find(ItemID);
	}
	return nullptr;
}

TMap<FName, AActor*> UInventoryComponent::GetItems() const
{
	return Items;
}