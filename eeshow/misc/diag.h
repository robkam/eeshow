/*
 * misc/diag.h - Diagnostics
 *
 * Written 2016 by Werner Almesberger
 * Copyright 2016 by Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef MISC_DIAG_H
#define	MISC_DIAG_H

#include <stdbool.h>


/*
 * 0: no progress indications
 * 1: reasonable progress indications
 * 2: verbose output
 * > 2: go wild !
 */

extern unsigned verbose;


/* ----- Internal errors --------------------------------------------------- */


#define	STRINGIFY(s) #s
#define	STRINGIFY_EXPAND(s) STRINGIFY(s)

#define	BUG(...) \
	fatal("BUG " __FILE__ ":" STRINGIFY_EXPAND(__LINE__) ": " __VA_ARGS__)


/* ----- Deferred errors --------------------------------------------------- */


void diag_defer_begin(void);
void diag_defer_end(bool report);


/* ----- Specialized diagnostic functions ---------------------------------- */


/* perror, based on "fatal" or "error" */

void __attribute__((noreturn)) diag_pfatal(const char *s);
void diag_perror(const char *s);

void __attribute__((noreturn)) pfatal_git(const char *s);
void perror_git(const char *s);


/* ----- General diagnostic functions -------------------------------------- */


/*
 * Terminate immediately. Further execution makes no sense.
 * E.g., out of memory.
 */

void __attribute__((noreturn)) fatal(const char *fmt, ...)
    __attribute__((format(printf, 1, 2)));

/*
 * Operation has failed, but the program as a whole may still be able to
 * continue. E.g., a schematics component was not found.
 */

void error(const char *fmt, ...)
    __attribute__((format(printf, 1, 2)));

/*
 * A minor operation has failed or some other issue was detected. This may
 * be (or lead to) a more serious problem, but does not immediately affect
 * operation.
 */

void warning(const char *fmt, ...)
    __attribute__((format(printf, 1, 2)));

/*
 * Progress message, used mainly for debugging. "level" is the minimum
 * verbosity level required.
 */

void progress(unsigned level, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));

#endif /* !MISC_DIAG_H */
