#ifndef PH_ENV_H
#define PH_ENV_H

typedef enum { VAL_NUMBER, VAL_STRING } ValueType;

typedef struct Value {
    ValueType type;
    double number;
    char *string;
} Value;

typedef struct Binding {
    char *name;
    Value *value;
    struct Binding *next;
} Binding;

typedef struct Env { Binding *head; } Env;

Env* env_create(void);
void env_free(Env *e);

Value* value_number(double n);
Value* value_string(const char *s);
void free_value(Value *v);

void env_set(Env *e, const char *name, const Value *v);
Value* env_get(Env *e, const char *name);

#endif /* PH_ENV_H */