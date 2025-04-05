// Fill out your copyright notice in the Description page of Project Settings.


#include "IVItemDatabase.h"

FItemBaseInfo* UIVItemDatabase::GetItemBaseInfo(const FName& ItemID)
{
	for (FItemBaseInfo& Item : Items)
	{
		if (Item.ItemID == ItemID)
		{
			return &Item;
		}
	}
	return nullptr;
}
