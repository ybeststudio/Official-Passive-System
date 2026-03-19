
// Ara

#ifdef ENABLE_ENERGY_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_ENERGY_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_ENERGY_SYSTEM", 0);
#endif


// Hemen altına ekle


#ifdef ENABLE_PASSIVE_SYSTEM
	PyModule_AddIntConstant(poModule, "ENABLE_PASSIVE_SYSTEM", 1);
#else
	PyModule_AddIntConstant(poModule, "ENABLE_PASSIVE_SYSTEM", 0);
#endif
