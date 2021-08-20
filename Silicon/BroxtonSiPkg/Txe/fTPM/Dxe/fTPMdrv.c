/** @file
  FTPM driver

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2007 - 2016 Intel Corporation.

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

@par Specification
**/

//
// External include files do NOT need to be explicitly specified in real EDKII
// environment
//
#include "fTPMDrv.h"
#include "fTPMCore.h"

extern DXE_SEC_POLICY_PROTOCOL *mDxePlatformSeCPolicy;

#define ONE_SECOND_TIMEOUT  1000000
#define FWU_TIMEOUT         90

//
// Global driver data
//
FTPM_INSTANCE *mfTPMContext;
EFI_HANDLE    mfTPMDrv;
EFI_EVENT     mExitBootServicesEvent;
EFI_EVENT     mLegacyBootEvent;

/**

  Show warning message to user.

  @param[in]  None

  @retval   EFI_STATUS

**/
EFI_STATUS
SeCWarningMessage (
  VOID
  )
{
  HECI_FWS_REGISTER SeCFirmwareStatus;

  SeCFirmwareStatus.ul = HeciPciRead32 (R_SEC_FW_STS0);

  //
  // Check for ME FPT Bad & FT BUP LD FLR
  //
  if (SeCFirmwareStatus.r.FptBad != 0 || SeCFirmwareStatus.r.FtBupLdFlr != 0) {
    SeCReportError (MSG_SEC_FW_UPDATE_FAILED);
  }

  return EFI_SUCCESS;
}



/**

  Send SEC the BIOS end of Post message.

  @param[in]  None

  @retval   EFI_STATUS

**/
EFI_STATUS
SeCEndOfPostEvent (
  VOID
  )
{
  EFI_STATUS        Status;
  EFI_HECI_PROTOCOL *Heci;
  UINT32            SeCStatus;
  UINT32 HeciBar1Value, HAlivenessResponse, Timeout;


  //
  // Init SEC Policy Library
  //
  Status = SeCPolicyLibInit ();
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  Status = gBS->LocateProtocol (
                  &gEfiHeciProtocolGuid,
                  NULL,
                  &Heci
                  );
  if (!EFI_ERROR (Status)) {
    //
    // Check SEC Status
    //
    Status = Heci->GetSeCStatus (&SeCStatus);
    ASSERT_EFI_ERROR (Status);

    //
    // Send EOP message when SEC is ready.  Do not care about if SEC FW INIT is completed.
    //
    DEBUG ((EFI_D_INFO, "Sending EOP...\n"));
    if (SEC_STATUS_SEC_STATE_ONLY (SeCStatus) == SEC_READY) {
      if (SeCEndOfPostEnabled ()) {
        //
        // Send SEC the BIOS Extensions exit code and End Of Post message.
        // if no success, we display an error message and halt the system.
        //
        Status = HeciSendEndOfPostMessage ();

        if ((EFI_ERROR (Status)) && (Status != EFI_UNSUPPORTED)) {
          SeCReportError (MSG_EOP_ERROR);
          CpuDeadLoop();
       }
      } // End of EOP setup option
    }
  } // End of EFI_ERROR of locate HECI driver

  //
  //  Before completing POST, BIOS must clear SICR_HOST_ALIVENESS_REQ.ALIVENESS_REQ as its last action, before handing over control to the OS.
  //
  HeciBar1Value =  HeciPciRead32 (HECI_BAR1);

  Mmio32And(HeciBar1Value,R_SICR_HOST_ALIVENESS_REQ,(~B_ALIVENESS_REQ));

  DEBUG ((EFI_D_INFO, "Before completing POST, BIOS must clear SICR_HOST_ALIVENESS_REQ.ALIVENESS_REQ \n"));

  HAlivenessResponse =  Mmio32(HeciBar1Value,R_HICR_HOST_ALIVENESS_RESP);

  Timeout=0;
  while(((HAlivenessResponse & B_ALIVENESS_ACK) != B_ALIVENESS_ACK) && (Timeout < 5) )
  {
    gBS->Stall (ONE_SECOND_TIMEOUT);
    HAlivenessResponse = Mmio32(HeciBar1Value,R_HICR_HOST_ALIVENESS_RESP);
    DEBUG ((EFI_D_INFO, "Read HOST Alive ACK: %x %x\n", HAlivenessResponse, Timeout));
    Timeout++;
  }
  //
  // TODO:
  // Note that after host writes to this bit, it must wait for an acknowledgement that is generated by bit <TBD> before assuming that SeC has received the request.
  // Until such acknowledgement is received, Host must not generate additional requests or request cancellations on this bit.
  //
  if (Timeout >= 5)
  {
    DEBUG ((EFI_D_INFO, "Timeout occurred waiting for host aliveness ACK.\n"));
  } else {
    DEBUG ((EFI_D_INFO, "SEC host aliveness ACK received.\n"));
  }

  PciOr32 (PCI_LIB_ADDRESS (0, 2, 0, 0x74), 0x00000007);

  return EFI_SUCCESS;
}


