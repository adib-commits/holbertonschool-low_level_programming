#include "variadic_functions.h"
#include <stdarg.h>
#include <stdio.h>

void print_all(const char * const format, ...)
{
va_list ap;
int i = 0, printed = 0;
char *str;

va_start(ap, format);

while (format && format[i])
{
if (format[i] == 'c' || format[i] == 'i' ||
format[i] == 'f' || format[i] == 's')
{
if (printed)
printf(", ");

switch (format[i])
{
case 'c':
printf("%c", va_arg(ap, int));
break;
case 'i':
printf("%d", va_arg(ap, int));
break;
case 'f':
printf("%f", va_arg(ap, double));
break;
case 's':
str = va_arg(ap, char *);
if (!str)
printf("(nil)");
else
printf("%s", str);
break;
}
printed = 1;
}
i++;
}

va_end(ap);
printf("\n");
}
