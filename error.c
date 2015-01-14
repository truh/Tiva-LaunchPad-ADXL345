/******************************************************************************
 *
 * error.c
 *
 * Copyright (c) 2014 Jakob Klepp
 *
 *****************************************************************************/

#include <stdint.h>

#ifdef DEBUG
void __error__(char *pcFilename, uint32_t ui32Line) {}
#endif
