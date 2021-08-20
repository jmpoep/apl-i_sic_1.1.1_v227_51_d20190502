;; @file
;  This is the code that goes from real-mode to protected mode.
;  It consumes the reset vector.
;
; @copyright
;  INTEL CONFIDENTIAL
;  Copyright 1999 - 2016 Intel Corporation.
;
;  The source code contained or described herein and all documents related to the
;  source code ("Material") are owned by Intel Corporation or its suppliers or
;  licensors. Title to the Material remains with Intel Corporation or its suppliers
;  and licensors. The Material may contain trade secrets and proprietary and
;  confidential information of Intel Corporation and its suppliers and licensors,
;  and is protected by worldwide copyright and trade secret laws and treaty
;  provisions. No part of the Material may be used, copied, reproduced, modified,
;  published, uploaded, posted, transmitted, distributed, or disclosed in any way
;  without Intel's prior express written permission.
;
;  No license under any patent, copyright, trade secret or other intellectual
;  property right is granted to or conferred upon you by disclosure or delivery
;  of the Materials, either expressly, by implication, inducement, estoppel or
;  otherwise. Any license under such intellectual property rights must be
;  express and approved by Intel in writing.
;
;  Unless otherwise agreed by Intel in writing, you may not remove or alter
;  this notice or any other notice embedded in Materials by Intel or
;  Intel's suppliers or licensors in any way.
;
;  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
;  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
;  the terms of your license agreement with Intel or your vendor. This file may
;  be modified by the user, subject to additional terms of the license agreement.
;
; @par Specification
;;
;
;------------------------------------------------------------------------------
  %include "PlatformNasm.inc"
  %include "Ia32Nasm.inc"
  %include "Chipset.inc"
  %include "SecCoreNasm.inc"
;
; Define SSE macros
;

;
;arg 1:RoutineLabel
;
%macro CALL_MMX   1
         CALL_MMX_EXT  %1, mm7
%endmacro

%macro RET_ESI  0
         RET_ESI_EXT   mm7
%endmacro

;
;args 1: RoutineLabel  2:MmxRegister
;
%macro CALL_MMX_EXT  2
  mov     esi, %%ReturnAddress
  movd    %2, esi              ; save ReturnAddress into MMX
  jmp     %1
%%ReturnAddress:
%endmacro

;
;args 1: ReturnAddress  2:MmxRegister
;
%macro LOAD_MMX_EXT 2
  mov     esi, %1
  movd    %2, esi              ; save ReturnAddress into MMX
%endmacro

;
;arg 1:MmxRegister
;
%macro RET_ESI_EXT   1
  movd    esi, %1              ; move ReturnAddress from MMX to ESI
  jmp     esi
%endmacro

extern   ASM_PFX(PcdGet32(PcdFspTemporaryRamSize))
extern   ASM_PFX(PcdGet32(PcdFspTemporaryRamDataSize))
extern   ASM_PFX(PcdGet32(PcdTemporaryRamBase))
extern   ASM_PFX(PcdGet32(PcdTemporaryRamSize))
extern   ASM_PFX(PcdGet32(PcdGlobalDataPointerAddress))
extern   ASM_PFX(PcdGet32(PcdPmcIpc1BaseAddress0))
extern   ASM_PFX(PcdGet32(PcdPmcIpc1BaseAddress1))
extern   ASM_PFX(PcdGet32(PcdP2SBBaseAddress))
extern   ASM_PFX(PcdGet64(PcdPciExpressBaseAddress))

;_TEXT_PROTECTED_MODE      SEGMENT PARA PUBLIC USE32 'CODE'
;ASSUME  CS:_TEXT_PROTECTED_MODE, DS:_TEXT_PROTECTED_MODE

align 4
global ASM_PFX(ProtectedModeSECStart)
ASM_PFX(ProtectedModeSECStart):
  jmp     $

global ASM_PFX(SecPlatformInit)
ASM_PFX(SecPlatformInit):
  movd  ebp,  mm7

  ;
  ; Load return address into MMX to prepare return
  ;

  STATUS_CODE (0x3)
  CALL_MMX  PlatformInitialization

  ;
  ; Check FSP Global pointer
  ; It must have not been initialized
  ;
  mov       eax, [ASM_PFX(PcdGet32(PcdGlobalDataPointerAddress))]
  mov       eax, dword [eax]
  cmp       eax, 0
  jz        FspGlobalPtrInvalid
  cmp       eax, 0xFFFFFFFF
  jz        FspGlobalPtrInvalid
  mov       eax, 0x80000003
  jmp       PlatformNemInitExit

