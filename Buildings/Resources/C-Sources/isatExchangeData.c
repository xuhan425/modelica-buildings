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
#include "stdlib.h"
#include "stdio.h"

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
	FILE *f = fopen("log.txt","a+");
	
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

  cosim->modelica->t = (REAL) t0;
  cosim->modelica->dt = (REAL) dt;

	fprintf(f, "Modelica time =%f\n",cosim->modelica->t);	
	
  /* Copy the Modelica data to shared memory*/
  for(i=0; i<cosim->para->nSur; i++) {
    cosim->modelica->temHea[i] = (REAL) u[i];
		fprintf(f, "temHea:i=%d, u[%d]=%f\n",i,i,u[i]);
  }

  if(cosim->para->sha==1) {
    for(j=0; j<cosim->para->nConExtWin; j++) {
      cosim->modelica->shaConSig[j] = (REAL) u[i+j];
      cosim->modelica->shaAbsRad[j] = (REAL) u[i+j+cosim->para->nConExtWin];
			fprintf(f, "j=%d, shaConsig[%d]=%f,shaAbsRad[%d]=%f\n",j,j,u[i+j],j,u[i+j+cosim->para->nConExtWin]);
    }
    i = i + 2*cosim->para->nConExtWin;
  }

  cosim->modelica->sensibleHeat = (REAL) u[i];
	fprintf(f,"i=%d,sensibleHeat=%f\n",i,u[i]);
  i++;

  cosim->modelica->latentHeat = (REAL) u[i];
	fprintf(f,"i=%d,latentHeat=%f\n",i,u[i]);
  i++;

  cosim->modelica->p = (REAL) u[i];
	fprintf(f,"i=%d,p=%f\n",i,u[i]);
  i++;

  for(j=0; j<cosim->para->nPorts; j++) {
    cosim->modelica->mFloRatPor[j] = (REAL) u[i+j];
    cosim->modelica->TPor[j] = (REAL) u[i+j+cosim->para->nPorts];
		fprintf(f, "j=%d, mFloRatPor[%d]=%f,TPor[%d]=%f\n",j,j,u[i+j],j,u[i+j+cosim->para->nPorts]);
  }

  i = i + 2*cosim->para->nPorts;
	fprintf(f,"i=%d\n",i);
  for(j=0; j<cosim->para->nPorts; j++)
    for(k=0; k<cosim->para->nXi; k++) {
      cosim->modelica->XiPor[j][k] = (REAL) u[i+j*cosim->para->nXi+k];
			fprintf(f, "j=%d,k=%d,XiPor[%d][%d]=%f\n",j,k,j,k,u[i+j*cosim->para->nXi+k]);
    }

  i = i + cosim->para->nPorts*cosim->para->nXi;
	fprintf(f,"i=%d\n",i);
  for(j=0; j<cosim->para->nPorts; j++)
    for(k=0; k<cosim->para->nC; k++) {
      cosim->modelica->CPor[j][k] = (REAL) u[i+j*cosim->para->nC+k];
			fprintf(f, "j=%d,k=%d,CPor[%d][%d]=%f\n",j,k,j,k,u[i+j*cosim->para->nC+k]);			
    }

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

  /* Get the temperature/heat flux for solid surface*/
  for(i=0; i<cosim->para->nSur; i++) {
    y[i] = cosim->ffd->temHea[i];
		fprintf(f, "temp/flux of surface: i=%d, y[%d]=%f\n",i,i,y[i]);		
  }

  /* Get the averaged room temperature*/
  y[i] = cosim->ffd->TRoo;
	fprintf(f, "ave room temp: i=%d, y[%d]=%f\n",i,i,y[i]);
  i++;
	fprintf(f,"i=%d\n",i);
  /* Get the temperature of shading device if there is a shading device*/
  if(cosim->para->sha==1) {
    for(j=0; j<cosim->para->nConExtWin; i++, j++) {
      y[i] = cosim->ffd->TSha[j];
			fprintf(f, "temperature shading device: i=%d, y[%d]=%f\n",i,i,y[i]);			
    }
  }

  /* Get the temperature fluid at the fluid ports*/
  for(j=0; j<cosim->para->nPorts; i++, j++) {
    y[i] = cosim->ffd->TPor[j];
		fprintf(f, "temperature fluid: i=%d, y[%d]=%f\n",i,i,y[i]);		
  }

  /* Get the mass fraction at fluid ports*/
  for(j=0; j<cosim->para->nPorts; j++)
    for(k=0; k<cosim->para->nXi; k++, i++) {
       y[i] = cosim->ffd->XiPor[j][k];
			fprintf(f, "mass fraction: i=%d, y[%d]=%f\n",i,i,y[i]);			 
    }

  /* Get the trace substance at fluid ports*/
  for(j=0; j<cosim->para->nPorts; j++)
    for(k=0; k<cosim->para->nC; k++, i++) {
       y[i] = cosim->ffd->CPor[j][k];
			 fprintf(f, "trace substance: i=%d, y[%d]=%f\n",i,i,y[i]);
    }

  /* Get the sensor data*/
  for(j=0; j<cosim->para->nSen; j++, i++) {
    y[i] = cosim->ffd->senVal[j];
		fprintf(f, "sensor data:i=%d, y[%d]=%f\n",i,i,y[i]);
  }

  /* Update the data status*/
  cosim->ffd->flag = 0;

  *t1 = cosim->ffd->t;

  return 0;
} /* End of cfdExchangeData()*/
