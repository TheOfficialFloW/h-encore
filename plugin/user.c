#include <psp2/kernel/clib.h>
#include <psp2/kernel/modulemgr.h>
#include <psp2/io/fcntl.h>
#include <psp2/io/stat.h>
#include <psp2/registrymgr.h>
#include <psp2/system_param.h>
#include <psp2/power.h>
#include <taihen.h>
#include "henkaku.h"
#include "language.h"
#include "config.h"

int _vshIoMount(int id, const char *path, int permissions, void *opt);
int _vshSblGetSystemSwVersion(SceKernelFwInfo *info);

extern unsigned char _binary_system_settings_xml_start;
extern unsigned char _binary_system_settings_xml_size;
extern unsigned char _binary_henkaku_settings_xml_start;
extern unsigned char _binary_henkaku_settings_xml_size;

static henkaku_config_t config;

static SceUID g_hooks[15];

static tai_hook_ref_t g_sceKernelGetSystemSwVersion_SceSettings_hook;
static int sceKernelGetSystemSwVersion_SceSettings_patched(SceKernelFwInfo *info) {
  int ret;
  SceKernelFwInfo real_info;
  ret = TAI_CONTINUE(int, g_sceKernelGetSystemSwVersion_SceSettings_hook, info);
  sceClibMemset(&real_info, 0, sizeof(SceKernelFwInfo));
  real_info.size = sizeof(SceKernelFwInfo);
  _vshSblGetSystemSwVersion(&real_info);
  if (BETA_RELEASE) {
    sceClibSnprintf(info->versionString, 16, "%s \xE5\xA4\x89\xE9\x9D\xA9-%d\xCE\xB2%d", real_info.versionString, HENKAKU_RELEASE, BETA_RELEASE);
  } else if (HENKAKU_RELEASE > 1) {
    sceClibSnprintf(info->versionString, 16, "%s \xE5\xA4\x89\xE9\x9D\xA9-%d", real_info.versionString, HENKAKU_RELEASE);
  } else {
    sceClibSnprintf(info->versionString, 16, "%s \xE5\xA4\x89\xE9\x9D\xA9", real_info.versionString);
  }
  return ret;
}

static tai_hook_ref_t g_update_check_hook;
static int update_check_patched(int a1, int a2, int *a3, int a4, int a5) {
  TAI_CONTINUE(int, g_update_check_hook, a1, a2, a3, a4, a5);
  *a3 = 0;
  return 0;
}

static tai_hook_ref_t g_game_update_check_hook;
static int game_update_check_patched(int newver, int *needsupdate) {
  TAI_CONTINUE(int, g_game_update_check_hook, newver, needsupdate);
  *needsupdate = 0;
  return 0;
}

static tai_hook_ref_t g_passphrase_decrypt_hook;
static void passphrase_decrypt_patched(void *dat0, void *dat1, void *dat2, char *passphrase, int *result) {
  TAI_CONTINUE(void, g_passphrase_decrypt_hook, dat0, dat1, dat2, passphrase, result);
  if (config.use_psn_spoofing && PSN_PASSPHRASE[0] != '\0' && *result == 1) {
    sceClibMemcpy(passphrase, PSN_PASSPHRASE, sizeof(PSN_PASSPHRASE));
  }
}

static tai_hook_ref_t g_SceVshBridge_333875AB_SceShell_hook;
static int SceVshBridge_333875AB_SceShell_patched(void) {
  TAI_CONTINUE(int, g_SceVshBridge_333875AB_SceShell_hook);
  return 1;
}

static void save_config_user(void) {
  SceUID fd;
  int rd;
  fd = sceIoOpen(CONFIG_PATH, SCE_O_TRUNC | SCE_O_CREAT | SCE_O_WRONLY, 6);
  if (fd >= 0) {
    rd = sceIoWrite(fd, &config, sizeof(config));
    sceIoClose(fd);
    if (rd != sizeof(config)) {
      LOG("config not right size: %d", rd);
    }
  } else {
    LOG("could not write config file");
  }
}