FspGlobalPtrInvalid:

  ;
  ; Set BIT16 and BIT17 in REG_SB_BIOS_CONFIG, Port 0x4, Offset 0x6.
  ; These bits need to be set before setting bits [1:0] in BIOS_RESET_CPL
  ; so that PUNIT will not power gate DFX.
  ;
  mov     edx, 0xCF8               ; Config MCD
  mov     eax, 0x800000d4
  out     dx,  eax

  mov     edx, 0xCFC               ; Set BIT16 and BIT17
  mov     eax, 0x30000
  out     dx,  eax

  mov     edx, 0xCF8               ; Config MCR
  mov     eax, 0x800000d0
  out     dx, eax

  mov     edx, 0xCFC
  mov     eax, 0x70406f0          ; Write_opcode + portID + offset
  out     dx,  eax

  ;
  ; Set BIOS_RESET_DONE (BIT0) and BIOS_ALL_DONE (BIT1) in
  ; PUNIT.BIOS_RESET_CPL register, Port 0x4, Offset 0x5.
  ;
  mov     edx, 0xCF8               ; Config MCD
  mov     eax, 0x800000d4
  out     dx,  eax

  mov     edx, 0xCFC
  mov     eax, 3                   ; Set BIT0 and BIT1
  out     dx,  ax

  mov     edx, 0xCF8               ; Config MCR
  mov     eax, 0x800000d0
  out     dx,  eax

  mov     edx, 0xCFC
  mov     eax, 0x70405f0          ; Write_opcode + portID + offset
  out     dx,  eax

  xor     eax, eax
PlatformNemInitExit:
  RET_ESI

PlatformInitialization:

  ;
  ; Program PCIEXBAR and enable it in 0/0/0
  ; Lo - Offset 0x60
  ; Hi - Offset 0x64
  ;
  mov      edi, [ASM_PFX(PcdGet64(PcdPciExpressBaseAddress))]

  mov      eax, 0x80000060
  mov      dx,  0xCF8
  out      dx,  eax
  mov      eax, edi
  or       eax, CPU_HEC_EN
  mov      dx,  0xCFC
  out      dx,  eax

  ;
  ; Program and enable all known base addresses
  ;

  ;
  ; Program and enable MCH base address.
  ;
  mov     Dword [edi + R_MCH_BASE], MCH_BASE_ADDRESS + B_MCH_BASE_ADDRESS_EN

;
;zchai[Todo]identify all the address that replace the BARs of the previous platforms.
;BXT starts to use fixed MMIO address
;
  ;
  ; Program and enable SPI base address.
  ; B0:D13:F2
  mov     edx, 0xCF8               ; Config SPI Base
  mov     eax, 0x8006A010
  out     dx, eax

  mov     edx, 0xCFC
  mov     eax, SPI_BASE_ADDRESS
  out     dx,  eax

  mov     edx, 0xCF8               ; Config SPI Base
  mov     eax, 0x8006A004
  out     dx, eax

  mov     edx, 0xCFC
  in      eax, dx
  or      eax, 0x2                   ; enable memory space
  out     dx,  eax

  ;
  ; Program and enable ACPI base address.
  ;

  ; Current GenFw doesn't support R_386_16 relocation type
  ; Use MACRO ACPI_BASE_ADDRESS instead of PcdGet16 (PcdScAcpiIoPortBaseAddress)
  ; TODO : GenFw support  this relocation type

  mov     Word [edi + R_PMC_ACPI_BASE], ACPI_BASE_ADDRESS

  ;
  ; Program D13:F1, PMC Bar0(IPC1) and Bar1, are 64bit bars.
  ; This should be moved into C code - no need to init this early
  ;
  mov     eax, [ASM_PFX(PcdGet32(PcdPmcIpc1BaseAddress0))]
  mov     Dword [edi + R_PMC_MMIO_BAR0], eax
  mov     Dword [edi + R_PMC_MMIO_BAR0 + 0x4], 0x0

  mov     eax, [ASM_PFX(PcdGet32(PcdPmcIpc1BaseAddress1))]
  mov     Dword [edi + R_PMC_MMIO_BAR1], eax
  mov     Dword [edi + R_PMC_MMIO_BAR1 + 0x4], 0x0

  ; Enable Bus IO space decode
  mov     Word  [edi + R_PMC_PCI_CMD], 0x7

  ;
  ; P2SB
  ;
  mov     eax, [ASM_PFX(PcdGet32(PcdP2SBBaseAddress))]
  mov     Dword  [edi + R_P2SB_MMIO_BAR0], eax
  mov     Word  [edi + R_P2SB_PCI_CMD],   0x6

  ;
  ; HPET memory address enable
  ;
  mov     Byte [edi + R_P2SB_HPTC], HPTC_AE

  ;
  ; Check RTC power well first
  ;
  mov     esi, [ASM_PFX(PcdGet32(PcdPmcIpc1BaseAddress0))]
  mov     ax,  WORD [esi + R_PMC_GEN_PMCON_1]
  test    ax,  0xFFFF              ; B_PMC_GEN_PMCON_GEN_RST_STS
  jz      check_RTC_PWR_STS
  test    ax,  0x200               ; B_PMC_GEN_PMCON_GEN_RST_STS
  jz      check_RTC_PWR_STS

