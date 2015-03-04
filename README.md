# Mock Office Antivirus provider

This is a mock implementation of [`IOfficeAntiVirus`](https://msdn.microsoft.com/en-us/library/ie/ms537369%28v=vs.85%29.aspx). Once registered, it will be invoked as a part of the tests performed by [`IAttachmentExecute::Save()`](https://msdn.microsoft.com/en-us/ie/bb776299(v=vs.98)). You can modify it to simulate what would happen if an actual anti-virus product finds an infection during `IAttachmentExecute::Save()`.

Use the provided `Makefile` to build.
