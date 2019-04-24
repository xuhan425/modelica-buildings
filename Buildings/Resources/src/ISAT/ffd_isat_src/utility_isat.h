///////////////////////////////////////////////////////////////////////////////
///
/// \file   utility.h
///
/// \brief  define utility functions for isat_ffd
///
/// \author Wei Tian
///         University of Miami, Schneider Electric
///         w.tian@umiami.edu, Wei.Tian@Schneider-Electric.com
///         Dan Li
///         University of Miami
///
/// \date   10/18/2018
///
///\  All RIGHTS RESERVED.
///////////////////////////////////////////////////////////////////////////////


#ifndef UTILITY_IAST_H_INCLUDED
#define UTILITY_IAST_H_INCLUDED
#include "common.h"
#include "../ffd_src/data_structure.h"
#include "../ffd_src/geometry.h"

FILE *file_log;

void mpc_log(char *message, MPC_MSG_TYPE msg_type);

#endif




