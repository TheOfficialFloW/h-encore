/**
 * @brief      Language container for HENkaku Settings
 */
#ifndef LANGUAGE_HEADER
#define LANGUAGE_HEADER

typedef struct {
  wchar_t *msg_henkaku_settings;
  wchar_t *msg_enable_psn_spoofing;
  wchar_t *msg_enable_unsafe_homebrew;
  wchar_t *msg_unsafe_homebrew_description;
  wchar_t *msg_enable_version_spoofing;
  wchar_t *msg_spoofed_version;
  wchar_t *msg_button_behavior;
  wchar_t *msg_button_enter;
  wchar_t *msg_button_cancel;
  wchar_t *msg_reload_taihen_config;
  wchar_t *msg_reload_taihen_config_success;
  wchar_t *msg_reboot_device;
  wchar_t *msg_content_downloader;
  wchar_t *msg_unlink_memory_card;
  wchar_t *msg_unlink_memory_card_success;
  wchar_t *msg_unlink_memory_card_error;
} language_container_t;

// by Nzaki0716 & SnyFbSx & kirisame0017
language_container_t language_japanese = {  
  L"HENkakuの設定",
  L"PSNの偽装を有効化",
  L"リスクのあるユーザープログラムを有効化",
  L"リスクのあるユーザープログラムは誤った操作や設定、または悪意のあるコードにより端末に修復不可能なダメージを与えることがあります。\nこれらをインストールする際は注意してください。",
  L"バージョンの偽装を有効化",
  L"偽装バージョンの確認",
  L"○ボタンの動作設定",
  L"決定",
  L"キャンセル",
  L"taiHEN config.txtのリロード",
  L"taiHEN config.txtのリロードが完了しました。",
  L"端末の再起動",
  L"コンテンツ ダウンローダー",
  L"メモリーカードとのリンクを解除する",
  L"ux0:id.datが削除されました。メモリーカードはこのVita以外でも使用できるようになりました。",
  L"ux0のマウントに失敗しました。メモリーカードは挿入されていますか？",
};

// by molecule
language_container_t language_english_us = {
  L"HENkaku Settings",
  L"Enable PSN spoofing",
  L"Enable Unsafe Homebrew",
  L"Unsafe homebrews can damage your device permanently, if they are misused, misconfigured, or malicious.\nPlease take caution when installing them.",
  L"Enable Version Spoofing",
  L"Spoofed Version",
  L"○ Button Behavior",
  L"Enter",
  L"Cancel",
  L"Reload taiHEN config.txt",
  L"taiHEN config.txt has been successfully reloaded.",
  L"Reboot Device",
  L"Content Downloader",
  L"Unlink Memory Card",
  L"ux0:id.dat has been deleted. This memory card can now be used on any Vita.",
  L"Failed to mount ux0. Is a memory card inserted?",
};

// by jokira & devnoname120 & CelesteBlue-dev & chronoss09 & francl35
language_container_t language_french = {
  L"Paramètres de HENkaku",
  L"Activer le spoof du PSN",
  L"Autoriser l'installation de homebrews non sécurisés",
  L"Les homebrews non sécurisés peuvent endommager votre système de façon permanente s'ils sont mal utilisés, mal configurés, ou malveillants.\nPrenez garde en activant ce paramètre.",
  L"Activer le spoof de version",
  L"Version spoofée",
  L"Action du bouton ○",
  L"Valider",
  L"Annuler",
  L"Recharger le fichier config.txt de taiHEN",
  L"Le fichier config.txt de taiHEN a bien été rechargé.",
  L"Redémarrer la console",
  L"Téléchargement de contenu",
  L"Délier la carte mémoire",
  L"ux0:id.dat a été supprimé. Cette carte mémoire peut être utilisée sur n'importe quelle Vita.",
  L"Le montage de ux0 a échoué. Y a-t-il une carte mémoire insérée?",
};

