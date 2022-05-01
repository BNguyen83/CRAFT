#ifndef FORCE_h
#define FORCE_h

void resetForceFlag();
void forceSetup();
void measureInsertion(int*);
void measureRemoval(int*);
void forceCalibrationSetup();
float forceCalibration();
#endif
