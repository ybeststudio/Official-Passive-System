
// Ara  ( bool CPythonNetworkStream::RecvSpecialEffect() ) içerisinde

		case SE_EQUIP_LOVE_PENDANT:
			effect = CInstanceBase::EFFECT_LOVE_PENDANT_EQUIP;
			break;

// Altına ekle

#ifdef ENABLE_PASSIVE_SYSTEM
		case SE_PASSIVE_EFFECT:
			effect = CInstanceBase::EFFECT_PASSIVE;
			break;
#endif
