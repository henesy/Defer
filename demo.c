#include <u.h>
#include <libc.h>

#include "defer.h"

void test1(void) {Deferral
	char *x = malloc(0x100);
	Defer(free(x));

	/* legal because deconstruction happens in reverse order */
	Defer({
		print("x is %s\n", x);
		x[1]++;
		print("now %s\n", x);
	});

	strcpy(x, "Hello world");

	Return;
}

int test2(void) {Deferral
	print("1\n");

	/* caveat: unlike in other programming languages, the return expression
	 * is evaluated after deferred statements.  Unfortunately, there is no
	 * good way around this.
	 */
	Defer(print("2\n"));
	Return print("3\n");
}

void
main(int, char*[]) {
	test1();
	test2();
	return;
}
