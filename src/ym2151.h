#ifndef __YM2151_H__
#define __YM2151_H__

#include <stdint.h>

#ifndef YM_BASE
	#ifdef __R38__
		#define YM_BASE 0x9fe0
	#else
		#define YM_BASE	0x9f40
	#endif
#endif

enum ym_param {
	YMVAL_TEST,YMVAL_KON,YMVAL_NE,YMVAL_NFRQ,YMVAL_CLKA1,YMVAL_CLKA2,
	YMVAL_CLK2,YMVAL_CSM,YMVAL_TRESET,YMVAL_IRQEN,YMVAL_LOADT,YMVAL_LFRQ,
	YMVAL_PMD,YMVAL_AMD,YMVAL_CT,YMVAL_W,YMVAL_R,YMVAL_L,YMVAL_FB,YMVAL_CONECT,
	YMVAL_KC,YMVAL_KF,YMVAL_PMS,YMVAL_AMS,YMVAL_DT1,YMVAL_MUL,YMVAL_TL,
	YMVAL_KS,YMVAL_AR,YMVAL_AMSEN,YMVAL_D1R,YMVAL_DT2,YMVAL_D2R,YMVAL_D1L,
	YMVAL_RR
};

typedef struct ym_interface {
	uint8_t address, data;
} ym_interface;

typedef struct ym_oper {
	uint8_t
		dt1,mul,tl,ks,ar,amsen,d1r,
		dt2,d2r,d1l,rr;
} ym_oper;

typedef struct ym_chan {
	uint8_t
		r,l,fb,connect,kc,kf,pms,ams;
		ym_oper op[4];
} ym_chan;

/*
typedef struct ym_global {
	uint8_t
		ne, nfrq, lfrq, w;
} ym_lfo;
*/

typedef struct ym_patch {
	ym_chan	chan;
	ym_oper	oper[4];
} ym_patch;

typedef struct ym_state {
	uint8_t
		ne, nfrq, lfrq, w, pmd, amd;
	ym_chan
	   voice[8];
} ym_state;

typedef struct ym_patchregs {
	uint8_t byte[26];
} ym_patchregs;


extern ym_state YM;

extern void ym_init();

// silences the voices but leaves the YM in the final state so that
// the user may plunk around with it in BASIC or whatever afterwards.
// voice=0-7 -> silence just that voice, all other values = silence all voices
extern void ym_silence(const uint8_t voice);

extern void ym_set_voice(const uint8_t voice, const ym_patch *patch);
extern void ym_update_voice(const uint8_t voice, const ym_patch *patch);
extern void ym_get_patchregs(const uint8_t voice, const ym_patchregs *regs);

// maybe these don't need to be exported to the rest of the project?
extern void ym_patchtoregs(ym_patch *patch, ym_patchregs *regs);
extern void ym_regstopatch(ym_patch *patch, ym_patchregs *regs);

#endif
