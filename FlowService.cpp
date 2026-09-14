//---------------------------------------------------------------------------

#include <fmx.h>
#ifdef _WIN32
#include <tchar.h>
#endif
#pragma hdrstop
#include <System.StartUpCopy.hpp>
//---------------------------------------------------------------------------
USEFORM("TMainForm.cpp", MainTabbedForm);
USEFORM("FormOrders.cpp", OrdersForm);
USEFORM("FormPhotoCapture.cpp", PhotoCaptureForm);
USEFORM("FormPhotoFix.cpp", PhotoFixForm);
USEFORM("FormDebug.cpp", DebugForm);
USEFORM("FormDeviceSlection.cpp", DeviceSelectionForm);
USEFORM("FormSettings.cpp", HSCSettingsForm);
USEFORM("FormTypeSelection.cpp", TypeSelectionForm);
//---------------------------------------------------------------------------
extern "C" int FMXmain()
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TMainTabbedForm), &MainTabbedForm);
		Application->CreateForm(__classid(TPhotoCaptureForm), &PhotoCaptureForm);
		Application->CreateForm(__classid(TPhotoFixForm), &PhotoFixForm);
		Application->CreateForm(__classid(TTypeSelectionForm), &TypeSelectionForm);
		Application->CreateForm(__classid(THSCSettingsForm), &HSCSettingsForm);
		Application->CreateForm(__classid(TOrdersForm), &OrdersForm);
		Application->CreateForm(__classid(TDeviceSelectionForm), &DeviceSelectionForm);
		Application->CreateForm(__classid(TDebugForm), &DebugForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
