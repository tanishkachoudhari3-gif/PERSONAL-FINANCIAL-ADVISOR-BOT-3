#ifndef FINADVISOR_ENGINE_H
#define FINADVISOR_ENGINE_H

#include <stddef.h>

typedef struct {
    double income, expenses, emi, emergency, age;
    int risk;
    int goal;
} FinanceInput;

typedef struct {
    double surplus, savings_rate, dti, emergency_months;
    double score, equity, debt, liquid, gold;
    double corpus10, corpus20;
} Advice;

void calculate_advice(const FinanceInput *input, Advice *out);
void advice_json(const FinanceInput *input, const Advice *a, char *buffer, size_t capacity);

#endif
