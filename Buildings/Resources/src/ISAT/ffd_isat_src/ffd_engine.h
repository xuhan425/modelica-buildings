///////////////////////////////////////////////////////////////////////////////
///
/// \file   ffd_engine.h
///
/// \brief  main entrance of parallel FFD program to be called by usrfgh.c
///         this is a mirror of ../ffd_src/open_main.c with minor revisions
///
/// \author Wei Tian
///         University of Miami, Schneider Electric
///         w.tian@umiami.edu, Wei.Tian@Schneider-Electric.com
///
/// \date   10/19/2018
///
/// \ This codes have been extensively validated and for the results please refer
///   to the journal paper:
///   Tian, Wei, Thomas Alonso Sevilla, and Wangda Zuo. "A systematic
///   evaluation of accelerating indoor airflow simulations using cross-platform parallel computing."
///   Journal of Building Performance Simulation 10.3 (2017): 243-255.
///
/// \ Update the functions for users to choose the available flatform and device.
///
///\  All RIGHTS RESERVED.
///////////////////////////////////////////////////////////////////////////////
#ifndef _FFD_ENGINE_H
#define _FFD_ENGINE_H

#include "../ffd_src/timing.h"
#include "../ffd_src/data_writer.h"
#include "../ffd_src/initialization.h"
#include "../ffd_src/geometry.h"
#include "../ffd_src/utility.h"
#include "../ffd_src/solver.h"

#include "../ffd_src/data_structure.h"
#include "../ffd_src/sci_reader.h"

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS
#ifdef __APPLE__ || __linux__
  #include <OpenCL/opencl.h>
#else
  #include <CL\cl.h>
#endif

#ifdef _MSC_VER
  #define _CRT_SECURE_NO_WARNINGS
#endif

#ifdef _msc_ver
  #include <windows.h>
#elif defined __GNUC__
  #ifdef __WIN64 || __WIN32
    #include <windows.h>
  #elif __APPLE__ || __linux__
    #include <unistd.h>
  #else
    #include <unistd.h>
  #endif
#else
  #include <unistd.h>
#endif

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
/// Main routine of FFD
///
///\para coupled simulation Integer to identify the simulation type
///
///\return 0 if no error occurred
///////////////////////////////////////////////////////////////////////////////
int ffd_prep(int cosimulation);

///////////////////////////////////////////////////////////////////////////////
/// Allocate memory for variables
///
///\param para Pointer to FFD parameters
///
///\return No return needed
///////////////////////////////////////////////////////////////////////////////
int allocate_memory_opencl(PARA_DATA *para);

#endif
