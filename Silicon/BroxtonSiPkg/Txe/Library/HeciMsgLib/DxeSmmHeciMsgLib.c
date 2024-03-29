/** @file
  Implementation file DXE / SMM specific HECI Message functionality

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 - 2019 Intel Corporation.

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.

  This file contains an 'Intel Peripheral Driver' and is uniquely identified as
  "Intel Reference Module" and is licensed for Intel CPUs and chipsets under
  the terms of your license agreement with Intel or your vendor. This file may
  be modified by the user, subject to additional terms of the license agreement.

@par Specification Reference

**/

#include <Uefi.h>
#include <Library/BaseCryptLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HeciMsgLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PciLib.h>
#include <Library/PeiDxeSmmMmPciLib.h>
#include <Library/TimerLib.h>
#include <openssl/hmac.h>

#include <SeCState.h>
#include <CoreBiosMsg.h>
#include <HeciRegs.h>
#include <SeCAccess.h>
#include <IndustryStandard/Pci22.h>
#include <Private/Library/HeciInitLib.h>

#include <Protocol/Heci.h>
#include <Protocol/SeCPlatformPolicy.h>

UINT8  Heci2DataBuffer[MAX (HECI2_BIOS_MAX_WRITE_MSG_SIZE, HECI2_BIOS_MAX_READ_MSG_SIZE)];

UINT32  mMonoCount         = 0;
UINT8   mHmacSha256Key[32] = {0};

/**
  Retrieves the size, in bytes, of the context buffer required for HMAC-SHA1 operations.

  @return  The size, in bytes, of the context buffer required for HMAC-SHA1 operations.

**/
UINTN
EFIAPI
HmacSha256GetContextSize (
  VOID
  )
{
  //
  // Retrieves the OpenSSL HMAC-SHA1 Context Size
  //
  return (UINTN) (sizeof (HMAC_CTX));
}

/**
  Initializes user-supplied memory pointed by HmacSha1Context as HMAC-SHA1 context for
  subsequent use.

  If HmacSha1Context is NULL, then return FALSE.

  @param[out]  HmacSha1Context  Pointer to HMAC-SHA1 context being initialized.
  @param[in]   Key              Pointer to the user-supplied key.
  @param[in]   KeySize          Key size in bytes.

  @retval TRUE   HMAC-SHA1 context initialization succeeded.
  @retval FALSE  HMAC-SHA1 context initialization failed.

**/
BOOLEAN
EFIAPI
HmacSha256Init (
  OUT  VOID         *HmacSha256Context,
  IN   CONST UINT8  *Key,
  IN   UINTN        KeySize
  )
{
  //
  // Check input parameters.
  //
  if (HmacSha256Context == NULL || KeySize > INT_MAX) {
    return FALSE;
  }

  //
  // OpenSSL HMAC-SHA1 Context Initialization
  //
  HMAC_CTX_init (HmacSha256Context);
  HMAC_Init_ex (HmacSha256Context, Key, (UINT32) KeySize, EVP_sha256(), NULL);

  return TRUE;
}

/**
  Makes a copy of an existing HMAC-SHA1 context.

  If HmacSha1Context is NULL, then return FALSE.
  If NewHmacSha1Context is NULL, then return FALSE.

  @param[in]  HmacSha1Context     Pointer to HMAC-SHA1 context being copied.
  @param[out] NewHmacSha1Context  Pointer to new HMAC-SHA1 context.

  @retval TRUE   HMAC-SHA1 context copy succeeded.
  @retval FALSE  HMAC-SHA1 context copy failed.

**/
BOOLEAN
EFIAPI
HmacSha256Duplicate (
  IN   CONST VOID  *HmacSha256Context,
  OUT  VOID        *NewHmacSha256Context
  )
{
  //
  // Check input parameters.
  //
  if (HmacSha256Context == NULL || NewHmacSha256Context == NULL) {
    return FALSE;
  }

  CopyMem (NewHmacSha256Context, HmacSha256Context, sizeof (HMAC_CTX));

  return TRUE;
}

