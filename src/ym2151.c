#include "ym2151.h"

#define YMI (*(ym_interface*) YM_BASE)

// declare hidden functions so the "exported" ones can use them.
uint8_t getReg(const uint8_t p_reg);
void writeYM(uint8_t a, uint8_t d);

static ym_state YM = {};

// exported functions:

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

// hidden functions

void writeYM(uint8_t a, uint8_t d)
{
	YMI.address = a;
	while (YMI.data & 0x80) {} // wait for busy status flag to be clear
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


