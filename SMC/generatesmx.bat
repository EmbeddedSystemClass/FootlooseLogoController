java -jar %SMC_HOME%/Smc.jar -c++ -noex -nocatch -nostreams -cast static_cast UserInterface.sm
java -jar %SMC_HOME%/Smc.jar -graph -glevel 1 UserInterface.sm
dot -Tpng -o ui.png .\UserInterfaceFSM.dot
move UserInterfaceFSM.cpp ..\src\app
move UserInterfaceFSM.h ..\include\app
pause