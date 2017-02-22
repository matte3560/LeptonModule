#include "ImageWidget.h"

// Slots

void ImageWidget::setImage(QImage image) // TODO: Copy image to GL texture
{
}


// Protected

void ImageWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void ImageWidget::resizeGL(int w, int h)
{
	// Update projection matrix and other size related settings:
	m_projection.setToIdentity();
	m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);
}

void ImageWidget::paintGL()
{
	// Draw the scene
	glClear(GL_COLOR_BUFFER_BIT);
}