/**

  Halt Boot for up to 90 seconds if Bit 11 of FW Status Register (FW_UPD_IN_PROGRESS) is set

  @param[in]  None

  @retval   None

**/
VOID
CheckFwUpdInProgress (
  VOID
  )
{
/*  HECI_FWS_REGISTER FwStatus;
  UINT8             StallCount;
  EFI_STATUS        Status;

  StallCount  = 0;
  Status      = mHeciContext->HeciCtlr.GetSeCStatus (&FwStatus.ul);
  if (!EFI_ERROR (Status)) {
    if (FwStatus.ul & SEC_FW_UPDATES_IN_PROGRESS) {
      Status = SeCPolicyLibInit ();
      if (Status == EFI_SUCCESS) {
        SeCReportError (MSG_SEC_FW_UPDATE_WAIT);
      }
    }

    while ((FwStatus.ul & SEC_FW_UPDATES_IN_PROGRESS) && (StallCount < FWU_TIMEOUT)) {
      gBS->Stall (ONE_SECOND_TIMEOUT);
      StallCount  = StallCount + 1;
      Status      = mHeciContext->HeciCtlr.GetSeCStatus (&FwStatus.ul);
    }
  }
*/
  return ;
}
/**

  Signal a event for SeC ready to boot.

  @param[in]  Event             - The event that triggered this notification function
  @param[in]  ParentImageHandle - Pointer to the notification functions context

  @retval   EFI_STATUS

**/
EFI_STATUS
EFIAPI
SeCReadyToBootEvent (
  EFI_EVENT           Event,
  VOID                *ParentImageHandle
  )
{
  EFI_STATUS  Status;
  UINT32      SeCMode;
  UINT32      SeCStatus;

  DEBUG ((EFI_D_INFO, "SeCReadyToBootEvent ++\n"));

  //
  // We will trigger all events in order
  //

  HeciGetSeCMode(&SeCMode);
  HeciGetSeCStatus(&SeCStatus);
  if ((SeCMode == SEC_MODE_NORMAL) &&
      ((SEC_STATUS_SEC_STATE_ONLY(SeCStatus) == SEC_IN_RECOVERY_MODE) ||
       (SEC_STATUS_SEC_STATE_ONLY(SeCStatus) == SEC_READY))) {


    CheckFwUpdInProgress ();

  //  Status = HeciTrConfig ();
  //  ASSERT_EFI_ERROR (Status);

    Status = SeCWarningMessage ();
    ASSERT_EFI_ERROR (Status);

    Status = SeCEndOfPostEvent ();
    ASSERT_EFI_ERROR (Status);
  }
  gBS->CloseEvent (Event);

  DEBUG ((EFI_D_INFO, "SeCReadyToBootEvent --\n"));
  return EFI_SUCCESS;
}
/**

  HECI driver entry point used to initialize support for the FTPM device.

  @param[in]  ImageHandle - Standard entry point parameter.
  @param[in]  SystemTable - Standard entry point parameter.

  @retval   EFI_STATUS

**/
EFI_STATUS
InitializefTPM (
  IN EFI_HANDLE           ImageHandle,
  IN EFI_SYSTEM_TABLE     *SystemTable
  )
{

  UINT32 Status, *t=0;
  DEBUG ((EFI_D_INFO, "InitializefTPM ++\n"));



  mfTPMDrv      = ImageHandle;
  mfTPMContext  = AllocateZeroPool (sizeof (FTPM_INSTANCE));
  //
  // Initialize FTPM protocol pointers
  //
  if (mfTPMContext != NULL) {
    mfTPMContext->fTPMCtlr.ResetfTPM     = ResetfTPM;
    mfTPMContext->fTPMCtlr.SendwACK      = HeciSendwACK;
    mfTPMContext->fTPMCtlr.ReadMsg       = HeciReceive;
    mfTPMContext->fTPMCtlr.SendMsg       = fTPMCmdSend;       // Send Generic command
    mfTPMContext->fTPMCtlr.InitHci       = fTPMInitialize;
    mfTPMContext->fTPMCtlr.ReInitfTPM    = fTPMReInitialize;
    mfTPMContext->fTPMCtlr.SeCResetWait  = SeCResetWait;
    mfTPMContext->fTPMCtlr.GetSeCStatus  = HeciGetSeCStatus;
    mfTPMContext->fTPMCtlr.GetSeCMode    = HeciGetSeCMode;
    mfTPMContext->fTPMCtlr.StartCmd      = StartCmd;          // Send fTPM Start command
  }

  //
  // Install the FTPM interface
  //
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mfTPMContext->Handle,
                  &gEfifTPMProtocolGuid,
                  &mfTPMContext->fTPMCtlr,
                  NULL
                  );
