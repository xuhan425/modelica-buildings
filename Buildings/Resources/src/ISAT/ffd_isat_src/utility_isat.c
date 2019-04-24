///////////////////////////////////////////////////////////////////////////////
///
/// \file   utility.c
///
/// \brief  define utility functions for isat_ffd
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
#include "utility_isat.h"


///////////////////////////////////////////////////////////////////////////////
/// Write the log file
///
///\param message Pointer the message
///\param msg_type Type ogf message
///
///\return No return needed
///////////////////////////////////////////////////////////////////////////////


void mpc_log(char *message, MPC_MSG_TYPE msg_type) {
  if(msg_type==MPC_NEW) {
    if((file_log=fopen("log.mpc","w+"))==NULL) {
        fprintf(stderr, "Error:can not open error file!\n");
        exit(1);
    }
  }
  else if((file_log=fopen("log.mpc","a+"))==NULL) {
    fprintf(stderr,"Error:can not open error file!\n");
    exit(1);
  }

  switch(msg_type) {
    case MPC_WARNING:
      fprintf(file_log, "WARNING in %s\n", message);
      break;
    case MPC_ERROR:
      fprintf(file_log, "ERROR in %s\n", message);
      break;
    // Normal log
    default:
      fprintf(file_log, "%s\n", message);
  }
  fclose(file_log);
} // End of mpc_log()

///////////////////////////////////////////////////////////////////////////////
/// Overwrite initial data according to ISAT ffdinput[]
/// 
/// Fixme: currently copy read sci data function, needs to be simplified by 
/// removing redundant codes
///
///\return 0
///////////////////////////////////////////////////////////////////////////////

extern double ffdInput[];
int overwrite_initial_data(PARA_DATA *para, REAL **var, int **BINDEX) {
	int i, id_rack = 0;

	//Assign input value
	int rewriteRack[10] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }; // the number of the elements should be equal to rack number
																																																									// 1: rewite readfile value by input; 0: not rewrite
	double rackPowerValue[10] = { 0 };
	if (ffdInput[0] == 0){
		ffdInput[0] = 1;
	}
	if (ffdInput[1] == 0) {
		ffdInput[1] = 1;
	}
	rackPowerValue[0] = ffdInput[0] * 1000;
	rackPowerValue[1] = (60000 - ffdInput[0] * 1000 * 2 - ffdInput[1] * 1000 * 2) / 6;
	rackPowerValue[2] = (60000 - ffdInput[0] * 1000 * 2 - ffdInput[1] * 1000 * 2) / 6;
	rackPowerValue[3] = (60000 - ffdInput[0] * 1000 * 2 - ffdInput[1] * 1000 * 2) / 6;
	rackPowerValue[4] = ffdInput[1] * 1000;
	rackPowerValue[5] = ffdInput[0] * 1000;
	rackPowerValue[6] = (60000 - ffdInput[0] * 1000 * 2 - ffdInput[1] * 1000 * 2) / 6;
	rackPowerValue[7] = (60000 - ffdInput[0] * 1000 * 2 - ffdInput[1] * 1000 * 2) / 6;
	rackPowerValue[8] = (60000 - ffdInput[0] * 1000 * 2 - ffdInput[1] * 1000 * 2) / 6;
	rackPowerValue[9] = ffdInput[1] * 1000;
	//printf("rackPowerValue = %f, %f, %f, %f, %f, %f, %f, %f, %f, %f\n", rackPowerValue[0], rackPowerValue[1], rackPowerValue[2], rackPowerValue[3], rackPowerValue[4], rackPowerValue[5], rackPowerValue[6], rackPowerValue[7], rackPowerValue[8], rackPowerValue[9]);
	//getchar();

	for (i = 0;i < para->bc->nb_rack;i++) {
		para->bc->RackFlowRate[id_rack] = para->bc->RackFlowRate[id_rack]/para->bc->HeatDiss[id_rack];
		//Rewrite SCI read value
		if (rewriteRack[id_rack]){
			para->bc->HeatDiss[id_rack] = rackPowerValue[id_rack];
			para->bc->RackFlowRate[id_rack] *= para->bc->HeatDiss[id_rack];
		}
			//para->bc->HeatDiss[id_rack] = 5000.0;
		//printf("id_rack: %d, %s\n", id_rack, para->bc->rackName[id_rack]);
		//printf("rackPowerValue[id_rack]: %f\n", rackPowerValue[id_rack]);
		//printf("para->bc->HeatDiss[id_rack]: %f\n", para->bc->HeatDiss[id_rack]);
		//getchar();
		id_rack += 1;
	}
	
	return 0;
} //end of overwrite_initial_data()


