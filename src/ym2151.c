#include "ym2151.h"

// declare the hidden functions so the "exported" ones can use them.
//uint8_t getReg(const uint8_t p_reg);
void writeYM(uint8_t a, uint8_t d);

#define YMI (*(ym_interface*) YM_BASE)

// LUTS for YM registers: base addr, mask, bitshift
enum regmap_fields { RMAP_ADDR, RMAP_MASK, RMAP_SHIFT, RMAP_count };

static const uint8_t regmap[YMVAL_count][RMAP_count] = {
	{0x01, 0xff, 0}, {0x08, 0xff, 0}, {0x0f, 0x80, 7},	// TEST,KON,NE
	{0x0f, 0x1f, 0}, {0x10, 0xff, 0}, {0x11, 0x03, 0},	// NFRQ,CLKA1,CLKA2
	{0x12, 0xff, 0}, {0x14, 0x80, 7}, {0x14, 0x30, 4},	// CLKB,CSM,TRESET
	{0x14, 0x0c, 2}, {0x14, 0x03, 0}, {0x18, 0xff, 0},	// IRQEN,LOADT,LFRQ
	{0x19, 0x7f, 0}, {0x19, 0x7f, 0}, {0x1b, 0xc0, 6},	// PMD,AMD,CT
	{0x1b, 0x03, 0},									// W
	{0x20, 0x80, 7}, {0x20, 0x40, 6}, {0x20, 0x38, 3},	// R, L, FB
	{0x20, 0x07, 0}, {0x28, 0x7f, 0}, {0x30, 0xfc, 2},	// CONECT, KC, KF
	{0x38, 0x70, 4}, {0x38, 0x03, 0},					// PMS, AMS
	{0x40, 0x70, 4}, {0x40, 0x0f, 0}, {0x60, 0x7f, 0},	// DT1, MUL, TL
	{0x80, 0xc0, 6}, {0x80, 0x1f, 0}, {0xa0, 0x80, 7},	// KS, AR, AMSEN
	{0xa0, 0x1f, 0}, {0xc0, 0xc0, 6}, {0xc0, 0x1f, 0},	// D1R, DT2, D2R
	{0xe0, 0xf0, 4}, {0xe0, 0x0f, 0}					// D1L, RR
};

#define YMREG(YM_PARAM)		(regmap[(YM_PARAM)][RMAP_ADDR])
#define VALMASK(YM_PARAM)	(regmap[(YM_PARAM)][RMAP_MASK])
#define VALSHIFT(YM_PARAM)	(regmap[(YM_PARAM)][RMAP_SHIFT])


ym_state YM = {};

uint8_t YMreg[256] = {};

void ym_init()
{
	#define PMDAMD	YMREG(YMVAL_AMD)
	#define KON		YMREG(YMVAL_KON)

	uint8_t i,chan,param;

	// Initialize the YM2151 chip and shadow registers

	// set all YM registers to 0
	while (++i != 0) {
		writeYM(i,0); // it's okay to miss i=0. OPM does not use reg 0.
		YMreg[i] = 0;
	}

	// write 0 to PMD (which gets missed above because the AMD/PMD register
	// uses the value's bit7 to determine which parameter is being written.
	writeYM(PMDAMD,0x80);

	// Initialize the shadow registers / send KeyUP to all 8 channels
	// (doing these both in an 8-iteration loop because in YM, things come
	// in 8s quite a lot....)
	for (chan=0; chan < 8 ; chan++)
	{
		writeYM(KON,chan);
		if (chan == 0)
			for (param=0; param<YMVAL_numglobal; param++)
				YM.global[param] = 0;
		for (param=0; param < YMVAL_numchan; param++)
			YM.channel[chan][param] = 0;
		for (param=0; param < YMVAL_numoper; param++)
		{
			YM.oper[0][chan][param] = 0;
			YM.oper[1][chan][param] = 0;
			YM.oper[2][chan][param] = 0;
			YM.oper[3][chan][param] = 0;
		}
	}
}

void ym_silence(const uint8_t voice)
{
	uint8_t i, op;

	uint8_t first = voice;
	uint8_t last  = voice+1;

	if (voice > 7) 	{ first = 0; last  = 8;	}

	for(i=first ; i<last ; i++)
	{
		// loop checks >= 0xe0 because op is 8bit thus can never be > 0xff
		for (op=0xe0+i; op >= 0xe0 ; op+=8)
			writeYM(op, 0x0f);			// set max RR for all 4 ops
		writeYM(0x08,i); 				// keyUP
		for (op=0xe0+i; op >= 0xe0 ; op+=8)
			writeYM(op, YMreg[op]);	// restore the D1L+RR value
	}
}

