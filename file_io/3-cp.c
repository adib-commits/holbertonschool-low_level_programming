#include "main.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * print_error - prints error message and exits
 * @code: exit code
 * @msg: message
 * @file: file name or NULL
 * @fd: file descriptor (for close error)
 */
void print_error(int code, char *msg, char *file, int fd)
{
	int i = 0;

	while (msg[i])
		write(2, &msg[i++], 1);

	if (file)
	{
		i = 0;
		while (file[i])
			write(2, &file[i++], 1);
	}

	if (fd != -1)
	{
		char c;

		c = fd + '0';
		write(2, &c, 1);
	}

	write(2, "\n", 1);
	exit(code);
}

/**
 * main - copies file content
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	int fd_from, fd_to, r, w;
	char buffer[1024];

	if (argc != 3)
		print_error(97, "Usage: cp file_from file_to", NULL, -1);

	fd_from = open(argv[1], O_RDONLY);
	if (fd_from == -1)
		print_error(98, "Error: Can't read from file ", argv[1], -1);

	fd_to = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_to == -1)
		print_error(99, "Error: Can't write to ", argv[2], -1);

	while ((r = read(fd_from, buffer, 1024)) > 0)
	{
		w = write(fd_to, buffer, r);
		if (w == -1 || w != r)
			print_error(99, "Error: Can't write to ", argv[2], -1);
	}

	if (r == -1)
		print_error(98, "Error: Can't read from file ", argv[1], -1);

	if (close(fd_from) == -1)
		print_error(100, "Error: Can't close fd ", NULL, fd_from);

	if (close(fd_to) == -1)
		print_error(100, "Error: Can't close fd ", NULL, fd_to);

	return (0);
}
