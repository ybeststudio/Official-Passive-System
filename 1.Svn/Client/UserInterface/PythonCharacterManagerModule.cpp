
// Ara 	 ( PyObject *poModule = Py_InitModule("chrmgr", s_methods); ) içerisinde


	PyModule_AddIntConstant(poModule, "EFFECT_LOVE_PENDANT_EQUIP", CInstanceBase::EFFECT_LOVE_PENDANT_EQUIP);

// Hemen altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	PyModule_AddIntConstant(poModule, "EFFECT_PASSIVE",		CInstanceBase::EFFECT_PASSIVE);
#endif
