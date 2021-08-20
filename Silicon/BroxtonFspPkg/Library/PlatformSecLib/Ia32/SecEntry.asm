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
  INCLUDE Platform.inc
  INCLUDE Ia32.inc
  INCLUDE Chipset.inc
  INCLUDE SecCore.inc

.686p
.xmm
.model small, c

CALL_MMX  MACRO   RoutineLabel
          local   ReturnAddress
          mov     esi, offset ReturnAddress
          movd    mm7, esi                      ; save ReturnAddress into MM7
          jmp     RoutineLabel
ReturnAddress:
          ENDM

RET_ESI   MACRO
          movd    esi, mm7                      ; restore ESP from MM7
          jmp     esi
          ENDM

CALL_MMX_EXT MACRO   RoutineLabel, MmxRegister
          local   ReturnAddress
          mov     esi, offset ReturnAddress
          movd    MmxRegister, esi              ; save ReturnAddress into MM7
          jmp     RoutineLabel
ReturnAddress:
          ENDM

LOAD_MMX_EXT MACRO   ReturnAddress, MmxRegister
          mov     esi, ReturnAddress
          movd    MmxRegister, esi              ; save ReturnAddress into MM7
          ENDM

RET_ESI_EXT  MACRO   MmxRegister
          movd    esi, MmxRegister              ; restore ESP from MM7
          jmp     esi
          ENDM

;
; Define SSE macros using SSE 2 instructions
;
SXMMN        MACRO   XMM, IDX, REG
             pinsrw  XMM, REG, (IDX AND 3) * 2
             ror     REG, 16
             pinsrw  XMM, REG, (IDX AND 3) * 2 + 1
             rol     REG, 16
             ENDM

LXMMN        MACRO   XMM, REG, IDX
             pshufd  XMM, XMM,  (0E4E4E4h SHR (IDX * 2))  AND 0FFh
             movd    REG, XMM
             pshufd  XMM, XMM,  (0E4E4E4h SHR (IDX * 2 + (IDX AND 1) * 4)) AND 0FFh
             ENDM


SAVE_ESP     MACRO
  SXMMN      xmm6, 0, esp
             ENDM

LOAD_ESP     MACRO
  movd       esp,  xmm6
             ENDM

EXTRN   PcdGet32(PcdTemporaryRamBase):DWORD
EXTRN   PcdGet32(PcdTemporaryRamSize):DWORD
EXTRN   PcdGet32(PcdGlobalDataPointerAddress):DWORD
EXTRN   PcdGet32(PcdPmcIpc1BaseAddress0):DWORD
EXTRN   PcdGet32(PcdPmcIpc1BaseAddress1):DWORD
EXTRN   PcdGet32(PcdP2SBBaseAddress):DWORD
EXTRN   PcdGet64(PcdPciExpressBaseAddress):DWORD
EXTRN   PcdGet16(PcdScAcpiIoPortBaseAddress):WORD
EXTRN   CopyFitTable:PROC
_TEXT_PROTECTED_MODE      SEGMENT PARA PUBLIC USE32 'CODE'
                          ASSUME  CS:_TEXT_PROTECTED_MODE, DS:_TEXT_PROTECTED_MODE

align 4
ProtectedModeSECStart PROC NEAR PUBLIC
  jmp     $
ProtectedModeSECStart ENDP

SecPlatformInit PROC NEAR PUBLIC
  movd  ebp,  mm7

  ;
  ; Load return address into MMX to prepare return
  ;

  STATUS_CODE (03h)
  CALL_MMX  PlatformInitialization

  ;
  ; Check FSP Global pointer
  ; It must have not been initialized
  ;
  mov       eax, PcdGet32(PcdGlobalDataPointerAddress)
  mov       eax, dword ptr [eax]
  cmp       eax, 0
  jz        FspGlobalPtrInvalid
  cmp       eax, 0FFFFFFFFh
  jz        FspGlobalPtrInvalid
  mov       eax, 080000003h
  jmp       PlatformNemInitExit

