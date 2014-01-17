jni
===
Project description:
integration - the main dll library project providing JNI funictionalities
model - a dll project containing simulation specific classes
embedding - a c++ exe project for testing functionality of the integration dll (requires modification of the simulation.cpp)
test - a Java project for testing functionality of the integration dll

All c++ dll projects require setting up includes, libraries and libraries path (can be done in Properties > C/C++ General > Paths and Symbols) libraries required are all dll from the OMNET++\bin directory that names ends with letter "d" (oppcmdenvd, oppsimd...). Bare in mind that libraries names given there are given without the "lib" prefix and the extension. In the locations please povide the paths where those libraries can be found.
Additionally the model project requires the library and the path to inet (debug version) of the library. This can be found in the (inet project)\out\gcc-debug\src directory after building the inet project.

Both model and integration require appropriate includes, which are OMnet++\include directory and inet relevant directories. Those are:

"D:/Workspaces/OMNeT++/Workspace/inet/src/linklayer/ethernet/switch"
"D:/Workspaces/OMNeT++/Workspace/inet/src/linklayer/ethernet"
"D:/Workspaces/OMNeT++/Workspace/inet/src/linklayer/queue"
"D:/Workspaces/OMNeT++/Workspace/inet/src/base"
"D:/Workspaces/OMNeT++/Workspace/inet/src/linklayer/contract"