static int load_config_user(void) {
  SceUID fd;
  int rd;
  int migrate = 0;
  fd = sceIoOpen(CONFIG_PATH, SCE_O_RDONLY, 0);
  if (fd < 0) {
    fd = sceIoOpen(OLD_CONFIG_PATH, SCE_O_RDONLY, 0);
    migrate = 1;
  }
  if (fd >= 0) {
    rd = sceIoRead(fd, &config, sizeof(config));
    sceIoClose(fd);
    if (rd == sizeof(config)) {
      if (config.magic == HENKAKU_CONFIG_MAGIC) {
        if (config.version >= 0) {
          if (migrate) {
            LOG("migrating settings to new path");
            save_config_user();
            sceIoRemove(OLD_CONFIG_PATH);
          }
          return 0;
        } else {
          LOG("config version too old");
        }
      } else {
        LOG("config incorrect magic: %x", config.magic);
      }
    } else {
      LOG("config not right size: %d", rd);
    }
  } else {
    LOG("config file not found");
  }
  // default config
  config.magic = HENKAKU_CONFIG_MAGIC;
  config.version = HENKAKU_RELEASE;
  config.use_psn_spoofing = 1;
  config.allow_unsafe_hb = 0;
  config.use_spoofed_version = 1;
  config.spoofed_version = SPOOF_VERSION;
  return 0;
}

static tai_hook_ref_t g_sceRegMgrGetKeyInt_SceSystemSettingsCore_hook;
static int sceRegMgrGetKeyInt_SceSystemSettingsCore_patched(const char *category, const char *name, int *value) {
  if (sceClibStrncmp(category, "/CONFIG/HENKAKU", 15) == 0) {
    if (value) {
      load_config_user();
      if (sceClibStrncmp(name, "enable_psn_spoofing", 19) == 0) {
        *value = config.use_psn_spoofing;
      } else if (sceClibStrncmp(name, "enable_unsafe_homebrew", 22) == 0) {
        *value = config.allow_unsafe_hb;
      } else if (sceClibStrncmp(name, "enable_version_spoofing", 23) == 0) {
        *value = config.use_spoofed_version;
      }
    }
    return 0;
  }
  return TAI_CONTINUE(int, g_sceRegMgrGetKeyInt_SceSystemSettingsCore_hook, category, name, value);
}

static tai_hook_ref_t g_sceRegMgrSetKeyInt_SceSystemSettingsCore_hook;
static int sceRegMgrSetKeyInt_SceSystemSettingsCore_patched(const char *category, const char *name, int value) {
  if (sceClibStrncmp(category, "/CONFIG/HENKAKU", 15) == 0) {
    if (sceClibStrncmp(name, "enable_psn_spoofing", 19) == 0) {
      config.use_psn_spoofing = value;
    } else if (sceClibStrncmp(name, "enable_unsafe_homebrew", 22) == 0) {
      config.allow_unsafe_hb = value;
    } else if (sceClibStrncmp(name, "enable_version_spoofing", 23) == 0) {
      config.use_spoofed_version = value;
    }
    save_config_user();
    henkaku_reload_config();
    return 0;
  }
  return TAI_CONTINUE(int, g_sceRegMgrSetKeyInt_SceSystemSettingsCore_hook, category, name, value);
}

static void build_version_string(int version, char *string, int length) {
  if (version && string && length >= 6) {
    char a = (version >> 24) & 0xF;
    char b = (version >> 20) & 0xF;
    char c = (version >> 16) & 0xF;
    char d = (version >> 12) & 0xF;
    sceClibMemset(string, 0, length);
    string[0] = '0' + a;
    string[1] = '.';
    string[2] = '0' + b;
    string[3] = '0' + c;
    string[4] = '\0';
    if (d) {
      string[4] = '0' + d;
      string[5] = '\0';
    }
  }
}

static tai_hook_ref_t g_sceRegMgrGetKeyStr_SceSystemSettingsCore_hook;
static int sceRegMgrGetKeyStr_SceSystemSettingsCore_patched(const char *category, const char *name, char *string, int length) {
  if (sceClibStrncmp(category, "/CONFIG/HENKAKU", 15) == 0) {
    if (sceClibStrncmp(name, "spoofed_version", 15) == 0) {
      if (string != NULL) {
        load_config_user();
        build_version_string(config.spoofed_version ? config.spoofed_version : SPOOF_VERSION, string, length);
      }
    }
    return 0;
  }
  return TAI_CONTINUE(int, g_sceRegMgrGetKeyStr_SceSystemSettingsCore_hook, category, name, string, length);
}

