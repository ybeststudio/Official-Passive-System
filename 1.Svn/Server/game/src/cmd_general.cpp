
// Ara

extern int g_server_id;

extern int g_nPortalLimitTime;

// Altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
namespace
{
	enum EPassiveRelicSockets
	{
		PASSIVE_RELIC_SOCKET_REMAIN = ITEM_SOCKET_REMAIN_SEC,
		PASSIVE_RELIC_SOCKET_ACTIVE = 1,
		PASSIVE_RELIC_SOCKET_PROC = 2,
	};

	enum EPassiveRelicMaterialIndex
	{
		PASSIVE_RELIC_MATERIAL_WEAPON,
		PASSIVE_RELIC_MATERIAL_ELEMENT,
		PASSIVE_RELIC_MATERIAL_ARMOR,
		PASSIVE_RELIC_MATERIAL_ACCE,
		PASSIVE_RELIC_MATERIAL_MAX,
	};

	struct TPassiveRelicMaterialInfo
	{
		BYTE bSubType;
		DWORD dwVnum;
	};

	struct TPassiveRelicBonusInfo
	{
		BYTE bBonusType;
		BYTE bApply;
		BYTE bValueCount;
		short sValues[10];
	};

	enum EPassiveRelicBonusTypes
	{
		PASSIVE_RELIC_BONUS_DIRECT,
	};

	const DWORD PASSIVE_RELIC_VNUM_MIN = 30272;
	const DWORD PASSIVE_RELIC_VNUM_MAX = 30276;
	const char* PASSIVE_RELIC_COOLDOWN_FLAG = "passive_relic.cooldown";
	const char* PASSIVE_RELIC_DECK_SELECTED_FLAG = "passive_relic.deck.selected";
	const char* PASSIVE_RELIC_DECK_INIT_FLAG = "passive_relic.deck.init";
	const char* PASSIVE_RELIC_DECK_ATTR_TYPE_FLAG_FMT = "passive_relic.deck%d.attr.type%d";
	const char* PASSIVE_RELIC_DECK_ATTR_VALUE_FLAG_FMT = "passive_relic.deck%d.attr.value%d";
	const char* PASSIVE_RELIC_DECK_PROC_STONE_FLAG_FMT = "passive_relic.deck%d.proc.stone";
	const char* PASSIVE_RELIC_DECK_PROC_DISMOUNT_FLAG_FMT = "passive_relic.deck%d.proc.dismount";
	const BYTE PASSIVE_RELIC_MAX_BONUS_COUNT = 4;
	const int PASSIVE_RELIC_DIRECT_UNEQUIP_SUCCESS_CHANCE = 20;
	const int PASSIVE_RELIC_FALLBACK_DURATION = 60 * 60 * 24 * 7;

	enum EPassiveRelicDeck
	{
		PASSIVE_RELIC_DECK_EARTH = 0,
		PASSIVE_RELIC_DECK_SKY = 1,
		PASSIVE_RELIC_DECK_MAX = 2,
	};

	const TPassiveRelicMaterialInfo kPassiveRelicMaterials[PASSIVE_RELIC_MATERIAL_MAX] =
	{
		{ MATERIAL_PASSIVE_WEAPON, 30255 },
		{ MATERIAL_PASSIVE_ELEMENT, 30258 },
		{ MATERIAL_PASSIVE_ARMOR, 30256 },
		{ MATERIAL_PASSIVE_ACCE, 30257 },
	};

	// https://tr-wiki.metin2.gameforge.com/index.php/Kalynty_Sistemi
	// Bonus de?erleri resmi oyuna göre düzenledim de?i?tirmek isterseniz a?a?ydaki de?erleri de?i?tirin
	/*
	SungMa STR/RES/VIT/INT için sabit 15
	Metin savunma için 1,2,3,4,5,6,8,10
	Metin ta?yna kar?y güç için 1,2,3,4,5,6,7,8,10
	Patron drop ?ansy için 1,2,3,4,5,6,7,8,10
	Patronlara kar?y gücü için 1,2,3,4,5,6,7,8,10
	*/

