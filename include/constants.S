/* constants.S -- sce constants
 *
 * Copyright (C) 2018 TheFloW
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

.equ NULL,                                   0

.equ SCE_KERNEL_DEFAULT_PRIORITY,            0x10000100

.equ SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, 0x09408060
.equ SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_RX,     0x1020d005
.equ SCE_KERNEL_MEMBLOCK_TYPE_KERNEL_RW,     0x1020d006

.equ SCE_NGS_VOICE_DEFINITION_MAGIC,         0x66647662
.equ SCE_NGS_VOICE_DEFINITION_XOR,           0x9e28dcce
.equ SCE_NGS_VOICE_DEFINITION_FLAGS,         0x00010001
.equ SCE_NGS_ERROR_INVALID_PARAM,            0x804a0002

.equ SCE_SYSMODULE_NGS,                      0xb

.equ SCE_O_RDONLY,                           1

.equ SCE_DISPLAY_SETBUF_NEXTFRAME,           1

.equ VDISP_STATE_EXIT,                       5
