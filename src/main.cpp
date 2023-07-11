





#include "Application.h"
#include "pgr.h"
#include "Windows.h"


int main( int argc, char ** argv )
{

	if (!Application::Init(argc, argv) )
	{
		return EXIT_FAILURE; 
	}
	if (!Application::StartMainLoop())
	{
		return EXIT_FAILURE; 
	};


	return EXIT_SUCCESS; 
}