#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include "SpectrumRendering.h"
#include <DisplayDevice.h>
#include <WindowInputDevice.h>
#include <KeyboardInputDevice.h>
#include <PointerInputDevice.h>
#include <TouchInputDevice.h>
#include "RendererEditor.h"
#include "IRenderer.h"

#include <Timer.h>
#include "Camera.h"
#include "GraphicObject2D.h"
#include "GraphicObject3D.h"
#include <CollisionResponse.h>

#include "SphereX.h"
#include "Cube.h"
#include <OBB.h>
#include "CubeList.h"

namespace NAMESPACE_RENDERING
{
	class Renderer 
		: public IRendererManager
	{
	private:
		std::vector<GraphicObject*> graphicObjects;

		Timer timer;

		bool isRunning = true;

	protected:
		DisplayDevice* displayDevice;
		std::vector<PointerInputDevice*> pointerInputDevices;
		std::vector<WindowInputDevice*> windowInputDevices;
		std::vector<KeyboardInputDevice*> keyboardInputDevices;
		std::vector<TouchInputDevice*> touchInputDevices;
		
		Camera* camera = NULL;
		RendererEditor* editor = NULL;

		void render2D(RenderData renderData);
		void render3D(RenderData renderData);

		void updateInputDevices(long long elapsedTime);

	public:
		API_INTERFACE virtual void init(DisplayDevice* displayDevice);
		API_INTERFACE virtual void start();
		API_INTERFACE void stop();
		API_INTERFACE void resize(sp_float width, sp_float height);

		API_INTERFACE void update();
		API_INTERFACE void render();

		API_INTERFACE void addPointerHandler(PointerInputDeviceHandler* handler);
		API_INTERFACE void addKeyboardHandler(KeyboardInputDeviceHandler* handler);
		API_INTERFACE void addTouchHandler(TouchInputDeviceHandler* handler);
		API_INTERFACE void addWindowHandler(WindowInputDeviceHandler* handler);

		API_INTERFACE void addGraphicObject(GraphicObject* graphicObject);
		API_INTERFACE bool hasGraphicObject(GraphicObject* graphicObject);
		API_INTERFACE void removeGraphicObject(GraphicObject* graphicObject);

		API_INTERFACE void addInputDevice(InputDevice* inputDevice);

		API_INTERFACE Camera* getCamera();

		API_INTERFACE inline void setRendererEditor(RendererEditor* editor)
		{
			this->editor = editor;
		}

		API_INTERFACE ~Renderer();
	};
}

#endif // RENDERER_HEADER