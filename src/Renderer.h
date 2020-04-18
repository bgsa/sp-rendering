#ifndef RENDERER_HEADER
#define RENDERER_HEADER

#include "SpectrumRendering.h"
#include <DisplayDevice.h>
#include <WindowInputDevice.h>
#include <KeyboardInputDevice.h>
#include <PointerInputDevice.h>
#include <TouchInputDevice.h>

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

		Camera* camera = nullptr;

		void render2D(RenderData renderData);
		void render3D(RenderData renderData);

		void updateInputDevices(long long elapsedTime);

	public:
		virtual void API_INTERFACE init(DisplayDevice* displayDevice);
		virtual void API_INTERFACE start();
		void API_INTERFACE resize(int width, int height);

		void API_INTERFACE update();
		void API_INTERFACE render();
		void API_INTERFACE exit();

		void API_INTERFACE addPointerHandler(PointerInputDeviceHandler* handler);
		void API_INTERFACE addKeyboardHandler(KeyboardInputDeviceHandler* handler);
		void API_INTERFACE addTouchHandler(TouchInputDeviceHandler* handler);
		void API_INTERFACE addWindowHandler(WindowInputDeviceHandler* handler);

		void API_INTERFACE addGraphicObject(GraphicObject* graphicObject);
		bool API_INTERFACE hasGraphicObject(GraphicObject* graphicObject);
		void API_INTERFACE removeGraphicObject(GraphicObject* graphicObject);

		void API_INTERFACE addInputDevice(InputDevice* inputDevice);

		Camera* getCamera();

		API_INTERFACE ~Renderer();
	};
}

#endif // RENDERER_HEADER