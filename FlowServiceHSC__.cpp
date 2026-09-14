//---------------------------------------------------------------------------

#include <fmx.h>

#ifdef _WIN32
#include <tchar.h>
#endif
#pragma hdrstop
#include <System.StartUpCopy.hpp>
//---------------------------------------------------------------------------
 USEFORM("TPhotoCapture.cpp", PhotoCaptureForm);
USEFORM("TPhotoFix.cpp", PhotoFixForm);
USEFORM("TTypeSelectionForm.cpp", TypeSelectionForm);
USEFORM("Unit1.cpp", Form1);
USEFORM("TMainForm.cpp", MainTabbedForm);
USEFORM("DeviceSlectionModule.cpp", DeviceSelectionForm);
USEFORM("THSCSettingsForm.cpp", HSCSettingsForm);
USEFORM("TDebugForm.cpp", DebugForm);
//---------------------------------------------------------------------------
extern "C" int FMXmain()
{
	try
	{
		Application->Initialize();
		Application->CreateForm(__classid(TMainTabbedForm), &MainTabbedForm);
		Application->CreateForm(__classid(TPhotoCaptureForm), &PhotoCaptureForm);
		Application->CreateForm(__classid(TPhotoFixForm), &PhotoFixForm);
		Application->CreateForm(__classid(THSCSettingsForm), &HSCSettingsForm);
		Application->CreateForm(__classid(TDebugForm), &DebugForm);
		Application->CreateForm(__classid(TDeviceSelectionForm), &DeviceSelectionForm);
		Application->CreateForm(__classid(TTypeSelectionForm), &TypeSelectionForm);
		Application->CreateForm(__classid(TForm1), &Form1);
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
