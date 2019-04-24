/*
 *
 * \file   isatStartCosimulation.c
 *
 * \brief  Function to start the coupled simulation
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
#include "isatCosimulation.h"
#include <ModelicaUtilities.h>
/*
 * Start the cosimulation
 *
 * Allocate memory for cosimulation variables
 *
 */
void *isatcosim() {

  /****************************************************************************
  | return modelica error if more than one isat instances are created
  ****************************************************************************/
  if ( cosim != NULL )
    ModelicaError("ModelicaError: Only one room with ISAT can be used, but more than one is used.");

  /****************************************************************************
  | Allocate memory for cosimulation variables
  ****************************************************************************/
  cosim = NULL;
  cosim = (CosimulationData *) malloc(sizeof(CosimulationData));
  if (cosim == NULL){
    ModelicaError("Failed to allocate memory for cosim in cfdcosim.c");
  }
  cosim->para = NULL;
  cosim->para = (ParameterSharedData *) malloc(sizeof(ParameterSharedData));
  if (cosim->para == NULL){
    ModelicaError("Failed to allocate memory for cosim->para in cfdcosim.c");
  }

  cosim->modelica = NULL;
  cosim->modelica = (ModelicaSharedData *) malloc(sizeof(ModelicaSharedData));
  if (cosim->para == NULL){
    ModelicaError("Failed to allocate memory for cosim->modelica in cfdcosim.c");
  }

  cosim->isat = NULL;
  cosim->isat = (ffdSharedData *) malloc(sizeof(ffdSharedData));
    if (cosim->para == NULL){
    ModelicaError("Failed to allocate memory for cosim->isat in cfdcosim.c");
  }

  /****************************************************************************
  | Initialize cosimulation variables
  ****************************************************************************/
  cosim->modelica->flag = 0;
  cosim->isat->flag = 0;
  cosim->para->flag = 1;
  cosim->para->ffdError = 0;
  cosim->para->nSur = 0;
  cosim->para->nSen = 0;
  cosim->para->nConExtWin = 0;
  cosim->para->nPorts = 0;
  cosim->para->sha = 0;
  cosim->para->nC = 0;
  cosim->para->nXi = 0;
  cosim->isat->msg = NULL;
  cosim->para->fileName = NULL;
  cosim->para->are = NULL;
  cosim->para->til = NULL;
  cosim->para->bouCon = NULL;
  cosim->para->name = NULL;
  cosim->para->sensorName = NULL;
  cosim->isat->senVal = NULL;
  cosim->modelica->CPor = NULL;
  cosim->isat->CPor = NULL;
  cosim->modelica->XiPor = NULL;
  cosim->isat->XiPor = NULL;
  cosim->modelica->TPor = NULL;
  cosim->isat->TPor = NULL;
  cosim->para->portName = NULL;
  cosim->modelica->mFloRatPor = NULL;
  cosim->isat->temHea = NULL;
  cosim->modelica->temHea = NULL;
  cosim->modelica->shaConSig = NULL;
  cosim->modelica->shaAbsRad = NULL;
  cosim->isat->TSha = NULL;

  return (void*) cosim;
} /* End of isatcosim()*/
