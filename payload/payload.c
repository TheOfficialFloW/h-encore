#include <inttypes.h>

#include "bootstrap.h"

// ALL 3.65-3.68 SPECIFIC SECTIONS ARE MARKED WITH "// BEGIN 3.65-3.68"

#if DEBUG
#define LOG(fmt, ...) debug_print("[%s:%d] " fmt "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif

#define NULL ((void *)0)

#define DACR_OFF(stmt)                 \
do {                                   \
	unsigned prev_dacr;                \
	__asm__ volatile(                  \
		"mrc p15, 0, %0, c3, c0, 0 \n" \
		: "=r" (prev_dacr)             \
	);                                 \
	__asm__ volatile(                  \
		"mcr p15, 0, %0, c3, c0, 0 \n" \
		: : "r" (0xFFFF0000)           \
	);                                 \
	stmt;                              \
	__asm__ volatile(                  \
		"mcr p15, 0, %0, c3, c0, 0 \n" \
		: : "r" (prev_dacr)            \
	);                                 \
} while (0)

#define INSTALL_HOOK(func, addr) \
do {                                               \
	unsigned *target;                              \
	target = (unsigned*)(addr);                    \
	*target++ = 0xE59FF000; /* ldr pc, [pc, #0] */ \
	*target++; /* doesn't matter */                \
	*target = (unsigned)func;                      \
} while (0)

#define INSTALL_HOOK_THUMB(func, addr) \
do {                                                \
	unsigned *target;                                 \
	target = (unsigned*)(addr);                       \
	*target++ = 0xC004F8DF; /* ldr.w	ip, [pc, #4] */ \
	*target++ = 0xBF004760; /* bx ip; nop */          \
	*target = (unsigned)func;                         \
} while (0)

#define INSTALL_RET(addr, ret) \
do {                                            \
	unsigned *target;                             \
	target = (unsigned*)(addr);                   \
	*target++ = 0xe3a00000 | ret; /* mov r0, #ret */ \
	*target = 0xe12fff1e; /* bx lr */             \
} while (0)

#define INSTALL_RET_THUMB(addr, ret) \
do {                                            \
	unsigned *target;                             \
	target = (unsigned*)(addr);                   \
	*target = 0x47702000 | ret; /* movs r0, #ret; bx lr */ \
} while (0)

#define ENTER_SYSCALL(state) do { \
  __asm__ volatile ("mrc p15, 0, %0, c13, c0, 3" : "=r" (state)); \
  __asm__ volatile ("mcr p15, 0, %0, c13, c0, 3" :: "r" (state << 16) : "memory"); \
} while(0)

#define EXIT_SYSCALL(state) do { \
  __asm__ volatile ("mcr p15, 0, %0, c13, c0, 3" :: "r" (state) : "memory"); \
} while (0)

typedef uint64_t u64_t;
typedef uint32_t u32_t;
typedef uint16_t u16_t;
typedef uint8_t u8_t;

typedef struct segment_info
{
	int           size;   // this structure size (0x18)
	int           perms;  // probably rwx in low bits
	void            *vaddr; // address in memory
	int           memsz;  // size in memory
	int           flags;  // meanig unknown
	int           res;    // unused?
} segment_info_t;

typedef struct SceModInfo {
	int size;           //0
	int UID;            //4
	int mod_attr;       //8
	char name[0x1C];        //0xC
	u32_t unk0;         //0x28
	void *module_start;     //0x2C addr0
	void *module_init;      //0x30 addr1
	void *module_stop;      //0x34 addr2
	void *exidx_start;      //0x38 addr3
	void *exidx_end;        //0x3C addr4
	void *addr5;        //0x40 addr5
	void *addr6;        //0x44 addr6
	void *module_top;       //0x48 addr7
	void *addr8;        //0x4C addr8
	void *addr9;        //0x50 addr9
	char filepath[0x100];   //0x54
	segment_info_t  segments[4]; //0x58
	u32_t unk2;         //0x1B4
} SceModInfo; //0x1B8

#define MOD_LIST_SIZE 0x80

typedef struct module_imports_2
{
  u16_t size; // 0x24
  u16_t version;
  u16_t flags;
  u16_t num_functions;
  u32_t reserved1;
  u32_t lib_nid;
  char     *lib_name;
  u32_t *func_nid_table;
  void     **func_entry_table;
  u32_t unk1;
  u32_t unk2;
} module_imports_2_t;

typedef struct module_exports // thanks roxfan
{
	u16_t   size;           // size of this structure; 0x20 for Vita 1.x
	u8_t    lib_version[2]; //
	u16_t   attribute;      // ?
	u16_t   num_functions;  // number of exported functions
	u32_t   num_vars;       // number of exported variables
	u32_t   num_tls_vars;   // number of exported TLS variables?  <-- pretty sure wrong // yifanlu
	u32_t   module_nid;     // NID of this specific export list; one PRX can export several names
	char    *lib_name;      // name of the export module
	u32_t   *nid_table;     // array of 32-bit NIDs for the exports, first functions then vars
	void    **entry_table;  // array of pointers to exported functions and then variables
} module_exports_t;

