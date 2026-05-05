#include "welcome.h"
#include <QtWidgets/QApplication>
#include "Qtprojecto.h"
#include "login.h"
#include"Qtags.h"
#include "structs.h"

welcome::welcome(QWidget *parent): QWidget(parent)
{

	setWindowTitle("Welcome Page");
	this->setAttribute(Qt::WA_StyledBackground, true);

	// Set welcome image
	image = new QLabel(this);
	QPixmap pix("img/welcome.png");
	image->move( 600, 100);

	image->setPixmap(pix);
	image->setScaledContents(true);
	image->setMinimumSize(600, 600);


	name = new QLabel("Welcome to طيييييير أنت",this);
	name->move(650, 700);
	name->setStyleSheet("font :36pt");
	 
	// Buttons 
	continueBtn = new QPushButton("خش برجلك الشمين ", this);
	continueBtn->setObjectName("continueBtn");
	continueBtn->move(920, 800);
	tagsBtn = new QPushButton("           الصنايعية          ", this);
	tagsBtn->setObjectName("tagsBtn");
	tagsBtn->move(760, 800);
	WapplyStyles();
	
	// Go to login function
	connect(continueBtn, &QPushButton::clicked, this, [=]() {
		Login* loginWin = new Login();
		loginWin->show();
		this->close(); // Close welcome page
		});


	// Go to tags function

	connect(tagsBtn, &QPushButton::clicked, this, [=]() {
		Qtags* tags = new Qtags();
		tags->show();
		this->close();
		});


}

void welcome::WapplyStyles() {
	this->setStyleSheet(

		"welcome { background-color: #020617; }"

        "#continueBtn { "
        "   background-color: transparent; "
        "   color: #94a3b8; "
        "   border: 1px solid #334155; "
        "   padding: 12px; "
        "   border-radius: 8px; "
        "   font-weight: bold; "
        "   margin-top: 10px; "
        "} "
        "#continueBtn:hover { "
        "   background-color: #00C8FF; "
        "   color: white; "
        "   border: 1px solid #00C8FF; "
        "} "

		"#tagsBtn { "
		"   background-color: transparent; "
		"   color: #94a3b8; "
		"   border: 1px solid #334155; "
		"   padding: 12px; "
		"   border-radius: 8px; "
		"   font-weight: bold; "
		"   margin-top: 10px; "
		"} "
		"#tagsBtn:hover { "
		"   background-color: #FF00F7; " 
		"   color: white; "
		"   border: 1px solid #FF00F7; "
		"} "
	);
}