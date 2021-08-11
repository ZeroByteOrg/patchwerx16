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
	// global parameters
	YMVAL_TEST,YMVAL_KON,YMVAL_NE,YMVAL_NFRQ,YMVAL_CLKA1,YMVAL_CLKA2,
	YMVAL_CLKB,YMVAL_CSM,YMVAL_TRESET,YMVAL_IRQEN,YMVAL_LOADT,YMVAL_LFRQ,
	YMVAL_PMD,YMVAL_AMD,YMVAL_CT,YMVAL_W,
	
	// per-channel parameters
	YMVAL_R,YMVAL_L,YMVAL_FB,YMVAL_CONECT,
	YMVAL_KC,YMVAL_KF,YMVAL_PMS,YMVAL_AMS,
	
	// per-operator parameters
	YMVAL_DT1,YMVAL_MUL,YMVAL_TL,YMVAL_KS,YMVAL_AR,YMVAL_AMSEN,
	YMVAL_D1R,YMVAL_DT2,YMVAL_D2R,YMVAL_D1L,YMVAL_RR,
	
	YMVAL_COUNT // terminator
};

typedef enum ym_parameters ym_param;

// THIS IS A STRUCT TO BE USED AS THE ELEMENT TYPE FOR A REG DECODER LUT
// -- WHICH MAY NOT ACTUALLY BE A NECESSARY DATA STRUCTURE.... ?
typedef struct ym_param_details {
	uint8_t base;		//base register address:
						// 	global = exact,
						// 	chan   = 0x20,0x28,0x30,0x38
						//  op     = 0x40,0x60,0x80,0xa0,0xc0,0xe0
	uint8_t chanshift;	// globals=8, channel=0, opers=8
	uint8_t opshift;	// globals=8, channel=0, opers=3
	uint8_t	mask;
} ym_param_details;

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

typedef struct ym_patch {
	ym_chan	chan;
	ym_oper	oper[4];
} ym_patch;

typedef struct ym_state {
	uint8_t
		test, kon, ne, nfrq, clka1, clka2, clkb,
		lfrq, pmd, amd, ct, w;
	ym_chan
	   voice[8];
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

extern void ym_setparam(ym_param param, uint8_t p_val, ...);

extern void ym_setparam_global(ym_param param, uint8_t p_val);
extern void ym_setparam_chan(ym_param param, uint8_t chan, uint8_t p_val);
extern void ym_setparam_oper(ym_param param, uint8_t chan, uint8_t op, uint8_t p_val);


// maybe these don't need to be exported to the rest of the project?
extern void ym_patchtoregs(ym_patch *patch, ym_patchregs *regs);
extern void ym_regstopatch(ym_patch *patch, ym_patchregs *regs);

#endif
