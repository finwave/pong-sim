/**
 * main.cpp source file
 * 
 * Entry point for the application
 */

#include "TheApp.h"

int main(int argc, char** argv)
{
	CTheApp app;
	app.Create();

	int retval = app.Run();
	return retval;
}