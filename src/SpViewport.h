#ifndef SP_VIEWPORT_HEADER
#define SP_VIEWPORT_HEADER

#include "SpectrumRendering.h"
#include "SpViewportData.h"
#include "SpSceneRenderer.h"
#include "OpenGLFramebuffer.h"
#include "SpRect.h"

namespace NAMESPACE_RENDERING
{
	class SpViewport
	{
	private:
		SpSceneRendererData renderData;
		OpenGLFramebuffer* _framebuffer;
		SpSceneRenderer* renderer;

	public:
		
		sp_uint activeCameraIndex;
		SpScene* scene;

		/// <summary>
		/// Default constructor
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline SpViewport()
		{
			_framebuffer = sp_mem_new(OpenGLFramebuffer);
			renderer = sp_mem_new(SpSceneRenderer);
			activeCameraIndex = SP_UINT_MAX;
			scene = nullptr;
		}
		
		/// <summary>
		/// Resize the viewport area
		/// </summary>
		/// <param name="area">Area</param>
		/// <returns>void</returns>
		API_INTERFACE inline void resize(const SpRect<sp_int>& area)
		{
			renderData.viewportData.x = area.x;
			renderData.viewportData.y = area.y;
			renderData.viewportData.width = area.width;
			renderData.viewportData.height = area.height;

			_framebuffer
				->use()
				->setup(area.width, area.height);
		}

		/// <summary>
		/// Render the viewport
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void render()
		{		
			renderData.framebuffer = _framebuffer;
			renderData.cameraIndex = activeCameraIndex;
			renderData.scene = scene;

			renderer->render(renderData);
		}

		/// <summary>
		/// Get the aspect ratio of this viewport
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline sp_float aspectRatio() const
		{
			return renderData.viewportData.aspectRatio();
		}

		/// <summary>
		/// Get the background color
		/// </summary>
		/// <returns>Color RGBA</returns>
		API_INTERFACE inline SpColorRGBA* backgroundColor()
		{
			return &renderData.backgroundColor;
		}

		/// <summary>
		/// Get the current framebuffer
		/// </summary>
		/// <returns>Framebuffer</returns>
		API_INTERFACE inline OpenGLFramebuffer* framebuffer() const
		{
			return _framebuffer;
		}

		/// <summary>
		/// Get the size of the viewport
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline SpSize<sp_int> size() const
		{
			return SpSize<sp_int>(renderData.viewportData.width, renderData.viewportData.height);
		}

		/// <summary>
		/// Release all allocated resources
		/// </summary>
		/// <returns></returns>
		API_INTERFACE inline void dispose()
		{
			if (_framebuffer != nullptr)
			{
				sp_mem_delete(_framebuffer, OpenGLFramebuffer);
				_framebuffer = nullptr;
			}

			if (renderer != nullptr)
			{
				sp_mem_delete(renderer, SpSceneRenderer);
				renderer = nullptr;
			}
		}

		~SpViewport()
		{
			dispose();
		}

	};
}

#endif // SP_VIEWPORT_HEADER