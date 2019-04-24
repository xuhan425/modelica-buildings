///////////////////////////////////////////////////////////////////////////////
///
/// \file   ffd_isat.c
///
/// \brief  define function to perform ffd_isat training and testing
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
#ifndef FFD_ISAT_H_INCLUDED
#define FFD_ISAT_H_INCLUDED
#endif

//#include "modelica_ffd_common.h"
#include "common.h"
#include "utility_isat.h"
#include "ffd_wrap.h"
#include "cosimulation.h"


void update_digit();
void nDemArrEva(int dimension, double xStep[], CosimulationData *cosim);
void evaluate (CosimulationData *cosim);
void randomCall (int nCall, int useNormalDistribution, int useUnboundedTest);
void accuracyTest ();
void writeRecord ();
double getRandom (int dimension, int useNormalDistribution, int useUnboundedTest);
double randNormal (double mu, double sigma);
void binaryTrain ();
double my_round(double x, int digits);
void findHash();
void addHash();




//#include "io.h"



