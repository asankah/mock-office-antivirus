// See LICENSE file.

#include "scoped_log.h"
#include <strsafe.h>

ScopedLog::ScopedLog(const wchar_t* filepath)
    : log_file_(INVALID_HANDLE_VALUE) {
  // Create the log file for append.
  log_file_ = CreateFile(filepath,
                         GENERIC_WRITE,
                         0,     // Share mode
                         NULL,  // Security attributes
                         OPEN_ALWAYS,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);
  if (log_file_ != INVALID_HANDLE_VALUE)
    SetFilePointer(log_file_, 0, NULL, FILE_END);
}

ScopedLog::~ScopedLog() {
  if (log_file_ == INVALID_HANDLE_VALUE)
    return;
  CloseHandle(log_file_);
}

void ScopedLog::Append(const wchar_t* format, ...) {
  va_list ap;
  wchar_t log_string[1024];
  if (log_file_ == INVALID_HANDLE_VALUE)
    return;

  va_start(ap, format);
  StringCbVPrintf(log_string, sizeof(log_string), format, ap);
  va_end(ap);

  SYSTEMTIME systime;
  wchar_t log_entry[1040];
  size_t cb = 0;
  DWORD bytes_written = 0;
  GetLocalTime(&systime);
  StringCbPrintf(log_entry, sizeof(log_entry),
                 L"%02d:%02d:%02d.%03d %s\r\n",
                 systime.wHour,
                 systime.wMinute,
                 systime.wSecond,
                 systime.wMilliseconds,
                 log_string);
  StringCbLength(log_entry, sizeof(log_entry), &cb);
  WriteFile(log_file_, log_entry, cb, &bytes_written, NULL);
}
