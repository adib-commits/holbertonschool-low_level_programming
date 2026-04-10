#include "main.h"
#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * print_magic - prints the Magic line of the ELF header
 * @buf: pointer to the raw ELF header bytes
 */
void print_magic(unsigned char *buf)
{
	int i;

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x", buf[i]);
		if (i < EI_NIDENT - 1)
			printf(" ");
	}
	printf("\n");
}

/**
 * print_osabi - prints the OS/ABI field
 * @osabi: the OS/ABI byte
 */
void print_osabi(unsigned char osabi)
{
	printf("  OS/ABI:                            ");
	switch (osabi)
	{
	case ELFOSABI_SYSV:       printf("UNIX - System V\n"); break;
	case ELFOSABI_HPUX:       printf("UNIX - HP-UX\n"); break;
	case ELFOSABI_NETBSD:     printf("UNIX - NetBSD\n"); break;
	case ELFOSABI_LINUX:      printf("UNIX - Linux\n"); break;
	case ELFOSABI_SOLARIS:    printf("UNIX - Solaris\n"); break;
	case ELFOSABI_AIX:        printf("UNIX - AIX\n"); break;
	case ELFOSABI_IRIX:       printf("UNIX - IRIX\n"); break;
	case ELFOSABI_FREEBSD:    printf("UNIX - FreeBSD\n"); break;
	case ELFOSABI_TRU64:      printf("UNIX - TRU64\n"); break;
	case ELFOSABI_MODESTO:    printf("Novell - Modesto\n"); break;
	case ELFOSABI_OPENBSD:    printf("UNIX - OpenBSD\n"); break;
	case ELFOSABI_ARM_AEABI:  printf("ARM EABI\n"); break;
	case ELFOSABI_ARM:        printf("ARM\n"); break;
	case ELFOSABI_STANDALONE: printf("Standalone App\n"); break;
	default: printf("<unknown: %x>\n", osabi); break;
	}
}

/**
 * print_type - prints the ELF type field
 * @type: the e_type value (may need byte-swap)
 * @data: EI_DATA byte to determine endianness
 */
void print_type(unsigned int type, unsigned char data)
{
	if (data == ELFDATA2MSB)
		type = (type >> 8) | (type << 8);
	printf("  Type:                              ");
	switch (type)
	{
	case ET_NONE: printf("NONE (No file type)\n"); break;
	case ET_REL:  printf("REL (Relocatable file)\n"); break;
	case ET_EXEC: printf("EXEC (Executable file)\n"); break;
	case ET_DYN:  printf("DYN (Shared object file)\n"); break;
	case ET_CORE: printf("CORE (Core file)\n"); break;
	default:      printf("<unknown: %x>\n", type); break;
	}
}

/**
 * print_entry - prints the entry point address
 * @buf: raw ELF header bytes
 * @class: EI_CLASS byte (32 or 64 bit)
 * @data: EI_DATA byte (endianness)
 */
void print_entry(unsigned char *buf, unsigned char class, unsigned char data)
{
	unsigned long long entry = 0;
	int i;

	printf("  Entry point address:               ");
	if (class == ELFCLASS64)
	{
		unsigned char *e = buf + offsetof(Elf64_Ehdr, e_entry);

		if (data == ELFDATA2MSB)
			for (i = 0; i < 8; i++)
				entry = (entry << 8) | e[i];
		else
			for (i = 7; i >= 0; i--)
				entry = (entry << 8) | e[i];
		printf("0x%llx\n", entry);
	}
	else
	{
		unsigned char *e = buf + offsetof(Elf32_Ehdr, e_entry);

		if (data == ELFDATA2MSB)
			for (i = 0; i < 4; i++)
				entry = (entry << 8) | e[i];
		else
			for (i = 3; i >= 0; i--)
				entry = (entry << 8) | e[i];
		printf("0x%llx\n", entry);
	}
}

/**
 * main - displays ELF header information
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: 0 on success, exits with 98 on error
 */
int main(int argc, char **argv)
{
	int fd;
	unsigned char buf[64];
	unsigned char class, data, version, osabi, abiversion;
	unsigned int type;
	ssize_t r;

	if (argc != 2)
	{
		dprintf(STDERR_FILENO, "Usage: elf_header elf_filename\n");
		exit(98);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't open file %s\n", argv[1]);
		exit(98);
	}

	r = read(fd, buf, 64);
	if (r < 64 || buf[0] != 0x7f || buf[1] != 'E' ||
		buf[2] != 'L' || buf[3] != 'F')
	{
		dprintf(STDERR_FILENO, "Error: Not an ELF file: %s\n", argv[1]);
		close(fd);
		exit(98);
	}

	class      = buf[EI_CLASS];
	data       = buf[EI_DATA];
	version    = buf[EI_VERSION];
	osabi      = buf[EI_OSABI];
	abiversion = buf[EI_ABIVERSION];
	type       = buf[16] | (buf[17] << 8);

	printf("ELF Header:\n");
	print_magic(buf);

	printf("  Class:                             ");
	if (class == ELFCLASS32)      printf("ELF32\n");
	else if (class == ELFCLASS64) printf("ELF64\n");
	else                          printf("<unknown: %x>\n", class);

	printf("  Data:                              ");
	if (data == ELFDATA2LSB)      printf("2's complement, little endian\n");
	else if (data == ELFDATA2MSB) printf("2's complement, big endian\n");
	else                          printf("<unknown: %x>\n", data);

	printf("  Version:                           %d", version);
	if (version == EV_CURRENT)    printf(" (current)\n");
	else                          printf("\n");

	print_osabi(osabi);

	printf("  ABI Version:                       %d\n", abiversion);

	print_type(type, data);
	print_entry(buf, class, data);

	close(fd);
	return (0);
}
