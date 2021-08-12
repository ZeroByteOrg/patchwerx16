#ifndef __YM2151_H__
#define __YM2151_H__

#include <stdint.h>
#include <stdarg.h>

#ifndef YM_BASE
	#ifdef __R38__
		#define YM_BASE 0x9fe0
	#else
		#define YM_BASE	0x9f40
	#endif
#endif


enum ym_parameters {
	//Global Parameters:
	YMVAL_TEST,YMVAL_KON,YMVAL_NE,YMVAL_NFRQ,YMVAL_CLKA1,YMVAL_CLKA2,
	YMVAL_CLKB,YMVAL_CSM,YMVAL_TRESET,YMVAL_IRQEN,YMVAL_LOADT,YMVAL_LFRQ,
	YMVAL_PMD,YMVAL_AMD,YMVAL_CT,YMVAL_W,

	//Per-Channel Parameters:
	YMVAL_R,YMVAL_L,YMVAL_FB,YMVAL_CONECT,
	YMVAL_KC,YMVAL_KF,YMVAL_PMS,YMVAL_AMS,

	//Per-Operator Parameters:
	YMVAL_DT1,YMVAL_MUL,YMVAL_TL,YMVAL_KS,YMVAL_AR,YMVAL_AMSEN,
	YMVAL_D1R,YMVAL_DT2,YMVAL_D2R,YMVAL_D1L,YMVAL_RR,
	YMVAL_count
};

enum ym_param_indexes {
	YMVAL_global	= 0,
	YMVAL_perchan	= YMVAL_R,
	YMVAL_oper		= YMVAL_DT1
};

enum ym_param_counts {
	YMVAL_numglobal	= YMVAL_R,
	YMVAL_numchan	= YMVAL_oper - YMVAL_perchan + 1,
	YMVAL_numoper	= YMVAL_count - YMVAL_oper + 1,
};

typedef enum ym_parameters ym_param;

typedef struct ym_interface {
	uint8_t address, data;
} ym_interface;

#if(0)
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

typedef struct ym_patch {
	ym_chan	chan;
	ym_oper	oper[4];
} ym_patch;

typedef struct ym_state1 {
	uint8_t
		test, kon, ne, nfrq, clka1, clka2, clkb,
		lfrq, pmd, amd, ct, w;
	ym_chan
	   voice[8];
} ym_state1;
#endif

typedef struct ym_state {
	uint8_t global[YMVAL_numglobal];
	uint8_t channel[8][YMVAL_numchan];
	uint8_t oper[4][8][YMVAL_numoper];
} ym_state;

typedef struct ym_patchregs {
	uint8_t byte[26];
} ym_patchregs;


extern ym_state YM;

extern void ym_init();
// initializaes the ym_state and the real HW

extern void ym_silence(const uint8_t voice);
// silences the voices but leaves the YM in the final state so that
// the user may plunk around with it in BASIC or whatever afterwards.
// voice=0-7 -> silence just that voice, all other values = silence all voices

extern void ym_apply_patch(const ym_patchregs *patch, const uint8_t voice);
// configures voice using a 26-byte patch. Applies to ym_state and real HW.

extern void ym_get_patchregs(const uint8_t voice, const ym_patchregs *regs);
// configures a 26-byte patch from the specified voice in ym_state.

extern void ym_set_param(ym_param p_param, uint8_t p_val, ...);
// sets YM2151 parameter p_param = p_val, updates shadow, and writes out to the chip

#endif