typedef struct module_info // thanks roxfan
{
	u16_t   modattribute;  // ??
	u16_t   modversion;    // always 1,1?
	char    modname[27];   ///< Name of the module
	u8_t    type;          // 6 = user-mode prx?
	void    *gp_value;     // always 0 on ARM
	int   ent_top;       // beginning of the export list (sceModuleExports array)
	int   ent_end;       // end of same
	int   stub_top;      // beginning of the import list (sceModuleStubInfo array)
	int   stub_end;      // end of same
	u32_t   module_nid;    // ID of the PRX? seems to be unused
	int   field_38;      // unused in samples
	int   field_3C;      // I suspect these may contain TLS info
	int   field_40;      //
	int   mod_start;     // 44 module start function; can be 0 or -1; also present in exports
	int   mod_stop;      // 48 module stop function
	int   exidx_start;   // 4c ARM EABI style exception tables
	int   exidx_end;     // 50
	int   extab_start;   // 54
	int   extab_end;     // 58
} module_info_t; // 5c?

int strcmp(const char *s1, const char *s2) {
	while (*s1 == *s2++)
		if (*s1++ == 0)
			return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}

static inline int memcpy(void *dst, const void *src, int len) {
	char *dst1 = (char *)dst;
	const char *src1 = (const char *)src;
	while (len > 0) {
		*dst1++ = *src1++;
		len--;
	}
	return 0;
}

module_info_t * find_modinfo(uint32_t start_addr, const char *needle) {
	start_addr &= ~0xF;
	start_addr += 4;

	while (1) {
		if (strcmp((const char *)start_addr, needle) == 0)
			 return (module_info_t *)(start_addr - 0x4);

		start_addr += 0x10;
	}

	return NULL;
}

void *find_export(module_info_t *mod, uint32_t nid) {
	if (!mod->ent_top)
		return NULL;

	for (unsigned ent = mod->ent_top; ent < mod->ent_end;) {
		module_exports_t *exports = (module_exports_t *)((unsigned int)mod + 0x5C + ent - mod->ent_top);

		for (int j = 0; j < exports->num_functions; ++j)
			if (exports->nid_table[j] == nid)
				return (void*)((u32_t *)exports->entry_table)[j];

		ent += exports->size;
	}

	return NULL;
}

void *find_import(module_info_t *mod, uint32_t lnid, uint32_t fnid) {
	if (!mod->stub_top)
		return NULL;

	for (unsigned stub = mod->stub_top; stub < mod->stub_end;) {
		// Older fw versions use a different import format
		module_imports_2_t *imports = (module_imports_2_t *)((unsigned int)mod + 0x5C + stub - mod->stub_top);

		if (imports->size == sizeof(module_imports_2_t) && imports->lib_nid == lnid) {
			for (int j = 0; j < imports->num_functions; ++j)
				if (imports->func_nid_table[j] == fnid)
					return (void*)((u32_t *)imports->func_entry_table)[j];
		}

		stub += imports->size;
	}

	return NULL;
}

static void (*debug_print)(char *fmt, ...) = 0;

static int (*hook_resume_sbl_F3411881)() = 0;
static int (*hook_resume_sbl_89CCDA2C)() = 0;
static int (*hook_resume_sbl_BC422443)() = 0;

static int (*ksceKernelGetModuleList)() = 0;
static int (*ksceKernelGetModuleInfo)() = 0;
static void (*ksceKernelCpuIcacheAndL2WritebackInvalidateRange)(uint32_t addr, uint32_t size) = 0;
static int (*ksceKernelCpuDcacheWritebackRange)(uint32_t addr, uint32_t len) = 0;
static int (*ksceIoOpen)(const char *, int, int) = 0;
static int (*ksceIoWrite)(int, char *, int) = 0;
static int (*ksceIoClose)(int) = 0;
static int (*ksceAppMgrLaunchAppByPath)(const char *name, const char *cmd, int cmdlen, int, void *, void *) = 0;
static int (*ksceAppMgrDestroyOtherAppByPid)(int pid) = 0;
static int (*ksceKernelLoadModule)(const char *path, int flags, int *opt) = 0;
static int (*ksceKernelStartModule)(int modid, int argc, void *args, int flags, void *opt, int *res) = 0;
static void (*ksceKernelSetSyscall)(u32_t num, void *function) = 0;
static int (*ksceKernelFreeMemBlock)(int blkid) = 0;
static int (*ksceKernelFindMemBlockByAddr)(void *base, int) = 0;
static int (*ksceKernelCreateThread)() = 0;
static int (*ksceKernelStartThread)() = 0;
static int (*ksceKernelExitDeleteThread)() = 0;
static int (*ksceKernelGetMemBlockBase)(int uid, void **base) = 0;
static int (*ksceKernelGetProcessInfo)(int pid, int *data) = 0;
static int (*ksceKernelGetProcessLocalStorageAddrForPid)(int pid, int key, void **out_addr, int create_if_doesnt_exist) = 0;
static int (*ksceCtrlPeekBufferPositive)(int port, void *pad_data, int count) = 0;

