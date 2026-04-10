#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>

/**
 * error_exit - prints error and exits
 */
void error_exit(void)
{
	dprintf(STDERR_FILENO, "Error: Not an ELF file\n");
	exit(98);
}

/**
 * print_magic - prints magic bytes
 */
void print_magic(unsigned char *e_ident)
{
	int i;

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
		printf("%02x%s", e_ident[i], i == EI_NIDENT - 1 ? "\n" : " ");
}

/**
 * print_class - prints class
 */
void print_class(unsigned char *e_ident)
{
	printf("  Class:                             ");

	if (e_ident[EI_CLASS] == ELFCLASS64)
		printf("ELF64\n");
	else if (e_ident[EI_CLASS] == ELFCLASS32)
		printf("ELF32\n");
	else
		printf("<unknown>\n");
}

/**
 * print_data - prints data encoding
 */
void print_data(unsigned char *e_ident)
{
	printf("  Data:                              ");

	if (e_ident[EI_DATA] == ELFDATA2LSB)
		printf("2's complement, little endian\n");
	else if (e_ident[EI_DATA] == ELFDATA2MSB)
		printf("2's complement, big endian\n");
	else
		printf("<unknown>\n");
}

/**
 * print_version - prints version
 */
void print_version(unsigned char *e_ident)
{
	printf("  Version:                           %d", e_ident[EI_VERSION]);
	if (e_ident[EI_VERSION] == EV_CURRENT)
		printf(" (current)");
	printf("\n");
}

/**
 * print_osabi - prints OS/ABI
 */
void print_osabi(unsigned char *e_ident)
{
	printf("  OS/ABI:                            ");

	switch (e_ident[EI_OSABI])
	{
	case ELFOSABI_SYSV:
		printf("UNIX - System V\n");
		break;
	case ELFOSABI_NETBSD:
		printf("UNIX - NetBSD\n");
		break;
	case ELFOSABI_SOLARIS:
		printf("UNIX - Solaris\n");
		break;
	default:
		printf("<unknown: %x>\n", e_ident[EI_OSABI]);
	}
}

/**
 * print_type - prints file type
 */
void print_type(unsigned int e_type)
{
	printf("  Type:                              ");

	switch (e_type)
	{
	case ET_EXEC:
		printf("EXEC (Executable file)\n");
		break;
	case ET_DYN:
		printf("DYN (Shared object file)\n");
		break;
	default:
		printf("<unknown>\n");
	}
}

/**
 * main - entry point
 */
int main(int argc, char **argv)
{
	int fd;
	Elf64_Ehdr header;

	if (argc != 2)
		error_exit();

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		error_exit();

	if (read(fd, &header, sizeof(header)) != sizeof(header))
		error_exit();

	if (header.e_ident[0] != 0x7f ||
	    header.e_ident[1] != 'E' ||
	    header.e_ident[2] != 'L' ||
	    header.e_ident[3] != 'F')
		error_exit();

	printf("ELF Header:\n");

	print_magic(header.e_ident);
	print_class(header.e_ident);
	print_data(header.e_ident);
	print_version(header.e_ident);
	print_osabi(header.e_ident);

	printf("  ABI Version:                       %d\n",
	       header.e_ident[EI_ABIVERSION]);

	print_type(header.e_type);

	printf("  Entry point address:               0x%lx\n",
	       header.e_entry);

	close(fd);
	return (0);
}