// by EricWeichhart & iamn0tdev & WZ-JK
language_container_t language_spanish = {
  L"Ajustes de HENkaku",
  L"Activar Spoofing para PSN",
  L"Activar Homebrew inseguro",
  L"Homebrews inseguros pueden dañar tu dispositivo permanentemente si son maliciosos, erróneamente usados o configurados incorrectamente.\nPor favor, tenga precaución al instalarlos.",
  L"Activar Spoofing de versión",
  L"Versión para Spoofing",
  L"Comportamiento del botón ○",
  L"Aceptar",
  L"Cancelar",
  L"Recargar taiHEN config.txt",
  L"taiHEN config.txt ha sido recargado correctamente.",
  L"Reiniciar dispositivo",
  L"Descargador de Contenido",
  L"Desvincular la Tarjeta de Memoria",
  L"El ux0:id.dat ha sido eliminado. Esta tarjeta de memoria ahora se puede utilizar en cualquier Vita.",
  L"Ha habido un fallo al montar el ux0. ¿Se ha insertado una tarjeta de memoria?",
};

// by Ziusun
language_container_t language_german = {
  L"HENkaku-Einstellungen",
  L"PSN-Zugang aktivieren",
  L"Unsichere Homebrew zulassen",
  L"Unsichere Homebrew kann das Gerät dauerhaft beschädigen, sollte sie unsachgemäß eingesetzt und konfiguriert werden oder schadhaft sein.\nDie Installation bitte immer mit größter Sorgfalt vornehmen.",
  L"Andere Firmware vortäuschen",
  L"Aktuelle Firmware",
  L"Funktion der ○-Taste",
  L"Bestätigen",
  L"Abbrechen",
  L"Die config.txt von taiHEN aktualisieren",
  L"Die config.txt von taiHEN wurde erfolgreich aktualisiert.",
  L"Gerät neustarten",
  L"Inhaltsdownloader",
  L"Speicherkarte entkoppeln",
  L"Der Dateieintrag ux0:id.dat wurde gelöscht. Diese Speicherkarte kann jetzt mit jeder Playstation Vita verwendet werden.",
  L"Die Partition ux0: konnte nicht gemountet werden. Ist eine Speicherkarte eingesteckt?",
};

// by blackjack4it & Checcolino & Rufis01
language_container_t language_italian = {
  L"Impostazioni Henkaku",
  L"Abilita lo Spoof PSN",
  L"Abilita Homebrews non sicuri",
  L"Gli Homebrews non sicuri possono danneggiare irrimediabilmente la tua console, se usati/configurati non correttamente o sospetti.\nFai molta attenzione prima di procedere alla loro installazione.",
  L"Abilita lo Spoof della versione corrente",
  L"Versione corrente (Spoof)",
  L"Comportamento del tasto ○",
  L"OK",
  L"Annulla",
  L"Ricarica taiHEN config.txt",
  L"TaiHEN config.txt ricaricato correttamente.",
  L"Riavvia dispositivo",
  L"Scarica contenuto",
  L"Disassocia Memory Card",
  L"ux0:id.dat è stato cancellato. Adesso questa Memory Card può essere usata su qualsiasi PS Vita.",
  L"Impossibile montare ux0. La Memory Card è inserita?",
};

// by ConsoleHax & 2dook & jja2000
language_container_t language_dutch = {
  L"HENkaku Instellingen",
  L"Activeer PSN spoofing",
  L"Onveilige Homebrew toestaan",
  L"Onveilige homebrew kan uw toestel permanent beschadigen indien deze onjuist geconfigureerd of kwaadaardig zijn.\nLet op met wat u installeert.",
  L"Schakel Versie Spoofing in",
  L"Spoofed Versie",
  L"Gedrag van de ○-knop",
  L"Enter",
  L"Annuleren",
  L"Herlaad taiHEN config.txt",
  L"taiHEN config.txt is succesvol herladen.",
  L"Herstart apparaat",
  L"Inhoud downloader",
  L"Geheugenkaart loskoppelen",
  L"ux0:id.dat is verwijderd. Deze geheugenkaart kan nu op elke Vita gebruikt worden.",
  L"Aankoppelen van ux0 is mislukt. Is er wel een geheugenkaart aanwezig?",
};

