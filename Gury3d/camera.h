/* I am shit at math and whatever this is, so I took most of the code from https://github.com/Vulpovile/Blocks3D/blob/0b8847cd8e7d5726870e9f65c558872e0eaf477e/src/source/CameraController.cpp */

#include <G3DAll.h>

#include "rbx.h"
#include "render_shapes.h"
#include "runservice.h"
#include "controller.h"
#include "part.h"

#define CAM_ZOOM_MIN 0.5f
#define CAM_ZOOM_MAX 1000.f /* should be dependant on extents */

namespace RBX
{
	enum CameraType
	{
		Fixed,
		Follow
	};

	class Camera : public RBX::Controller
	{
	public:

		GCamera* camera;
		CameraType cameraType;

		static Reflection::PropertyDescriptor<RBX::Camera, CoordinateFrame> prop_cframe;

		float yaw, pitch, zoom;
		bool isUsingRightMouse;
		bool isZooming;		
		bool isInFirstPerson;

		RBX::PartInstance* focusPart;
		static RenderDevice* rd;

		Vector3 focusPosition;
		Vector3 translation;

		CoordinateFrame cframe;

		/* blocks3d stuff */

		void lookAt(const Vector3& position);
		void setFrame(const CoordinateFrame& cf);

		CoordinateFrame getCoordinateFrame();
		void setCoordinateFrame(CoordinateFrame cf)
		{
			Camera::singleton()->pan(&cf, 0, 0, 1, 0.41f);
			camera->setCoordinateFrame(cf);
		}

		void refreshZoom(const CoordinateFrame& frame);

		void pan(CoordinateFrame* frame, float spdX, float spdY, bool lerp=0, float lerpTime=0.69999998f);
		void panLock(CoordinateFrame* frame, float spdX, float spdY);

		void Zoom(short delta);

		/* My stuff */

		void tiltUp(double deg=25, bool enactedByZoom=0);
		void tiltDown(double deg=25, bool enactedByZoom = 0);

		void cam_zoom(bool inout);

		void update(Rendering::G3DApp* app);
		void setCamera(GCamera* c) { camera = c; }

		float getLerp() { return 0.49999998f; }

		virtual void move();
		void zoomExtents();
		void zoomExtents(RBX::Extents extents);

		static void cameraInit(GCamera* __camera, RenderDevice* rd);
		static RBX::Camera* singleton();
		
		Camera() : focusPosition(Vector3(0, 0, 0)), yaw(0.f), pitch(0.f), zoom(14.f) 
		{
			setSpeed(2.4f);
			cameraType = CameraType::Fixed;
			setClassName("Camera");
			setName("Camera");
		}

		virtual ~Camera() {}
	};
}