#define IS_DIGIT(i) (i >= '0' && i <= '9')
static tai_hook_ref_t g_sceRegMgrSetKeyStr_SceSystemSettingsCore_hook;
static int sceRegMgrSetKeyStr_SceSystemSettingsCore_patched(const char *category, const char *name, const char *string, int length) {
  if (sceClibStrncmp(category, "/CONFIG/HENKAKU", 15) == 0) {
    if (sceClibStrncmp(name, "spoofed_version", 15) == 0) {
      if (string != NULL) {
        if (IS_DIGIT(string[0]) && string[1] == '.' && IS_DIGIT(string[2]) && IS_DIGIT(string[3])) {
          char a = string[0] - '0';
          char b = string[2] - '0';
          char c = string[3] - '0';
          char d = IS_DIGIT(string[4]) ? string[4] - '0' : '\0';
          config.spoofed_version = ((a << 24) | (b << 20) | (c << 16) | (d << 12));
          save_config_user();
          henkaku_reload_config();
        }
      }
    }
    return 0;
  }
  return TAI_CONTINUE(int, g_sceRegMgrSetKeyStr_SceSystemSettingsCore_hook, category, name, string, length);
}

typedef struct {
  int size;
  const char *name;
  int type;
  int unk;
} SceRegMgrKeysInfo;

static tai_hook_ref_t g_sceRegMgrGetKeysInfo_SceSystemSettingsCore_hook;
static int sceRegMgrGetKeysInfo_SceSystemSettingsCore_patched(const char *category, SceRegMgrKeysInfo *info, int unk) {
  if (sceClibStrncmp(category, "/CONFIG/HENKAKU", 15) == 0) {
    if (info) {
      if (sceClibStrncmp(info->name, "spoofed_version", 15) == 0) {
        info->type = 0x00030001; // type string
      } else {
        info->type = 0x00040000; // type integer
      }
    }
    return 0;
  }
  return TAI_CONTINUE(int, g_sceRegMgrGetKeysInfo_SceSystemSettingsCore_hook, category, info, unk);
}

static int (* g_OnButtonEventIduSettings_hook)(const char *id, int a2, void *a3);
static int OnButtonEventIduSettings_patched(const char *id, int a2, void *a3) {
  int ret;
  uint32_t buf[6];
  if (sceClibStrncmp(id, "id_reload_taihen_config", 23) == 0) {
    return taiReloadConfig();
  } else if (sceClibStrncmp(id, "id_reboot_device", 16) == 0) {
    return scePowerRequestColdReset();
  } else if (sceClibStrncmp(id, "id_unlink_memory_card", 21) == 0) {
    sceClibMemset(buf, 0, sizeof(buf));
    if ((ret = _vshIoMount(0x800, NULL, 2, buf)) < 0) {
      if (ret != 0x80010011) { // SCE_ERROR_ERRNO_EEXIST (already mounted)
        return ret;
      }
    }
    if ((ret = sceIoRemove("ux0:id.dat")) < 0) {
      if (ret != 0x80010002) { // SCE_ERROR_ERRNO_ENOENT (no entry)
        return ret;
      }
    }
    return 0;
  }
  return g_OnButtonEventIduSettings_hook(id, a2, a3);
}

static tai_hook_ref_t g_scePafToplevelInitPluginFunctions_SceSettings_hook;
static int scePafToplevelInitPluginFunctions_SceSettings_patched(void *a1, int a2, uint32_t *funcs) {
  int res = TAI_CONTINUE(int, g_scePafToplevelInitPluginFunctions_SceSettings_hook, a1, a2, funcs);
  char *plugin = (char *)((uint32_t *)a1)[1];
  if (sceClibStrncmp(plugin, "idu_settings_plugin", 19) == 0) {
    if (funcs[6] != (uint32_t)OnButtonEventIduSettings_patched) {
      g_OnButtonEventIduSettings_hook = (void *)funcs[6];
      funcs[6] = (uint32_t)OnButtonEventIduSettings_patched;
    }
  }
  return res;
}