// by gnmmarechal & menelkir & bosshunter
language_container_t language_portuguese_pt = {
  L"Definições do HENkaku",
  L"Ativar spoof da PSN",
  L"Permitir aplicações inseguras",
  L"Aplicações inseguras podem danificar o sistema permanentemente, se forem mal usadas, configuradas ou maliciosas.\nTenha cuidado ao instalá-las.",
  L"Ativar spoof da versão",
  L"Versão do spoof",
  L"Ação do botão ○",
  L"Entrar",
  L"Cancelar",
  L"Recarrege o ficheiro config.txt do taiHEN",
  L"O ficheiro taiHEN config.txt foi recarregado com sucesso.",
  L"Reiniciar o dispositivo",
  L"Descarregar Conteúdo",
  L"Desassociar Cartão de Memória",
  L"ux0:id.dat foi eliminado. Este cartão de memória pode agora ser usado em qualquer Vita.",
  L"Falha ao montar ux0. O cartão de memória está inserido?",
};

// by Tenek & MuskratDesman & dec0dOS
language_container_t language_russian = {
  L"Настройки HENkaku",
  L"Включить спуфинг PSN",
  L"Включить небезопасные приложения",
  L"Небезопасные приложения могут повредить ваше устройство навсегда, если они используются неправильно, неправильно настроены или вредоносны.\nПожалуйста, проявляйте осторожность при их установке.",
  L"Включить подмену версии",
  L"Поддельная версия",
  L"Поведение кнопки ○",
  L"Ввод",
  L"Отмена",
  L"Перезагрузить taiHEN config.txt",
  L"taiHEN config.txt был успешно перезагружен.",
  L"Перезапустить устройство",
  L"Загрузчик контента",
  L"Отвязать карту памяти",
  L"ux0:id.dat был удален. Эта карта памяти теперь может быть использована на любой PSVita.",
  L"Ошибка при монтировании ux0. Проверьте, вставлена ли карта памяти",
};

// by TriggerHavoc
language_container_t language_korean = {
  L"HENkaku 설정",
  L"PSN 스푸핑 활성화",
  L"안전하지 않은 홈브류 활성화",
  L"만약 홈브류가 오설정, 또는 악성코드등 오용된다면, 안전하지 않은 홈브류 활성화 옵션은 당신의 디바이스에 영구적으로 피해를 줄 수 있습니다.\n홈브류를 설치할 때 주의해주세요.",
  L"버전 스푸핑 활성화",
  L"스푸핑 버전",
  L"○ 버튼 설정",
  L"확인",
  L"취소",
  L"taiHEN config.txt를 다시 로드하기",
  L"taiHEN config.txt를 성공적으로 로드했습니다.",
  L"기기 재부팅",
  L"컨텐츠 다운로더",
  L"메모리 카드 언링크",
  L"ux0:id.dat 파일이 삭제되었습니다. 이제 이 메모리 카드는 모든 비타에서 사용하실 수 있습니다.",
  L"ux0에 마운트 할 수 없습니다. 메모리 카드가 올바르게 삽입되어 있습니까?",
};

// by iTZQing & FlexingTiger & magpte
language_container_t language_chinese_t = {
  L"HENkaku設置",
  L"啟用PSN偽裝",
  L"啟用不安全自制軟件",
  L"如果不安全自制軟件被誤用、配置出現錯誤或軟件本身是惡意程序，可能會永久性損壞你的設備。\n請謹慎安裝！",
  L"啟用版本偽裝",
  L"偽裝版本",
  L"○鍵配置",
  L"確定",
  L"取消",
  L"重載taiHEN config.txt",
  L"重載taiHEN config.txt成功",
  L"重啟設備",
  L"下載內容管理器",
  L"解除記憶卡關聯",
  L"已刪除ux0:id.dat，現在這張記憶卡可應用到任意PSV上",
  L"掛載ux0失敗，請檢查記憶卡是否插入",
};

// by iTZQing & FlexingTiger & magpte
language_container_t language_chinese_s = {
  L"HENkaku设置",
  L"启用PSN伪装",
  L"启用不安全自制软件",
  L"如果不安全自制软件被误用、配置出现错误或软件本身是恶意程序，可能会永久性损坏你的设备。\n请谨慎安装！",
  L"启用版本伪装",
  L"伪装版本",
  L"○键配置",
  L"确定",
  L"取消",
  L"重载taiHEN config.txt",
  L"重载taiHEN config.txt成功",
  L"重启设备",
  L"下载内容管理器",
  L"解除存储卡关联",
  L"已删除ux0:id.dat，现在这张存储卡可用在任意PSV上",
  L"挂载ux0失败，请检查存储卡是否插入",
};

