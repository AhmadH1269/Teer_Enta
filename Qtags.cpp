#include "Qtags.h"
#include "welcome.h"
Qtags::Qtags(QWidget *parent) : QWidget(parent)
{
	this->setWindowTitle("Tags_page");
	this->setMinimumSize(800, 600);
	this->setStyleSheet("background-color : #020617;");

	layout = new QVBoxLayout(this);
	layout->setContentsMargins(40, 40, 40, 40);


	//table
	names_table = new QTableWidget(0, 2);
	names_table->setHorizontalHeaderLabels({ "Name", "AKA" });
	names_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // for organize the columns
	names_table->verticalHeader()->setVisible(false); // for hide the vertical side num
	names_table->setEditTriggers(QAbstractItemView::NoEditTriggers); // to make it read only

	names_table->setStyleSheet(
		"QTableWidget { background-color: #0f172a; color: white; border-radius: 12px; border: none; }"
		"QHeaderView::section { background-color: #1e293b; color: #94a3b8; padding: 15px; font-weight: bold; border: none; }"
		//"QTableWidget::item { padding: 15px; border-bottom: 1px solid #1e293b; }"
	);

	// function for add new row in the table
	auto addRow = [&](QString name, QString AKA) {
		int r = names_table->rowCount();
		names_table->insertRow(r);
		
		names_table->setRowHeight(r, 60);

		QFont font = names_table->font();
		font.setPointSize(14);
		font.setBold(true);

		QTableWidgetItem* nameItem = new QTableWidgetItem(name);
		QTableWidgetItem* AKAItem = new QTableWidgetItem(AKA);

		nameItem->setFont(font);
		AKAItem->setFont(font);

		names_table->setItem(r, 0, nameItem);
		names_table->setItem(r, 1, AKAItem);
		};
	
	addRow("Eyad Salah", " المعلم صاحب السرفر ");
	addRow("Ahmad Medhat"," الحج بابا التيم ");
	addRow("Nour Eldin Mahmoud", "بسيوني");
	addRow("Youssef Ahmed", " الباشمهندزز جلهوم ");
	addRow("Moataz Ahmed", " ابو عيون ملونة");
	addRow("Ziad Mohamed", "الجنجر");
	addRow("Ziad Badawy", "بضوي");

	layout->addWidget(names_table);

	BackBtn = new QPushButton("لف و ارجع تاني ");
	BackBtn->setObjectName("BackBtn");
	BackBtn->setStyleSheet(
		"#BackBtn { "
		        "   background-color: transparent; "
		        "   color: #94a3b8; "
		        "   border: 1px solid #334155; "
		        "   padding: 12px; "
		        "   border-radius: 8px; "
		        "   font-weight: bold; "
		        "   margin-top: 10px; "
		        "} "
		        "#BackBtn:hover { "
		        "   background-color: #00C8FF; " 
		        "   color: white; "
		        "   border: 1px solid #00C8FF; "
		        "} "
	);
	layout->addWidget(BackBtn);


	connect(BackBtn, &QPushButton::clicked, this, [=]() {
				welcome* welcomepage = new welcome();
				welcomepage->show();
				this->close();
				}
	);
}



