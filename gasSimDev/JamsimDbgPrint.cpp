

#include "stdarg.h"
#include "string.h"
#include "stdint.h"
#include "stdio.h"
#include "JamsimDbgPrint.h"

JamsimDbgPrint::JamsimDbgPrint() 
{
    clear();
}    



void JamsimDbgPrint::printf(const char *format, ...)
{
    int written;
    va_list args;
    va_start(args, format);
    written = vsnprintf(_buffer, HALF_BUFFER_SIZE, format, args);
    va_end(args);
    if (_buffer + written > _bufferStart + HALF_BUFFER_SIZE)    { // time to wrap
        int overflow = (_buffer + written) - (_bufferStart + HALF_BUFFER_SIZE); 
        memmove(_bufferStart, _bufferStart + HALF_BUFFER_SIZE, overflow);
        _buffer = _bufferStart + overflow;
    } 
    else {
        _buffer += written;
    }
}

void JamsimDbgPrint::clear() 
{
    _buffer = _bufferStart;
    *_buffer = 0;

}
