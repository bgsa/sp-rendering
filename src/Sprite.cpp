#include "Sprite.h"

namespace NAMESPACE_RENDERING
{
	SpriteShaderAttributes spriteAttributes = SpriteShaderAttributes{
		{ //position 
			0.f, 0.f, //bottom-left
			0.f, 1.f, //top-left
			1.f, 0.f, //bottom-right
			1.f, 1.f, //top-right
		},
		{ //color 
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f
		},
		{ //texture (u,v)
			0.0f, 0.0f,  // bottom-left
			0.0f, 1.0f,  // top-left
			1.0f, 0.0f,  // bottom-right
			1.0f, 1.0f   // top-right
		}
	};

	void Sprite::loadSpriteAnimation()
	{
		SP_FILE file;
		file.open("resources/sprites/sonic.xml", std::ios::in);
		const sp_size fileSize = file.length();
		sp_char* spriteXml = ALLOC_ARRAY(sp_char, fileSize);
		file.read(spriteXml, fileSize);
		file.close();

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_string(spriteXml);

		if (result.status != pugi::status_ok)
			Log::error("Error loading XML file!");

		pugi::xml_node spriteAnimationsNode = doc.root().child("SpriteAnimation");;
		pugi::xml_node framesNode = spriteAnimationsNode.child("Frames");
		pugi::xml_node animationsNode = spriteAnimationsNode.child("Animations");

		int frameIndex = 0;
		for (pugi::xml_node frameNode : framesNode.children("Frame"))
		{
			float positionX = frameNode.attribute("positionX").as_float();;
			float positionY = frameNode.attribute("positionY").as_float();;
			float width = frameNode.attribute("width").as_float();;
			float height = frameNode.attribute("height").as_float();;

			spriteData.frames[frameIndex].posisionX = positionX;
			spriteData.frames[frameIndex].positionY = positionY;
			spriteData.frames[frameIndex].width = width;
			spriteData.frames[frameIndex].height = height;

			frameIndex++;
		}

		for (pugi::xml_node animationNode : animationsNode.children("Animation"))
		{
			std::string name = std::string(animationNode.attribute("name").value());
			std::string text = std::string(animationNode.attribute("indexes").value());
			std::vector<std::string> textSplited = StringHelper::split(text, ",");
			std::vector<int> indexes;

			for (size_t i = 0; i < textSplited.size(); i++)
			{
				int value = atoi(textSplited[i].c_str());
				indexes.push_back(value);
			}

			spriteData.animations.insert(std::pair<std::string, std::vector<int>>(name, indexes));
		}
	}

	void Sprite::translate(float x, float y, float z)
	{
		Mat4f translateMatrix = Mat4f::createTranslate(x, y, z);
		model = translateMatrix * model;
	}

	void Sprite::scale(float xScale, float yScale, float zScale = 0)
	{
		Mat4f scaleMatrix = Mat4f::createScale(xScale, yScale, zScale);
		model = scaleMatrix * model;
	}

	void Sprite::rotate(float angle, float x, float y, float z)
	{
		Mat4f rotationMatrix = Mat4f::createRotate(angle, x, y, z);
		model = rotationMatrix * model;
	}

	void Sprite::rotateByCenter(float angle, float x, float y, float z)
	{
		translate(0.5f, 0.5f, 0);
		rotate(angle, x, y, z);
		translate(-0.5f, -0.5f, 0);
	}

	void Sprite::init()
	{
		init(spriteAttributes);
	}

	void Sprite::init(SpriteShaderAttributes attributes)
	{
		const char* vertexShaderSource = "#version 300 es \n"
			"uniform mat4 projectionView; \n"
			"uniform mat4 model; \n"

			"in  vec2 Position; \n"
			"in  vec4 Color; \n"
			"in  vec2 Texture; \n"

			"out vec4 fragmentColor; \n"
			"out vec2 fragmentTexture; \n"

			"void main() \n"
			"{											\n"
			"	gl_Position = projectionView * model * vec4(Position, 0.0, 1.0); \n"

			"	fragmentTexture = Texture; \n"
			"	fragmentColor = Color; \n"
			"}											\n";

		const char* fragmentShaderSource = "#version 300 es \n"
			"precision mediump float; \n"

			"uniform sampler2D tex; \n"

			"in  vec4 fragmentColor; \n"
			"in  vec2 fragmentTexture; \n"

			"out vec4 FragColor; \n"

			"void main() \n"
			"{\n"
			"	FragColor = texture(tex, fragmentTexture) * fragmentColor; \n"
			"}";

		init(attributes, vertexShaderSource, fragmentShaderSource);
	}

	void Sprite::init(SpriteShaderAttributes attributes, const char * vertexShaderSource, const char * fragmentShaderSource)
	{
		this->attributes = attributes;

		programShader = Shader::loadShaderProgram(vertexShaderSource, fragmentShaderSource);

		loadSpriteAnimation();

		initVBO();

		//scale(200.f, 200.f);
	}

