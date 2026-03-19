


// Ara  	( PyObject *poModule = Py_InitModule("item", s_methods); ) içerisinde

	PyModule_AddIntConstant(poModule, "EQUIPMENT_SHIELD", c_Equipment_Shield);

// Hemen altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	PyModule_AddIntConstant(poModule, "EQUIPMENT_PASSIVE",		c_Equipment_Passive);
#endif


// Ara

	PyModule_AddIntConstant(poModule, "ITEM_TYPE_RING", CItemData::ITEM_TYPE_RING);

// ALtına ekle
#ifdef ENABLE_PASSIVE_SYSTEM
	PyModule_AddIntConstant(poModule, "ITEM_TYPE_PASSIVE",		CItemData::ITEM_TYPE_PASSIVE);
#endif

// Ara

	PyModule_AddIntConstant(poModule, "USE_TIME_CHARGE_FIX", CItemData::USE_TIME_CHARGE_FIX);

// Altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	PyModule_AddIntConstant(poModule, "PASSIVE_JOB", CItemData::PASSIVE_JOB);
#endif

// Ara

	PyModule_AddIntConstant(poModule, "MATERIAL_DS_REFINE_HOLLY", CItemData::MATERIAL_DS_REFINE_HOLLY);
	
// Altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	PyModule_AddIntConstant(poModule, "MATERIAL_PASSIVE_WEAPON", CItemData::MATERIAL_PASSIVE_WEAPON);
	PyModule_AddIntConstant(poModule, "MATERIAL_PASSIVE_ARMOR", CItemData::MATERIAL_PASSIVE_ARMOR);
	PyModule_AddIntConstant(poModule, "MATERIAL_PASSIVE_ACCE", CItemData::MATERIAL_PASSIVE_ACCE);
	PyModule_AddIntConstant(poModule, "MATERIAL_PASSIVE_ELEMENT", CItemData::MATERIAL_PASSIVE_ELEMENT);
#endif
