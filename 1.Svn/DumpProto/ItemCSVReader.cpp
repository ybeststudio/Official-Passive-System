
// üste bir yere ekleyin 

#define ENABLE_PASSIVE_SYSTEM


int get_Item_Type_Value(string inputString)
{
	string arType[] =
	{

// Kod bloğun en sonuna ekleyin

#ifdef ENABLE_PASSIVE_SYSTEM
		,"ITEM_PASSIVE"
#endif

// Ara

	ITEM_TYPE_MAX
};

// Değiştir

#ifdef ENABLE_PASSIVE_SYSTEM
	ITEM_PASSIVE		= 38,  // sıra numarası farklılık gösterebilir o yüzden kendinize göre düzenleyin.
#endif

	ITEM_TYPE_MAX
};


// Ara

	subtypes[ITEM_USE] =
	{
		"USE_POTION",
	....
	...
	};

// Kod bloğun hemen sonuna ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	subtypes[ITEM_PASSIVE] =
	{
		"PASSIVE_JOB"
	};
#endif


// Ara

		"MATERIAL_DS_REFINE_HOLLY"

// Hemen altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
		,"MATERIAL_PASSIVE_WEAPON"
		,"MATERIAL_PASSIVE_ARMOR"
		,"MATERIAL_PASSIVE_ACCE"
		,"MATERIAL_PASSIVE_ELEMENT"
#endif
