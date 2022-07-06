
#include "sequencer1.h"

void Sequencer1(sequence *seq, void (*step1)(), unsigned long time1) {
    seq->t1 = time1;
    seq->s1func = step1;
}

void Sequencer1_set_step1_time(sequence *seq, unsigned long time) {
    seq->t1 = time;
}

void Sequencer1_reset(sequence *seq) {
    seq->next_step_time = 0;
}

unsigned long Sequencer1_get_step1_time(sequence *seq) {
    return seq->t1;
}

void Sequencer1_run(sequence *seq) {
    if (HAL_GetTick() >= seq->next_step_time) {
        seq->s1func();
        seq->next_step_time = HAL_GetTick() + seq->t1;
    }
}