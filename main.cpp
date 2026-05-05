#include "Qtprojecto.h"
#include <QtWidgets/QApplication>
#include "welcome.h"
#include "QtAirport.h"
#include "data_base.h"
#include "structs.h"
#include "functions.h"

int main(int argc, char* argv[])
{
	LOAD_DATA();
	
	QApplication app(argc, argv);
	

	// To LOGIN:
		// Admin:
			// Username: zizo	Password: admin123
		// User:
			// Username: User	Email: user@gmail.com	Password: User813*w
		// Airports:
			// Email: port@gmail.com	Password: Port13*w
			// Email: port2@gmail.com	Password: Port13*w
	// Be careful it's space-sensitive!!!
	// You can login with either username or email!!

	
	welcome w;
	w.show();
	
	SAVE_DATA();

	//تمت بحمد الله 
	//هذا من فضل ربي	
	return app.exec();
}