#pragma warn (unused-param,push,off)
extern void ym_apply_patch(const ym_patchregs *patch, const uint8_t voice)
{
	
}

extern void ym_get_patchregs(const uint8_t voice, const ym_patchregs *regs)
{
	
}
#pragma warn (unused-param,pop)

void ym_set_param(ym_param p_param, uint8_t p_val, ...)
{
	va_list ap;
	uint8_t reg, val;
	static ym_param param;
	static uint8_t  newval;
	static uint8_t  o;
	static uint8_t  c;
	
	uint8_t *map;
	
	// cc65 docs recommend local copies of params
	// for variadic functions to not be quite so slow
	param = p_param;
	newval = p_val;	// localize the other fixed parameter (see param)

	map = (uint8_t*)regmap[param];
	o = 0;
	c = 0;
	
	// NOP if invalid parameter
//	if (param >= YMVAL_count || param < 0) return;
	
	if (param >= YMVAL_perchan)
	{
		va_start(ap,p_val);
		c = va_arg(ap, uint8_t); // regs >= 0x20 all use reg+chan. Get chan.
		c &= 7; // just to protect me from myself....
		if (param >= YMVAL_oper)
		{
			o = va_arg(ap, uint8_t); // regs >= 0x40 are operator regs
			o &= 3; // just to protect me from myself....
			YM.oper[o][c][param-YMVAL_oper] = newval & (map[RMAP_MASK] >> map[RMAP_SHIFT]);
		}
		else
			YM.channel[c][param-YMVAL_perchan] = newval & (map[RMAP_MASK] >> map[RMAP_SHIFT]);
		va_end(ap);
	}
	else
		YM.global[param] = newval & (map[RMAP_MASK] >> map[RMAP_SHIFT]);
	o = o << 3;	// oper offset is always operID * 8. (even when o=0) :-D

	newval = newval << map[RMAP_SHIFT];
	newval &= map[RMAP_MASK];

	reg = map[RMAP_ADDR] + o + c;
	val = YMreg[reg];
	
	if (param == YMVAL_PMD) newval |= 0x80; // special case for PMD/AMD register
	
	val &= ~map[RMAP_MASK];
	val |= newval;
	writeYM(reg,val);
	
	YMreg[reg] = val;
}

// hidden functions ---------------------------------------------

void writeYM(uint8_t a, uint8_t d)
{
	YMI.address = a;
	while ((int8_t)YMI.data < 0) {} // wait for busy status flag to be clear
	YMI.data = d;
}

#if(0)
uint8_t getReg(const uint8_t p_reg)
{
	static uint8_t group, v, o, val, reg;

	reg = p_reg;
	v = reg & 0x07;
	o = (reg & 0x18) >> 3;
	group = (reg & 0xe0) >> 5;
	switch(group) {
	case 0: // global registers
		switch (reg) {
		case 0x0f: // NE+NFRQ
			return YM.ne << 7 | YM.nfrq;
		case 0x18:
			return YM.lfrq;
		case 0x19:
			return YM.pmd;
		case 0x1a: // doing what YMFM does - pretending it's a seperate reg.
			return YM.amd;
		case 0x1b: // w (lfo waveform)
			return YM.w;
		}
		break;
	case 1: // per-channel registers
		// using "o" to select which of the 4 per-channel params to return
		switch (o) {
		case 0: // R+L+FB+CON
			val = YM.voice[v].r << 7 | YM.voice[v].l << 6;
			val |= YM.voice[v].fb << 3;
			val |= YM.voice[v].connect;
			return val;
			break;
		case 1: // kc
			return YM.voice[v].kc;
			break;
		case 2: // kf
			return YM.voice[v].kf << 2;
			break;
		case 3: // PMS+AMS
			return YM.voice[v].ams | YM.voice[v].pms << 4;
			break;
		}
		break;
	case 2: // DT1+MUL
		return (YM.voice[v].op[o].dt1 << 4) | YM.voice[v].op[o].mul;
	case 3: // TL
		return YM.voice[v].op[o].tl;
	case 4: // KS+AR
		return YM.voice[v].op[o].ks << 6 | YM.voice[v].op[o].ar;
	case 5: // AMSEN+D1R
		return YM.voice[v].op[o].amsen << 7 | YM.voice[v].op[o].d1r;
	case 6: // DT2+D2R
		return YM.voice[v].op[o].dt2 << 6 | YM.voice[v].op[o].d2r;
	case 7: // D1L+RR
		return YM.voice[v].op[o].d1l << 4 | YM.voice[v].op[o].rr;
	}
}
#endif
