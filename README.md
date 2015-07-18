![t6_logo](http://i.imgur.com/xHh9MAY.png)
-------

Torque 6 Editor is an MIT licensed editing suite for the [Torque 6](https://github.com/andr3wmac/Torque6/) engine.

Links
--------

 - [Torque 6](https://github.com/andr3wmac/Torque6/)
 - [Twitter](https://twitter.com/torque6engine)
 - [YouTube](https://www.youtube.com/channel/UCD--TmjTZU9FstD5yg4yKDg)
 - [Forums](http://forums.torque3d.org/viewforum.php?f=32)

Building
--------

Torque 6 Editor uses [GENie](https://github.com/bkaradzic/genie) to generate projects to build the editor and the engine.  The binaries and scripts are found in the build directory. Windows users can use build/GENERATE_VS2010/12/13.BAT to generate Visual Studio projects. 

Torque 6 Editor has a dependancy on wxWidgets. On windows it expects WXWIN environment variable to be defined with a path to the wxWidgets installation. You must build both Release and Debug libs for wxWidgets. For more information see: https://wiki.wxwidgets.org/Install

The engine folder is a git submodule of the main engine repo. You don't have to clone the engine, it will automatically be cloned and updated as a git submodule.
