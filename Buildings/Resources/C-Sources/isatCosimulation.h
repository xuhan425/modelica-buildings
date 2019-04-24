/*
 *
 * @file   isatCosimulation.h
 *
 * @brief  Header file for coupled simulation with ISAT
 *
 * \author Wangda Zuo
 *         University of Colorado Boulder
 *         Wangda.Zuo@colorado.edu 
 *
 *         Tian Wei
 *         University of Miami
 *         W.tian@miami.edu
 *
 *         Xu Han
 *         University of Colorado Boulder
 *         xuha3556@colorado.edu
 *
 * \date   4/23/2019
 *
 */

#include <stdio.h>

#if defined(_MSC_VER) || defined(__WIN32__) /* Windows */
#include <windows.h>
#else /* Linux*/
#include <dlfcn.h>  /*For load shared library*/
#include <unistd.h> /*For Linux function*/
#define Sleep(x) sleep(x/1000)
#endif

#ifndef _MODELICA_ISAT_COMMON_H
#define _MODELICA_ISAT_COMMON_H
#include "../src/ISAT/ffd_isat_src/modelica_isat_common.h"
#include <stdint.h> /* Needed to detect 32 vs. 64 bit using UINTPTR_MAX*/
#endif

CosimulationData *cosim;

/*declare the isat_dll function in DLL*/
void *isat_dll(CosimulationData *cosim);
