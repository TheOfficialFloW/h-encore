/* gadgets.S -- rop gadgets
 *
 * Copyright (C) 2018 TheFloW
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

// call
.equ blx_r0_add_sp_4_pop_pc,            0x8102a2f3
.equ blx_r0_add_sp_14_pop_pc,           0x8102a3d3
.equ blx_r1_add_sp_4_pop_pc,            0x81005579
.equ blx_r2_add_sp_c_pop_pc,            0x8100a107
.equ blx_r3_add_sp_14_pop_pc,           0x8100a16b
.equ blx_lr_add_sp_14_pop_pc,           0x8100a1cb

// load
.equ pop_pc,                            0x810002c1
.equ pop_r0_r1_r2_r4_pc,                0x810321ed
.equ pop_r0_r1_r2_r3_r4_r5_r7_pc,       0x810321fd
.equ pop_r3_r5_pc,                      0x81021795
.equ pop_r4_pc,                         0x81000065
.equ pop_r4_r5_r6_r7_r8_sb_pc,          0x8102675d
.equ ldm_sp_r0_r1_add_sp_c_pop_pc,      0x81026bad
.equ ldm_r0_r0_r2_ip_sp_lr_pc,          0x8102aac0 // arm
.equ ldm_r8_r0_r1_r4_r5_sl_ip_lr_pc,    0x8103a66c // arm
.equ ldr_r0_sp_add_sp_4_pop_pc,         0x81000bf5
.equ ldr_r0_r0_bx_lr,                   0x81005843

// store
.equ str_r0_r1_add_sp_4_pop_pc,         0x8100717b

// move
.equ mov_r0_r4_blx_lr,                  0x8100914b
.equ mov_r1_r4_blx_r3,                  0x8100a701
.equ movs_r1_r0_pop_r4_pc,              0x8102978d
.equ movs_r2_r4_add_sp_8_bx_lr,         0x8102d339
.equ movs_r4_r0_add_sp_c_pop_pc,        0x81000c69

// arithmetic
.equ add_r0_r1_add_sp_10_bx_lr,         0x8102daa7
.equ adcs_r0_lr_r5_lsl_12_pop_pc,       0x810111a9
.equ adds_r1_r1_r3_add_r0_r2_r1_bx_lr,  0x8102ea0f
.equ add_sp_14_pop_pc,                  0x81000d5f
.equ subs_r1_r4_r1_blx_r3,              0x8102f1fd
.equ muls_r0_r2_r0_subs_r3_r3_r0_bx_lr, 0x8102c21b

// logical
.equ eors_r4_r0_movt_r0_8103_bx_lr,     0x8102e5fb

// compare
.equ cmp_eq_r0_r1_add_sp_8_bx_lr,       0x8101f581

// misc
.equ vdispEnd,                          0x81001e33
.equ vdispSetState,                     0x81001efb
.equ vdispCtrl,                         0x8103c9f8
.equ empty_string,                      0x81000006
.equ savedata0_system_dat_path,         0x81035614