// by v5000a & poppamies
language_container_t language_finnish = {
  L"HENkakun Asetukset",
  L"Ota PSN spooffaaminen käyttöön",
  L"Salli vaaralliset homebrew-ohjelmat",
  L"Vaaralliset homebrew-ohjelmat voivat vahingoittaa laitettasi pysyvästi, mikäli niitä käytetään väärin, ne konfiguroidaan väärin, tai ne ovat haittaohjelmia.\nOle varovainen asentaessasi niitä.",
  L"Ota version spooffaaminen käyttöön",
  L"Spooffattu versio",
  L"○-näppäimen toiminto",
  L"Valitse",
  L"Peruuta",
  L"Lataa taiHEN config.txt uudelleen",
  L"taiHEN config.txt uudelleenladattiin onnistuneesti.",
  L"Käynnistä laite uudelleen",
  L"Sisällönlataaja",
  L"Irrota muistikortti",
  L"ux0:id.dat on poistettu. Voit nyt käyttää tätä muistikorttia millä tahansa Vitalla.",
  L"ux0:n liittäminen epäonnistui. Onko muistikortti laitteessa?",
};

// by MrOrbital & GizmoTheGreen & Wassburgare
language_container_t language_swedish = {
  L"HENkaku-inställningar",
  L"Aktivera fejkad PSN-inloggning",
  L"Tillåt osäkra homebrews",
  L"Osäkra homebrews kan skada din enhet permanent om de används felaktigt, är felkonfigurerade eller innehåller skadlig kod.\nVar försiktig vid installation av sådana.",
  L"Aktivera fejkad version",
  L"Fejkad version",
  L"○-knappens beteende",
  L"Välj",
  L"Avbryt",
  L"Ladda om taiHEN config.txt",
  L"taiHEN config.txt har laddats om.",
  L"Starta om enhet",
  L"Innehållsnedladdare",
  L"Avlänka minneskort",
  L"ux0:id.dat har tagits bort. Det här minneskortet kan nu användas i valfri Vita.",
  L"Misslyckades med att montera ux0. Har du satt i ett minneskort?",
};

// by coestergaard & andrsnDK
language_container_t language_danish = {
  L"HENkaku indstillinger",
  L"Aktiver PSN spoofing",
  L"Tillad usikre homebrews",
  L"Usikre homebrews kan skade din enhed permanent, hvis de bruges forkert, er fejlkonfigurerede eller indeholder skadelig kode.\nVær forsigtig når du installerer disse.",
  L"Aktiver version spoofing",
  L"Spoofed version",
  L"○ knap funktion",
  L"Vælg",
  L"Afbryd",
  L"Genindlæs taiHEN config.txt",
  L"taiHEN config.txt er blevet genindlæst.",
  L"Genstart enhed",
  L"Indholdshenter",
  L"Fjern link til hukommelseskort",
  L"ux0:id.dat er blevet slettet. Dette hukommelseskort kan nu bruges på en vilkårlig Vita.",
  L"Kunne ikke montere ux0. Er der indsat et hukommelseskort?",
};

// by baniel105 & irchagaming & Promises
language_container_t language_norwegian = {
  L"HENkaku Instillinger",
  L"Aktiver forfalsket PSN-innlogging",
  L"Tillat usikker Homebrew",
  L"Usikre Homebrews kan ødelegge enhenten din permanent, hvis de er misbrukt, feilkonfigerert, eller inneholder skadelig programvare.\nVær forsiktig ved installasjon.",
  L"Aktiver forfalsket versjon",
  L"Forfalsket versjon",
  L"Funksjon på ○-knapp",
  L"Velg",
  L"Avbryt",
  L"Last på nytt taiHEN config.txt",
  L"taiHEN config.txt har blitt lastet.",
  L"Start enheten på nytt",
  L"Innholds Nedlaster",
  L"Lås opp Minnekort",
  L"ux0:id.dat er slettet. Dette minnekortet kan brukest på hvilken som helst Vita.",
  L"Klarte ikkje montere av ux0. Har du satt i eit minnekort?",
};

