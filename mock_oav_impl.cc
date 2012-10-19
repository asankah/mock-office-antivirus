// See LICENSE file.

#define UNICODE 1
#define _UNICODE 1
#define AVVENDOR

#include <windows.h>

#define _ATL_ATTRIBUTES
#include <atlbase.h>
#include <atlcom.h>

#include <InitGuid.h>
#include "msoav_with_attributes.h"
#include "scoped_log.h"

using namespace ATL;

namespace {

const WCHAR* BoolToString(unsigned long value) {
  return value ? L"true" : L"false";
}

const WCHAR* PrintableString(const WCHAR* str) {
  return str ? str : L"(null)";
}

}

#if defined(WIN64)
#error Not for x64
#endif

[module(dll, name="MockOAVImpl", uuid="4F86DF74-E13F-41FE-A1F8-7078022AB9B0")];
[coclass, progid("MockOAVImpl.1"), vi_progid("MockOAVImpl"),
 implements_category("CATID_MSOfficeAntiVirus"),
 uuid("9EF58E5C-9837-46EE-AE33-A490253D229D"),
 threading(both)
]
class CMockOAVImpl : public IOfficeAntiVirus
{
 public:
  CMockOAVImpl() {}

  // Implementation of Scan(). Logs some stuff and invokes HandleScanRequest()
  // to do the actual work.
  STDMETHOD_(HRESULT,Scan)(MSOAVINFOP infop) {
    MSOAVINFO* info = reinterpret_cast<MSOAVINFO*>(infop);

    // Creates the log in the current directory. You probably want an absolute
    // path or write to the temp folder.
    ScopedLog log(L"mock_oav_impl.log");

    log.Append(L"Begin request");
    log.Append(L"fPath=%s, fReadOnlyRequest=%s, "
               L"fInstalled=%s, fHttpDownload=%s",
               BoolToString(info->fPath),
               BoolToString(info->fReadOnlyRequest),
               BoolToString(info->fInstalled),
               BoolToString(info->fHttpDownload));

    if (!info->fPath) {
      return S_OK;
    }

    log.Append(L"Path=%s", PrintableString(info->u.pwzFullPath));
    log.Append(L"Hostname=%s", PrintableString(info->pwzHostName));
    log.Append(L"Origin=%s", PrintableString(info->pwzOrigURL));

    HRESULT hr = HandleScanRequest(info);
    log.Append(L"Returning hr=%08X", hr);

    return hr;
  }

 private:
  // Handler implementation. Returns errors based on filename for testing.
  HRESULT HandleScanRequest(const MSOAVINFO* info) {
    const WCHAR* path = info->u.pwzFullPath;
    if (path == NULL)
      return S_OK;

    if (wcsstr(path, L".false") != NULL)
      return S_FALSE;

    if (wcsstr(path, L".fail") != NULL)
      return E_FAIL;

    if (wcsstr(path, L".notfound") != NULL)
      return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);

    if (wcsstr(path, L".access") != NULL)
      return E_ACCESSDENIED;
    
    if (wcsstr(path, L".badexe") != NULL)
      return HRESULT_FROM_WIN32(ERROR_BAD_EXE_FORMAT);

    if (wcsstr(path, L".blocked") != NULL)
      return INET_E_SECURITY_PROBLEM;

    return S_OK;
  }
};
