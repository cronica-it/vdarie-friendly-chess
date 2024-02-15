# Friendly Chess

This project is an archive of the **Friendly Chess** program by Viorel Darie.

## Eclipse

The project includes an Eclipse configuration to build it with MinGW GCC.

To use it:

- install a MinGW GCC (for example [xPack GCC](https://github.com/xpack-dev-tools/gcc-xpack/releases))
- download Eclipse CDT and install it
- clone the project form GitHub, the `master` branch
- import the project into Eclipse
- configure the MINGW_HOME path, in both Debug & Release configurations
    (project Properties -> C/C++ Build -> Environment)

To launch debug or run sessions, use the available launchers.

## Note

For unknown reasons, the program fails to display the menu line; to terminate it, guess the top right location of the close button.
