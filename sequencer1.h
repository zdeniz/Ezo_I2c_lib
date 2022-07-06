
#ifndef SEQUENCER1_H
#define SEQUENCER1_H

#include "main.h"

typedef struct
{
    unsigned long t1;
    void (*s1func)();
    uint32_t next_step_time;
} sequence;


void Sequencer1(sequence *seq, void (*step1)(), unsigned long time1);
void Sequencer1_reset(sequence *seq);
void Sequencer1_run(sequence *seq);
void Sequencer1_set_step1_time(sequence *seq, unsigned long time);
unsigned long Sequencer1_get_step1_time(sequence *seq);

#endif