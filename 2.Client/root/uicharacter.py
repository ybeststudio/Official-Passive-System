
# Ara

	STAT_MINUS_DESCRIPTION = localeInfo.STAT_MINUS_DESCRIPTION

# Hemen altına ekle

	if app.ENABLE_PASSIVE_SYSTEM:
		wndPassive = None


# Ara
		self.activeSkillGroupName = self.GetChild("Active_Skill_Group_Name")

# Hemen altına ekle

		if app.ENABLE_PASSIVE_SYSTEM:
			self.passiveexpandedbtn = self.GetChild("passive_expanded_btn")
			self.passiveexpandedbtn.SetEvent(ui.__mem_func__(self.__ShowPassiveButton))

# Ara

	def Destroy(self):
		self.ClearDictionary()

# Hemen altına ekle

		if app.ENABLE_PASSIVE_SYSTEM:
			if self.wndPassive:
				self.wndPassive.Destroy()
				self.wndPassive = 0

# Ara

	def __ShowAlignmentToolTip(self):
		self.toolTipAlignment.ShowToolTip()

# Hemen altına ekle

	if app.ENABLE_PASSIVE_SYSTEM:
		def __ShowPassiveButton(self):
			print "Click Passive Button"
			if self.wndPassive:
				if self.wndPassive.IsShow(): 
					self.wndPassive.Hide()
				else:
					self.wndPassive.Show()
			else:
				self.wndPassive = PassiveWindow(self)
				self.wndPassive.Show()

		def TryAttachPassiveMaterialFromInventory(self, inventorySlot):
			if not self.wndPassive or not self.wndPassive.IsShow():
				return FALSE

			return self.wndPassive.TryAttachMaterialFromInventory(inventorySlot)