/**
  Digests the input data and updates HMAC-SHA1 context.

  This function performs HMAC-SHA1 digest on a data buffer of the specified size.
  It can be called multiple times to compute the digest of long or discontinuous data streams.
  HMAC-SHA1 context should be already correctly intialized by HmacSha1Init(), and should not
  be finalized by HmacSha1Final(). Behavior with invalid context is undefined.

  If HmacSha1Context is NULL, then return FALSE.

  @param[in, out]  HmacSha1Context Pointer to the HMAC-SHA1 context.
  @param[in]       Data            Pointer to the buffer containing the data to be digested.
  @param[in]       DataSize        Size of Data buffer in bytes.

  @retval TRUE   HMAC-SHA1 data digest succeeded.
  @retval FALSE  HMAC-SHA1 data digest failed.

**/
BOOLEAN
EFIAPI
HmacSha256Update (
  IN OUT  VOID        *HmacSha256Context,
  IN      CONST VOID  *Data,
  IN      UINTN       DataSize
  )
{
  //
  // Check input parameters.
  //
  if (HmacSha256Context == NULL) {
    return FALSE;
  }

  //
  // Check invalid parameters, in case that only DataLength was checked in OpenSSL
  //
  if (Data == NULL && DataSize != 0) {
    return FALSE;
  }

  //
  // OpenSSL HMAC-SHA1 digest update
  //
  HMAC_Update (HmacSha256Context, Data, DataSize);

  return TRUE;
}

/**
  Completes computation of the HMAC-SHA1 digest value.

  This function completes HMAC-SHA1 digest computation and retrieves the digest value into
  the specified memory. After this function has been called, the HMAC-SHA1 context cannot
  be used again.
  HMAC-SHA1 context should be already correctly intialized by HmacSha1Init(), and should
  not be finalized by HmacSha1Final(). Behavior with invalid HMAC-SHA1 context is undefined.

  If HmacSha1Context is NULL, then return FALSE.
  If HmacValue is NULL, then return FALSE.

  @param[in, out]  HmacSha1Context  Pointer to the HMAC-SHA1 context.
  @param[out]      HmacValue        Pointer to a buffer that receives the HMAC-SHA1 digest
                                    value (20 bytes).

  @retval TRUE   HMAC-SHA1 digest computation succeeded.
  @retval FALSE  HMAC-SHA1 digest computation failed.

**/
BOOLEAN
EFIAPI
HmacSha256Final (
  IN OUT  VOID   *HmacSha256Context,
  OUT     UINT8  *HmacValue
  )
{
  UINT32  Length;

  //
  // Check input parameters.
  //
  if (HmacSha256Context == NULL || HmacValue == NULL) {
    return FALSE;
  }

  //
  // OpenSSL HMAC-SHA1 digest finalization
  //
  HMAC_Final (HmacSha256Context, HmacValue, &Length);
  HMAC_CTX_cleanup (HmacSha256Context);

  return TRUE;
}

EFI_STATUS
ComputeHmacSha256Signature (
  IN  UINT8  *Data,
  IN  UINTN  DataSize,
  OUT UINT8  *Signature
) {
  UINTN      ContextSize;
  VOID       *HmacContext;
  UINT8      Digest[64];
  BOOLEAN    Status;

  //
  // HMAC-SHA1 Digest Validation
  //
  ZeroMem (Digest, 64);

  ContextSize = HmacSha256GetContextSize ();
  HmacContext = AllocatePool (ContextSize);

  DEBUG ((EFI_D_INFO, "Compute HMAC Signature. DataSize = %x ContextSize = %x\n", DataSize, ContextSize));

  DEBUG ((EFI_D_INFO, "Init... "));

  Status  = HmacSha256Init (HmacContext, mHmacSha256Key, sizeof (mHmacSha256Key));
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "[Fail]"));
    return EFI_ABORTED;
  }

  DEBUG ((EFI_D_INFO, "Update...\n"));

  Status  = HmacSha256Update (HmacContext, Data, DataSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "[Fail]"));
    return EFI_ABORTED;
  }

  DEBUG ((EFI_D_INFO, "Finalize...\n"));

  Status  = HmacSha256Final (HmacContext, Digest);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO, "[Fail]"));
    return EFI_ABORTED;
  }

  CopyMem (Signature, Digest, SHA256_DIGEST_SIZE);
  FreePool (HmacContext);

  return EFI_SUCCESS;
}

