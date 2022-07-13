
#include "sequencer.h"

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

void Sequencer2(sequence *seq, void (*step1)(), unsigned long time1,
                void (*step2)(), unsigned long time2) {
    
    seq->t1 = time1;
    seq->t2 = time2;

    seq->s1func = step1;
    seq->s2func = step2;
}

void Sequencer2_set_step1_time(sequence *seq,unsigned long time) {
    seq->t1 = time;
}

void Sequencer2_set_step2_time(sequence *seq, unsigned long time) {
    seq->t2 = time;
}

unsigned long Sequencer2_get_step1_time(sequence *seq) {
    return seq->t1;
}
unsigned long Sequencer2_get_step2_time(sequence *seq) {
    return seq->t2;
}

void Sequencer2_reset(sequence *seq) {
    seq->current_step = 1;
    seq->next_step_time = 0;
}

void Sequencer2_run(sequence *seq) {
    switch (seq->current_step) {
        case 1:
            if (HAL_GetTick() >= seq->next_step_time) {
                seq->s1func();
                seq->next_step_time = HAL_GetTick() + seq->t1;
                seq->current_step = 2;
            }
            break;

        case 2:
            if (HAL_GetTick() >= seq->next_step_time) {
                seq->s2func();
                seq->next_step_time = HAL_GetTick() + seq->t2;
                seq->current_step = 1;
            }
            break;
    }
}