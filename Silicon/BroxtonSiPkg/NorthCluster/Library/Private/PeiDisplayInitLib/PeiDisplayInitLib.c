/** @file
  PEIM to initialize Early Display.

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2015 - 2016 Intel Corporation.

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

@par Specification Reference:
**/

#include <Library/DisplayInitLib.h>
#include <Library/I2CLib.h>
#include <Include/Ppi/Graphics.h>
#include <Ppi/GraphicsPlatformPolicyPpi.h>
#include <Ppi/FirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Pi/PiPeiCis.h>
#include <Guid/FirmwareFileSystem2.h>
#include <Ppi/SaPolicy.h>
#include <Library/PostCodeLib.h>

GRAPHICS_CONFIG                    *mGtConfig;
EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE  *Mode = NULL;


STATIC
EFI_PEI_NOTIFY_DESCRIPTOR  mPeiGraphicsNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiGraphicsPpiGuid,
  PeiGraphicsNotifyCallback
};

/**
  PeiDisplayInit: Initialize the IGD using GraphicsInitPeim and display logo in PEI

  @param[in] GtConfig                    GRAPHICS_CONFIG to access the GtConfig related information
             GtConfig.GraphicsConfigPtr  Address of the Graphics Configuration Table
             GtConfig.LogoPtr            Address of Logo to be displayed
             GtConfig.LogoSize           Logo Size

  @retval     EFI_STATUS
**/
EFI_STATUS
PeiDisplayInit (
  IN   GRAPHICS_CONFIG             *GtConfig
  )
{
  EFI_STATUS    Status = EFI_SUCCESS;
  UINTN         PciCfgBase;

  DEBUG ((DEBUG_INFO, "PeiDisplayInit: Begin \n"));

  if (GtConfig->PeiGraphicsPeimInit == 0) {
    return EFI_SUCCESS;
  }

  if (GtConfig->GraphicsConfigPtr == NULL) {
    DEBUG((DEBUG_INFO, "Bad VBT Pointer passed, Exiting PeiDisplayInit \n"));
    return EFI_SUCCESS;
  }

  mGtConfig = GtConfig;
  PciCfgBase = MmPciBase (SA_MC_BUS, 2, 0);
  ///
  /// Install Stall PPI - Consumed by GraphicsInitPeim
  ///
  Status = InstallStallPpi();
  ASSERT_EFI_ERROR (Status);

  if((MmioRead16 (PciCfgBase + R_SA_IGD_VID) != 0xFFFF)) {
    ///
    /// Register notify to on PeiGraphicsPpi
    ///
    Status = PeiServicesNotifyPpi (&mPeiGraphicsNotifyList);
  }

  return Status;
}

