#include <stdio.h>
#include <stdarg.h>
#include "variadic_functions.h"

void print_all(const char * const format, ...)
{
	va_list ap;
	int i = 0;
	char *sep = "";
	char *s;

	va_start(ap, format);

	while (format && format[i])
	{
		if (format[i] == 'c')
			printf("%s%c", sep, va_arg(ap, int)), sep = ", ";

		if (format[i] == 'i')
			printf("%s%d", sep, va_arg(ap, int)), sep = ", ";

		if (format[i] == 'f')
			printf("%s%f", sep, va_arg(ap, double)), sep = ", ";

		if (format[i] == 's')
		{
			s = va_arg(ap, char *);
			if (!s)
				s = "(nil)";
			printf("%s%s", sep, s), sep = ", ";
		}

		i++;
	}

	printf("\n");
	va_end(ap);
}
