#include <windows.h>
#include <stdio.h>

int
main(int argc, char *argv[]) {

  STARTUPINFO startupInfo;
  PROCESS_INFORMATION piProcInfoP1, piProcInfoP2;
  HANDLE hReadPipe, hWritePipe;
  SECURITY_ATTRIBUTES saAttr;

  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = NULL;

  if (!CreatePipe(&hReadPipe,  // hReadPipe
		  &hWritePipe, // hWritePipe
		  &saAttr,     // lpPipeAttributes
		  0)) {        // nSize

    fprintf(stderr, "Error creando tuberias: %ld\n", GetLastError());

    ExitProcess((DWORD) EXIT_FAILURE);
  }

  // Inicializando las estructuras de la informacion de los procesos
  ZeroMemory(&piProcInfoP1, sizeof(PROCESS_INFORMATION));

  // Inicializando la informacion que cada proceso necesita
  ZeroMemory(&startupInfo, sizeof(STARTUPINFO));
  startupInfo.cb = sizeof(STARTUPINFO);
  startupInfo.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);
  startupInfo.hStdOutput = hWritePipe;
  startupInfo.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
  startupInfo.dwFlags    |= STARTF_USESTDHANDLES;

  if (!CreateProcess(NULL, TEXT("mostrar-archivo"), NULL, NULL,
                     TRUE, 0, NULL, NULL, &startupInfo, &piProcInfoP1)) {

    fprintf(stderr, "Error creando proceso mostrar-archivo: %ld\n",
	    GetLastError());

    ExitProcess((DWORD) EXIT_FAILURE);
  }

  CloseHandle(hWritePipe);
  // Inicializando las estructuras de la informacion de los procesos
  ZeroMemory(&piProcInfoP2, sizeof(PROCESS_INFORMATION));

  // Inicializando la informacion que cada proceso necesita
  ZeroMemory(&startupInfo, sizeof(STARTUPINFO));

  startupInfo.cb = sizeof(STARTUPINFO);
  startupInfo.hStdInput  = hReadPipe;
  startupInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
  startupInfo.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
  startupInfo.dwFlags    |= STARTF_USESTDHANDLES;

  if (!CreateProcess(NULL, TEXT("concatenar"), NULL, NULL,
                     TRUE, 0, NULL, NULL, &startupInfo, &piProcInfoP2)) {
    fprintf(stderr, "Error creando proceso concatenar-archivo: %ld\n",
	    GetLastError());
    ExitProcess((DWORD) EXIT_FAILURE);
  }

  CloseHandle(hReadPipe);
  WaitForSingleObject(piProcInfoP1.hProcess, INFINITE);
  WaitForSingleObject(piProcInfoP2.hProcess, INFINITE);

  CloseHandle(piProcInfoP1.hProcess);
  CloseHandle(piProcInfoP1.hThread);
  CloseHandle(piProcInfoP2.hProcess);
  CloseHandle(piProcInfoP2.hThread);

  ExitProcess((DWORD) EXIT_SUCCESS);
}
