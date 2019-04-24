/*
	*
	* \file   cosimulation.c
	*
	* \brief  Functions for cosimulation
	*
	* \author Wangda Zuo
	*         University of Miami
	*         W.Zuo@miami.edu
	*
	* \date   8/3/2013
	*
	* This file provides functions that are used for conducting the coupled simulation
	* with Modelica
	*
	*/

#include "cosimulation.h"
/*static CosimulationData *cosim;*/

/*
	* Read the coupled simulation parameters defined by Modelica
	*
	* @param para Pointer to FFD parameters
	* @param var Pointer to FFD simulation variables
	* @param BINDEX pointer to boundary index
	*
	* @return 0 if no error occurred
	*/
int read_cosim_parameter(CosimulationData *cosim){
  int i;

  cosim_log("-------------------------------------------------------------------",
          COSIM_NORMAL);
  if(cosim->para->version=="DEBUG") {
    cosim_log("read_cosim_parameter(): "
            "Received the following coupled simulation parameters:",
             COSIM_NORMAL);
  }

  /****************************************************************************
  | Compare number of solid surface boundaries
  | (Wall, Window Glass with and without shading, and Window Frame)
  ****************************************************************************/
		if (cosim->para->version == "DEBUG") {
			cosim_log("Modelica Surfaces are:", COSIM_NORMAL);
			for (i = 0; i<cosim->para->nSur; i++) {
				sprintf(comsg, "\t%s", cosim->para->name[i]);
				cosim_log(comsg, COSIM_NORMAL);
			}
		}

  /****************************************************************************
  | Compare the number of fluid ports
  ****************************************************************************/
		if (cosim->para->version == "DEBUG") {
			sprintf(comsg, "\tnPorts=%d", cosim->para->nPorts);
			cosim_log(comsg, COSIM_NORMAL);
			}

  /****************************************************************************
  | Compare the number of sensors
  ****************************************************************************/
		if (cosim->para->version == "DEBUG") {
			sprintf(comsg, "\tnSen=%d", cosim->para->nSen);
			cosim_log(comsg, COSIM_NORMAL);
		}

  /****************************************************************************
  | Compare the number of species
  ****************************************************************************/
		if (cosim->para->version == "DEBUG") {
			sprintf(comsg, "\tnXi=%d", cosim->para->nXi);
			cosim_log(comsg, COSIM_NORMAL);
		}

  /****************************************************************************
  | Compare the number of trace substances
  ****************************************************************************/
		if (cosim->para->version == "DEBUG") {
			sprintf(comsg, "\tnC=%d", cosim->para->nC);
			cosim_log(comsg, COSIM_NORMAL);

			sprintf(comsg, "\tnConExtWin=%d", cosim->para->nConExtWin);
			cosim_log(comsg, COSIM_NORMAL);

			sprintf(comsg, "\tsha=%d", cosim->para->sha);
			cosim_log(comsg, COSIM_NORMAL);
		}

  /****************************************************************************
  | Print the information for surface boundaries
  ****************************************************************************/
  for(i=0; i<cosim->para->nSur; i++) {
    sprintf(comsg, "\tSurface %d: %s", i, cosim->para->name[i]);
    cosim_log(comsg, COSIM_NORMAL);
    sprintf(comsg, "\t\tArea:%f[m2],\t Tilt:%f[deg]",
            cosim->para->are[i], cosim->para->til[i]);
    cosim_log(comsg, COSIM_NORMAL);
    switch (cosim->para->bouCon[i]) {
      case 1:
        cosim_log("\t\tThermal boundary: Fixed temperature", COSIM_NORMAL);
        break;
      case 2:
        cosim_log("\t\tThermal boundary: Fixed heat flux", COSIM_NORMAL);
        break;
      default:
        sprintf(comsg,
        "Invalid value (%d) for thermal boundary condition. "
        "1: Fixed T; 2: Fixed heat flux",
        cosim->para->bouCon[i]);
        cosim_log(comsg, COSIM_ERROR);
        return 1;
    }
  }

  for(i=0; i<cosim->para->nPorts; i++) {
    sprintf(comsg, "\tFluid Ports %d: %s", i, cosim->para->portName[i]);
    cosim_log(comsg, COSIM_NORMAL);
  }

  for(i=0; i<cosim->para->nSen; i++) {
    sprintf(comsg, "\tSensor %d: %s", i, cosim->para->sensorName[i]);
    cosim_log(comsg, COSIM_NORMAL);
  }

  /****************************************************************************
  | Compare name and surface area of boundaries
  ****************************************************************************/
/*  if(compare_boundary_names(para)!=0) {
    cosim_log("read_cosim_parameter(): The boundary names were not consistent.",
    COSIM_ERROR);
    return 1;
  }
  else if(compare_boundary_area(para, var, BINDEX)!=0) {
    cosim_log("read_cosim_parameter(): The boundary areas were not consistent.",
    COSIM_ERROR);
    return 1;
  }
*/
  /****************************************************************************
  | Get the start time of co-simulation
  ****************************************************************************/
/*  para->mytime->t = cosim->modelica->t;
  sprintf(comsg, "read_cosim_parameter(): Simulation starts at %fs",
          para->mytime->t);
  cosim_log(comsg, COSIM_NORMAL);
*/
  /****************************************************************************
  | Get the air density if there is a fluid port
  ****************************************************************************/
/*  if(para->bc->nb_port>0 && para->prob->rho!=cosim->para->rho_start) {
    sprintf(comsg, "read_cosim_parameter(): Overwrite the density of air "
           "%f kg/m3 with %f kg/m3", para->prob->rho,
           cosim->para->rho_start);
    para->prob->rho = cosim->para->rho_start;
    cosim_log(comsg, COSIM_NORMAL);
  }
*/
  return 0;
} /* End of read_cosim_parameter()*/

	/*
		* Read the data from Modelica
		*
		* @param para Pointer to FFD parameters
		* @param var Pointer to FFD simulation variables
		* @param BINDEX pointer to boundary index
		*
		* @return 0 if no error occurred
		*/
