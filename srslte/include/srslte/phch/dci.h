/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2014 The srsLTE Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution.
 *
 * \section LICENSE
 *
 * This file is part of the srsLTE library.
 *
 * srsLTE is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsLTE is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * A copy of the GNU Lesser General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#ifndef DCI_
#define DCI_

#include <stdint.h>

#include "srslte/config.h"
#include "srslte/common/phy_common.h"
#include "srslte/phch/ra.h"

typedef _Complex float cf_t;

/**
 * DCI message generation according to the formats, as specified in
 * 36.212 Section 5.3.3.1
 *
 */


#define DCI_MAX_BITS  57

typedef enum {
  SRSLTE_DCI_FORMAT0, 
  SRSLTE_DCI_FORMAT1, 
  SRSLTE_DCI_FORMAT1A, 
  SRSLTE_DCI_FORMAT1C, 
  SRSLTE_DCI_FORMAT_ERROR
} srslte_dci_format_t;

// Each type is for a different interface to packing/unpacking functions
typedef struct SRSLTE_API {
  enum {
    SRSLTE_DCI_MSG_TYPE_PUSCH_SCHED, 
    SRSLTE_DCI_MSG_TYPE_PDSCH_SCHED, 
    SRSLTE_DCI_MSG_TYPE_MCCH_CHANGE, 
    SRSLTE_DCI_MSG_TYPE_TPC_COMMAND, 
    SRSLTE_DCI_MSG_TYPE_RA_PROC_PDCCH
  } type;
  srslte_dci_format_t format;
}srslte_dci_msg_type_t;

typedef enum {
  SRSLTE_DCI_SPEC_COMMON_ = 0, 
  SRSLTE_DCI_SPEC_UE = 1
} dci_spec_t;

typedef struct SRSLTE_API {
  uint32_t L;    // Aggregation level
  uint32_t ncce; // Position of first CCE of the dci
} srslte_dci_location_t;

typedef struct SRSLTE_API {
  uint8_t data[DCI_MAX_BITS];
  uint32_t nof_bits;
} srslte_dci_msg_t;

/* Converts a received PDSCH DL scheduling DCI message 
 * to ra structures ready to be passed to the harq setup function
 */
SRSLTE_API int srslte_dci_msg_to_ra_dl(srslte_dci_msg_t *msg, 
                                       uint16_t msg_rnti,
                                       srslte_cell_t cell,
                                       uint32_t cfi,
                                       srslte_ra_pdsch_t *ra_dl);

SRSLTE_API int srslte_dci_msg_to_ra_ul(srslte_dci_msg_t *msg, 
                                       uint32_t nof_prb,
                                       uint32_t n_rb_ho, 
                                       srslte_ra_pusch_t *ra_ul);

SRSLTE_API int srslte_dci_rar_to_ra_ul(uint32_t rba, 
                                       uint32_t trunc_mcs, 
                                       bool hopping_flag, 
                                       uint32_t nof_prb, 
                                       srslte_ra_pusch_t *ra); 

SRSLTE_API srslte_dci_format_t srslte_dci_format_from_string(char *str);

SRSLTE_API char* srslte_dci_format_string(srslte_dci_format_t format);

SRSLTE_API int srslte_dci_location_set(srslte_dci_location_t *c, 
                                       uint32_t L, 
                                       uint32_t nCCE);

SRSLTE_API bool srslte_dci_location_isvalid(srslte_dci_location_t *c);

SRSLTE_API int srslte_dci_msg_get_type(srslte_dci_msg_t *msg, 
                                       srslte_dci_msg_type_t *type, 
                                       uint32_t nof_prb, 
                                       uint16_t msg_rnti);

SRSLTE_API void srslte_dci_msg_type_fprint(FILE *f, 
                                           srslte_dci_msg_type_t type);

// For srslte_dci_msg_type_t = SRSLTE_DCI_MSG_TYPE_PUSCH_SCHED
SRSLTE_API int srslte_dci_msg_pack_pusch(srslte_ra_pusch_t *data, 
                                         srslte_dci_msg_t *msg, 
                                         uint32_t nof_prb);

SRSLTE_API int srslte_dci_msg_unpack_pusch(srslte_dci_msg_t *msg, 
                                           srslte_ra_pusch_t *data, 
                                           uint32_t nof_prb);

// For srslte_dci_msg_type_t = SRSLTE_DCI_MSG_TYPE_PDSCH_SCHED
SRSLTE_API int srslte_dci_msg_pack_pdsch(srslte_ra_pdsch_t *data, 
                                         srslte_dci_msg_t *msg, 
                                         srslte_dci_format_t format, 
                                         uint32_t nof_prb, 
                                         bool srslte_crc_is_crnti);

SRSLTE_API int srslte_dci_msg_unpack_pdsch(srslte_dci_msg_t *msg, 
                                           srslte_ra_pdsch_t *data, 
                                           uint32_t nof_prb, 
                                           bool srslte_crc_is_crnti);

SRSLTE_API uint32_t srslte_dci_format_sizeof(srslte_dci_format_t format, 
                                             uint32_t nof_prb);

#endif // DCI_
