#pragma once

// GL related includes
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector3D>


class ImageWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT;

	public:
		ImageWidget(QWidget *parent);
		~ImageWidget();

	public slots:
		void setImage(QImage image);

	protected:
		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();

	private:
		// Variables related to OpenGL rendering
		QOpenGLShaderProgram *m_program;
		QOpenGLVertexArrayObject m_vao;
		QOpenGLBuffer m_vbo;
		QOpenGLBuffer m_index;
		QOpenGLTexture *m_texture;
		QMatrix4x4 m_mvp;

		// Parameters
		float m_fov;
		float m_pitch, m_yaw;
		float m_scale;
		float m_aspect;
		QVector3D m_pos;
};