FspGlobalPtrInvalid:

  ;
  ; Set BIT16 and BIT17 in REG_SB_BIOS_CONFIG, Port 0x4, Offset 0x6.
  ; These bits need to be set before setting bits [1:0] in BIOS_RESET_CPL
  ; so that PUNIT will not power gate DFX.
  ;
  mov     edx, 0CF8h               ; Config MCD
  mov     eax, 800000d4h
  out     dx,  eax

  mov     edx, 0CFCh               ; Set BIT16 and BIT17
  mov     eax, 30000h
  out     dx,  eax

  mov     edx, 0CF8h               ; Config MCR
  mov     eax, 800000d0h
  out     dx, eax

  mov     edx, 0CFCh
  mov     eax, 0070406f0h          ; Write_opcode + portID + offset
  out     dx,  eax

  ;
  ; Set BIOS_RESET_DONE (BIT0) and BIOS_ALL_DONE (BIT1) in
  ; PUNIT.BIOS_RESET_CPL register, Port 0x4, Offset 0x5.
  ;
  mov     edx, 0CF8h               ; Config MCD
  mov     eax, 800000d4h
  out     dx,  eax

  mov     edx, 0CFCh
  mov     eax, 3                   ; Set BIT0 and BIT1
  out     dx,  ax

  mov     edx, 0CF8h               ; Config MCR
  mov     eax, 800000d0h
  out     dx,  eax

  mov     edx, 0CFCh
  mov     eax, 0070405f0h          ; Write_opcode + portID + offset
  out     dx,  eax

  xor     eax, eax
PlatformNemInitExit:
  RET_ESI

SecPlatformInit ENDP


PlatformInitialization    PROC    NEAR    PRIVATE

  ;
  ; Program PCIEXBAR and enable it in 0/0/0
  ; Lo - Offset 0x60
  ; Hi - Offset 0x64
  ;
  mov      edi, PcdGet64(PcdPciExpressBaseAddress)

  mov      eax, 080000060h
  mov      dx,  0CF8h
  out      dx,  eax
  mov      eax, edi
  or       eax, CPU_HEC_EN
  mov      dx,  0CFCh
  out      dx,  eax

  ;
  ; Program and enable all known base addresses
  ;

  ;
  ; Program and enable MCH base address.
  ;
  mov     Dword Ptr [edi + R_MCH_BASE], MCH_BASE_ADDRESS + B_MCH_BASE_ADDRESS_EN

;
;zchai[Todo]identify all the address that replace the BARs of the previous platforms.
;BXT starts to use fixed MMIO address
;
  ;
  ; Program and enable SPI base address.
  ; B0:D13:F2
  mov     edx, 0CF8h               ; Config SPI Base
  mov     eax, 8006A010h
  out     dx, eax

  mov     edx, 0CFCh
  mov     eax, SPI_BASE_ADDRESS
  out     dx,  eax

  mov     edx, 0CF8h               ; Config SPI Base
  mov     eax, 8006A004h
  out     dx, eax

  mov     edx, 0CFCh
  in      eax, dx
  or      eax, 2h                   ; enable memory space
  out     dx,  eax

  ;
  ; Program and enable ACPI base address.
  ;
  movzx   eax, PcdGet16 (PcdScAcpiIoPortBaseAddress)
  mov     Word Ptr [edi + R_PMC_ACPI_BASE], ax

  ;
  ; Program D13:F1, PMC Bar0(IPC1) and Bar1, are 64bit bars.
  ; This should be moved into C code - no need to init this early
  ;
  mov     eax, PcdGet32(PcdPmcIpc1BaseAddress0)
  mov     Dword Ptr [edi + R_PMC_MMIO_BAR0], eax
  mov     Dword Ptr [edi + R_PMC_MMIO_BAR0 + 4h], 0h

  mov     eax, PcdGet32(PcdPmcIpc1BaseAddress1)
  mov     Dword Ptr [edi + R_PMC_MMIO_BAR1], eax
  mov     Dword Ptr [edi + R_PMC_MMIO_BAR1 + 4h], 0h

  ; Enable Bus IO space decode
  mov     Word Ptr  [edi + R_PMC_PCI_CMD], 07h

  ;
  ; P2SB
  ;
  mov     eax, PcdGet32(PcdP2SBBaseAddress)
  mov     Dword Ptr  [edi + R_P2SB_MMIO_BAR0], eax
  mov     Word  Ptr  [edi + R_P2SB_PCI_CMD],   06h

  ;
  ; HPET memory address enable
  ;
  mov     Byte Ptr [edi + R_P2SB_HPTC], HPTC_AE


  ;
  ; Check RTC power well first
  ;
  mov     esi, PcdGet32(PcdPmcIpc1BaseAddress0)
  mov     ax,  WORD PTR [esi + R_PMC_GEN_PMCON_1]
  test    ax,  0FFFFh              ; B_PMC_GEN_PMCON_GEN_RST_STS
  jz      check_RTC_PWR_STS
  test    ax,  0200h               ; B_PMC_GEN_PMCON_GEN_RST_STS
  jz      check_RTC_PWR_STS

