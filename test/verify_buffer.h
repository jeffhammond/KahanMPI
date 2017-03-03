#ifndef VERIFY_BUFFER_H
#define VERIFY_BUFFER_H

#include <stdio.h>
#include <math.h>

/* This is the generic buffer verification for when char is the
 * type and we use memset for assignment. */
static size_t verify_buffer(char *buf, size_t count, int expected_value)
{
    size_t errors = 0;
    for (size_t i = 0; i < (size_t)count; i++) {
        errors += (buf[i] != (unsigned char)expected_value);
    }
    return errors;
}

static size_t verify_doubles(double *buf, size_t count, double expected_value)
{
    size_t errors = 0;
    for (size_t i = 0; i < (size_t)count; i++) {
        const double absdiff = fabs(buf[i] - expected_value);
        if (absdiff>1.e-4) errors++;
    }
    return errors;
}

static void set_doubles(double *buf, size_t count, double value)
{
    for (size_t i = 0; i < (size_t)count; i++) {
        buf[i] = value;
    }
}

static size_t verify_floats(float *buf, size_t count, float expected_value)
{
    size_t errors = 0;
    for (size_t i = 0; i < count; i++) {
        const float absdiff = fabs(buf[i] - expected_value);
        if (absdiff>1.e-4) errors++;
    }
    return errors;
}

static void set_floats(float *buf, size_t count, float value)
{
    for (size_t i = 0; i < count; ++i) {
        buf[i] = value;
    }
}

#endif // VERIFY_BUFFER_H
