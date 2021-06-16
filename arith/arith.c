#include "lv2/core/lv2.h"

#include <stdlib.h>
#include <math.h>

#define ARITH_URI NXS_URI "arith"

typedef enum {
	ARITH_LEFT = 0,
	ARITH_RIGHT = 1,
	ARITH_OUT = 2,
	ARITH_FUNCTION = 3,
} arith_port_index_t;

typedef enum {
	FUNC_ADD = 0,
	FUNC_SUB = 1,
	FUNC_MUL = 2,
	FUNC_DIV = 3,
	FUNC_MOD = 4,
	FUNC_POW = 5,
	FUNC_LAST = 6,
} arith_func_t;

typedef struct {
	const float *left;
	const float *right;
	float *out;
	const float *function;
} arith_data_t;

static LV2_Handle arith_instantiate(
		const LV2_Descriptor *descriptor,
		double rate,
		const char *bundle_path,
		const LV2_Feature * const *features
) {
	return (LV2_Handle) calloc(1, sizeof(arith_data_t));
}

static void arith_connect_port(
		LV2_Handle instance,
		uint32_t port,
		void *data
) {
	arith_data_t *arith = (arith_data_t *) instance;
	switch((arith_port_index_t) port) {
		case ARITH_LEFT: arith->left = (const float *) data; break;
		case ARITH_RIGHT: arith->right = (const float *) data; break;
		case ARITH_OUT: arith->out = (float *) data; break;
		case ARITH_FUNCTION: arith->function = (const float *) data; break;
	}
}

static void arith_activate(LV2_Handle instance) {}

static void arith_run(
		LV2_Handle instance,
		uint32_t n_samples
) {
	const arith_data_t *arith = (const arith_data_t *) instance;

	arith_func_t func = (arith_func_t)*(arith->function);
	const float *const left = arith->left;
	const float *const right = arith->right;
	float *const out = arith->out;
	uint32_t pos;

	if(func < 0) func = 0;
	if(func >= FUNC_LAST) func = FUNC_LAST - 1;

#define _binop_gen(op) for(pos = 0; pos < n_samples; pos++) out[pos] = left[pos] op right[pos]
#define _binop_func(fn) for(pos = 0; pos < n_samples; pos++) out[pos] = fn(left[pos], right[pos])

	switch(func) {
		case FUNC_ADD:
			_binop_gen(+);
			break;

		case FUNC_SUB:
			_binop_gen(-);
			break;

		case FUNC_MUL:
			_binop_gen(*);
			break;

		case FUNC_DIV:
			_binop_gen(/);
			break;

		case FUNC_MOD:
			_binop_func(fmod);
			break;

		case FUNC_POW:
			_binop_func(pow);
			break;
	}
}

static void arith_deactivate(LV2_Handle instance) {}

static void arith_cleanup(LV2_Handle instance) { free(instance); }

static const void *extension_data(const char *uri) { return NULL; }

static const LV2_Descriptor arith_descriptor = {
	ARITH_URI,
	arith_instantiate,
	arith_connect_port,
	arith_activate,
	arith_run,
	arith_deactivate,
	arith_cleanup,
	extension_data,
};

LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t index) {
	return index == 0 ? &arith_descriptor : NULL;
}
