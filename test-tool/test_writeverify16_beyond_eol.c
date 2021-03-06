/* -*-  mode:c; tab-width:8; c-basic-offset:8; indent-tabs-mode:nil;  -*- */
/* 
   Copyright (C) 2013 Ronnie Sahlberg <ronneisahlberg@gmail.com>
   
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>

#include <CUnit/CUnit.h>

#include "iscsi.h"
#include "scsi-lowlevel.h"
#include "iscsi-test-cu.h"


void
test_writeverify16_beyond_eol(void)
{ 
        int i;

        CHECK_FOR_DATALOSS;
        CHECK_FOR_SBC;

        logging(LOG_VERBOSE, LOG_BLANK_LINE);
        logging(LOG_VERBOSE, "Test WRITEVERIFY16 1-256 blocks one block beyond the end");
        memset(scratch, 0xa6, 256 * block_size);
        for (i = 1; i <= 256; i++) {
                if (maximum_transfer_length && maximum_transfer_length < i) {
                        break;
                }
                WRITEVERIFY16(sd, num_blocks + 1 - i,
                              i * block_size, block_size, 0, 0, 1, 0, scratch,
                              EXPECT_LBA_OOB);
        }

        logging(LOG_VERBOSE, "Test WRITEVERIFY16 1-256 blocks at LBA==2^63");
        for (i = 1; i <= 256; i++) {
                if (maximum_transfer_length && maximum_transfer_length < i) {
                        break;
                }
                WRITEVERIFY16(sd, 0x8000000000000000ULL,
                              i * block_size, block_size, 0, 0, 1, 0, scratch,
                              EXPECT_LBA_OOB);
        }

        logging(LOG_VERBOSE, "Test WRITEVERIFY16 1-256 blocks at LBA==-1");
        for (i = 1; i <= 256; i++) {
                if (maximum_transfer_length && maximum_transfer_length < i) {
                        break;
                }
                WRITEVERIFY16(sd, -1,
                              i * block_size, block_size, 0, 0, 1, 0, scratch,
                              EXPECT_LBA_OOB);
        }

        logging(LOG_VERBOSE, "Test WRITEVERIFY16 2-256 blocks all but one block beyond the end");
        for (i = 2; i <= 256; i++) {
                if (maximum_transfer_length && maximum_transfer_length < i) {
                        break;
                }
                WRITEVERIFY16(sd, num_blocks - 1,
                              i * block_size, block_size, 0, 0, 1, 0, scratch,
                              EXPECT_LBA_OOB);
        }
}
