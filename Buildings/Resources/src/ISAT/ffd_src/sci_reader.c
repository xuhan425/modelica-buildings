///////////////////////////////////////////////////////////////////////////////
///
/// \file   sci_reader.c
///
/// \brief  Read mesh and simulation data defined by SCI
///
/// \author Wangda Zuo
///         University of Miami
///         W.Zuo@miami.edu
///         Mingang Jin, Qingyan Chen
///         Purdue University
///         Jin55@purdue.edu, YanChen@purdue.edu
///         Wei Tian
///         University of Miami, Schneider Electric
///         w.tian@umiami.edu, Wei.Tian@Schneider-Electric.com
///
/// \date   6/15/2017
///
///////////////////////////////////////////////////////////////////////////////

#include "sci_reader.h"
//#include <Python.h>

///////////////////////////////////////////////////////////////////////////////
/// check the input forat as current FFD accepts a format excluding unused info
///
///\return 0 if no error occurred
///////////////////////////////////////////////////////////////////////////////
/*
int check_input_format() {
  // Embed a python function in C to perform the edit for a file
  // Inspired by online resource: https://stackoverflow.com/questions/12142174/run-a-python-script-with-arguments
  // Check my Wiki Page: https://github.com/se-thermalanalytics/DC-DoE-CU-LBL-SE/wiki/Embed-python-script-in-C-codes
  FILE* file;
  int argc;
  char * argv[1];

  argc = 1;
  argv[0] = "check_input.py";

  // set up a python environment
  Py_SetProgramName(argv[0]);
  Py_Initialize();
  PySys_SetArgv(argc, argv);
  file = fopen(argv[0],"r");
  PyRun_SimpleFile(file, argv[0]);
  Py_Finalize();

  return 0 ;
}
*/
///////////////////////////////////////////////////////////////////////////////
/// Read the basic index information from input.cfd
///
/// Specific method for advection will be selected according to the variable
/// type.
///
///\param para Pointer to FFD parameters
///\param var Pointer to FFD simulation variables
///
///\return 0 if no error occurred
///////////////////////////////////////////////////////////////////////////////
int read_sci_max(PARA_DATA *para, REAL **var) {
  char string[400];

  // Open the file
  if((file_params=fopen(para->inpu->parameter_file_name,"r")) == NULL) {
    fprintf(stderr,"Error:can not open the file \"%s\".\n",
      para->inpu->parameter_file_name);
    return 1;
  }

  // Get the first line for the length in X, Y and Z directions
  fgets(string, 400, file_params);
  if (ifDouble) {
    sscanf(string, "%lf %lf %lf", &para->geom->Lx, &para->geom->Ly, &para->geom->Lz);
  }
  else {
    sscanf(string, "%f %f %f", &para->geom->Lx, &para->geom->Ly, &para->geom->Lz);
  }


  // Get the second line for the number of cells in X, Y and Z directions
  fgets(string, 400, file_params);
  sscanf(string,"%d %d %d", &para->geom->imax, &para->geom->jmax,
    &para->geom->kmax);

  fclose(file_params);
  return 0;
} // End of read_sci_max()

