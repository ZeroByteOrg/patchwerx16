#include "ym2151.h"
#include <stdio.h>

// declare hidden functions so the "exported" ones can use them.
uint8_t getReg(const uint8_t p_reg);
void writeYM(uint8_t a, uint8_t d);

#define YMI (*(ym_interface*) YM_BASE)

// LUTS for YM registers: base addr, mask, bitshift
enum regmap_fields { RM_ADDR, RM_MASK, RM_SHIFT, RM_count };

static const uint8_t regmap[YMVAL_COUNT][RM_count] = {
	{0x01, 0xff, 0}, {0x08, 0xff, 0}, {0x0f, 0x01, 7},	// TEST,KON,NE
	{0x0f, 0x1f, 0}, {0x10, 0xff, 0}, {0x11, 0x03, 0},	// NFRQ,CLKA1,CLKA2
	{0x12, 0xff, 0}, {0x14, 0x01, 7}, {0x14, 0x03, 4},	// CLKB,CSM,TRESET
	{0x14, 0x03, 2}, {0x14, 0x03, 0}, {0x18, 0xff, 0},	// IRQEN,LOADT,LFRQ
	{0x19, 0x7f, 0}, {0x19, 0x7f, 0}, {0x1b, 0x03, 6},	// PMD,AMD,CT
	{0x1b, 0x03, 0},									// W
	{0x20, 0x01, 7}, {0x20, 0x01, 6}, {0x20, 0x07, 3},	// R, L, FB
	{0x20, 0x07, 0}, {0x28, 0x7f, 0}, {0x30, 0x3f, 2},	// CONECT, KC, KF
	{0x38, 0x07, 4}, {0x38, 0x03, 0},					// PMS, AMS
	{0x40, 0x07, 4}, {0x40, 0x0f, 0}, {0x60, 0x7f, 0},	// DT1, MUL, TL
	{0x80, 0x03, 6}, {0x80, 0x1f, 0}, {0xa0, 0x01, 7},	// KS, AR, AMSEN
	{0xa0, 0x1f, 0}, {0xc0, 0x03, 6}, {0xc0, 0x1f, 0},	// D1R, DT2, D2R
	{0xe0, 0x0f, 4}, {0xe0, 0x0f, 0}					// D1L, RR
};

ym_state YM = {};

void ym_init()
{
	uint8_t i = 0;
	while (++i != 0)
	{
		writeYM(i,0);
	}
	// TODO: initialize the ym_state YM structure as well....
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
			writeYM(op, getReg(op));	// restore the D1L+RR value
	}
}

extern void ym_apply_patch(const ym_patchregs *patch, const uint8_t voice);
// configures voice using a 26-byte patch. Applies to ym_state and real HW.

extern void ym_get_patchregs(const uint8_t voice, const ym_patchregs *regs);
// configures a 26-byte patch from the specified voice in ym_state.


/*
 *  >>>>>>>>>> THIS IS WHERE I WAS WORKING <<<<<<<<<<<<<< //
 *  notes to self: I think I may actually not do it this way, but
 * just make a few fn's to return pointers to parameters in the ym_state
 * along with the corresponding enum?
*/

void ym_setparam(ym_param param, uint8_t p_val, ...)
{
	va_list ap;
	uint8_t reg, val;
	static ym_param p;
	static uint8_t  v;
	static uint8_t  o = 0;
	static uint8_t  c = 0;
	
	uint8_t *map = (uint8_t*)regmap[p];
	
	p = param; // cc65 docs recommend local copies of params
	v = p_val; // for variadic functions to not be quite so slow
	
	va_start(ap,p_val);
	if (reg >= YMVAL_R)
	{
		c = va_arg(ap, uint8_t); // regs >= 0x20 all use reg+chan. Get chan.
		c &= 7; // just to protect me from myself....
		if (reg >= YMVAL_DT1)
		{
			o = va_arg(ap, uint8_t); // regs >= 0x40 are operator regs
			o &= 3; // just to protect me from myself....
		}
	}
	else
	{
		// hmmm - how TF do I get from an enum to a struct member name???
		

	}
	// code is currently broken because I've forgotten to actually update the shadow struct!
	va_end(ap);
	o = o << 3;	// oper offset is always operID * 8. (even when o=0) :-D
	reg = map[RM_ADDR] + o + c;
	v &= map[RM_MASK];
	v = v << map[RM_SHIFT];
	if (p == YMVAL_PMD) v |= 0x80; // special case for PMD/AMD register
	val = getReg(reg);
printf("old=%02x  new=",val);
	val &= ~map[RM_MASK];
	val |= v;
printf("%02x --> reg[%02x]\n",val,reg);
	writeYM(reg,val);
}

#pragma warn (unused-param,push,off)

