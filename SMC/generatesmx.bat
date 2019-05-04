@echo OFF
call setenv.bat
@echo ON
java -jar %SMC_HOME%/Smc.jar -c++ -noex -nocatch -nostreams -cast static_cast UserInterface.sm
@echo OFF
java -jar %SMC_HOME%/Smc.jar -graph -glevel 1 UserInterface.sm
%GRAPHVIZ_HOME%/dot.exe -Tpng -o ui.png .\UserInterfaceFSM.dot
move UserInterfaceFSM.cpp ..\src\app
move UserInterfaceFSM.h ..\include\app
pause