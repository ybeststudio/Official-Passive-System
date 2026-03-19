
// Ara enum EItemTypes

enum EItemTypes
{
	....
	....
	....

	ITEM_TYPE_MAX
};

//  ITEM_TYPE_MAX üstüne ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	ITEM_PASSIVE,		// 38 (sayı sizin sıra numarasınıza göre değişebilir
#endif

// Ara 

enum EUseSubTypes
{
....
...
};

// Altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
enum EPassiveSubTypes
{
	PASSIVE_JOB,
};
#endif


// Ara MATERIAL_DS_REFINE_HOLLY, kodun altına ekle

enum EMaterialSubTypes
{
....
....
	MATERIAL_DS_REFINE_HOLLY,
	
/// kodun sonuna ekle
	
#ifdef ENABLE_PASSIVE_SYSTEM
	MATERIAL_PASSIVE_WEAPON,
	MATERIAL_PASSIVE_ARMOR,
	MATERIAL_PASSIVE_ACCE,
	MATERIAL_PASSIVE_ELEMENT,
#endif
