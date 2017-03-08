#include "ImageWidget.h"

#include <QDebug>

// Constructor

ImageWidget::ImageWidget(QWidget *parent) : QOpenGLWidget(parent),
	m_program(0),
	m_vert(QOpenGLBuffer::VertexBuffer),
	m_uv(QOpenGLBuffer::VertexBuffer),
	m_index(QOpenGLBuffer::IndexBuffer),

	m_fov(45.0f),
	m_pitch(0.0f),
	m_yaw(0.0f),
	m_scale(1.0f),
	m_img_aspect(1.0f),
	m_pos(0.0f, 0.0f),

	m_fb_res(0,0),

	m_imgres(0,0)
{
}

ImageWidget::~ImageWidget()
{
	delete m_program;
}


// Slots

void ImageWidget::setImage(QImage image)
{
	if (context() != 0)
	{
		if (image.format() != QImage::Format_RGB888)
			image = image.convertToFormat(QImage::Format_RGB888);

		// Check if size and format matches previous image
		if (image.size() == m_imgres)
		{
			makeCurrent();
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width(), image.height(), GL_RGB, GL_UNSIGNED_BYTE,
					image.bits());
			doneCurrent();
		}
		else
		{
			makeCurrent();
			glBindTexture(GL_TEXTURE_2D, m_texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width(), image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
					image.bits());
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			m_imgres = image.size();
			m_img_aspect = image.width() / float(image.height());
			doneCurrent();
		}

		update();
	}
}

void ImageWidget::setParameters(float fov, float scale, float pitch, float yaw, QVector2D position)
{
	m_fov = fov;
	m_scale = scale;
	m_pitch = pitch;
	m_yaw = yaw;
	m_pos = position;

	// Update MVP transform
	updateMVP();
}


// Protected

void ImageWidget::initializeGL()
{
	initializeOpenGLFunctions();

	// Set the clear color for the framebuffer
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Configure shader program
	m_program = new QOpenGLShaderProgram();
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
			"attribute highp vec3 vertex;\n"
			"attribute highp vec2 uv;\n"
			"varying highp vec2 tex_coord;\n"
			"uniform highp mat4 matrix;\n"
			"void main(void)\n"
			"{\n"
			"   gl_Position = matrix * vec4(vertex, 1.0f);\n"
			"   tex_coord = uv;\n"
			"}");
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
			//"uniform mediump vec4 color;\n"
			"varying highp vec2 tex_coord;\n"
			"uniform sampler2D texture;\n"
			"void main(void)\n"
			"{\n"
			"   gl_FragColor = texture2D(texture, tex_coord);\n"
			"}");
	m_program->link();
	qInfo() << "isLinked()" << m_program->isLinked();
	qInfo() << "log()" << m_program->log();
	m_program->bind();

	// Configure texture and fill with red image
	QImage red(80, 60, QImage::Format_RGB888);
	red.fill(Qt::red);
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, red.width(), red.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, red.bits());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Configure vertex data
	m_vert.create();
	{
		GLfloat data[12] = {
			-1.0f, -1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			-1.0f, 1.0f, 0.0f
			};
		m_vert.bind();
		m_vert.allocate(&data, 12*sizeof(GLfloat));
	}
	m_uv.create();
	{
		GLfloat data[8] = {
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			};
		m_uv.bind();
		m_uv.allocate(&data, 8*sizeof(GLfloat));
	}
	m_index.create();
	{
		GLuint data[6] = {
			0, 1, 2,
			0, 2, 3
			};
		m_index.bind();
		m_index.allocate(&data, 6*sizeof(GLuint));
	}

	// Create and bind VAO
	m_vao.create();
	m_vao.bind();

	// Set up array attrib pointer to vertex data
	int location;
	m_vert.bind();
	location = m_program->attributeLocation("vertex");
	glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	m_uv.bind();
	location = m_program->attributeLocation("uv");
	glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);

	// Set up index array
	m_index.bind();
}

void ImageWidget::resizeGL(int w, int h)
{
	// Store new FB resolution
	m_fb_res = QSize(w, h);

	// Update MVP transform
	updateMVP();
}

void ImageWidget::paintGL()
{
	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw scene
	m_vao.bind();
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Check for errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		qInfo() << "GL error:" << error;
}


void ImageWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
	// Toggle fullscreen mode
	if (isFullScreen())
		showNormal();
	else
		showFullScreen();
}


// Private
void ImageWidget::updateMVP()
{
	QMatrix4x4 projection, scale, rotate, translate;

	// Update projection matrix and other size related settings:
	projection.perspective(m_fov, m_fb_res.width() / float(m_fb_res.height()), 0.01f, 10.0f);
	scale.scale(m_scale, m_scale/m_img_aspect);
	rotate.rotate(m_pitch, 0.0f, 1.0f);
	rotate.rotate(m_yaw, 1.0f, 0.0f);
	translate.translate(m_pos.x(), m_pos.y(), -3.0f);

	// Set MVP transform
	m_mvp = projection * translate * rotate * scale;
	int location = m_program->uniformLocation("matrix");
	m_program->setUniformValue(location, m_mvp);
}
