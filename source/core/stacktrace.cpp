// This file is part of CaesarIA.
//
// CaesarIA is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CaesarIA is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CaesarIA.  If not, see <http://www.gnu.org/licenses/>.
//
// Timo Bingmann from http://idlebox.net/
// Copyright 2012-2014 Dalerank, dalerankn8@gmail.com

#include "stacktrace.hpp"

#include <stdlib.h>
#if !defined(GAME_PLATFORM_WIN) && !defined(GAME_PLATFORM_ANDROID)
  #include <execinfo.h>
#endif

#if !defined(GAME_PLATFORM_HAIKU) && !defined(GAME_PLATFORM_ANDROID) && !defined(_MSC_VER)
  #include <cxxabi.h>
#endif

#include "scopedptr.hpp"
#include "bytearray.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include "vfs/directory.hpp"
#include "osystem.hpp"
#include <signal.h>

#ifdef GAME_PLATFORM_WIN
#include <windows.h>
#include <dbghelp.h>
#endif

namespace crashhandler
{

#ifdef GAME_PLATFORM_WIN
/** Convert exception code to human readable string. */
static const char *ExceptionName(DWORD exceptionCode)
{
  switch (exceptionCode)
  {
    case EXCEPTION_ACCESS_VIOLATION:         return "Access violation";
    case EXCEPTION_DATATYPE_MISALIGNMENT:    return "Datatype misalignment";
    case EXCEPTION_BREAKPOINT:               return "Breakpoint";
    case EXCEPTION_SINGLE_STEP:              return "Single step";
    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "Array bounds exceeded";
    case EXCEPTION_FLT_DENORMAL_OPERAND:     return "Float denormal operand";
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "Float divide by zero";
    case EXCEPTION_FLT_INEXACT_RESULT:       return "Float inexact result";
    case EXCEPTION_FLT_INVALID_OPERATION:    return "Float invalid operation";
    case EXCEPTION_FLT_OVERFLOW:             return "Float overflow";
    case EXCEPTION_FLT_STACK_CHECK:          return "Float stack check";
    case EXCEPTION_FLT_UNDERFLOW:            return "Float underflow";
    case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "Integer divide by zero";
    case EXCEPTION_INT_OVERFLOW:             return "Integer overflow";
    case EXCEPTION_PRIV_INSTRUCTION:         return "Privileged instruction";
    case EXCEPTION_IN_PAGE_ERROR:            return "In page error";
    case EXCEPTION_ILLEGAL_INSTRUCTION:      return "Illegal instruction";
    case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "Noncontinuable exception";
    case EXCEPTION_STACK_OVERFLOW:           return "Stack overflow";
    case EXCEPTION_INVALID_DISPOSITION:      return "Invalid disposition";
    case EXCEPTION_GUARD_PAGE:               return "Guard page";
    case EXCEPTION_INVALID_HANDLE:           return "Invalid handle";
  }
  return "Unknown exception";
}

/** Print out a stacktrace (x86 and x64) using the dbghelp StackWalk64 API. */
static void Stacktrace(LPEXCEPTION_POINTERS e)
{
  CONTEXT ctx = *e->ContextRecord;
  STACKFRAME64 sf;
  ZeroMemory(&sf, sizeof(sf));

  DWORD machine;
#if defined(_M_X64) || defined(__x86_64__)
  machine = IMAGE_FILE_MACHINE_AMD64;
  sf.AddrPC.Offset = ctx.Rip;
  sf.AddrStack.Offset = ctx.Rsp;
  sf.AddrFrame.Offset = ctx.Rbp;
#else
  machine = IMAGE_FILE_MACHINE_I386;
  sf.AddrPC.Offset = ctx.Eip;
  sf.AddrStack.Offset = ctx.Esp;
  sf.AddrFrame.Offset = ctx.Ebp;
#endif
  sf.AddrPC.Mode = AddrModeFlat;
  sf.AddrStack.Mode = AddrModeFlat;
  sf.AddrFrame.Mode = AddrModeFlat;

  HANDLE process = GetCurrentProcess();
  HANDLE thread = GetCurrentThread();

  char symBuffer[sizeof(IMAGEHLP_SYMBOL64) + MAX_PATH];
  IMAGEHLP_SYMBOL64* sym = reinterpret_cast<IMAGEHLP_SYMBOL64*>(symBuffer);

  for (int count = 0; count < 64; ++count)
  {
    BOOL more = StackWalk64(machine, process, thread, &sf, &ctx, NULL,
                            SymFunctionTableAccess64, SymGetModuleBase64, NULL);
    if (!more || sf.AddrPC.Offset == 0)
      break;

    DWORD64 modBase = SymGetModuleBase64(process, sf.AddrPC.Offset);
    char modname[MAX_PATH] = "Unknown";
    if (modBase)
      GetModuleFileNameA(reinterpret_cast<HMODULE>(static_cast<uintptr_t>(modBase)), modname, MAX_PATH);

    unsigned long long pc = static_cast<unsigned long long>(sf.AddrPC.Offset);
    unsigned long long rva = modBase ? pc - static_cast<unsigned long long>(modBase) : 0ull;

    ZeroMemory(symBuffer, sizeof(symBuffer));
    sym->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
    sym->MaxNameLength = MAX_PATH;
    DWORD64 disp = 0;

    if (SymGetSymFromAddr64(process, sf.AddrPC.Offset, &disp, sym))
      Logger::warning("({}) {} ({}+0x{:x}) [0x{:x}] rva=0x{:x}", count, modname, sym->Name,
                      static_cast<unsigned long long>(disp), pc, rva);
    else
      Logger::warning("({}) {} [0x{:x}] rva=0x{:x}", count, modname, pc, rva);
  }
}

/** Callback for SymEnumerateModules */
#if _MSC_VER >= 1500
static BOOL CALLBACK EnumModules(PCSTR moduleName, ULONG baseOfDll, PVOID userContext)
#else
static BOOL CALLBACK EnumModules(LPSTR moduleName, DWORD baseOfDll, PVOID userContext)
#endif
{
  Logger::warning("0x%08x\t%s\n", baseOfDll, moduleName);
  return TRUE;
}

/** Called by windows if an exception happens. */
static LONG CALLBACK ExceptionHandler(LPEXCEPTION_POINTERS e)
{
  // Prologue.
  Logger::warning("CaesarIA {} has crashed.", GAME_BUILD_NUMBER );
  // Initialize IMAGEHLP.DLL.
  SymInitialize(GetCurrentProcess(), ".", TRUE);

  // Record exception info.
  Logger::warning("Exception: {} (0x{:08x})", ExceptionName(e->ExceptionRecord->ExceptionCode),
                  static_cast<unsigned long>(e->ExceptionRecord->ExceptionCode));
  Logger::warning("Exception Address: 0x{:x}",
                  static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(e->ExceptionRecord->ExceptionAddress)));
  if (e->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION && e->ExceptionRecord->NumberParameters >= 2)
    Logger::warning("Access violation: {} address 0x{:x}",
                    e->ExceptionRecord->ExceptionInformation[0] == 0 ? "reading" : "writing",
                    static_cast<unsigned long long>(e->ExceptionRecord->ExceptionInformation[1]));