/**
  Run this function after SaPolicy PPI to be installed.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
PeiGraphicsNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS   Status;
  UINTN        PciCfgBase;

  DEBUG ((DEBUG_INFO, "PeiGraphicsNotifyCallback Entry\n"));
  PostCode (0xA04);
  PciCfgBase = MmPciBase (SA_MC_BUS, 2, 0);

  ///
  /// Initialize Gttmmadr and Gmadr
  ///
  MmioWrite32 (PciCfgBase + R_SA_IGD_GTTMMADR, mGtConfig->GttMmAdr);
  MmioWrite32 (PciCfgBase + R_SA_IGD_GMADR, mGtConfig->GmAdr);
  MmioWrite32 (PciCfgBase + R_SA_IGD_CMD, 0x07);

  DEBUG ((DEBUG_INFO, "CallPpiAndFillFrameBuffer\n"));
  PostCode (0xA05);

  Status = CallPpiAndFillFrameBuffer (mGtConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Call Ppi and fill frame buffer \n"));
  }
  ///
  /// Disable  Gttmmadr and Gmadr  MMIO after we are done with Logo display.
  ///
  MmioWrite32 (PciCfgBase + R_SA_IGD_CMD, 0x00);
  DEBUG ((DEBUG_INFO, "PeiGraphicsNotifyCallback Exit\n"));
  PostCode (0xA09);
  return Status;
}

/**
  CallPpiAndFillFrameBuffer: Call GraphicsInitPeim PeiGraphicsPpi to initalize display and get Mode info.
  Publish GraphicsInfoHob and call FillFrameBufferAndShowLogo

  @param[in]  GtConfig            GRAPHICS_CONFIG to access the GtConfig related information
              GtConfig.LogoPtr    Address of Logo to be displayed
              GtConfig.LogoSize   Logo Size

  @retval    EFI_STATUS
**/
EFI_STATUS
EFIAPI
CallPpiAndFillFrameBuffer (
  IN   GRAPHICS_CONFIG             *GtConfig
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  EFI_PEI_GRAPHICS_PPI        *GfxInitPpi;
  EFI_PEI_GRAPHICS_INFO_HOB   *PlatformGraphicsOutput;

  DEBUG ((DEBUG_INFO, "CallPpiAndFillFrameBuffer: Begin \n"));

  ///
  /// Locate GfxInit Ppi
  ///
  Status = PeiServicesLocatePpi (&gEfiPeiGraphicsPpiGuid, 0, NULL, (VOID *)&GfxInitPpi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to locate Ppi GraphicsPpiInit and GraphicsPpiGetMode. \n"));
    return Status;
  }

  ///
  /// Call PeiGraphicsPpi.GraphicsPpiInit to initilize the display
  ///
  DEBUG ((DEBUG_INFO, "GraphicsPpiInit Start\n"));
  PostCode (0xA06);
  Status = GfxInitPpi->GraphicsPpiInit (GtConfig->GraphicsConfigPtr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GraphicsPpiInit failed. \n"));
    return Status;
  }

  Mode       = AllocateZeroPool (sizeof (EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE));
  if(Mode == NULL){
    DEBUG ((DEBUG_ERROR, "Failed to Allocate Pool. \n"));
    return EFI_OUT_OF_RESOURCES;
  }

  Mode->Info = AllocateZeroPool (sizeof (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION));
  if(Mode->Info == NULL){
    DEBUG ((DEBUG_ERROR, "Failed to Allocate Pool. \n"));
    return EFI_OUT_OF_RESOURCES;
  }
  ///
  /// Call PeiGraphicsPpi.GraphicsPpiGetMode to get display resolution
  ///
  DEBUG ((DEBUG_INFO, "GraphicsPpiGetMode Start\n"));
  PostCode (0xA07);
  Status = GfxInitPpi->GraphicsPpiGetMode (Mode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GraphicsPpiGetMode failed. \n"));
    return Status;
  }

  ///
  /// Print Mode information recived from GraphicsPeim
  ///
  DEBUG ((DEBUG_INFO, "MaxMode:0x%x \n", Mode->MaxMode));
  DEBUG ((DEBUG_INFO, "Mode:0x%x \n", Mode->Mode));
  DEBUG ((DEBUG_INFO, "SizeOfInfo:0x%x \n", Mode->SizeOfInfo));
  DEBUG ((DEBUG_INFO, "FrameBufferBase:0x%x \n", Mode->FrameBufferBase));
  DEBUG ((DEBUG_INFO, "FrameBufferSize:0x%x \n", Mode->FrameBufferSize));
  DEBUG ((DEBUG_INFO, "Version:0x%x \n", Mode->Info->Version));
  DEBUG ((DEBUG_INFO, "HorizontalResolution:0x%x \n", Mode->Info->HorizontalResolution));
  DEBUG ((DEBUG_INFO, "VerticalResolution:0x%x \n", Mode->Info->VerticalResolution));
  DEBUG ((DEBUG_INFO, "PixelFormat:0x%x \n", Mode->Info->PixelFormat));
  DEBUG ((DEBUG_INFO, "PixelsPerScanLine:0x%x \n", Mode->Info->PixelsPerScanLine));

  ///
  /// Publish GraphicsInfoHob to be used by platform code
  ///
  PlatformGraphicsOutput = BuildGuidHob (&gEfiGraphicsInfoHobGuid, sizeof (EFI_PEI_GRAPHICS_INFO_HOB));

  if (PlatformGraphicsOutput == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to build GraphicsInfoHob. \n"));
    return EFI_OUT_OF_RESOURCES;
  }

  PlatformGraphicsOutput->GraphicsMode.Version              = Mode->Info->Version;
  PlatformGraphicsOutput->GraphicsMode.HorizontalResolution = Mode->Info->HorizontalResolution;
  PlatformGraphicsOutput->GraphicsMode.VerticalResolution   = Mode->Info->VerticalResolution;
  PlatformGraphicsOutput->GraphicsMode.PixelFormat          = Mode->Info->PixelFormat;
  PlatformGraphicsOutput->GraphicsMode.PixelInformation     = Mode->Info->PixelInformation;
  PlatformGraphicsOutput->GraphicsMode.PixelsPerScanLine    = Mode->Info->PixelsPerScanLine;
  PlatformGraphicsOutput->FrameBufferBase                   = Mode->FrameBufferBase;
  PlatformGraphicsOutput->FrameBufferSize                   = Mode->FrameBufferSize;

  ///
  /// Display Logo if user provides valid Bmp image
  ///
  if (GtConfig->LogoPtr != 0) {
    DEBUG ((DEBUG_INFO, "FillFrameBufferAndShowLogo Start\n"));
    PostCode (0xA08);
    Status = FillFrameBufferAndShowLogo ( GtConfig);
  }

  DEBUG ((DEBUG_INFO, "CallPpiAndFillFrameBuffer: End \n"));

  return Status;
}

