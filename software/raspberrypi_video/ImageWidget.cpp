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
	m_aspect(80.0f/60.0f),
	m_pos(0.0f, 0.0f, -3.0f),

	m_imgres(0,0),
	m_imgformat(QImage::Format_Invalid)
{
}

ImageWidget::~ImageWidget()
{
	delete m_program;
	delete m_texture;
}


// Slots

void ImageWidget::setImage(QImage image)
{
	if (this->context() != 0)
	{
		// Check if size and format matches previous image
		if (image.size() == m_imgres && image.format() == m_imgformat)
		{
			this->makeCurrent();
			m_texture->setData(image, QOpenGLTexture::DontGenerateMipMaps);
			this->doneCurrent();
		}
		else
		{
			this->makeCurrent();
			m_texture->destroy();
			m_texture->create();
			m_texture->setData(image, QOpenGLTexture::DontGenerateMipMaps);
			this->doneCurrent();
		}


		this->update();
	}
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

	// Create texture and fill with red image
	QImage red(80, 60, QImage::Format_RGB888);
	red.fill(Qt::red);
	m_texture = new QOpenGLTexture(red, QOpenGLTexture::DontGenerateMipMaps);

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
	QMatrix4x4 projection, scale, rotate, translate;

	// Update projection matrix and other size related settings:
	projection.perspective(m_fov, w / float(h), 0.01f, 10.0f);
	scale.scale(m_scale, m_scale/m_aspect);
	rotate.rotate(m_pitch, 0.0f, 1.0f);
	rotate.rotate(m_yaw, 1.0f, 0.0f);
	translate.translate(m_pos);

	// Set MVP transform
	m_mvp = projection * translate * rotate * scale * m_mvp;

	if (m_program != 0 && m_program->isLinked())
	{
		int location = m_program->uniformLocation("matrix");
		m_program->setUniformValue(location, m_mvp);
	}
}

void ImageWidget::paintGL()
{
	// Clear framebuffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Draw scene
	m_vao.bind();
	m_texture->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Check for errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		qInfo() << "GL error:" << error;
}
