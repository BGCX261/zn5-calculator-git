#include <ZApplication.h>
#include "mZWidget.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	ZApplication *app = new ZApplication(argc,argv);
	mZWidget *mzw = new mZWidget();
	mzw->show();
	app->exec();

	delete mzw;
	delete app;

	return 0;
}

