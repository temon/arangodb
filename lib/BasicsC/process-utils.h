////////////////////////////////////////////////////////////////////////////////
/// @brief collection of process functions
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2013 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Esteban Lombeyda
/// @author Copyright 2008-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_BASICS_C_PROCESS_UTILS_H
#define TRIAGENS_BASICS_C_PROCESS_UTILS_H 1

#include "BasicsC/common.h"

#include "BasicsC/threads.h"

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                  public constants
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief invalid process
////////////////////////////////////////////////////////////////////////////////

#define INVALID_PROCESS (0)

// -----------------------------------------------------------------------------
// --SECTION--                                                      public types
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief returns information about the process
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_process_info_s {
  uint64_t _minorPageFaults;
  uint64_t _majorPageFaults;
  uint64_t _userTime;
  uint64_t _systemTime;
  int64_t _numberThreads;
  int64_t _residentSize; // resident set size in number of bytes
  uint64_t _virtualSize;
  uint64_t _scClkTck;
}
TRI_process_info_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief status of an external process
////////////////////////////////////////////////////////////////////////////////

typedef enum {
  TRI_EXT_NOT_STARTED = 0, 	// not yet started
  TRI_EXT_PIPE_FAILED = 1, 	// pipe before start failed
  TRI_EXT_FORK_FAILED = 2,	// fork failed
  TRI_EXT_RUNNING = 3,		// running
  TRI_EXT_NOT_FOUND = 4,	// unknown pid
  TRI_EXT_TERMINATED = 5,	// process has terminated normally
  TRI_EXT_ABORTED = 6,		// process has terminated abnormally
  TRI_EXT_STOPPED = 7,		// process has been stopped
  TRI_EXT_KILL_FAILED = 8,	// kill failed
}
TRI_external_status_e;

////////////////////////////////////////////////////////////////////////////////
/// @brief identifier of an external process
////////////////////////////////////////////////////////////////////////////////

typedef pid_t TRI_external_id_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief external process description
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_external_s {
  char* _executable;
  size_t _numberArguments;
  char** _arguments;

  TRI_external_id_t _pid;

  int _readPipe;
  int _writePipe;

  TRI_external_status_e _status;
  int _exitStatus;
}
TRI_external_t;

////////////////////////////////////////////////////////////////////////////////
/// @brief external process status
////////////////////////////////////////////////////////////////////////////////

typedef struct TRI_external_status_s {
  TRI_external_status_e _status;
  int _exitStatus;
}
TRI_external_status_t;

// -----------------------------------------------------------------------------
// --SECTION--                                                  public functions
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief converts usec and sec into seconds
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_HAVE_GETRUSAGE
uint64_t TRI_MicrosecondsTv (struct timeval* tv);
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief returns information about the current process
////////////////////////////////////////////////////////////////////////////////

TRI_process_info_t TRI_ProcessInfoSelf (void);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns information about the process
////////////////////////////////////////////////////////////////////////////////

TRI_process_info_t TRI_ProcessInfo (TRI_pid_t pid);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the size of the current process
////////////////////////////////////////////////////////////////////////////////

uint64_t TRI_ProcessSizeSelf (void);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the size of the process given its pid
////////////////////////////////////////////////////////////////////////////////

uint64_t TRI_ProcessSize (TRI_pid_t pid);

////////////////////////////////////////////////////////////////////////////////
/// @brief sets the process name
////////////////////////////////////////////////////////////////////////////////

void TRI_SetProcessTitle (char const* title);

////////////////////////////////////////////////////////////////////////////////
/// @brief starts an external process
////////////////////////////////////////////////////////////////////////////////

TRI_external_id_t TRI_CreateExternalProcess (const char* executable,
                                             const char** arguments,
                                             size_t n);

////////////////////////////////////////////////////////////////////////////////
/// @brief returns the status of an external process
////////////////////////////////////////////////////////////////////////////////

TRI_external_status_t TRI_CheckExternalProcess (pid_t pid);

////////////////////////////////////////////////////////////////////////////////
/// @brief kills an external process
////////////////////////////////////////////////////////////////////////////////

void TRI_KillExternalProcess (pid_t pid);

// -----------------------------------------------------------------------------
// --SECTION--                                                            MODULE
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// --SECTION--                                            modules initialisation
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @brief initialises the process components
////////////////////////////////////////////////////////////////////////////////

void TRI_InitialiseProcess (int argc, char* argv[]);

////////////////////////////////////////////////////////////////////////////////
/// @brief shut downs the process components
////////////////////////////////////////////////////////////////////////////////

void TRI_ShutdownProcess (void);

#ifdef __cplusplus
}
#endif

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