force_cold_boot_path:
  mov     cx, ax                   ; Save
  mov     dx, ACPI_BASE_ADDRESS
  add     dx, R_ACPI_PM1_CNT
  in      ax, dx
  and     ax, ~ (V_ACPI_PM1_CNT_S5) ; Clear sleep type field SLP_TYP [12:10]
  out     dx, ax

  mov     ax, cx                   ; restore

check_RTC_PWR_STS:
  test    ax, 0x4                 ; B_PMC_GEN_PMCON_RTC_PWR_STS
  jz      no_RTC_pwr_failure

  ;
  ; According to CHV BIOS Specification, the following sequence must be programmed
  ; in order to ensure RTC state has been initialized.
  ;
  ; The System BIOS should execute the sequence below if the RTC_PWR_STS bit is set before memory initialization.
  ; This will ensure that the RTC state machine has been initialized.
  ;  1. If the RTC_PWR_STS bit is set, steps 2 through 5 should be executed.
  ;  2. Set RTC Register 0Ah[6:4] to '110' or '111'.
  ;  3. Set RTC Register 0Bh[7].
  ;  4. Set RTC Register 0Ah[6:4] to '010'.
  ;  5. Clear RTC Register 0Bh[7].

init_RTC_state_machine:

  ;
  ; Set RTC Register 0Ah[6:4] to '110' or '111'.
  ;
  mov     al, 0xA
  out     0x70, al
  nop                  ; Delay
  nop                  ; Delay
  mov     al, 0x66
  out     0x71, al
  nop                  ; Delay
  nop                  ; Delay

  ;
  ; Set RTC Register 0Bh[7].
  ;
  mov     al, 0xB
  out     0x70, al
  nop                  ; Delay
  nop                  ; Delay
  in      al, 0x71
  nop                  ; Delay
  nop                  ; Delay
  or      al, 0x80
  out     0x71, al
  nop                  ; Delay
  nop                  ; Delay

  ;
  ; Set RTC Register 0Ah[6:4] to '010'.
  ;
  mov     al, 0xA
  out     0x70, al
  nop                  ; Delay
  nop                  ; Delay
  mov     al, 0x26
  out     0x71, al
  nop                  ; Delay
  nop                  ; Delay

  ;
  ; Clear RTC Register 0Bh[7].
  ;
  mov     al, 0xB
  out     0x70, al
  nop                  ; Delay
  nop                  ; Delay
  in      al, 0x71
  nop                  ; Delay
  nop                  ; Delay
  and     al, ~ 0x80
  out     0x71, al
  nop                  ; Delay
  nop                  ; Delay

no_RTC_pwr_failure:
  ;
  ; Enable SPI Prefetch
  ;
  mov     esi, SPI_BASE_ADDRESS
  or      Dword [esi + R_SPI_BCR], 0x8     ; Bits [3:2] = '10' - enable prefetching and caching.

  ;
  ; Program 8259 Interrupt Controller to disable all interrupts
  ;
  mov     al, 0xFF
  out     0x21, al                  ; Mask off all interrupts in master 8259
  out     0xa1, al                 ; Mask off all interrupts in slave 8259


  ;
  ; Check to see if 0xCF9 Global Reset bit is set. if set clear it.
  ;
  mov     esi, [ASM_PFX(PcdGet32(PcdPmcIpc1BaseAddress0))]
  mov     eax, DWORD [esi + R_PMC_PMIR]
  test    eax, B_PMC_PMIR_CF9GR       ; Check whether 0xCF9 Global Reset bit is set
  jz      GlobalresetClear            ; If no, continue
  and     eax, ~ (B_PMC_PMIR_CF9GR) ; Clear 0xCF9 Global Reset bit
  mov     DWORD [esi], eax