// by Grzybojad & szczuru & ItsYogSothoth & szczuru
language_container_t language_polish = {
  L"Ustawienia HENkaku",
  L"Włącz fałszowanie PSN/SEN (spoofing)",
  L"Zezwól na podejrzane Homebrew",
  L"Podejrzane aplikacje mogą zawierać złośliwe oprogramowanie, przez co mogą trwale uszkodzić twoje urządzenie, jeżeli zostaną niepoprawnie użyte, bądź źle skonfigurowane.\nProszę zachować ostrożność przy ich instalacji.",
  L"Włącz fałszowanie wersji oprogramowania (spoofing)",
  L"Fałszowana wersja",
  L"Akcja przycisku ○",
  L"OK",
  L"Anuluj",
  L"Załaduj ponownie config.txt taiHEN",
  L"config.txt taiHEN został pomyślnie załadowany ponownie.",
  L"Uruchom ponownie konsolę",
  L"Pobieranie zawartości",
  L"Odłącz kartę pamięci",
  L"Plik ux0:id.dat został usunięty z karty pamięci. Można jej teraz używać z każdym systemem PS Vita.",
  L"Błąd podczas montowania partycji ux0, czy karta pamięci jest włożona?",
};

// by GrayJack & haouingvalt & menelkir
language_container_t language_portuguese_br = {
  L"Configurações do HENkaku",
  L"Habilitar mimetização da PSN ",
  L"Habilitar softwares 'homebrew' inseguros",
  L"Softwares 'homebrew' inseguros podem danificar o seu aparelho permanentemente caso sejam usados ou configurados incorretamente, ou até mesmo maliciosos.\nPor favor, tenha cuidado ao instalá-los.",
  L"Habilitar mimetização de Versão",
  L"Versão a mimetizar",
  L"Ação do botão ○",
  L"Entrar",
  L"Cancelar",
  L"Recarrege o arquivo config.txt do taiHEN",
  L"taiHEN config.txt foi recarregado com sucesso.",
  L"Reiniciar o dispositivo",
  L"Baixar Conteúdo",
  L"Desassociar o Cartão de Memória",
  L"ux0:id.dat foi deletado. Agora este cartão de memória pode ser usado em qualquer Vita.",
  L"Falha ao montar ux0. O cartão de memória está inserido?",
};

// by molecule
language_container_t language_english_gb = {
  L"HENkaku Settings",
  L"Enable PSN spoofing",
  L"Enable Unsafe Homebrew",
  L"Unsafe homebrews can damage your device permanently, if they are misused, misconfigured, or malicious.\nPlease take caution when installing them.",
  L"Enable Version Spoofing",
  L"Spoofed Version",
  L"○ Button Behaviour",
  L"Enter",
  L"Cancel",
  L"Reload taiHEN config.txt",
  L"taiHEN config.txt has been successfully reloaded.",
  L"Reboot Device",
  L"Content Downloader",
  L"Unlink Memory Card",
  L"ux0:id.dat has been deleted. This memory card can now be used on any Vita.",
  L"Failed to mount ux0. Is a memory card inserted?",
};

// by Chronicl3
language_container_t language_turkish = {
  L"HENkaku Ayarları",
  L"PSN spoofing Etkinleştir",
  L"Yabancı Uygulamalara İzin Ver ",
  L"Bilinmeyen kaynaklardan uygulama yüklerken dikkatli olunuz.\nKötü niyetli,yanlış yapılandırılmış uygulamalar cihazınıza kalıcı zararlar verebilir.",
  L"Versiyon Spoofing Etkinleştir",
  L"Spoofed Versiyon",
  L"○ Düğmesi Davranışı",
  L"Gir",
  L"İptal",
  L"taiHEN config.txt'yi yeniden yükle",
  L"taiHEN config.txt başarılı bir şekilde yeniden yüklendi.",
  L"Cihazı yeniden başlat",
  L"İçerik İndiricisi",
  L"Hafıza Kartı Bağlantısını Kaldır",
  L"ux0:id.dat silindi.Bu hafıza kartı şuan herhangi bir vita da kullanılabilir.",
  L"ux0 dizin okuma başarısız oldu.Hafıza kartınız takılı mı?",
};

#endif // LANGUAGE_HEADER
