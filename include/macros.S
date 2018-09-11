/* macros.S -- rop macros
 *
 * Copyright (C) 2018 TheFloW
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

// *dst = *src + val
.macro load_add_store dst, src, val
  add_lv   \src, \val
  store_rv  ret, \dst
.endm

// blx r3
.macro blx_r3
  .word blx_r3_add_sp_14_pop_pc            // pc
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// set lr to gadget add_sp_4_pop_pc
.macro set_lr_to_add_sp_4_pop_pc
  .if lr_okay == 0
    .word ldr_r0_sp_add_sp_4_pop_pc        // pc
    .word pop_pc                           // r0
    .word blx_r0_add_sp_4_pop_pc           // pc
  .endif
  .set lr_okay, 1
.endm

// set lr to gadget add_sp_14_pop_pc
.macro set_lr_to_add_sp_14_pop_pc
  .word ldr_r0_sp_add_sp_4_pop_pc          // pc
  .word pop_pc                             // r0
  .word blx_r0_add_sp_14_pop_pc            // pc
  .set lr_okay, 0
.endm

// ret = lr
.macro get_lr
  .word pop_r3_r5_pc                       // pc
  .word 0xDEADBEEF                         // r3
  .word 0                                  // r5
  .word adcs_r0_lr_r5_lsl_12_pop_pc        // pc
.endm

// set r1
.macro set_r1 a0
  .word pop_r3_r5_pc                       // pc
  .word pop_pc                             // r3
  .word 0xDEADBEEF                         // r5
  .word pop_r4_pc                          // pc
  .word \a0                                // r4
  .word mov_r1_r4_blx_r3                   // pc
  .set lr_okay, 0
.endm

// set r0, r2, ip, sp, lr and pc
.macro set_r0_r2_ip_sp_lr_pc a0
  .word ldr_r0_sp_add_sp_4_pop_pc          // pc
  .word \a0                                // r0
  .word ldm_r0_r0_r2_ip_sp_lr_pc           // pc
.endm

// *a1 = *a0
.macro store_lv a0, a1
  set_lr_to_add_sp_4_pop_pc                // pc
  .word ldm_sp_r0_r1_add_sp_c_pop_pc       // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word 0xDEADBEEF                         // dummy
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word str_r0_r1_add_sp_4_pop_pc          // pc
  .word 0xDEADBEEF                         // dummy
.endm

// *a1 = ret
.macro store_rv a0, a1
  set_r1 \a1                               // pc
  .word str_r0_r1_add_sp_4_pop_pc          // pc
  .word 0xDEADBEEF                         // dummy
.endm

// *a1 = a0
.macro store_vv a0, a1
  .word ldm_sp_r0_r1_add_sp_c_pop_pc       // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word 0xDEADBEEF                         // dummy
  .word str_r0_r1_add_sp_4_pop_pc          // pc
  .word 0xDEADBEEF                         // dummy
.endm

// ret = ret ^ a1
.macro xor_rv a0, a1
  .word movs_r4_r0_add_sp_c_pop_pc         // pc
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  set_lr_to_add_sp_4_pop_pc                // pc
  .word ldr_r0_sp_add_sp_4_pop_pc          // pc
  .word \a1                                // r0
  .word eors_r4_r0_movt_r0_8103_bx_lr      // pc
  .word 0xDEADBEEF                         // dummy
  .word mov_r0_r4_blx_lr                   // pc
  .word 0xDEADBEEF                         // dummy
.endm

// ret = *a0 + a1
.macro add_lv a0, a1
  set_lr_to_add_sp_4_pop_pc                // pc
  .word ldm_sp_r0_r1_add_sp_c_pop_pc       // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word 0xDEADBEEF                         // dummy
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word add_r0_r1_add_sp_10_bx_lr          // pc
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
.endm

// ret = ret + a1
.macro add_rv a0, a1
  call_rv add_r0_r1_add_sp_10_bx_lr, \a0, \a1
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = ret * a1 + a2
.macro mul_add_rvv, a0, a1, a2
  // Multiply r0 with a1 and save it in r3
  .word pop_r4_pc                          // pc
  .word -(\a1)                             // r4
  .word pop_r3_r5_pc                       // pc
  .word movs_r2_r4_add_sp_8_bx_lr          // r3
  .word 0xDEADBEEF                         // r5
  blx_r3                                   // pc
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  set_r1 muls_r0_r2_r0_subs_r3_r3_r0_bx_lr // pc
  .word pop_r3_r5_pc                       // pc
  .word 0                                  // r3
  .word 0xDEADBEEF                         // r5
  .word blx_r1_add_sp_4_pop_pc             // pc
  .word 0xDEADBEEF                         // dummy

  // Move r3 to r1
  .word ldm_sp_r0_r1_add_sp_c_pop_pc       // pc
  .word adds_r1_r1_r3_add_r0_r2_r1_bx_lr   // r0
  .word 0                                  // r1
  .word 0xDEADBEEF                         // dummy
  .word blx_r0_add_sp_4_pop_pc             // pc
  .word 0xDEADBEEF                         // dummy

  // Add a2 to r1 and save it in r0
  .word pop_r3_r5_pc                       // pc
  .word add_r0_r1_add_sp_10_bx_lr          // r3
  .word 0xDEADBEEF                         // r5
  .word ldr_r0_sp_add_sp_4_pop_pc          // pc
  .word \a2                                // r0
  blx_r3                                   // pc
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy

  .set lr_okay, 0
.endm

// ret = (ret == a1) ? 1 : 0
.macro cmp_eq_rv a0, a1
  set_r1 \a1                               // pc
  .word pop_r3_r5_pc                       // pc
  .word cmp_eq_r0_r1_add_sp_8_bx_lr        // r3
  .word 0xDEADBEEF                         // r5
  blx_r3                                   // pc
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = f(*a0)
.macro call_l f, a0
  set_lr_to_add_sp_4_pop_pc                // pc
  .word ldr_r0_sp_add_sp_4_pop_pc          // pc
  .word \a0                                // r0
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word \f                                 // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = f(*a0, a1)
.macro call_lv f, a0, a1
  set_lr_to_add_sp_4_pop_pc                // pc
  .word ldm_sp_r0_r1_add_sp_c_pop_pc       // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word 0xDEADBEEF                         // dummy
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word \f                                 // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = f(*a0, a1, a2)
.macro call_lvv f, a0, a1, a2
  set_lr_to_add_sp_4_pop_pc                // pc
  .word pop_r0_r1_r2_r4_pc                 // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word 0xDEADBEEF                         // r4
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word \f                                 // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = f(*a0, a1, a2, a3)
.macro call_lvvv f, a0, a1, a2, a3
  set_lr_to_add_sp_4_pop_pc                // pc
  .word pop_r0_r1_r2_r3_r4_r5_r7_pc        // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word \a3                                // r3
  .word 0xDEADBEEF                         // r4
  .word 0xDEADBEEF                         // r5
  .word 0xDEADBEEF                         // r7
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word \f                                 // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = f(*a0, *a1, a2, a3)
.macro call_llvv f, a0, a1, a2, a3
  set_lr_to_add_sp_4_pop_pc                // pc
  .word pop_r0_r1_r2_r3_r4_r5_r7_pc        // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word \a3                                // r3
  .word 0xDEADBEEF                         // r4
  .word 0xDEADBEEF                         // r5
  .word 0xDEADBEEF                         // r7
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word movs_r1_r0_pop_r4_pc               // pc
  .word 0xDEADBEEF                         // r4
  .word ldr_r0_sp_add_sp_4_pop_pc          // pc
  .word \a0                                // r0
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word \f                                 // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = f(ret, a1)
.macro call_rv f, a0, a1
  set_r1 \a1                               // pc
  .word pop_r3_r5_pc                       // pc
  .word \f                                 // r3
  .word 0xDEADBEEF                         // r5
  blx_r3                                   // pc
  .set lr_okay, 0
.endm

// ret = f(a0, *a1)
.macro call_vl f, a0, a1
  set_lr_to_add_sp_4_pop_pc                // pc
  .word ldr_r0_sp_add_sp_4_pop_pc          // pc
  .word \a1                                // r0
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word movs_r1_r0_pop_r4_pc               // pc
  .word 0xDEADBEEF                         // r4
  .word ldr_r0_sp_add_sp_4_pop_pc          // pc
  .word \a0                                // r0
  .word \f                                 // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = f(a0)
.macro call_v f, a0
  .word ldm_sp_r0_r1_add_sp_c_pop_pc       // pc
  .word \a0                                // r0
  .word \f                                 // r1
  .word 0xDEADBEEF                         // dummy
  .word blx_r1_add_sp_4_pop_pc             // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = f(a0, a1)
.macro call_vv f, a0, a1
  .word pop_r0_r1_r2_r4_pc                 // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \f                                 // r2
  .word 0xDEADBEEF                         // r4
  .word blx_r2_add_sp_c_pop_pc             // pc
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = f(a0, a1, a2)
.macro call_vvv f, a0, a1, a2
  set_lr_to_add_sp_4_pop_pc                // pc
  .word pop_r0_r1_r2_r4_pc                 // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word 0xDEADBEEF                         // r4
  .word \f                                 // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = f(a0, a1, a2, a3)
.macro call_vvvv f, a0, a1, a2, a3
  call_vvvvv \f, \a0, \a1, \a2, \a3, 0xDEADBEEF
.endm

// ret = f(a0, a1, a2, a3, a4)
.macro call_vvvvv f, a0, a1, a2, a3, a4
  set_lr_to_add_sp_4_pop_pc                // pc
  .word pop_r0_r1_r2_r3_r4_r5_r7_pc        // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word \a3                                // r3
  .word 0xDEADBEEF                         // r4
  .word 0xDEADBEEF                         // r5
  .word 0xDEADBEEF                         // r7
  .word \f                                 // pc
  .word \a4                                // on stack
  .set lr_okay, 0
.endm

// ret = f(a0, a1, a2, a3, a4, a5)
.macro call_vvvvvv f, a0, a1, a2, a3, a4, a5
  call_vvvvvvv \f, \a0, \a1, \a2, \a3, \a4, \a5, 0xDEADBEEF
.endm

// ret = f(a0, a1, a2, a3, a4, a5, a6)
.macro call_vvvvvvv f, a0, a1, a2, a3, a4, a5, a6
  set_lr_to_add_sp_14_pop_pc               // pc
  .word pop_r0_r1_r2_r3_r4_r5_r7_pc        // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word \a3                                // r3
  .word 0xDEADBEEF                         // r4
  .word 0xDEADBEEF                         // r5
  .word 0xDEADBEEF                         // r7
  .word \f                                 // pc
  .word \a4                                // on stack
  .word \a5                                // on stack
  .word \a6                                // on stack
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = (*f)(*a0)
.macro load_call_l f, a0
  load_call_lv \f, \a0, 0xDEADBEEF
.endm

// ret = (*f)(*a0, a1)
.macro load_call_lv f, a0, a1
  set_lr_to_add_sp_4_pop_pc                // pc
  store_lv \f, call_\@                     // pc
  .word ldm_sp_r0_r1_add_sp_c_pop_pc       // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word 0xDEADBEEF                         // dummy
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
call_\@:
  .word 0xDEADBEEF                         // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = (*f)(*a0, a1, a2)
.macro load_call_lvv f, a0, a1, a2
  set_lr_to_add_sp_4_pop_pc                // pc
  store_lv \f, call_\@                     // pc
  .word pop_r0_r1_r2_r4_pc                 // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word 0xDEADBEEF                         // r4
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
call_\@:
  .word 0xDEADBEEF                         // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = (*f)(*a0, a1, a2)
.macro load_call_lvv_2 f, a0, a1, a2
  set_lr_to_add_sp_4_pop_pc                // pc
  store_lv \f, call_\@                     // pc
  .word pop_r0_r1_r2_r4_pc                 // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word 0xDEADBEEF                         // r4
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word add_sp_14_pop_pc                   // pc
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
call_\@:
  .word 0xDEADBEEF                         // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = (*f)(*a0, a1, a2, a3)
.macro load_call_lvvv f, a0, a1, a2, a3
  set_lr_to_add_sp_4_pop_pc                // pc
  store_lv \f, call_\@                     // pc
  .word pop_r0_r1_r2_r3_r4_r5_r7_pc        // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word \a3                                // r3
  .word 0xDEADBEEF                         // r4
  .word 0xDEADBEEF                         // r5
  .word 0xDEADBEEF                         // r7
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
call_\@:
  .word 0xDEADBEEF                         // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = (*f)(*a0, *a1, a2, a3)
.macro load_call_llvv f, a0, a1, a2, a3
  set_lr_to_add_sp_4_pop_pc                // pc
  store_lv \f, call_\@                     // pc
  .word pop_r0_r1_r2_r3_r4_r5_r7_pc        // pc
  .word \a1                                // r0
  .word 0xDEADBEEF                         // r1
  .word \a2                                // r2
  .word \a3                                // r3
  .word 0xDEADBEEF                         // r4
  .word 0xDEADBEEF                         // r5
  .word 0xDEADBEEF                         // r7
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word movs_r1_r0_pop_r4_pc               // pc
  .word 0xDEADBEEF                         // r4
  .word ldr_r0_sp_add_sp_4_pop_pc          // pc
  .word \a0                                // r0
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
call_\@:
  .word 0xDEADBEEF                         // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = (*f)(a0)
.macro load_call_v f, a0
  set_lr_to_add_sp_4_pop_pc                // pc
  store_lv \f, call_\@                     // pc
  .word ldr_r0_sp_add_sp_4_pop_pc          // pc
  .word \a0                                // r0
call_\@:
  .word 0xDEADBEEF                         // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = (*f)(a0, a1)
.macro load_call_vv f, a0, a1
  set_lr_to_add_sp_4_pop_pc                // pc
  store_lv \f, call_\@                     // pc
  .word ldm_sp_r0_r1_add_sp_c_pop_pc       // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word 0xDEADBEEF                         // dummy
call_\@:
  .word 0xDEADBEEF                         // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = (*f)(a0, a1, a2)
.macro load_call_vvv f, a0, a1, a2
  set_lr_to_add_sp_4_pop_pc                // pc
  store_lv \f, call_\@                     // pc
  .word pop_r0_r1_r2_r4_pc                 // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word 0xDEADBEEF                         // r4
call_\@:
  .word 0xDEADBEEF                         // pc
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm

// ret = (*f)(a0, a1, a2, a3)
.macro load_call_vvvv f, a0, a1, a2, a3
  load_call_vvvvv \f, \a0, \a1, \a2, \a3, 0xDEADBEEF
.endm

// ret = (*f)(a0, a1, a2, a3, a4)
.macro load_call_vvvvv f, a0, a1, a2, a3, a4
  set_lr_to_add_sp_4_pop_pc                // pc
  store_lv \f, call_\@                     // pc
  .word pop_r0_r1_r2_r3_r4_r5_r7_pc        // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word \a3                                // r3
  .word 0xDEADBEEF                         // r4
  .word 0xDEADBEEF                         // r5
  .word 0xDEADBEEF                         // r7
call_\@:
  .word 0xDEADBEEF                         // pc
  .word \a4                                // on stack
  .set lr_okay, 0
.endm

// ret = (*f)(a0, a1, a2, a3, a4, a5)
.macro load_call_vvvvvv f, a0, a1, a2, a3, a4, a5
  load_call_vvvvvvv \f, \a0, \a1, \a2, \a3, \a4, \a5, 0xDEADBEEF
.endm

// ret = (*f)(a0, a1, a2, a3, a4, a5, a6)
.macro load_call_vvvvvvv f, a0, a1, a2, a3, a4, a5, a6
  set_lr_to_add_sp_4_pop_pc                // pc
  .word ldm_sp_r0_r1_add_sp_c_pop_pc       // pc
  .word \f                                 // r0
  .word ldm_data_r8 + 0x18                 // r1
  .word 0xDEADBEEF                         // dummy
  .word ldr_r0_r0_bx_lr                    // pc
  .word 0xDEADBEEF                         // dummy
  .word str_r0_r1_add_sp_4_pop_pc          // pc
  .word 0xDEADBEEF                         // dummy
  .word pop_r4_r5_r6_r7_r8_sb_pc           // pc
  .word 0xDEADBEEF                         // r4
  .word 0xDEADBEEF                         // r5
  .word 0xDEADBEEF                         // r6
  .word 0xDEADBEEF                         // r7
  .word ldm_data_r8                        // r8
  .word 0xDEADBEEF                         // sb
  .word ldm_r8_r0_r1_r4_r5_sl_ip_lr_pc     // pc
  .word pop_r0_r1_r2_r3_r4_r5_r7_pc        // pc
  .word \a0                                // r0
  .word \a1                                // r1
  .word \a2                                // r2
  .word \a3                                // r3
  .word 0xDEADBEEF                         // r4
  .word 0xDEADBEEF                         // r5
  .word 0xDEADBEEF                         // r7
  .word blx_lr_add_sp_14_pop_pc            // pc
  .word \a4                                // on stack
  .word \a5                                // on stack
  .word \a6                                // on stack
  .word 0xDEADBEEF                         // dummy
  .word 0xDEADBEEF                         // dummy
  .set lr_okay, 0
.endm
