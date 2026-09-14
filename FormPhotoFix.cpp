//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "FormPhotoFix.h"
#include <System.IOUtils.hpp>

#ifdef __ANDROID__
#include <AndroidApi.JNI.JavaTypes.hpp>
#include <Androidapi.Helpers.hpp>
#endif

#include "TDebug.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TPhotoFixForm *PhotoFixForm;

 TTimer * TPhotoFixForm::SpillTimer;

//---------------------------------------------------------------------------
__fastcall TPhotoFixForm::TPhotoFixForm(TComponent* Owner)
	: TForm(Owner)
{

   SpillTimer=new TTimer(NULL);

    if(!DirectoryExists(L"/storage/emulated/0/HSC Mobile/"))
  {
    CreateDir(L"/storage/emulated/0/HSC Mobile/");
  }


  CheckCamPermission();


}



//---------------------------------------------------------------------------
void __fastcall TPhotoFixForm::Button1Click(TObject *Sender)
{
this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::CameraSampleBufferReady(TObject *Sender,
          const TMediaTime ATime)
{
  TThread::Synchronize(TThread::CurrentThread, CameraGetPhoto);
}
//---------------------------------------------------------------------------
 //---------------------------------------------------------------------------
// Функция "забирания" фото из камеры
void __fastcall TPhotoFixForm::CameraGetPhoto(void)
{
	TBitmap *Bitmap =new TBitmap;
	int C;
	   TAlphaColor C1 , C2 ,C3 ;

	 Camera->SampleBufferToBitmap(Bitmap, true);
	  TBitmapData Data;
	if (Bitmap->Map(TMapAccess::ReadWrite, Data ))
	{
		C1 = Data.GetPixel(0,0);
		C2 =  Data.GetPixel(10,10);
		C3 =  Data.GetPixel(12,12);
        C = C1;
	};

	  Bitmap->Unmap(Data );

  if ((C1==C2)&&(C1==C3)&&(C2==C3)&&(C==-16742656))
{
	//GREEN SCRENN
		 //	ImageAfter->Bitmap =  Bitmap ;
		 aTime=0;



}
else {
  if (state == 0)
  {
  }
  else if (state == 1)
  {
		ImageBefore->Bitmap =  Bitmap ;
		//Camera->SampleBufferToBitmap(Bitmap,true );//= Bitmap;
  }
  else if (state == 2)
  {
		ImageAfter->Bitmap =  Bitmap ;
		//Camera->SampleBufferToBitmap(Bitmap,true );//= Bitmap;;
  }
	 }
   delete Bitmap;
}

void __fastcall TPhotoFixForm::Button2Click(TObject *Sender)
{
    Camera->Active      = true;
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::Button3Click(TObject *Sender)
{
        if (Camera->Active      ==  false)
        {Camera->Active      =  true;  }
        else
        {Camera->Active      =  false; }
       ;
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void TPhotoFixForm::CheckCamPermission(void)
{
#ifdef __ANDROID__
  DynamicArray<String> permissions;
  permissions.Length = 1;
  permissions[0] = JStringToString(TJManifest_permission::JavaClass->CAMERA);
  PermissionsService()->RequestPermissions(permissions,
        [this](const DynamicArray<String> APermissions,
               const DynamicArray<TPermissionStatus> AGrantResults)
    {
      if ((AGrantResults.Length == 1) and(AGrantResults[0] == TPermissionStatus::Granted))
      {
        camPermissionGranted = true;
      }
      else
      {
        camPermissionGranted = false;
      }
    } );
#endif
}
//---------------------------------------------------------------------------
// Проверка наличия доступа к хранилищу
void TPhotoFixForm::CheckStoragePermission(void)
{
  #ifdef __ANDROID__
  DynamicArray<String> permissions;
  permissions.Length = 1;
  permissions[0] = JStringToString(TJManifest_permission::JavaClass->WRITE_EXTERNAL_STORAGE);
  PermissionsService()->RequestPermissions(permissions,
        [this](const DynamicArray<String> APermissions,
               const DynamicArray<TPermissionStatus> AGrantResults)
    {
      if ((AGrantResults.Length == 1) and(AGrantResults[0] == TPermissionStatus::Granted))
      {
        storagePremissionGranted = true;
      }
      else
      {
        storagePremissionGranted = false;
      }
    } );
       #endif
}

void TPhotoFixForm::SaveVerificationResults(void)
{
  bool success = true;
  UnicodeString dirName = L"";
  TXMLDocument *XmlDoc = nullptr;
  _di_IXMLNode rootNode = nullptr;
  _di_IXMLNode sampleNode = nullptr;
  float rate = 0.0f;
  dirName = L"/storage/emulated/0/HSC Mobile/";
  System::Word Y, M, D, H, MN, S, MS;
//------------------формируем имя файла ----------------------
	DecodeDateTime(Now(),Y,M,D,H,MN,S,MS); //System.DateUtils.hpp;

	System::UnicodeString fname=
	System::Ioutils::TPath::GetCameraPath()+
    System::Ioutils::TPath::DirectorySeparatorChar+
	Format("_%d-%d-%d_%d_%d_%d_%d",
	ARRAYOFCONST((Y,M,D,H,MN,S,MS)))+
	System::Ioutils::TPath::ExtensionSeparatorChar+"jpg";
  do
  {
    if(!DirectoryExists(dirName))
    {
      CreateDir(dirName);
    }
    dirName = dirName/* + DateToStr(Date()) + "_" + TimeToStr(Time())*/;
    //
    // Если папка с таким именем существует, то будем дописывать "(номер)" к имени папки
    if(DirectoryExists(dirName))
    {
      success = false;
      for(uint32_t i = 0;  i < 1000; i++)
      {
        dirName += "(" + UIntToStr(i) + ")";
        if(!DirectoryExists(dirName)) { success = true; break; }
      }
    }
    if(false == success)
    {
      ShowMessage(L"Ошибка сохранения данных");
      break;
    }
    CreateDir(dirName);
    if(PhotoBefore != nullptr) PhotoBefore->SaveToFile(fname);//(dirName + L"/PhotoBefore.jpg");
    if(PhotoAfter != nullptr)  PhotoAfter->SaveToFile(fname);//(dirName + L"/PhotoAfter.jpg");
    ImageBefore->Bitmap =   PhotoBefore;
    XmlDoc = new TXMLDocument(NULL);
    XmlDoc->DOMVendor = DOMVendors->Vendors[0]; //< OMNI XML кроссплатформенный вендор
    XmlDoc->XML->Clear();
    XmlDoc->FileName = "";
    XmlDoc->Active = true;
    //
    // Создадим главную ветку и добавим узел об устройстве
    rootNode   = XmlDoc->AddChild(L"report");
    sampleNode = rootNode->AddChild(L"Device");
     /*
    sampleNode->SetAttribute(L"Serial", SerialNum);
    sampleNode->SetAttribute(L"DeviceType", DeviceType);
    sampleNode->SetAttribute(L"CertificateNum", CertificateNum);
    //
	// Теперь добавим информацию об условиях поверки
    sampleNode = rootNode->AddChild(L"Conditions");
    sampleNode->SetAttribute(L"TempAir", Conditions.tempAir);
    sampleNode->SetAttribute(L"PressureAir", Conditions.pressureAir);
    sampleNode->SetAttribute (L"HumidityAir", Conditions.humidityAir);
    sampleNode->SetAttribute(L"TempWater", Conditions.tempWater);
    sampleNode->SetAttribute(L"PressureWater", Conditions.pressureWater);
    sampleNode->SetAttribute(L"DensityWater", Conditions.densityWater);
    */
    //
	// Теперь сохраним результаты выполнения поверки
    sampleNode = rootNode->AddChild(L"Results");
    /*
    sampleNode->SetAttribute(L"Time", verifyTime);
    rate = static_cast<float>(verifyData.totalPulsesCnt[0]) / verifyData.MeasureData.time;
    sampleNode->SetAttribute(L"Rate", rate);
    sampleNode->SetAttribute(L"Volume", verifyData.totalPulsesCnt[0]);
        */
    fname=
	System::Ioutils::TPath::GetCameraPath()+
    System::Ioutils::TPath::DirectorySeparatorChar+
	Format("_%d-%d-%d_%d_%d_%d_%d",
	ARRAYOFCONST((Y,M,D,H,MN,S,MS)))+
	System::Ioutils::TPath::ExtensionSeparatorChar+"xml";

   // XmlDoc->SaveToFile(dirName + L"/Report.xml");
    XmlDoc->SaveToFile(fname);
    //XmlDoc->L
    XmlDoc->Active = false;
   // delete XmlDoc;
  } while(0);
//  delete PhotoBefore;
  PhotoBefore = nullptr;
//  delete PhotoAfter;
  PhotoAfter = nullptr;
 // ChangeState(HSC_STATE_PING);
}

void __fastcall  TPhotoFixForm::DataReadFormsUpdate(void)
{

		LabelTime->Text 	= FloatToStrF(time,ffNumber,10,0);
  //	LabelVolume->Text 	= FloatToStr(volume);
  //	LabelFlow->Text 	= FloatToStrF(flowEtalon,ffNumber,10,2);

   // str[inte++] = LabelFlow->Text ;
 //    temp[inte][1] = volume;
  //   temp[inte][2] = flowEtalon;


 //	StringGridTest->Cells[0][0] 	= TestMeter->Name;


	StringGridTest->Cells[0][0] 	=   EtalonMeter->FlowValue->GetStringMeanValue();

//    volume = TestMeter->GetVolume();
	StringGridTest->Cells[1][0] 	=  EtalonMeter->VolumeValue->GetStringValue();  //FloatToStrF(volume,ffNumber,10,2);





    //this->MainTabs->Repaint();


 	ProgressBar->Value =  (int)time;   //(HSC->GetTime());

	 TThread::Synchronize(NULL,StringGridTest->Repaint);
	TThread::Synchronize(NULL,LabelTime->Repaint);

    //TPhotoFixForm->Fo
}

void __fastcall  TPhotoFixForm::DataReadFormsFinalUpdate(void)
{

	LabelTime->Text 				= FloatToStrF(DurationTime,ffNumber,10,2);

	StringGridTest->Cells[0][0] 	= EtalonMeter->FlowValue->GetStringMeanValue();

	StringGridTest->Cells[1][0] 	= EtalonMeter->VolumeValue->GetStringValue();//FloatToStrF(volumeEtalon,ffNumber,10,2);

 	ProgressBar->Value 				=  (int)time;

	TestButton->Text 		= L"Сохранить";
	TestButton->Tag =  3;
	CancelButton->Visible = true;


	TThread::Synchronize(NULL,StringGridTest->Repaint);
	TThread::Synchronize(NULL,LabelTime->Repaint);
	TThread::Synchronize(NULL,TestButton->Repaint);
	TThread::Synchronize(NULL,CancelButton->Repaint);
}





void __fastcall TPhotoFixForm::Button4Click(TObject *Sender)
{
    PhotoBefore= ImageBefore->Bitmap;
    PhotoAfter= ImageAfter->Bitmap;
    SaveVerificationResults();

}

 void  TPhotoFixForm::ChangeState(uint8_t Newstate)
  {
		if (Active==1)
		{
           newstate = Newstate;
		  TThread::Synchronize(NULL,UpdateState);
		}

  }

 void __fastcall TPhotoFixForm::UpdateState(void)
  {

	float volume;
  switch(newstate)

  {
  //Запуск измерения
	case 2:
	{
      time =0;
	  BeginTime = Time();
	  TimerMesure->Enabled = true;
	  PhotoBefore = ImageBefore->Bitmap;
	  EditImageBeforeOpen();
	   SpillTimer->Enabled = true;

				 Debug->MSG.Type="PHT";
				 Debug->MSG.Direction="PhotoBefore";
				 Debug->MSG.Data="";
				 Debug->AddMSG ("LEN");

	  state =  2;
	  TestButton->Text 		= L"СТОП";
	  EtalonMeter->VolumeValue->SetValue((float)0);

	  DataReadFormsUpdate();

	}
	  break;
	  case 0:
	{
		if (state == 2)
	{
	   EndTime = Time();
        TimerMesure->Enabled = false;
		PhotoAfter= ImageAfter->Bitmap;
              EditImageAfterOpen();


				 Debug->MSG.Type="PHT";
				 Debug->MSG.Direction="PhotoAfter";
				 Debug->MSG.Data="";
				 Debug->AddMSG ("LEN");

	  state =  0;

	  TestButton->Text = L"Обработка";
	  TestButton->Tag 		= 3;

	  TimeEdit -> Enabled 	= true;

	  DurationTime = (float) MilliSecondsBetween(BeginTime,EndTime)/1000;   //MilliSecondsBetween(EndTime, BeginTime)/1000;

	  volume =  EtalonMeter->FlowValue->GetDoubleMeanValue(L"л/с") * DurationTime;
	  EtalonMeter->VolumeValue->SetValue(volume);

	   volumeEtalon = volume;

	  DataReadFormsFinalUpdate();

	}
    }
      break;
		  default: break;
  }

  }




//---------------------------------------------------------------------------


void __fastcall TPhotoFixForm::TestButtonClick(TObject *Sender)
{


     if (TestButton->Tag == 1)
	{
       TestButton->Text 		= L"Запуск";
       TimeEdit -> Enabled 	= False;
 	   //	VolumeEdit -> Enabled 	= False;
        TestButton->Tag 		= 2;
      //  state = 0;

		ProgressBar->Value = 0;
		ProgressBar->Max = StrToFloat(TestMeter->Point.Time);

		TestMeter->ResetTest();
		//EtalonMeter->ResetTest();
        TestMeter->SetEtalon(EtalonMeter);


		//HSC->StartMeasureInstant(TestMeter->Point.Time+10);


		SpillTimer->Interval=  StrToInt(TestMeter->Point.Time+100)*1000;

      	StringGridTest->Cells[2][0] 	= "";
        StringGridTest->Cells[3][0] 	= "";


		EditBefore->Text = "";
		EditAfter->Text = "";

		ChangeState(2);


	  //  SpillTimer->Enabled = true;
      //  newstate = 2;
      //  UpdateState();
     }
        	else if (TestButton->Tag == 2)

	{
	  //	HSC->StopMeasure();
		SpillTimer->Enabled = false;
		TimerMesure->Enabled = false;

		TestButton->Text	 	= L"Остановка";
		TestButton->Tag 		= 3;
		TimeEdit -> Enabled 	= True;

		ChangeState(0);
	}

	else if (TestButton->Tag == 3)
	{
		AfterSpillage();
		TestMeter->SaveDataPoint(EtalonMeter);
		TestButton->Text	 	= L"СТАРТ";
		state = 1;
		TestButton->Tag 		= 1;
        TimeEdit -> Enabled 	= True;
     //   VolumeEdit -> Enabled 	= True;
	 BeforeTabItem->IsSelected = true;
     CancelButton->Visible = false;
	 TFlowMeter::SaveToFile(TestMeter,0);
    }


}

void TPhotoFixForm::StopMesure()
{

}

void TPhotoFixForm::StartMesure()
{



}
//---------------------------------------------------------------------------

void TPhotoFixForm::AfterSpillage(void)
{
	float f, fa, fb, v, error;

	   if  ((TryStrToFloat_(EditBefore->Text,fb))&&(EditBefore->Text!=""))
       {
	   if ((TryStrToFloat_(EditAfter->Text,fa))&&(EditAfter->Text!=""))
	   {
		 fa = fa *1000 ;
		 fb = fb *1000 ;
		  v = (fa-fb);
		  error=((v-volumeEtalon)*100)/volumeEtalon;

		  StringGridTest->Cells[2][0] 	= FloatToStrF(v,ffNumber,10,2);
		  StringGridTest->Cells[3][0] 	= FloatToStrF(error,ffNumber,10,2);

		  TestMeter->DataPoint.Time 		= DurationTime;
		  TestMeter->DataPoint.Error 		= error;
		  TestMeter->DataPoint.VolumeBefore	= fb;
		  TestMeter->DataPoint.VolumeAfter	= fa;
		  TestMeter->DataPoint.Volume 		= v;
		  TestMeter->DataPoint.Q 			= EtalonMeter->FlowValue->GetFloatValue();
		  TestMeter->DataPoint.EtlVolume 	= volumeEtalon;

		  BeforeTabItem->IsSelected = true;
		  EditAfter->TextSettings->FontColor=claWhite;
		  EditBefore->TextSettings->FontColor=claWhite;
		  EditAfter->Repaint();


	   }  else

       {
         	//ShowMessage(L"Ошибка заполнения данных");
         //   EditAfter->TextSettings->FontColor=claRed;
            EditAfter->Repaint();
       }

         } else
         {
           // EditBefore->TextSettings->FontColor=claRed;
            EditBefore->Repaint();
		 }


}



void __fastcall TPhotoFixForm::FormShow(TObject *Sender)
{
		state = 1;
        TimerOn->Enabled = true;
        Camera->Active      = true;
        BeforeTabItem->IsSelected = true;
		CameraState=1;
		 /*
		TestButton->Text	 	= L"Измерение";
		TestButton->Tag 		= 1;
		TimeEdit -> Enabled 	= True;
          */
        CameraButton->Visible = false;
	  	FocusButton->Visible = false;
		HighQButton->Visible = false;
	//	LowQButton->Visible = false;
		MediumQButton->Visible = false;

		Active = 1;

		SpillTimer->OnTimer = OnTimer;
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::OnTimer(TObject *Sender)
{
	 // HSC->StopMeasure();
	  SpillTimer->Enabled = false;

/*  test */
 //       newstate = 0;
 //       UpdateState();
 /*      */
                 Debug->MSG.Type="PHT";
       			 Debug->MSG.Direction="";
                 Debug->MSG.Data="";
        		 Debug->AddMSG ("StopTimer");
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::FormHide(TObject *Sender)
{
	state = 0;
	Camera->Active      = false;
	Active=0;
		if (OnTurnUpConnection != nullptr)
		OnTurnUpConnection(this);
}

	  void TPhotoFixForm::SetOnTurnUpConnection(TOn On)
	{
		OnTurnUpConnection = On;
	}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::EditBeforeEnter(TObject *Sender)
{
		 EditBefore->TextSettings->FontColor=claWhite;
		  EditBefore->StyledSettings=EditBefore->StyledSettings << TStyledSetting::FontColor;


			   if (EditBefore->Tag==0) {
		 EditImageBeforeOpen();
	   }
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::CameraButtonClick(TObject *Sender)
{
       if (Camera->Active      ==  false)
        {Camera->Active      =  true;  }
        else
        {Camera->Active      =  false; }
       ;
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::MediumQButtonClick(TObject *Sender)
{
  	Camera->Active      =  false;
	 Camera->Quality = TVideoCaptureQuality::MediumQuality;
     Camera->Active      =  true;
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::FocusButtonClick(TObject *Sender)
{

Camera->Active      =  false;
Camera->FocusMode = TFocusMode::AutoFocus;
Camera->Active      =  true;

}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::HighQButtonClick(TObject *Sender)
{      Camera->Active      =  false;
	 Camera->Quality = TVideoCaptureQuality::LowQuality;
     Camera->Active      =  true;
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::LowQButtonClick(TObject *Sender)
{        Camera->Active      =  false;
		 Camera->Quality = TVideoCaptureQuality::LowQuality;
         Camera->Active      =  true;
}
//---------------------------------------------------------------------------



void __fastcall TPhotoFixForm::TabControlGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled)
{
        	String s;

	if (GestureToIdent(EventInfo.GestureID, s)) {

		 if (s == "sgiLeft")
		 {
			 AfterTabItem->IsSelected = true;
         }
         else if (s == "sgiRight")
		 {
			BeforeTabItem->IsSelected = true;
		 }

		//ShowMessage(s);
	}
	else {
	  //	ShowMessage("Could not translate gesture identifier");
	}
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::BeforeTabItemGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled)
{
       	String s;

	if (GestureToIdent(EventInfo.GestureID, s)) {

         if (s == "sgiLeft")
         {
             AfterTabItem->IsSelected = true;
         }
         else if (s == "sgiRight")
         {
            BeforeTabItem->IsSelected = true;
         }

		//ShowMessage(s);
	}
	else {
	  //	ShowMessage("Could not translate gesture identifier");
	}
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::AfterTabItemGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled)
{
        	String s;

	if (GestureToIdent(EventInfo.GestureID, s)) {

         if (s == "sgiLeft")
         {
             AfterTabItem->IsSelected = true;
         }
         else if (s == "sgiRight")
         {
            BeforeTabItem->IsSelected = true;
         }

		//ShowMessage(s);
	}
	else {
	  //	ShowMessage("Could not translate gesture identifier");
	}



}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::ImageAfterGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled)
{

		String s;

	if (GestureToIdent(EventInfo.GestureID, s)) {

         if (s == "sgiLeft")
         {
             AfterTabItem->IsSelected = true;
         }
         else if (s == "sgiRight")
         {
            BeforeTabItem->IsSelected = true;
         }

		//ShowMessage(s);
	}
	else {
	  //	ShowMessage("Could not translate gesture identifier");
	}



}
//---------------------------------------------------------------------------



void __fastcall TPhotoFixForm::EditBeforeExit(TObject *Sender)
{

       float fa;

		   if  (TryStrToFloat_(EditBefore->Text,fa))
	   {
		   EditBefore->StyledSettings=EditAfter->StyledSettings >>TStyledSetting::FontColor;
		//  EditBefore->TextSettings->FontColor=claGreen;
		  AfterTabItem->IsSelected = true;

	   } else
		{
			EditBefore->StyledSettings=EditAfter->StyledSettings >>TStyledSetting::FontColor;
		 //	EditBefore->TextSettings->FontColor=claRed;
		}


}


void __fastcall TPhotoFixForm::TimerOnTimer(TObject *Sender)
{
	if (CameraState==1)
    {
        try
        {
        Camera->Active = false;
		Camera->Quality = TVideoCaptureQuality::MediumQuality;
        Camera->FocusMode = TFocusMode::AutoFocus;
        CameraState=2;
        }
		catch (const Exception& e)
        { }
    }
    else if (CameraState==2)
    {
	   Camera->Active      = true;
	   CameraState=0;
		TimerOn->Enabled = false;
	}


}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::VisibleButtonClick(TObject *Sender)
{

	if (CameraButton->Visible == true)
    {
      	CameraButton->Visible = false;
	 //	FocusButton->Visible = false;
		HighQButton->Visible = false;
	  //	LowQButton->Visible = false;

    } else
    {
        CameraButton->Visible = true;
		FocusButton->Visible = true;
	  	HighQButton->Visible = true;
	 //	LowQButton->Visible = true;

    }

}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::EditAfterEnter(TObject *Sender)
{
		 EditBefore->TextSettings->FontColor=claWhite;
		  EditBefore->StyledSettings=EditBefore->StyledSettings << TStyledSetting::FontColor;


			   if (EditBefore->Tag==0) {
		 EditImageAfterOpen();
	   }
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::EditAfterExit(TObject *Sender)
{
   float fa;


    		   if  (TryStrToFloat_(EditAfter->Text,fa))
	   {
		  EditAfter->StyledSettings=EditAfter->StyledSettings >> TStyledSetting::FontColor;
		 // EditAfter->TextSettings->FontColor=claGreen;
		  EditAfter->Repaint();

	   } else
		{
		   EditAfter->StyledSettings=EditAfter->StyledSettings >> TStyledSetting::FontColor;
		//   EditAfter->TextSettings->FontColor=claRed;
		   EditAfter->Repaint();

		}
  // uint16_t r;
  /*         if  (TryStrToFloat(EditAfter->Text,fa))
	   {
		  EditAfter->TextSettings->FontColor=claGreen;
		  EditAfter->Repaint();

       } else
        {
		   EditAfter->TextSettings->FontColor=claRed;
		   EditAfter->Repaint();

		}
	*/


	   // TryStrToInt("1123", r);
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::CameraFocus(void)
{
	Camera->Active      =  false;
	Camera->FocusMode = TFocusMode::AutoFocus;
	//Camera->StartCapture();
	Camera->Active      =  true;
}


void __fastcall TPhotoFixForm::ImageBeforeClick(TObject *Sender)
{
   if (state==1)
   {
		TThread::Synchronize(TThread::CurrentThread, CameraFocus);
   }
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::ImageAfterClick(TObject *Sender)  {
 try
{
   if (state==2)
   {
		TThread::Synchronize(TThread::CurrentThread, CameraFocus);
   }
} catch(...)
{

}
}
//---------------------------------------------------------------------------


void __fastcall TPhotoFixForm::EditAfterKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
	   UnicodeString M;
	if(((KeyChar<L'0')||(KeyChar>L'9'))&&(KeyChar!=L'.')&&(KeyChar!=L','))//((Key!=8)&&(Key!=46)))
{
	  KeyChar=NULL;   }
   /* if((KeyChar==L'.')||(KeyChar==L','))
    {
        M=FloatToStr(0.1);
        KeyChar=M[2];
    } */

        if(KeyChar==L'.')
        { KeyChar=L',';  }


       if (Key==13U)
   {
	  EditAfter->ResetFocus();
	  EditImageAfterClose();
   }
}
//---------------------------------------------------------------------------


void __fastcall TPhotoFixForm::EditBeforeKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
	   UnicodeString M;
	if(((KeyChar<L'0')||(KeyChar>L'9'))&&(KeyChar!=L'.')&&(KeyChar!=L','))//(Key!=8)&&(Key!=46))

	{  KeyChar=NULL; }
   /* if((KeyChar==L'.')||(KeyChar==L','))
	{
        M=FloatToStr(0.1);
        KeyChar=M[2];
    } */

        if(KeyChar==L'.')
        { KeyChar=L',';  }

   if (Key==13U)
   {
	  EditBefore->ResetFocus();
	  EditImageBeforeClose();
	  AfterTabItem->IsSelected = true;
	  TFlowMeter::SaveToFile(TestMeter,0);
   }

}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::CancelButtonClick(TObject *Sender)
{
                CancelButton->Visible = false;

				TestButton->Text	 	= L"СТАРТ";
        		state = 1;
				TestButton->Tag 		= 1;
        		TimeEdit -> Enabled 	= True;
				BeforeTabItem->IsSelected = true;



}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::Button6Click(TObject *Sender)
{
    if (Camera->HasTorch)
    {
    if (Camera->TorchMode == TTorchMode::ModeOn)
    {
    	Camera->TorchMode = TTorchMode::ModeOff;
    }   else
    {
        Camera->TorchMode = TTorchMode::ModeOn;
    }
    }
}
//---------------------------------------------------------------------------
/*
void __fastcall TPhotoFixForm::ButtonMonitorClick(TObject *Sender)
{
    time =  0;

   if (ButtonMonitor->Tag == 0)
   {
    ButtonMonitor->IsPressed = true;
    ButtonMonitor->Tag = 1;

    TestMeter->ResetTest();

    EtalonMeter->ResetTest();

    HSC->StartMonitor();

   }
   else
   {
    ButtonMonitor->IsPressed = false;
    ButtonMonitor->Tag = 0;

    HSC->StopMonitor();

   }
}
*/
//---------------------------------------------------------------------------



void __fastcall TPhotoFixForm::EditAfterValidate(TObject *Sender, UnicodeString &Text)

{
	  AfterSpillage();


}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::EditBeforeValidate(TObject *Sender, UnicodeString &Text)

{
         AfterSpillage();
}
//---------------------------------------------------------------------------


void __fastcall TPhotoFixForm::ButtonMonitorClick(TObject *Sender)
{
 time =  0;

   if (ButtonMonitor->Tag == 0)
   {
    ButtonMonitor->IsPressed = true;
    ButtonMonitor->Tag = 1;

	TestMeter->ResetTest();
	EtalonMeter->ResetTest();
    TestMeter->SetEtalon(EtalonMeter);


		Anim1->Enabled=true;
	Anim2->Enabled=    false;

    HSC->StartMonitor();

   }
   else
   {
    ButtonMonitor->IsPressed = false;
    ButtonMonitor->Tag = 0;

	HSC->StopMonitor();

    Glow1->Enabled=false;
    Anim1->Enabled=false;
    Anim2->Enabled=true;

   }
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::LabelLimitGesture (TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled)
{
	String s;

    	if (GestureToIdent(EventInfo.GestureID, s)) {

		 if (s == "sgiLeft")
		 {
			 AfterTabItem->IsSelected = true;
         }
         else if (s == "sgiRight")
		 {
			BeforeTabItem->IsSelected = true;
		 }

		//ShowMessage(s);
	}
	else {
	  //	ShowMessage("Could not translate gesture identifier");
	}
}
//---------------------------------------------------------------------------


void __fastcall TPhotoFixForm::TimeEditExit(TObject *Sender)
{
	 int i;

	if (TryStrToInt_(TimeEdit->Text,i) ) {
	  TestMeter->Point.Time = i;
	} else
{
	TimeEdit->Text = "";
}
}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::TimerMesureTimer(TObject *Sender)
{
	time = (int) MilliSecondsBetween(BeginTime,Time())/1000; ;

	if ((TestMeter->Point.Time*2)<=time) {

		ChangeState(0);



	 } else
	{
		volume =  EtalonMeter->FlowValue->GetDoubleMeanValue(L"л/с") * time;
		EtalonMeter->VolumeValue->SetValue(volume);
		DataReadFormsUpdate();
	}
}
//---------------------------------------------------------------------------


void __fastcall TPhotoFixForm::ButtonApplyBeforeClick(TObject *Sender)
{
		EditImageBeforeClose();
		AfterTabItem->IsSelected = true;
}
//---------------------------------------------------------------------------

void TPhotoFixForm::EditImageBeforeOpen()
{
	LayoutButtons->Visible = false;
	LayoutPanel->Visible = false;

	ImageBeforeEditor->Visible = true;
	ImageBefore->Visible = false;
    EditBefore->Tag=1;
//	TabControl->Enabled = false;

	ImageBeforeEditor->Enabled = true;
	ImageBefore->Enabled = false;
	ImageBeforeEditor->RotationAngle  =0;
	ImageBeforeEditor->Align = TAlignLayout::Client;
	ImageBeforeEditor->Bitmap =  ImageBefore->Bitmap;
	ButtonApplyBefore->Enabled = true;
	ImageBeforeEditor->Align = TAlignLayout::None;
}

void TPhotoFixForm::EditImageBeforeClose()
{

	LayoutButtons->Visible = true;
	LayoutPanel->Visible = true;
	 EditBefore->Tag=0;
	ImageBeforeEditor->Visible = false;
	ImageBefore->Visible = true;

	//TabControl->Enabled = true;

	ImageBeforeEditor->Enabled = false;
	ImageBefore->Enabled = true;

	ButtonApplyBefore->Enabled = false;

}
 /*
void TPhotoFixForm::EditImageAfterOpen()
{
	LayoutButtons->Visible = false;
	LayoutPanel->Visible = false;
	EditAfter->Tag=1;

   //	TabControl->Enabled = false;

	ImageAfterEditor->Visible = true;
	ImageAfter->Visible = false;

	ImageAfterEditor->Enabled = true;
	ImageAfter->Enabled = false;
	ImageAfterEditor->RotationAngle  =0;
	ImageAfterEditor->Align = TAlignLayout::Client;
	ImageAfterEditor->Bitmap =  ImageAfter->Bitmap;
	ButtonApplyAfter->Enabled = true;
	ImageAfterEditor->Align = TAlignLayout::None;
}
*/
void TPhotoFixForm::EditImageAfterClose()
{

	LayoutButtons->Visible = true;
	LayoutPanel->Visible = true;
	EditAfter->Tag=0;
 //	TabControl->Enabled = true;

	ImageAfterEditor->Visible = false;
	ImageAfter->Visible = true;

	ImageAfterEditor->Enabled = false;
	ImageAfter->Enabled = true;

	ButtonApplyAfter->Enabled = false;

}
void TPhotoFixForm::EditImageAfterOpen()
{
	AfterTabItem->IsSelected = true;
	LayoutButtons->Visible = false;
	LayoutPanel->Visible = false;
	EditAfter->Tag=1;

   //	TabControl->Enabled = false;

	ImageAfterEditor->Visible = true;
	ImageAfter->Visible = false;

	ImageAfterEditor->Enabled = true;
	ImageAfter->Enabled = false;
	ImageAfterEditor->RotationAngle  =0;
	ImageAfterEditor->Align = TAlignLayout::Client;
	ImageAfterEditor->Bitmap =  ImageAfter->Bitmap;
	ButtonApplyAfter->Enabled = true;
	ImageAfterEditor->Align = TAlignLayout::None;
}
/*
void TPhotoFixForm::EditImageAfterClose()
{
	LayoutButtons->Visible = true;
	LayoutPanel->Visible = true;
	EditAfter->Tag=0;
 //	TabControl->Enabled = true;

	ImageAfterEditor->Visible = false;
	ImageAfter->Visible = true;

	ImageAfterEditor->Enabled = false;
	ImageAfter->Enabled = true;

	ButtonApplyAfter->Enabled = false;

}
 */

void __fastcall TPhotoFixForm::ButtonApplyAfterClick(TObject *Sender)
{
		EditImageAfterClose();

}

void __fastcall TPhotoFixForm::ImageBeforeEditorGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled)
{
	 if (EventInfo.Flags.Contains(TInteractiveGestureFlag::gfBegin))
		{
			// Record the initial dimensions and distance between the fingers
			InitialWidth = ImageBeforeEditor->Width;
			InitialHeight = ImageBeforeEditor->Height;
			InitialDistance = (float) EventInfo.Distance;
			LocationStartX =  EventInfo.Location.X;
			LocationStartY  =  EventInfo.Location.Y;
			AngleStart = (int) ImageBeforeEditor->RotationAngle;
			  position = ImageBeforeEditor->Position->Point;

			  if (static_cast<System::Word>(EventInfo.GestureID) == igiRotate)
			  {
				 ImageBeforeEditor->RotationAngle = 0;
				 ImageBeforeEditor->Tag=2;
			  }

			  if (static_cast<System::Word>(EventInfo.GestureID) == igiZoom)
			  {
				 ImageBeforeEditor->Tag=1;
			  }

			  if (static_cast<System::Word>(EventInfo.GestureID) == igiPan)
			  {
				 ImageBeforeEditor->Tag=3;
			  }



		}

		 if (EventInfo.Flags.Contains(TInteractiveGestureFlag::gfEnd))
		{
			// Record the initial dimensions and distance between the fingers
		 //	Image1->Width = InitialWidth;
		//	Image1->Height = InitialHeight;
			//InitialDistance = EventInfo.Distance;
			//LocationStartX =  EventInfo.Location.X;
			//LocationStartY  =  EventInfo.Location.Y;
			//AngleStart = Image1->RotationAngle;
			ImageBeforeEditor->Tag=0;
		}


	switch (EventInfo.GestureID)
	{
		case igiZoom:

		{
			if ((ImageBeforeEditor->Tag==1)||(ImageBeforeEditor->Tag==0))


		{
				 TPointF	Center =   EventInfo.Location;


			float scaleFactor = (float) (EventInfo.Distance) / (float)(InitialDistance);
			int newWidth = InitialWidth * scaleFactor;
			int newHeight = InitialHeight * scaleFactor;

			 float scale =  (float) newWidth/InitialWidth;

	// Calculate the position of the cursor relative to the image
	// Calculate the new position of the image so that the cursor remains at the same position
	TPointF newPosition = position + ((Center - position) * (1 - scale));
	// Set the new size and position of the image
	ImageBeforeEditor->Width = newWidth;
	ImageBeforeEditor->Height = newHeight;

	ImageBeforeEditor->Position->Point = newPosition;




					 ImageBeforeEditor->Tag=1;

			}


			break;
		   }

		case igiPan:
			{
			// move the image
			 if (ImageBeforeEditor->Tag==3)
			{
				 ImageBeforeEditor->Position->X = (EventInfo.Location.X- LocationStartX)+position.X;
				 ImageBeforeEditor->Position->Y = (EventInfo.Location.Y- LocationStartY)+position.Y;
				 ImageBeforeEditor->Tag=3;
			 }

		Handled = true; }
			break;

		case igiRotate:
			// rotate the image
				if ((ImageBeforeEditor->Tag==2)||(ImageBeforeEditor->Tag==0))
					{
			ImageBeforeEditor->RotationAngle =  AngleStart-(EventInfo.Angle *57.3);
			ImageBeforeEditor->Tag=2;
					 }
			Handled = true;

			break;
    }

	Handled = true;


}
//---------------------------------------------------------------------------

void __fastcall TPhotoFixForm::ImageAfterEditorGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled)
{

		String s;

	if (GestureToIdent(EventInfo.GestureID, s)) {

		 if (s == "sgiLeft")
		 {
			 AfterTabItem->IsSelected = true;
		 }
		 else if (s == "sgiRight")
		 {
			BeforeTabItem->IsSelected = true;
		 }

		//ShowMessage(s);
	}
	else {
	  //	ShowMessage("Could not translate gesture identifier");
	}




 {
	 if (EventInfo.Flags.Contains(TInteractiveGestureFlag::gfBegin))
		{
			// Record the initial dimensions and distance between the fingers
			InitialWidth = ImageAfter->Width;
			InitialHeight = ImageAfter->Height;
			InitialDistance = (float) EventInfo.Distance;
			LocationStartX =  EventInfo.Location.X;
			LocationStartY  =  EventInfo.Location.Y;
			AngleStart = (int) ImageAfter->RotationAngle;
			  position = ImageAfter->Position->Point;

			  if (static_cast<System::Word>(EventInfo.GestureID) == igiRotate)
			  {
				 ImageAfter->RotationAngle = 0;
				 ImageAfter->Tag=2;
			  }

			  if (static_cast<System::Word>(EventInfo.GestureID) == igiZoom)
			  {
				 ImageAfter->Tag=1;
			  }

			  if (static_cast<System::Word>(EventInfo.GestureID) == igiPan)
			  {
				 ImageAfter->Tag=3;
			  }



		}

		 if (EventInfo.Flags.Contains(TInteractiveGestureFlag::gfEnd))
		{
			// Record the initial dimensions and distance between the fingers
		 //	Image1->Width = InitialWidth;
		//	Image1->Height = InitialHeight;
			//InitialDistance = EventInfo.Distance;
			//LocationStartX =  EventInfo.Location.X;
			//LocationStartY  =  EventInfo.Location.Y;
			//AngleStart = Image1->RotationAngle;
			ImageBefore->Tag=0;
		}


	switch (EventInfo.GestureID)
	{
		case igiZoom:

		{
			if (ImageAfter->Tag==1)


		{
				 TPointF	Center =   EventInfo.Location;

			float scaleFactor = (float) (EventInfo.Distance) / (float)(InitialDistance);
			int newWidth = InitialWidth * scaleFactor;
			int newHeight = InitialHeight * scaleFactor;

			 float scale =  (float) newWidth/InitialWidth;

	// Calculate the position of the cursor relative to the image
	// Calculate the new position of the image so that the cursor remains at the same position
	TPointF newPosition = position + ((Center - position) * (1 - scale));
	// Set the new size and position of the image
	ImageAfter->Width = newWidth;
	ImageAfter->Height = newHeight;

	ImageAfter->Position->Point = newPosition;




					 ImageAfter->Tag=1;

			}


			break;
		   }

		case igiPan:
			{
			// move the image
			 if (ImageAfter->Tag==3) {
				 ImageAfter->Position->X = (EventInfo.Location.X- LocationStartX)+position.X;
				 ImageAfter->Position->Y = (EventInfo.Location.Y- LocationStartY)+position.Y;
				 ImageAfter->Tag=3;
			 }

		Handled = true; }
			break;

		case igiRotate:
			// rotate the image
				if (ImageAfter->Tag==2)
					{
			ImageAfter->RotationAngle =  AngleStart-(EventInfo.Angle *57.3);
			ImageAfter->Tag=2;
					 }
			Handled = true;
			break;
    }

	Handled = true;





}




}
//---------------------------------------------------------------------------


