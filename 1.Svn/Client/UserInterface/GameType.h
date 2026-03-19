

// Ara (sizde farklı olabilir)

#ifdef ENABLE_NEW_EQUIPMENT_SYSTEM
	const DWORD c_New_Equipment_Start = c_Equipment_Start + CItemData::WEAR_RING1;
	const DWORD c_New_Equipment_Count = CItemData::WEAR_MAX_NUM - CItemData::WEAR_RING1;

	const DWORD c_Equipment_Ring1 = c_Equipment_Start + CItemData::WEAR_RING1;
	const DWORD c_Equipment_Ring2 = c_Equipment_Start + CItemData::WEAR_RING2;
	const DWORD c_Equipment_Belt = c_Equipment_Start + CItemData::WEAR_BELT;

// Hemen altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	const DWORD c_Equipment_Passive = c_Equipment_Start + CItemData::WEAR_PASSIVE;
#endif
