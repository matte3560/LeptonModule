#include <QApplication>
#include <QtDebug>

// Widgets
#include <QQuickWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QUrl>
#include <QPushButton>

// Local
#include "LeptonThread.h"
#include "ImageWidget.h"

int main( int argc, char **argv )
{
	//create the app
	QApplication a( argc, argv );

	// Create layout to contain options widgets
	QVBoxLayout* optionsLayout = new QVBoxLayout;

	// Add projection settings to options layout
	QQuickWidget* projectionSettings = new QQuickWidget(QUrl("projection.qml"));
	projectionSettings->setResizeMode(QQuickWidget::SizeRootObjectToView);
	optionsLayout->addWidget(projectionSettings);

	//create a FFC button
	QPushButton* button1 = new QPushButton("Perform FFC");
	optionsLayout->addWidget(button1);

	//create an image widget for the thermal images
	ImageWidget imageWidget;

	//create a thread to gather SPI data
	//when the thread emits updateImage, the image widget should update its image accordingly
	LeptonThread thread;
	QObject::connect(&thread, SIGNAL(updateImage(QImage)), &imageWidget, SLOT(setImage(QImage)));
	
	//connect ffc button to the thread's ffc action
	QObject::connect(button1, SIGNAL(clicked()), &thread, SLOT(performFFC()));
	thread.start();

	// Create widget for options
	QWidget optionsWidget;
	optionsWidget.setLayout(optionsLayout);
	
	optionsWidget.show();
	imageWidget.show();

	return a.exec();
}