# En alta müsait bir yere ekleyin
if app.ENABLE_PASSIVE_SYSTEM:
	class PassiveWindow(ui.ScriptWindow):
	
		def __init__(self, wndInventory):
			ui.ScriptWindow.__init__(self)
	
			self.isLoaded = 0
			self.wndInventory = wndInventory
			self.wndPassive = None
			self.wndPassiveMaterial = None
			self.wndPassiveJobDownSlot = None
			self.chargeButton = None
			self.addButton = None
			self.activateButton = None
			self.deckButton1 = None
			self.deckButton2 = None
			self.selectedDeckIndex = 0
			self.toolTipItem = uiToolTip.ItemToolTip()
			self.toolTipItem.SetFollow(TRUE)
			self.toolTipItem.HideToolTip()
			self.questionDialog = None
			self.materialSlotDataByDeck = {
				0 : self.__CreateDefaultMaterialSlotData(),
				1 : self.__CreateDefaultMaterialSlotData(),
			}
			self.materialSlotData = self.materialSlotDataByDeck[self.selectedDeckIndex]
	
			self.__LoadWindow()
	
		def __del__(self):
			ui.ScriptWindow.__del__(self)
	
		def Show(self):
			self.__LoadWindow()
			self.RefreshEquipSlotWindow()
			self.SetCenterPosition()
			self.SetTop()
	
			ui.ScriptWindow.Show(self)
	
		def Close(self):
			if self.toolTipItem:
				self.toolTipItem.HideToolTip()
			self.__CloseQuestionDialog()
			self.Hide()
	
		def __LoadWindow(self):
			if self.isLoaded == 1:
				return
	
			self.isLoaded = 1
	
			try:
				pyScrLoader = ui.PythonScriptLoader()
				pyScrLoader.LoadScriptFile(self, "uiscript/passiveattr.py")
			except:
				import exception
				exception.Abort("PassiveWindow.LoadWindow.LoadObject")
	
			try:
				wndPassive = self.GetChild("passive_attr_sub_slot")
				wndPassiveMaterial = self.GetChild("passive_material_slot")
				wndPassiveJobDownSlot = self.GetChild("passive_job_down_slot")
				chargeButton = self.GetChild("ChargeButton")
				addButton = self.GetChild("AddButton")
				activateButton = self.GetChild("ActivateButton")
				deckButton1 = self.GetChild("deck_button1")
				deckButton2 = self.GetChild("deck_button2")
				self.GetChild("TitleBar").SetCloseEvent(ui.__mem_func__(self.Close))
				
			except:
				import exception
				exception.Abort("PassiveWindow.LoadWindow.BindObject")
	
			self.wndPassive = wndPassive
			self.wndPassiveMaterial = wndPassiveMaterial
			self.wndPassiveJobDownSlot = wndPassiveJobDownSlot
			self.chargeButton = chargeButton
			self.addButton = addButton
			self.activateButton = activateButton
			self.deckButton1 = deckButton1
			self.deckButton2 = deckButton2
			self.wndPassive.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptySlot))
			self.wndPassive.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectItemSlot))
			self.wndPassive.SetUnselectItemSlotEvent(ui.__mem_func__(self.UseItemSlot))
			self.wndPassive.SetUseSlotEvent(ui.__mem_func__(self.UseItemSlot))
			self.wndPassive.SetOverInItemEvent(ui.__mem_func__(self.OverInItem))
			self.wndPassive.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
			self.wndPassive.Show()
			self.wndPassiveMaterial.SetSelectEmptySlotEvent(ui.__mem_func__(self.SelectEmptyMaterialSlot))
			self.wndPassiveMaterial.SetSelectItemSlotEvent(ui.__mem_func__(self.SelectMaterialItemSlot))
			self.wndPassiveMaterial.SetUnselectItemSlotEvent(ui.__mem_func__(self.SelectMaterialItemSlot))
			self.wndPassiveMaterial.SetUseSlotEvent(ui.__mem_func__(self.SelectMaterialItemSlot))
			self.wndPassiveMaterial.SetOverInItemEvent(ui.__mem_func__(self.OverInMaterialItem))
			self.wndPassiveMaterial.SetOverOutItemEvent(ui.__mem_func__(self.OverOutItem))
			self.wndPassiveMaterial.Show()

			if self.wndPassiveJobDownSlot:
				self.wndPassiveJobDownSlot.Hide()

			self.deckButton1.SetEvent(ui.__mem_func__(self.__SelectDeck), 0)
			self.deckButton2.SetEvent(ui.__mem_func__(self.__SelectDeck), 1)
			self.deckButton1.SetMouseLeftButtonDownEvent(ui.__mem_func__(self.__SelectDeck), 0)
			self.deckButton2.SetMouseLeftButtonDownEvent(ui.__mem_func__(self.__SelectDeck), 1)
			self.chargeButton.SetEvent(ui.__mem_func__(self.__OpenChargeQuestionDialog))
			self.addButton.SetEvent(ui.__mem_func__(self.__AddRelicBonus))
			self.activateButton.SetToggleDownEvent(ui.__mem_func__(self.__ToggleRelicActive))
			self.activateButton.SetToggleUpEvent(ui.__mem_func__(self.__ToggleRelicActive))
			self.__RefreshDeckButtonState()
			self.__RefreshButtonState()

		def OnPressEscapeKey(self):
			self.Close()
			return True

		def __GetPassiveSlotIndex(self):
			return item.EQUIPMENT_PASSIVE

		def __GetMaterialSlotOrder(self):
			return (1, 2, 3, 4)

		def __CreateDefaultMaterialSlotData(self):
			return {
				1 : {"vnum" : 30255, "pos" : -1},
				2 : {"vnum" : 30258, "pos" : -1},
				3 : {"vnum" : 30256, "pos" : -1},
				4 : {"vnum" : 30257, "pos" : -1},
			}

		def __RefreshDeckButtonState(self):
			if not self.deckButton1 or not self.deckButton2:
				return

			if self.selectedDeckIndex == 0:
				self.deckButton1.Down()
				self.deckButton2.SetUp()
			else:
				self.deckButton1.SetUp()
				self.deckButton2.Down()

		def __SelectDeck(self, deckIndex):
			if deckIndex == self.selectedDeckIndex:
				self.__RefreshDeckButtonState()
				self.__SendRelicCommand("sky" if deckIndex == 1 else "earth")
				return

			self.selectedDeckIndex = deckIndex
			self.materialSlotData = self.materialSlotDataByDeck[self.selectedDeckIndex]
			self.__RefreshDeckButtonState()
			self.__RefreshMaterialSlots()
			self.__SendRelicCommand("sky" if deckIndex == 1 else "earth")

		def __SyncDeckSelectionFromButtons(self):
			if not self.deckButton1 or not self.deckButton2:
				return

			if self.deckButton2.IsDown() and self.selectedDeckIndex != 1:
				self.__SelectDeck(1)
			elif self.deckButton1.IsDown() and self.selectedDeckIndex != 0:
				self.__SelectDeck(0)

		def __ValidateMaterialSlots(self):
			for slotIndex in self.__GetMaterialSlotOrder():
				itemData = self.materialSlotData[slotIndex]
				slotPos = itemData["pos"]
				if slotPos < 0:
					continue

				if player.GetItemIndex(slotPos) != itemData["vnum"]:
					itemData["pos"] = -1

		def __RefreshMaterialSlots(self):
			self.__ValidateMaterialSlots()

			for slotIndex in self.__GetMaterialSlotOrder():
				itemData = self.materialSlotData[slotIndex]
				slotPos = itemData["pos"]
				if slotPos >= 0:
					itemVNum = player.GetItemIndex(slotPos)
					itemCount = player.GetItemCount(slotPos)
					if itemCount <= 1:
						itemCount = 0

					self.wndPassiveMaterial.SetItemSlot(slotIndex, itemVNum, itemCount)
					self.wndPassiveMaterial.ActivateSlot(slotIndex, wndMgr.COLOR_TYPE_WHITE)
				else:
					self.wndPassiveMaterial.ClearSlot(slotIndex)
					self.wndPassiveMaterial.DeactivateSlot(slotIndex)

			self.wndPassiveMaterial.RefreshSlot()

		def __HasPassiveRelic(self):
			return player.GetItemIndex(self.__GetPassiveSlotIndex()) != 0

		def __IsPassiveRelicActive(self):
			if not self.__HasPassiveRelic():
				return FALSE

			return player.GetItemMetinSocket(self.__GetPassiveSlotIndex(), 1) != 0

		def __RefreshButtonState(self):
			hasPassiveRelic = self.__HasPassiveRelic()
			if hasPassiveRelic:
				self.chargeButton.Enable()
				self.addButton.Enable()
				self.activateButton.Enable()
			else:
				self.chargeButton.Disable()
				self.addButton.Disable()
				self.activateButton.Disable()

			if self.__IsPassiveRelicActive():
				self.activateButton.Down()
			else:
				self.activateButton.SetUp()

		def __GetMaterialArgumentString(self):
			positions = []
			for slotIndex in self.__GetMaterialSlotOrder():
				slotPos = self.materialSlotData[slotIndex]["pos"]
				if slotPos < 0:
					return ""
				positions.append(str(slotPos))

			return " ".join(positions)

		def __SendRelicCommand(self, command, useMaterials = FALSE):
			chatCommand = "/passive_relic " + command
			if useMaterials:
				materialArgs = self.__GetMaterialArgumentString()
				if not materialArgs:
					chat.AppendChat(chat.CHAT_TYPE_INFO, "Tum ruh itemlerini yuvalara yerlestir.")
					return

				chatCommand += " " + materialArgs

			net.SendChatPacket(chatCommand)

		def __CloseQuestionDialog(self):
			if not self.questionDialog:
				return

			self.questionDialog.Close()
			self.questionDialog = None

		def __OpenChargeQuestionDialog(self):
			if not self.__HasPassiveRelic():
				return

			self.__CloseQuestionDialog()
			questionDialog = uiCommon.QuestionDialog("thin")
			questionDialog.SetText("Kutsal ruhu þarj ederken nesneler bu esnada kaybolur. Emin misin?")
			questionDialog.SetAcceptEvent(ui.__mem_func__(self.__AcceptCharge))
			questionDialog.SetCancelEvent(ui.__mem_func__(self.__CloseQuestionDialog))
			questionDialog.Open()
			self.questionDialog = questionDialog

		def __AcceptCharge(self):
			self.__CloseQuestionDialog()
			self.__SendRelicCommand("charge", TRUE)

		def __AddRelicBonus(self):
			if not self.__HasPassiveRelic():
				return

			self.__SendRelicCommand("add", TRUE)

		def __ToggleRelicActive(self):
			if not self.__HasPassiveRelic():
				return

			self.__SendRelicCommand("activate")

		def __SetToolTipPosition(self):
			if not self.toolTipItem:
				return

			(anchorX, anchorY) = self.wndPassive.GetGlobalPosition()
			anchorWidth = self.wndPassive.GetWidth()

			if anchorWidth <= 0 or (anchorX == 0 and anchorY == 0):
				(mouseX, mouseY) = wndMgr.GetMousePosition()
				self.toolTipItem.SetToolTipPosition(int(mouseX), int(mouseY))
				return

			tooltipWidth = self.toolTipItem.GetWidth()
			tooltipHeight = self.toolTipItem.GetHeight()
			screenWidth = wndMgr.GetScreenWidth()
			screenHeight = wndMgr.GetScreenHeight()

			tooltipLeft = anchorX + anchorWidth + 8
			if tooltipLeft + tooltipWidth > screenWidth:
				tooltipLeft = max(0, anchorX - tooltipWidth - 8)

			tooltipTop = max(0, min(anchorY, screenHeight - tooltipHeight - 10))
			tooltipPosX = int(tooltipLeft + tooltipWidth / 2)
			tooltipPosY = int(tooltipTop + tooltipHeight)
			self.toolTipItem.SetToolTipPosition(tooltipPosX, tooltipPosY)

		def __ShowToolTip(self):
			if self.toolTipItem:
				self.__SetToolTipPosition()
				self.toolTipItem.SetInventoryItem(self.__GetPassiveSlotIndex(), player.EQUIPMENT)
				self.__SetToolTipPosition()

		def OverInItem(self, slotIndex):
			if mouseModule.mouseController.isAttached():
				return

			if slotIndex != self.__GetPassiveSlotIndex():
				return

			if player.GetItemIndex(slotIndex) == 0:
				return

			self.__ShowToolTip()

		def OverInMaterialItem(self, slotIndex):
			if mouseModule.mouseController.isAttached():
				return

			if not slotIndex in self.materialSlotData:
				return

			slotPos = self.materialSlotData[slotIndex]["pos"]
			if slotPos < 0:
				return

			self.__SetToolTipPosition()
			self.toolTipItem.SetInventoryItem(slotPos)
			self.__SetToolTipPosition()

		def OverOutItem(self):
			if self.toolTipItem:
				self.toolTipItem.HideToolTip()

		def SelectItemSlot(self, slotIndex):
			if mouseModule.mouseController.isAttached():
				attachedSlotType = mouseModule.mouseController.GetAttachedType()
				attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
				attachedItemVNum = mouseModule.mouseController.GetAttachedItemIndex()

				if attachedSlotType == player.SLOT_TYPE_INVENTORY and attachedItemVNum in (100100, 100101):
					if player.GetItemIndex(self.__GetPassiveSlotIndex()) != 0:
						mouseModule.mouseController.DeattachObject()
						net.SendChatPacket("/passive_relic extract %d" % attachedSlotPos)
						return

			self.UseItemSlot(slotIndex)

		def SelectEmptySlot(self, selectedSlotPos):
			passiveSlotIndex = self.__GetPassiveSlotIndex()
			if selectedSlotPos != passiveSlotIndex and selectedSlotPos != 0:
				return

			if not mouseModule.mouseController.isAttached():
				return

			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemVNum = mouseModule.mouseController.GetAttachedItemIndex()
			attachedWindow = player.SlotTypeToInvenType(attachedSlotType)

			if player.RESERVED_WINDOW == attachedWindow:
				return

			mouseModule.mouseController.DeattachObject()

			if attachedWindow == player.EQUIPMENT:
				return

			if attachedSlotType == player.SLOT_TYPE_INVENTORY and attachedItemVNum in (100100, 100101):
				if player.GetItemIndex(passiveSlotIndex) != 0:
					net.SendChatPacket("/passive_relic extract %d" % attachedSlotPos)
				return

			net.SendItemUsePacket(attachedSlotPos)

		def SelectEmptyMaterialSlot(self, selectedSlotPos):
			if not selectedSlotPos in self.materialSlotData:
				return

			if not mouseModule.mouseController.isAttached():
				return

			attachedSlotType = mouseModule.mouseController.GetAttachedType()
			attachedSlotPos = mouseModule.mouseController.GetAttachedSlotNumber()
			attachedItemVNum = mouseModule.mouseController.GetAttachedItemIndex()

			if player.SLOT_TYPE_INVENTORY != attachedSlotType:
				return

			if attachedItemVNum != self.materialSlotData[selectedSlotPos]["vnum"]:
				chat.AppendChat(chat.CHAT_TYPE_INFO, "Bu item bu yuvaya yerlestirilemez.")
				return

			self.materialSlotData[selectedSlotPos]["pos"] = attachedSlotPos
			mouseModule.mouseController.DeattachObject()
			self.__RefreshMaterialSlots()

		def SelectMaterialItemSlot(self, selectedSlotPos):
			if not selectedSlotPos in self.materialSlotData:
				return

			self.materialSlotData[selectedSlotPos]["pos"] = -1
			self.__RefreshMaterialSlots()

		def TryAttachMaterialFromInventory(self, inventorySlot):
			if inventorySlot < 0:
				return FALSE

			itemVNum = player.GetItemIndex(inventorySlot)
			if itemVNum == 0:
				return FALSE

			for slotIndex in self.__GetMaterialSlotOrder():
				itemData = self.materialSlotData[slotIndex]
				if itemData["vnum"] != itemVNum:
					continue

				currentPos = itemData["pos"]
				if currentPos == inventorySlot:
					return TRUE

				if currentPos >= 0 and player.GetItemIndex(currentPos) == itemData["vnum"]:
					continue

				itemData["pos"] = inventorySlot
				self.__RefreshMaterialSlots()
				return TRUE

			return FALSE

		def __FindEmptyInventorySlot(self):
			for slotPos in xrange(player.INVENTORY_PAGE_COUNT * player.INVENTORY_PAGE_SIZE):
				if player.GetItemIndex(slotPos) == 0:
					return slotPos
			return -1

		def UseItemSlot(self, slotIndex):
			passiveSlotIndex = self.__GetPassiveSlotIndex()
			if player.GetItemIndex(passiveSlotIndex) == 0:
				return

			emptyInventorySlot = self.__FindEmptyInventorySlot()
			if emptyInventorySlot >= 0:
				net.SendItemMovePacket(passiveSlotIndex, emptyInventorySlot, 1)

			self.__SendRelicCommand("unequip")
			net.SendItemUsePacket(passiveSlotIndex)

		def RefreshEquipSlotWindow(self):
			slotNumber = self.__GetPassiveSlotIndex()
			itemVNum = player.GetItemIndex(slotNumber)
			itemCount = player.GetItemCount(slotNumber)
			if itemCount <= 1:
				itemCount = 0

			self.wndPassive.SetItemSlot(slotNumber, itemVNum, itemCount)
			if itemVNum:
				self.wndPassive.ActivateSlot(slotNumber, wndMgr.COLOR_TYPE_WHITE)
			else:
				self.wndPassive.DeactivateSlot(slotNumber)

			self.wndPassive.RefreshSlot()
			self.__RefreshMaterialSlots()
			self.__RefreshDeckButtonState()
			self.__RefreshButtonState()

		def OnUpdate(self):
			if self.IsShow():
				self.__SyncDeckSelectionFromButtons()
				self.RefreshEquipSlotWindow()
				if self.toolTipItem and self.toolTipItem.IsShow():
					self.__SetToolTipPosition()
					if not self.wndPassive.IsIn() and not self.wndPassiveMaterial.IsIn():
						self.toolTipItem.HideToolTip()
