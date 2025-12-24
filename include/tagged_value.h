#pragma once

typedef enum
{
    TV_NUMBER,
    TV_STRING,
    TV_BOOLEAN,
    TV_NULL,
} tagged_value_e;

typedef struct
{
    tagged_value_e type;
    union
    {
        double number;
        char *str;
        void *ptr;
        unsigned char boolean;
    } as;

} tagged_value_t;

#define IS_NUMBER(obj) (obj.type == TV_NUMBER)
#define IS_STRING(obj) (obj.type == TV_STRING)
#define IS_BOOLEAN(obj) (obj.type == TV_BOOLEAN)
#define IS_NULL(obj) (obj.type == TV_NULL)

#define AS_NUMBER(obj) (obj.as.number)
#define AS_STRING(obj) (obj.as.str)
#define AS_BOOLEAN(obj) (obj.as.boolean)
#define AS_NULL(obj) (obj.as.null)

#define MAKE_NUMBER(number) ((tagged_value_t){.type = TV_NUMBER, .as.number = number})
#define MAKE_STRING(str) ((tagged_value_t){.type = TV_STRING, .as.str = str})
#define MAKE_BOOLEAN(boolean) ((tagged_value_t){.type = TV_BOOLEAN, .as.boolean = boolean})
#define MAKE_NULL() ((tagged_value_t){.type = TV_NULL, .as.ptr = ((void *)0)})