/**
  Returns whether trusted channel is enabled.

  @param[in]  None.

  @retval     TRUE if trusted channel is enabled.
              FALSE if trusted channel is disabled.
**/
BOOLEAN
IsTrustedChannelEnabled (
  VOID
)
{
  return (mMonoCount != 0);
}

/**
  Updates the SHA256 signature and monotonic counter fields of a HECI message header.

  @param[in]  MessageHeader   A pointer to the message header
  @param[in]  TotalHeaderSize The total header size
  @param[in]  TotalDataSize   The total data size

  @retval     Whether the header could be updated
**/
EFI_STATUS
EFIAPI
UpdateTrustedHeader (
  IN OUT  UINT8       *MessageHeader,
  IN      UINT32      TotalHeaderSize,
  IN      UINT32      TotalDataSize
  )
{
  EFI_STATUS                         Status;
  MBP_SMM_TRUSTED_KEY                SmmTrustedKey;
  HECI2_TRUSTED_CHANNEL_BIOS_HEADER  *TrustedChannelHeader = NULL;
  UINT8                              Digest[64];

  if (MessageHeader == NULL || TotalHeaderSize < sizeof (HECI2_TRUSTED_CHANNEL_BIOS_HEADER)) {
    return EFI_INVALID_PARAMETER;
  }

  if (mMonoCount == 0) {
    //
    // Since the is the first message, get the SMM trusted key for this boot
    //
    Status = HeciGetSMMTrustedKey (&SmmTrustedKey);

    if (!EFI_ERROR (Status)) {
      CopyMem (mHmacSha256Key, SmmTrustedKey.SmmTrustedKey, sizeof (mHmacSha256Key));
      mMonoCount = SmmTrustedKey.MonotonicCounter + 1;
      ZeroMem (&SmmTrustedKey, sizeof (SmmTrustedKey));
      DEBUG ((EFI_D_INFO, "HMAC Monotonic Count = %d\n", mMonoCount));
    } else {
      DEBUG ((EFI_D_ERROR, "Unable to get the SMM trusted key. Cannot send HECI2 transactions.\n"));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  TrustedChannelHeader = (HECI2_TRUSTED_CHANNEL_BIOS_HEADER  *) MessageHeader;

  TrustedChannelHeader->MonotonicCounter = mMonoCount++;

  DEBUG ((EFI_D_INFO, "Current HMAC monotonic count = %d.\n", TrustedChannelHeader->MonotonicCounter));

  //
  // Compute the HMAC SHA-256 digest
  //
  // Includes all fields except the signature field
  //
  ComputeHmacSha256Signature (
    (UINT8*) MessageHeader + HECI2_HMAC_SHA256_SIGNATURE_SIZE,
    (TotalHeaderSize + TotalDataSize) - HECI2_HMAC_SHA256_SIGNATURE_SIZE,
    Digest
    );

  CopyMem (&TrustedChannelHeader->Signature, Digest, HECI2_HMAC_SHA256_SIGNATURE_SIZE);

  DEBUG ((EFI_D_INFO, "Trusted channel signature computed = 0x%x\n", Digest));

  return EFI_SUCCESS;
}

/**
  Write data to NVM file through HECI2.

  @param[in] FileName   The file name.
  @param[in] Offset     The offset of data.
  @param[in] Data       The data content.
  @param[in] DataSize   Data's size.
  @param[in] Truncate   Truncate the file.

  @return EFI_SUCCESS   Write NVM file success.
  @return Others        Write NVM file failed.
**/
EFI_STATUS
Heci2WriteNVMFile (
  IN CONST CHAR8    *FileName,
  IN       UINT32   Offset,
  IN CONST UINT8    *Data,
  IN       UINTN    DataSize,
  IN       BOOLEAN  Truncate
  )
{
  EFI_STATUS                            Status;
  UINT32                                HeciSendLength;
  UINT32                                HeciRecvLength;
  UINT32                                SeCMode;
  UINTN                                 CurrentWriteDataSize;
  INTN                                  RemainingDataSize;

  HECI2_TRUSTED_CHANNEL_BIOS_WRITE_REQ  *WriteFileMessage = NULL;
  CONST UINT8                           *DataPtr          = Data;

  DEBUG ((EFI_D_INFO, "BIOS Start Send HECI Message: Heci2WriteNVMFile\n"));
  DEBUG ((EFI_D_INFO, "    Truncate bit: %a.\n", (Truncate) ? "Set" : "Not set"));

  Status = HeciGetSeCMode (HECI1_DEVICE, &SeCMode);
  if (EFI_ERROR (Status) || (SeCMode != SEC_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }
  DEBUG ((EFI_D_INFO, "GetSeCMode successful\n"));

  RemainingDataSize = DataSize;

  do {
    if (RemainingDataSize > MAX_HECI2_WRITE_DATA_SIZE) {
      CurrentWriteDataSize = MAX_HECI2_WRITE_DATA_SIZE;
    } else {
      CurrentWriteDataSize = RemainingDataSize;
    }

    SetMem (Heci2DataBuffer, sizeof (Heci2DataBuffer), 0);

    WriteFileMessage = (HECI2_TRUSTED_CHANNEL_BIOS_WRITE_REQ *) Heci2DataBuffer;

    WriteFileMessage->Offset    = Offset;
    WriteFileMessage->DataSize  = (UINT16) CurrentWriteDataSize;
    WriteFileMessage->Truncate  = (Truncate) ? 1 : 0;
    Truncate = FALSE;

    WriteFileMessage->TrustedChannelHeader.CommandId = HECI2_WRITE_DATA_CMD_ID;

    //
    // Copy the name of the NVM file to write
    //
    ASSERT (AsciiStrLen (FileName) <= sizeof (WriteFileMessage->FileName));
    ASSERT (sizeof (Heci2DataBuffer) > sizeof (HECI2_TRUSTED_CHANNEL_BIOS_WRITE_REQ));
    AsciiStrCpyS ((CHAR8 *) WriteFileMessage->FileName, sizeof (WriteFileMessage->FileName), FileName);

    CopyMem ((UINT8*) Heci2DataBuffer + sizeof (HECI2_TRUSTED_CHANNEL_BIOS_WRITE_REQ), DataPtr, CurrentWriteDataSize);

    //
    // Fill in the HMAC signature and update the monotonic counter
    //
    Status = UpdateTrustedHeader (
               Heci2DataBuffer,
               sizeof (HECI2_TRUSTED_CHANNEL_BIOS_WRITE_REQ),
               (UINT32) CurrentWriteDataSize
               );

    if (EFI_ERROR (Status)) {
      return Status;
    }

    DEBUG ((
      EFI_D_INFO,
      "HECI2 write data size = 0x%x.\n",
      sizeof (HECI2_TRUSTED_CHANNEL_BIOS_WRITE_REQ) + CurrentWriteDataSize
      ));

    HeciSendLength = (UINT32) (sizeof (HECI2_TRUSTED_CHANNEL_BIOS_WRITE_REQ) + CurrentWriteDataSize);
    HeciRecvLength = sizeof (HECI2_TRUSTED_CHANNEL_BIOS_WRITE_RESP);

    Status = Heci2SendwACK (
               (UINT32 *) Heci2DataBuffer,
               HeciSendLength,
               &HeciRecvLength,
               BIOS_FIXED_HOST_ADDR,
               HECI2_BIOS_MCA_FIXED_ADDR
               );
    if (EFI_ERROR (Status)) {
      break;
    }

    //
    // @todo: 1. Verify the monotonic counter matches that of the request
    //        2. The signature is the HMAC-SHA2 of the response message including
    //           the monotonic counter but not including the signature itself.
    //

    DEBUG ((EFI_D_INFO, "HECI2Write CommandId       =%x\n", WriteFileMessage->TrustedChannelHeader.CommandId));
    DEBUG ((EFI_D_INFO, "HECI2Write RequestResponse =%x\n", WriteFileMessage->TrustedChannelHeader.RequestResponse));
    DEBUG ((EFI_D_INFO, "HECI2Write Status          =%x\n", ((HECI2_TRUSTED_CHANNEL_BIOS_WRITE_RESP *) WriteFileMessage)->Status));

    if (((HECI2_TRUSTED_CHANNEL_BIOS_WRITE_RESP *)WriteFileMessage)->Status != 0) {
        Status = EFI_DEVICE_ERROR;
        break;
    }

    DataPtr += CurrentWriteDataSize;
    Offset  += (UINT32) CurrentWriteDataSize;

    RemainingDataSize -= CurrentWriteDataSize;
  } while (RemainingDataSize > 0);

  return Status;
}

/**
  Read NVM file data through HECI2.

  @param[in]     FileName       The file name.
  @param[in]     Offset         The offset of data.
  @param[out]    Data           The data buffer.
  @param[in,out] DataSize       Data's size.
  @param[in]     Heci2Protocol  Pointer to a HECI2 protocol instance.

  @return        EFI_SUCCESS    Read NVM file success.
  @return        Others         Read NVM file failed.
**/
EFI_STATUS
Heci2ReadNVMFile (
  IN     CONST  UINT8               *FileName,
  IN            UINT32              Offset,
  OUT           UINT8               *Data,
  IN OUT        UINTN               *DataSize,
  IN     CONST  EFI_HECI_PROTOCOL   *Heci2Protocol
  )
{
  EFI_STATUS                            Status;
  UINT32                                HeciSendLength;
  UINT32                                HeciRecvLength;
  HECI2_TRUSTED_CHANNEL_BIOS_READ_REQ   *ReadFileMessage;
  HECI2_TRUSTED_CHANNEL_BIOS_READ_RESP  *Res;
  UINT32                                SeCMode;
  UINT32                                CurrentReadDataSize;
  INTN                                  RemainingDataSize;
  UINTN                                 TotalDataSize;

  DEBUG ((DEBUG_INFO, "BIOS Start Send HECI Message: Heci2ReadNVMFile\n"));
  DEBUG ((DEBUG_INFO, "Size of HECI2_READ_DATA_SIZE = 0x%x.\n", *DataSize));

  Status = HeciGetSeCMode (HECI1_DEVICE, &SeCMode);
  if (EFI_ERROR (Status) || (SeCMode != SEC_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }
  DEBUG ((EFI_D_INFO, "GetSeCMode successful\n"));

  RemainingDataSize = *DataSize;
  TotalDataSize     = 0;

  do {
    if (RemainingDataSize > MAX_HECI2_READ_DATA_SIZE) {
      CurrentReadDataSize = MAX_HECI2_READ_DATA_SIZE;
    } else {
      CurrentReadDataSize = (UINT32) RemainingDataSize;
    }

    SetMem (Heci2DataBuffer, sizeof (Heci2DataBuffer), 0);

    ReadFileMessage = (HECI2_TRUSTED_CHANNEL_BIOS_READ_REQ *) Heci2DataBuffer;

    ReadFileMessage->Offset   = Offset;
    ReadFileMessage->DataSize = (UINT16) CurrentReadDataSize;

    ReadFileMessage->TrustedChannelHeader.CommandId  = HECI2_READ_DATA_CMD_ID;

    //
    // Copy the name of the NVM file to read
    //
    ASSERT (AsciiStrLen (FileName) <= sizeof (ReadFileMessage->FileName));
    ASSERT (sizeof (Heci2DataBuffer) > sizeof (HECI2_TRUSTED_CHANNEL_BIOS_READ_REQ));
    AsciiStrCpyS ((CHAR8 *) ReadFileMessage->FileName, sizeof (ReadFileMessage->FileName), (CONST CHAR8 *) FileName);

    //
    // Fill in the HMAC signature and update the monotonic counter
    //
    Status = UpdateTrustedHeader (
               Heci2DataBuffer,
               sizeof (HECI2_TRUSTED_CHANNEL_BIOS_READ_REQ),
               0
               );

    if (EFI_ERROR (Status)) {
      return Status;
    }

    DEBUG ((
      EFI_D_INFO,
      "HECI2 read data header size is[ %x  + %x ] = %x\n",
      sizeof (HECI2_TRUSTED_CHANNEL_BIOS_READ_RESP),
      CurrentReadDataSize,
      sizeof (HECI2_TRUSTED_CHANNEL_BIOS_READ_RESP) + CurrentReadDataSize
      ));

    HeciSendLength = sizeof (HECI2_TRUSTED_CHANNEL_BIOS_READ_REQ);
    HeciRecvLength = sizeof (HECI2_TRUSTED_CHANNEL_BIOS_READ_RESP) + CurrentReadDataSize;

    Status = Heci2Protocol->SendwACK(
               HECI2_DEVICE,
               (UINT32 *) Heci2DataBuffer,
               HeciSendLength,
               &HeciRecvLength,
               BIOS_FIXED_HOST_ADDR,
               HECI2_BIOS_MCA_FIXED_ADDR
               );

    if (EFI_ERROR (Status)) {
      break;
    }
    //
    // @todo: 1. Verify the monotonic counter matches that of the request
    //        2. The signature is the HMAC-SHA2 of the response message including
    //           the monotonic counter but not including the signature itself.
    //
    Res = (HECI2_TRUSTED_CHANNEL_BIOS_READ_RESP *) Heci2DataBuffer;

    DEBUG ((EFI_D_INFO, "HECI2Read CommandId        =%x\n", ReadFileMessage->TrustedChannelHeader.CommandId));
    DEBUG ((EFI_D_INFO, "HECI2Read RequestResponse  =%x\n", ReadFileMessage->TrustedChannelHeader.RequestResponse));
    DEBUG ((EFI_D_INFO, "HECI2Read Status           =%x\n", Res->Status));
    DEBUG ((EFI_D_INFO, "HECI2Read DataSize         =%x\n", Res->DataSize));
    if (Res->Status != 0) {
        Status = EFI_DEVICE_ERROR;
        break;
    }

    CopyMem (
      Data,
      (VOID *) (((UINTN) Heci2DataBuffer) + sizeof (HECI2_TRUSTED_CHANNEL_BIOS_READ_RESP)),
      Res->DataSize
      );

    Data   += CurrentReadDataSize;
    Offset += CurrentReadDataSize;

    TotalDataSize     += Res->DataSize;
    RemainingDataSize -= CurrentReadDataSize;

  } while (RemainingDataSize > 0);

  *DataSize = TotalDataSize;
  DEBUG ((EFI_D_INFO, " HECI2 - Total DataSize     =%x\n", *DataSize));

  return Status;
}

/**
  Get NVM file's size through HECI2.

  @param[in]  FileName       The file name.
  @param[out] FileSize       The file's size.
  @param[in]  Heci2Protocol  A pointer to a HECI2 protocol instance.

  @return     EFI_SUCCESS    Get NVM file size success.
  @return     Others         Get NVM file size failed.
**/
EFI_STATUS
Heci2GetNVMFileSize (
  IN  CONST  UINT8              *FileName,
  OUT        UINTN              *FileSize,
  IN  CONST  EFI_HECI_PROTOCOL  *Heci2Protocol
  )
{
  EFI_STATUS                                      Status;
  UINT32                                          HeciSendLength;
  UINT32                                          HeciRecvLength;
  HECI2_TRUSTED_CHANNEL_BIOS_NVMSIZE_REQ         *GetFileSizeMessage;
  UINT32                                          SeCMode;
  UINTN                                           CurrentWriteDataSize = 0;
  HECI2_TRUSTED_CHANNEL_BIOS_NVMSIZE_RESP         *Res;

  DEBUG ((EFI_D_INFO, "BIOS Start Send HECI Message: Heci2GetNVMFileSize\n"));

  Status = HeciGetSeCMode (HECI1_DEVICE, &SeCMode);
  if (EFI_ERROR (Status) || (SeCMode != SEC_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }
  DEBUG ((EFI_D_INFO, "GetSeCMode successful\n"));

  SetMem (Heci2DataBuffer, sizeof (Heci2DataBuffer), 0);

  GetFileSizeMessage = (HECI2_TRUSTED_CHANNEL_BIOS_NVMSIZE_REQ *) Heci2DataBuffer;
  GetFileSizeMessage->TrustedChannelHeader.CommandId = HECI2_FILE_SIZE_CMD_ID;
  AsciiStrCpyS ((CHAR8 *) GetFileSizeMessage->FileName, sizeof ((CHAR8 *) GetFileSizeMessage->FileName) / sizeof (CHAR16), FileName);

  //
  // Fill in the HMAC signature and update the monotonic counter
  //
  Status = UpdateTrustedHeader (
            Heci2DataBuffer,
            sizeof (HECI2_TRUSTED_CHANNEL_BIOS_NVMSIZE_REQ),
            (UINT32) CurrentWriteDataSize
            );

  DEBUG ((
     EFI_D_INFO,
    "HECI2 GetNVMfilesize data size = 0x%x\n",
    sizeof (HECI2_TRUSTED_CHANNEL_BIOS_NVMSIZE_REQ) + CurrentWriteDataSize
    ));

  HeciSendLength = (UINT32) (sizeof (HECI2_TRUSTED_CHANNEL_BIOS_NVMSIZE_REQ) + CurrentWriteDataSize);
  HeciRecvLength = sizeof (HECI2_TRUSTED_CHANNEL_BIOS_NVMSIZE_RESP);

  Status = Heci2Protocol->SendwACK(
                  HECI2_DEVICE,
                  (UINT32 *)Heci2DataBuffer,
                  HeciSendLength,
                  &HeciRecvLength,
                  BIOS_FIXED_HOST_ADDR,
                  HECI2_BIOS_MCA_FIXED_ADDR
                  );
  Res = (HECI2_TRUSTED_CHANNEL_BIOS_NVMSIZE_RESP *) Heci2DataBuffer;

  DEBUG ((EFI_D_INFO, "CommandId        =%x\n", Res->TrustedChannelHeader.CommandId));
  DEBUG ((EFI_D_INFO, "RequestResponse  =%x\n", Res->TrustedChannelHeader.RequestResponse));
  DEBUG ((EFI_D_INFO, "Status           =%x\n", Res->Status));
  DEBUG ((EFI_D_INFO, "DataSize         =%x\n", Res->DataSize));
  *FileSize = Res->DataSize;

  return Status;
}

/**
  Send Get Proxy State message through Heci2.

  @todo: This needs to be updated to use HECI2_TRUSTED_CHANNEL

  @return EFI_SUCCESS   Send message success.
  @return Others              Send message failed.
**/
EFI_STATUS
Heci2GetProxyState(
  VOID
)
{
  EFI_STATUS                      Status;
  UINT32                          HeciSendLength;
  UINT32                          HeciRecvLength;
  HECI2_BIOS_MESSAGE              *ReadFileMessage;
  UINT32                          SeCMode;
  UINT32                          DataBuffer[0x70];

  DEBUG((EFI_D_INFO, "BIOS Start Send HECI Message: Heci2GetProxyState\n"));
  Status = HeciGetSeCMode(HECI2_DEVICE, &SeCMode);
  if (EFI_ERROR (Status) || (SeCMode != SEC_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  SetMem(DataBuffer, sizeof(DataBuffer), 0);

  ReadFileMessage = (HECI2_BIOS_MESSAGE*)DataBuffer;
  ReadFileMessage->header.cmd_id = HECI2_GET_PROXY_STATE_CMD_ID;

  DEBUG ((EFI_D_INFO, "HECI2_BIOS_MESSAGE size is %x\n", sizeof(HECI2_BIOS_MESSAGE)));
  HeciSendLength                        = sizeof(HECI2_BIOS_MESSAGE);
  HeciRecvLength                        = sizeof(DataBuffer);

  Status = Heci2SendwoACK (
                  DataBuffer,
                  HeciSendLength,
                  &HeciRecvLength,
                  BIOS_FIXED_HOST_ADDR,
                  HECI2_BIOS_MCA_FIXED_ADDR
                  );


  return Status;
}

/**
  Check is the HECI2 device has interrupt.

  @return TRUE     HECI2 device interrupt.
  @return FALSE    No interrupt.
**/
BOOLEAN Heci2GetInterrupt(
  VOID
  )
{
  volatile HECI_HOST_CONTROL_REGISTER  *HostControlReg;
  UINTN                                 HeciBar;

  HeciBar = CheckAndFixHeciForAccess(HECI2_DEVICE);

  DEBUG((EFI_D_INFO, "Start Heci2GetInterrupt\n"));

  HostControlReg = (volatile HECI_HOST_CONTROL_REGISTER  *)(UINTN)(HeciBar + H_CSR);
  if (HostControlReg->r.H_IS == 0) {
    return FALSE;
  }
  return TRUE;
}

/**
  Lock Directory message through HECI2.

  @todo: This needs to be updated to use HECI2_TRUSTED_CHANNEL

  @param[in] DirName       The Directory name.
  @param[in] Heci2Protocol The HECI protocol to send the message to HECI2 device.

  @return EFI_SUCCESS   Send EOP message success.
  @return Others              Send EOP message failed.
**/
EFI_STATUS
Heci2LockDirectory (
  IN UINT8               *DirName,
  IN EFI_HECI_PROTOCOL   *Heci2Protocol
  )
{
  EFI_STATUS            Status;
  UINT32                HeciSendLength;
  UINT32                HeciRecvLength;
  HECI2_BIOS_MESSAGE    *LockDirMessage;
  UINT32                SeCMode;
  UINT32                DataBuffer[0x40];

  DEBUG((EFI_D_INFO, "BIOS Start Send HECI Message: Heci2LockDirectory\n"));
  Status = HeciGetSeCMode(HECI2_DEVICE, &SeCMode);
  if (EFI_ERROR (Status) || ((SeCMode != SEC_MODE_NORMAL) && (SEC_MODE_RECOVER != SeCMode))) {
    return EFI_UNSUPPORTED;
  }
  DEBUG ((EFI_D_INFO, "GetSeCMode successful\n"));

  SetMem(DataBuffer, sizeof(DataBuffer), 0);

  LockDirMessage = (HECI2_BIOS_MESSAGE*)DataBuffer;
  LockDirMessage->header.cmd_id = HECI2_LOCK_DIR_CMD_ID;
  AsciiStrCpyS ((CHAR8 *)LockDirMessage->Body.lockDirReq.DirName, MAX_DIR_NAME, (CONST CHAR8 *)DirName);

  DEBUG ((EFI_D_INFO, "HECI2_BIOS_MESSAGE size is %x\n", sizeof(HECI2_BIOS_MESSAGE)));
  HeciSendLength              = sizeof(HECI2_BIOS_MESSAGE);
  HeciRecvLength              = sizeof(DataBuffer);

  Status = Heci2Protocol->SendwACK(
          HECI2_DEVICE,
          DataBuffer,
          HeciSendLength,
          &HeciRecvLength,
          BIOS_FIXED_HOST_ADDR,
          HECI2_BIOS_MCA_FIXED_ADDR
          );

  DEBUG ((EFI_D_INFO, "CommandId        =%x\n", LockDirMessage->header.cmd_id));
  DEBUG ((EFI_D_INFO, "RequestResponse  =%x\n", LockDirMessage->header.req_resp));
  DEBUG ((EFI_D_INFO, "Status           =%x\n", LockDirMessage->Body.readResp.Status));

  return Status;
}

/**
  Send Get proxy State message through HEC2 but not waiting for response.

  @todo: This needs to be updated to use HECI2_TRUSTED_CHANNEL

  @param[in] Heci2Protocol The HECI protocol to send the message to HECI2 device.

  @return EFI_SUCCESS      Send get proxy state message success.
  @return Others           Send get proxy state message failed.
**/
EFI_STATUS
Heci2GetProxyStateNoResp(
  IN EFI_HECI_PROTOCOL         *Heci2Protocol
  )
{
  EFI_STATUS                      Status;
  UINT32                          HeciSendLength;
  UINT32                          HeciRecvLength;
  HECI2_BIOS_MESSAGE              *ReadFileMessage;
  UINT32                          SeCMode;
  UINT32                           DataBuffer[0x70];

  DEBUG((EFI_D_INFO, "BIOS Start Send HECI Message: Heci2GetProxyState\n"));
  Status = HeciGetSeCMode(HECI2_DEVICE, &SeCMode);
  if (EFI_ERROR (Status) || (SeCMode != SEC_MODE_NORMAL)) {
    return EFI_UNSUPPORTED;
  }

  SetMem(DataBuffer, sizeof(DataBuffer), 0);

  ReadFileMessage = (HECI2_BIOS_MESSAGE*)DataBuffer;
  ReadFileMessage->header.cmd_id = HECI2_GET_PROXY_STATE_CMD_ID;

  DEBUG ((EFI_D_INFO, "HECI2_BIOS_MESSAGE size is %x\n", sizeof(HECI2_BIOS_MESSAGE)));
  HeciSendLength                        = sizeof(HECI2_BIOS_MESSAGE);
  HeciRecvLength                        = sizeof(DataBuffer);

  Status = Heci2Protocol->SendMsg (
             HECI2_DEVICE,
             (UINT32*)DataBuffer,
             HeciSendLength,
             BIOS_FIXED_HOST_ADDR,
             HECI2_BIOS_MCA_FIXED_ADDR
             );

  return Status;
}
