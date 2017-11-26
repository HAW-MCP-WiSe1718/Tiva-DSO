#include "graphics.h"

#ifndef UI_WAVEFORM_H_
#define UI_WAVEFORM_H_

#define UI_WAVEFORM_GRIDXSTART 0
#define UI_WAVEFORM_GRIDXEND   (480-1)
#define UI_WAVEFORM_GRIDYSTART 16
#define UI_WAVEFORM_GRIDYEND   (272-1)
#define UI_WAVEFORM_GRIDXSPACE 40
#define UI_WAVEFORM_GRIDYSPACE 32

#define UI_WAVEFORM_WAVEXSTART (1-1)
#define UI_WAVEFORM_WAVEXEND (480-1)

#define UI_WAVEFORM_PRINTBUFFER_SIZE 480

#define UI_WAVEFORM_WAVE_DATA_END_REACHED 480

#define UI_WAVEFORM_WAVE_COLOR GRAPHICS_REDCOLOR

int16_t aiPrintBuffer[UI_WAVEFORM_PRINTBUFFER_SIZE];

void vUIWaveformInit(void);

void vUIWaveformGridInit(void);

void vUIWaveformPrintWave(tuGraphicsColor);
void vUIWaveformPrintWaveSingle(uint16_t,tuGraphicsColor);
void vUIWaveformPrintPartOfWave(uint16_t,uint16_t,tuGraphicsColor);

void vUIWaveformUpdateWave(int16_t*,tuGraphicsColor);
void vUIWaveformUpdateWaveSingle(int16_t*,uint16_t,tuGraphicsColor);
void vUIWaveformUpdatePartOfWave(int16_t*,uint16_t,uint16_t,tuGraphicsColor);

#endif /* UI_WAVEFORM_H_ */
