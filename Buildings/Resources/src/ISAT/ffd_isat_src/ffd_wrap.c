///////////////////////////////////////////////////////////////////////////////
///
/// \file   ffd_wrap.c
///
/// \brief  wrapper to call FFD engine to evaluate the input; the function ffd_isat
///         is then called by usrfgh. A important note is that this function call only
///         evaluate f, not g, which is the jacobian matrix.
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

#include "ffd_wrap.h"

//Extern global variables form mpc.c
extern int useErrorContraolCorrection;
extern float VPower;

//Global variables shared with ffd()
double ffdInput[nx_SIZE];          // westWallT, eastWallT
double ffdOutput[nf_SIZE];         // centerT, centerUx, centerUy, centerUz
double **ffd_exist_result;         // used to store the existing data

///////////////////////////////////////////////////////////////////////////////
/// Solver to evaluate f, g, and h which called by USRFGH
///
///\param Array of need
///                     Note: if need[0] = 1, evaluate f
///                           if need[1] = 1, evaluate g
///\param Array of x
///\param Array of f
///\param Array of g
///\param Pointer of user definded struct
///
///\return No return needed
///////////////////////////////////////////////////////////////////////////////
void ffd_ISAT (int need[], double x[], double f[], double g[][nf_SIZE], void *p){
  // static 2D array requires that second level of pointers be constpointer to static array

  int i;
  int i_index = 0;
  ffd_log("Start the FFD simulation by ISAT", FFD_NEW);

  /****************************************************************************
  | Copy input to global variables
  ****************************************************************************/
  for (i = 0; i < nx_SIZE; i++)
    ffdInput[i] = x[i];

  /****************************************************************************
  | Call FFD and caculate gradient
  ****************************************************************************/
  if (READ_FFD_RESULT) {
    for (i_index = 0;i_index < 2500;i_index++) {
      if (fabs(ffdInput[0] - ffd_exist_result[i_index][0]) < 1e-6 && fabs(ffdInput[1] - ffd_exist_result[i_index][1]) < 1e-6) {
        f[0] = 1 * ffd_exist_result[i_index][2];
        //f[1] = VPower * ffd_exist_result[i_index][3];
        //f[2] = 1 * ffd_exist_result[i_index][4];
        // terminate the function
								need[0] = 0;
								need[1] = 0;
        return;
      }

    }
  }

  if (need[0]){
    if(ffd_main()!=0)
      mpc_log("ffd_ISAT(): simulation failed", MPC_ERROR);
  }


		need[1] = 0;
  if (need[1]){
    mpc_log("ffd_ISAT(): Gradient evaluation is not been implemented", MPC_ERROR);
    //exit(1);
  }

  /****************************************************************************
  | Copy result to ISAT shared pointer
  ****************************************************************************/
  for (i = 0; i < nf_SIZE; i++)
    f[i] = ffdOutput[i];


  /****************************************************************************
  | Weights for error control 
  | This is need since each compoinet of output vector has different order of magnitudes
  ****************************************************************************/
  if (useErrorContraolCorrection){
    f[0]=1*f[0];
    //f[1]=f[1]*1;
    //f[2]=f[1]*1;
    //f[3]=f[1]*1;
  }
}
///////////////////////////////////////////////////////////////////////////////
/// Read existing ffd_isat log file
///
///\return No return needed
///////////////////////////////////////////////////////////////////////////////
FILE *file_params_isat;
int read_existing() {
  char string[400];
  int i = 0;
  int j = -1;
  if ((file_params_isat = fopen("existing.mpc", "r")) == NULL) {
    sprintf(msg, "read_existing(): Could not open the file \"%s\".", "existing.mpc");
    ffd_log(msg, FFD_ERROR);
    return 1;
  }

  //initialize the viarbale to store reading data
  //ffd_exist_result = (double **)malloc(1000 * sizeof(double *));
		ffd_exist_result = (double **)malloc(2500 * sizeof(double *));
  //for (i = 0;i < 1000;i++) {
		for (i = 0;i < 2500;i++) {
    ffd_exist_result[i] = (double *)malloc(5 * sizeof(double));
  }


  // read data from file
  j = -1;
  while (fgets(string, 400, file_params_isat) != NULL) {
    j++;

    sscanf(string, "%lf%lf%lf%lf%lf", &ffd_exist_result[j][0], &ffd_exist_result[j][1], &ffd_exist_result[j][2],
      &ffd_exist_result[j][3], &ffd_exist_result[j][4]);
    printf("input is %f\t%f\t%f\t%f\t%f\n", ffd_exist_result[j][0], ffd_exist_result[j][1], ffd_exist_result[j][2],
      ffd_exist_result[j][3], ffd_exist_result[j][4]);
  }
  fclose(file_params_isat);

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
/// Evaluate Numerical Differentiation of f(x), essentially a jacobian function
///
///\param Pointer of g[nx][nf]
///
///\return No return needed
///////////////////////////////////////////////////////////////////////////////
void numericalDifferentiation (double g[][nf_SIZE]){
  // Evaluate Numerical Differentiation of f(x)

  mpc_log("numericalDifferentiation(): Numerical Differentiation has not been implemented", MPC_ERROR);
  exit(1);
}