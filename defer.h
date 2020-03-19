#define DEFER_MAX_DEFERRED_STATEMENTS 32

#define _defer_tokpaste(a, b) a ## b

#define _Defer(block, n) do { \
	_deferrals[_num_deferrals++] = && _defer_tokpaste(_defer_ini, n); \
	if (0) { \
		_defer_tokpaste(_defer_ini, n): \
		block; \
		if (_num_deferrals) { \
			goto *_deferrals[--_num_deferrals]; \
		} else { \
			goto *_defer_return_loc; \
		} \
	} \
} while (0)

#define _Return(n) \
	if (_num_deferrals) { \
		_defer_return_loc = && _defer_fini_ ## n; \
		goto *_deferrals[--_num_deferrals]; \
	} \
\
	_defer_fini_ ## n: \
	return

#define Deferral \
volatile unsigned char _num_deferrals = 0; \
jmp_buf _defer_return_loc = {0}, _deferrals[DEFER_MAX_DEFERRED_STATEMENTS] = {0};

#define Defer(block) do { \
	if (setjmp(_deferrals[_num_deferrals++])) { \
		block; \
		if (_num_deferrals) { \
			longjmp(_deferrals[--_num_deferrals], 1); \
		} else { \
			longjmp(_defer_return_loc, 1); \
		} \
	} \
} while (0)

/* TODO: better to have that break or not?  Need to check asm output */
#define Return do { \
	if (setjmp(_defer_return_loc)) break; \
\
	if (_num_deferrals) { \
		longjmp(_deferrals[--_num_deferrals], 1); \
	} \
} while (0); return
