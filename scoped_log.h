// See LICENSE file.

#include <windows.h>

class ScopedLog {
 public:
  // Creates a log at |filepath|.
  ScopedLog(const wchar_t* filepath);
  ~ScopedLog();

  // Appends an entry to the log. The entry begins with a timestamp.
  void Append(const wchar_t* format, ...);

 private:
  HANDLE log_file_;
};
