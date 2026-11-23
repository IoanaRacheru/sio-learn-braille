#ifndef HARDWARE_DIAGNOSTICS_H
#define HARDWARE_DIAGNOSTICS_H

void hardware_diagnostics_begin();
// Poll from loop(). Returns true once the startup report is complete.
bool hardware_diagnostics_complete();

#endif