force_cold_boot_path:
  mov     cx, ax                   ; Save
  mov     dx, PcdGet16 (PcdScAcpiIoPortBaseAddress)
  add     dx, R_ACPI_PM1_CNT
  in      ax, dx
  and     ax, NOT (V_ACPI_PM1_CNT_S5) ; Clear sleep type field SLP_TYP [12:10]
  out     dx, ax

  mov     ax, cx                   ; restore

check_RTC_PWR_STS:
  test    ax, 004h                 ; B_PMC_GEN_PMCON_RTC_PWR_STS
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
  mov     al, 0Ah
  out     070h, al
  nop                  ; Delay
  nop                  ; Delay
  mov     al, 066h
  out     071h, al
  nop                  ; Delay
  nop                  ; Delay

  ;
  ; Set RTC Register 0Bh[7].
  ;
  mov     al, 0Bh
  out     070h, al
  nop                  ; Delay
  nop                  ; Delay
  in      al, 071h
  nop                  ; Delay
  nop                  ; Delay
  or      al, 080h
  out     071h, al
  nop                  ; Delay
  nop                  ; Delay

  ;
  ; Set RTC Register 0Ah[6:4] to '010'.
  ;
  mov     al, 0Ah
  out     070h, al
  nop                  ; Delay
  nop                  ; Delay
  mov     al, 026h
  out     071h, al
  nop                  ; Delay
  nop                  ; Delay

  ;
  ; Clear RTC Register 0Bh[7].
  ;
  mov     al, 0Bh
  out     070h, al
  nop                  ; Delay
  nop                  ; Delay
  in      al, 071h
  nop                  ; Delay
  nop                  ; Delay
  and     al, NOT 080h
  out     071h, al
  nop                  ; Delay
  nop                  ; Delay

no_RTC_pwr_failure:
  ;
  ; Enable SPI Prefetch
  ;
  mov     esi, SPI_BASE_ADDRESS
  or      Dword Ptr [esi + R_SPI_BCR], 08h     ; Bits [3:2] = '10' - enable prefetching and caching.

  ;
  ; Program 8259 Interrupt Controller to disable all interrupts
  ;
  mov     al, 0FFh
  out     21h, al                  ; Mask off all interrupts in master 8259
  out     0a1h, al                 ; Mask off all interrupts in slave 8259

  ;
  ; Check to see if 0xCF9 Global Reset bit is set. if set clear it.
  ;
  mov     esi, PcdGet32(PcdPmcIpc1BaseAddress0)
  mov     eax, DWORD PTR [esi + R_PMC_PMIR]
  test    eax, B_PMC_PMIR_CF9GR       ; Check whether 0xCF9 Global Reset bit is set
  jz      GlobalresetClear            ; If no, continue
  and     eax, NOT (B_PMC_PMIR_CF9GR) ; Clear 0xCF9 Global Reset bit
  mov     DWORD PTR [esi], eax

GlobalresetClear:
  ;
  ; Clear HPET Timer 0 Lower and Upper Comparator Value.
  ;

  xor     eax, eax
  mov     esi, HPET_COMP_1
  mov     Dword Ptr [esi], eax
  mov     esi, HPET_COMP_2
  mov     Dword ptr [esi], eax

  ;
  ; Read Bunit.BMISC BIT1 to check F-segment set
  ; Determine if INIT or Hard Reset
  ;
  mov     esi, MCH_BASE_ADDRESS + BUNIT_BMISC
  mov     eax, Dword Ptr [esi]


  test    eax, B_BMISC_RFSDRAM                ; Check bit offset 1
  jnz      @f

reset:
  ;
  ; Do a hard Reset if INIT.
  ;
  mov     al,  6
  mov     dx,  0cf9h
  out     dx,  al                  ; Hard reset
  jmp     $

@@:

  ;
  ; Enable Devices
  ;
  mov     dx,  0cf8h
  mov     eax, 80000054h
  out     dx,  eax
  add     dx,  4
%ifdef PM_DISABLE_DEV2
  mov     eax, 00008003h
%else
  mov     eax, 0b8000019h
%endif
  out     dx,  eax

  ;
  ; WA for ACPI PM1 timer BXT 0 and 1
  ;
  movzx   eax, PcdGet16 (PcdScAcpiIoPortBaseAddress)
  mov     ecx, 0121h
  add     eax, BIT16 + R_ACPI_PM1_TMR  ; Bit 16 is enable and 15:0 address
  mov     edx, 2FBA2E25h
  wrmsr

SkipAcpiTimerWA:

  xor     eax, eax
  jmp     ebp

PlatformInitialization    ENDP

