// license:BSD-3-Clause
// copyright-holders:Robbbert
/***************************************************************************

Jonos Escort

2013-09-12 Skeleton driver

It seems there were about 6 models of Escort, mostly Z-80A based running
CP/M. However, this one appears to be an 8085-based terminal.

Haven't found any info.

There are interrupt handlers at 5.5 (0x002c) and 6.5 (0x0034).


****************************************************************************/

#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "machine/keyboard.h"
#include "screen.h"


class jonos_state : public driver_device
{
public:
	jonos_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
		, m_p_videoram(*this, "videoram")
		, m_p_chargen(*this, "chargen")
		{ }

	DECLARE_READ8_MEMBER(keyboard_r);
	DECLARE_WRITE8_MEMBER(cursor_w);
	u32 screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);
	void kbd_put(u8 data);

	void jonos(machine_config &config);
	void jonos_mem(address_map &map);
private:
	u8 m_framecnt;
	u8 m_term_data;
	u8 m_curs_ctrl;
	u16 m_curs_pos;
	virtual void machine_reset() override;
	required_device<cpu_device> m_maincpu;
	required_shared_ptr<u8> m_p_videoram;
	required_region_ptr<u8> m_p_chargen;
};



ADDRESS_MAP_START(jonos_state::jonos_mem)
	ADDRESS_MAP_UNMAP_HIGH
	AM_RANGE(0x0000, 0x0fff) AM_ROM AM_REGION("roms", 0)
	AM_RANGE(0x1800, 0x27ff) AM_RAM AM_SHARE("videoram")
	AM_RANGE(0x3000, 0x3001) AM_WRITE(cursor_w) // unknown device
	AM_RANGE(0x4000, 0x4001) // unknown device
	AM_RANGE(0x5000, 0x5003) AM_READ(keyboard_r) // unknown device
	AM_RANGE(0x6000, 0x6001) // unknown device
ADDRESS_MAP_END

/* Input ports */
static INPUT_PORTS_START( jonos )
INPUT_PORTS_END

void jonos_state::kbd_put(u8 data)
{
	m_term_data = data;
}

READ8_MEMBER( jonos_state::keyboard_r )
{
	if (offset == 0)
	{
		u8 data = m_term_data;
		m_term_data = 0;
		return data;
	}
	else
	if (m_term_data && offset == 2)
		return 0x20;

	return 0;
}

WRITE8_MEMBER( jonos_state::cursor_w )
{
	if (offset == 1) // control byte
		m_curs_ctrl = (data == 0x80) ? 1 : 0;
	else
	if (m_curs_ctrl == 1)
	{
		m_curs_pos = (m_curs_pos & 0xff00) | data;
		m_curs_ctrl++;
	}
	else
	if (m_curs_ctrl == 2)
		m_curs_pos = (m_curs_pos & 0xff) | (data << 8);
}

void jonos_state::machine_reset()
{
	m_curs_ctrl = 0;
	m_curs_pos = 0;
	m_term_data = 0;
}

uint32_t jonos_state::screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect)
{
	m_framecnt++;
	u8 y,ra,chr,gfx,inv;
	u16 sy=0,x;
	u16 ma = (m_p_videoram[0x7da] + (m_p_videoram[0x7db] << 8)) & 0x7ff;

	for (y = 0; y < 24; y++)
	{
		for (ra = 0; ra < 12; ra++)
		{
			u16 *p = &bitmap.pix16(sy++);
			u16 cpos = y << 8;

			for (x = ma; x < ma + 80; x++)
			{
				chr = m_p_videoram[x];
				inv = (BIT(chr, 7) ^ ((cpos == m_curs_pos) && BIT(m_framecnt, 5))) ? 0xff : 0;
				chr &= 0x7f;

				if (ra < 8)
					gfx = m_p_chargen[(chr<<3) | ra ] ^ inv;
				else
					gfx = m_p_chargen[(chr<<3) | (ra&7) | 0x400] ^ inv;

				/* Display a scanline of a character */
				*p++ = BIT(gfx, 0);
				*p++ = BIT(gfx, 1);
				*p++ = BIT(gfx, 2);
				*p++ = BIT(gfx, 3);
				*p++ = BIT(gfx, 4);
				*p++ = BIT(gfx, 5);
				*p++ = BIT(gfx, 6);
				*p++ = BIT(gfx, 7);
				cpos++;
			}
		}
		ma+=80;
		if (ma > 0x77f)
			ma -= 0x780;
	}
	return 0;
}

/* F4 Character Displayer */
static const gfx_layout jonos_charlayout =
{
	8, 12,                   /* 8 x 12 characters */
	128,                    /* 128 characters */
	1,                  /* 1 bits per pixel */
	{ 0 },                  /* no bitplanes */
	/* x offsets */
	{ 7, 6, 5, 4, 3, 2, 1, 0 },
	/* y offsets */
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8, 1024*8, 1025*8, 1026*8, 1027*8 },
	8*8                    /* every char takes 8 bytes */
};

static GFXDECODE_START( jonos )
	GFXDECODE_ENTRY( "chargen", 0x0000, jonos_charlayout, 0, 1 )
GFXDECODE_END


MACHINE_CONFIG_START(jonos_state::jonos)
	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", I8085A, XTAL(16'000'000) / 4)
	MCFG_CPU_PROGRAM_MAP(jonos_mem)

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(2500)) /* not accurate */
	MCFG_SCREEN_UPDATE_DRIVER(jonos_state, screen_update)
	MCFG_SCREEN_SIZE(640, 288)
	MCFG_SCREEN_VISIBLE_AREA(0, 639, 0, 287)
	MCFG_SCREEN_PALETTE("palette")

	MCFG_GFXDECODE_ADD("gfxdecode", "palette", jonos)
	MCFG_PALETTE_ADD_MONOCHROME("palette")

	MCFG_DEVICE_ADD("keyboard", GENERIC_KEYBOARD, 0)
	MCFG_GENERIC_KEYBOARD_CB(PUT(jonos_state, kbd_put))
MACHINE_CONFIG_END


/* ROM definition */
ROM_START( jonos )
	ROM_REGION( 0x1000, "roms", 0 )
	ROM_LOAD( "jocrts04.rom", 0x0000, 0x1000, CRC(6a3d4048) SHA1(bdb0bc2c8c4e54261376e4ea3c2827d00d3d89bc) )

	ROM_REGION( 0x0800, "user1", 0 ) // Z80 code for serial & parallel ports
	ROM_LOAD( "joz80.rom",    0x0000, 0x0800, CRC(de1e8998) SHA1(270df08caf30cc8f18e740ef05dc8727a925a5da) )

	ROM_REGION( 0x0800, "chargen", 0 )
	ROM_LOAD( "jochset0.rom", 0x0000, 0x0800, CRC(1d8e9640) SHA1(74f3604acc71f9bc1e1f9479f6438feda79293a2) )
ROM_END

/* Driver */

//   YEAR   NAME    PARENT  COMPAT   MACHINE  INPUT  CLASS        INIT     COMPANY  FULLNAME  FLAGS
COMP( 198?, jonos,  0,      0,       jonos,   jonos, jonos_state, 0,       "Jonos", "Escort", MACHINE_NOT_WORKING | MACHINE_NO_SOUND)
