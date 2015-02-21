AveDesk 1.1 Desklet and Effect SDK
----------------------------------
(c) Andreas Verhoeven 2004

This SDK is includes the Desklet and the Effect SDK for AveDesk 1.1. Included is an HLP (help) file which contains information about every event and function available.

Also included are two examples: A RecycleBin desklet and a Glow Effect. The example files may freely be used.

Files:

DeskletSDK.cpp	- the main source file for the Desklet SDK. Add this to your project if you are making a Desklet.
DeskletSDK.h	- the header file for the DeskletSDK, include this into your project.

DrawPluginSDK.cpp	- the main source file for the Effect SDK. Add this to your project if you are making an effect file.
DrawPluginSDK.h		- the header file for the Effect SDK. include this into your project.

Also included is a experimental (nothing really well tested) C++ templated version of the DeskletSDK. Basically, this should allow a developer to create desklets quicker and easier. Only DeskletImpl.hpp and a copied exports.def hav to be included; this file will include all necessary other files. Take a look at the ExampleDesklet class included in the SDK. I might turn this into a VC6 wizard soon.
 - AveDeskEvents.hpp, used to delegate the C style events to implementations of CDeskletImpl.
 - AveDeskUtil.cpp, implements some commonly used functions as static functions.
 - AveDeskUtil.hpp, idem.
 - DeskletImpl.hpp, implementation of template class CDeskletImpl. This file needs to be included only.
 - DeskletSDK.cpp, used by deskletimpl.hpp to access the C api.
 - DeskletSDK.h, used by deskletimpl.hpp to accesss the C api.
 - exports.def, defaults DEF linkage file. You will need to add this to your project and, if necessary, uncomment some lines.
 - Layers.hpp, Layer helper class.
 - Layers.cpp, idem.
 - Layer.hpp,  idem.
 - Layer.cpp,  idem.

 - ExampleDesklet.hpp, exampledesklet using the CDeskletImpl.
 - ExampleDesklet.cpp, implementation of the exampledesklet.


A Delphi version of this SDK is coming very soon, I am writing some GDI+ wrapper functions currently. Once those are done, I will kick the delphi sdk out.