///////////////////////////////////////////////////////////////////////////////
/// Check the number of racks in the input file
///
///\param para Pointer to FFD parameters
///\param var Pointer to FFD simulation variables
///\param BINDEX Pointer to boundary index
///
///\return 0 if no error occurred
///////////////////////////////////////////////////////////////////////////////
int check_num_racks(PARA_DATA *para, REAL **var, int **BINDEX) {
  char string[400];
  int tmp = 0;

  // Open the file
  if((file_params=fopen(para->inpu->parameter_file_name,"r")) == NULL) {
    fprintf(stderr,"Error:can not open the file \"%s\".\n",
      para->inpu->parameter_file_name);
    return 1;
  }
  // Read line by line
  while(fgets(string, 400, file_params) != NULL) {
    // if Rack is in the line, then count it
    if (strstr(string, "Rack") != NULL) tmp += 1;
  }
  // close the file
  fclose(file_params);

  // write the number of rack to global parameter
  para->bc->nb_rack = tmp;

  // return 0
  return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// Check the number of tiles in the input file
///
///\param para Pointer to FFD parameters
///\param var Pointer to FFD simulation variables
///\param BINDEX Pointer to boundary index
///
///\return 0 if no error occurred
///////////////////////////////////////////////////////////////////////////////
int check_num_tiles(PARA_DATA *para, REAL **var, int **BINDEX) {
  char string[400];
  int tmp = 0;

  // Open the file
  if((file_params=fopen(para->inpu->parameter_file_name,"r")) == NULL) {
    fprintf(stderr,"Error:can not open the file \"%s\".\n",
      para->inpu->parameter_file_name);
    return 1;
  }
  // Read line by line
  while(fgets(string, 400, file_params) != NULL) {
    // if Rack is in the line, then count it
    if (strstr(string, "Tile") != NULL) tmp += 1;
  }
  // close the file
  fclose(file_params);

  // return 0
  return tmp;
}

///////////////////////////////////////////////////////////////////////////////
/// Read other information from sci input file
///
///\param para Pointer to FFD parameters
///\param var Pointer to FFD simulation variables
///\param var_type Type of variable
///\param BINDEX Pointer to boundary index
///
///\return 0 if no error occurred
///////////////////////////////////////////////////////////////////////////////
//FIXME: THIS FUNCTION IS TOO LENGTHT AND DIFFICULT TO READ AND MAINTAIN---WEITIAN
int read_sci_input(PARA_DATA *para, REAL **var, int **BINDEX) {
  int i, j, k;
  int ii,ij,ik;
  REAL tempx, tempy, tempz;
  REAL Lx = para->geom->Lx;
  REAL Ly = para->geom->Ly;
  REAL Lz = para->geom->Lz;
  REAL *gx = var[GX], *gy = var[GY], *gz = var[GZ];
  REAL *x = var[X], *y = var[Y], *z = var[Z];
  //int IWWALL,IEWALL,ISWALL,INWALL,IBWALL,ITWALL;
  int SI,SJ,SK,EI,EJ,EK,FLTMP;
  REAL TMP,MASS,U,V,W;
  char name[100];
  int imax = para->geom->imax;
  int jmax = para->geom->jmax;
  int kmax = para->geom->kmax;
  int index=0;
  int IMAX = imax+2, IJMAX = (imax+2)*(jmax+2);
  char string[400];
  REAL *delx, *dely, *delz;
  REAL *flagp = var[FLAGP];
  int bcnameid = -1;
  int tile_or_outlet = 2;
  char tile_name_tmp[1000]; // temporarily store the tile name
  char rack_name_tmp[1000]; // temporarily store the rack name
  char curtain_name_tmp[1000]; //temporarily store the curtain name
  REAL curtain_open_ratio = 0.0;  //temporarily store the curtain opening ratio
  REAL tile_opening = 1.0;
  int id_rack = 0; // the unique id for rack
  REAL momentum_kick = 0.0; // momentum force
  REAL h_momentum = 0.1524; // 0.1524 equals to 6 inches
  REAL beta_tmp = 0.0; // openting ratio of perforated tile

  // Open the parameter file
  if((file_params=fopen(para->inpu->parameter_file_name,"r")) == NULL ) {
    sprintf(msg,"read_sci_input(): Could not open the file \"%s\".",
            para->inpu->parameter_file_name);
    ffd_log(msg, FFD_ERROR);
    return 1;
  }

  sprintf(msg, "read_sci_input(): Start to read sci input file %s",
          para->inpu->parameter_file_name);
  ffd_log(msg, FFD_NORMAL);

  // Ignore the first and second lines
  fgets(string, 400, file_params);
  fgets(string, 400, file_params);

  /*****************************************************************************
  | Convert the cell dimensions defined by SCI to coordinates in FFD
  *****************************************************************************/
  // Allocate temporary memory for dimension of each cell
  delx = (REAL *) malloc ((imax+2)*sizeof(REAL));
  dely = (REAL *) malloc ((jmax+2)*sizeof(REAL));
  delz = (REAL *) malloc ((kmax+2)*sizeof(REAL));

  if( !delx || !dely ||!delz ) {
    ffd_log("read_sci_input(): Cannot allocate memory for delx, dely or delz.",
            FFD_ERROR);
    return 1;
  }

  delx[0]=0;
  dely[0]=0;
  delz[0]=0;

  // Read cell dimensions in X, Y, Z directions
  if (ifDouble) {
    for (i = 1; i <= imax; i++) fscanf(file_params, "%lf", &delx[i]);
    fscanf(file_params, "\n");
    for (j = 1; j <= jmax; j++) fscanf(file_params, "%lf", &dely[j]);
    fscanf(file_params, "\n");
    for (k = 1; k <= kmax; k++) fscanf(file_params, "%lf", &delz[k]);
    fscanf(file_params, "\n");
  }
  else {
    for (i = 1; i <= imax; i++) fscanf(file_params, "%f", &delx[i]);
    fscanf(file_params, "\n");
    for (j = 1; j <= jmax; j++) fscanf(file_params, "%f", &dely[j]);
    fscanf(file_params, "\n");
    for (k = 1; k <= kmax; k++) fscanf(file_params, "%f", &delz[k]);
    fscanf(file_params, "\n");
  }

  // Store the locations of grid cell surfaces
  tempx = 0.0; tempy = 0.0; tempz = 0.0;
  for(i=0; i<=imax+1; i++) {
    tempx += delx[i];
    if(i>=imax) tempx = Lx;
    for(j=0; j<=jmax+1; j++)
      for(k=0; k<=kmax+1; k++) var[GX][IX(i,j,k)]=tempx;
  }

  for(j=0; j<=jmax+1; j++) {
    tempy += dely[j];
    if(j>=jmax) tempy = Ly;
    for(i=0; i<=imax+1; i++)
      for(k=0; k<=kmax+1; k++) var[GY][IX(i,j,k)] = tempy;
  }

  for(k=0; k<=kmax+1; k++) {
    tempz += delz[k];
    if(k>=kmax) tempz = Lz;
    for(i=0; i<=imax+1; i++)
      for(j=0; j<=jmax+1; j++) var[GZ][IX(i,j,k)] = tempz;
  }

  /*****************************************************************************
  | Convert the coordinates for cell surfaces to
  | the coordinates for the cell center
  *****************************************************************************/
  FOR_ALL_CELL
    if(i<1)
      x[IX(i,j,k)] = 0;
    else if(i>imax)
      x[IX(i,j,k)] = Lx;
    else
      x[IX(i,j,k)] = (REAL) 0.5 * (gx[IX(i,j,k)]+gx[IX(i-1,j,k)]);

    if(j<1)
      y[IX(i,j,k)] = 0;
    else if(j>jmax)
      y[IX(i,j,k)] = Ly;
    else
      y[IX(i,j,k)] = (REAL) 0.5 * (gy[IX(i,j,k)]+gy[IX(i,j-1,k)]);

    if(k<1)
      z[IX(i,j,k)] = 0;
    else if(k>kmax)
      z[IX(i,j,k)] = Lz;
    else
      z[IX(i,j,k)] = (REAL) 0.5 * (gz[IX(i,j,k)]+gz[IX(i,j,k-1)]);
  END_FOR

  // Get the wall property
  //fgets(string, 400, file_params);
  //sscanf(string,"%d%d%d%d%d%d", &IWWALL, &IEWALL, &ISWALL,
         //&INWALL, &IBWALL, &ITWALL);

  /*****************************************************************************
  | Read total number of boundary conditions
  *****************************************************************************/
  fgets(string, 400, file_params);
  sscanf(string,"%d", &para->bc->nb_bc);
  sprintf(msg, "read_sci_input(): para->bc->nb_bc=%d", para->bc->nb_bc);
  ffd_log(msg, FFD_NORMAL);

  /*****************************************************************************
  | Read the inlet boundary conditions
  *****************************************************************************/
  // Get number of inlet boundaries
  fgets(string, 400, file_params);
  sscanf(string,"%d", &para->bc->nb_inlet);
  sprintf(msg, "read_sci_input(): para->bc->nb_inlet=%d", para->bc->nb_inlet);
  ffd_log(msg, FFD_NORMAL);

  index=0;
  // Set inlet boundary
  if(para->bc->nb_inlet!=0) {
    /*-------------------------------------------------------------------------
    | Allocate the memory for bc name
    -------------------------------------------------------------------------*/
    para->bc->inletName = (char**) malloc(para->bc->nb_inlet*sizeof(char*));
    if(para->bc->inletName==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for "
              "para->bc->inletName.", FFD_ERROR);
      return 1;
    }

    /*-------------------------------------------------------------------------
    | Loop for each inlet boundary
    --------------------------------------------------------------------------*/
    for(i=0; i<para->bc->nb_inlet; i++) {
      /*.......................................................................
      | Get the names of boundary
      .......................................................................*/
      fgets(string, 400, file_params);
      // Get the length of name (The name may contain white space)
      for(j=0; string[j] != '\n'; j++) {
        continue;
      }

      para->bc->inletName[i] = (char*)malloc((j+1)*sizeof(char));
      if(para->bc->inletName[i]==NULL) {
        sprintf(msg, "read_sci_input(): Could not allocate memory for "
                "para->bc->inletName[%d].", i);
        ffd_log(msg, FFD_ERROR);
        return 1;
      }

      strncpy(para->bc->inletName[i], (const char*)string, j);
      // Add an ending
      para->bc->inletName[i][j] = '\0';
      sprintf(msg, "read_sci_input(): para->bc->inletName[%d]=%s",
              i, para->bc->inletName[i]);
      ffd_log(msg, FFD_NORMAL);
      /*.......................................................................
      | Get the boundary conditions
      .......................................................................*/
      fgets(string, 400, file_params);
      if (ifDouble) {
        sscanf(string, "%d%d%d%d%d%d%lf%lf%lf%lf%lf", &SI, &SJ, &SK, &EI,
          &EJ, &EK, &TMP, &MASS, &U, &V, &W);
      }
      else {
        sscanf(string, "%d%d%d%d%d%d%f%f%f%f%f", &SI, &SJ, &SK, &EI,
          &EJ, &EK, &TMP, &MASS, &U, &V, &W);
      }
      sprintf(msg, "read_sci_input(): VX=%f, VY=%f, VZ=%f, T=%f, Xi=%f",
              U, V, W, TMP, MASS);
      ffd_log(msg, FFD_NORMAL);

      /*.......................................................................
      | Assign the boundary conditions to cells
      .......................................................................*/      
      if(EI==0) {
        if(SI==1) SI = 0;
        EI = SI + EI;
        EJ = SJ + EJ - 1;
        EK = SK + EK - 1;
      }

      if(EJ==0) {
        if(SJ==1) SJ = 0;
        EI = SI + EI - 1;
        EJ = SJ + EJ;
        EK = SK + EK - 1;
      }

      if(EK==0) {
        if(SK==1) SK = 0;
        EI = SI + EI - 1;
        EJ = SJ + EJ - 1;
        EK = SK + EK;
      }
      /*.......................................................................
      | If it is a perforated tile, then calculate the momentum kick
      | 'Jet' is the key word
      | Suppose the height of momentum height is 6 inch
      .......................................................................*/
      if (strstr(para->bc->inletName[i], "Jet") != NULL) {
        // find beta
        if (ifDouble) {
            sscanf(para->bc->inletName[i], "%s%lf", tile_name_tmp, &beta_tmp);
        }
        else {
            sscanf(para->bc->inletName[i], "%s%f", tile_name_tmp, &beta_tmp);
        }
        // find momentum and source term
        for(ii=SI; ii<=EI; ii++){
          for(ij=SJ; ij<=EJ; ij++){
            for(ik=SK; ik<=EK; ik++) {
              if (SI == EI) {
                momentum_kick = get_momentum_kick(fabs(U),beta_tmp,h_momentum);
                var[VXS][IX(ii+1,ij,ik)] = momentum_kick*vol(para,var,ii+1,ij,ik);
              }
              else if (SJ == EJ){
                momentum_kick = get_momentum_kick(fabs(V),beta_tmp,h_momentum);
                var[VYS][IX(ii,ij+1,ik)] = momentum_kick*vol(para,var,ii,ij+1,ik);
              }
              else if (SK == EK){
                momentum_kick = get_momentum_kick(fabs(W),beta_tmp,h_momentum);
                var[VZS][IX(ii,ij,ik+1)] = momentum_kick*vol(para,var,ii,ij,ik+1);
              }
            }
          }
        }
      }
      // Assign the inlet boundary condition for each cell
      for(ii=SI; ii<=EI; ii++){
        for(ij=SJ; ij<=EJ; ij++){
          for(ik=SK; ik<=EK; ik++) {
            BINDEX[0][index] = ii;
            BINDEX[1][index] = ij;
            BINDEX[2][index] = ik;
            BINDEX[4][index] = i;
            index++;

            var[TEMPBC][IX(ii,ij,ik)] = TMP;
            var[VXBC][IX(ii,ij,ik)] = U;
            var[VYBC][IX(ii,ij,ik)] = V;
            var[VZBC][IX(ii,ij,ik)] = W;
            var[Xi1BC][IX(ii,ij,ik)] = MASS;

            flagp[IX(ii,ij,ik)] = INLET; // Cell flag to be inlet
            if(para->outp->version==DEBUG) {
              sprintf(msg, "read_sci_input(): get inlet cell[%d,%d,%d]=%.1f",
                ii, ij, ik, flagp[IX(ii,ij,ik)]);
              ffd_log(msg, FFD_NORMAL);
            }
          } // End of assigning the inlet B.C. for each cell
        }
      }
    } // End of loop for each inlet boundary
  } // End of setting inlet boundary

  /*****************************************************************************
  | Read the outlet boundary conditions
  *****************************************************************************/
  fgets(string, 400, file_params);
  sscanf(string, "%d", &para->bc->nb_outlet);
  sprintf(msg, "read_sci_input(): para->bc->nb_outlet=%d", para->bc->nb_outlet);
  ffd_log(msg, FFD_NORMAL);

  if(para->bc->nb_outlet!=0) {
    para->bc->outletName = (char**) malloc(para->bc->nb_outlet*sizeof(char*));
    if(para->bc->outletName==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for "
              "para->bc->outletName.", FFD_ERROR);
      return 1;
    }

    for(i=0; i<para->bc->nb_outlet; i++) {
      /*.......................................................................
      | Get the names of boundary
      .......................................................................*/
      fgets(string, 400, file_params);
      // Get the length of name (The name may contain white space)
      for(j=0; string[j] != '\n'; j++) {
        continue;
      }

      para->bc->outletName[i] = (char*)malloc((j+1)*sizeof(char));
      if(para->bc->outletName[i]==NULL) {
        sprintf(msg, "read_sci_input(): Could not allocate memory "
          "for para->bc->outletName[%d].", i);
        ffd_log(msg, FFD_ERROR);
        return 1;
      }

      strncpy(para->bc->outletName[i], (const char*)string, j);
      // Add an ending
      para->bc->outletName[i][j] = '\0';
      sprintf(msg, "read_sci_input(): para->bc->outletName[%d]=%s",
              i, para->bc->outletName[i]);
      ffd_log(msg, FFD_NORMAL);

      /*.......................................................................
      | Get the boundary conditions
      .......................................................................*/
      fgets(string, 400, file_params);
      if (ifDouble) {
        sscanf(string, "%d%d%d%d%d%d%lf%lf%lf%lf%lf",
          &SI, &SJ, &SK, &EI,
          &EJ, &EK, &TMP, &MASS, &U, &V, &W);
      }
      else {
        sscanf(string, "%d%d%d%d%d%d%f%f%f%f%f",
          &SI, &SJ, &SK, &EI,
          &EJ, &EK, &TMP, &MASS, &U, &V, &W);
      }

      sprintf(msg, "read_sci_input(): VX=%f, VY=%f, VX=%f, T=%f, Xi=%f",
              U, V, W, TMP, MASS);
      ffd_log(msg, FFD_NORMAL);

      if(EI==0) {
        if(SI==1) SI=0;
        EI = SI + EI;
        EJ = SJ + EJ - 1;
        EK = SK + EK - 1;
      }

      if(EJ==0) {
        if(SJ==1) SJ=0;
        EI = SI+EI-1;
        EJ = SJ+EJ;
        EK = SK+EK-1;
      }

      if(EK==0) {
        if(SK==1) SK = 0;
        EI = SI+EI-1;
        EJ = SJ+EJ-1;
        EK = SK+EK;
      }
      // Determine to assgin OUTLET or TILE
      if (strstr(para->bc->outletName[i], "Tile") != NULL) {
          tile_or_outlet = TILE;
          // read the direction tiles are put
          if (SI == EI)
            para->geom->tile_putX = 1;
          else if (SJ == EJ)
            para->geom->tile_putY = 1;
          else if (SK == EK)
            para->geom->tile_putZ = 1;
          //printf("putX, putY, putZ is %d, %d, %d\n",para->geom->tile_putX,para->geom->tile_putY,para->geom->tile_putZ);
          //getchar();
      }
      else {
          tile_or_outlet = OUTLET;
      }

      // Assign the outlet boundary condition for each cell
      for(ii=SI; ii<=EI ;ii++){
        for(ij=SJ; ij<=EJ ;ij++){
          for(ik=SK; ik<=EK; ik++) {
            // If cell hasn't been updated (default value -1)
            // this can avoid overwriting of previous bc assignment
            if(flagp[IX(ii,ij,ik)]<0) {
              BINDEX[0][index] = ii;
              BINDEX[1][index] = ij;
              BINDEX[2][index] = ik;
              BINDEX[4][index] = para->bc->nb_inlet + i;
              index++;

              // Give the initial value, but the value will be overwritten later
              var[TEMPBC][IX(ii,ij,ik)] = TMP;
              var[VXBC][IX(ii,ij,ik)] = U;
              var[VYBC][IX(ii,ij,ik)] = V;
              var[VZBC][IX(ii,ij,ik)] = W;
              var[Xi1BC][IX(ii,ij,ik)] = MASS;
              flagp[IX(ii,ij,ik)] = tile_or_outlet;

              // if it is tile, then record the opening ratio, 25%, 40%, or 56%
              // and its corresponding resistance
              if (tile_or_outlet == TILE) {
                // read name
                if (ifDouble) {
                    sscanf(para->bc->outletName[i], "%s%lf", tile_name_tmp, &var[TILE_OPEN_BC][IX(ii, ij, ik)]);
                }
                else {
                    sscanf(para->bc->outletName[i], "%s%f", tile_name_tmp, &var[TILE_OPEN_BC][IX(ii, ij, ik)]);
                }

                // read the resistance parameter
                tile_opening = var[TILE_OPEN_BC][IX(ii, ij, ik)];
                //printf("is opening is %f\n", var[TILE_OPEN_BC][IX(ii, ij, ik)]);

                // Calculate the resistance based on the paper
                // @inproceedings{vangilder2015development,
                //		title = { Development of a Raised - Floor Plenum Design Tool },
                //		author = { VanGilder, James W and Zhang, Xuanhang Simon },
                //		year = { 2015 },
                //		organization = { American Society of Mechanical Engineers }
                //		}
                // SHOULD MULTIPLE BY A 0.5 AT THE END, ACCORDINTG TO THE EQUATIONS.
                var[TILE_RESI_BC][IX(ii, ij, ik)] = 1/pow(tile_opening,2)*(1.0 + 0.5*pow(1- tile_opening, 0.75)+1.414*pow(1- tile_opening, 0.375))*0.5;
              }
            }
            // output debug information
            if(para->outp->version==DEBUG) {
              sprintf(msg, "read_sci_input(): get outlet cell[%d,%d,%d]=%.1f",
                ii, ij, ik, flagp[IX(ii,ij,ik)]);
              ffd_log(msg, FFD_NORMAL);
            }
          } // End of assigning the outlet B.C. for each cell
        }
      }
    } // End of loop for each outlet boundary
  } // End of setting outlet boundary

  /*****************************************************************************
  | - Copy the inlet and outlet information to ports
  | - Allocate memory for related port variables
  *****************************************************************************/
  para->bc->nb_port = para->bc->nb_inlet+para->bc->nb_outlet;

  if(para->bc->nb_port>0) {
    // Allocate memory for the array of ports' names
    para->bc->portName = (char**) malloc(para->bc->nb_port*sizeof(char*));
    if(para->bc->portName==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for para->bc->portName.",
      FFD_ERROR);
      return 1;
    }
    /*--------------------------------------------------------------------------
    | Copy the inlet names to ports' names
    --------------------------------------------------------------------------*/
    for(i=0; i<para->bc->nb_inlet; i++) {
      // Allocate memory for inlet name
      para->bc->portName[i] =
        (char*) malloc(sizeof(char)*(strlen(para->bc->inletName[i])+1));

      if(para->bc->portName[i]==NULL) {
        ffd_log("read_sci_input():"
                "Could not allocate memory for para->bc->portName.",
        FFD_ERROR);
        return 1;
      }

      // Copy the inlet name
      strcpy(para->bc->portName[i], para->bc->inletName[i]);
      sprintf(msg, "read_sci_input(): Port[%d]:%s",
              i, para->bc->portName[i]);
      ffd_log(msg, FFD_NORMAL);
    }

    /*--------------------------------------------------------------------------
    | Copy the outlet names to ports' names
    --------------------------------------------------------------------------*/
    j = para->bc->nb_inlet;
    for(i=0; i<para->bc->nb_outlet; i++) {
      // Allocate memory for outlet name
      para->bc->portName[i+j] =
        (char*) malloc(sizeof(char)*(strlen(para->bc->outletName[i])+1));
      if(para->bc->portName[i+j]==NULL) {
        ffd_log("read_sci_input(): "
                "Could not allocate memory for para->bc->portName.",
        FFD_ERROR);
        return 1;
      }
      else {
        strcpy(para->bc->portName[i+j], para->bc->outletName[i]);
        sprintf(msg, "read_sci_input(): Port[%d]:%s",
                i+j, para->bc->portName[i+j]);
        ffd_log(msg, FFD_NORMAL);
      }
    }
    /*--------------------------------------------------------------------------
    | Allocate memory for the surface area
    --------------------------------------------------------------------------*/
    para->bc->APort = (REAL*) malloc(para->bc->nb_port*sizeof(REAL));
    if(para->bc->APort==NULL) {
      ffd_log("read_sci_input(): "
              "Could not allocate memory for para->bc->APort.",
      FFD_ERROR);
      return 1;
    }
    /*--------------------------------------------------------------------------
    | Allocate memory for the velocity (used for inlet only)
    --------------------------------------------------------------------------*/
    para->bc->velPort = (REAL*) malloc(para->bc->nb_port*sizeof(REAL));
    if(para->bc->velPort==NULL) {
      ffd_log("read_sci_input(): "
              "Could not allocate memory for para->bc->velPort.",
      FFD_ERROR);
      return 1;
    }
    /*--------------------------------------------------------------------------
    | Allocate memory for the averaged velocity
    --------------------------------------------------------------------------*/
    para->bc->velPortAve = (REAL*) malloc(para->bc->nb_port*sizeof(REAL));
    if(para->bc->velPortAve==NULL) {
      ffd_log("read_sci_input(): "
              "Could not allocate memory for para->bc->velAve.",
      FFD_ERROR);
      return 1;
    }
    /*--------------------------------------------------------------------------
    | Allocate memory for mean velocity
    --------------------------------------------------------------------------*/
    para->bc->velPortMean = (REAL*) malloc(para->bc->nb_port*sizeof(REAL));
    if(para->bc->velPortMean==NULL) {
      ffd_log("read_sci_input(): "
              "Could not allocate memory for para->bc->velPortMean.",
      FFD_ERROR);
      return 1;
    }

    /*--------------------------------------------------------------------------
    | Allocate memory for volumetric flow rate of tiles
    --------------------------------------------------------------------------*/
    para->bc->QPort = (REAL*)malloc(para->bc->nb_port * sizeof(REAL));
    if (para->bc->QPort == NULL) {
        ffd_log("read_sci_input(): "
            "Could not allocate memory for para->bc->QPort.",
            FFD_ERROR);
        return 1;
    }

    /*--------------------------------------------------------------------------
    | Allocate memory for the temperature (used for inlet only)
    --------------------------------------------------------------------------*/
    para->bc->TPort = (REAL*) malloc(para->bc->nb_port*sizeof(REAL));
    if(para->bc->TPort==NULL) {
      ffd_log("read_sci_input(): "
              "Could not allocate memory for para->bc->TPort.",
      FFD_ERROR);
      return 1;
    }
    /*--------------------------------------------------------------------------
    | Allocate memory for the averaged temperature
    --------------------------------------------------------------------------*/
    para->bc->TPortAve = (REAL*) malloc(para->bc->nb_port*sizeof(REAL));
    if(para->bc->TPortAve==NULL) {
      ffd_log("read_sci_input(): "
              "Could not allocate memory for para->bc->TPortAve.",
      FFD_ERROR);
      return 1;
    }
    /*--------------------------------------------------------------------------
    | Allocate memory for the mean velocity
    --------------------------------------------------------------------------*/
    para->bc->TPortMean = (REAL*) malloc(para->bc->nb_port*sizeof(REAL));
    if(para->bc->TPortMean==NULL) {
      ffd_log("read_sci_input(): "
              "Could not allocate memory for para->bc->TPortMean.",
      FFD_ERROR);
      return 1;
    }
    /*--------------------------------------------------------------------------
    | Allocate memory for port ID
    --------------------------------------------------------------------------*/
    para->bc->portId = (int*) malloc(para->bc->nb_port*sizeof(int));
    if(para->bc->portId==NULL) {
      ffd_log("read_sci_input(): "
              "Could not allocate memory for para->bc->portId.",
      FFD_ERROR);
      return 1;
    }
  }

  /*****************************************************************************
  | Read the internal solid block boundary conditions
  *****************************************************************************/
  fgets(string, 400, file_params);
  sscanf(string, "%d", &para->bc->nb_block);
  sprintf(msg, "read_sci_input(): para->bc->nb_block=%d", para->bc->nb_block);
  ffd_log(msg, FFD_NORMAL);
  // initialize the variables associated with rack modeling
  // allocate memory for cooling airflow direction for racks, and mapping of outlet to inlet cells in rack
  if (para->bc->nb_rack != 0) {
    // allocate memory for rack names
    para->bc->rackName = (char **)malloc(para->bc->nb_rack*sizeof(char*));
    if(para->bc->rackName==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for para->bc->rackName.",
      FFD_ERROR);
      return 1;
    }
    // allocate memory for the flow rates of rack (CFM/kW)
    para->bc->RackFlowRate = (REAL *) malloc(para->bc->nb_rack*sizeof(REAL));
    if(para->bc->RackFlowRate==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for para->bc->RackFlowRate.",
      FFD_ERROR);
      return 1;
    }
    // set default value of 125 CFM/kW--->m3/s/w. 1 CFM = 0.00047194745 M3/s
    for (i=0; i<para->bc->nb_rack;i++) {
      para->bc->RackFlowRate[i] = 125*0.00047194745/1000;
    }

    // allocate memory for the heat dissipation of rack in W
    para->bc->HeatDiss = (REAL *) malloc(para->bc->nb_rack*sizeof(REAL));
    if(para->bc->HeatDiss==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for para->bc->HeatDiss.",
      FFD_ERROR);
      return 1;
    }
    // set default value of heat dissipation of a rack as 5000 W (5kW)
    for (i=0; i<para->bc->nb_rack;i++) {
      para->bc->HeatDiss[i] = 5000;
    }
    // allocate memory for inlet or outlet area of rack
    para->bc->RackArea = (REAL *) malloc(para->bc->nb_rack*sizeof(REAL));
    if(para->bc->RackArea==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for para->bc->RackArea.",
      FFD_ERROR);
      return 1;
    }
    // set default value of inlet or outlet area of rack as 0
    for (i=0; i<para->bc->nb_rack;i++) {
      para->bc->RackArea[i] = 0.0;
    }
    // allocate memory for the flow direction of rack
    para->bc->RackDir = (int *) malloc(para->bc->nb_rack*sizeof(int));
    if(para->bc->RackDir==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for para->bc->RackDir.",
      FFD_ERROR);
      return 1;
    }
    // set the default direction of flow in rack, to the X
    for (i=0; i<para->bc->nb_rack;i++) {
     para->bc->RackDir[i] = 1;
    }

    // allocate memory for mapping of outlet cells to the inlets cells for each rack
    para->bc->RackMap = (int **) malloc(para->bc->nb_rack*sizeof(int *));
    if(para->bc->RackMap==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for para->bc->RackMap.",
      FFD_ERROR);
      return 1;
    }
    for (i=0; i<para->bc->nb_rack;i++) {
      // allocate memory for para->bc->RackMap[i]
      para->bc->RackMap[i] = (int *) malloc(3*sizeof(int));
      if(para->bc->RackMap[i]==NULL) {
        ffd_log("read_sci_input(): Could not allocate memory for para->bc->RackMap[i].",
        FFD_ERROR);
        return 1;
      }
      // set initial value of 0 for para->bc->RackMap[i]
      para->bc->RackMap[i][0] = 0;
      para->bc->RackMap[i][1] = 0;
      para->bc->RackMap[i][2] = 0;
    } //end of for (i=0; i<para->bc->nb_rack;i++)

    // allocate memory for coordinates of the monitoring points in front racks
    para->sens->coordMoniPoints = (REAL ***)malloc(para->bc->nb_rack*sizeof(REAL **));
    if(para->sens->coordMoniPoints==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for para->sens->coordMoniPoints.",
      FFD_ERROR);
      return 1;
    }
    // initialize coordinates of the monitoring points in front racks
    for (i=0;i<para->bc->nb_rack;i++) {
      para->sens->coordMoniPoints[i] = (REAL **)malloc(RACK_MONITOR_POINTS*sizeof(REAL *));
      if (para->sens->coordMoniPoints[i]!=NULL){
        for (j=0;j<RACK_MONITOR_POINTS;j++){
          para->sens->coordMoniPoints[i][j] = (REAL *)malloc(3*sizeof(REAL));
          if ( para->sens->coordMoniPoints[i][j]!=NULL){
            for (k=0;k<3;k++){
              para->sens->coordMoniPoints[i][j][k] = 0.0;
            }
          }
        }
      }
    }
    // allocate memory for indexes of the monitoring points in front racks
    para->sens->indexMoniPoints = (int ***)malloc(para->bc->nb_rack*sizeof(int **));
    if(para->sens->indexMoniPoints==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for para->sens->indexMoniPoints.",
      FFD_ERROR);
      return 1;
    }
    // initialize coordinates of the monitoring points in front racks
    for (i=0;i<para->bc->nb_rack;i++) {
      para->sens->indexMoniPoints[i] = (int **)malloc(RACK_MONITOR_POINTS*sizeof(int *));
      if (para->sens->indexMoniPoints[i]!=NULL){
        for (j=0;j<RACK_MONITOR_POINTS;j++){
          para->sens->indexMoniPoints[i][j] = (int *)malloc(3*sizeof(int));
          if (para->sens->indexMoniPoints[i][j]!=NULL){
            for (k=0;k<3;k++){
              para->sens->indexMoniPoints[i][j][k] = 0;
            }
          }
        }
      }
    }
  } //end of if (para->bc->nb_rack != 0)

  // read data of all blocks and racks
  if(para->bc->nb_block!=0) {
    para->bc->blockName = (char**) malloc(para->bc->nb_block*sizeof(char*));
    if(para->bc->blockName==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for para->bc->blockName.",
      FFD_ERROR);
      return 1;
    }

    for(i=0; i<para->bc->nb_block; i++) {
      /*.......................................................................
      | Get the names of boundary
      .......................................................................*/
      fgets(string, 400, file_params);
      // Get the length of name (The name may contain white space)
      for(j=0; string[j] != '\n'; j++) {
        continue;
      }

      para->bc->blockName[i] = (char*)malloc((j+1)*sizeof(char));
      if(para->bc->blockName[i]==NULL) {
        sprintf(msg,"read_sci_input(): Could not allocate memory for "
          "para->bc->blockName[%d].", i);
        ffd_log(msg, FFD_ERROR);
        return 1;
      }

      strncpy(para->bc->blockName[i], (const char*)string, j);
      // Add an ending
      para->bc->blockName[i][j] = '\0';
      sprintf(msg, "read_sci_input(): para->bc->blockName[%d]=%s",
              i, para->bc->blockName[i]);
      ffd_log(msg, FFD_NORMAL);

      /*.......................................................................
      | Get the boundary conditions
      .......................................................................*/
      fgets(string, 400, file_params);
      // X_index_start, Y_index_Start, Z_index_Start,
      // X_index_End, Y_index_End, Z_index_End,
      // Thermal Condition (0: Flux; 1:Temperature), Value of thermal condition
      if (ifDouble) {
        sscanf(string, "%d%d%d%d%d%d%d%lf", &SI, &SJ, &SK, &EI, &EJ, &EK,
          &FLTMP, &TMP);
      }
      else {
        sscanf(string, "%d%d%d%d%d%d%d%f", &SI, &SJ, &SK, &EI, &EJ, &EK,
          &FLTMP, &TMP);
      }
      sprintf(msg, "ThermalBC=%d, T/q_dot=%f", FLTMP, TMP);
      ffd_log(msg, FFD_NORMAL);

      // Find the start and end in each direction of the object
      if(SI==1) {
        SI=0;
        if(EI>=imax) EI=EI+SI+1;
        else EI=EI+SI;
      }
      else
        EI=EI+SI-1;

      if(SJ==1) {
        SJ=0;
        if(EJ>=jmax) EJ=EJ+SJ+1;
        else EJ=EJ+SJ;
      }
      else
        EJ=EJ+SJ-1;

      if(SK==1) {
        SK=0;
        if(EK>=kmax) EK=EK+SK+1;
        else EK=EK+SK;
      }
      else
        EK=EK+SK-1;

      // read the plastic curtain or ceiling tile (initially implemented by Wei Tian, 9/13/2018)
      // FIXME: only one cell in the width direction is allowed, otherwise no resistance can be added
      if ((strstr(para->bc->blockName[i], "Curtain") != NULL) || (strstr(para->bc->blockName[i], "CeilingTile") != NULL)) {
          // read name
          if (ifDouble) {
              sscanf(para->bc->blockName[i], "%s%lf", curtain_name_tmp, &curtain_open_ratio);
          }
          else {
              sscanf(para->bc->blockName[i], "%s%f", curtain_name_tmp, &curtain_open_ratio);
          }
        // assign the cell properties
        for(ii=SI; ii<=EI; ii++){
          for(ij=SJ; ij<=EJ; ij++){
            for(ik=SK; ik<=EK; ik++) {
              //BINDEX[0][index] = ii;
              //BINDEX[1][index] = ij;
              //BINDEX[2][index] = ik;
              flagp[IX(ii,ij,ik)] = FLUID;
              // assign resistance based on the facing of the curtain
              if (SI==EI){
                var[RESX][IX(ii,ij,ik)] = 1/pow(curtain_open_ratio,2)*(1.0 + 0.5*pow(1- curtain_open_ratio, 0.75)
                                                         +1.414*pow(1- curtain_open_ratio, 0.375))*0.5;
              }
              else if (SJ==EJ){
                var[RESY][IX(ii,ij,ik)] = 1/pow(curtain_open_ratio,2)*(1.0 + 0.5*pow(1- curtain_open_ratio, 0.75)
                                                         +1.414*pow(1- curtain_open_ratio, 0.375))*0.5;
              }
              else if (SK==EK){
                var[RESZ][IX(ii,ij,ik)] = 1/pow(curtain_open_ratio,2)*(1.0 + 0.5*pow(1- curtain_open_ratio, 0.75)
                                                         +1.414*pow(1- curtain_open_ratio, 0.375))*0.5;
              }
              else{
                printf("the configuration of the curtain is not supported\n");
                getchar();
              }
              // increment the index
              index++;
            }
          }
        }
      } //end of read the plastic curtain

      // set the boundary conditions based on that if it is regular block or a rack
      // if it is a rack
      else if (strstr(para->bc->blockName[i], "Rack") != NULL) {
        // split the name information
        if (ifDouble) {
          sscanf(para->bc->blockName[i], "%s%d%lf%lf", rack_name_tmp, &para->bc->RackDir[id_rack], &para->bc->HeatDiss[id_rack], &para->bc->RackFlowRate[id_rack]);
        }
        else {
          sscanf(para->bc->blockName[i], "%s%d%f%f", rack_name_tmp, &para->bc->RackDir[id_rack], &para->bc->HeatDiss[id_rack], &para->bc->RackFlowRate[id_rack]);
        }
        // store the rack name to global variable (maximum 100)
        para->bc->rackName[id_rack] = (char *)malloc(100*sizeof(char));
        strcpy(para->bc->rackName[id_rack], (const char*) rack_name_tmp);
        // find the initial flow rate
        para->bc->RackFlowRate[id_rack] *= para->bc->HeatDiss[id_rack];
        // Find the map based on the flow direction
        if (para->bc->RackDir[id_rack] == 1 || para->bc->RackDir[id_rack] == -1) {
          para->bc->RackMap[id_rack][0] = EI-SI;
          para->bc->RackMap[id_rack][1] = 0;
          para->bc->RackMap[id_rack][2] = 0;
        }
        else if (para->bc->RackDir[id_rack] == 2 || para->bc->RackDir[id_rack] == -2) {
          para->bc->RackMap[id_rack][0] = 0;
          para->bc->RackMap[id_rack][1] = EJ-SJ;
          para->bc->RackMap[id_rack][2] = 0;
        }

        else if (para->bc->RackDir[id_rack] == 3 || para->bc->RackDir[id_rack] == -3) {
          para->bc->RackMap[id_rack][0] = 0;
          para->bc->RackMap[id_rack][1] = 0;
          para->bc->RackMap[id_rack][2] = EK-SK;
        }
        else {
          para->bc->RackMap[id_rack][0] = 0;
          para->bc->RackMap[id_rack][1] = 0;
          para->bc->RackMap[id_rack][2] = 0;
        }

        //Store the cell information and boundary condition by looping all rack cells
        for(ii=SI; ii<=EI; ii++){
          for(ij=SJ; ij<=EJ; ij++){
            for(ik=SK; ik<=EK; ik++) {
              BINDEX[0][index] = ii;
              BINDEX[1][index] = ij;
              BINDEX[2][index] = ik;
              BINDEX[3][index] = FLTMP;
              BINDEX[4][index] = id_rack;
              BINDEX[5][index] = RACK;
              index++;
              // assign thermal BCs
              switch(FLTMP) {
                case 1:
                  var[TEMPBC][IX(ii,ij,ik)] = TMP;
                  break;
                case 0:
                  var[QFLUXBC][IX(ii,ij,ik)] = TMP;
                  break;
                default:
                  sprintf(msg, "read_sci_input(): Thermal BC (%d)"
                    "for cell(%d,%d,%d) was not defined",
                    FLTMP, ii, ij, ik);
                  ffd_log(msg, FFD_ERROR);
                  return 1;
              }
              // mark the face cells and cells in the middle
              if (para->bc->RackDir[id_rack] == 1) {
                if (ii==SI) {
                  flagp[IX(ii,ij,ik)] = RACK_INLET; // Flag for rack inlet
                }
                else if (ii==EI) {
                  flagp[IX(ii,ij,ik)] = RACK_OUTLET; // Flag for rack outlet
                }
                else{
                  flagp[IX(ii,ij,ik)] = SOLID; // Flag for cells in the middle
                }
              }
              else if (para->bc->RackDir[id_rack] == -1) {
                if (ii==SI) {
                  flagp[IX(ii,ij,ik)] = RACK_OUTLET; // Flag for rack outlet
                }
                else if (ii==EI) {
                  flagp[IX(ii,ij,ik)] = RACK_INLET; // Flag for rack inlet
                }
                else{
                  flagp[IX(ii,ij,ik)] = SOLID; // Flag for cells in the middle
                }
              }
              else if (para->bc->RackDir[id_rack] == 2) {
                if (ij==SJ) {
                  flagp[IX(ii,ij,ik)] = RACK_INLET; // Flag for rack inlet
                }
                else if (ij==EJ) {
                  flagp[IX(ii,ij,ik)] = RACK_OUTLET; // Flag for rack outlet
                }
                else{
                  flagp[IX(ii,ij,ik)] = SOLID; // Flag for cells in the middle
                }
              }
              else if (para->bc->RackDir[id_rack] == -2) {
                if (ij==SJ) {
                  flagp[IX(ii,ij,ik)] = RACK_OUTLET; // Flag for rack outlet
                }
                else if (ij==EJ) {
                  flagp[IX(ii,ij,ik)] = RACK_INLET; // Flag for rack inlet
                }
                else{
                  flagp[IX(ii,ij,ik)] = SOLID; // Flag for cells in the middle
                }
              }
              else if (para->bc->RackDir[id_rack] == 3) {
                if (ik==SK) {
                  flagp[IX(ii,ij,ik)] = RACK_INLET; // Flag for rack inlet
                }
                else if (ik==EK) {
                  flagp[IX(ii,ij,ik)] = RACK_OUTLET; // Flag for rack outlet
                }
                else{
                  flagp[IX(ii,ij,ik)] = SOLID; // Flag for cells in the middle
                }
              }
              else if (para->bc->RackDir[id_rack] == -3) {
                if (ik==SK) {
                  flagp[IX(ii,ij,ik)] = RACK_OUTLET; // Flag for rack outlet
                }
                else if (ik==EK) {
                  flagp[IX(ii,ij,ik)] = RACK_INLET; // Flag for rack inlet
                }
                else{
                  flagp[IX(ii,ij,ik)] = SOLID; // Flag for cells in the middle
                }
              }

            } // End of assigning value for internal solid block
          } // End of assigning value for internal solid block
        } // End of assigning value for internal solid block
        // locate the four points at front of the racks
        if (get_monitor_points(para,var,SI,SJ,SK,EI,EJ,EK,para->bc->RackDir[id_rack],id_rack)!=0){
          sprintf(msg,"read_sci_input(): cannot find the monitoring points at the front of the RACK %d",id_rack+1);
          ffd_log(msg,FFD_ERROR);
        }
        // Update the index of rack
        id_rack += 1;
      }// end of if (strstr(para->bc->blockName[i], "Rack") != NULL)
      // if it is a regular block
      else {
        // set the boundary conditions
        for(ii=SI; ii<=EI; ii++){
          for(ij=SJ; ij<=EJ; ij++){
            for(ik=SK; ik<=EK; ik++) {
              BINDEX[0][index] = ii;
              BINDEX[1][index] = ij;
              BINDEX[2][index] = ik;
              BINDEX[3][index] = FLTMP;
              BINDEX[4][index] = i;
              index++;

              switch(FLTMP) {
                case 1:
                  var[TEMPBC][IX(ii,ij,ik)] = TMP;
                  break;
                case 0:
                  var[QFLUXBC][IX(ii,ij,ik)] = TMP;
                  break;
                default:
                  sprintf(msg, "read_sci_input(): Thermal BC (%d)"
                    "for cell(%d,%d,%d) was not defined",
                    FLTMP, ii, ij, ik);
                  ffd_log(msg, FFD_ERROR);
                  return 1;
              }
              flagp[IX(ii,ij,ik)] = SOLID; // Flag for solid
            } // End of assigning value for internal solid block
          }// End of assigning value for internal solid block
        }// End of assigning value for internal solid block
      } // end of else
    } // end of for(i=0; i<para->bc->nb_block; i++)
  }// end of if(para->bc->nb_block!=0)

  /*****************************************************************************
  | Read the wall boundary conditions
  *****************************************************************************/
  fgets(string, 400, file_params);
  sscanf(string,"%d", &para->bc->nb_wall);
  sprintf(msg, "read_sci_input(): para->bc->nb_wall=%d", para->bc->nb_wall);
  ffd_log(msg, FFD_NORMAL);

  if(para->bc->nb_wall!=0) {
    /*-------------------------------------------------------------------------
    | Allocate the memory for bc name and id
    -------------------------------------------------------------------------*/
    para->bc->wallName = (char**)malloc(para->bc->nb_wall*sizeof(char*));
    if(para->bc->wallName==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for "
      "para->bc->wallName.", FFD_ERROR);
      return 1;
    }

    para->bc->wallId = (int *)malloc(sizeof(int)*para->bc->nb_wall);
    if(para->bc->wallId==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for "
      "para->bc->wallId.", FFD_ERROR);
      return 1;
    }

    for(i=0; i<para->bc->nb_wall; i++)
      para->bc->wallId[i] = -1;

    para->bc->AWall = (REAL*) malloc(para->bc->nb_wall*sizeof(REAL));
    if(para->bc->AWall==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for "
      "para->bc->AWall.", FFD_ERROR);
      return 1;
    }

    para->bc->temHea = (REAL*) malloc(para->bc->nb_wall*sizeof(REAL));
    if(para->bc->temHea==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for "
      "para->bc->heaTem.", FFD_ERROR);
      return 1;
    }

    para->bc->temHeaAve = (REAL*) malloc(para->bc->nb_wall*sizeof(REAL));
    if(para->bc->temHeaAve==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for "
      "para->bc->temHeaAve.", FFD_ERROR);
    return 1;
    }

    para->bc->temHeaMean = (REAL*) malloc(para->bc->nb_wall*sizeof(REAL));
    if(para->bc->temHeaMean==NULL) {
      ffd_log("read_sci_input(): Could not allocate memory for "
      "para->bc->temHeaMean.", FFD_ERROR);
      return 1;
    }

    /*-------------------------------------------------------------------------
    | Read wall conditions for each wall
    -------------------------------------------------------------------------*/
    for(i=0; i<para->bc->nb_wall; i++) {
      /*.......................................................................
      | Get the names of boundary
      .......................................................................*/
      fgets(string, 400, file_params);
      // Get the length of name (The name may contain white space)
      for(j=0; string[j] != '\n'; j++) {
        continue;
      }

      para->bc->wallName[i] = (char*)malloc((j+1)*sizeof(char));
      if(para->bc->wallName[i]==NULL) {
        sprintf(msg, "read_sci_input(): Could not allocate memory for "
                "para->bc->wallName[%d].", i);
        ffd_log(msg, FFD_ERROR);
        return 1;
      }

      strncpy(para->bc->wallName[i], (const char*)string, j);
      // Add an ending
      para->bc->wallName[i][j] = '\0';
      sprintf(msg, "read_sci_input(): para->bc->wallName[%d]=\"%s\"",
             i, para->bc->wallName[i]);
      ffd_log(msg, FFD_NORMAL);
      /*.......................................................................
      | Get the boundary conditions
      .......................................................................*/
      // X_index_start, Y_index_Start, Z_index_Start,
      // X_index_End, Y_index_End, Z_index_End,
      // Thermal Condition (0: Flux; 1:Temperature), Value of thermal condition
      fgets(string, 400, file_params);
      if (ifDouble) {
        sscanf(string, "%d%d%d%d%d%d%d%lf", &SI, &SJ, &SK, &EI,
          &EJ, &EK, &FLTMP, &TMP);
      }
      else {
        sscanf(string, "%d%d%d%d%d%d%d%f", &SI, &SJ, &SK, &EI,
          &EJ, &EK, &FLTMP, &TMP);
      }
        sprintf(msg, "read_sci_input(): ThermalBC=%d, T/q_dot=%f",
                FLTMP, TMP);
        ffd_log(msg, FFD_NORMAL);

      // Reset X index
      if(SI==1) { // West
        SI = 0;
        if(EI>=imax) EI = EI + 1;
      }
      else if(SI==imax+1) // East
        EI = EI + SI;
      else // Internal
        EI = EI + SI - 1;

      // Reset Y index
      if(SJ==1) { // South
        SJ = 0;
        if(EJ>=jmax) EJ = EJ + 1;
      }
      else if(SJ==jmax+1) // North
        EJ = EJ + SJ;
      else // Internal
        EJ = EJ + SJ - 1;
      // Reset Z index
      if(SK==1) { // Floor
        SK = 0;
        if(EK>=kmax) EK = EK + 1;
      }
      else if (SK==kmax+1) // Ceiling
        EK = EK + SK;
      else // Internal
          EK = EK + SK -1;

      // Assign value for each wall cell
      for(ii=SI; ii<=EI; ii++){
        for(ij=SJ; ij<=EJ; ij++){
          for(ik=SK; ik<=EK; ik++) {
            // If cell hasn't been updated (default value -1)
            if(flagp[IX(ii,ij,ik)]<0) {
              BINDEX[0][index] = ii;
              BINDEX[1][index] = ij;
              BINDEX[2][index] = ik;
              // Define the thermal boundary property
              BINDEX[3][index] = FLTMP;
              BINDEX[4][index] = i;
              index++;

              // Set the cell to solid
              flagp[IX(ii,ij,ik)] = SOLID;
              if(FLTMP==1) var[TEMPBC][IX(ii,ij,ik)] = TMP;
              if(FLTMP==0) var[QFLUXBC][IX(ii,ij,ik)] = TMP;
            }
          } // End of assigning value for each wall cell
        }
      }
    } // End of assigning value for each wall surface
  } // End of assigning value for wall boundary

  /*****************************************************************************
  | Read the boundary conditions for contaminant source
  | Warning: The data is ignored in current version
  *****************************************************************************/
  fgets(string, 400, file_params);
  sscanf(string,"%d", &para->bc->nb_source);
  sprintf(msg, "read_sci_input(): para->bc->nb_source=%d", para->bc->nb_source);
  ffd_log(msg, FFD_NORMAL);

  if(para->bc->nb_source!=0) {
    if (ifDouble) {
      sscanf(string, "%s%d%d%d%d%d%d%lf",
        name, &SI, &SJ, &SK, &EI, &EJ, &EK, &MASS);
    }
    else {
      sscanf(string, "%s%d%d%d%d%d%d%f",
        name, &SI, &SJ, &SK, &EI, &EJ, &EK, &MASS);
    }
    bcnameid++;

    sprintf(msg, "read_sci_input(): Source %s is not used in current version.",
            name);
    ffd_log(msg, FFD_WARNING);
    sprintf(msg, "read_sci_input(): Xi_dot=%f", MASS);
    ffd_log(msg, FFD_NORMAL);
    //Warning: Need to add code to assign the BC value as other part does
  }

  para->geom->index=index;

  /*****************************************************************************
  | Read other simulation data
  *****************************************************************************/
  // Discard the unused data
  //fgets(string, 400, file_params); //maximum iteration
  //fgets(string, 400, file_params); //maximum iteration
  //fgets(string, 400, file_params); //convergence rate
  //fgets(string, 400, file_params); //Turbulence model
  //fgets(string, 400, file_params); //initial value
  //fgets(string, 400, file_params); //minimum value
  //fgets(string, 400, file_params); //maximum value
  //fgets(string, 400, file_params); //fts value
  //fgets(string, 400, file_params); //under relaxation
  //fgets(string, 400, file_params); //reference point
  //fgets(string, 400, file_params); //monitoring point

  // Discard setting for restarting the old FFD simulation
  //fgets(string, 400, file_params);
  /*
  sscanf(string,"%d", &para->inpu->read_old_ffd_file);
  sprintf(msg, "read_sci_input(): para->inpu->read_old_ffd_file=%d",
          para->inpu->read_old_ffd_file);
  ffd_log(msg, FFD_NORMAL);
  */
  // Discard the unused data
  //fgets(string, 400, file_params); //print frequency
  //fgets(string, 400, file_params); //Pressure variable Y/N
  //fgets(string, 400, file_params); //Steady state, buoyancy.

  // Discard physical properties
  //fgets(string, 400, file_params);
  /*
  sscanf(string,"%f %f %f %f %f %f %f %f %f", &para->prob->rho,
         &para->prob->nu, &para->prob->cond,
         &para->prob->gravx, &para->prob->gravy, &para->prob->gravz,
         &para->prob->beta, &trefmax, &para->prob->Cp);

  sprintf(msg, "read_sci_input(): para->prob->rho=%f", para->prob->rho);
  ffd_log(msg, FFD_NORMAL);

  sprintf(msg, "read_sci_input(): para->prob->nu=%f", para->prob->nu);
  ffd_log(msg, FFD_NORMAL);

  sprintf(msg, "read_sci_input(): para->prob->cond=%f", para->prob->cond);
  ffd_log(msg, FFD_NORMAL);

  sprintf(msg, "read_sci_input(): para->prob->gravx=%f", para->prob->gravx);
  ffd_log(msg, FFD_NORMAL);

  sprintf(msg, "read_sci_input(): para->prob->gravy=%f", para->prob->gravy);
  ffd_log(msg, FFD_NORMAL);

  sprintf(msg, "read_sci_input(): para->prob->gravz=%f", para->prob->gravz);
  ffd_log(msg, FFD_NORMAL);

  sprintf(msg, "read_sci_input(): para->prob->beta=%f", para->prob->beta);
  ffd_log(msg, FFD_NORMAL);

  //para->prob->trefmax=trefmax;
  sprintf(msg, "read_sci_input(): para->prob->Cp=%f", para->prob->Cp);
  ffd_log(msg, FFD_NORMAL);
  */

  // Read simulation time settings
  fgets(string, 400, file_params);
  sscanf(string,"%lf %lf %d", &para->mytime->t_start, &para->mytime->dt,
    &para->mytime->step_total);

  sprintf(msg, "read_sci_input(): para->mytime->t_start=%lf",
          para->mytime->t_start);
  ffd_log(msg, FFD_NORMAL);

  sprintf(msg, "read_sci_input(): para->mytime->dt=%lf", para->mytime->dt);
  ffd_log(msg, FFD_NORMAL);

  sprintf(msg, "read_sci_input(): para->mytime->step_total=%d",
          para->mytime->step_total);
  ffd_log(msg, FFD_NORMAL);

  //fgets(string, 400, file_params); //prandtl

  /*****************************************************************************
  | Conclude the reading process
  *****************************************************************************/
  fclose(file_params);

  free(delx);
  free(dely);
  free(delz);

  sprintf(msg, "read_sci_input(): Read sci input file %s",
          para->inpu->parameter_file_name);
  ffd_log(msg, FFD_NORMAL);
  return 0;
} // End of read_sci_input()


