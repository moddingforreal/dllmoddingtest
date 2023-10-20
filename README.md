# DLLMODDINGTEST
## About
What this program does is try to load in DLLs (windows) or SOs (linux) from a list of filenames in libs.txt .
It will try to find a specific function in every library and try to create an array of function pointers.
Then it runs through the array and executes the function pointers.

The purpose of the program is to be a test to show how a modding-API-type thing could be made for a C game or generally a C program.
For example, have a function of the same name in every library, the main program will find this "entry point" function.
It should probably provide a version and also a list of other functions it wants to be run on certain events.
The main game then would load the functions into a function pointer array and loop through the arrays for their respective events.

This way the main program can be standalone but also accept "mods", if the mods are properly done.
This is what I will probably use for a modding system for textrpg-c (I will then release also a modding kit containing headers and instructions to make mods).

Since the way for loading windows and linux libraries dynamically like this is different, I use the LoadLibrary functions from the windows.h header for the windows part, 
and the dlopen/dlsym/dlclose functions from the dlfcn.h header for the linux part