extern double ffdOutput[];
// Write output data to ffdoutput[]
int write_output_data(PARA_DATA *para, REAL **var, int **BINDEX) {
/*
	int imax = para->geom->imax, jmax = para->geom->jmax;
	int kmax = para->geom->kmax;
	int IMAX = imax + 2, IJMAX = (imax + 2)*(jmax + 2);
	//ffdOutput[0] = average_volume(&para, var, var[TEMPM]);
	//printf("ffdOuput=\t%f\n", ffdOutput[0]);
	//getchar();
	ffdOutput[0] = var[TEMPM][IX(imax / 2, jmax / 2, kmax / 2)];
	//printf("the ffdOutput[0] is %f\n", ffdOutput[0]);
	//getchar();
*/
	int i, j;
	int ip, jp, kp;
	REAL T1, T2, T3, T4, Tave=0, Tmax=0;
	int imax = para->geom->imax, jmax = para->geom->jmax;
	int kmax = para->geom->kmax;
	int IMAX = imax + 2, IJMAX = (imax + 2)*(jmax + 2);
	// find monitor index
	if (get_monitor_index(para, var) != 0) {
		ffd_log("write_monitor_data(): cannot find the index of monitoring cells", FFD_ERROR);
	}

	for (i = 0;i<para->bc->nb_rack;i++) {
		// P1
		ip = para->sens->indexMoniPoints[i][0][0];
		jp = para->sens->indexMoniPoints[i][0][1];
		kp = para->sens->indexMoniPoints[i][0][2];
		T1 = var[TEMP][IX(ip, jp, kp)];
		if (T1 > Tmax) {
			Tmax = T1;
		}
		// P2
		ip = para->sens->indexMoniPoints[i][1][0];
		jp = para->sens->indexMoniPoints[i][1][1];
		kp = para->sens->indexMoniPoints[i][1][2];
		T2 = var[TEMP][IX(ip, jp, kp)];
		if (T2 > Tmax) {
			Tmax = T2;
		}
		// P3
		ip = para->sens->indexMoniPoints[i][2][0];
		jp = para->sens->indexMoniPoints[i][2][1];
		kp = para->sens->indexMoniPoints[i][2][2];
		T3 = var[TEMP][IX(ip, jp, kp)];
		if (T3 > Tmax) {
			Tmax = T3;
		}
		// P4
		ip = para->sens->indexMoniPoints[i][3][0];
		jp = para->sens->indexMoniPoints[i][3][1];
		kp = para->sens->indexMoniPoints[i][3][2];
		T4 = var[TEMP][IX(ip, jp, kp)];
		if (T4 > Tmax) {
			Tmax = T4;
		}
		// calculate average rack inlet temp
		//Tave = (T1 + T2 + T3 + T4) / 4;
		//printf("rack no. %d, T= %f\n", i, Tmax);
		//getchar();
		//printf("Tave: %f\n", Tave);
		// update maximum rack inlet temp
		//if (Tave > Tmax) {
		//	Tmax = Tave;	
		//}
	}

	ffdOutput[0] = Tmax; //output the maximum rack inlet temp
	//printf("ffdOutput[0]: %f\n", ffdOutput[0]);
	//getchar();

	return 0;
} // set_overwrite_data()