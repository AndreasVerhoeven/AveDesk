
#pragma comment(linker, "/MERGE:.rdata=.text") // merge all into one section
#pragma comment(linker, "/MERGE:.text=.text")
#pragma comment(linker, "/MERGE:.reloc=.text")
#pragma comment(linker, "/OPT:NOWIN98") // makes it even smaller!
#pragma optimize("gsy", on) // smallest

int __stdcall GetAveDeskVersion()
{
	return 130;
}