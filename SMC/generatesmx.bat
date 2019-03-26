java -jar %SMC_HOME%/Smc.jar -c++ -cast static_cast -noex -nostreams UserInterface.sm
java -jar %SMC_HOME%/Smc.jar -graph UserInterface.sm
move UserInterfaceFSM.cpp ..\src\app
move UserInterfaceFSM.h ..\include\app