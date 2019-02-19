/**
 * @brief      Internal functions and defines
 */
#ifndef HENKAKU_HEADER
#define HENKAKU_HEADER

#include <stdint.h>

// TODO: Move to sdk
int sceClibPrintf(const char *fmt, ...);
int sceClibSnprintf(char *buf, size_t len, const char *fmt, ...);

int henkaku_reload_config(void);
int taiReloadConfig(void);

/** Logging function */
#ifdef ENABLE_LOGGING
#ifdef __VITA_KERNEL__
#define LOG(fmt, ...) printf("[%s:%d] " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define LOG(fmt, ...) sceClibPrintf("[%s:%d] " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif
#else
#define LOG(fmt, ...)
#endif

/** Config */
typedef struct {
  uint32_t magic;
  uint32_t version;
  uint32_t use_psn_spoofing;
  uint32_t allow_unsafe_hb;
  uint32_t use_spoofed_version;
  uint32_t spoofed_version;
} __attribute__((packed)) henkaku_config_t;

#define HENKAKU_CONFIG_MAGIC (0x4C434C4D)
#define CONFIG_PATH "ur0:tai/henkaku_config.bin"
#define OLD_CONFIG_PATH "ux0:temp/app_work/MLCL00001/rec/config.bin"
#define SPOOF_VERSION (0x3700000)

#endif // HENKAKU_HEADER
