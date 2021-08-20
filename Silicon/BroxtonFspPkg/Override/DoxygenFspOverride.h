/** @file
This file contains doxygen commands definitions for creating
ClientSiliconPkg override documentation

 @copyright
  INTEL CONFIDENTIAL
  Copyright 2016 Intel Corporation.

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

/**
 @page FSP Package override

  FSP Override tags (FSPO-xxxx) document issues that Royal Park team has resolved with an override of Royal Park.

  The Active FSPO table lists every issue which is currently resolved with an override. It is a catalog of all such existing overrides.

  The Retired FSPO table lists issues which previously required such overrides. It is purely historical as these overrides have all been deleted (are no longer needed).
<BR>
<CENTER>
<STRONG>Active FSPOs</STRONG>
<BR>in BroxtonFspPkg/Override
</CENTER>

<table border="1" style="width:1200px">
<tr>
  <th>Tag</th>
  <th>Client HSD</th>
  <th>BP HSD</th>
  <th>Status / Planned EOL</th>
  <th>Description</th>
</tr>
  <tr> <td>FSPO-0001</td> <td></td>    <td></td>        <td></td>           <td>1604149023:APL: During POST if system BIOS pass NULL of UPD region pointer to FspMemoryInit & FspSiliconInit then System will hang.</td> </tr>
  <tr> <td>FSPO-APL-0002</td> <td></td>    <td></td>        <td></td>           <td>1504005677:Enabling Performance measurement for FSP Binary - Publish FSP performance records through FPDT.</td> </tr>
  </table>

<BR>
<CENTER>
<STRONG>Retired FSPOs</STRONG>
<BR>formerly in BroxtonFspPkg/Override
</CENTER>

<table border="1" style="width:1200px">
<tr>
  <th>Tag</th>
  <th>Client HSD</th>
  <th>BP HSD</th>
  <th>Actual EOL</th>
  <th>Description</th>
</tr>
  </table>

**/

