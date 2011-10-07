#pragma once

#ifndef __SH3COMN_H__
#define __SH3COMN_H__

/* SH3 lower area regs */

#define SH3_LOWER_REGBASE (0x04000000)
#define SH3_LOWER_REGEND  (0x07ffffff)

#define INTEVT2		((0x4000000 - SH3_LOWER_REGBASE)/4)
#define PINTER_IPRC	((0x4000014 - SH3_LOWER_REGBASE)/4)
#define PCCR_PDCR	((0x4000104 - SH3_LOWER_REGBASE)/4)
#define PECR_PFCR	((0x4000108 - SH3_LOWER_REGBASE)/4)
#define PGCR_PHCR	((0x400010c - SH3_LOWER_REGBASE)/4)
#define PJCR_PKCR	((0x4000110 - SH3_LOWER_REGBASE)/4)
#define PLCR_SCPCR	((0x4000114 - SH3_LOWER_REGBASE)/4)
#define PEDR_PFDR	((0x4000128 - SH3_LOWER_REGBASE)/4)
#define PJDR_PKDR	((0x4000130 - SH3_LOWER_REGBASE)/4)

/* SH3 upper area */


#define SH3_UPPER_REGBASE (0xffffd000)
#define SH3_UPPER_REGEND  (0xffffffff)

#define SH3_ICR0_IPRA_ADDR	((0xfffffee0 - SH3_UPPER_REGBASE)/4)
#define SH3_IPRB_ADDR		((0xfffffee4 - SH3_UPPER_REGBASE)/4)

#define SH3_TOCR_TSTR_ADDR	((0xfffffe90 - SH3_UPPER_REGBASE)/4)
#define SH3_TCOR0_ADDR		((0xfffffe94 - SH3_UPPER_REGBASE)/4)
#define SH3_TCNT0_ADDR		((0xfffffe98 - SH3_UPPER_REGBASE)/4)
#define SH3_TCR0_ADDR		((0xfffffe9c - SH3_UPPER_REGBASE)/4)
#define SH3_TCOR1_ADDR		((0xfffffea0 - SH3_UPPER_REGBASE)/4)
#define SH3_TCNT1_ADDR		((0xfffffea4 - SH3_UPPER_REGBASE)/4)
#define SH3_TCR1_ADDR		((0xfffffea8 - SH3_UPPER_REGBASE)/4)
#define SH3_TCOR2_ADDR		((0xfffffeac - SH3_UPPER_REGBASE)/4)
#define SH3_TCNT2_ADDR		((0xfffffeb0 - SH3_UPPER_REGBASE)/4)
#define SH3_TCR2_ADDR		((0xfffffeb4 - SH3_UPPER_REGBASE)/4)
#define SH3_TCPR2_ADDR		((0xfffffeb8 - SH3_UPPER_REGBASE)/4)
#define SH3_TRA_ADDR		((0xffffffd0 - SH3_UPPER_REGBASE)/4)
#define SH3_EXPEVT_ADDR		((0xffffffd4 - SH3_UPPER_REGBASE)/4)
#define SH3_INTEVT_ADDR		((0xffffffd8 - SH3_UPPER_REGBASE)/4)

#endif /* __SH3COMN_H__ */

