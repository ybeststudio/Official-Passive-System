
/// ara

bool CHARACTER::StartRiding()
{
	if (IsDead() == true)
	{
		LocaleChatPacket(CHAT_TYPE_INFO, 93, "");
		return false;
	}

	HorseSummon(false);

	MountVnum(dwMountVnum);

// Altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	RemoveAffect(AFFECT_PASSIVE_RELIC_DISMOUNT_SPEED);
#endif

