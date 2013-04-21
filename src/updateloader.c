/*
 * updateloader.c
 *
 * A simple program upload kickstart and s1l to smartarm32xx board.
 * Created by (C) Copyright 2009
 * Zeliang Li, xiyou College of PR.China, lizeliang.linux@gmail.com.
 * Fei Song, xiyou College of PR.China, songfei.org@gmail.com
 *
 *************************************************************************************
 *
 * Modified 01-Sep-2009 (C)Copyright 2009
 *
 *************************************************************************************
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <getopt.h>		/*struct option */
#include <libgen.h>		/* basename() */

#include  "updateloader.h"

static const char *shortopts = "b:B:c:e:k:p:s:t:h";
struct option longopts[] = {
	{"bootloader", required_argument, NULL, 'b'},
	{"Block", required_argument, NULL, 'B'},
	{"check", required_argument, NULL, 'c'},
	{"erase", required_argument, NULL, 'e'},
	{"kickstart", required_argument, NULL, 'k'},
	{"program", required_argument, NULL, 'p'},
	{"stage1", required_argument, NULL, 's'},
	{"terminal", required_argument, NULL, 't'},
	{"help", no_argument, NULL, 'h'},
	{0, 0, 0, 0},
};

void help_menu(int argc, char **argv)
{
	printf
	    ("Updateloader 0.2: A upload bootloader software for GNU/linux\n");
	printf("Usage: %s -t comname -b bootloader "
	       "[-e|c|p [ALL|NAND|NOR|SPI]]" "[-k|s filename]"
	       "[-B blocknum]\n", basename(argv[0]));
	printf("Options:\n");
	printf("\t-b, --bootloader\t\tUpdate bootloader file name\n");
	printf("\t-B, --Block\t\t\tThe number of block\n");
	printf("\t-c, --check [ALL|NAND|NOR|SPI]\t"
	       "Check code in Flash/Memory is aviliable \n");
	printf("\t-e, --erase [ALL|NAND|NOR|SPI]\tErase Flash/Memory\n");
	printf("\t-k, --kickstart\t\t\tUpdate kickstart file name\n");
	printf("\t-p, --program\t\t\tProgram in Flash/Memory\n");
	printf("\t-s, --stage1\t\t\tUpdate stage1 file name\n");
	printf("\t-t, --terminal\t\t\tChioce which ttyS to open\n");
	printf("\t-h, --help\t\t\tHelp for updateloader\n");
}

void menuinfo(int argc, char **argv, struct updateloader *up)
{
	int ch;
	struct stat buf;

	opterr = 0;
	while ((ch = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1) {
		switch (ch) {
		case 'b':
			up->bfile = optarg;
			if (stat(up->bfile, &buf) == -1) {
				printf("Usage: -b bootloader must be given\n");
				exit(-1);
			}
			break;
		case 'B':
			up->block = atoi(optarg);
			if ((up->block < 0) || (up->block > 2048)) {
				printf("error block\n");
				exit(-1);
			}
			break;
		case 'c':
			if (strcmp("ALL", optarg) == 0) {
				up->flags |= CHECK_ALL;
			} else if (strcmp("NAND", optarg) == 0) {
				up->flags |= CHECK_NAND;
			} else if (strcmp("NOR", optarg) == 0) {
				up->flags |= CHECK_NOR;
			} else if (strcmp("SPI", optarg) == 0) {
				up->flags |= CHECK_SPI;
			} else {
				printf("Usage: -c [ALL|NAND|NOR|SPI]\n");
				exit(-1);
			}
			break;
		case 'e':
			if (strcmp("ALL", optarg) == 0) {
				up->flags |= ERASE_ALL;
			} else if (strcmp("NAND", optarg) == 0) {
				up->flags |= ERASE_NAND;
			} else if (strcmp("NOR", optarg) == 0) {
				up->flags |= ERASE_NOR;
			} else if (strcmp("SPI", optarg) == 0) {
				up->flags |= ERASE_SPI;
			} else {
				printf("Usage: -e [ALL|NAND|NOR|SPI]\n");
				exit(-1);
			}
			break;
		case 'k':
			up->kfile = optarg;
			if (stat(up->kfile, &buf) == -1) {
				printf("Usage: -k kickstart should be given\n");
				exit(-1);
			}
			break;
		case 'p':
			if (strcmp("ALL", optarg) == 0) {
				up->flags |= PROGM_ALL;
			} else if (strcmp("NAND", optarg) == 0) {
				up->flags |= PROGM_NAND;
			} else if (strcmp("NOR", optarg) == 0) {
				up->flags |= PROGM_NOR;
			} else if (strcmp("SPI", optarg) == 0) {
				up->flags |= PROGM_SPI;
			} else {
				printf("Usage: -p [ALL|NAND|NOR|SPI]\n");
				exit(-1);
			}
			break;
		case 's':
			up->sfile = optarg;
			if (stat(up->sfile, &buf) == -1) {
				printf("Usage: -s stage1 should be given\n");
				exit(-1);
			}
		case 't':
			up->termial = optarg;
			if ((stat(up->termial, &buf) != -1)
			    && S_ISCHR(buf.st_mode)) {
				continue;
			} else {
				printf("Usage: -t termial must be given\n");
				exit(-1);
			}
			break;
		case 'h':
		default:
			help_menu(argc, argv);
			exit(0);
		}
	}
}

int main(int argc, char **argv)
{
	char *devfile = "/dev/ttyS0";
	struct updateloader up = { NULL, NULL, 0, -1, NULL, NULL };

	if (argc < 4) {
		help_menu(argc, argv);
		exit(-1);
	}
	menuinfo(argc, argv, &up);
	printf("termial = %s\nbfile=%s\nflags=0x%0x\nblock=%d\n", up.termial,
	       up.bfile, up.flags, up.block);
	return 0;

	int fd = open(devfile, O_RDWR);
	if (fd == -1) {
		perror("Can't open dev");
		exit(0);
	}

	init_com(fd);

	printf("Please reset your board!\n");

	conn_com(fd);

	return 0;
}
