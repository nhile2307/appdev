#include <stdio.h>
#include "sound.h"
#include <math.h>
#include "screen.h"

void showID(char *idname, char *id){
    int i;
    printf("%s : ", idname);
    for(i=0; i<4; i++) printf("%c", id[i]);
    puts("");
}

// this function gets 1 second of samples (16000) and calculates
// 80 pieces of decibel value, we know we need to calculate one
// decibel value, which is calculated by RMS formula, from 200 samples.

void displayWAVDATA(short s[]){
    double rms[80];
    int dB[80];             // used to send decibel values to bar chart
    short *ptr = s;         // we use the pointer poiting to the beginning of array
    int i, j;               // for nested loop counters
                            // outer loop repeats 80 times
                            // inner loop repeats 200 times
    for(i=0; i<80; i++){
        double sum = 0;     // accumulate sum of squares
        for(j=0; j<200; j++){
            sum += (*ptr) * (*ptr);
            ptr ++;         // pointing to the next sample
        }
        rms[i] = sqrt(sum/200);
#ifdef DEBUG
        printf("rms[%d] = %f\n", i, rms[i]);
#endif
        dB[i] = 20*log10(rms[i]);
    }
#ifndef DEBUG
    barChart(dB);           // function shows the bar chart

    int peaks = findPeaks(dB);
    setColors(WHITE, bg(BLACK));
    printf("\033[1;61H");
    printf("Peaks: %d       \n", peaks);
#endif
}

int findPeaks(int d[]){
    int c = 0, i;
    for(i=1; i<80; i++){
        if(d[i]>=75 && d[i-1]<75) c++;
}
    if(d[0]>=75) c++;
    return c;
}


void displayWAVHDR(struct WAVHDR h){
#ifdef DEBUG
    showID("ChunkID", h.ChunkID);
    printf("Chunk size: %d\n", h.ChunkSize);
    showID("Format", h.Format);
    showID("Subchunk1ID", h.Subchunk1ID);
    printf("Subchunk1 size: %d\n", h.Subchunk1Size);
    printf("Audio format: %d\n", h.Format);
    printf("Num. of channels: %d\n", h.NumChannels);
    printf("Sample Rate: %d\n", h.SampleRate);
    printf("Byte Rate: %d\n", h.ByteRate);
    printf("Block align: %d\n", h.BlockAlign);
    printf("Bits per sample: %d\n", h.BitsPerSample);
    showID("Subchunk2ID", h.Subchunk2ID);
    printf("Subchunk2 size: %d\n", h.Subchunk2Size);

#else
    setColors(WHITE, bg(RED));
    printf("\033[1;1H");
printf("test.wav ");

    setColors(YELLOW, bg(BLUE));
    printf("\033[1;10H");
    printf("Sample rate:%d ", h.SampleRate);

    setColors(CYAN, bg(MAGENTA));
    printf("\033[1;27H");
    printf("Duration:%.2fsec", (float)h.Subchunk2Size/h.ByteRate);

    setColors(RED, bg(YELLOW));

#endif
}
