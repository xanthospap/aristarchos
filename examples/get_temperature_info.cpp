#include "atmcdLXd.h"
#include <cstdio>
#include <unistd.h>
#include <chrono>
#include <thread>

/// @brief Camera selector
/// This function will select a camera, taking care of the case when multiple
/// cameras are available. To 'select' a camera, means to get its handle and
/// set it as current. In case only one camera is available, none of the
/// above is really needed.
/// @param[in] cameraNum  The camera number we want to get the handle of. Must
///            be cameraNum >= 0
/// @return An int denoting the camera number which we set as current. Should
///         be the same as cameraNum and always >= 0. If we return a value < 0,
///         an error has occured.
int CameraSelect(int cameraNum = 0) noexcept {
  if (cameraNum > 0) {
    at_32 lNumCameras;
    GetAvailableCameras(&lNumCameras);

    if (cameraNum < lNumCameras && cameraNum >= 0) {
      at_32 lCameraHandle;
      GetCameraHandle(cameraNum, &lCameraHandle);
      SetCurrentCamera(lCameraHandle);
      return cameraNum;
    } else
      return -1;
  }
  return 0;
}

int main() {
  if (CameraSelect() < 0) {
    fprintf(stderr, "[ERROR] Failed to get/select camera.\n");
    return 1;
  }

  // Initialize CCD
  int error = Initialize("/usr/local/etc/andor");
  if (error != DRV_SUCCESS) {
    fprintf(stderr, "[ERROR] Initialisation error...exiting\n");
    return 1;
  }
  
  // sleep to allow initialization to complete
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));

  // get temperature range in Celsius
  int mintemp, maxtemp;
  if (GetTemperatureRange(&mintemp, &maxtemp)!=DRV_SUCCESS) {
    fprintf(stderr, "[ERROR] Failed to get camera's temperature range!\n");
    return 1;
  }
  printf("Temperature range for camera: %5d to %5d Celsius\n", mintemp, maxtemp);

  // get temperature in degrees of the detector and the status of 
  // cooling process
  float ftemperature;
  unsigned int status;
  status = GetTemperatureF(&ftemperature);
  printf("Temperature is %7.3f, function returned: ", ftemperature);
  switch (status) {
    case DRV_NOT_INITIALIZED:
      printf("System not initialized.\n");
      break;
    case (DRV_ACQUIRING):
      printf("Acquisition in progress.\n");
      break;
    case (DRV_ERROR_ACK):
      printf("Unable to communicate with card.\n");
      break;
    case (DRV_TEMP_OFF):
      printf("Temperature is OFF.\n");
      break;
    case (DRV_TEMP_STABILIZED):
      printf("Temperature has stabilized at set point.\n");
      break;
    case (DRV_TEMP_NOT_REACHED):
      printf("Temperature has not reached set point.\n");
      break;
    case (DRV_TEMP_DRIFT):
      printf("Temperature had stabilised but has since drifted.\n");
      break;
    case (DRV_TEMP_NOT_STABILIZED):
      printf("Temperature reached but not stabilized.\n");
      break;
    default:
      printf("Well, exit status denotes nothing .... weird\n");
  }

  // get temperature of the detector to the nearest degree; also
  // get the status of cooling process.
  int itemperature;
  status = GetTemperature(&itemperature);
  printf("Temperature is %4d, function returned: ", itemperature);
  switch (status) {
    case DRV_NOT_INITIALIZED:
      printf("System not initialized.\n");
      break;
    case (DRV_ACQUIRING):
      printf("Acquisition in progress.\n");
      break;
    case (DRV_ERROR_ACK):
      printf("Unable to communicate with card.\n");
      break;
    case (DRV_TEMP_OFF):
      printf("Temperature is OFF.\n");
      break;
    case (DRV_TEMP_STABILIZED):
      printf("Temperature has stabilized at set point.\n");
      break;
    case (DRV_TEMP_NOT_REACHED):
      printf("Temperature has not reached set point.\n");
      break;
    case (DRV_TEMP_DRIFT):
      printf("Temperature had stabilised but has since drifted.\n");
      break;
    case (DRV_TEMP_NOT_STABILIZED):
      printf("Temperature reached but not stabilized.\n");
      break;
    default:
      printf("Well, exit status denotes nothing .... weird\n");
  }
}