///////////////////////////////////////////////////////////////////////////////
/// Identify the properties of cells
///
///\param para Pointer to FFD parameters
///\param var Pointer to FFD simulation variables
///
///\return 0 if no error occurred
///////////////////////////////////////////////////////////////////////////////
void mark_cell(PARA_DATA *para, REAL **var, int **BINDEX) {
  int i, j, k;
  int ii, ij, ik, index;
  int rack_id, rack_direction;
  int imax = para->geom->imax;
  int jmax = para->geom->jmax;
  int kmax = para->geom->kmax;
  int IMAX = imax+2, IJMAX = (imax+2)*(jmax+2);
  REAL *flagu = var[FLAGU],*flagv = var[FLAGV],*flagw = var[FLAGW];
  REAL *flagp = var[FLAGP];
  int put_X = para->geom->tile_putX, put_Y =para->geom->tile_putY, put_Z = para->geom->tile_putZ;

  flagp[IX(0,0,0)] = SOLID;
  flagp[IX(0,0,kmax+1)] = SOLID;
  flagp[IX(0,jmax+1,0)] = SOLID;
  flagp[IX(0,jmax+1,kmax+1)] = SOLID;
  flagp[IX(imax+1,0,0)] = SOLID;
  flagp[IX(imax+1,0,kmax+1)] = SOLID;
  flagp[IX(imax+1,jmax+1,0)] = SOLID;
  flagp[IX(imax+1,jmax+1,kmax+1)] = SOLID;

  FOR_EACH_CELL

    if(flagp[IX(i,j,k)]>=0) continue;

    if(flagp[IX(i-1,j,k)]>=0 && flagp[IX(i+1,j,k)]>=0 &&
      flagp[IX(i,j-1,k)]>=0 && flagp[IX(i,j+1,k)]>=0 &&
      flagp[IX(i,j,k-1)]>=0 && flagp[IX(i,j,k+1)]>=0 )
      flagp[IX(i,j,k)] = SOLID;
  END_FOR

  FOR_ALL_CELL
    // Mark solids
    if(flagp[IX(i,j,k)]==SOLID) {

      flagu[IX(i,j,k)]=SOLID;
      flagv[IX(i,j,k)]=SOLID;
      flagw[IX(i,j,k)]=SOLID;

      if(i!=0) flagu[IX(i-1,j,k)]=SOLID;
      if(j!=0) flagv[IX(i,j-1,k)]=SOLID;
      if(k!=0) flagw[IX(i,j,k-1)]=SOLID;
    }// Mark solids

    // Mark rack outlets
    if(flagp[IX(i,j,k)]==RACK_OUTLET) {
      // get direction of rack
      for (index=0;index<para->geom->index;index++){
        ii = BINDEX[0][index];
        ij = BINDEX[1][index];
        ik = BINDEX[2][index];
        if (i==ii && j==ij && k==ik){
          rack_id = BINDEX[4][index];
          rack_direction = para->bc->RackDir[rack_id];
          break;
        }
      }
      // Mark cells
      flagu[IX(i,j,k)]=INLET;
      flagv[IX(i,j,k)]=INLET;
      flagw[IX(i,j,k)]=INLET;

      if (rack_direction == -1){
        if (i!=0) {
          flagu[IX(i-1,j,k)]=INLET;
        }
      }
      if (rack_direction == -2){
        if (j!=0) {
          flagv[IX(i,j-1,k)]=INLET;
        }
      }
      if (rack_direction == -3){
        if (k!=0) {
          flagw[IX(i,j,k-1)]=INLET;
        }
      }
    }// end of marking rack outlets

    // Mark rack inlets
    if(flagp[IX(i,j,k)]==RACK_INLET) {
      // get direction of rack
      for (index=0;index<para->geom->index;index++){
        ii = BINDEX[0][index];
        ij = BINDEX[1][index];
        ik = BINDEX[2][index];
        if (i==ii && j==ij && k==ik){
          rack_id = BINDEX[4][index];
          rack_direction = para->bc->RackDir[rack_id];
          break;
        }
      }
       // mark cells
      flagu[IX(i,j,k)]=OUTLET;
      flagv[IX(i,j,k)]=OUTLET;
      flagw[IX(i,j,k)]=OUTLET;

      if (rack_direction == 1){
        if (i!=0) {
          flagu[IX(i-1,j,k)]=OUTLET;
        }
      }
      if (rack_direction == 2){
        if (j!=0) {
          flagv[IX(i,j-1,k)]=OUTLET;
        }
      }
      if (rack_direction == 3){
        if (k!=0) {
          flagw[IX(i,j,k-1)]=OUTLET;
        }
      }
    }// end of marking rack inlets

    // Mark inlets
    if(flagp[IX(i,j,k)]==INLET) {
      flagu[IX(i,j,k)]=INLET;
      flagv[IX(i,j,k)]=INLET;
      flagw[IX(i,j,k)]=INLET;
      if(i!=0) flagu[IX(i-1,j,k)]=INLET;
      if(j!=0) flagv[IX(i,j-1,k)]=INLET;
      if(k!=0) flagw[IX(i,j,k-1)]=INLET;
    }// end of marking inlets

    // Mark outlets
    if(flagp[IX(i,j,k)]==OUTLET || flagp[IX(i, j, k)] == TILE) {
      flagu[IX(i,j,k)]=OUTLET;
      flagv[IX(i,j,k)]=OUTLET;
      flagw[IX(i,j,k)]=OUTLET;
      if(i!=0) flagu[IX(i-1,j,k)]=OUTLET;
      if(j!=0) flagv[IX(i,j-1,k)]=OUTLET;
      if(k!=0) flagw[IX(i,j,k-1)]=OUTLET;

    }// end of marking outlets

  END_FOR
} // End of mark_cell()
