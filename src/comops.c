/*
 * comops.c
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
#include <termios.h>

#include "updateloader.h"

void init_com(int fd)
{
	int status;
	struct termios Opt;

	tcgetattr(fd, &Opt);
	tcflush(fd, TCIOFLUSH);
	cfsetispeed(&Opt, SPEED);
	cfsetospeed(&Opt, SPEED);

	Opt.c_cflag &= ~CSIZE;
	Opt.c_cflag |= DATA_BITS;
	Opt.c_cflag &= ~PARENB;
	Opt.c_iflag &= ~INPCK;
	Opt.c_cflag &= STOP_BITS;
	Opt.c_cc[VTIME] = 100;
	Opt.c_cc[VMIN] = 0;

	status = tcsetattr(fd, TCSANOW, &Opt);
	if (status != 0) {
		perror("Init Com  Error");
		return;
	}

	tcflush(fd, TCIOFLUSH);
}

int conn_com(int fd)
{
	char inbuf[10], outbuf[10];
	int ret;

	ret = read(fd, inbuf, 1);
	if (inbuf[0] == 0x35) {
		outbuf[0] = 'A';
		ret = write(fd, outbuf, 1);
		ret = read(fd, inbuf, 1);
		if (inbuf[0] == 0x35) {
			outbuf[0] = 'U';
			outbuf[1] = '3';
			ret = write(fd, outbuf, 2);
			ret = read(fd, inbuf, 1);
			if (inbuf[0] == 'R') {
				printf("Connect board success!\n");
				return 0;
			} else {
				perror("Hardware 3 Error");
				return -1;
			}
		} else {
			perror("Hardware 2 Error");
			return -1;
		}
	} else {
		perror("Hardware 2 Error");
		return -1;
	}

	return 0;
}