// context for the hooks
static unsigned g_homebrew_decrypt = 0;
static module_info_t *modulemgr_info = 0;
static module_info_t *scenpdrm_info = 0;
static module_info_t *appmgr_info = 0;
static u32_t appmgr_code = 0;
static u32_t appmgr_size = 0;
static u32_t appmgr_data = 0;
static u32_t ngs_data = 0;

// save the block id of our own memory
static int g_rx_block = 0;
static void *syscall_stub = 0;
static void **syscall_table = 0;
static int syscall_id = 0;

// shell
static int shell_pid = 0;
static int app_pid = 0;

int hook_SceSblAIMgrForDriver_D78B04A2(void)
{
	return 1;
}

int hook_SceSblAIMgrForDriver_F4B98F66(void)
{
	return 1;
}

int hook_sysroot_421EFC96(void)
{
	return 0;
}

int hook_ksceSblAimgrIsCEX(void)
{
	return 0;
}

// setup file decryption
unsigned hook_sbl_F3411881(unsigned a1, unsigned a2, unsigned a3, unsigned a4) {
	LOG("sbl_F3411881(0x%x, 0x%x, 0x%x, 0x%x)", a1, a2, a3, a4);

	unsigned res = hook_resume_sbl_F3411881(a1, a2, a3, a4);
	LOG("sbl_F3411881: %x", res);
	unsigned *somebuf = (unsigned*)a4;
	u64_t authid;

	if (res == 0x800f0624 || res == 0x800f0616 || res == 0x800f0024 || (res >= 0x800f0b30 && res <= 0x800f0b3f)) {
		DACR_OFF(
			g_homebrew_decrypt = 1;
		);
	// BEGIN 3.65-3.68
		somebuf[42] = 0x40;
	// END 3.65-3.68

		return 0;
	} else {
		DACR_OFF(
			g_homebrew_decrypt = 0;
		);
	}
	return res;
}

// setup output buffer
unsigned hook_sbl_89CCDA2C(unsigned a1, unsigned a2) {
	LOG("sbl_89CCDA2C(0x%x, 0x%x) hb=0x%x", a1, a2, g_homebrew_decrypt);
	if (g_homebrew_decrypt == 1)
		return 2; // always compressed!
	return hook_resume_sbl_89CCDA2C(a1, a2);
}

// decrypt
unsigned hook_sbl_BC422443(unsigned a1, unsigned a2, unsigned a3) {
	LOG("sbl_BC422443(0x%x, 0x%x, 0x%x) hb=0x%x", a1, a2, a3, g_homebrew_decrypt);
	if (g_homebrew_decrypt == 1)
		return 0;
	return hook_resume_sbl_BC422443(a1, a2, a3);
}

static const char ur0_temp_path[] = "ur0:temp";

static int has_sigpatches = 0;
static char old_sbl_F3411881[16] = {0};
static char old_sbl_89CCDA2C[16] = {0};
static char old_sbl_BC422443[16] = {0};
static char old_sblai_D78B04A2[16] = {0};
static char old_sblai_F4B98F66[16] = {0};
static char old_sysroot_421EFC96[16] = {0};
static char old_ksceSblAimgrIsCEX[16] = {0};
static char old_ux0_data_path[9] = {0};
static void *ux0_data_path_addr = 0;

