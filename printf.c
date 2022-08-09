#include "main.h"

/**
 * clean - Peforms clean operations
 * @params: va_list
 * @output: A buffer_t struct.
 */
void clean(va_list params, buffer_t *output)
{
	va_end(params);
	write(1, output->start, output->len);
	free_buffer(output);
}

/**
 * print - Reads through the format string
 * @format: Character string to print
 * @output: A buffer_t struct containing a buffer.
 * @params: A va_list of arguments.
 *
 * Return: The number of characters
 */
int print(const char *format, va_list params, buffer_t *output)
{
	int i, wid, prec, ret = 0;
	char tmp;
	unsigned char flags, len;
	unsigned int (*f)(va_list, buffer_t *, unsigned char,
			int, int, unsigned char);

	for (i = 0; *(format + i); i++)
	{
		len = 0;
		if (*(format + i) == '%')
		{
			tmp = 0;
			flags = handle_flags(format + i + 1, &tmp);
			wid = handle_width(params, format + i + tmp + 1, &tmp);
			prec = handle_precision(params, format + i + tmp + 1,
					&tmp);
			len = handle_length(format + i + tmp + 1, &tmp);

			f = handle_specifiers(format + i + tmp + 1);
			if (f)
			{
				i += tmp + 1;
				ret += f(params, output, flags, wid, prec, len);
				continue;
			}
			else if (!*(format + i + tmp + 1))
			{
				ret = -1;
				break;
			}
		}
		ret += _memcpy(output, (format + i), 1);
		i += (len != 0) ? 1 : 0;
	}
	clean(params, output);
	return (ret);
}

/**
 * _printf - Outputs a formatted string.
 * @format: Character string to print
 *
 * Return: The number of characters
 */
int _printf(const char *format, ...)
{
	buffer_t *output;
	va_list params;
	int ret;

	if (!format)
		return (-1);
	output = init_buffer();
	if (!output)
		return (-1);

	va_start(params, format);

	ret = print(format, params, output);

	return (ret);
}
