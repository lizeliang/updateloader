/*
 * updateloader.h
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

#ifndef __UPDATELOADER_H_
#define __UPDATELOADER_H_

#define SPEED B115200
#define DATA_BITS CS8
#define STOP_BITS ~CSTOPB

struct updateloader {
	char *termial;
	char *bfile;
	unsigned int flags;
	unsigned int block;
	char *kfile;
	char *sfile;
};

#define ERASE_MODE (0xff << 16)
#define CHECK_MODE (0xff << 8)
#define PROGM_MODE (0xff)

#define ERASE_ALL 	(0x3 << 22)
#define ERASE_NAND 	(0x3 << 20)
#define ERASE_NOR 	(0x3 << 18)
#define ERASE_SPI 	(0x3 << 16)

#define CHECK_ALL 	(0x3 << 14)
#define CHECK_NAND	(0x3 << 12)
#define CHECK_NOR 	(0x3 << 10)
#define CHECK_SPI 	(0x3 << 8)

#define PROGM_ALL 	(0x3 << 6)
#define PROGM_NAND 	(0x3 << 4)
#define PROGM_NOR 	(0x3 << 2)
#define PROGM_SPI 	(0x3 << 0)

extern void init_com(int fd);
extern int conn_com(int fd);
#endif