void temp_pkgpatches(void) {
	void *addr;
	LOG("inserting temporary patches");
	addr = find_import(scenpdrm_info, 0xFD00C69A, 0xD78B04A2);
	LOG("sblai_D78B04A2 stub: %p", addr);
	DACR_OFF(
		memcpy(old_sblai_D78B04A2, addr, 16);
		INSTALL_HOOK(hook_SceSblAIMgrForDriver_D78B04A2, addr);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("hooked sblai_D78B04A2");
	addr = find_import(scenpdrm_info, 0xFD00C69A, 0xF4B98F66);
	LOG("sblai_F4B98F66 stub: %p", addr);
	DACR_OFF(
		memcpy(old_sblai_F4B98F66, addr, 16);
		INSTALL_HOOK(hook_SceSblAIMgrForDriver_F4B98F66, addr);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("hooked sblai_F4B98F66");

	__asm__ volatile ("isb" ::: "memory");
}

void remove_pkgpatches(void) {
	void *addr;
	LOG("removing temporary patches");
	addr = find_import(scenpdrm_info, 0xFD00C69A, 0xD78B04A2);
	LOG("sblai_D78B04A2 stub: %p", addr);
	DACR_OFF(
		memcpy(addr, old_sblai_D78B04A2, 16);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("unhooked sblai_D78B04A2");
	addr = find_import(scenpdrm_info, 0xFD00C69A, 0xF4B98F66);
	LOG("sblai_F4B98F66 stub: %p", addr);
	DACR_OFF(
		memcpy(addr, old_sblai_F4B98F66, 16);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("unhooked sblai_F4B98F66");

	__asm__ volatile ("isb" ::: "memory");
}

void temp_sigpatches(void) {
	void *addr;
	LOG("inserting temporary patches");
	addr = find_import(modulemgr_info, 0x7ABF5135, 0xF3411881);
	LOG("sbl_F3411881 stub: %p\n", addr);
	DACR_OFF(
		memcpy(old_sbl_F3411881, addr, 16);
		INSTALL_HOOK(hook_sbl_F3411881, addr);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("hooked sbl_F3411881");
	addr = find_import(modulemgr_info, 0x7ABF5135, 0x89CCDA2C);
	LOG("sbl_89CCDA2C stub: %p", addr);
	DACR_OFF(memcpy(old_sbl_89CCDA2C, addr, 16));
	DACR_OFF(
		memcpy(old_sbl_89CCDA2C, addr, 16);
		INSTALL_HOOK(hook_sbl_89CCDA2C, addr);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("hooked sbl_89CCDA2C");
	addr = find_import(modulemgr_info, 0x7ABF5135, 0xBC422443);
	LOG("sbl_BC422443 stub: %p", addr);
	DACR_OFF(
		memcpy(old_sbl_BC422443, addr, 16);
		INSTALL_HOOK(hook_sbl_BC422443, addr);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("hooked sbl_BC422443");
	addr = find_import(appmgr_info, 0x2ED7F97A, 0x421EFC96);
	LOG("sysroot_421EFC96 stub: %p", addr);
	DACR_OFF(
		memcpy(old_sysroot_421EFC96, addr, 16);
		INSTALL_HOOK(hook_sysroot_421EFC96, addr);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("hooked sysroot_421EFC96");

	// To bypass 0x8080004C error on FW 3.60+
	addr = find_import(appmgr_info, 0xFD00C69A, 0xD78B04A2);
	LOG("ksceSblAimgrIsCEX stub: %p", addr);
	if (addr) {
		DACR_OFF(
			memcpy(old_ksceSblAimgrIsCEX, addr, 16);
			INSTALL_HOOK(hook_ksceSblAimgrIsCEX, addr);
			ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
			ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
		);
		LOG("hooked ksceSblAimgrIsCEX");
	}

	DACR_OFF(
		memcpy(old_ux0_data_path, ux0_data_path_addr, sizeof(old_ux0_data_path));
		memcpy(ux0_data_path_addr, ur0_temp_path, sizeof(ur0_temp_path));
	);
    LOG("hooked ux0:data path");

	DACR_OFF(has_sigpatches = 1);

	__asm__ volatile ("isb" ::: "memory");
}

void remove_sigpatches(void) {
	void *addr;
	if (!has_sigpatches) return;
	LOG("removing temporary patches");
	addr = find_import(modulemgr_info, 0x7ABF5135, 0xF3411881);
	LOG("sbl_F3411881 stub: %p", addr);
	DACR_OFF(
		memcpy(addr, old_sbl_F3411881, 16);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("unhooked sbl_F3411881");
	addr = find_import(modulemgr_info, 0x7ABF5135, 0x89CCDA2C);
	LOG("sbl_89CCDA2C stub: %p", addr);
	DACR_OFF(
		memcpy(addr, old_sbl_89CCDA2C, 16);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("unhooked sbl_89CCDA2C");
	addr = find_import(modulemgr_info, 0x7ABF5135, 0xBC422443);
	LOG("sbl_BC422443 stub: %p", addr);
	DACR_OFF(
		memcpy(addr, old_sbl_BC422443, 16);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("unhooked sbl_BC422443");
	addr = find_import(appmgr_info, 0x2ED7F97A, 0x421EFC96);
	LOG("sysroot_421EFC96 stub: %p", addr);
	DACR_OFF(
		memcpy(addr, old_sysroot_421EFC96, 16);
		ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
		ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
	);
	LOG("unhooked sysroot_421EFC96");
	addr = find_import(appmgr_info, 0xFD00C69A, 0xD78B04A2);
	LOG("ksceSblAimgrIsCEX stub: %p", addr);
	if (addr) {
		DACR_OFF(
			memcpy(addr, old_ksceSblAimgrIsCEX, 16);
			ksceKernelCpuIcacheAndL2WritebackInvalidateRange((uint32_t)addr & ~0x1F, 0x20);
			ksceKernelCpuDcacheWritebackRange((uint32_t)addr & ~0x1F, 0x20);
		);
		LOG("unhooked ksceSblAimgrIsCEX");
	}

	DACR_OFF(
		memcpy(ux0_data_path_addr, old_ux0_data_path, sizeof(old_ux0_data_path));
	);
	LOG("unhooked ux0:data path");

	DACR_OFF(has_sigpatches = 0);

	__asm__ volatile ("isb" ::: "memory");
}

static void find_ux0_data_path_addr() {
	char *from = (char *)(appmgr_code + appmgr_info->stub_top);
	char *to = (char *)(appmgr_code + appmgr_size);
	while (strcmp(from, "ux0:data") != 0 && from < (to - 8))
		from += 4;
	DACR_OFF(ux0_data_path_addr = from);
}

static void get_shell_and_app_pid(int *shell_pid, int *app_pid) {
	unsigned data[0xE8/4];
	int ret, pid, ppid;

	data[0] = sizeof(data);
	ret = ksceKernelGetProcessInfo(0, data);
	pid = data[1];
	ppid = data[5];
	LOG("ret: %x, ppid: %x", ret, ppid);
	*shell_pid = ppid;
	*app_pid = pid;
}

int load_taihen(void) {
  int state;
	int opt, taiid, modid, ret, result;

  ENTER_SYSCALL(state);

	// load taiHEN
	opt = 4;
	taiid = ksceKernelLoadModule("ur0:tai/taihen.skprx", 0, &opt);
	LOG("LoadTaiHEN: 0x%08X", taiid);
	remove_sigpatches();
	LOG("Removed temp patches");
	result = 0;
	ret = ksceKernelStartModule(taiid, 0, NULL, 0, NULL, &result);
	LOG("StartTaiHEN: 0x%08X, 0x%08X", ret, result);
	if (ret == 0) {
		ret = result;
	}
	if (ret < 0) {
		goto end;
	}

	// load henkaku kernel
	modid = ksceKernelLoadModule("ur0:tai/henkaku.skprx", 0, &opt);
	LOG("LoadHENKaku kernel: 0x%08X", modid);
	result = 0;
	ret = ksceKernelStartModule(modid, 4, &shell_pid, 0, NULL, &result);
	LOG("StartHENkaku kernel: 0x%08X, 0x%08X", ret, result);
	if (ret == 0) {
		ret = result;
	}
	if (ret < 0) {
		goto end;
	}

end:
	EXIT_SYSCALL(state);
	return ret;
}

static void __attribute__((noinline, naked)) free_and_exit(int blk, void *free, void *lr) {
	// now free the executable memory. this frees our current function so we have
	// to ensure we do not return here
	__asm__ volatile ("mov lr, %0\n"
										"bx r1" :: "r" (lr) : "lr");
}

void cleanup_memory(void) {
	void *lr;
	__asm__ volatile ("mov %0, lr" : "=r" (lr));
	LOG("calling cleanup from %x", lr);
	// TODO: Delete bootstrap.self (#37)
	// remove syscalls
	LOG("removing syscalls");
	ksceKernelSetSyscall(syscall_id + 0, syscall_stub);
	ksceKernelSetSyscall(syscall_id + 1, syscall_stub);
	ksceKernelSetSyscall(syscall_id + 2, syscall_stub);
	ksceKernelSetSyscall(syscall_id + 3, syscall_stub);
	ksceKernelSetSyscall(syscall_id + 4, syscall_stub);
	LOG("freeing executable memory");
	return free_and_exit(g_rx_block, ksceKernelFreeMemBlock, lr);
}

int mount_savedata(int pid) {
	char mount_point[16];
	void *info;

	void *(* sceAppMgrFindProcessInfoByPid)(void *data, int pid);
	int (* sceAppMgrMountById)(int pid, void *info, int id, const char *titleid, const char *path,
		const char *desired_mount_point, const void *klicensee, char *mount_point);

	switch (appmgr_info->module_nid) {
		case 0x1C9879D6: // 3.65 retail
			sceAppMgrFindProcessInfoByPid = (void *)(appmgr_code + 0x2DE1);
			sceAppMgrMountById = (void *)(appmgr_code + 0x19E61);
			break;

		case 0x54E2E984: // 3.67 retail
		case 0xC3C538DE: // 3.68 retail
			sceAppMgrFindProcessInfoByPid = (void *)(appmgr_code + 0x2DE1);
			sceAppMgrMountById = (void *)(appmgr_code + 0x19E6D);
			break;

		default:
			return -1;
	}

	info = sceAppMgrFindProcessInfoByPid((void *)(appmgr_data + 0x500), pid);
	if (!info)
		return -1;

	return sceAppMgrMountById(pid, info + 0x580, 0x3ED, "PCSG90096", "ux0:user/00/savedata/PCSG90096", "savedata0:", NULL, mount_point);
}

/* Install path and arguments */
const char launch_path_ur[] = "ur0:/temp/bootstrap.self";
const char launch_path_ux[] = "ux0:/data/bootstrap.self";
const char launch_args[] = "\0\0\0\0-nonsuspendable\0-livearea_off\0";

static int exists(const char *path) {
	int fd = ksceIoOpen(path, 1, 0);
	if (fd < 0)
		return 0;
	ksceIoClose(fd);
	return 1;
}

int thread_main(int args, void *argp) {
	char real_args[sizeof(launch_args)];
	int opt[52/4];
	int ctx[16/4];
	unsigned pad[0x20/4];
	const char *launch_path;
	int fd;
	int ret;

	void *lr;
	__asm__ volatile ("mov %0, lr" : "=r" (lr));

	LOG("Main kernel thread, called from %x!", lr);

	memcpy(real_args, launch_args, sizeof(launch_args));
	*(uint16_t *)&real_args[0] = syscall_id;

	ksceCtrlPeekBufferPositive(0, pad, 1);
	if ((pad[2] & (0x200 | 0x800)) ||
		!exists("ur0:tai/henkaku.suprx") ||
		!exists("ur0:tai/henkaku.skprx") ||
		!exists("ur0:tai/taihen.skprx") ||
		(!exists("ur0:tai/config.txt") &&
		!exists("ux0:tai/config.txt"))) {
		LOG("Loading bootstrap to system");
		launch_path = launch_path_ux;
		ret = fd = ksceIoOpen(launch_path, 0x603, 0x6);
		LOG("ux ksceIoOpen: %x", fd);
		if (fd < 0) {
			launch_path = launch_path_ur;
			fd = ksceIoOpen(launch_path, 0x603, 0x6);
			LOG("ur ksceIoOpen: %x", fd);
		}
		if (fd >= 0) {
			ret = ksceIoWrite(fd, bootstrap_self, bootstrap_self_len);
			LOG("ksceIoWrite: %x", ret);
			ksceIoClose(fd);

			for (int i = 0; i < sizeof(opt)/4; i++) {
				opt[i] = 0;
			}
			opt[0] = sizeof(opt);
			LOG("Launching bootstrap...");
			ret = ksceAppMgrLaunchAppByPath(launch_path, real_args, sizeof(launch_args), 0, opt, NULL);
			LOG("ksceAppMgrLaunchAppByPath: %x", ret);
		}
		if (ret < 0) {
			LOG("unable to write bootstrap!");
			remove_sigpatches();
			remove_pkgpatches();
			__asm__ volatile ("mov lr, %0\n"
												"mov r0, %1\n"
												"bx r0\n" :: "r" (lr), "r" (cleanup_memory) : "r0", "lr");
			LOG("should not be here!");
			while (1);
		}
	} else {
		ksceAppMgrDestroyOtherAppByPid(app_pid);
		remove_pkgpatches();
		load_taihen();
		__asm__ volatile ("mov lr, %0\n"
											"mov r0, %1\n"
											"bx r0\n" :: "r" (lr), "r" (cleanup_memory) : "r0", "lr");
		LOG("should not be here!");
		while (1);
	}

	LOG("done with kernel thread!");
	return 0;
}

int add_syscalls(void) {
	ksceKernelSetSyscall(syscall_id + 0, load_taihen);
	ksceKernelSetSyscall(syscall_id + 1, remove_pkgpatches);
	ksceKernelSetSyscall(syscall_id + 2, remove_sigpatches);
	ksceKernelSetSyscall(syscall_id + 3, cleanup_memory);
	ksceKernelSetSyscall(syscall_id + 4, mount_savedata);
	return 0;
}

void resolve_imports(unsigned sysmem_base) {
	module_info_t *sysmem_info = find_modinfo(sysmem_base, "SceSysmem");
	u32_t modulemgr_base;

	// BEGIN 3.65-3.68 specific offsets here, used to find Modulemgr from just sysmem base
	LOG("sysmem base: 0x%08x", sysmem_base);
	void *sysmem_data = (void*)(*(u32_t*)((u32_t)(sysmem_base) + 0x26a28) - 0xA0);
	LOG("sysmem data base: 0x%08x", sysmem_data);
	modulemgr_base = (*(u32_t*)((u32_t)(sysmem_data) + 0x438c) - 0x40);
	LOG("modulemgr base: 0x%08x", modulemgr_base);
	// END 3.65-3.68 specific offsets

	DACR_OFF(modulemgr_info = find_modinfo((u32_t)modulemgr_base, "SceKernelModulemgr"));
	LOG("modulemgr modinfo: 0x%08x", modulemgr_info);

	DACR_OFF(
		ksceKernelGetModuleList = find_export(modulemgr_info, 0xB72C75A4);
		ksceKernelGetModuleInfo = find_export(modulemgr_info, 0xDAA90093);
	);

	LOG("ksceKernelGetModuleList: %08x", ksceKernelGetModuleList);
	LOG("ksceKernelGetModuleInfo: %08x", ksceKernelGetModuleInfo);

	int *modlist[MOD_LIST_SIZE];
	int modlist_records;
	SceModInfo info;
	int ret;

	modlist_records = MOD_LIST_SIZE;
	ret = ksceKernelGetModuleList(0x10005, 0x7FFFFFFF, 1, modlist, &modlist_records);
	LOG("sceKernelGetModuleList() returned 0x%x", ret);
	LOG("modlist_records: %d", modlist_records);
	module_info_t *threadmgr_info = 0, *sblauthmgr_info = 0, *processmgr_info = 0, *ctrl_info = 0, *iofilemgr_info = 0;
	u32_t modulemgr_data = 0;
	for (int i = 0; i < modlist_records; ++i) {
		info.size = sizeof(info);
		ret = ksceKernelGetModuleInfo(0x10005, modlist[i], &info);
		if (strcmp(info.name, "SceKernelThreadMgr") == 0) {
			threadmgr_info = find_modinfo((u32_t)info.segments[0].vaddr, "SceKernelThreadMgr");
		}
		if (strcmp(info.name, "SceSblAuthMgr") == 0) {
			sblauthmgr_info = find_modinfo((u32_t)info.segments[0].vaddr, "SceSblAuthMgr");
		}
		if (strcmp(info.name, "SceNpDrm") == 0) {
			DACR_OFF(scenpdrm_info = find_modinfo((u32_t)info.segments[0].vaddr, "SceNpDrm"));
		}
		if (strcmp(info.name, "SceKernelModulemgr") == 0) {
			modulemgr_data = (u32_t)info.segments[1].vaddr;
		}
		if (strcmp(info.name, "SceAppMgr") == 0) {
			DACR_OFF(appmgr_info = find_modinfo((u32_t)info.segments[0].vaddr, "SceAppMgr"));
			DACR_OFF(appmgr_code = (u32_t)info.segments[0].vaddr);
			DACR_OFF(appmgr_size = (u32_t)info.segments[0].memsz);
			DACR_OFF(appmgr_data = (u32_t)info.segments[1].vaddr);
		}
		if (strcmp(info.name, "SceIofilemgr") == 0) {
			iofilemgr_info = find_modinfo((u32_t)info.segments[0].vaddr, "SceIofilemgr");
		}
		if (strcmp(info.name, "SceProcessmgr") == 0) {
			processmgr_info = find_modinfo((u32_t)info.segments[0].vaddr, "SceProcessmgr");
		}
		if (strcmp(info.name, "SceCtrl") == 0) {
			ctrl_info = find_modinfo((u32_t)info.segments[0].vaddr, "SceCtrl");
		}
		if (strcmp(info.name, "SceNgs") == 0) {
			DACR_OFF(ngs_data = (u32_t)info.segments[1].vaddr);
		}
	}

	LOG("threadmgr_info: 0x%08x | sblauthmgr_info: 0x%08x | scenpdrm_info: 0x%08x", threadmgr_info, sblauthmgr_info, scenpdrm_info);

	DACR_OFF(
		hook_resume_sbl_F3411881 = find_export(sblauthmgr_info, 0xF3411881);
		hook_resume_sbl_89CCDA2C = find_export(sblauthmgr_info, 0x89CCDA2C);
		hook_resume_sbl_BC422443 = find_export(sblauthmgr_info, 0xBC422443);

		ksceKernelCpuIcacheAndL2WritebackInvalidateRange = find_export(sysmem_info, 0x73E895EA);
		ksceKernelCpuDcacheWritebackRange = find_export(sysmem_info, 0x9CB9F0CE);
		ksceIoOpen = find_export(iofilemgr_info, 0x75192972);
		ksceIoClose = find_export(iofilemgr_info, 0xf99dd8a3);
		ksceIoWrite = find_export(iofilemgr_info, 0x21ee91f0);
		ksceAppMgrLaunchAppByPath = find_export(appmgr_info, 0xB0A37065);
		ksceAppMgrDestroyOtherAppByPid = find_export(appmgr_info, 0xFC89D33D);
		ksceKernelLoadModule = find_export(modulemgr_info, 0x86D8D634);
		ksceKernelStartModule = find_export(modulemgr_info, 0x0675B682);
		ksceKernelSetSyscall = find_export(modulemgr_info, 0x2E4A10A0);
		ksceKernelFreeMemBlock = find_export(sysmem_info, 0x9e1c61);
		ksceKernelFindMemBlockByAddr = find_export(sysmem_info, 0x8a1742f6);
		ksceKernelCreateThread = find_export(threadmgr_info, 0xC6674E7D);
		ksceKernelStartThread = find_export(threadmgr_info, 0x21F5419B);
		ksceKernelExitDeleteThread = find_export(threadmgr_info, 0x1D17DECF);
		ksceKernelGetMemBlockBase = find_export(sysmem_info, 0xA841EDDA);
		ksceKernelGetProcessInfo = find_export(processmgr_info, 0x0AFF3EAE);
		ksceKernelGetProcessLocalStorageAddrForPid = find_export(processmgr_info, 0xAF80F39C);
		ksceCtrlPeekBufferPositive = find_export(ctrl_info, 0xEA1D3A34);
	);

	// BEGIN 3.65-3.68
	int *syscall_lo = (int *)(modulemgr_data + 0x2038c);
	DACR_OFF(
		syscall_table = (void **) (*((u32_t*)(modulemgr_data + 0x20388)));
		syscall_id = (*syscall_lo & 0xFFF) | 1; // id must not be x00
		syscall_stub = (void *)(modulemgr_base + 0x9fc5);
	);
	*syscall_lo = syscall_id + 5;
	// END 3.65-3.68
}

void fix_ngs() {
	int key;
	void *out_addr;
	uint32_t block_info;
	uint32_t num_blocks;
	uint32_t *blocks;
	unsigned data[0xE8/4];
	int pid;

	data[0] = sizeof(data);
	ksceKernelGetProcessInfo(0, data);
	pid = data[1];

	key = *(uint32_t *)(ngs_data + 0xC60 + 0x10);
	ksceKernelGetProcessLocalStorageAddrForPid(pid, key, &out_addr, 1);

	if (out_addr) {
		block_info = *(uint32_t *)(out_addr + 0x320);
		if (block_info) {
			num_blocks = *(uint32_t *)(block_info + 0x8);
			blocks = *(uint32_t **)(block_info + 0x24);

			for (int i = 0; i < num_blocks; i++) {
				blocks[i] = 0;
			}

			*(uint32_t *)(block_info + 0x8) = 0;
		}
	}
}

void flash_screen(uint32_t framebuf, uint32_t color) {
	uint32_t prev_dacr;
	uint32_t base;
	int pitch, height;
	int x, y;

	__asm__ volatile ("mrc p15, 0, %0, c3, c0, 0" : "=r" (prev_dacr));
	__asm__ volatile ("mcr p15, 0, %0, c3, c0, 0" :: "r" (0x55555555));

	__asm__ volatile ("ldrt %0, [%1]" : "=r" (base) : "r" (framebuf + 0x04));
	__asm__ volatile ("ldrt %0, [%1]" : "=r" (pitch) : "r" (framebuf + 0x08));
	__asm__ volatile ("ldrt %0, [%1]" : "=r" (height) : "r" (framebuf + 0x14));

	for (y = 0; y < height; y++) {
		for (x = 0; x < pitch; x++) {
			__asm__ volatile ("strt %0, [%1]" :: "r" (color), "r" (base += 4));
		}
	}

	__asm__ volatile ("mcr p15, 0, %0, c3, c0, 0" :: "r" (prev_dacr));	
}

void start() __attribute__ ((weak, alias ("payload")));
void _start() __attribute__ ((weak, alias ("payload")));
void __attribute__ ((section (".text.start"))) payload(void *rx_block, uint32_t sysmem_addr, uint32_t framebuf) {
	// find sysmem base, etc
	uint32_t sysmem_base = sysmem_addr;
	int ret;
	// BEGIN 3.65-3.68
	void (*debug_print_local)(char *s, ...) = (void*)(sysmem_base + 0x1A155);
	// END 3.65-3.68

	DACR_OFF(
		debug_print = debug_print_local;
	);

	int state;
	ENTER_SYSCALL(state);

	// flash purple
	flash_screen(framebuf, 0xFFFF00FF);

	LOG("+++ Entered kernel payload +++");
	LOG("payload=0x%x, size=0x%x, sp=0x%x", payload, rx_size, &ret);

	LOG("resolving imports");
	resolve_imports(sysmem_base);

	LOG("fixing ngs");
	fix_ngs();

	LOG("set up syscalls starting at id: %x", syscall_id);
	add_syscalls();

	LOG("getting shell and app pid");
	DACR_OFF(get_shell_and_app_pid(&shell_pid, &app_pid));

	LOG("adding temporary patches");
	find_ux0_data_path_addr();
	temp_pkgpatches();
	temp_sigpatches();

	LOG("find rx block");
	DACR_OFF(
		g_rx_block = ksceKernelFindMemBlockByAddr(rx_block, 0);
	);

	LOG("flush changes");
	void *base;
	ret = ksceKernelGetMemBlockBase(g_rx_block, &base);
	LOG("ksceKernelGetMemBlockBase: %x, %x", ret, base);
	ksceKernelCpuDcacheWritebackRange((uint32_t)base, 0x1000);

	int tid;
	LOG("starting kernel thread");
	tid = ksceKernelCreateThread("stage2", thread_main, 64, 0x1000, 0, 0, 0);
	LOG("create tid: %x", tid);
	ret = ksceKernelStartThread(tid, 0, NULL);
	LOG("start thread: %x", ret);

	EXIT_SYSCALL(state);

	LOG("killing self");
	ksceKernelExitDeleteThread(0);

	LOG("should not be here!");
	while(1) {}
}
