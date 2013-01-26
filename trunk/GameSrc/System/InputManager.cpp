#include "../System/InputManager.hpp"

#include <faceAPI/sm_api.h>

SINGLETON_CONSTRUCTOR(InputManager), IEventListener()
{
	_inputList.clear();
}

SINGLETON_DESTRUCTOR(InputManager)
{
	_inputList.clear();

	smAPIQuit();
}

void receiveHeadPose(void *,smHTHeadPose head_pose)
{
	// Position of head is in struct head_pose.head_pos as a 3D x,y,z coordinate in meters
	// measured releative to the camera, where Z is the camera axis.

	// Rotation of head is in struct head_pose.head_rot as a 3D x,y,z euler angle rotation in radians
	// measured releative to the camera, where Z is the camera axis.

	// Position of left and right eyeball centers also available as 3D position coordinates.
	// as head_pose.left_eye_pos and head_pose.right_eye_pos
}

void InputManager::SetUpInput( void )
{
	smAPIInit();
	smCameraRegisterType(SM_API_CAMERA_TYPE_WDM)

	smEngineHandle engine_handle = 0;
	smEngineCreate(SM_API_ENGINE_LATEST_HEAD_TRACKER,&engine_handle)

	smVideoDisplayHandle video_display_handle = 0;
	THROW_ON_ERROR(smVideoDisplayCreate(engine_handle,&video_display_handle,0,TRUE));

	smWindowHandle win_handle = 0;
	THROW_ON_ERROR(smVideoDisplayGetWindowHandle(video_display_handle,&win_handle));    
	SetWindowText(win_handle, _T("Webcam Debug"));

	smEngineStart(engine_handle);
}

void InputManager::HandleEvent( const EventData& theevent )
{
	if(theevent.GetEventType() == Event_Unload)
	{
		for(InputListIterator iter = _inputList.begin(); iter != _inputList.end(); iter++)
		{
			delete (*iter);
		}

		_inputList.clear();
	}
}

void InputManager::ProcessInput( void )
{
	for(InputListIterator iter = _inputList.begin(); iter != _inputList.end(); iter++)
	{
		delete (*iter);
	}

	_inputList.clear();
}

void InputManager::AddInput( IInput* const input )
{
	_inputList.push_back(input);
}

void InputManager::RemoveInput( IInput* const input )
{
	_inputList.remove(input);
}

