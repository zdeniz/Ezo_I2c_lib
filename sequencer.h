
#ifndef SEQUENCER1_H
#define SEQUENCER1_H

#include "main.h"

typedef struct
{
    unsigned long t1;
    unsigned long t2;
    unsigned long t3;
    unsigned long t4;
    void (*s1func)();
    void (*s2func)();
    void (*s3func)();
    void (*s4func)();
    uint32_t next_step_time;
    uint8_t current_step;
} sequence;

void Sequencer_reset(sequence *seq);
void Sequencer_set_step1_time(sequence *seq, unsigned long time);
void Sequencer_set_step2_time(sequence *seq, unsigned long time);
void Sequencer_set_step3_time(sequence *seq, unsigned long time);
// void Sequencer_set_step4_time(sequence *seq, unsigned long time);
unsigned long Sequencer_get_step1_time(sequence *seq);
unsigned long Sequencer_get_step2_time(sequence *seq);
unsigned long Sequencer_get_step3_time(sequence *seq);
// unsigned long Sequencer_get_step4_time(sequence *seq);

void Sequencer1(sequence *seq, void (*step1)(), unsigned long time1);
void Sequencer1_run(sequence *seq);

void Sequencer2(sequence *seq, void (*step1)(), unsigned long time1,
                void (*step2)(), unsigned long time2);
void Sequencer2_run(sequence *seq);

void Sequencer3(sequence *seq, void (*step1)(), unsigned long time1,
                void (*step2)(), unsigned long time2,
                void (*step3)(), unsigned long time3);
void Sequencer3_run(sequence *seq);

#endif