/**
  FillFrameBufferAndShowLogo: Fill frame buffer with the image

  @param[in] GtConfig                    GRAPHICS_CONFIG to access the GtConfig related information
             GtConfig.GraphicsConfigPtr  Address of the Graphics Configuration Table

  @retval    EFI_STATUS
**/

EFI_STATUS
FillFrameBufferAndShowLogo(
  IN   GRAPHICS_CONFIG             *GtConfig
)
{
  VOID*                         Buffer;
  UINT32                        Size;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Blt = NULL;
  UINTN                         BltSize;
  UINTN                         Height;
  UINTN                         Width;
  EFI_STATUS                    Status = EFI_SUCCESS;
  UINTN                         LogoDestX;
  UINTN                         LogoDestY;
  UINTN                         SrcY,DstY;
  UINT8                         *SrcAddress;
  UINT8                         *DstAddress;
  UINT32                        BytesPerScanLine;

  DEBUG ((DEBUG_INFO, "FillFrameBufferAndShowLogo: Start \n"));

  if (Mode == NULL) {
    DEBUG ((DEBUG_INFO, "Returning from FillFrameBufferAndShowLogo() due to invalid mode\n"));
    return EFI_UNSUPPORTED;
  }

  ///
  /// Get the Logo pointer and Size .
  ///
  Buffer = GtConfig->LogoPtr;
  Size =   GtConfig->LogoSize;

  if (Buffer == 0) {
    DEBUG ((DEBUG_ERROR, "No Logo information. Returning from FillFrameBufferAndShowLogo()\n"));
    return EFI_UNSUPPORTED;
  }
  ///
  /// Convert Bmp Image to GopBlt
  ///
  Status = ConvertBmpToGopBlt (
             Buffer,
             (UINTN) Size,
             (VOID **) &Blt,
             &BltSize,
             &Height,
             &Width
             );
  ASSERT_EFI_ERROR (Status);

  //
  // if Convert Bmp to blt successful fill frame buffer with the image
  //
  if (!EFI_ERROR (Status)) {
    //
    // if Convert Bmp to blt successful Center the logo and fill frame buffer.
    //
    LogoDestX = (Mode->Info->HorizontalResolution - Width) /2;
    LogoDestY = (Mode->Info->VerticalResolution - Height) /2;
    BytesPerScanLine = Mode->Info->PixelsPerScanLine*sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL);
    //
    // Fill framebuffer with the logo line by line
    //
    for (SrcY = 0, DstY = LogoDestY; DstY < (LogoDestY + Height); SrcY++, DstY++) {
      DstAddress = (UINT8 *) (UINTN) (Mode->FrameBufferBase + DstY * BytesPerScanLine + LogoDestX * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
      SrcAddress = (UINT8 *) ((UINT8 *) Blt + (SrcY * Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL)));
      CopyMem (DstAddress, SrcAddress, Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
    }
  }
  FreePool (Blt);

  DEBUG ((DEBUG_INFO, "FillFrameBufferAndShowLogo: END \n"));

  return Status;
}

