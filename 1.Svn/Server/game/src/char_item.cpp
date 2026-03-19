
// Ara

		case ITEM_PICK:

// Hemen altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
		case ITEM_PASSIVE:
#endif



// Ara

bool CHARACTER::UseItem(TItemPos Cell, TItemPos DestCell)
{


	if (IS_SET(item->GetFlag(), ITEM_FLAG_LOG))
	{

// Kod bloğun hemen üstüne ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	if (item->GetVnum() == 100100 || item->GetVnum() == 100101)
	{
		LPITEM pkRelic = GetWear(WEAR_PASSIVE);
		if (!pkRelic)
		{
			ChatPacket(CHAT_TYPE_INFO, "Equip a relic first.");
			return false;
		}

		const int iSuccessChance = (item->GetVnum() == 100101) ? 100 : 30;
		const int iEmptyPos = GetEmptyInventory(pkRelic->GetSize());
		if (iEmptyPos < 0)
		{
			LocaleChatPacket(CHAT_TYPE_INFO, 191, "");
			return false;
		}

		if (item->GetCount() > 0)
			item->SetCount(item->GetCount() - 1);

		if (pkRelic->GetSocket(1) != 0)
		{
			pkRelic->ModifyPoints(false);
			pkRelic->SetSocket(1, 0);
			RemoveAffect(AFFECT_PASSIVE_RELIC_STONE_DEF);
			RemoveAffect(AFFECT_PASSIVE_RELIC_DISMOUNT_SPEED);
			ComputeBattlePoints();
			UpdatePacket();
		}

		if (number(1, 100) > iSuccessChance)
		{
			pkRelic->RemoveFromCharacter();
			pkRelic->SetCount(0);
			ChatPacket(CHAT_TYPE_INFO, "Relic extraction failed. The relic was destroyed.");
			return true;
		}

		pkRelic->RemoveFromCharacter();
		pkRelic->AddToCharacter(this, TItemPos(INVENTORY, iEmptyPos));
		ChatPacket(CHAT_TYPE_INFO, "Relic extraction succeeded.");
		return true;
	}
#endif



// Ara 

		else if (true == CItemVnumHelper::IsLovePendant(dwVnum))
		{
			this->EffectPacket(SE_EQUIP_LOVE_PENDANT);
		}

// Kod bloğun hemen altına ekle
#ifdef ENABLE_PASSIVE_SYSTEM
		else if (true == CItemVnumHelper::IsPassive(dwVnum))
		{
			this->EffectPacket(SE_PASSIVE_EFFECT);
			this->SpecificEffectPacket("d:/ymir work/effect/etc/buff/buff_passive_01.mse");
		}
#endif
