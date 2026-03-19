# Metin2 Official Passive System (Ruh Kalıntısı / Kalıntı Bonusu)

**Created by BestStudio**

Resmi Metin2 Kalıntı Bonusu (Ruh Kalıntısı) Metin2 Wiki kaynağından edinilen bilgiler  doğrultusunda geliştirilmiştir. Halka açık paylaşım için hazırlanmış sürümüdür.

[Gif](https://media.giphy.com/media/jalFezTLK0ZK8yyztb/giphy.gif) | [Video](https://youtu.be/_RbJWpcsmAY)

---

## Sistem Özeti

- **Ruh kalıntıları** (30272–30276): Sınıfa özel takılabilir eşya; 2 deck (Toprak/Gök), her deck’te 4 bonus + proc taşı + binek hızı proc.
- **Malzemeler** (30255–30258): Kılıç, zırh, takı, element ruhu — kalıntı üretimi için.
- **Ejderha kerpeteni** (100100/100101): Kalıntıyı çıkarma (30% / 100% başarı).

---

## Ön Koşullar (Zorunlu)

Bu sistem aşağıdaki flag’lere **bağımlıdır**. Kaynak kodunuzda bunlar yoksa önce eklemeniz gerekir:

| Flag | Açıklama |
|------|----------|
| `ENABLE_CONQUEROR_LEVEL` | Şampiyon seviye; `APPLY_SUNGMA_*` (STR, HP, MOVE, IMMUNE) |
| `ENABLE_NEW_BONUS_SYSTEM` | `APPLY_ATTBONUS_STONE`, `APPLY_ATTBONUS_BOSS` |

Ayrıca `WEAR_MAX_NUM` ≥ 64 ve `WEAR_PASSIVE` slot’u olmalıdır.

---

## Klasör Yapısı

```
Official Passive System/
├── README.md                 # Bu dosya
├── 1.Svn/                    # Kaynak kod patch'leri (manuel uygulanır)
│   ├── Server/               # Game + DB + common
│   ├── Client/               # C++ client
│   └── DumpProto/            # ItemCSVReader
├── 2.Client/                 # Hazır client dosyaları (kopyalanır)
│   ├── root/                 # Python UI (ui.py, uicharacter.py, vb.)
│   ├── uiscript/             # passiveattr.py, characterwindow.py
│   ├── locale/tr/            # locale_game.txt, locale_interface.txt
│   ├── effect/               # buff efektleri
│   └── ymir work/            # .sub UI dosyaları
└── 3.Proto/                  # item_proto, item_names (DumpProto çıktısına eklenir)
```

---

## Kurulum Adımları

### 1. Server – service.h

`common/service.h` içinde:

```c
#define ENABLE_PASSIVE_SYSTEM
```

### 2. Server – common (item_length.h, length.h)

`1.Svn/Server/common/` içindeki `item_length.h` ve `length.h` patch’lerini projenize uyarlayın:

- `ITEM_PASSIVE` → `enum EItemTypes` (ITEM_TYPE_MAX üstüne)
- `EPassiveSubTypes`, `MATERIAL_PASSIVE_*` → `item_length.h`
- `WEAR_PASSIVE` → `enum EWearPositions` (`length.h`)
- `SE_PASSIVE_EFFECT` → `enum SPECIAL_EFFECT` (`length.h`)

**Not:** `ITEM_PASSIVE` sıra numarası (örn. 38) projenize göre değişebilir; `ITEM_TYPE_MAX`’ten hemen önce ekleyin.

### 3. Server – affect.h

`AFFECT_QUEST_START_IDX = 1000` satırından **hemen önce** ekleyin:

```c
#ifdef ENABLE_PASSIVE_SYSTEM
	AFFECT_PASSIVE_RELIC_STONE_DEF = 707,
	AFFECT_PASSIVE_RELIC_DISMOUNT_SPEED = 708,
#endif
```

**Uyarı:** 707/708 sizin projenizde başka bir affect ile çakışıyorsa değerleri değiştirin.

### 4. Server – VnumHelper.h

`IsLovePendant` benzeri bir helper’ın altına:

```c
#ifdef ENABLE_PASSIVE_SYSTEM
	static const bool IsPassive(DWORD vnum) { return 30272 <= vnum && vnum <= 30276; }
#endif
```

### 5. Server – Game (cmd_general.cpp, char_item.cpp, item.cpp, cmd.cpp, char_horse.cpp)

`1.Svn/Server/game/src/` içindeki dosyalardaki **"Ara" / "Altına ekle"** yorumlarına göre patch’leri uygulayın. Her dosyada ilgili blokları bulup `#ifdef ENABLE_PASSIVE_SYSTEM` bloklarını ekleyin.

### 6. Server – DB (ProtoReader.cpp)

`1.Svn/Server/db/src/ProtoReader.cpp` patch’lerini uygulayın (`ITEM_PASSIVE`, `MATERIAL_PASSIVE_*`).

### 7. DumpProto

`1.Svn/DumpProto/ItemCSVReader.cpp` patch’lerini uygulayın. `#define ENABLE_PASSIVE_SYSTEM` ve ilgili `arType` / `subtypes` eklemeleri.

### 8. Client – C++

`1.Svn/Client/` altındaki dosyaları projenize uyarlayın:

- `Locale_inc.h` → `ENABLE_PASSIVE_SYSTEM` tanımı
- `Packet.h`, `GameType.h`, `ItemData.h`, `InstanceBase.h`
- `PythonItemModule.cpp`, `PythonCharacterManagerModule.cpp`, `PythonApplicationModule.cpp`, `PythonNetworkStreamPhaseGameItem.cpp`, `PythonPlayerSettingsModule.h`

`PythonApplicationModule.cpp` içinde `app.ENABLE_PASSIVE_SYSTEM` Python modülüne aktarılır.

### 9. Client – Hazır Dosyalar (2.Client)

`2.Client/` içeriğini client pack’inize kopyalayın:

- `root/` → `uiscript` veya `root` (proje yapınıza göre)
- `uiscript/passiveattr.py`, `characterwindow.py` vb.
- `locale/tr/` → `locale/tr/`
- `effect/` → `effect/`
- `ymir work/` → Ymir WorkSpace ile derlenip `etc/ui/` altına alınır

**ui.py** içinde `LoadElement_Passive_TitleBar` ve `Passive_TitleBar` sınıfı tanımlı olmalı; `system/PassiveAttr.xml` (veya eşdeğeri) bu yapıyı kullanır.

**Gerekli UI assetleri:** `passiveattr.py` `d:/ymir work/ui/game/passive_attr/` dizinini kullanır; `ui.py` içindeki `Passive_TitleBar` dosyalarını bekler. Bu görseller yoksa kendi asset’lerinizi kullanacak şekilde yolu değiştirin.

### 10. Proto ve İkonlar

`3.Proto/item_proto.txt` ve `item_names.txt` içeriğini DumpProto çıktınıza ekleyin (veya mevcut `item_proto`/`item_names` dosyalarınıza merge edin).

**item_proto** içinde şunlar olmalı:

- Malzemeler: 30255, 30256, 30257, 30258
- Ruh kalıntıları: 30272, 30273, 30274, 30275, 30276

**Anti-flag uyumu:** Projeniz `ANTI_WARRIOR` kullanıyorsa `ANTI_MUSA` yerine onu yazın. `ANTI_SHAMAN` varsa `ANTI_MUDANG` yerine onu kullanın.

**İkonlar:** `icon/item/30272.tga` … `30276.tga` dosyalarını ekleyin (veya kendi ikonlarınızı kullanın).

---

## Vnum Özeti

| Vnum | Tip | Açıklama |
|------|-----|----------|
| 30255 | MATERIAL | Kılıç ruhu |
| 30256 | MATERIAL | Zırh ruhu |
| 30257 | MATERIAL | Takı ruhu |
| 30258 | MATERIAL | Element ruhu |
| 30272 | ITEM_PASSIVE | Savaşçı ruh kalıntısı |
| 30273 | ITEM_PASSIVE | Ninja ruh kalıntısı |
| 30274 | ITEM_PASSIVE | Sura ruh kalıntısı |
| 30275 | ITEM_PASSIVE | Şaman ruh kalıntısı |
| 30276 | ITEM_PASSIVE | Lycan ruh kalıntısı |
| 100100 | EXTRACT | Ejderha kerpeteni (30%) |
| 100101 | EXTRACT | Ejderha kerpeteni+ (100%) |

---

## Sorun Giderme

| Sorun | Olası neden |
|-------|--------------|
| `APPLY_SUNGMA_*` tanımsız | `ENABLE_CONQUEROR_LEVEL` eksik |
| `APPLY_ATTBONUS_STONE` tanımsız | `ENABLE_NEW_BONUS_SYSTEM` eksik |
| `WEAR_PASSIVE` tanımsız | `length.h` patch’i uygulanmamış |
| Kalıntı penceresi açılmıyor | `app.ENABLE_PASSIVE_SYSTEM` 0 veya UI dosyaları eksik |
| item_proto hatası | `ITEM_PASSIVE`, `PASSIVE_JOB`, `MATERIAL_PASSIVE_*` DumpProto/ProtoReader’da yok |
| AFFECT çakışması | 707/708 başka affect’te kullanılıyorsa ID’leri değiştirin |

---

## İletişim

- 💻 **GitHub:** [github.com/ybeststudio](https://github.com/ybeststudio)
- 🎧 **Discord Server:** [discord.gg/NXmc6JrwYr](https://discord.gg/NXmc6JrwYr)
- 🆔 **Discord ID:** beststudio
- 🌐 **Web:** [bestpro.dev](https://bestpro.dev)
- 💬 **TurkMMO Forum:** [Best Studio](https://forum.turkmmo.com/uye/2104546-best-studio/)
- 📺 **YouTube:** [@ybeststudio](https://www.youtube.com/@ybeststudio)
- 📷 **Instagram:** [@ybeststudio](https://www.instagram.com/ybeststudio)
- 👥 **Facebook:** [ybeststudio](https://www.facebook.com/ybeststudio/)
- 🐦 **Twitter:** [@ybeststudio](https://twitter.com/ybeststudio)
- 🎵 **TikTok:** [@ybeststudio](https://tiktok.com/@ybeststudio)

---

## Lisans ve Kullanım

Bu proje eğitim ve araştırma amaçlıdır. Ticari kullanım için gerekli lisansları kontrol edin.

---

## Katkı

Bu sistem BestStudio tarafından resmi Metin2 Wiki üzerindeki bilgiler ve çeşitli video kaynaklardan edinilen bilgiler doğrultusunda geliştirilmiş ve paylaşıma sunulmuştur. Sorularınız için GitHub issue açabilirsiniz.
