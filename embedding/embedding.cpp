#include <windows.h>
#include <iostream>
#include <string.h>

typedef void (*f_funci)(std::string envName);

int main() {
		HINSTANCE hGetProcIDDLL = LoadLibrary("libintegration.dll");

	  if (!hGetProcIDDLL) {
	    std::cout << "could not load the dynamic library" << std::endl;
	    return 1;
	  }

	  f_funci funci = (f_funci)GetProcAddress(hGetProcIDDLL, "simulate");
	  if (!funci) {
	    std::cout << "could not locate the function" << std::endl;
	    return 1;
	  }

	  funci("Cmdenv");

	  std::cout << "funci() returned " << std::endl;

	  return 0;
}
