# See LICENSE file.

O=obj
LINK=link /nologo
CC=cl /nologo

OBJFILES=\
	$(O)\scoped_log.obj	\
	$(O)\mock_oav_impl.obj

DLLFILE=$(O)\mock_oav_impl.dll

CFLAGS=$(CFLAGS) -DUNICODE -D_UNICODE

all: mkdirs $(DLLFILE)

mkdirs:
!if !exist($(O))
	mkdir $(O)
!endif

$(DLLFILE): $(OBJFILES)
	$(LINK) /DLL /OUT:$@ $**

.cc{$(O)}.obj:
	$(CC) /c /MT $(CFLAGS) /EHsc /Fo$@ $<

clean:
	del $(OBJFILES)
	del $(DLLFILE)

register:
	regsvr32 /i /n $(DLLFILE)

unregister:
	regsvr32 /u $(DLLFILE)
