
# Ara kod bloğun içinde

	def UseItemSlot(self, slotIndex):
        .....
        ....
        
        ......

		slotIndex = self.__InventoryLocalSlotPosToGlobalSlotPos(slotIndex)

# Hemen altına ekle

		if app.ENABLE_PASSIVE_SYSTEM:
			if self.interface and hasattr(self.interface, "wndCharacter") and self.interface.wndCharacter:
				itemVnum = player.GetItemIndex(slotIndex)
				if itemVnum in (30255, 30256, 30257, 30258):
					if self.interface.wndCharacter.TryAttachPassiveMaterialFromInventory(slotIndex):
						mouseModule.mouseController.DeattachObject()
						self.OverOutItem()
						return