float *read_cosim_data(CosimulationData *cosim){
	int i, j;
	int size = cosim->para->nSur + cosim->para->nPorts * 2;
	float *input = malloc(sizeof(float)*size);

	cosim_log("-------------------------------------------------------------------",
		COSIM_NORMAL);
	if (cosim->para->version == "DEBUG") {
		cosim_log("read_cosim_data(): Start to read data from Modelica.",
			COSIM_NORMAL);
	}
	/****************************************************************************
	| Wait for data to be updated by the other program
	****************************************************************************/
	while (cosim->modelica->flag == 0) {
		if (cosim->para->version == "DEBUG") {
			sprintf(comsg,
				"read_cosim_data(): Data is not ready with "
				"cosim->modelica->flag=%d",
				cosim->modelica->flag);
			cosim_log(comsg, COSIM_NORMAL);
		}

		Sleep(1000);
		if (cosim->para->version == "DEBUG")
			cosim_log("read_cosim_data(): Sleep for 1000.", COSIM_NORMAL);
	}

	if (cosim->para->version == "DEBUG") {
		cosim_log("read_cosim_data(): Modelica data is ready.", COSIM_NORMAL);
		sprintf(comsg,
			"read_cosim_data(): Received the following data at t=%f[s]",
			cosim->modelica->t);
		cosim_log(comsg, COSIM_NORMAL);
	}

	/****************************************************************************
	| Read and assign the thermal boundary conditions
	****************************************************************************/
	/*  if(assign_thermal_bc(para,var,BINDEX)!=0) {
						cosim_log("read_cosim_data(): Could not assign the Modelica thermal data to FFD",
													COSIM_ERROR);
					return 1;
			}
	*/
	/****************************************************************************
	| Read and assign the shading boundary conditions
	| Warning: This is not been used in current version
	****************************************************************************/
	/*  if(cosim->para->sha==1) {
					cosim_log("Shading control signal and adsorbed radiation by the shade:",
													COSIM_NORMAL);
					for(i=0; i<cosim->para->nConExtWin; i++) {
							sprintf(comsg, "Surface[%d]: %f,\t%f\n",
															i, cosim->modelica->shaConSig[i],
															cosim->modelica->shaAbsRad[i]);
							cosim_log(comsg, COSIM_NORMAL);
					}
			}
			else
					if(cosim->para->version=="DEBUG") {
							cosim_log("\tNo shading devices.", COSIM_NORMAL);
					}
	*/
	/****************************************************************************
	| Read and assign the inlet conditions
	****************************************************************************/
	/*  if(cosim->para->nPorts>0) {
					if(assign_port_bc(para,var,BINDEX)!=0) {
							cosim_log(" read_cosim_data(): Could not assign the Modelica inlet BC to FFD",
							COSIM_ERROR);
							return 1;
					}
			}
			else
					if(cosim->para->version=="DEBUG") {
								cosim_log("\tNo fluid ports.", COSIM_NORMAL);
					}
	*/

	/*-------------------------------------------------------------------------*/
	/* Convert the surface data from Modelica order to FFD order*/
	/*-------------------------------------------------------------------------*/
	for (j = 0; j < cosim->para->nSur; j++) {
		i = j;
		switch (cosim->para->bouCon[j]) {
		case 1: /* Temperature*/
			input[i] = cosim->modelica->temHea[j] - 273.15;
			sprintf(comsg, "\tinput[%d] was assigned with %s: T=%f[degC]",
				i, cosim->para->name[j], input[i]);
			cosim_log(comsg, COSIM_NORMAL);
			break;
		case 2: /* Heat flow rate*/
			input[i] = cosim->modelica->temHea[j];
			sprintf(comsg, "\tinput[%d] was assigned with %s: Q_dot=%f[W]",
				i, cosim->para->name[j], input[i]);
			cosim_log(comsg, COSIM_NORMAL);
			break;
		default:
			sprintf(comsg,
				"Invalid value (%d) for thermal boundary condition. "
				"Expected value are 1->Fixed T; 2->Fixed heat flux",
				cosim->para->bouCon[i]);
			cosim_log(comsg, COSIM_ERROR);
			return 1;
		}
	}

	/****************************************************************************
	| Convert the port data from Modelica to FFD for the Inlet
	****************************************************************************/
	for (j = 0; j < cosim->para->nPorts; j++) {
		i = cosim->para->nSur + j * 2;/*currently only two varaibles ,T and m, at each port*/

		/*-------------------------------------------------------------------------
		| Convert for mass flow rate and temperature
		-------------------------------------------------------------------------*/
		input[i] = cosim->modelica->mFloRatPor[j];/*mass flow rate kg/s*/
		input[i + 1] = cosim->modelica->TPor[j] - 273.15;/*temp degC*/

		sprintf(comsg, "\tinput[%d] was assigned with %s: m=%f[kg/s]",
			i, cosim->para->portName[j], input[i]);
		cosim_log(comsg, COSIM_NORMAL);

		sprintf(comsg, "\tinput[%d] was assigned with %s: T=%f[degC]",
			i + 1, cosim->para->portName[j], input[i + 1]);
		cosim_log(comsg, COSIM_NORMAL);

		/*-------------------------------------------------------------------------
		| Convert nXi types of species (currently not supported)
		-------------------------------------------------------------------------*/
		/*				for (Xid = 0; Xid<cosim->para->nXi; Xid++) {
							para->bc->XiPort[j][Xid] = cosim->modelica->XiPor[i][Xid];
							sprintf(comsg, "\tXi[%d]=%f", Xid, para->bc->XiPort[j][Xid]);
							cosim_log(comsg, COSIM_NORMAL);
						}
						/*-------------------------------------------------------------------------
						| Convert nC types of trace substances (currently not supported)
						-------------------------------------------------------------------------*/
						/*				for (Cid = 0; Cid<cosim->para->nC; Cid++) {
											para->bc->CPort[j][Cid] = cosim->modelica->CPor[i][Cid];
											sprintf(comsg, "\tC[%d]=%f", Cid, para->bc->CPort[j][Cid]);
											cosim_log(comsg, COSIM_NORMAL);
										}
									}
						*/
						/****************************************************************************
						| Post-Process after reading the data
						****************************************************************************/
						/* Change the flag to indicate that the data has been read*/
		cosim->modelica->flag = 0;
		/*printf("cosim->modelica->flag=%d\n", cosim->modelica->flag);*/
		if (cosim->para->version == "DEBUG") {
			cosim_log("read_cosim_data(): Ended reading data from Modelica.",
				COSIM_NORMAL);
		}

		return 0;
	} /* End of read_cosim_data()*/

}
	/*
		* Write the FFD data for Modelica
		*
		* @param para Pointer to FFD parameters
		* @param var Pointer to FFD simulation variables
		*
		* @return 0 if no error occurred
		*/