	const TPassiveRelicBonusInfo kPassiveRelicBonusPool[] =
	{
		{ PASSIVE_RELIC_BONUS_DIRECT, APPLY_SUNGMA_STR, 1, { 15 } },
		{ PASSIVE_RELIC_BONUS_DIRECT, APPLY_SUNGMA_HP, 1, { 15 } },
		{ PASSIVE_RELIC_BONUS_DIRECT, APPLY_SUNGMA_MOVE, 1, { 15 } },
		{ PASSIVE_RELIC_BONUS_DIRECT, APPLY_SUNGMA_IMMUNE, 1, { 15 } },
		{ PASSIVE_RELIC_BONUS_DIRECT, APPLY_ATTBONUS_STONE, 9, { 1, 2, 3, 4, 5, 6, 7, 8, 10 } },
		{ PASSIVE_RELIC_BONUS_DIRECT, APPLY_ATTBONUS_BOSS, 9, { 1, 2, 3, 4, 5, 6, 7, 8, 10 } },
		{ PASSIVE_RELIC_BONUS_DIRECT, APPLY_ITEM_DROP_BONUS, 9, { 1, 2, 3, 4, 5, 6, 7, 8, 10 } },
	};

	LPITEM GetEquippedPassiveRelic(LPCHARACTER ch)
	{
		if (!ch)
			return NULL;

		return ch->GetWear(WEAR_PASSIVE);
	}

	bool IsPassiveRelicActive(LPITEM pkItem)
	{
		return pkItem && pkItem->GetSocket(PASSIVE_RELIC_SOCKET_ACTIVE) != 0;
	}
	int GetPassiveRelicDurationSeconds(LPITEM pkItem)
	{
		if (!pkItem)
			return 0;

		const int iDuration = pkItem->GetDuration();
		if (iDuration > 0)
			return iDuration;

		const int iRemain = pkItem->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN);
		if (iRemain > 0)
			return iRemain;

