#include "lv2/core/lv2.h"

#include <stdlib.h>

#define CLICKS_URI NXS_URI "clicks_pops"

typedef enum { CLICKS_OUT = 0, CLICKS_THRES = 1 } clicks_port_index_t;

typedef struct {
	float *out;
	const float *thres;
} clicks_data_t;

static LV2_Handle clicks_instantiate(
		const LV2_Descriptor *descriptor,
		double rate,
		const char *bundle_path,
		const LV2_Feature * const *features
) {
	return (LV2_Handle) calloc(1, sizeof(clicks_data_t));
}

static void clicks_connect_port(
		LV2_Handle instance,
		uint32_t port,
		void *data
) {
	clicks_data_t *clicks = (clicks_data_t *) instance;
	switch((clicks_port_index_t) port) {
		case CLICKS_OUT: clicks->out = (float *) data; break;
		case CLICKS_THRES: clicks->thres = (const float *) data; break;
	}
}

static void clicks_activate(LV2_Handle instance) {}

static void clicks_run(
		LV2_Handle instance,
		uint32_t n_samples
) {
	const clicks_data_t *clicks = (const clicks_data_t *) instance;
	const float thres = *(clicks->thres);

	float *const out = clicks->out;
	uint32_t pos;

	for(pos = 0; pos < n_samples; pos++) {
		out[pos] = (
				(float)rand() / (float)RAND_MAX < thres ?
				(float)rand() / (float)RAND_MAX : 0.0
		);
	}
}

static void clicks_deactivate(LV2_Handle instance) {}
static void clicks_cleanup(LV2_Handle instance) {
	free(instance);
}

static const void *extension_data(const char *uri) { return NULL; }

static const LV2_Descriptor clicks_descriptor = {
	CLICKS_URI,
	clicks_instantiate,
	clicks_connect_port,
	clicks_activate,
	clicks_run,
	clicks_deactivate,
	clicks_cleanup,
	extension_data,
};

LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t index) {
	return index == 0 ? &clicks_descriptor : NULL;
}
