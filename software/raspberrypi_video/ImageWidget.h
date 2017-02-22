#pragma once


class ImageWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	public:
		ImageWidget(QWidget *parent) : QOpenGLWidget(parent) { }

	protected:
		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();
};