//  CpuDeadLoop();
  StartCmd(t);
  //
  // Initialize the HECI device
  //
 // Status = InitializeHeciPrivate ();


  if ((EFI_ERROR (Status)) || (mfTPMContext == NULL)) {
    //
    // Don't install on ERR
    //
    DEBUG ((EFI_D_ERROR, "fTPM not initialized - Removing devices from PCI space!\n"));
    DisableAllSECDevices ();
    //
    // Store the current value of DEVEN for S3 resume path
    //
    //DeviceStatusSave ();

    return EFI_SUCCESS;
  }

  if (EFI_ERROR (Status)) {
    return EFI_SUCCESS;
  }
  //
  // Initialize the SeC Reference Code Information
  //
 // mHeciContext->fTPMRcInfo.Revision = SEC_RC_INFO_PROTOCOL_REVISION_1;

  //
  // SeC Reference Code formats 0xAABBCCDD
  //   DD - Build Number
  //   CC - Reference Code Revision
  //   BB - Reference Code Minor Version
  //   AA - Reference Code Major Version
  // Example: SeC Reference Code 0.7.1 should be 00 07 01 00 (0x00070100)
  //
 // mHeciContext->SeCRcInfo.RCVersion.Data = SEC_RC_VERSION;

  //
  // Install the SeC Reference Code Information
  //
//  Status = gBS->InstallMultipleProtocolInterfaces (
 //                 &mHeciContext->Handle,
//                  &gEfiSeCRcInfoProtocolGuid,
 //                 &mHeciContext->SeCRcInfo,
//                  NULL
//                  );
//  if (EFI_ERROR (Status)) {
//    return EFI_SUCCESS;
 // }


  //
  // Create an Ready to Boot event.
  //
/*  Status = EfiCreateEventReadyToBootEx (
            EFI_TPL_CALLBACK,
            SeCReadyToBootEvent,
            (VOID *) &ImageHandle,
            &ReadyToBootEvent
            );
  ASSERT_EFI_ERROR (Status);
*/
  //SeCReadyToBootEvent(ReadyToBootEvent,ImageHandle);
  DEBUG ((EFI_D_INFO, "InitializeHECI --\n"));
  return Status;
}