GlobalresetClear:
  ;
  ; Clear HPET Timer 0 Lower and Upper Comparator Value.
  ;

  xor     eax, eax
  mov     esi, HPET_COMP_1
  mov     Dword [esi], eax
  mov     esi, HPET_COMP_2
  mov     Dword [esi], eax

  ;
  ; Read Bunit.BMISC BIT1 to check F-segment set
  ; Determine if INIT or Hard Reset
  ;
  mov     esi, MCH_BASE_ADDRESS + BUNIT_BMISC
  mov     eax, Dword [esi]

  test    eax, B_BMISC_RFSDRAM                ; Check bit offset 1
  jnz      skipreset

reset:
  ;
  ; Do a hard Reset if INIT.
  ;
  mov     al,  6
  mov     dx,  0xcf9
  out     dx,  al                  ; Hard reset
  jmp     $

skipreset:

  ;
  ; Enable Devices
  ;
  mov     dx,  0xcf8
  mov     eax, 0x80000054
  out     dx,  eax
  add     dx,  4
%ifdef PM_DISABLE_DEV2
  mov     eax, 0x8003
%else
  mov     eax, 0xb8000019
%endif
  out     dx,  eax

  ;
  ; WA for ACPI PM1 timer BXT 0 and 1
  ;
  mov     eax, ACPI_BASE_ADDRESS
  mov     ecx, 0x121
  add     eax, BIT16 + R_ACPI_PM1_TMR  ; Bit 16 is enable and 15:0 address
  mov     edx, 0x2FBA2E25
  wrmsr

SkipAcpiTimerWA:

  xor     eax, eax
  jmp     ebp

global ASM_PFX(LoadUcode)
ASM_PFX(LoadUcode):
  ;
  ; For SIMICS we skip the ucode loading
  ;
  xor    eax, eax
  movd   ebp, mm7
  jmp    ebp

global ASM_PFX(SecCarInit)
ASM_PFX(SecCarInit):
  ;
  ;  Enable cache for use as stack and for caching code
  ;  The algorithm is specified in the processor BIOS writer's guide
  ;

  ;
  ;  Ensure that the system is in flat 32 bit protected mode.
  ;
  ;  Platform Specific - configured earlier
  ;
  ;  Ensure that only one logical processor in the system is the BSP.
  ;  (Required step for clustered systems).
  ;
  ;  Platform Specific - configured earlier

  ;  Ensure all APs are in the Wait for SIPI state.
  ;  This includes all other logical processors in the same physical processor
  ;  as the BSP and all logical processors in other physical processors.
  ;  If any APs are awake, the BIOS must put them back into the Wait for
  ;  SIPI state by issuing a broadcast INIT IPI to all excluding self.
  ;
  mov     edi, APIC_ICR_LO               ; 0FEE00300h - Send INIT IPI to all excluding self
  mov     eax, ORAllButSelf + ORSelfINIT ; 0000C4500h
  mov     [edi], eax

.1:
  mov     eax, [edi]
  bt      eax, 12                       ; Check if send is in progress
  jc      .1                            ; Loop until idle

  ;
  ;   Load microcode update into BSP.
  ;
  ;   Ensure that all variable-range MTRR valid flags are clear and
  ;   IA32_MTRR_DEF_TYPE MSR E flag is clear.  Note: This is the default state
  ;   after hardware reset.
  ;
  ;   Platform Specific - MTRR are usually in default state.
  ;

  ;
  ;   Initialize all fixed-range and variable-range MTRR register fields to 0.
  ;
   mov   ecx, IA32_MTRR_CAP         ; get variable MTRR support
   rdmsr
   movzx ebx, al                    ; EBX = number of variable MTRR pairs
   shl   ebx, 2                     ; *4 for Base/Mask pair and WORD size
   add   ebx, (MtrrCountFixed * 2)        ; EBX = size of  Fixed and Variable MTRRs

   xor   eax, eax                       ; Clear the low dword to write
   xor   edx, edx                       ; Clear the high dword to write