		return PASSIVE_RELIC_FALLBACK_DURATION;
	}

	long GetPassiveRelicProcSocket(LPITEM pkItem)
	{
		return pkItem ? pkItem->GetSocket(PASSIVE_RELIC_SOCKET_PROC) : 0;
	}

	short GetPassiveRelicStoneDefProcValue(LPITEM pkItem)
	{
		return static_cast<short>(GetPassiveRelicProcSocket(pkItem) & 0xFFFF);
	}

	short GetPassiveRelicDismountMoveProcValue(LPITEM pkItem)
	{
		return static_cast<short>((GetPassiveRelicProcSocket(pkItem) >> 16) & 0xFFFF);
	}

	void SetPassiveRelicStoneDefProcValue(LPITEM pkItem, short sValue)
	{
		if (!pkItem)
			return;

		const long lCurrent = GetPassiveRelicProcSocket(pkItem);
		const long lUpdated = (lCurrent & 0xFFFF0000L) | (static_cast<long>(sValue) & 0xFFFFL);
		pkItem->SetSocket(PASSIVE_RELIC_SOCKET_PROC, lUpdated);
	}

	void SetPassiveRelicDismountMoveProcValue(LPITEM pkItem, short sValue)
	{
		if (!pkItem)
			return;

		const long lCurrent = GetPassiveRelicProcSocket(pkItem);
		const long lUpdated = (lCurrent & 0x0000FFFFL) | ((static_cast<long>(sValue) & 0xFFFFL) << 16);
		pkItem->SetSocket(PASSIVE_RELIC_SOCKET_PROC, lUpdated);
	}

	int GetPassiveRelicBonusCount(LPITEM pkItem)
	{
		if (!pkItem)
			return 0;

		return pkItem->GetAttributeCount();
	}

	void RefreshPassiveRelicState(LPCHARACTER ch, LPITEM pkItem);

	int ClampPassiveRelicDeckIndex(int iDeck)
	{
		if (iDeck < PASSIVE_RELIC_DECK_EARTH || iDeck >= PASSIVE_RELIC_DECK_MAX)
			return PASSIVE_RELIC_DECK_EARTH;
		return iDeck;
	}

	std::string BuildPassiveRelicDeckAttrTypeFlag(int iDeck, int iAttrIndex)
	{
		char szFlag[64];
		snprintf(szFlag, sizeof(szFlag), PASSIVE_RELIC_DECK_ATTR_TYPE_FLAG_FMT, iDeck, iAttrIndex);
		return szFlag;
	}

	std::string BuildPassiveRelicDeckAttrValueFlag(int iDeck, int iAttrIndex)
	{
		char szFlag[64];
		snprintf(szFlag, sizeof(szFlag), PASSIVE_RELIC_DECK_ATTR_VALUE_FLAG_FMT, iDeck, iAttrIndex);
		return szFlag;
	}

	std::string BuildPassiveRelicDeckProcStoneFlag(int iDeck)
	{
		char szFlag[64];
		snprintf(szFlag, sizeof(szFlag), PASSIVE_RELIC_DECK_PROC_STONE_FLAG_FMT, iDeck);
		return szFlag;
	}

	std::string BuildPassiveRelicDeckProcDismountFlag(int iDeck)
	{
		char szFlag[64];
		snprintf(szFlag, sizeof(szFlag), PASSIVE_RELIC_DECK_PROC_DISMOUNT_FLAG_FMT, iDeck);
		return szFlag;
	}

	int GetPassiveRelicSelectedDeck(LPCHARACTER ch)
	{
		if (!ch)
			return PASSIVE_RELIC_DECK_EARTH;

		return ClampPassiveRelicDeckIndex(ch->GetQuestFlag(PASSIVE_RELIC_DECK_SELECTED_FLAG));
	}

	void SetPassiveRelicSelectedDeck(LPCHARACTER ch, int iDeck)
	{
		if (!ch)
			return;

		ch->SetQuestFlag(PASSIVE_RELIC_DECK_SELECTED_FLAG, ClampPassiveRelicDeckIndex(iDeck));
	}

	void SavePassiveRelicDeckState(LPCHARACTER ch, LPITEM pkRelic, int iDeck)
	{
		if (!ch || !pkRelic)
			return;

		iDeck = ClampPassiveRelicDeckIndex(iDeck);

		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			const TPlayerItemAttribute& rAttr = pkRelic->GetAttribute(i);
			ch->SetQuestFlag(BuildPassiveRelicDeckAttrTypeFlag(iDeck, i), rAttr.bType);
			ch->SetQuestFlag(BuildPassiveRelicDeckAttrValueFlag(iDeck, i), rAttr.sValue);
		}

		ch->SetQuestFlag(BuildPassiveRelicDeckProcStoneFlag(iDeck), GetPassiveRelicStoneDefProcValue(pkRelic));
		ch->SetQuestFlag(BuildPassiveRelicDeckProcDismountFlag(iDeck), GetPassiveRelicDismountMoveProcValue(pkRelic));
	}

	void ClearPassiveRelicBonuses(LPITEM pkRelic)
	{
		if (!pkRelic)
			return;

		while (pkRelic->GetAttributeCount() > 0)
		{
			if (!pkRelic->RemoveAttributeAt(pkRelic->GetAttributeCount() - 1))
				break;
		}

		SetPassiveRelicStoneDefProcValue(pkRelic, 0);
		SetPassiveRelicDismountMoveProcValue(pkRelic, 0);
	}

	void LoadPassiveRelicDeckState(LPCHARACTER ch, LPITEM pkRelic, int iDeck)
	{
		if (!ch || !pkRelic)
			return;

		iDeck = ClampPassiveRelicDeckIndex(iDeck);
		ClearPassiveRelicBonuses(pkRelic);

		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			const int iType = ch->GetQuestFlag(BuildPassiveRelicDeckAttrTypeFlag(iDeck, i));
			const int iValue = ch->GetQuestFlag(BuildPassiveRelicDeckAttrValueFlag(iDeck, i));
			if (iType <= 0 || iValue == 0)
				continue;

			pkRelic->AddAttribute(static_cast<BYTE>(iType), static_cast<short>(iValue));
		}

		SetPassiveRelicStoneDefProcValue(pkRelic, static_cast<short>(ch->GetQuestFlag(BuildPassiveRelicDeckProcStoneFlag(iDeck))));
		SetPassiveRelicDismountMoveProcValue(pkRelic, static_cast<short>(ch->GetQuestFlag(BuildPassiveRelicDeckProcDismountFlag(iDeck))));
	}

	void ResetPassiveRelicDeckState(LPCHARACTER ch, int iDeck)
	{
		if (!ch)
			return;

		iDeck = ClampPassiveRelicDeckIndex(iDeck);
		for (int i = 0; i < ITEM_ATTRIBUTE_MAX_NUM; ++i)
		{
			ch->SetQuestFlag(BuildPassiveRelicDeckAttrTypeFlag(iDeck, i), 0);
			ch->SetQuestFlag(BuildPassiveRelicDeckAttrValueFlag(iDeck, i), 0);
		}

		ch->SetQuestFlag(BuildPassiveRelicDeckProcStoneFlag(iDeck), 0);
		ch->SetQuestFlag(BuildPassiveRelicDeckProcDismountFlag(iDeck), 0);
	}

	void InitializePassiveRelicDeckState(LPCHARACTER ch, LPITEM pkRelic)
	{
		if (!ch || !pkRelic)
			return;

		if (ch->GetQuestFlag(PASSIVE_RELIC_DECK_INIT_FLAG) != 0)
			return;

		SavePassiveRelicDeckState(ch, pkRelic, PASSIVE_RELIC_DECK_EARTH);
		ResetPassiveRelicDeckState(ch, PASSIVE_RELIC_DECK_SKY);
		SetPassiveRelicSelectedDeck(ch, PASSIVE_RELIC_DECK_EARTH);
		ch->SetQuestFlag(PASSIVE_RELIC_DECK_INIT_FLAG, 1);
	}

	void SwitchPassiveRelicDeck(LPCHARACTER ch, LPITEM pkRelic, int iNextDeck)
	{
		if (!ch || !pkRelic)
			return;

		InitializePassiveRelicDeckState(ch, pkRelic);

		const int iCurrentDeck = GetPassiveRelicSelectedDeck(ch);
		iNextDeck = ClampPassiveRelicDeckIndex(iNextDeck);

		if (iCurrentDeck == iNextDeck)
			return;

		const bool bWasActive = pkRelic->IsEquipped() && IsPassiveRelicActive(pkRelic);
		if (bWasActive)
			pkRelic->ModifyPoints(false);

		SavePassiveRelicDeckState(ch, pkRelic, iCurrentDeck);
		LoadPassiveRelicDeckState(ch, pkRelic, iNextDeck);
		SetPassiveRelicSelectedDeck(ch, iNextDeck);

		if (bWasActive)
		{
			ch->RemoveAffect(AFFECT_PASSIVE_RELIC_STONE_DEF);
			ch->RemoveAffect(AFFECT_PASSIVE_RELIC_DISMOUNT_SPEED);
			pkRelic->ModifyPoints(true);
		}

		RefreshPassiveRelicState(ch, pkRelic);
	}

	bool CanUsePassiveRelicCommand(LPCHARACTER ch)
	{
		if (!ch || !ch->IsPC())
			return false;

		if (ch->IsDead() || ch->IsStun())
			return false;

		if (ch->GetExchange() || ch->GetMyShop() || ch->GetShopOwner() || ch->IsOpenSafebox() || ch->IsCubeOpen())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "You cannot use the relic right now.");
			return false;
		}

		if (ch->GetQuestFlag(PASSIVE_RELIC_COOLDOWN_FLAG) > get_global_time())
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Please wait a moment and try again.");
			return false;
		}

		return true;
	}

	void TouchPassiveRelicCooldown(LPCHARACTER ch)
	{
		if (ch)
			ch->SetQuestFlag(PASSIVE_RELIC_COOLDOWN_FLAG, get_global_time() + 1);
	}

	void RefreshPassiveRelicState(LPCHARACTER ch, LPITEM pkItem)
	{
		if (!ch || !pkItem)
			return;

		if (pkItem->IsEquipped())
		{
			ch->ComputeBattlePoints();
			ch->UpdatePacket();
		}

		pkItem->UpdatePacket();
	}

	void SetPassiveRelicActive(LPCHARACTER ch, LPITEM pkItem, bool bActive)
	{
		if (!ch || !pkItem)
			return;

		const bool bWasActive = IsPassiveRelicActive(pkItem);
		if (bWasActive == bActive)
			return;

		if (pkItem->IsEquipped() && bWasActive)
			pkItem->ModifyPoints(false);

		pkItem->SetSocket(PASSIVE_RELIC_SOCKET_ACTIVE, bActive ? 1 : 0);

		if (pkItem->IsEquipped() && bActive)
		{
			pkItem->ModifyPoints(true);
			ch->EffectPacket(SE_PASSIVE_EFFECT);
			ch->SpecificEffectPacket("d:/ymir work/effect/etc/buff/buff_passive_01.mse");
		}
		else if (!bActive)
		{
			ch->RemoveAffect(AFFECT_PASSIVE_RELIC_STONE_DEF);
			ch->RemoveAffect(AFFECT_PASSIVE_RELIC_DISMOUNT_SPEED);
		}

		RefreshPassiveRelicState(ch, pkItem);
	}

	bool ParsePassiveRelicMaterialCells(const char* argument, int aiCells[PASSIVE_RELIC_MATERIAL_MAX])
	{
		if (!argument)
			return false;

		for (int i = 0; i < PASSIVE_RELIC_MATERIAL_MAX; ++i)
		{
			char arg[256];
			argument = one_argument(argument, arg, sizeof(arg));
			if (!*arg)
				return false;

			str_to_number(aiCells[i], arg);
		}

		return true;
	}

	LPITEM GetPassiveRelicMaterialItem(LPCHARACTER ch, int iCell, const TPassiveRelicMaterialInfo& rInfo)
	{
		if (!ch)
			return NULL;

		if (iCell < 0 || iCell >= INVENTORY_AND_EQUIP_SLOT_MAX)
			return NULL;

		LPITEM pkItem = ch->GetInventoryItem(iCell);
		if (!pkItem)
			return NULL;

		if (pkItem->GetType() != ITEM_MATERIAL)
			return NULL;

		if (pkItem->GetSubType() != rInfo.bSubType)
			return NULL;

		if (pkItem->GetVnum() != rInfo.dwVnum)
			return NULL;

		if (pkItem->GetCount() <= 0)
			return NULL;

		return pkItem;
	}

	bool CollectPassiveRelicMaterialItems(LPCHARACTER ch, const int aiCells[PASSIVE_RELIC_MATERIAL_MAX], LPITEM apItems[PASSIVE_RELIC_MATERIAL_MAX])
	{
		for (int i = 0; i < PASSIVE_RELIC_MATERIAL_MAX; ++i)
		{
			for (int j = i + 1; j < PASSIVE_RELIC_MATERIAL_MAX; ++j)
			{
				if (aiCells[i] == aiCells[j])
					return false;
			}

			apItems[i] = GetPassiveRelicMaterialItem(ch, aiCells[i], kPassiveRelicMaterials[i]);
			if (!apItems[i])
				return false;
		}

		return true;
	}

	void ConsumePassiveRelicMaterialItems(LPITEM apItems[PASSIVE_RELIC_MATERIAL_MAX])
	{
		for (int i = 0; i < PASSIVE_RELIC_MATERIAL_MAX; ++i)
		{
			if (!apItems[i])
				continue;

			if (apItems[i]->GetCount() > 1)
				apItems[i]->SetCount(apItems[i]->GetCount() - 1);
			else
				apItems[i]->SetCount(0);
		}
	}

	bool AddRandomPassiveRelicBonus(LPITEM pkRelic)
	{
		if (!pkRelic)
			return false;

		std::vector<int> vecAvailableIndexes;
		for (int i = 0; i < static_cast<int>(sizeof(kPassiveRelicBonusPool) / sizeof(kPassiveRelicBonusPool[0])); ++i)
		{
			const TPassiveRelicBonusInfo& rBonusInfo = kPassiveRelicBonusPool[i];
			switch (rBonusInfo.bBonusType)
			{
				case PASSIVE_RELIC_BONUS_DIRECT:
					if (!pkRelic->HasAttr(rBonusInfo.bApply))
						vecAvailableIndexes.push_back(i);
					break;
			}
		}

		if (vecAvailableIndexes.empty())
			return false;

		const int iSelectedIndex = vecAvailableIndexes[number(0, vecAvailableIndexes.size() - 1)];
		const TPassiveRelicBonusInfo& rBonusInfo = kPassiveRelicBonusPool[iSelectedIndex];
		const short sValue = rBonusInfo.sValues[number(0, rBonusInfo.bValueCount - 1)];
		switch (rBonusInfo.bBonusType)
		{
			case PASSIVE_RELIC_BONUS_DIRECT:
				pkRelic->AddAttribute(rBonusInfo.bApply, sValue);
				return true;
		}

		return false;
	}
}