	void Sprite::setUpPositionAttribute()
	{
		glVertexAttribPointer(positionAttribute,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			0); //Specify that our coordinate data is going into attribute index 0(shaderAttribute), and contains three floats per vertex
		glEnableVertexAttribArray(positionAttribute); //habilita atributo de coordenadas
	}

	void Sprite::setUpColorAttribute()
	{
		glVertexAttribPointer(colorAttribute,
			4,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)(sizeof(attributes.position)) // deslocamento do primeiro elemento
		);
		glEnableVertexAttribArray(colorAttribute); //habilita atributo de coordenadas
	}

	void Sprite::setUpTextureAttribute()
	{
		glVertexAttribPointer(textureAttribute,
			2, //quantidad de valores por vertices da textura
			GL_FLOAT,		   // o tipo de cada elementos
			GL_FALSE,          // estado dos nossos valores.
			0,					// sem dados intercalados em cada posição da textura, ou seja, os valores da texturas estão contíguos no array de vértices
			(void*)(sizeof(attributes.position) + (sizeof(attributes.color))) // deslocamento do primeiro elemento
		);
		glEnableVertexAttribArray(textureAttribute);
	}

	void Sprite::initVBO()
	{
		glGenBuffers(1, &squareVBO);  //aloca o buffr
		glBindBuffer(GL_ARRAY_BUFFER, squareVBO); //associa o bufffer ao ponteiro
		glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteShaderAttributes), &attributes, GL_DYNAMIC_DRAW);  //insere os dados no buffer para usar glDraw*

		// Get the sampler location
		modelLocation = glGetUniformLocation(programShader, "model");
		projectionViewLocation = glGetUniformLocation(programShader, "projectionView");
		positionAttribute = glGetAttribLocation(programShader, "Position");
		textureCoordinateLocation = glGetUniformLocation(programShader, "texCoord");
		colorAttribute = glGetAttribLocation(programShader, "Color");
		textureAttribute = glGetAttribLocation(programShader, "Texture");

		setUpPositionAttribute();
		setUpColorAttribute();
		setUpTextureAttribute();

		//Image *image = ImageFactory::load("resources/images/mickey.bmp");
		Image *image = ImageFactory::load("resources/images/sprite.png");
		unsigned char* pixels = image->getData();
		GLsizei width = image->getWidth();
		GLsizei height = image->getHeight();
		GLenum colorFormat = image->getColorFormat();

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//caso utilize mipmap ...
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

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

	long long tick = 0;
	bool updateFrame = true;
	void Sprite::update(long long elapsedTime)
	{
		tick += elapsedTime;

		if (tick > 100)
		{
			tick = 0;
			updateFrame = true;
		}
		else
			updateFrame = false;


		//update UV coordinates
		static int index = 0;
		if (updateFrame) {
			if (index == 3)
				index = 0;
			else
				index++;
		}

		int frameIndex = spriteData.animations["walking"][index];

		attributes.texture[0] = spriteData.frames[frameIndex].posisionX;
		attributes.texture[1] = spriteData.frames[frameIndex].positionY;

		attributes.texture[2] = spriteData.frames[frameIndex].posisionX;
		attributes.texture[3] = spriteData.frames[frameIndex].positionY + spriteData.frames[frameIndex].height;

		attributes.texture[4] = spriteData.frames[frameIndex].posisionX + spriteData.frames[frameIndex].width;
		attributes.texture[5] = spriteData.frames[frameIndex].positionY;

		attributes.texture[6] = spriteData.frames[frameIndex].posisionX + spriteData.frames[frameIndex].width;
		attributes.texture[7] = spriteData.frames[frameIndex].positionY + spriteData.frames[frameIndex].height;
	}

	void Sprite::render(Mat4f projectionViewMatrix)
	{
		glUseProgram(programShader);

		glBindBuffer(GL_ARRAY_BUFFER, squareVBO);
		glBindTexture(GL_TEXTURE_2D, texture);

		//update buffer - texture UV   SE UPDATE FRAME ?? !!
		glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteShaderAttributes), &attributes, GL_DYNAMIC_DRAW);  //insere os dados no buffer para usar glDraw*

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model);
		glUniformMatrix4fv(projectionViewLocation, 1, GL_FALSE, projectionViewMatrix);

		setUpPositionAttribute();
		setUpColorAttribute();
		setUpTextureAttribute();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	Sprite::~Sprite()
	{
		if (programShader != 0)
			glDeleteProgram(programShader);

		if (squareVBO != 0)
			glDeleteBuffers(1, &squareVBO);

		if (texture != 0 && glIsTexture(texture))
			glDeleteTextures(1, &texture);
	}
}