InitMtrrLoop:
   add   ebx, -2
   movzx ecx, WORD [cs:(MtrrInitTable + ebx)]  ; ecx <- address of mtrr to zero
   wrmsr
   jnz   InitMtrrLoop                   ; loop through the whole table

  ;
  ;   Configure the default memory type to un-cacheable (UC) in the
  ;   IA32_MTRR_DEF_TYPE MSR.
  ;
  mov     ecx, MTRR_DEF_TYPE            ; Load the MTRR default type index
  rdmsr
  and     eax, ~(00000CFFh)             ;Clear the enable bits and def type UC.
  wrmsr

  ; Configure MTRR_PHYS_MASK_HIGH for proper addressing above 4GB
  ; based on the physical address size supported for this processor
  ; This is based on read from CPUID EAX = 080000008h, EAX bits [7:0]
  ;
  ; Examples:
  ;  MTRR_PHYS_MASK_HIGH = 00000000Fh  For 36 bit addressing
  ;  MTRR_PHYS_MASK_HIGH = 0000000FFh  For 40 bit addressing
  ;
  mov   eax, 80000008h                  ; Address sizes leaf
  cpuid
  sub   al, 32
  movzx eax, al
  xor   esi, esi
  bts   esi, eax
  dec   esi                             ; esi <- MTRR_PHYS_MASK_HIGH

  ;
  ;   Configure the DataStack region as write-back (WB) cacheable memory type
  ;   using the variable range MTRRs.
  ;

  ;
  ; Set the base address of the DataStack cache range
  ;
  mov     eax, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]
  or      eax, MTRR_MEMORY_TYPE_WB
                                        ; Load the write-back cache value
  xor     edx, edx                      ; clear upper dword
  mov     ecx, MTRR_PHYS_BASE_0         ; Load the MTRR index
  wrmsr                                 ; the value in MTRR_PHYS_BASE_0

  ;
  ; Set the mask for the DataStack cache range
  ; Compute MTRR mask value:  Mask = NOT (Size - 1)
  ;
  mov  eax, [ASM_PFX(PcdGet32 (PcdTemporaryRamSize))]
  dec  eax
  not  eax
  or   eax, MTRR_PHYS_MASK_VALID
                                        ; turn on the Valid flag
  mov  edx, esi                         ; edx <- MTRR_PHYS_MASK_HIGH
  mov  ecx, MTRR_PHYS_MASK_0            ; For proper addressing above 4GB
  wrmsr                                 ; the value in MTRR_PHYS_BASE_0

  ;
  ;   Configure the BIOS code region as write-protected (WP) cacheable
  ;   memory type using a single variable range MTRR.
  ;
  ;   Platform Specific - ensure region to cache meets MTRR requirements for
  ;   size and alignment.
  ;

  ;
  ; Set the base address of the CodeRegion cache range
  ;
  mov     eax, dword [esp + 0x04]
  cmp     eax, 0
  jz      InvalidParameter

  mov     edi, dword [eax + 0x8]   ; Code regin base
  mov     eax, dword [eax + 0xC]   ; Code regin size
  cmp     eax,  0
  jz      CodeRegionMtrrdone
  cmp     edi, 0
  jz      InvalidParameter
  jmp     ValidateCodeBaseSize

InvalidParameter:
  mov     eax, 0x80000002              ; RETURN_INVALID_PARAMETER
  jmp     InitializeNEMExit

ValidateCodeBaseSize:
  ;
  ; Make sure the range length is power of 2
  ;
  bsr     ecx, eax                      ; Get the least significant set bit of 1 for length
  bsf     edx, eax                      ; Get the reversed most significant set bit of 1 for length
  cmp     ecx, edx
  jnz     CheckFail

  ;
  ; Make sure the range base is aligned properly with the range length
  ;
  bsf     ecx, edi                      ; Get the least significant set bit of 1
  cmp     ecx, edx
  jae     CheckOk

CheckFail:
  mov     eax, 0x80000002               ; RETURN_INVALID_PARAMETER
  jmp     InitializeNEMExit