void ym_setparam_global(ym_param param, uint8_t p_val)
{
	// cc65 docs recommend using local copies of fixed params
	// in variadic functions to improve speed.

	// ne, nfrq, lfrq, w, pmd, amd;
	uint8_t reg;
	uint8_t val = p_val;

	switch (param) {
	case YMVAL_NE:
		val = val << 7;
		val |= YM.nfrq;
		reg = 0x0f;
		break;
	case YMVAL_NFRQ:
		val &= 0x1f;
		val |= YM.ne << 7;
		reg = 0x0f;
		break;
	case YMVAL_LFRQ:
		reg = 0x18;
		break;
	case YMVAL_W:
		val &= 1; // we're ignoring CT portion of register
		reg = 0x1b;
		break;
	case YMVAL_PMD:
		val &= 0x7f;
		val |= 0x80; // PMD / AMD use same reg, but MSB=1 for PMD...
		reg = 0x19;
		break;
	case YMVAL_AMD:
		val &= 0x7f; // ... and MSB=0 for AMD.
		reg = 0x19;
		break;
	default:
		//return 0;
		return;
	}
	writeYM(reg, val);
}

void ym_setparam_chan(ym_param param, uint8_t chan, uint8_t p_val)
{
	uint8_t reg;
	uint8_t val = p_val;

	switch(param) {
	case YMVAL_R:
		val = val << 7;
		val |= YM.voice[chan].l << 6;
		val |= YM.voice[chan].fb << 3;
		val |= YM.voice[chan].connect;
		reg = 0x20+chan;
		break;
	case YMVAL_L:
		val &= 0x01;
		val = val << 6;
		val |= YM.voice[chan].r << 7;
		val |= YM.voice[chan].fb << 3;
		val |= YM.voice[chan].connect;
		reg = 0x20+chan;
		break;
	case YMVAL_FB:
		val &= 0x07;
		val = val << 3;
		val |= YM.voice[chan].r << 7;
		val |= YM.voice[chan].l << 6;
		val |= YM.voice[chan].connect;
		reg = 0x20+chan;
		break;
	case YMVAL_CONECT:
		val &= 0x07;
		val |= YM.voice[chan].r << 7;
		val |= YM.voice[chan].l << 6;
		val |= YM.voice[chan].fb << 3;
		reg = 0x20+chan;
		break;
	case YMVAL_KC:
		val = val & 0x7f;
		reg = 0x28+chan;
		break;
	case YMVAL_KF:
		val = val << 2;
		reg = 0x30+chan;
		break;
	case YMVAL_PMS:
		val = YM.voice[chan].ams | ((val & 0x07) << 4);
		reg = 0x38+chan;
		break;
	case YMVAL_AMS:
		val = YM.voice[chan].pms | val & 0x02;
		reg = 0x38+chan;
		break;
	default:
		return;
	}
	writeYM(reg, val);
}

void ym_setparam_oper(ym_param param, uint8_t chan, uint8_t op, uint8_t p_val)
{
	uint8_t val = p_val;
	uint8_t reg = chan + 8 * op; // pre-compute operator's offset into reg range...

	switch(param) {
	case YMVAL_DT1:
		val &= 0x07;
		val = val << 4;
		val |= YM.voice[chan].op[op].mul;
		reg += 0x40;
		break;
	case YMVAL_MUL:
		val &= 0x0f;
		val |= YM.voice[chan].op[op].dt1 << 4;
		reg += 0x40;
		break;
	case YMVAL_TL:
		val &= 0x7f;
		reg += 0x60;
		break;
	case YMVAL_KS:
		val &= 0x03;
		val = val << 6;
		val |= YM.voice[chan].op[op].ar;
		reg += 0x80;
		break;
	case YMVAL_AR:
		val &= 0x1f;
		val |= YM.voice[chan].op[op].ks << 6;
		reg += 0x80;
		break;
	case YMVAL_AMSEN:
		val = val << 7;
		val |= YM.voice[chan].op[op].d1r;
		reg += 0xa0;
		break;
	case YMVAL_D1R:
		val &= 0x1f;
		val |= YM.voice[chan].op[op].amsen << 7;
		reg += 0xa0;
		break;
	case YMVAL_DT2:
		val = val << 6;
		val |= YM.voice[chan].op[op].d2r;
		reg += 0xc0;
		break;
	case YMVAL_D2R:
		val &= 0x1f;
		val |= YM.voice[chan].op[op].dt2 << 6;
		reg += 0xc0;
		break;
	case YMVAL_D1L:
		val = val << 4;
		val |= YM.voice[chan].op[op].rr;
		reg += 0xe0;
		break;
	case YMVAL_RR:
		val &= 0x0f;
		val |= YM.voice[chan].op[op].d1l << 4;
		reg += 0xe0;
		break;
	default:
		return;
	}
	writeYM(reg, val);
}

#pragma warn (unused-param,pop)


// hidden functions ---------------------------------------------

void writeYM(uint8_t a, uint8_t d)
{
	YMI.address = a;
	while ((int8_t)YMI.data < 0) {} // wait for busy status flag to be clear
	YMI.data = d;
}

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
