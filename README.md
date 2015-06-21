# e2b
EAGLE 2 Build

A viewer application for opening and viewing Cadsoft EAGLE >v6 files. 
Written in C++ using Qt 5.x.
XML parser is generated from the dtd file provided with the EAGLE with the kode. 
For details see the docs folder.

Main goals:

 - Make the board population by hand easier. (Wizard like walkthrough
   the BoM highlighting the same parts on the board one-by-one).
 - Mirror the board
 - Command line interface for highlighting multiple nets/parts.

Other plans:

 - Gencad support

Heavilly inspired by Teradyne Design 2 Build (D2B).