CheckOk:
  ;
  ; Define "local" vars for this routine
  ; Note that mm0 is used to store BIST result for BSP,
  ; mm1 is used to store the number of processor and BSP APIC ID,
  ;
  ;
  %define CODE_SIZE_TO_CACHE      mm3
  %define CODE_BASE_TO_CACHE      mm4
  %define NEXT_MTRR_INDEX         mm5
  %define NEXT_MTRR_SIZE          mm6
  ;
  ; Initialize "locals"
  ;
  sub     ecx, ecx
  movd    NEXT_MTRR_INDEX, ecx          ; Count from 0 but start from MTRR_PHYS_BASE_1

  ;
  ; Save remaining size to cache
  ;
  movd    CODE_SIZE_TO_CACHE, eax       ; Size of code cache region that must be cached
  movd    CODE_BASE_TO_CACHE, edi       ; Base code cache address

NextMtrr:
  ;
  ; Get remaining size to cache
  ;
  movd    eax, CODE_SIZE_TO_CACHE
  and     eax, eax
  jz      CodeRegionMtrrdone            ; If no left size - we are done
  ;
  ; Determine next size to cache.
  ; We start from bottom up. Use the following algorythm:
  ; 1. Get our own alignment. Max size we can cache equals to our alignment
  ; 2. Determine what is bigger - alignment or remaining size to cache.
  ;    If aligment is bigger - cache it.
  ;      Adjust remaing size to cache and base address
  ;      Loop to 1.
  ;    If remaining size to cache is bigger
  ;      Determine the biggest 2^N part of it and cache it.
  ;      Adjust remaing size to cache and base address
  ;      Loop to 1.
  ; 3. End when there is no left size to cache or no left MTRRs
  ;
  movd    edi, CODE_BASE_TO_CACHE
  bsf     ecx, edi                      ; Get index of lowest bit set in base address
  ;
  ; Convert index into size to be cached by next MTRR
  ;
  mov     edx, 0x1
  shl     edx, cl                       ; Alignment is in edx
  cmp     edx, eax                      ; What is bigger, alignment or remaining size?
  jbe     GotSize                       ; JIf aligment is less
  ;
  ; Remaining size is bigger. Get the biggest part of it, 2^N in size
  ;
  bsr     ecx, eax                      ; Get index of highest set bit
  ;
  ; Convert index into size to be cached by next MTRR
  ;
  mov     edx, 1
  shl     edx, cl                       ; Size to cache

GotSize:
  mov     eax, edx
  movd    NEXT_MTRR_SIZE, eax           ; Save

  ;
  ; Compute MTRR mask value:  Mask = NOT (Size - 1)
  ;
  dec     eax                           ; eax - size to cache less one byte
  not     eax                           ; eax contains low 32 bits of mask
  or      eax, MTRR_PHYS_MASK_VALID     ; Set valid bit

  ;
  ; Program mask register
  ;
  mov     ecx, MTRR_PHYS_MASK_1         ; setup variable mtrr
  movd    ebx, NEXT_MTRR_INDEX
  add     ecx, ebx

  mov     edx, esi                      ; edx <- MTRR_PHYS_MASK_HIGH
  wrmsr
  ;
  ; Program base register
  ;
  sub     edx, edx
  mov     ecx, MTRR_PHYS_BASE_1         ; setup variable mtrr
  add     ecx, ebx                      ; ebx is still NEXT_MTRR_INDEX

  movd    eax, CODE_BASE_TO_CACHE
  or      eax, MTRR_MEMORY_TYPE_WP      ; set type to write protect
  wrmsr
  ;
  ; Advance and loop
  ; Reduce remaining size to cache
  ;
  movd    ebx, CODE_SIZE_TO_CACHE
  movd    eax, NEXT_MTRR_SIZE
  sub     ebx, eax
  movd    CODE_SIZE_TO_CACHE, ebx

  ;
  ; Increment MTRR index
  ;
  movd    ebx, NEXT_MTRR_INDEX
  add     ebx, 2
  movd    NEXT_MTRR_INDEX, ebx
  ;
  ; Increment base address to cache
  ;
  movd    ebx, CODE_BASE_TO_CACHE
  movd    eax, NEXT_MTRR_SIZE
  add     ebx, eax
  movd    CODE_BASE_TO_CACHE, ebx

  jmp     NextMtrr

CodeRegionMtrrdone:
;  ; Program the variable MTRR's MASK register for WDB
;  ; (Write Data Buffer, used in MRC, must be WC type)
;  ;
;  mov     ecx, MTRR_PHYS_MASK_1
;  movd    ebx, NEXT_MTRR_INDEX
;  add     ecx, ebx
;  mov     edx, esi                                          ; edx <- MTRR_PHYS_MASK_HIGH
;  mov     eax, WDB_REGION_SIZE_MASK OR MTRR_PHYS_MASK_VALID ; turn on the Valid flag
;  wrmsr

