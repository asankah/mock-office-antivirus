#pragma once

// From msoav.h:

typedef struct _msoavinfo
{
  int cbsize;                           // == sizeof(MSOAVINFO)
  struct {
    ULONG fPath:1;          // when true use pwzFullPath else use lpstg
    ULONG fReadOnlyRequest:1; //user requests file to be opened read/only
    ULONG fInstalled:1;	    // the file at pwzFullPath is an installed file
    ULONG fHttpDownload:1;  // the file at pwzFullPath is a temp file
                            // downloaded from http/ftp
  };
  HWND hwnd;                // parent window to use for prompting?
  union {
    WCHAR *pwzFullPath; //full path to the file about to be opened
    LPSTORAGE lpstg;    //OLE Storage of the doc about to be opened
  } u;
  WCHAR *pwzHostName;    // Host application name.
  WCHAR *pwzOrigURL;     // URL of the origin of this downloaded file.
} MSOAVINFO;

// {56FFCC30-D398-11d0-B2AE-00A0C908FA49}
DEFINE_GUID(IID_IOfficeAntiVirus,
0x56ffcc30, 0xd398, 0x11d0, 0xb2, 0xae, 0x0, 0xa0, 0xc9, 0x8, 0xfa, 0x49);

// {56FFCC31-D398-11d0-B2AE-00A0C908FA49}
DEFINE_GUID(CATID_MSOfficeAntiVirus,
0x56ffcc30, 0xd398, 0x11d0, 0xb2, 0xae, 0x0, 0xa0, 0xc9, 0x8, 0xfa, 0x49);

// Used with our custom declaration of IOfficeAntiVirus interface below:
[export]
typedef void* MSOAVINFOP;

// IOfficeAntiVirus with attributes.
// (http://msdn.microsoft.com/en-us/library/ms537369(v=vs.85).aspx)
[uuid("56FFCC30-D398-11d0-B2AE-00A0C908FA49"), object, local]
__interface IOfficeAntiVirus : IUnknown {

  // The Scan() method is documented to take a MSOAVINFO*. But due to bit fields
  // having trouble being marshaled, we are passing in a void* instead.  This is
  // a local call, so we can cast the pointer in our implementation.
  STDMETHOD_(HRESULT, Scan)([in] THIS_ MSOAVINFOP pmsoavinfo) PURE;
};
