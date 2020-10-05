set cur=%0\..
clang++ ../main.cpp -o ../../Compiled/Windows/Fractals.exe -DBU_SDL -DBU_WINDOWS -DBU_APP_NAME=\"Fractals\" -DBU_DESKTOP -std=gnu++17 ../Bibliotekum_Ultimatum/Binaries/SDL_win/SDL2main.lib ../Bibliotekum_Ultimatum/Binaries/SDL_win/SDL2.lib -lopengl32 -lshell32 -m32 -O3 -Xlinker /subsystem:windows
if exist "%cur%\..\..\Compiled\Windows\Fractals_resources" rmdir /S /Q %cur%\..\..\Compiled\Windows\Fractals_resources
robocopy %cur%\..\Fractals_resources %cur%\..\..\Compiled\Windows\Fractals_resources /E
