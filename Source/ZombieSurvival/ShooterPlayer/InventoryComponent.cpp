#include "InventoryComponent.h"

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
		Items.Add(ItemID, Item);
	}
}

void UInventoryComponent::RemoveItem(FName ItemID)
{
	Items.Remove(ItemID);
}

AActor* UInventoryComponent::GetItem(FName ItemID) const
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