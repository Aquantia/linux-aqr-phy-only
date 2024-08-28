/* SPDX-License-Identifier: GPL-2.0
 * HWMON driver for Aquantia PHY
 *
 * Author: Nikita Yushchenko <nikita.yoush@cogentembedded.com>
 * Author: Andrew Lunn <andrew@lunn.ch>
 * Author: Heiner Kallweit <hkallweit1@gmail.com>
 */

#include <linux/device.h>
#include <linux/phy.h>

#if IS_ENABLED(CONFIG_AQUANTIA_MACSEC)
#include "macsec/macsec.h"
#include "aqr_macsec/aqr_macsec.h"
#endif

#if IS_REACHABLE(CONFIG_HWMON)
int aqr_hwmon_probe(struct phy_device *phydev);
#else
static inline int aqr_hwmon_probe(struct phy_device *phydev) { return 0; }
#endif

/*Include XFI mode support*/
#define USXGMII_IN_XFI_MODE

#define MDIO_C22EXT_WAKEUP_FRAME_DETECT         0xC355
#define ENABLE_WAKEUP_FRAME_DETECT              BIT(0)

#define MDIO_C22EXT_MAGIC_FRAME_DETECT          0xC356
#define ENABLE_MAGIC_FRAME_DETECT               BIT(0)

#define MDIO_C22EXT_WOL_INTN_TRIGGER            0xC357
#define ENABLE_WOL_INTN_TRIGGER					BIT(15)

/* MDIO_MMD_C22EXT */
#define MDIO_C22EXT_STAT_SGMII_RX_GOOD_FRAMES		0xd292
#define MDIO_C22EXT_STAT_SGMII_RX_BAD_FRAMES		0xd294
#define MDIO_C22EXT_STAT_SGMII_RX_FALSE_CARRIER		0xd297
#define MDIO_C22EXT_STAT_SGMII_TX_GOOD_FRAMES		0xd313
#define MDIO_C22EXT_STAT_SGMII_TX_BAD_FRAMES		0xd315
#define MDIO_C22EXT_STAT_SGMII_TX_FALSE_CARRIER		0xd317
#define MDIO_C22EXT_STAT_SGMII_TX_COLLISIONS		0xd318
#define MDIO_C22EXT_STAT_SGMII_TX_LINE_COLLISIONS	0xd319
#define MDIO_C22EXT_STAT_SGMII_TX_FRAME_ALIGN_ERR	0xd31a
#define MDIO_C22EXT_STAT_SGMII_TX_RUNT_FRAMES		0xd31b

struct aqr107_hw_stat {
	const char *name;
	int reg;
	int size;
};

#define SGMII_STAT(n, r, s) { n, MDIO_C22EXT_STAT_SGMII_ ## r, s }
static const struct aqr107_hw_stat aqr107_hw_stats[] = {
	SGMII_STAT("sgmii_rx_good_frames",	    RX_GOOD_FRAMES,	26),
	SGMII_STAT("sgmii_rx_bad_frames",	    RX_BAD_FRAMES,	26),
	SGMII_STAT("sgmii_rx_false_carrier_events", RX_FALSE_CARRIER,	 8),
	SGMII_STAT("sgmii_tx_good_frames",	    TX_GOOD_FRAMES,	26),
	SGMII_STAT("sgmii_tx_bad_frames",	    TX_BAD_FRAMES,	26),
	SGMII_STAT("sgmii_tx_false_carrier_events", TX_FALSE_CARRIER,	 8),
	SGMII_STAT("sgmii_tx_collisions",	    TX_COLLISIONS,	 8),
	SGMII_STAT("sgmii_tx_line_collisions",	    TX_LINE_COLLISIONS,	 8),
	SGMII_STAT("sgmii_tx_frame_alignment_err",  TX_FRAME_ALIGN_ERR,	16),
	SGMII_STAT("sgmii_tx_runt_frames",	    TX_RUNT_FRAMES,	22),
};
#define AQR107_SGMII_STAT_SZ ARRAY_SIZE(aqr107_hw_stats)

struct aqr107_priv {
	u64 sgmii_stats[AQR107_SGMII_STAT_SZ];

#if IS_ENABLED(CONFIG_AQUANTIA_MACSEC)
	struct aqr_port port;
	struct aqr_macsec_cfg macsec_cfg;
#endif
	u32 wol;
};
