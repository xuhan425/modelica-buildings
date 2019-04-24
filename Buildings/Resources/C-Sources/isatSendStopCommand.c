/*
 *
 * \file   isatSendStopCommand.c
 *
 * \brief  Function to send a stop command to terminate the ISAT simulation
 *
 * \author Wangda Zuo
 *         University of Colorado Boulder
 *         Wangda.Zuo@colorado.edu 
 *
 *         Xu Han
 *         University of Colorado Boulder
 *         xuha3556@colorado.edu
 *
 * \date   4/23/2019
 *
 */
#include "isatCosimulation.h"

/*
 * Send a stop command to terminate the ISAT simulation
 *
 * @return No return needed
 */
void isatSendStopCommand(void *thread) {

  size_t i = 0;
  size_t imax = 10000;

  /*send stop command to ISAT*/
  cosim->para->flag = 0;

  /* Wait for the feedback from ISAT*/
 while(cosim->para->flag==0 && i<imax) {
    if(cosim->para->ffdError==1) {
      ModelicaError(cosim->isat->msg);
    }
    else {
      Sleep(10);
      i++;
    }
  }

  if(i<imax) {
    if(cosim->para->ffdError==1) {
      ModelicaError(cosim->isat->msg);
    }
    else {
      ModelicaMessage("Successfully stopped the ISAT simulation.\n");
    }
  }
  else {
    ModelicaMessage("Error: Cannot stop the ISAT simulation in required time.");
  }

  /*free memory for variables*/
  if (cosim->para->fileName != NULL){
    free(cosim->para->fileName);
  }
  if (cosim->para->are != NULL){
    free(cosim->para->are);
  }
  if (cosim->para->til != NULL){
    free(cosim->para->til);
  }
  if (cosim->para->bouCon != NULL){
    free(cosim->para->bouCon);
  }
  if (cosim->para->nSur>0){
    for(i=0; i<cosim->para->nSur; i++) {
      free(cosim->para->name[i]);
    }
    if (cosim->para->name != NULL){
      free(cosim->para->name);
    }
  }
  if (cosim->para->nSen>0){
    if (cosim->isat->senVal != NULL){
      free(cosim->isat->senVal);
    }
    for(i=0; i<cosim->para->nSen; i++) {
      free(cosim->para->sensorName[i]);
    }
    if (cosim->para->sensorName != NULL){
      free(cosim->para->sensorName);
    }
  }
  if (cosim->modelica->temHea != NULL){
    free(cosim->modelica->temHea);
  }
  if (cosim->para->sha==1){
    if (cosim->modelica->shaConSig != NULL){
      free(cosim->modelica->shaConSig);
    }
    if (cosim->modelica->shaAbsRad != NULL){
      free(cosim->modelica->shaAbsRad);
    }
    if (cosim->isat->TSha != NULL){
      free(cosim->isat->TSha);
    }
  }
  if (cosim->para->nPorts>0){
    for(i=0; i<cosim->para->nPorts; i++) {
      free(cosim->modelica->XiPor[i]);
      free(cosim->isat->XiPor[i]);
      free(cosim->modelica->CPor[i]);
      free(cosim->isat->CPor[i]);
    }
    if (cosim->modelica->CPor != NULL){
      free(cosim->modelica->CPor);
    }
    if (cosim->isat->CPor != NULL){
      free(cosim->isat->CPor);
    }
    if (cosim->modelica->XiPor != NULL){
      free(cosim->modelica->XiPor);
    }
    if (cosim->isat->XiPor != NULL){
      free(cosim->isat->XiPor);
    }
    if (cosim->modelica->TPor != NULL){
      free(cosim->modelica->TPor);
    }
    if (cosim->isat->TPor != NULL){
      free(cosim->isat->TPor);
    }
    for(i=0; i<cosim->para->nPorts; i++) {
      free(cosim->para->portName[i]);
    }
    if (cosim->para->portName != NULL){
      free(cosim->para->portName);
    }
    if (cosim->modelica->mFloRatPor != NULL){
      free(cosim->modelica->mFloRatPor);
    }
  }
  if (cosim->isat->temHea != NULL){
    free(cosim->isat->temHea);
  }
  if (cosim->para != NULL){
    free(cosim->para);
  }
  if (cosim->modelica != NULL){
    free(cosim->modelica);
  }
  if (cosim->isat != NULL){
    free(cosim->isat);
  }
  if (cosim != NULL){
    free(cosim);
  }

} /* End of isatSendStopCommand*/