int write_cosim_data(CosimulationData *cosim){
  int i, j, id;

  cosim_log("-------------------------------------------------------------------",
          COSIM_NORMAL);
  if(cosim->para->version=="DEBUG") {
    cosim_log("write_cosim_parameter(): "
            "Start to write the following coupled simulation data to Modelica:",
             COSIM_NORMAL);
  }

  /****************************************************************************
  | Wait if the previous data has not been read by Modelica
  ****************************************************************************/
  while(cosim->ffd->flag==1) {
    cosim_log("write_cosim_data(): Wait since previous data is not taken "
            "by Modelica", COSIM_NORMAL);
    Sleep(1000);
  }

  /****************************************************************************
  | Start to write new data
  ****************************************************************************/
  /*cosim->ffd->t = para->mytime->t;*/

  sprintf(comsg, "write_cosim_data(): Start to write FFD data to Modelica ");
  cosim_log(comsg, COSIM_NORMAL);

  /****************************************************************************
  | Set the time and space averaged temperature of space
  | Convert T from degC to K (currently not supported)
  ****************************************************************************/
  cosim->ffd->TRoo = 25.0;/*assumed a fixed value*/
  sprintf(comsg, "\tAveraged room temperature %f[degC]", cosim->ffd->TRoo);
  cosim->ffd->TRoo += 273.15;
  cosim_log(comsg, COSIM_NORMAL);

  /****************************************************************************
  | Set temperature of shading devices (currently not supported)
  ****************************************************************************/
  if(cosim->para->sha==1) {
    cosim_log("\tTemperature of the shade:", COSIM_NORMAL);
    for(i=0; i<cosim->para->nConExtWin; i++) {
      /*Note: The shade feature is to be implemented*/
      cosim->ffd->TSha[i] = 20 + 273.15;
      sprintf(comsg, "\t\tSurface %d: %f[K]\n",
              i, cosim->ffd->TSha[i]);
      cosim_log(comsg, COSIM_NORMAL);
    }
  }
			
  /****************************************************************************
  | Set data for fluid ports
  ****************************************************************************/
  cosim_log("\tFlow information at the ports:", COSIM_NORMAL);
  for(i=0; i<cosim->para->nPorts; i++) {
    /* Get the corresponding ID in Modelica*/
			 /*d = i;*/
    /*-------------------------------------------------------------------------
    | Assign the temperature
    -------------------------------------------------------------------------*/
    cosim->ffd->TPor[i] = cosim->modelica->TPor[i];
    sprintf(comsg, "\t\t%s: TPor[%d]=%f",
            cosim->para->portName[i], i,
            cosim->ffd->TPor[i]);
    cosim_log(comsg, COSIM_NORMAL);
    /*-------------------------------------------------------------------------
    | Assign the Xi
    -------------------------------------------------------------------------*/
/*    if(cosim->para->version=="DEBUG") {
      sprintf(comsg, "\t\t\tn_Xi=%d, id=%d", para->bc->nb_Xi, id);
      cosim_log(comsg, COSIM_NORMAL);
    }*/

    for(j=0; j<cosim->para->nXi; j++) {
      /*para->bc->velPortMean[i] = fabs(para->bc->velPortMean[i]) + SMALL;
      cosim->ffd->XiPor[i][j] = para->bc->XiPortMean[i][j]
                                      / para->bc->velPortMean[i];*/
					cosim->ffd->XiPor[i][j] = 0.01;

      sprintf(comsg, "\t\t%s: Xi[%d]=%f",
              cosim->para->portName[i], j,
              cosim->ffd->XiPor[i][j]);
      cosim_log(comsg, COSIM_NORMAL);
    }
    /*-------------------------------------------------------------------------
    | Assign the C
    -------------------------------------------------------------------------*/
    for(j=0; j<cosim->para->nC; j++) {
      /*para->bc->velPortMean[i] = fabs(para->bc->velPortMean[i]) + SMALL;
      cosim->ffd->CPor[id][j] = para->bc->CPortMean[i][j]
                                    / para->bc->velPortMean[i];*/
					cosim->ffd->CPor[i][j] = 0.01;
      sprintf(comsg, "\t\t%s: C[%d]=%f",
              cosim->para->portName[i], j,
              cosim->ffd->CPor[i][j]);
      cosim_log(comsg, COSIM_NORMAL);
    }
  }

  /****************************************************************************
  | Set data for solid surfaces
  ****************************************************************************/
  cosim_log("\tInformation at solid surfaces:", COSIM_NORMAL);
		for (i = 0; i < cosim->para->nSur; i++) {

    /* Set the B.C. Temperature*/
    if(cosim->para->bouCon[i]==2) {
      /*cosim->ffd->temHea[id] = para->bc->temHeaMean[i]
                                  / para->bc->AWall[i] + 273.15;*/
					 cosim->ffd->temHea[i] = 25 + 273.15;
      sprintf(comsg, "\t\t%s: %f[K]",
              cosim->para->name[i], cosim->ffd->temHea[i]);
    }
    /* Set the heat flux*/
    else {
      /*cosim->ffd->temHea[id] = para->bc->temHeaMean[i];*/
					 cosim->ffd->temHea[i] = 0.0;
      sprintf(comsg, "\t\t%s: %f[W]",
              cosim->para->name[i], cosim->ffd->temHea[i]);
    }
    cosim_log(comsg, COSIM_NORMAL);
  }

  /****************************************************************************
  | Set data for sensors
  ****************************************************************************/
/*  if (set_sensor_data(para, var)!=0) {
    cosim_log("\tCould not get sensor data", COSIM_ERROR);
    return 1;
  }
  else
    cosim_log("\tSensor Information:", COSIM_NORMAL);*/

  for(i=0; i<cosim->para->nSen; i++) {
    /*cosim->ffd->senVal[i] = para->sens->senVal[i];*/
			cosim->ffd->senVal[i] = 25 + 273.15;
    sprintf(comsg, "\t\t%s: %f",
            cosim->para->sensorName[i], cosim->ffd->senVal[i]);
    cosim_log(comsg, COSIM_NORMAL);
  }

  /****************************************************************************
  | Inform Modelica that the FFD data is updated
  ****************************************************************************/
  cosim->ffd->flag = 1;

  return 0;
} /* End of write_cosim_data()*/


		///////////////////////////////////////////////////////////////////////////////
		/// Write the log file
		///
		///\param message Pointer the message
		///\param msg_type Type of message
		///
		///\return 0 if no error occurred
		///////////////////////////////////////////////////////////////////////////////
void cosim_log(char *message, COSIM_MSG_TYPE msg_type) {
	if (msg_type == COSIM_NEW) {
		if ((file_log = fopen("log.cosim", "w+")) == NULL) {
			fprintf(stderr, "Error:can not open error file!\n");
			exit(1);
		}
	}

	else if ((file_log = fopen("log.cosim", "a+")) == NULL) {
		fprintf(stderr, "Error:can not open error file!\n");
		exit(1);
	}

	switch (msg_type) {
	case COSIM_WARNING:
		fprintf(file_log, "WARNING in %s\n", message);
		break;
	case COSIM_ERROR:
		fprintf(file_log, "ERROR in %s\n", message);
		break;
		// Normal log
	default:
		fprintf(file_log, "%s\n", message);
	}
	fclose(file_log);

} // End of cosim_log()