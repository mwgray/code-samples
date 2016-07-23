// Copyright 2006-13 HumaNature Studios Inc.

#include "PhyreCorePch.h"

#include "HumaNature/Debug/PlatformDebug.h"
#include "HumaNature/Debug/StackTracer.h"

namespace core {


#ifdef __HNS_WIN32__   

    static void LocalDekoPurecallHandler()
    {
        safeAssertEquals(false,"A pure virtual function call has been made - most likely during a constructor or destructor");
    }

    static const char* seDescription( const DWORD& code )
    {
        switch( code ) 
        {
        case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION"         ;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED"    ;
        case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT"               ;
        case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT"    ;
        case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND"     ;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO"       ;
        case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT"       ;
        case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION"    ;
        case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW"             ;
        case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK"          ;
        case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW"            ;
        case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION"      ;
        case EXCEPTION_IN_PAGE_ERROR:            return "EXCEPTION_IN_PAGE_ERROR"            ;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO"       ;
        case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW"             ;
        case EXCEPTION_INVALID_DISPOSITION:      return "EXCEPTION_INVALID_DISPOSITION"      ;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION" ;
        case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION"         ;
        case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP"              ;
        case EXCEPTION_STACK_OVERFLOW:           return "EXCEPTION_STACK_OVERFLOW"           ;
        default: return "UNKNOWN EXCEPTION" ;
        }
    }    

    static LONG WINAPI Win32ExceptionHandler(LPEXCEPTION_POINTERS p)
    {
        // Translate SEH exception to a C++ exception.  NOTE: compiling with /EHa is required!!
        static char msg[256];
        sprintf_s(msg, 256, "Unhandled exception 0x%08x at 0x%08x",
            p->ExceptionRecord->ExceptionCode,
            p->ExceptionRecord->ExceptionAddress);

        StackTracer::Instance().PlatformPrintStack(kFatal, p, 0);

        logError("Halt", msg);
        logFatal("Halt", "Exception [%s]", seDescription(p->ExceptionRecord->ExceptionCode));

        return EXCEPTION_EXECUTE_HANDLER;
    }

    void PlatformDebug::initialize()
    {
        SetUnhandledExceptionFilter(&Win32ExceptionHandler);    

        _set_purecall_handler(&LocalDekoPurecallHandler);
    }
#else 
    void PlatformDebug::initialize()
    {

    }
#endif

} // namespace core