/**
  Convert a *.BMP graphics image to a GOP blt buffer. If a NULL Blt buffer
  is passed in a GopBlt buffer will be allocated by this routine. If a GopBlt
  buffer is passed in it will be used if it is big enough.

  @param[in]      BmpImage      Pointer to BMP file
  @param[in]      BmpImageSize  Number of bytes in BmpImage
  @param[in]      GopBlt        Buffer containing GOP version of BmpImage.
  @param[in,out]  GopBltSize    Size of GopBlt in bytes.
  @param[in,out]  PixelHeight   Height of GopBlt/BmpImage in pixels
  @param[in,out]  PixelWidth    Width of GopBlt/BmpImage in pixels

  @retval EFI_SUCCESS           GopBlt and GopBltSize are returned.
  @retval EFI_UNSUPPORTED       BmpImage is not a valid *.BMP image
  @retval EFI_BUFFER_TOO_SMALL  The passed in GopBlt buffer is not big enough.
                                GopBltSize will contain the required size.
  @retval EFI_OUT_OF_RESOURCES  No enough buffer to allocate.

**/

EFI_STATUS
ConvertBmpToGopBlt (
  IN     VOID      *BmpImage,
  IN     UINTN     BmpImageSize,
  IN OUT VOID      **GopBlt,
  IN OUT UINTN     *GopBltSize,
  OUT UINTN        *PixelHeight,
  OUT UINTN        *PixelWidth
  )
{
  EFI_STATUS                    Status = EFI_SUCCESS;
  UINT8                         *Image;
  UINT8                         *ImageHeader;
  BMP_IMAGE_HEADER              *BmpHeader;
  BMP_COLOR_MAP                 *BmpColorMap;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *BltBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Blt;
  UINT64                        BltBufferSize;
  UINTN                         Index;
  UINTN                         Height;
  UINTN                         Width;
  UINTN                         ImageIndex;
  UINT32                        DataSizePerLine;
  BOOLEAN                       IsAllocated;
  UINT32                        ColorMapNum;
  UINTN                         MemPages;

  if (sizeof (BMP_IMAGE_HEADER) > BmpImageSize) {
    return EFI_INVALID_PARAMETER;
  }

  BmpHeader = (BMP_IMAGE_HEADER *) BmpImage;

  if (BmpHeader->CharB != 'B' || BmpHeader->CharM != 'M') {
    return EFI_UNSUPPORTED;
  }

  //
  // Doesn't support compress.
  //
  if (BmpHeader->CompressionType != 0) {
    return EFI_UNSUPPORTED;
  }

  //
  // Only support BITMAPINFOHEADER format.
  // BITMAPFILEHEADER + BITMAPINFOHEADER = BMP_IMAGE_HEADER
  //
  if (BmpHeader->HeaderSize != (sizeof (BMP_IMAGE_HEADER) - OFFSET_OF (BMP_IMAGE_HEADER, HeaderSize))) {
    return EFI_UNSUPPORTED;
  }

  //
  // The data size in each line must be 4 byte alignment.
  //
  DataSizePerLine = ((BmpHeader->PixelWidth * BmpHeader->BitPerPixel + 31) >> 3) & (~0x3);
  BltBufferSize = MultU64x32 (DataSizePerLine, BmpHeader->PixelHeight);
  if (BltBufferSize > (UINT32) ~0) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Calculate Color Map offset in the image.
  //
  Image       = BmpImage;
  BmpColorMap = (BMP_COLOR_MAP *) (Image + sizeof (BMP_IMAGE_HEADER));
  if (BmpHeader->ImageOffset < sizeof (BMP_IMAGE_HEADER)) {
    return EFI_INVALID_PARAMETER;
  }

  if (BmpHeader->ImageOffset > sizeof (BMP_IMAGE_HEADER)) {
    switch (BmpHeader->BitPerPixel) {
      case 1:
        ColorMapNum = 2;
        break;
      case 4:
        ColorMapNum = 16;
        break;
      case 8:
        ColorMapNum = 256;
        break;
      default:
        ColorMapNum = 0;
        break;
    }
    if (BmpHeader->ImageOffset - sizeof (BMP_IMAGE_HEADER) != sizeof (BMP_COLOR_MAP) * ColorMapNum) {
      return EFI_INVALID_PARAMETER;
    }
  }

  //
  // Calculate graphics image data address in the image
  //
  Image         = ((UINT8 *) BmpImage) + BmpHeader->ImageOffset;
  ImageHeader   = Image;

  //
  // Calculate the BltBuffer needed size.
  //
  BltBufferSize = MultU64x32 ((UINT64) BmpHeader->PixelWidth, BmpHeader->PixelHeight);

  //
  // Ensure the BltBufferSize * sizeof (EFI_GOP_BLT_PIXEL) doesn't overflow
  //
  if (BltBufferSize > DivU64x32 ((UINTN) ~0, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL))) {
    return EFI_UNSUPPORTED;
  }
  BltBufferSize = MultU64x32 (BltBufferSize, sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

  IsAllocated   = FALSE;
  if (*GopBlt == NULL) {
    //
    // GopBlt is not allocated by caller.
    //
    *GopBltSize = (UINTN) BltBufferSize;
    MemPages =  (*GopBltSize/ EFI_PAGE_SIZE) + 1;
    Status = PeiServicesAllocatePages (
               EfiBootServicesData,
               MemPages,
               ((EFI_PHYSICAL_ADDRESS*) GopBlt)
               );
    if (EFI_ERROR (Status)) {
      return EFI_OUT_OF_RESOURCES;
    }
    DEBUG ((DEBUG_INFO, "GopBlt = 0x%X \n", *GopBlt));
    IsAllocated = TRUE;
    if (*GopBlt == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  } else {
    //
    // GopBlt has been allocated by caller.
    //
    if (*GopBltSize < (UINTN) BltBufferSize) {
      *GopBltSize = (UINTN) BltBufferSize;
      return EFI_BUFFER_TOO_SMALL;
    }
  }

  *PixelWidth   = BmpHeader->PixelWidth;
  *PixelHeight  = BmpHeader->PixelHeight;

  //
  // Convert image from BMP to Blt buffer format
  //
  BltBuffer = *GopBlt;
  for (Height = 0; Height < BmpHeader->PixelHeight; Height++) {
    Blt = &BltBuffer[ (BmpHeader->PixelHeight - Height - 1) * BmpHeader->PixelWidth];
    for (Width = 0; Width < BmpHeader->PixelWidth; Width++, Image++, Blt++) {
      switch (BmpHeader->BitPerPixel) {
        case 1:
          //
          // Convert 1-bit (2 colors) BMP to 24-bit color
          //
          for (Index = 0; Index < 8 && Width < BmpHeader->PixelWidth; Index++) {
            Blt->Red    = BmpColorMap[ ((*Image) >> (7 - Index)) & 0x1].Red;
            Blt->Green  = BmpColorMap[ ((*Image) >> (7 - Index)) & 0x1].Green;
            Blt->Blue   = BmpColorMap[ ((*Image) >> (7 - Index)) & 0x1].Blue;
            Blt++;
            Width++;
          }

          Blt--;
          Width--;
          break;

        case 4:
          //
          // Convert 4-bit (16 colors) BMP Palette to 24-bit color
          //
          Index       = (*Image) >> 4;
          Blt->Red    = BmpColorMap[Index].Red;
          Blt->Green  = BmpColorMap[Index].Green;
          Blt->Blue   = BmpColorMap[Index].Blue;
          if (Width < (BmpHeader->PixelWidth - 1)) {
            Blt++;
            Width++;
            Index       = (*Image) & 0x0f;
            Blt->Red    = BmpColorMap[Index].Red;
            Blt->Green  = BmpColorMap[Index].Green;
            Blt->Blue   = BmpColorMap[Index].Blue;
          }
          break;

        case 8:
          //
          // Convert 8-bit (256 colors) BMP Palette to 24-bit color
          //
          Blt->Red    = BmpColorMap[*Image].Red;
          Blt->Green  = BmpColorMap[*Image].Green;
          Blt->Blue   = BmpColorMap[*Image].Blue;
          break;

        case 24:
          //
          // It is 24-bit BMP.
          //
          Blt->Blue   = *Image++;
          Blt->Green  = *Image++;
          Blt->Red    = *Image;
          break;

        default:
          //
          // Other bit format BMP is not supported.
          //
          if (IsAllocated) {
            FreePool (*GopBlt);
            *GopBlt = NULL;
          }
          return EFI_UNSUPPORTED;
          break;
      };

    }
    ImageIndex = (UINTN) (Image - ImageHeader);
    if ((ImageIndex % 4) != 0) {
      //
      // Bmp Image starts each row on a 32-bit boundary!
      //
      Image = Image + (4 - (ImageIndex % 4));
    }
  }

  return EFI_SUCCESS;
}


/**
  TODO- May not require this
  PeiGetSectionFromFv finds the file in FV and gets file Address and Size

  @param[in] NameGuid               File GUID
  @param[out] Address               Pointer to the File Address
  @param[out] Size                  Pointer to File Size

  @retval EFI_SUCCESS                Successfull in reading the section from FV
**/
EFI_STATUS
EFIAPI
PeiGetSectionFromFv (
  IN CONST  EFI_GUID        NameGuid,
  OUT VOID                  **Address,
  OUT UINT32               *Size
  )
{
  EFI_STATUS  Status;
  EFI_PEI_FIRMWARE_VOLUME_PPI          *FvPpi;
  EFI_FV_FILE_INFO                     FvFileInfo;
  EFI_PEI_FILE_HANDLE                  VbtFileHandle;
  EFI_GUID                             *VbtGuid;
  EFI_COMMON_SECTION_HEADER            *Section;
  EFI_PEI_FV_HANDLE                    FvHandle;

  Status = PeiServicesLocatePpi (
            &gEfiFirmwareFileSystem2Guid,
            0,
            NULL,
            (VOID **) &FvPpi
            );
  ASSERT_EFI_ERROR (Status);

  FvHandle = NULL;
  Status = FvPpi->FindFileByName (FvPpi, &NameGuid, &FvHandle, &VbtFileHandle);
  if (!EFI_ERROR(Status) && VbtFileHandle != NULL) {

  DEBUG ((DEBUG_INFO, "Find SectionByType \n"));

    Status = FvPpi->FindSectionByType (FvPpi, EFI_SECTION_RAW, VbtFileHandle, (VOID **) &VbtGuid);
    if (!EFI_ERROR (Status)) {

    DEBUG ((DEBUG_INFO, "GetFileInfo \n"));

      Status = FvPpi->GetFileInfo (FvPpi, VbtFileHandle, &FvFileInfo);
      Section = (EFI_COMMON_SECTION_HEADER *)FvFileInfo.Buffer;

      if (IS_SECTION2 (Section)) {
        ASSERT (SECTION2_SIZE (Section) > 0x00FFFFFF);
        *Size = SECTION2_SIZE (Section) - sizeof (EFI_COMMON_SECTION_HEADER2);
        *Address = ((UINT8 *)Section + sizeof (EFI_COMMON_SECTION_HEADER2));
      } else {
        *Size = SECTION_SIZE (Section) - sizeof (EFI_COMMON_SECTION_HEADER);
        *Address = ((UINT8 *)Section + sizeof (EFI_COMMON_SECTION_HEADER));
      }
      DEBUG ((DEBUG_INFO, "PeiGetSectionFromFv Size = %d\n", *Size));
      DEBUG ((DEBUG_INFO, "PeiGetSectionFromFv Address = 0x%x\n", *Address ));
    }
  }
  return EFI_SUCCESS;
}