;  ;
;  ; Program the variable MTRR's BASE register for WDB
;  ;
;  dec     ecx
;  xor     edx, edx
;  mov     eax, WDB_REGION_BASE_ADDRESS OR MTRR_MEMORY_TYPE_WC
;  wrmsr

  ;
  ; Enable the MTRRs by setting the IA32_MTRR_DEF_TYPE MSR E flag.
  ;
  mov     ecx, MTRR_DEF_TYPE            ; Load the MTRR default type index
  rdmsr
  or      eax, MTRR_DEF_TYPE_E          ; Enable variable range MTRRs
  wrmsr

NoL0x2Cace:
  ;
  ;   Enable the logical processor's (BSP) cache: execute INVD and set
  ;   CR0.CD = 0, CR0.NW = 0.
  ;
  mov     eax, cr0
  and     eax, ~ (CR0_CACHE_DISABLE + CR0_NO_WRITE)
  invd
  mov     cr0, eax
  ;
  ;   Enable No-Eviction Mode Setup State by setting
  ;   NO_EVICT_MODE  MSR 2E0h bit [0] = '1'.
  ;
  mov     ecx, NO_EVICT_MODE
  rdmsr
  or      eax, 1
  wrmsr

  ;
  ;   One location in each 64-byte cache line of the DataStack region
  ;   must be written to set all cache values to the modified state.
  ;
  mov     edi, [ASM_PFX(PcdGet32(PcdTemporaryRamBase))]
  mov     ecx, [ASM_PFX(PcdGet32(PcdTemporaryRamSize))]
  shr     ecx, 6
  mov     eax, CACHE_INIT_VALUE
.3:
  mov  [edi], eax
  sfence
  add  edi, 64
  loop  .3

  ;
  ;   One location in each 64-byte cache line of the Code region
  ;   must be written to set all cache values to the modified state.
  ;
  mov     edi, 0xFFFF8000
  mov     ecx, 0x00008000
  shr     ecx, 6
.4:
  mov  eax, [edi]
  sfence
  add  edi, 64
  loop  .4

  ;
  ;   Enable No-Eviction Mode Run State by setting
  ;   NO_EVICT_MODE MSR 2E0h bit [1] = '1'.
  ;
  mov     ecx, NO_EVICT_MODE
  rdmsr
  or      eax, 2
  wrmsr

  ;
  ; Finished with cache configuration
  ;

  ;
  ; Optionally Test the Region...
  ;

  ;
  ; Test area by writing and reading
  ;
  cld
  mov     edi, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]
  mov     ecx, [ASM_PFX(PcdGet32 (PcdTemporaryRamSize))]
  shr     ecx, 2
  mov     eax, CACHE_TEST_VALUE
TestDataStackArea:
  stosd
  cmp     eax, DWORD [edi-4]
  jnz     DataStackTestFail
  loop    TestDataStackArea
  jmp     DataStackTestPass

  ;
  ; Cache test failed
  ;
DataStackTestFail:
  STATUS_CODE (0xD0)
  jmp     $

  ;
  ; Configuration test failed
  ;
ConfigurationTestFailed:
  STATUS_CODE (0xD1)
  jmp     $

DataStackTestPass:

  ;
  ; At this point you may continue normal execution.  Typically this would include
  ; reserving stack, initializing the stack pointer, etc.
  ;

  ;
  ; After memory initialization is complete, please follow the algorithm in the BIOS
  ; Writer's Guide to properly transition to a normal system configuration.
  ; The algorithm covers the required sequence to properly exit this mode.
  ;
  xor    eax, eax

InitializeNEMExit:

  RET_ESI

align 16
global ASM_PFX(BootGDTtable)

;
; GDT[0]: 0x00: Null entry, never used.
;
%define NULL_SEL $ - GDT_BASE        ; Selector [0]
GDT_BASE:
ASM_PFX(BootGDTtable): DD 0
                    DD  0
;
; Linear data segment descriptor
;
%define LINEAR_SEL $ - GDT_BASE        ; Selector [0x8]
    DW  0xFFFF                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  0x92                            ; present, ring 0, data, expand-up, writable
    DB  0xCF                            ; page-granular, 32-bit
    DB  0