ACMD(do_passive_relic)
{
	char arg1[256];
	argument = one_argument(argument, arg1, sizeof(arg1));

	if (!*arg1)
		return;

	const bool bIsUnequipCommand = !str_cmp(arg1, "unequip");
	if (!bIsUnequipCommand && !CanUsePassiveRelicCommand(ch))
		return;

	LPITEM pkRelic = GetEquippedPassiveRelic(ch);
	if (!pkRelic)
	{
		ch->ChatPacket(CHAT_TYPE_INFO, "Equip your passive relic first.");
		return;
	}

	InitializePassiveRelicDeckState(ch, pkRelic);

	if (!str_cmp(arg1, "earth") || !str_cmp(arg1, "sky"))
	{
		const int iNextDeck = !str_cmp(arg1, "sky") ? PASSIVE_RELIC_DECK_SKY : PASSIVE_RELIC_DECK_EARTH;
		SwitchPassiveRelicDeck(ch, pkRelic, iNextDeck);
		TouchPassiveRelicCooldown(ch);
		ch->ChatPacket(CHAT_TYPE_INFO, iNextDeck == PASSIVE_RELIC_DECK_SKY ? "Sky relic deck selected." : "Earth relic deck selected.");
		return;
	}

	if (!str_cmp(arg1, "extract"))
	{
		char argCell[256];
		one_argument(argument, argCell, sizeof(argCell));
		if (!*argCell)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Place a valid pincer item on the relic.");
			return;
		}

		int iExtractCell = -1;
		str_to_number(iExtractCell, argCell);
		if (iExtractCell < 0 || iExtractCell >= INVENTORY_AND_EQUIP_SLOT_MAX)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Invalid extraction item slot.");
			return;
		}

		LPITEM pkExtractItem = ch->GetInventoryItem(iExtractCell);
		if (!pkExtractItem || (pkExtractItem->GetVnum() != 100100 && pkExtractItem->GetVnum() != 100101))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "You need Dragon Pincer or Dragon Pincer+.");
			return;
		}

		const int iSuccessChance = (pkExtractItem->GetVnum() == 100101) ? 100 : 30;
		const int iEmptyPos = ch->GetEmptyInventory(pkRelic->GetSize());
		if (iEmptyPos < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "You need an empty inventory slot to extract the relic.");
			return;
		}

		pkExtractItem->SetCount(pkExtractItem->GetCount() - 1);

		if (IsPassiveRelicActive(pkRelic))
			SetPassiveRelicActive(ch, pkRelic, false);

		if (number(1, 100) > iSuccessChance)
		{
			SavePassiveRelicDeckState(ch, pkRelic, GetPassiveRelicSelectedDeck(ch));
			pkRelic->RemoveFromCharacter();
			pkRelic->SetCount(0);
			ch->ChatPacket(CHAT_TYPE_INFO, "Relic extraction failed. The relic was destroyed.");
			TouchPassiveRelicCooldown(ch);
			return;
		}

		SavePassiveRelicDeckState(ch, pkRelic, GetPassiveRelicSelectedDeck(ch));
		pkRelic->RemoveFromCharacter();
		pkRelic->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
		ch->ChatPacket(CHAT_TYPE_INFO, "Relic extraction succeeded.");
		TouchPassiveRelicCooldown(ch);
		return;
	}

	if (!str_cmp(arg1, "unequip"))
	{
		if (IsPassiveRelicActive(pkRelic))
			SetPassiveRelicActive(ch, pkRelic, false);

		if (number(1, 100) > PASSIVE_RELIC_DIRECT_UNEQUIP_SUCCESS_CHANCE)
		{
			SavePassiveRelicDeckState(ch, pkRelic, GetPassiveRelicSelectedDeck(ch));
			pkRelic->RemoveFromCharacter();
			pkRelic->SetCount(0);
			ch->ChatPacket(CHAT_TYPE_INFO, "Direct relic extraction failed. The relic was destroyed.");
			return;
		}

		int iEmptyPos = ch->GetEmptyInventory(pkRelic->GetSize());
		if (iEmptyPos < 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "You need an empty inventory slot to unequip the relic.");
			return;
		}

		SavePassiveRelicDeckState(ch, pkRelic, GetPassiveRelicSelectedDeck(ch));
		pkRelic->RemoveFromCharacter();
		pkRelic->AddToCharacter(ch, TItemPos(INVENTORY, iEmptyPos));
		ch->ChatPacket(CHAT_TYPE_INFO, "The relic has been removed from the slot.");
		return;
	}

	if (!str_cmp(arg1, "charge"))
	{
		int aiCells[PASSIVE_RELIC_MATERIAL_MAX];
		LPITEM apItems[PASSIVE_RELIC_MATERIAL_MAX] = {};
		if (!ParsePassiveRelicMaterialCells(argument, aiCells) || !CollectPassiveRelicMaterialItems(ch, aiCells, apItems))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Place the four spirit items into the relic slots first.");
			return;
		}

		const int iDuration = GetPassiveRelicDurationSeconds(pkRelic);
		if (iDuration <= 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "This relic cannot be charged.");
			return;
		}

		if (pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN) >= iDuration)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "The relic is already fully charged.");
			return;
		}

		TouchPassiveRelicCooldown(ch);

		if (pkRelic->IsEquipped() && -1 != pkRelic->GetProto()->cLimitTimerBasedOnWearIndex)
			pkRelic->StopTimerBasedOnWearExpireEvent();

		pkRelic->SetSocket(PASSIVE_RELIC_SOCKET_REMAIN, iDuration);

		if (pkRelic->IsEquipped() && -1 != pkRelic->GetProto()->cLimitTimerBasedOnWearIndex)
			pkRelic->StartTimerBasedOnWearExpireEvent();

		ConsumePassiveRelicMaterialItems(apItems);
		RefreshPassiveRelicState(ch, pkRelic);
		ch->ChatPacket(CHAT_TYPE_INFO, "The relic time has been restored.");
		return;
	}

	if (!str_cmp(arg1, "add"))
	{
		int aiCells[PASSIVE_RELIC_MATERIAL_MAX];
		LPITEM apItems[PASSIVE_RELIC_MATERIAL_MAX] = {};
		if (!ParsePassiveRelicMaterialCells(argument, aiCells) || !CollectPassiveRelicMaterialItems(ch, aiCells, apItems))
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Place the four spirit items into the relic slots first.");
			return;
		}

		if (GetPassiveRelicBonusCount(pkRelic) >= PASSIVE_RELIC_MAX_BONUS_COUNT)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "The relic cannot receive more bonuses.");
			return;
		}

		const bool bWasActive = pkRelic->IsEquipped() && IsPassiveRelicActive(pkRelic);
		if (bWasActive)
			pkRelic->ModifyPoints(false);

		const bool bAdded = AddRandomPassiveRelicBonus(pkRelic);

		if (bWasActive)
			pkRelic->ModifyPoints(true);

		if (!bAdded)
		{
			RefreshPassiveRelicState(ch, pkRelic);
			ch->ChatPacket(CHAT_TYPE_INFO, "The relic already has every available bonus.");
			return;
		}

		TouchPassiveRelicCooldown(ch);
		ConsumePassiveRelicMaterialItems(apItems);
		RefreshPassiveRelicState(ch, pkRelic);
		ch->ChatPacket(CHAT_TYPE_INFO, "A random relic bonus has been added.");
		return;
	}

	if (!str_cmp(arg1, "activate"))
	{
		if (GetPassiveRelicBonusCount(pkRelic) <= 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Add a relic bonus first.");
			return;
		}

		const int iDuration = GetPassiveRelicDurationSeconds(pkRelic);
		if (iDuration > 0 && pkRelic->GetSocket(PASSIVE_RELIC_SOCKET_REMAIN) <= 0)
		{
			ch->ChatPacket(CHAT_TYPE_INFO, "Charge the relic before activation.");
			return;
		}

		TouchPassiveRelicCooldown(ch);

		const bool bNextState = !IsPassiveRelicActive(pkRelic);
		SetPassiveRelicActive(ch, pkRelic, bNextState);
		ch->ChatPacket(CHAT_TYPE_INFO, bNextState ? "The relic is now active." : "The relic is now inactive.");
		return;
	}
}
#endif
