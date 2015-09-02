# Thud

## Wymagania
* C++11 (gcc 4.7 i nowsze) W systemie Windows dobrze działa [tdm-gcc](http://tdm-gcc.tdragon.net/).
* [CMake](http://www.cmake.org/) (wersja 2.8 lub nowsza)
* [wxWidgets](https://www.wxwidgets.org/) (wersja 2.8 lub nowsza)
* [OpenSceneGraph](http://www.openscenegraph.org/) (wersja 3.2.2). Powinna zawierać OpenThreads.
* [TinyXML2](http://www.grinninglizard.com/tinyxml2/) (wersja 2.6.2)
* [OpenCL](https://www.khronos.org/opencl/) (wersja 1.1) Należy pamiętać o zainstalowaniu implementacji w zależności od posiadanego sprzętu.
* OpenGL (zazwyczaj dostarczane razem ze sterownikami do karty graficznej)

## Kompilacja
CMake powinien posiadać skrypty wyszukiwania dla wszystkich bibliotek z wyjątkiem TinyXML2, do której skrypt znajduje się w katalogu CMakeModules i jest automatycznie ładowany przez CMakeLists.txt.

Najwygodniej jest uruchomić `cmake-gui` i po ustawieniu ścieżki do kodu naciśnąć `Configure`. Ewentualne brakujące ścieżki można wtedy podać w gui.

## Uruchomienie
Plikiem wynikowym jest Thud(.exe). Do uruchomienia nie potrzebne są żadne parametry, ale należy pilnować, żeby pliki **CollisionWithJumps.cl**, **gui.xrc**, **scenes.xml** oraz katalog **models** znajdowały się w katalogu z którego wywołujemy program.
