/* env.c — simple environment / symbol table */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "env.h"

Env* env_create(void) {
    Env *e = (Env*)malloc(sizeof(Env));
    e->head = NULL;
    return e;
}

void env_free(Env *e) {
    if (!e) return;
    Binding *b = e->head;
    while (b) {
        Binding *n = b->next;
        free(b->name);
        free_value(b->value);
        free(b);
        b = n;
    }
    free(e);
}

Value* value_number(double n) {
    Value *v = (Value*)malloc(sizeof(Value));
    v->type = VAL_NUMBER;
    v->number = n;
    v->string = NULL;
    return v;
}

Value* value_string(const char *s) {
    Value *v = (Value*)malloc(sizeof(Value));
    v->type = VAL_STRING;
    v->number = 0;
    v->string = s ? strdup(s) : NULL;
    return v;
}

void free_value(Value *v) {
    if (!v) return;
    if (v->type == VAL_STRING && v->string) free(v->string);
    free(v);
}

void env_set(Env *e, const char *name, const Value *v) {
    if (!e || !name) return;
    Binding *b = e->head;
    while (b) { if (strcmp(b->name, name) == 0) break; b = b->next; }
    if (!b) {
        b = (Binding*)malloc(sizeof(Binding));
        b->name = strdup(name);
        b->value = NULL;
        b->next = e->head;
        e->head = b;
    }
    if (b->value) free_value(b->value);
    if (!v) b->value = NULL;
    else if (v->type == VAL_NUMBER) b->value = value_number(v->number);
    else b->value = value_string(v->string);
}

Value* env_get(Env *e, const char *name) {
    if (!e || !name) return NULL;
    Binding *b = e->head;
    while (b) { if (strcmp(b->name, name) == 0) return b->value; b = b->next; }
    return NULL;
}