LoadUcode   PROC    NEAR    PUBLIC
  ;
  ; For SIMICS we skip the ucode loading
  ;
  xor    eax, eax
  movd   ebp, mm7
  jmp    ebp
LoadUcode  ENDP

SecCarInit  PROC    NEAR    PUBLIC
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

@@:
  mov     eax, [edi]
  bt      eax, 12                       ; Check if send is in progress
  jc      @B                            ; Loop until idle

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
   add   ebx, MtrrCountFixed * 2    ; EBX = size of  Fixed and Variable MTRRs

   xor   eax, eax                       ; Clear the low dword to write
   xor   edx, edx                       ; Clear the high dword to write
   ;;;mov   ebx, MtrrCount * 2             ; ebx <- sizeof MtrrInitTable
InitMtrrLoop:
   add   ebx, -2
   movzx ecx, WORD PTR cs:MtrrInitTable[ebx]  ; ecx <- address of mtrr to zero
   wrmsr
   jnz   InitMtrrLoop                   ; loop through the whole table

  ;
  ;   Configure the default memory type to un-cacheable (UC) in the
  ;   IA32_MTRR_DEF_TYPE MSR.
  ;
  mov     ecx, MTRR_DEF_TYPE            ; Load the MTRR default type index
  rdmsr
  and     eax, NOT (00000CFFh)          ; Clear the enable bits and def type UC.
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
  mov     eax, PcdGet32(PcdTemporaryRamBase)
  or      eax, MTRR_MEMORY_TYPE_WB
                                        ; Load the write-back cache value
  xor     edx, edx                      ; clear upper dword
  mov     ecx, MTRR_PHYS_BASE_0         ; Load the MTRR index
  wrmsr                                 ; the value in MTRR_PHYS_BASE_0

  ;
  ; Set the mask for the DataStack cache range
  ; Compute MTRR mask value:  Mask = NOT (Size - 1)
  ;
  mov  eax, PcdGet32(PcdTemporaryRamSize)
  bsr  ecx, eax                      ; Get the least significant set bit of 1 for length
  bsf  edx, eax                      ; Get the reversed most significant set bit of 1 for length
  cmp  ecx, edx
  jz   @F
  mov  eax, 2
  shl  eax, cl
@@:
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
  mov     eax, dword ptr [esp + 0x04]
  cmp     eax, 0
  jz      InvalidParameter

  mov     edi, [eax].SecCarInitParams.CodeRegionBase
  mov     eax, [eax].SecCarInitParams.CodeRegionSize
  cmp     eax, 0
  jz      CodeRegionMtrrdone
  cmp     edi, 0
  jz      InvalidParameter
  jmp     ValidateCodeBaseSize

InvalidParameter:
  mov     eax, 080000002h              ; RETURN_INVALID_PARAMETER
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
  jae     @F

CheckFail:
  mov     eax, 080000002h               ; RETURN_INVALID_PARAMETER
  jmp     InitializeNEMExit

@@:
  ;
  ; Define "local" vars for this routine
  ; Note that mm0 is used to store BIST result for BSP,
  ; mm1 is used to store the number of processor and BSP APIC ID,
  ;
  ;
  CODE_SIZE_TO_CACHE    TEXTEQU  <mm3>
  CODE_BASE_TO_CACHE    TEXTEQU  <mm4>
  NEXT_MTRR_INDEX       TEXTEQU  <mm5>
  NEXT_MTRR_SIZE        TEXTEQU  <mm6>
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
  mov     edx, 1h
  shl     edx, cl                       ; Alignment is in edx
  cmp     edx, eax                      ; What is bigger, alignment or remaining size?
  jbe     gotSize                       ; JIf aligment is less
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

NoL2Cache:
  ;
  ;   Enable the logical processor's (BSP) cache: execute INVD and set
  ;   CR0.CD = 0, CR0.NW = 0.
  ;
  mov     eax, cr0
  and     eax, NOT (CR0_CACHE_DISABLE + CR0_NO_WRITE)
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
  mov     edi, PcdGet32(PcdTemporaryRamBase)
  mov     ecx, PcdGet32(PcdTemporaryRamSize)
  shr     ecx, 6
  mov     eax, CACHE_INIT_VALUE
@@:
  mov  [edi], eax
  sfence
  add  edi, 64
  loopd  @b

  ;
  ;   One location in each 64-byte cache line of the Code region
  ;   must be written to set all cache values to the modified state.
  ;
  mov     edi, 0xFFFF8000
  mov     ecx, 0x00008000
  shr     ecx, 6