;
; Linear code segment descriptor
;
%define LINEAR_CODE_SEL $ - GDT_BASE        ; Selector [0x10]
    DW  0xFFFF                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  0x9B                            ; present, ring 0, data, expand-up, not-writable
    DB  0xCF                            ; page-granular, 32-bit
    DB  0
;
; System data segment descriptor
;
%define SYS_DATA_SEL $ - GDT_BASE        ; Selector [0x18]
    DW  0xFFFF                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  0x93                            ; present, ring 0, data, expand-up, not-writable
    DB  0xCF                            ; page-granular, 32-bit
    DB  0

;
; System code segment descriptor
;
%define SYS_CODE_SEL $ - GDT_BASE        ; Selector [0x20]
    DW  0xFFFF                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  0x9A                            ; present, ring 0, data, expand-up, writable
    DB  0xCF                            ; page-granular, 32-bit
    DB  0
;
; Spare segment descriptor
;
%define SYS16_CODE_SEL $ - GDT_BASE        ; Selector [0x28]
    DW  0xFFFF                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0xE                             ; Changed from F000 to E000.
    DB  0x9B                            ; present, ring 0, code, expand-up, writable
    DB  0x0                             ; byte-granular, 16-bit
    DB  0
;
; Spare segment descriptor
;
%define SYS16_DATA_SEL $ - GDT_BASE        ; Selector [0x30]
    DW  0xFFFF                          ; limit 0xFFFF
    DW  0                               ; base 0
    DB  0
    DB  0x93                            ; present, ring 0, data, expand-up, not-writable
    DB  0x0                             ; byte-granular, 16-bit
    DB  0

;
; Spare segment descriptor
;
%define SPARE5_SEL $ - GDT_BASE        ; Selector [0x38]
    DW  0                               ; limit 0
    DW  0                               ; base 0
    DB  0
    DB  0                               ; present, ring 0, data, expand-up, writable
    DB  0                               ; page-granular, 32-bit
    DB  0
%define GDT_SIZE $ - ASM_PFX(BootGDTtable)    ; Size, in bytes

global ASM_PFX(BootGdtDescr)
GdtDesc:                                ; GDT descriptor
%define OffsetGDTDesc $ - Flat32Start
ASM_PFX(BootGdtDescr):
    DW GDT_SIZE - 1        ; GDT limit
    DD BootGDTtable ; GDT base address

ASM_PFX(NemInitLinearAddress):
NemInitLinearOffset:
    DD  ASM_PFX(ProtectedModeSECStart)    ; Offset of our 32 bit code
    DW  LINEAR_CODE_SEL

MtrrInitTable:
    DW  MTRR_DEF_TYPE
    DW  MTRR_FIX_64K_00000
    DW  MTRR_FIX_16K_80000
    DW  MTRR_FIX_16K_A0000
    DW  MTRR_FIX_4K_C0000
    DW  MTRR_FIX_4K_C8000
    DW  MTRR_FIX_4K_D0000
    DW  MTRR_FIX_4K_D8000
    DW  MTRR_FIX_4K_E0000
    DW  MTRR_FIX_4K_E8000
    DW  MTRR_FIX_4K_F0000
    DW  MTRR_FIX_4K_F8000

MtrrCountFixed EQU (($ - MtrrInitTable) / 2)

    DW  MTRR_PHYS_BASE_0
    DW  MTRR_PHYS_MASK_0
    DW  MTRR_PHYS_BASE_1
    DW  MTRR_PHYS_MASK_1
    DW  MTRR_PHYS_BASE_2
    DW  MTRR_PHYS_MASK_2
    DW  MTRR_PHYS_BASE_3
    DW  MTRR_PHYS_MASK_3
    DW  MTRR_PHYS_BASE_4
    DW  MTRR_PHYS_MASK_4
    DW  MTRR_PHYS_BASE_5
    DW  MTRR_PHYS_MASK_5
    DW  MTRR_PHYS_BASE_6
    DW  MTRR_PHYS_MASK_6
    DW  MTRR_PHYS_BASE_7
    DW  MTRR_PHYS_MASK_7
    DW  MTRR_PHYS_BASE_8
    DW  MTRR_PHYS_MASK_8
    DW  MTRR_PHYS_BASE_9
    DW  MTRR_PHYS_MASK_9