  // Record list of loaded DLLs.
  //Logger::warning("DLL information:\n");
  //SymEnumerateModules(GetCurrentProcess(), EnumModules, NULL);

  // Record stacktrace.
  Logger::warning("Stacktrace:");
  Stacktrace(e);
  Logger::warning("Stacktrace end");

  // Unintialize IMAGEHLP.DLL
  SymCleanup(GetCurrentProcess());

  // Cleanup.
  // FIXME: update closing of demo to new netcode

  // Inform user.
  char dir[MAX_PATH];
  GetCurrentDirectory(sizeof(dir) - 1, dir);
  std::string msg = utils::format( 0xff,
    "CaesarIA has crashed.\n\n"
    "A stacktrace has been written to:\n"
    "%s\\stdout.txt", dir);
  OSystem::error( "CaesarIA: Unhandled exception", msg );

  // this seems to silently close the application
  return EXCEPTION_EXECUTE_HANDLER;

  // this triggers the microsoft "application has crashed" error dialog
  //return EXCEPTION_CONTINUE_SEARCH;

  // in practice, 100% CPU usage but no continuation of execution
  // (tested segmentation fault and division by zero)
  //return EXCEPTION_CONTINUE_EXECUTION;
}

#endif

/** Print a demangled stack backtrace of the caller function to FILE* out. */
void printstack( bool showMessage, unsigned int starting_frame, unsigned int max_frames )
{
#if !defined(GAME_PLATFORM_WIN) && !defined(GAME_PLATFORM_ANDROID)
  if (showMessage)
  {
    std::string file = vfs::Directory::applicationDir().getFilePath("stdout.txt").toString();
    std::string msg = fmt::format( "CaesarIA has crashed.\n\n"
                                   "A stacktrace has been written to:\n"
                                   "{}", file );
    OSystem::error( "CaesarIA: Unhandled exception", msg );
  }

  Logger::warning("Stacktrace::begin :");

  // storage array for stack trace address data
  void* addrlist[max_frames+1];

  // retrieve current stack addresses
  int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

  if (addrlen == 0)
  {
    Logger::warning(" <empty, possibly corrupt>\n");
    return;
  }

  // resolve addresses into strings containing "filename(function+address)",
  // this array must be free()-ed
  char** symbollist = backtrace_symbols(addrlist, addrlen);

  // allocate string which will be filled with the demangled function name
  size_t funcnamesize = 256;
  ByteArray funcname(funcnamesize);

  // iterate over the returned symbol lines. skip the first, it is the
  // address of this function.
  for (int i = starting_frame + 1; i < addrlen; i++)
  {
    char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

    // find parentheses and +address offset surrounding the mangled name:
    // ./module(function+0x15c) [0x8048a6d]
    for (char *p = symbollist[i]; *p; ++p)
    {
      if (*p == '(') begin_name = p;
      else if (*p == '+') begin_offset = p;
      else if (*p == ')' && begin_offset)
      {
        end_offset = p;
        break;
      }
    }

    if (begin_name && begin_offset && end_offset
    && begin_name < begin_offset)
    {
      *begin_name++ = '\0';
      *begin_offset++ = '\0';
      *end_offset = '\0';

      // mangled name is now in [begin_name, begin_offset) and caller
      // offset in [begin_offset, end_offset). now apply
      // __cxa_demangle():

      int status;
      char* ret = abi::__cxa_demangle(begin_name, funcname.data(), &funcnamesize, &status);
      if (status == 0)
      {
        funcname = ret; // use possibly realloc()-ed string
        Logger::warning(" {0} : {1}+{2}", symbollist[i], funcname.data(), begin_offset);
      }
      else
      {
        // demangling failed. Output function name as a C function with
        // no arguments.
        Logger::warning(" {0} : {1}+{2}", symbollist[i], begin_name, begin_offset);
      }
    }
    else
    {
      // couldn't parse the line? print the whole line.
      Logger::warning(" {}", symbollist[i]);
    }
  }

  //free(funcname);
  free(symbollist);
#elif defined(GAME_PLATFORM_WIN)
  Logger::warning( "Game: stacltrace finished." );
#else
  Logger::warning("Stack trace not available");

#endif // GAME_PLATFORM_LINUX
}

/** Install crash handler. */

void CrashHandler_handleCrash(int signum)
{
  switch(signum)
  {
    case SIGABRT: Logger::warning("SIGABRT: abort() called somewhere in the program."); break;
    case SIGSEGV: Logger::warning("SIGSEGV: Illegal memory access."); break;
    case SIGILL: Logger::warning("SIGILL: Executing a malformed instruction. (possibly invalid pointer)"); break;
    case SIGFPE: Logger::warning("SIGFPE: Illegal floating point instruction (possibly division by zero)."); break;
  }

  printstack();
  exit(signum);
}

void install()
{
#ifdef GAME_PLATFORM_WIN
  SetUnhandledExceptionFilter(ExceptionHandler);
#else
  signal( SIGABRT, CrashHandler_handleCrash);
  signal( SIGSEGV, CrashHandler_handleCrash);
  signal( SIGILL , CrashHandler_handleCrash);
  signal( SIGFPE , CrashHandler_handleCrash);
#endif
}

/** Uninstall crash handler. */
void remove()
{
#ifdef GAME_PLATFORM_WIN
  SetUnhandledExceptionFilter(NULL);
#endif
}

}//end namespace crashnadler



