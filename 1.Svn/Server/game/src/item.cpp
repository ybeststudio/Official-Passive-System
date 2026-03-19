
// Ara

int CItem::FindEquipCell(LPCHARACTER ch, int iCandidateCell)
{
	// Costume items (ITEM_COSTUME) do not need WearFlag. (Dividing the wear position by sub type. Do I need to give a wear flag again..)
	// Dragon Soul Stones (ITEM_DS, ITEM_SPECIAL_DS) are also classified by SUB_TYPE. New rings and belts are classified by ITEM_TYPE -_-
	if ((0 == GetWearFlag() || ITEM_TOTEM == GetType()) && ITEM_COSTUME != GetType() && ITEM_DS != GetType() && ITEM_SPECIAL_DS != GetType() && ITEM_RING != GetType()
#ifdef ENABLE_MOUNT_SYSTEM
		&& ITEM_MOUNT != GetType()
#endif
#ifdef ENABLE_PET_SYSTEM
		&& ITEM_PET != GetType()
#endif

// Alta ekle

#ifdef ENABLE_PASSIVE_SYSTEM
		&& ITEM_PASSIVE != GetType()
#endif



// Ara

	else if (GetType() == ITEM_RING)
	{
		if (ch->GetWear(WEAR_RING1))
			return WEAR_RING2;
		else
			return WEAR_RING1;
	}
#ifdef ENABLE_MOUNT_SYSTEM
	else if (GetType() == ITEM_MOUNT)
		return WEAR_MOUNT;
#endif
#ifdef ENABLE_PET_SYSTEM
	else if (GetType() == ITEM_PET)
		return WEAR_PET;
#endif

// Hemen altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	else if (GetType() == ITEM_PASSIVE)
		return WEAR_PASSIVE;
#endif

// Ara bloğun içinde

void CItem::ModifyPoints(bool bAdd)
{

		accessoryGrade = 0;
	}
	else
	{
		accessoryGrade = MIN(GetAccessorySocketGrade(), ITEM_ACCESSORY_SOCKET_MAX_NUM);
	}

// Hemen altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	if (CItemVnumHelper::IsPassive(GetVnum()) && GetSubType() == PASSIVE_JOB && GetSocket(1) == 0)
		return;
#endif


// Ara

bool CItem::IsEquipable() const
{
	switch (this->GetType())
	{
		case ITEM_COSTUME:
		case ITEM_ARMOR:
		case ITEM_WEAPON:
		case ITEM_ROD:
		case ITEM_PICK:
		case ITEM_UNIQUE:
		case ITEM_DS:
		case ITEM_SPECIAL_DS:
		case ITEM_RING:
#ifdef ENABLE_MOUNT_SYSTEM
		case ITEM_MOUNT:
#endif
#ifdef ENABLE_PET_SYSTEM
		case ITEM_PET:
#endif

// Altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
		case ITEM_PASSIVE:
#endif


// Ara

bool CItem::Unequip()
{

	if (IsRideItem())
		ClearMountAttributeAndAffect();

// Altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
	if (CItemVnumHelper::IsPassive(GetVnum()) && GetSubType() == PASSIVE_JOB)
	{
		m_pOwner->RemoveAffect(AFFECT_PASSIVE_RELIC_STONE_DEF);
		m_pOwner->RemoveAffect(AFFECT_PASSIVE_RELIC_DISMOUNT_SPEED);
	}
#endif
