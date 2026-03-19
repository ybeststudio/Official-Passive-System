
# Ara

class HorizontalBar(Window):

# Hemen classın üstüne ekle

if app.ENABLE_PASSIVE_SYSTEM:
	class Passive_TitleBar(Window):
		BLOCK_WIDTH = 22
		BLOCK_HEIGHT = 22
	
		def __init__(self):
			Window.__init__(self)
			self.AddFlag("attach")
	
		def __del__(self):
			Window.__del__(self)
	
		def MakeTitleBar(self, width, color):
			width = max(70, width)
	
			imgLeft = ImageBox()
			imgCenter = ExpandedImageBox()
			imgRight = ImageBox()
			imgLeft.AddFlag("not_pick")
			imgCenter.AddFlag("not_pick")
			imgRight.AddFlag("not_pick")
			imgLeft.SetParent(self)
			imgCenter.SetParent(self)
			imgRight.SetParent(self)
	
			imgLeft.LoadImage("gui/thanos/title_sol.png")
			imgCenter.LoadImage("gui/thanos/title_orta.png")
			imgRight.LoadImage("gui/thanos/title_sag.png")
	
			imgLeft.Show()
			imgCenter.Show()
			imgRight.Show()
	
			btnClose = Button()
			btnClose.SetParent(self)
			btnClose.SetUpVisual("gui/thanos/close.png")
			btnClose.SetOverVisual("gui/thanos/close.png")
			btnClose.SetDownVisual("gui/thanos/close.png")
			btnClose.SetToolTipText(localeInfo.UI_CLOSE, 0, -23)
			btnClose.Show()
	
			self.imgLeft = imgLeft
			self.imgCenter = imgCenter
			self.imgRight = imgRight
			self.btnClose = btnClose
	
			self.SetWidth(width)
	
		def SetWidth(self, width):
			self.imgCenter.SetRenderingRect(0.0, 0.0, float((width - self.BLOCK_WIDTH*2) - self.BLOCK_WIDTH) / self.BLOCK_WIDTH, 0.0)
			self.imgCenter.SetPosition(self.BLOCK_WIDTH, 0)
			self.imgRight.SetPosition(width - self.BLOCK_WIDTH, 0)
	
			if localeInfo.IsARABIC():
				self.btnClose.SetPosition(40, 40)
			else:
				self.btnClose.SetPosition(width - self.btnClose.GetWidth(), 0)
				
			self.SetSize(width, self.BLOCK_HEIGHT)
	
		def SetCloseEvent(self, event):
			self.btnClose.SetEvent(event)


## Ara

	def LoadElementThinBoard(self, window, value, parentWindow):
        

## üstüne ekle

	## Passive
	if app.ENABLE_PASSIVE_SYSTEM:
		def LoadElement_Passive_TitleBar(self, window, value, parentWindow):

			if False == self.CheckKeyList(value["name"], value, self.TITLE_BAR_KEY_LIST):
				return False

			window.MakeTitleBar(int(value["width"]), value.get("color", "red"))
			self.LoadDefaultData(window, value, parentWindow)

			return True
