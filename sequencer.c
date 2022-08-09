
#include "sequencer.h"

void Sequencer_reset(sequence *seq) {
    seq->current_step = 1;
    seq->next_step_time = 0;
}

void Sequencer_set_step1_time(sequence *seq, unsigned long time) {
    seq->t1 = time;
}

void Sequencer_set_step2_time(sequence *seq, unsigned long time) {
    seq->t2 = time;
}

void Sequencer_set_step3_time(sequence *seq, unsigned long time) {
    seq->t3 = time;
}

unsigned long Sequencer_get_step1_time(sequence *seq) {
    return seq->t1;
}

unsigned long Sequencer_get_step2_time(sequence *seq) {
    return seq->t2;
}

unsigned long Sequencer_get_step3_time(sequence *seq) {
    return seq->t3;
}

void Sequencer1(sequence *seq, void (*step1)(), unsigned long time1) {
    seq->t1 = time1;
    seq->s1func = step1;
}

void Sequencer1_reset(sequence *seq) {
    seq->next_step_time = 0;
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

void Sequencer3(sequence *seq, void (*step1)(), unsigned long time1,
                void (*step2)(), unsigned long time2,
                void (*step3)(), unsigned long time3) {
    seq->t1 = time1;
    seq->t2 = time2;
    seq->t3 = time3;

    seq->s1func = step1;
    seq->s2func = step2;
    seq->s3func = step3;
}

void Sequencer3_run(sequence *seq) {
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
                seq->current_step = 3;
            }
            break;

        case 3:
            if (HAL_GetTick() >= seq->next_step_time) {
                seq->s3func();
                seq->next_step_time = HAL_GetTick() + seq->t3;
                seq->current_step = 1;
            }
            break;
    }
}