@@:
  mov  eax, [edi]
  sfence
  add  edi, 64
  loopd  @b

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
  mov     edi, PcdGet32(PcdTemporaryRamBase)
  mov     ecx, PcdGet32(PcdTemporaryRamSize)
  shr     ecx, 2
  mov     eax, CACHE_TEST_VALUE
TestDataStackArea:
  stosd
  cmp     eax, DWORD PTR [edi-4]
  jnz     DataStackTestFail
  loop    TestDataStackArea
  jmp     DataStackTestPass

  ;
  ; Cache test failed
  ;
DataStackTestFail:
  STATUS_CODE (0D0h)
  jmp     $

  ;
  ; Configuration test failed
  ;
ConfigurationTestFailed:
  STATUS_CODE (0D1h)
  jmp     $

DataStackTestPass:

  ;
  ; At this point you may continue normal execution.  Typically this would include
  ; reserving stack, initializing the stack pointer, etc.
  ;
  SAVE_ESP
  mov       esp, PcdGet32 (PcdTemporaryRamBase)
  add       esp, PcdGet32 (PcdTemporaryRamSize)
  call      CopyFitTable
  LOAD_ESP
  ;
  ; After memory initialization is complete, please follow the algorithm in the BIOS
  ; Writer's Guide to properly transition to a normal system configuration.
  ; The algorithm covers the required sequence to properly exit this mode.
  ;
  xor    eax, eax

InitializeNEMExit:

  RET_ESI
SecCarInit  ENDP

align 10h
PUBLIC  BootGDTtable

;
; GDT[0]: 0x00: Null entry, never used.
;
NULL_SEL        EQU $ - GDT_BASE        ; Selector [0]
GDT_BASE:
BootGDTtable        DD  0
                    DD  0
;
; Linear data segment descriptor
;
LINEAR_SEL      EQU $ - GDT_BASE        ; Selector [0x8]
    DW  0FFFFh                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  092h                            ; present, ring 0, data, expand-up, writable
    DB  0CFh                            ; page-granular, 32-bit
    DB  0
;
; Linear code segment descriptor
;
LINEAR_CODE_SEL EQU $ - GDT_BASE        ; Selector [0x10]
    DW  0FFFFh                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  09Bh                            ; present, ring 0, data, expand-up, not-writable
    DB  0CFh                            ; page-granular, 32-bit
    DB  0
;
; System data segment descriptor
;
SYS_DATA_SEL    EQU $ - GDT_BASE        ; Selector [0x18]
    DW  0FFFFh                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  093h                            ; present, ring 0, data, expand-up, not-writable
    DB  0CFh                            ; page-granular, 32-bit
    DB  0

;
; System code segment descriptor
;
SYS_CODE_SEL    EQU $ - GDT_BASE        ; Selector [0x20]
    DW  0FFFFh                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  09Ah                            ; present, ring 0, data, expand-up, writable
    DB  0CFh                            ; page-granular, 32-bit
    DB  0
;
; Spare segment descriptor
;
SYS16_CODE_SEL  EQU $ - GDT_BASE        ; Selector [0x28]
    DW  0FFFFh                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0Eh                             ; Changed from F000 to E000.
    DB  09Bh                            ; present, ring 0, code, expand-up, writable
    DB  00h                             ; byte-granular, 16-bit
    DB  0
;
; Spare segment descriptor
;
SYS16_DATA_SEL  EQU $ - GDT_BASE        ; Selector [0x30]
    DW  0FFFFh                          ; limit 0xFFFF
    DW  0                               ; base 0
    DB  0
    DB  093h                            ; present, ring 0, data, expand-up, not-writable
    DB  00h                             ; byte-granular, 16-bit
    DB  0

;
; Spare segment descriptor
;
SPARE5_SEL      EQU $ - GDT_BASE        ; Selector [0x38]
    DW  0                               ; limit 0
    DW  0                               ; base 0
    DB  0
    DB  0                               ; present, ring 0, data, expand-up, writable
    DB  0                               ; page-granular, 32-bit
    DB  0
GDT_SIZE        EQU $ - BootGDTtable    ; Size, in bytes

PUBLIC BootGdtDescr
GdtDesc:                                ; GDT descriptor
OffsetGDTDesc   EQU $ - Flat32Start
BootGdtDescr    DW  GDT_SIZE - 1        ; GDT limit
                DD  OFFSET BootGDTtable ; GDT base address

NemInitLinearAddress   LABEL   FWORD
NemInitLinearOffset    LABEL   DWORD
    DD  OFFSET ProtectedModeSECStart    ; Offset of our 32 bit code
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

_TEXT_PROTECTED_MODE    ENDS
END
