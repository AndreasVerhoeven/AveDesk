AveDesk SDK For AveDesk 1.3
===========================
(c) Copyright Andreas Verhoeven 2005


This SDK includes all files to create AveDesk Desklet SDKs.


NEW DOCUMENTATION
=================
Previous versions of this SDK included a HLP file describing
all functions and events. For version 1.3, this HLP file was 
not updated, but instead all functions and events are commented
in doxygen / javadoc style.
For example:

/**
 * Gets AveDesk's version.
 * @return the version number of the current AveDesk host-application.
 * @remark the version number is defined as major * 100 + minor * 10 + subversion;
 */
int CALLBACK DeskletGetAveDeskVersion();


describing what the function does, the parameters, the return values and other
things such as remarks.


You can simply run your favorite doxygen / javadoc style on:
 - deskletevents_doxygen.cpp
 - avefuncs_doxygen.cpp 

to generate a nice HTML overview of those functions and comments.



DESKLET EXAMPLES
================
3 examples are included in this SDK under Samples as Visual C 6 Projects. You'll need to have an up-to-date Platform SDK to compile the examples.

- FlippyNotes
	A  simple desklet that uses AveControls and the layers interface. All values are hardcoded. Shows how to setup a simple desklet that has 2 sides and flips. Also shows how to use AveControls to be able to handle text-input.

- WordSeacher
	A desklet which source code is commented as a tutorial. Shows how to have a desklet with 2 different sizes (collapsed, expanded), how to do a sliding animation, how to use AveControls.

- Translator
	A more advanced desklet that uses Skins and implements a skin-enumerator. Shows how to use multiple textfields controls, how to flip between 2 sides and how to implement skins and a skin-enumerator.

EFFECT EXAMPLES
===============

- GLowEffect
	A simple effect that shows how to make an icon glow on mouse-over. Also shows how to implement properties.
