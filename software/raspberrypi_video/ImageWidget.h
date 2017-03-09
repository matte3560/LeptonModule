#pragma once

// GL related includes
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QVector2D>

// Image related includes
#include <QImage>
#include <QSize>


class ImageWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT;

	public:
		ImageWidget(QWidget *parent = NULL);
		~ImageWidget();

	public slots:
		void setImage(QImage image);
		void setParameters(double fov, double scale, double pitch, double yaw, QVector2D position);

	protected:
		// OpenGL
		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();

		// Window fullscreen
		void mouseDoubleClickEvent(QMouseEvent* event);

	private:
		void updateMVP();

	private:
		// Variables related to OpenGL rendering
		QOpenGLShaderProgram *m_program;
		QOpenGLVertexArrayObject m_vao;
		QOpenGLBuffer m_vert;
		QOpenGLBuffer m_uv;
		QOpenGLBuffer m_index;
		GLuint m_texture;
		QMatrix4x4 m_mvp;

		// Parameters
		float m_fov;
		float m_pitch, m_yaw;
		float m_scale;
		float m_img_aspect;
		QVector2D m_pos;
		QSize m_fb_res;

		// Image parameters
		QSize m_imgres;
};