static tai_hook_ref_t g_scePafMiscLoadXmlLayout_SceSettings_hook;
static int scePafMiscLoadXmlLayout_SceSettings_patched(int a1, void *xml_buf, int xml_size, int a4) {
  if ((82+22) < xml_size && sceClibStrncmp(xml_buf+82, "system_settings_plugin", 22) == 0) {
    xml_buf = (void *)&_binary_system_settings_xml_start;
    xml_size = (int)&_binary_system_settings_xml_size;
  } else if ((79+19) < xml_size && sceClibStrncmp(xml_buf+79, "idu_settings_plugin", 19) == 0) {
    xml_buf = (void *)&_binary_henkaku_settings_xml_start;
    xml_size = (int)&_binary_henkaku_settings_xml_size;
  }
  return TAI_CONTINUE(int, g_scePafMiscLoadXmlLayout_SceSettings_hook, a1, xml_buf, xml_size, a4);
}

static tai_hook_ref_t g_scePafToplevelGetText_SceSystemSettingsCore_hook;
static wchar_t *scePafToplevelGetText_SceSystemSettingsCore_patched(void *arg, char **msg) {
  language_container_t *language_container;
  int language = -1;
  sceRegMgrGetKeyInt("/CONFIG/SYSTEM", "language", &language);
  switch (language) {
    case SCE_SYSTEM_PARAM_LANG_JAPANESE:      language_container = &language_japanese;      break;
    case SCE_SYSTEM_PARAM_LANG_ENGLISH_US:    language_container = &language_english_us;    break;
    case SCE_SYSTEM_PARAM_LANG_FRENCH:        language_container = &language_french;        break;
    case SCE_SYSTEM_PARAM_LANG_SPANISH:       language_container = &language_spanish;       break;
    case SCE_SYSTEM_PARAM_LANG_GERMAN:        language_container = &language_german;        break;
    case SCE_SYSTEM_PARAM_LANG_ITALIAN:       language_container = &language_italian;       break;
    case SCE_SYSTEM_PARAM_LANG_DUTCH:         language_container = &language_dutch;         break;
    case SCE_SYSTEM_PARAM_LANG_PORTUGUESE_PT: language_container = &language_portuguese_pt; break;
    case SCE_SYSTEM_PARAM_LANG_RUSSIAN:       language_container = &language_russian;       break;
    case SCE_SYSTEM_PARAM_LANG_KOREAN:        language_container = &language_korean;        break;
    case SCE_SYSTEM_PARAM_LANG_CHINESE_T:     language_container = &language_chinese_t;     break;
    case SCE_SYSTEM_PARAM_LANG_CHINESE_S:     language_container = &language_chinese_s;     break;
    case SCE_SYSTEM_PARAM_LANG_FINNISH:       language_container = &language_finnish;       break;
    case SCE_SYSTEM_PARAM_LANG_SWEDISH:       language_container = &language_swedish;       break;
    case SCE_SYSTEM_PARAM_LANG_DANISH:        language_container = &language_danish;        break;
    case SCE_SYSTEM_PARAM_LANG_NORWEGIAN:     language_container = &language_norwegian;     break;
    case SCE_SYSTEM_PARAM_LANG_POLISH:        language_container = &language_polish;        break;
    case SCE_SYSTEM_PARAM_LANG_PORTUGUESE_BR: language_container = &language_portuguese_br; break;
    case SCE_SYSTEM_PARAM_LANG_ENGLISH_GB:    language_container = &language_english_gb;    break;
    case SCE_SYSTEM_PARAM_LANG_TURKISH:       language_container = &language_turkish;       break;
    default:                                  language_container = &language_english_us;    break;
  }
  if (msg && sceClibStrncmp(*msg, "msg_", 4) == 0) {
    #define LANGUAGE_ENTRY(name) \
      else if (sceClibStrncmp(*msg, #name, sizeof(#name)) == 0) { \
        return language_container->name; \
      }
    if (0) {}
    LANGUAGE_ENTRY(msg_henkaku_settings)
    LANGUAGE_ENTRY(msg_enable_psn_spoofing)
    LANGUAGE_ENTRY(msg_enable_unsafe_homebrew)
    LANGUAGE_ENTRY(msg_unsafe_homebrew_description)
    LANGUAGE_ENTRY(msg_enable_version_spoofing)
    LANGUAGE_ENTRY(msg_spoofed_version)
    LANGUAGE_ENTRY(msg_button_behavior)
    LANGUAGE_ENTRY(msg_button_enter)
    LANGUAGE_ENTRY(msg_button_cancel)
    LANGUAGE_ENTRY(msg_reload_taihen_config)
    LANGUAGE_ENTRY(msg_reload_taihen_config_success)
    LANGUAGE_ENTRY(msg_reboot_device)
    LANGUAGE_ENTRY(msg_content_downloader)
    LANGUAGE_ENTRY(msg_unlink_memory_card)
    LANGUAGE_ENTRY(msg_unlink_memory_card_success)
    LANGUAGE_ENTRY(msg_unlink_memory_card_error)
    #undef LANGUAGE_ENTRY
  }
  return TAI_CONTINUE(wchar_t *, g_scePafToplevelGetText_SceSystemSettingsCore_hook, arg, msg);
}

static SceUID g_system_settings_core_modid = -1;
static tai_hook_ref_t g_sceKernelLoadStartModule_SceSettings_hook;
static SceUID sceKernelLoadStartModule_SceSettings_patched(char *path, SceSize args, void *argp, int flags, SceKernelLMOption *option, int *status) {
  SceUID ret = TAI_CONTINUE(SceUID, g_sceKernelLoadStartModule_SceSettings_hook, path, args, argp, flags, option, status);
  if (ret >= 0 && sceClibStrncmp(path, "vs0:app/NPXS10015/system_settings_core.suprx", 44) == 0) {
    g_system_settings_core_modid = ret;
    g_hooks[7] = taiHookFunctionImport(&g_scePafToplevelInitPluginFunctions_SceSettings_hook, 
                                        "SceSettings", 
                                        0x4D9A9DD0, // ScePafToplevel
                                        0xF5354FEF, 
                                        scePafToplevelInitPluginFunctions_SceSettings_patched);
    g_hooks[8] = taiHookFunctionImport(&g_scePafMiscLoadXmlLayout_SceSettings_hook, 
                                        "SceSettings", 
                                        0x3D643CE8, // ScePafMisc
                                        0x19FE55A8, 
                                        scePafMiscLoadXmlLayout_SceSettings_patched);
    g_hooks[9] = taiHookFunctionImport(&g_sceRegMgrGetKeyInt_SceSystemSettingsCore_hook, 
                                        "SceSystemSettingsCore", 
                                        0xC436F916, // SceRegMgr
                                        0x16DDF3DC, 
                                        sceRegMgrGetKeyInt_SceSystemSettingsCore_patched);
    g_hooks[10] = taiHookFunctionImport(&g_sceRegMgrSetKeyInt_SceSystemSettingsCore_hook, 
                                        "SceSystemSettingsCore", 
                                        0xC436F916, // SceRegMgr
                                        0xD72EA399, 
                                        sceRegMgrSetKeyInt_SceSystemSettingsCore_patched);
    g_hooks[11] = taiHookFunctionImport(&g_sceRegMgrGetKeyStr_SceSystemSettingsCore_hook, 
                                        "SceSystemSettingsCore", 
                                        0xC436F916, // SceRegMgr
                                        0xE188382F, 
                                        sceRegMgrGetKeyStr_SceSystemSettingsCore_patched);
    g_hooks[12] = taiHookFunctionImport(&g_sceRegMgrSetKeyStr_SceSystemSettingsCore_hook, 
                                        "SceSystemSettingsCore", 
                                        0xC436F916, // SceRegMgr
                                        0x41D320C5, 
                                        sceRegMgrSetKeyStr_SceSystemSettingsCore_patched);
    g_hooks[13] = taiHookFunctionImport(&g_sceRegMgrGetKeysInfo_SceSystemSettingsCore_hook, 
                                        "SceSystemSettingsCore", 
                                        0xC436F916, // SceRegMgr
                                        0x58421DD1, 
                                        sceRegMgrGetKeysInfo_SceSystemSettingsCore_patched);
    g_hooks[14] = taiHookFunctionImport(&g_scePafToplevelGetText_SceSystemSettingsCore_hook, 
                                        "SceSystemSettingsCore", 
                                        0x4D9A9DD0, // ScePafToplevel
                                        0x19CEFDA7, 
                                        scePafToplevelGetText_SceSystemSettingsCore_patched);
  }
  return ret;
}

static tai_hook_ref_t g_sceKernelStopUnloadModule_SceSettings_hook;
static int sceKernelStopUnloadModule_SceSettings_patched(SceUID modid, SceSize args, void *argp, int flags, SceKernelULMOption *option, int *status) {
  if (modid == g_system_settings_core_modid) {
    g_system_settings_core_modid = -1;
    if (g_hooks[7] >= 0) taiHookRelease(g_hooks[7], g_scePafToplevelInitPluginFunctions_SceSettings_hook);
    if (g_hooks[8] >= 0) taiHookRelease(g_hooks[8], g_scePafMiscLoadXmlLayout_SceSettings_hook);
    if (g_hooks[9] >= 0) taiHookRelease(g_hooks[9], g_sceRegMgrGetKeyInt_SceSystemSettingsCore_hook);
    if (g_hooks[10] >= 0) taiHookRelease(g_hooks[10], g_sceRegMgrSetKeyInt_SceSystemSettingsCore_hook);
    if (g_hooks[11] >= 0) taiHookRelease(g_hooks[11], g_sceRegMgrGetKeyStr_SceSystemSettingsCore_hook);
    if (g_hooks[12] >= 0) taiHookRelease(g_hooks[12], g_sceRegMgrSetKeyStr_SceSystemSettingsCore_hook);
    if (g_hooks[13] >= 0) taiHookRelease(g_hooks[13], g_sceRegMgrGetKeysInfo_SceSystemSettingsCore_hook);
    if (g_hooks[14] >= 0) taiHookRelease(g_hooks[14], g_scePafToplevelGetText_SceSystemSettingsCore_hook);
  }
  return TAI_CONTINUE(int, g_sceKernelStopUnloadModule_SceSettings_hook, modid, args, argp, flags, option, status);
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args) {
  tai_module_info_t info;
  LOG("loading HENkaku config for user");
  load_config_user();
  g_hooks[0] = taiHookFunctionImport(&g_sceKernelLoadStartModule_SceSettings_hook, 
                                      "SceSettings", 
                                      0xCAE9ACE6, // SceLibKernel
                                      0x2DCC4AFA, 
                                      sceKernelLoadStartModule_SceSettings_patched);
  LOG("sceKernelLoadStartModule hook: %x", g_hooks[0]);
  g_hooks[1] = taiHookFunctionImport(&g_sceKernelStopUnloadModule_SceSettings_hook, 
                                      "SceSettings", 
                                      0xCAE9ACE6, // SceLibKernel
                                      0x2415F8A4, 
                                      sceKernelStopUnloadModule_SceSettings_patched);
  LOG("sceKernelStopUnloadModule hook: %x", g_hooks[1]);
  g_hooks[2] = taiHookFunctionImport(&g_sceKernelGetSystemSwVersion_SceSettings_hook, 
                                      "SceSettings", 
                                      0xEAED1616, // SceModulemgr
                                      0x5182E212, 
                                      sceKernelGetSystemSwVersion_SceSettings_patched);
  LOG("sceKernelGetSystemSwVersion hook: %x", g_hooks[2]);
  g_hooks[3] = g_hooks[4] = g_hooks[5] = -1;
  info.size = sizeof(info);
  if (taiGetModuleInfo("SceShell", &info) >= 0) {
    if (config.use_psn_spoofing) {
      // we don't have a nice clean way of doing PSN spoofing (update prompt disable) so 
      // we are stuck with hard coding offsets. Since module NID is different for each 
      // version and retail/dex/test unit, this should allow us to specify different 
      // offsets.
      switch (info.module_nid) {
        case 0x0552F692: { // retail 3.60 SceShell
          g_hooks[3] = taiHookFunctionOffset(&g_update_check_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x363de8,  // offset
                                             1,         // thumb
                                             update_check_patched);
          g_hooks[4] = taiHookFunctionOffset(&g_game_update_check_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x37beda,  // offset
                                             1,         // thumb
                                             game_update_check_patched);
          g_hooks[5] = taiHookFunctionOffset(&g_passphrase_decrypt_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x325230,  // offset
                                             1,         // thumb
                                             passphrase_decrypt_patched);
          break;
        }
        case 0xEAB89D5C: { // PTEL 3.60 SceShell thanks to CelesteBlue for offsets
          g_hooks[3] = taiHookFunctionOffset(&g_update_check_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x35A830,  // offset
                                             1,         // thumb
                                             update_check_patched);
          g_hooks[4] = taiHookFunctionOffset(&g_game_update_check_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x372832,  // offset
                                             1,         // thumb
                                             game_update_check_patched);
          g_hooks[5] = taiHookFunctionOffset(&g_passphrase_decrypt_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x31BC78,  // offset
                                             1,         // thumb
                                             passphrase_decrypt_patched);
          break;
        }
        case 0x6CB01295: { // PDEL 3.60 SceShell thanks to anonymous for offsets
          g_hooks[3] = taiHookFunctionOffset(&g_update_check_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x12c882,  // offset
                                             1,         // thumb
                                             update_check_patched);
          g_hooks[4] = taiHookFunctionOffset(&g_game_update_check_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x36df3e,  // offset
                                             1,         // thumb
                                             game_update_check_patched);
          g_hooks[5] = taiHookFunctionOffset(&g_passphrase_decrypt_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x317384,  // offset
                                             1,         // thumb
                                             passphrase_decrypt_patched);
          break;
        }
        case 0x5549BF1F:   // retail 3.65 SceShell
        case 0x34B4D82E:   // retail 3.67 SceShell
        case 0x12DAC0F3: { // retail 3.68 SceShell
          g_hooks[3] = taiHookFunctionOffset(&g_update_check_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x36422c,  // offset
                                             1,         // thumb
                                             update_check_patched);
          g_hooks[4] = taiHookFunctionOffset(&g_game_update_check_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x37c31e,  // offset
                                             1,         // thumb
                                             game_update_check_patched);
          g_hooks[5] = taiHookFunctionOffset(&g_passphrase_decrypt_hook, 
                                             info.modid, 
                                             0,         // segidx
                                             0x325674,  // offset
                                             1,         // thumb
                                             passphrase_decrypt_patched);
          break;
        }
        default: {
          LOG("SceShell NID %X not recognized, skipping PSN spoofing patches", info.module_nid);
        }
      }
    }
    // add patch to skip id.dat checks (only works if plugin loads before start)
    g_hooks[6] = taiHookFunctionImport(&g_SceVshBridge_333875AB_SceShell_hook, 
                                        "SceShell", 
                                        0x35C5ACD4, // SceVshBridge
                                        0x333875AB, 
                                        SceVshBridge_333875AB_SceShell_patched);
  } else {
    LOG("skipping psn spoofing patches");
  }
  return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
  LOG("stopping module");
  // free hooks that didn't fail
  if (g_hooks[0] >= 0) taiHookRelease(g_hooks[0], g_sceKernelLoadStartModule_SceSettings_hook);
  if (g_hooks[1] >= 0) taiHookRelease(g_hooks[1], g_sceKernelStopUnloadModule_SceSettings_hook);
  if (g_hooks[2] >= 0) taiHookRelease(g_hooks[2], g_sceKernelGetSystemSwVersion_SceSettings_hook);
  if (g_hooks[3] >= 0) taiHookRelease(g_hooks[3], g_update_check_hook);
  if (g_hooks[4] >= 0) taiHookRelease(g_hooks[4], g_game_update_check_hook);
  if (g_hooks[5] >= 0) taiHookRelease(g_hooks[5], g_passphrase_decrypt_hook);
  if (g_hooks[6] >= 0) taiHookRelease(g_hooks[6], g_SceVshBridge_333875AB_SceShell_hook);
  return SCE_KERNEL_STOP_SUCCESS;
}
