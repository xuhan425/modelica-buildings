
///////////////////////////////////////////////////////////////////////////////
///
/// \file   ffd_wrap.h
///
/// \brief  wrapper to call FFD engine to evaluate the input; the function ffd_isat
///         is then called by usrfgh. A important note is that this function call only
///         evaluate f, not g, which is the jacobian matrix.
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
#ifndef FFD_WRAP_H_INCLUDED
#define FFD_WRAP_H_INCLUDED
#include "ffd_engine.h"
#include "common.h"
#include "utility_isat.h"

void ffd_ISAT (int need[], double x[], double f[], double g[][nf_SIZE], void *p);
int read_existing();
void numericalDifferentiation (double g[][nf_SIZE]);

#endif
