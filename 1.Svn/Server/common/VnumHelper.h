// Ara

class CItemVnumHelper
{
	public:
		static const bool IsPhoenix(DWORD vnum)				{ return 53001 == vnum; }
		static const bool IsRamadanMoonRing(DWORD vnum)		{ return 71135 == vnum; }
		static const bool IsHalloweenCandy(DWORD vnum)		{ return 71136 == vnum; }
		static const bool IsHappinessRing(DWORD vnum)		{ return 71143 == vnum; }
		static const bool IsLovePendant(DWORD vnum)			{ return 71145 == vnum; }

// Altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
		static	const bool	IsPassive(DWORD vnum)			{ return 30272 <= vnum && vnum <= 30276; }
#endif
