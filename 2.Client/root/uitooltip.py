
## Ara

		elif item.ITEM_TYPE_PET == itemType and app.ENABLE_PET_SYSTEM:
			bHasRealtimeFlag = 0
			for i in xrange(item.LIMIT_MAX_NUM):
				(limitType, limitValue) = item.GetLimit(i)

				if item.LIMIT_REAL_TIME == limitType:
					bHasRealtimeFlag = 1

			if 1 == bHasRealtimeFlag:
				if item.LIMIT_REAL_TIME == item.GetLimitType(0) or item.LIMIT_REAL_TIME_START_FIRST_USE == item.GetLimitType(0):
					self.AppendMallItemLastTime(metinSlot[0], item.GetLimitValue(0))
				else:
					self.AppendMallItemLastTime(metinSlot[0], item.GetLimitValue(1))

			self.AppendSpace(5)
			for g in xrange(item.ITEM_APPLY_MAX_NUM):
				(affectType, affectValue) = item.GetAffect(g)
				affectString = self.__GetAffectString(affectType, affectValue)
				if affectString:
					affectColor = self.GetChangeTextLineColor(affectValue)
					self.AppendTextLine(affectString, affectColor)

		# END_ITEM_TYPE_PET

# Altına ekle

		elif app.ENABLE_PASSIVE_SYSTEM and item.ITEM_TYPE_PASSIVE == itemType:
			self.__AppendLimitInformation()
			self.__AppendAffectInformation()
			self.__AppendAttributeInformation(attrSlot)
			self.AppendWearableInformation()
