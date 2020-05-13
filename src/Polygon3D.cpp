#include "Polygon3D.h"

namespace NAMESPACE_RENDERING
{
	void Polygon3D::initVBO()
	{
		float* vertexBuffer = getVertexBuffer();

		glGenBuffers(1, &vertexBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, vertexes.count * sizeof(Vec3f) + vertexes.count * sizeof(Vec2f), vertexBuffer, GL_STATIC_DRAW);

		projectionMatrixLocation = glGetUniformLocation(programShader, "projection");
		viewMatrixLocation = glGetUniformLocation(programShader, "view");
		transformMatrixLocation = glGetUniformLocation(programShader, "modelView");
		colorLocation = glGetUniformLocation(programShader, "Color");
		textureEnableLocation = glGetUniformLocation(programShader, "isTextureEnabled");

		positionAttribute = glGetAttribLocation(programShader, "Position");
		textureAttribute = glGetAttribLocation(programShader, "Texture");

		setUpPositionAttribute();
		setUpTextureAttribute();
	}

	void Polygon3D::setUpPositionAttribute()
	{
		glVertexAttribPointer(positionAttribute,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			0);
		glEnableVertexAttribArray(positionAttribute);
	}

	void Polygon3D::setUpTextureAttribute()
	{
		glVertexAttribPointer(textureAttribute,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)(sizeof(float) * vertexes.count * 3));
		glEnableVertexAttribArray(textureAttribute);
	}

	float* Polygon3D::getVertexBuffer()
	{
		int* indexes = vertexes.findExtremePointsAlongAxisXYZ();

		Vec3f minX = vertexes.points[indexes[0]];
		Vec3f maxX = vertexes.points[indexes[1]];
		Vec3f minY = vertexes.points[indexes[2]];
		Vec3f maxY = vertexes.points[indexes[3]];
		Vec3f minZ = vertexes.points[indexes[4]];
		Vec3f maxZ = vertexes.points[indexes[5]];

		float deltaX = -(minX[0] - maxX[0]);
		float deltaY = -(minY[1] - maxY[1]);
		float deltaZ = -(minZ[2] - maxZ[2]);

		//Log::info("Bouding Box Min(" + StringHelper::toString(minX.x) + " , " + StringHelper::toString(minY.y) + " , " + StringHelper::toString(minZ.z) + ")");
		//Log::info("Bouding Box Max(" + StringHelper::toString(maxX.x) + " , " + StringHelper::toString(maxY.y) + " , " + StringHelper::toString(maxZ.z) + ")");

		sp_float* buffer = new sp_float[vertexes.count * 3 + vertexes.count * 2];
		sp_size bufferIndex = 0;

		for (sp_uint i = 0; i < vertexes.count; i++)
		{
			Vec3f point = vertexes.points[i];

			buffer[bufferIndex] = point[0];
			buffer[bufferIndex + 1] = point[1];
			buffer[bufferIndex + 2] = point[2];

			//Log::info("Point (" + StringHelper::toString(point.x) + " , " + StringHelper::toString(point.y) + " , " + StringHelper::toString(point.z) + ")");

			bufferIndex += 3;
		}

		for (sp_uint i = 0; i < vertexes.count; i++)
		{
			Vec3f texturePoint = vertexes.points[i];

			if (texturePoint[0] - minX[0] == 0.0f)
				buffer[bufferIndex] = 0.0f;
			else
				buffer[bufferIndex] = (texturePoint[0] - minX[0]) / deltaX;

			if (texturePoint[2] - minZ[2] == 0.0f)
				buffer[bufferIndex + 1] = 0.0f;
			else
				buffer[bufferIndex + 1] = (texturePoint[2] - minY[2]) / deltaZ;

			//Log::info("Texture Point (" + StringHelper::toString(buffer[bufferIndex]) + " , " + StringHelper::toString(buffer[bufferIndex + 1]) + ")");

			bufferIndex += 2;
		}

		return buffer;
	}

	void Polygon3D::init()
	{
		std::string vertexShaderSource = "#version 300 es \n"
			"uniform mat4 projection; \n"
			"uniform mat4 view; \n"
			"uniform mat4 modelView; \n"
			"uniform vec4 Color; \n"
			"uniform int  isTextureEnabled; \n"

			"in  vec3 Position; \n"
			"in  vec2 Texture;         \n"
			"out vec4 fragmentColor; \n"
			"out vec2 fragmentTexture;         \n"
			"flat out int  fragmentIsTextureEnabled; \n"

			"void main() \n"
			"{																	 \n"
			"	gl_Position = projection * view * modelView * vec4(Position, 1.0);      \n"
			"	fragmentColor = Color;											 \n"
			"	fragmentTexture = Texture;											 \n"
			"	fragmentIsTextureEnabled = isTextureEnabled;											 \n"
			"}																	 \n";

		std::string fragmentShaderSource = "#version 300 es \n"
			"precision mediump float; \n"

			"uniform sampler2D tex; \n"

			"in  vec4 fragmentColor; \n"
			"in  vec2 fragmentTexture; \n"
			"flat in  int fragmentIsTextureEnabled; \n"

			"out vec4 FragColor; \n"

			"void main() \n"
			"{\n"
			"		if ( fragmentIsTextureEnabled == 1 ) \n"
			"			FragColor = texture(tex, fragmentTexture) * fragmentColor; \n"
			"		else															 \n"
			"			FragColor = fragmentColor; \n"
			"}";

		programShader = Shader::loadShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());

		initVBO();
	}

	void Polygon3D::renderBorder(RenderData renderData)
	{
		glUniform4f(colorLocation, borderColor->Red, borderColor->Green, borderColor->Blue, borderColor->Alpha);

		float currentLineWidth = GLConfig::getGLfloat(GL_LINE_WIDTH);
		glLineWidth(borderWidth);

		glDrawElements(GL_LINE_STRIP, facesSize, GL_UNSIGNED_BYTE, faces);

		glLineWidth(currentLineWidth);
	}

	void Polygon3D::render(RenderData renderData)
	{
		glUseProgram(programShader);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBindTexture(GL_TEXTURE_2D, textureBuffer);

		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, renderData.projectionMatrix);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, renderData.viewMatrix);
		glUniformMatrix4fv(transformMatrixLocation, 1, GL_FALSE, transform);
		glUniform4f(colorLocation, color->Red, color->Green, color->Blue, color->Alpha);
		glUniform1i(textureEnableLocation, isTextureEnabled() ? 1 : 0);

		setUpPositionAttribute();
		setUpTextureAttribute();

		glDrawElements(GL_TRIANGLES, facesSize, GL_UNSIGNED_BYTE, faces);

		if (borderWidth > 0.0f)
			renderBorder(renderData);
	}

	void Polygon3D::setUpImage(std::string filename)
	{
		Image *image = ImageFactory::load(filename.c_str());
		sp_uchar* pixels = image->data();
		GLsizei width = image->width();
		GLsizei height = image->height();
		GLenum colorFormat = image->getColorFormat();

		glGenTextures(1, &textureBuffer);
		glBindTexture(GL_TEXTURE_2D, textureBuffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//set texture border color
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

		glTexImage2D(GL_TEXTURE_2D, //target
			0, //level
			colorFormat, //GL_RGB, //internalFormat
			width, //width,
			height, //height,
			0, //border
			colorFormat, //format GL_RGB  or GL_RGBA (format of data read from textel file) (GL_RGB for *.bmp files and GL_RGBA for *.rgb files) GL_ALPHA GL_RGB GL_RGBA GL_LUMINANCE GL_LUMINANCE_ALPHA
			GL_UNSIGNED_BYTE, //type , GL_FLOAT
			pixels); //const void *pixels

		delete image;
	}

	std::string Polygon3D::getTexture()
	{
		return textureFilename;
	}
	void Polygon3D::setTexture(std::string filename)
	{
		setUpImage(filename);
		textureFilename = filename;
		isTextureEnabledValue = true;
	}

	bool Polygon3D::isTextureEnabled()
	{
		return isTextureEnabledValue;
	}
	void Polygon3D::setTextureEnabled(bool isEnabled)
	{
		isTextureEnabledValue = isEnabled;
	}

	Polygon3D::~Polygon3D()
	{
		if (borderColor != nullptr)
			delete borderColor;
	}
}