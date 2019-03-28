/*
 *
 * \file   cfdExchangeData.c
 *
 * \brief  Function to exchange data between Modelica and CFD
 *
 * \author Wangda Zuo
 *         University of Miami
 *         W.Zuo@miami.edu
 *
 * \date   8/3/2013
 *
 */
#include "isatCosimulation.h"
/*
 * Exchange the data between Modelica and CFD
 *
 * @param t0 Current time of integration for Modelica
 * @param dt Time step size for next synchronization defined by Modelica
 * @param u Pointer to the input data from Modelica to CFD
 * @param nU Number of inputs from Modelica to CFD
 * @param nY Number of outputs from CFD to Modelica
 * @param y Pointer to the output data from CFD to Modelica
 *
 * @return 0 if no error occurred
 */
int isatExchangeData(double t0, double dt, double *u, size_t nU, size_t nY,
                 double *t1, double *y) {
  size_t i, j, k;
  int verbose = 0;

  /*--------------------------------------------------------------------------
  | Write data to CFD
  | Command:
  | -1: fake data
  |  0: data has been read by the other program
  |  1: data waiting for the other program to read
  --------------------------------------------------------------------------*/
  /* If previous data hasn't been read, wait*/
  while(cosim->modelica->flag==1) {
    if(cosim->para->ffdError==1)
      ModelicaError(cosim->ffd->msg);
    else
      sleep(1);
  }

  cosim->modelica->input[0] = (REAL) u[0];	
  cosim->modelica->input[1] = (REAL) u[1];		

  /* Set the flag to new data*/
  cosim->modelica->flag = 1;

  /****************************************************************************
  | Copy data from CFD
  ****************************************************************************/
  /* If the data is not ready or not updated, check again*/
  while(cosim->ffd->flag!=1) {
    if(cosim->para->ffdError==1)
      ModelicaError(cosim->ffd->msg);
    else
      sleep(1);
  }

	y[0] = cosim->ffd->output[0];

  /* Update the data status*/
  cosim->ffd->flag = 0;

  /**t1 = cosim->ffd->t;*/

  return 0;
} /* End of cfdExchangeData()*/
