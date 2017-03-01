#include <QApplication>
#include <QThread>
#include <QMutex>
#include <QMessageBox>

#include <QColor>
#include <QLabel>
#include <QtDebug>
#include <QString>
#include <QPushButton>

#include "LeptonThread.h"
#include "ImageWidget.h"

int main( int argc, char **argv )
{
	//create the app
	QApplication a( argc, argv );
	
	QWidget optionsWidget;
	optionsWidget.setGeometry(400, 300, 340, 290);

	//create an image widget, and set it's image to the placeholder
	ImageWidget imageWidget;
	imageWidget.setGeometry(0, 0, 320, 240);

	//create a FFC button
	QPushButton button1("Perform FFC", &optionsWidget);
	button1.setGeometry(320/2-50, 290-35, 100, 30);

	//create a thread to gather SPI data
	//when the thread emits updateImage, the image widget should update its image accordingly
	LeptonThread thread;
	QObject::connect(&thread, SIGNAL(updateImage(QImage)), &imageWidget, SLOT(setImage(QImage)));
	
	//connect ffc button to the thread's ffc action
	QObject::connect(&button1, SIGNAL(clicked()), &thread, SLOT(performFFC()));
	thread.start();
	
	optionsWidget.show();
	imageWidget.show();

	return a.exec();
}

