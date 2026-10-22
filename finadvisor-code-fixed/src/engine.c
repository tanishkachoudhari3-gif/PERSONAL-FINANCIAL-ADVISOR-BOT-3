#include "engine.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

static double clamp(double v, double lo, double hi) { return v < lo ? lo : (v > hi ? hi : v); }
static double future_value(double monthly, double annual, int years) {
    double r = annual / 12.0, n = years * 12.0;
    if (r <= 0.0) return monthly * n;
    return monthly * ((pow(1.0 + r, n) - 1.0) / r) * (1.0 + r);
}

void calculate_advice(const FinanceInput *in, Advice *o) {
    double income = clamp(in->income, 0, 1000000000.0);
    double expenses = clamp(in->expenses, 0, 1000000000.0);
    double emi = clamp(in->emi, 0, 1000000000.0);
    double emergency = clamp(in->emergency, 0, 1000000000.0);
    double age = clamp(in->age, 18, 100);
    int risk = in->risk < 1 || in->risk > 3 ? 2 : in->risk;
    double outflow = expenses + emi;
    o->surplus = income - outflow;
    o->savings_rate = income > 0 ? (o->surplus / income) * 100.0 : 0;
    o->dti = income > 0 ? (emi / income) * 100.0 : (emi > 0 ? 100 : 0);
    o->emergency_months = outflow > 0 ? emergency / outflow : 0;

    double savings_pts = clamp(o->savings_rate / 25.0, 0, 1) * 35;
    double dti_pts = (1.0 - clamp(o->dti / 50.0, 0, 1)) * 35;
    double ef_pts = clamp(o->emergency_months / 6.0, 0, 1) * 30;
    o->score = clamp(savings_pts + dti_pts + ef_pts, 0, 100);

    int liquid_priority = o->emergency_months < 6.0;
    double eq_base = risk == 1 ? 25 : (risk == 3 ? 65 : 50);
    if (age > 45) eq_base -= (age - 45) * 0.5;
    if (liquid_priority) eq_base -= 15;
    if (o->surplus <= 0) eq_base = 0;
    o->equity = clamp(eq_base, 0, 70);
    o->liquid = liquid_priority ? 35 : 20;
    o->debt = o->dti > 35 ? 30 : (100 - o->equity - o->liquid) * 0.6;
    o->debt = clamp(o->debt, 0, 100 - o->equity - o->liquid);
    o->gold = 100 - o->equity - o->liquid - o->debt;

    double annual = risk == 1 ? 0.08 : (risk == 3 ? 0.15 : 0.12);
    double investable = o->surplus > 0 ? o->surplus : 0;
    o->corpus10 = future_value(investable, annual, 10);
    o->corpus20 = future_value(investable, annual, 20);
}

void advice_json(const FinanceInput *in, const Advice *a, char *b, size_t cap) {
    const char *risk = in->risk == 1 ? "Conservative" : (in->risk == 3 ? "Aggressive" : "Moderate");
    const char *goal = in->goal == 1 ? "Emergency fund" : (in->goal == 2 ? "Debt payoff" : (in->goal == 3 ? "Home / major asset" : "Long-term wealth"));
    snprintf(b, cap,
        "{\"income\":%.0f,\"expenses\":%.0f,\"emi\":%.0f,\"emergency\":%.0f,\"age\":%.0f,\"risk\":\"%s\",\"goal\":\"%s\",\"surplus\":%.0f,\"savingsRate\":%.1f,\"dti\":%.1f,\"emergencyMonths\":%.1f,\"score\":%.0f,\"allocation\":{\"equity\":%.0f,\"debt\":%.0f,\"liquid\":%.0f,\"gold\":%.0f},\"corpus10\":%.0f,\"corpus20\":%.0f}",
        in->income,in->expenses,in->emi,in->emergency,in->age,risk,goal,a->surplus,a->savings_rate,a->dti,a->emergency_months,a->score,a->equity,a->debt,a->liquid,a->gold,a->corpus10,a->corpus20);
}
