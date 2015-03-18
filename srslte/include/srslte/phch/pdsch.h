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


#ifndef PDSCH_
#define PDSCH_

#include "srslte/config.h"
#include "srslte/common/phy_common.h"
#include "srslte/mimo/precoding.h"
#include "srslte/mimo/layermap.h"
#include "srslte/modem/mod.h"
#include "srslte/modem/demod_soft.h"
#include "srslte/scrambling/scrambling.h"
#include "srslte/phch/dci.h"
#include "srslte/phch/regs.h"
#include "srslte/phch/sch.h"
#include "srslte/phch/harq.h"

#define SRSLTE_PDSCH_MAX_TDEC_ITERS         5

typedef _Complex float cf_t;

/* PDSCH object */
typedef struct SRSLTE_API {
  srslte_cell_t cell;
  
  uint32_t max_re;
  bool rnti_is_set; 
  uint16_t rnti; 
  
  /* buffers */
  // void buffers are shared for tx and rx
  cf_t *ce[SRSLTE_MAX_PORTS];
  cf_t *symbols[SRSLTE_MAX_PORTS];
  cf_t *x[SRSLTE_MAX_PORTS];
  cf_t *d;
  void *e;

  /* tx & rx objects */
  srslte_srslte_modem_table_t mod[4];
  srslte_demod_soft_t demod;
  srslte_sequence_t seq[SRSLTE_NSUBFRAMES_X_FRAME];
  srslte_precoding_t precoding; 

  srslte_sch_t dl_sch;
  
} srslte_pdsch_t;

SRSLTE_API int srslte_pdsch_init(srslte_pdsch_t *q, 
                                 srslte_cell_t cell);

SRSLTE_API void srslte_pdsch_free(srslte_pdsch_t *q);

SRSLTE_API int srslte_pdsch_set_rnti(srslte_pdsch_t *q, 
                                     uint16_t rnti);

SRSLTE_API int srslte_pdsch_encode(srslte_pdsch_t *q,
                                   srslte_harq_t *harq_process,
                                   uint8_t *data, 
                                   cf_t *sf_symbols[SRSLTE_MAX_PORTS]);

SRSLTE_API int srslte_pdsch_encode_rnti(srslte_pdsch_t *q,
                                        srslte_harq_t *harq_process,
                                        uint8_t *data, 
                                        uint16_t rnti,
                                        cf_t *sf_symbols[SRSLTE_MAX_PORTS]);

SRSLTE_API int srslte_pdsch_decode(srslte_pdsch_t *q, 
                                   srslte_harq_t *harq_process, 
                                   cf_t *sf_symbols, 
                                   cf_t *ce[SRSLTE_MAX_PORTS],
                                   float noise_estimate, 
                                   uint8_t *data);

SRSLTE_API int srslte_pdsch_decode_rnti(srslte_pdsch_t *q, 
                                        srslte_harq_t *harq_process, 
                                        cf_t *sf_symbols, 
                                        cf_t *ce[SRSLTE_MAX_PORTS],
                                        float noise_estimate, 
                                        uint16_t rnti,
                                        uint8_t *data);

SRSLTE_API float srslte_pdsch_average_noi(srslte_pdsch_t *q); 

SRSLTE_API uint32_t srslte_pdsch_last_noi(srslte_pdsch_t *q); 

#endif
