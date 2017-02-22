#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>


class ImageWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT;

	public:
		ImageWidget(QWidget *parent) : QOpenGLWidget(parent) { }

	public slots:
		void setImage(QImage image);

	protected:
		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();

	private:
		QMatrix4x4 m_projection;
};
