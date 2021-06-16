#include "lv2/core/lv2.h"

#include <stdlib.h>

#define CLIP_URI NXS_URI "clip"

typedef enum { CLIP_IN = 0, CLIP_OUT = 1, CLIP_LIMIT = 2 } clip_port_index_t;

typedef struct {
	const float *in;
	float *out;
	const float *limit;
} clip_data_t;

static LV2_Handle clip_instantiate(
		const LV2_Descriptor *descriptor,
		double rate,
		const char *bundle_path,
		const LV2_Feature * const *features
) {
	return (LV2_Handle) calloc(1, sizeof(clip_data_t));
}

static void clip_connect_port(
		LV2_Handle instance,
		uint32_t port,
		void *data
) {
	clip_data_t *clip = (clip_data_t *) instance;
	switch((clip_port_index_t) port) {
		case CLIP_IN: clip->in = (const float *) data; break;
		case CLIP_OUT: clip->out = (float *) data; break;
		case CLIP_LIMIT: clip->limit = (const float *) data; break;
	}
}

static void clip_activate(LV2_Handle instance) {}

static void clip_run(
		LV2_Handle instance,
		uint32_t n_samples
) {
	const clip_data_t *clip = (const clip_data_t *) instance;
	const float limit = *(clip->limit);

	const float *const in = clip->in;
	float *const out = clip->out;
	float sample;
	uint32_t pos;

	for(pos = 0; pos < n_samples; pos++) {
		sample = in[pos];
		out[pos] = (
				sample > limit ? limit :
				sample < (-limit) ? -limit :
				sample
		);
	}
}

static void clip_deactivate(LV2_Handle instance) {}
static void clip_cleanup(LV2_Handle instance) {
	free(instance);
}

static const void *extension_data(const char *uri) { return NULL; }

static const LV2_Descriptor clip_descriptor = {
	CLIP_URI,
	clip_instantiate,
	clip_connect_port,
	clip_activate,
	clip_run,
	clip_deactivate,
	clip_cleanup,
	extension_data,
};

LV2_SYMBOL_EXPORT const LV2_Descriptor *lv2_descriptor(uint32_t index) {
	return index == 0 ? &clip_descriptor : NULL;
}
