
// Ara enum EItemType içerisinde

			ITEM_TYPE_MAX_NUM,
		};

// Değiştir

#ifdef ENABLE_PASSIVE_SYSTEM
			ITEM_TYPE_PASSIVE,				// 38 passive
#endif
			ITEM_TYPE_MAX_NUM,
		};


// Ara

			MATERIAL_DS_REFINE_HOLLY,
			
// Hemen altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
			MATERIAL_PASSIVE_WEAPON,
			MATERIAL_PASSIVE_ARMOR,
			MATERIAL_PASSIVE_ACCE,
			MATERIAL_PASSIVE_ELEMENT,
#endif


// Ara

		enum EUseSubTypes
		{
		...
		...
		...
		};

// Bloktan sonrasına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
		enum EPassiveSubTypes
		{
			PASSIVE_JOB,
		};
#endif

// Ara EWearPositions içerisinde


			WEAR_MAX_NUM = 64,
		};


// Değiştir

#ifdef ENABLE_PASSIVE_SYSTEM
			WEAR_PASSIVE,
#endif
			WEAR_MAX_NUM = 64,
		};
