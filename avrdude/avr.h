/*
 * Copyright 2000  Brian S. Dean <bsd@bsdhome.com>
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY BRIAN S. DEAN ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL BRIAN S. DEAN BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * 
 */

/* $Id$ */

#ifndef __avr_h__
#define __avr_h__

#include <stdio.h>


#define PPI_AVR_VCC    0x0f  /* ppi pins 2-5, data reg bits 0-3 */
#define PIN_AVR_BUFF   6
#define PIN_AVR_RESET  7
#define PIN_AVR_SCK    8
#define PIN_AVR_MOSI   9
#define PIN_AVR_MISO  10
#define PIN_LED_ERR    1
#define PIN_LED_RDY   14
#define PIN_LED_PGM   16
#define PIN_LED_VFY   17



/*
 * AVR memory designations; the order of these is important, these are
 * used as indexes into statically initialized data, don't change them
 * around.
 */
typedef enum {
  AVR_EEPROM,
  AVR_FLASH
} AVRMEM;

#define AVR_MAXMEMTYPES 2     /* just flash and eeprom */

#if 0
struct avrmem {
  AVRMEM          memtype;
  int             startaddr;
  int             size;
  unsigned char * buf;
  struct avrmem * next;
};
#endif

struct avrpart {
  char          * partdesc;         /* long part name */
  char          * optiontag;        /* short part name */
  int             memsize[AVR_MAXMEMTYPES]; /* sizes for eeprom,
                                               flash, etc, indexed by
                                               AVR_EEPROM or AVR_FLASH */
  unsigned char   f_readback;       /* flash write polled readback value */
  unsigned char   e_readback[2];    /* eeprom write polled readback values */
  int             min_write_delay;  /* microseconds */
  int             max_write_delay;  /* microseconds */
  int             chip_erase_delay; /* microseconds */
  unsigned char * mem[AVR_MAXMEMTYPES]; /* pointers to avr memory
                                           buffers, indexed by
                                           AVR_EEPROM or AVR_FLASH */
};

extern struct avrpart parts[];



int avr_list_parts ( FILE * f, char * prefix );

struct avrpart * avr_find_part ( char * p );

int avr_txrx_bit ( int fd, int bit );

unsigned char avr_txrx ( int fd, unsigned char byte );

int avr_cmd ( int fd, unsigned char cmd[4], unsigned char res[4] );

unsigned char avr_read_byte ( int fd, struct avrpart * p,
                              AVRMEM memtype, unsigned short addr );

int avr_read ( int fd, struct avrpart * p, AVRMEM memtype );

int avr_write_byte ( int fd, struct avrpart * p, AVRMEM memtype, 
                     unsigned short addr, unsigned char data );

int avr_write ( int fd, struct avrpart * p, AVRMEM memtype, int size );

int avr_program_enable ( int fd );

int avr_chip_erase ( int fd, struct avrpart * p );

int avr_signature ( int fd, unsigned char sig[4] );

void avr_powerup ( int fd );

void avr_powerdown ( int fd );

int avr_initialize ( int fd, struct avrpart * p );

char * avr_memtstr ( AVRMEM memtype );

int avr_initmem ( struct avrpart * p );

int avr_verify(struct avrpart * p, struct avrpart * v, AVRMEM memtype, 
               int size);

void avr_display ( FILE * f, struct avrpart * p, char * prefix );


#endif