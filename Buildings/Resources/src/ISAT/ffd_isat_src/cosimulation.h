/*
	*
	* @file   cosimulation.h
	*
	* @brief  Functions for cosimulation
	*
	* @author Wangda Zuo
	*         University of Miami
	*         W.Zuo@miami.edu
	*
	* @date   8/3/2013
	*
	* This file provides functions that are used for conducting the coupled simulation
	* with Modelica
	*
	*/
#ifndef _COSIMULATION_H
#define _COSIMULATION_H
#endif

#ifdef _MSC_VER
#include <windows.h>
#elif defined __GNUC__
#ifdef _WIN64
#include <windows.h>
#elif _WIN32
#include <windows.h>
#elif __APPLE__
#include <unistd.h>
#elif __linux__
#include <unistd.h>
#else
#include <unistd.h>
#endif
#else
#include <unistd.h>
#endif

#include <stdio.h>

/*
#ifndef _DATA_STRUCTURE_H
#define _DATA_STRUCTURE_H
#include "../ffd_src/data_structure.h"
#endif
*/
#ifndef _MODELICA_FFD_COMMON_H
#define _MODELICA_FFD_COMMON_H
#include "modelica_ffd_common.h"
#endif

#ifndef _UTILITY_H
#define _UTILITY_H
#include "../ffd_src/utility.h"
#endif
/*
#ifndef _GEOMETRY_H
#define _GEOMETRY_H
#include "../ffd_src/geometry.h"
#endif
*/
#ifndef _MSC_VER /*Linux*/
#define Sleep(x) sleep(x/1000)
#endif

//#include "isat_dll.h"
//#include "ffd_isat.h"

FILE *file_log;
static CosimulationData *cosim;

/*
	* Write the FFD data for Modelica
	*
	* @param para Pointer to FFD parameters
	* @param var Pointer to FFD simulation variables
	*
	* @return 0 if no error occurred
	*/
int write_cosim_data(CosimulationData *cosim);

/*
	* Read the data from Modelica
	*
	* @param para Pointer to FFD parameters
	* @param var Pointer to FFD simulation variables
	* @param BINDEX pointer to boundary index
	*
	* @return 0 if no error occurred
	*/
float *read_cosim_data(CosimulationData *cosim);

void cosim_log(char *message, COSIM_MSG_TYPE msg_type);

int write_cosim_data(CosimulationData *cosim);