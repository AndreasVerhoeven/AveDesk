
AveDeskShellExtps.dll: dlldata.obj AveDeskShellExt_p.obj AveDeskShellExt_i.obj
	link /dll /out:AveDeskShellExtps.dll /def:AveDeskShellExtps.def /entry:DllMain dlldata.obj AveDeskShellExt_p.obj AveDeskShellExt_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del AveDeskShellExtps.dll
	@del AveDeskShellExtps.lib
	@del AveDeskShellExtps.exp
	@del dlldata.obj
	@del AveDeskShellExt_p.obj
	@del AveDeskShellExt_i.obj
