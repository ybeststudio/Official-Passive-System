
// Ara

struct command_info cmd_info[] =

// Ara bloğun hemen üstüne ekle

#ifdef ENABLE_PASSIVE_SYSTEM
ACMD(do_passive_relic);
#endif


// Ara

	{ "\n",		NULL,		0,		POS_DEAD,		GM_IMPLEMENTOR	}
};

// Ara bloğun hemen üstüne ekle

#ifdef ENABLE_AFFECT_BUFF_REMOVE
	{	"remove_buff",				do_remove_buff,						0,						POS_DEAD,			GM_PLAYER			},
#endif
