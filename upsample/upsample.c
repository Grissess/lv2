#include "lv2/core/lv2.h"

#include <stdlib.h>
#include <math.h>

#define UPSAMPLE_URI NXS_URI "upsample"

typedef enum {
	UPSAMPLE_IN = 0,
	UPSAMPLE_OUT = 1,
} upsample_port_index_t;

typedef struct {
	const float *in;
	float *out;
} upsample_data_t;

static LV2_Handle upsample_instantiate(
		const LV2_Descriptor *descriptor,
		double rate,
		const char *bundle_path,
		const LV2_Feature * const *features
) {
	return (LV2_Handle) calloc(1, sizeof(upsample_data_t));
}

static void upsample_connect_port(
		LV2_Handle instance,
		uint32_t port,
		void *data
) {
	upsample_data_t *upsample = (upsample_data_t *) instance;
	switch((upsample_port_index_t) port) {
		case UPSAMPLE_IN: upsample->in = (const float *) data; break;
		case UPSAMPLE_OUT: upsample->out = (float *) data; break;
	}
}

static void upsample_activate(LV2_Handle instance) {}

static void upsample_run(
		LV2_Handle instance,
		uint32_t n_samples
) {
	const upsample_data_t *upsample = (const upsample_data_t *) instance;
	const float *const in = upsample->in;
	const float sample = *in;
	float *const out = upsample->out;
	uint32_t pos;

	for(pos = 0; pos < n_samples; pos++) {
		out[pos] = sample;
	}
}

static void upsample_deactivate(LV2_Handle instance) {}

static void upsample_cleanup(LV2_Handle instance) { free(instance); }

static const void *extension_data(const char *uri) { return NULL; }

static const LV2_Descriptor upsample_descriptor = {
	UPSAMPLE_URI,
	upsample_instantiate,
	upsample_connect_port,
	upsample_activate,
	upsample_run,
	upsample_deactivate,
	upsample_cleanup,
	extension_data,
};

LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t index) {
	return index == 0 ? &upsample_descriptor : NULL;
}
