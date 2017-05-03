/************************************************************************************
 * include/nuttx/wireless/ieee802154/ieee802154_ioctl.h
 * IEEE802.15.4 character driver IOCTL commands
 *
 *   Copyright (C) 2017 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ************************************************************************************/

/* This file includes common definitions to be used in all wireless character drivers
 * (when applicable).
 */

#ifndef __INCLUDE_NUTTX_WIRELESS_IEEE802154_IEEE802154_IOCTL_H
#define __INCLUDE_NUTTX_WIRELESS_IEEE802154_IEEE802154_IOCTL_H

/************************************************************************************
 * Included Files
 ************************************************************************************/

#include <nuttx/config.h>
#include <nuttx/wireless/ioctl.h>

#include <nuttx/wireless/ieee802154/ieee802154_mac.h>

#ifdef CONFIG_WIRELESS_IEEE802154

/************************************************************************************
 * Pre-processor Definitions
 ************************************************************************************/

/* IEEE 802.15.4 Radio Character Driver IOCTL commands ******************************/
/* None defined */

/* IEEE 802.15.4 MAC Character Driver IOCTL commands ********************************/


#define MAC802154IOC_MCPS_REGISTER    _WLCIOC(IEEE802154_FIRST)
#define MAC802154IOC_MLME_REGISTER    _WLCIOC(IEEE802154_FIRST+1)

/************************************************************************************
 * Public Types
 ************************************************************************************/

struct mac802154dev_notify_s
{
  uint8_t mn_signo;       /* Signal number to use in the notification */
};

struct mac802154dev_txframe_s
{
  struct ieee802154_frame_meta_s meta;
  FAR uint8_t *payload;
};

#endif /* CONFIG_WIRELESS_IEEE802154 */
#endif /* __INCLUDE_NUTTX_WIRELESS_IEEE802154_IEEE802154_IOCTL_H */
