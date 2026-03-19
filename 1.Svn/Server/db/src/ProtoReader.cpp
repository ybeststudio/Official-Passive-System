
// Ara

int32_t get_Item_Type_Value(const string &inputString)
{
	string arType[] =
	{
		....
		....
		....
		"ITEM_RING",			//33

// Altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
		"ITEM_PASSIVE"			//38
#endif


// Ara

	subtypes[ITEM_USE] =
	{
		......
	};

// Altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	subtypes[ITEM_PASSIVE] =
	{
		"PASSIVE_JOB"
	};
#endif


// Ara

	subtypes[ITEM_MATERIAL] =
	{
		"MATERIAL_LEATHER",
		"MATERIAL_BLOOD",
		"MATERIAL_ROOT",
		"MATERIAL_NEEDLE",
		"MATERIAL_JEWEL",
		"MATERIAL_DS_REFINE_NORMAL",
		"MATERIAL_DS_REFINE_BLESSED",
		"MATERIAL_DS_REFINE_HOLLY"

// Kodun sonrasına ekle		
		
#ifdef ENABLE_PASSIVE_SYSTEM
		,"MATERIAL_PASSIVE_WEAPON"
		,"MATERIAL_PASSIVE_ARMOR"
		,"MATERIAL_PASSIVE_ACCE"
		,"MATERIAL_PASSIVE_ELEMENT"
#endif
