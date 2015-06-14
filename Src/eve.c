#include "eve.h"
#include "ft_gpu.h"
#include "ft800.h"



void play_sound (uint8_t sound, uint8_t note, uint8_t volume) {

	ft800memWrite8(REG_VOL_SOUND, volume);
	ft800memWrite16(REG_SOUND, (sound | (note << 8)));
	ft800memWrite8(REG_PLAY, 1);
}
