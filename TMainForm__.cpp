// ---------------------------------------------------------------------------

#include <fmx.h>
#include <System.UITypes.hpp>
#pragma hdrstop

#include "TMainForm.h"
#include "DeviceSlectionModule.h"
#include "TPhotoCapture.h"
#include "TPhotoFix.h"
#include "TDebugForm.h"
#include "TTypeSelectionForm.h"
#include "Unit1.h"

#include <System.IOUtils.hpp>
#include "TSettingsClass.h"
#include "TDebug.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"

TDebug *Debug;

float f;

TMainTabbedForm *MainTabbedForm;

TTimer *TMainTabbedForm::TimerStop ;


 void TMainTabbedForm::TryFilesDowmload()
 {
	UnicodeString URLadress;
	UnicodeString Dir = L" ";
	UnicodeString fname = L" ";
	UnicodeString Message;
	TMemoryStream * MS;
	TFileStream *fs;

 //Settings

	 #ifdef __ANDROID__
	 Dir =  System::Ioutils::TPath::GetSharedDownloadsPath();
	#endif

	#ifndef __ANDROID__
	Dir =  System::Ioutils::TPath::GetDocumentsPath();
	#endif

		fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "Settings" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";


	// Если пользовательского не существует.
	if (!FileExists(fname)) {
	 // Создаем хотя бы директорию


	  //Используем временные файлы
	#ifdef __ANDROID__
	Dir =  System::Ioutils::TPath::GetPublicPath();
	#endif

	#ifndef __ANDROID__
	Dir =  System::Ioutils::TPath::GetDocumentsPath();
	#endif

	if (!DirectoryExists(Dir)) {
		if (!DirectoryExists(Dir)) {
		 if (!CreateDir(Dir))
		 {
		 throw Exception(L"Cannot create directory.");
		 return;
		 }
		}
		}

		fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "Settings" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";

		TSettingsClass::Dir = Dir;

	if (!FileExists(fname))
	{

	 // в AURL находится HTTP путь к файлу

	__try
	{
	__try
	{



		  //	ShowMessage(L"Файлы загружаем с сервера.");



		fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "Settings" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";

		//fs = new TFileStream (fname,fmCreate);
		URLadress = L"https://getfile.dokpub.com/yandex/get/https://disk.yandex.ru/d/1bDyWrjWBMgtOw";
		try {

			//TDownloadURL::DownloadRawBytes(URLadress,fs);
			MS = new TMemoryStream;
			NetHTTPClient1->Get(URLadress, MS);
			MS->SaveToFile(fname);
			delete MS;

		} catch (...) {
		   //
		   //DeleteFile(fname);
		   ShowMessage(L"Ошибка соединения. Файлы не загружены с сервера.");
		   Message = L"Настройки с сервера не загружены";
		  //	throw Exception(Message);
		   delete MS;
		   return;
		}

		// fs->Free();

		fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "MeterTypes" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";

		if (!FileExists(fname))
		{
	   //	fs = new TFileStream (fname,fmCreate);
		URLadress = L"https://getfile.dokpub.com/yandex/get/https://disk.yandex.ru/d/HliWcqGqtaENog";
				try {
		 //	TDownloadURL::DownloadRawBytes(URLadress,fs);
			MS = new TMemoryStream;
			NetHTTPClient1->Get(URLadress, MS);
			MS->SaveToFile(fname);
			delete MS;

		} catch (...) {
		   //fs->Free();
		   //DeleteFile(fname);
					   Message = L"Типы с сервера не загружены";
		   //	throw Exception(Message);
			delete MS;
          ShowMessage(L"Ошибка соединения. Файлы не загружены с сервера.");
		   return;
		}
	 //	fs->Free();
		}


		/*
		fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "1" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "wav";


		if (!FileExists(fname))
		{
//		fs = new TFileStream (fname,fmCreate);
		URLadress =  L"https://getfile.dokpub.com/yandex/get/https://disk.yandex.ru/d/ZZcjDjCsWkW9Bg";
						try {
//			TDownloadURL::DownloadRawBytes(URLadress,fs);
			NetHTTPClient1->Get(URLadress, MS);
			MS->SaveToFile(fname);

		} catch (...) {
//		   fs->Free();
//		   DeleteFile(fname);
					   Message = L"Звук с сервера не загружены";
			throw Exception(Message);
		   return;
		}
  //		fs->Free();
		}

		   */












					ShowMessage(L"Файлы загружены с сервера.");



	}
	__finally
	{
		//delete fs;

	}
	}
	__except(true)//(GetExceptionCode())
	{
		ShowMessage(L"Ошибка соединения. Файлы не загружены с сервера.");
	}

	}

	}

		TSettingsClass::Dir = Dir;
 }


// ---------------------------------------------------------------------------
__fastcall TMainTabbedForm::TMainTabbedForm(TComponent* Owner) : TForm(Owner) {

btPermissionGranted = false;
	camPermissionGranted = false;

	TimerStop =new TTimer(NULL);




	deviceState = HSC_STATE_STANDBY;
	try {
		TryFilesDowmload();
  	} catch (...)
	{
	}

	try {
	TSettingsClass::LoadFromFile();
	} catch (...)
	{
	}

	try {
	TOrderClass::LoadFromFile();
	} catch (...)
	{
	}


	Value = new TMeterValue ();
	Value->SetAsFlowRate();

	Value = new TMeterValue ();
	Value->SetAsFlowRate();

	// Debug = new TDebug();
	//
	// Создание и настройка объекта класса HSC
	HSC = new THscDevice();

	HSC->SetOnSecDataRead(OnSecDataRead);
	HSC->SetOnDataRead(OnDataRead);
	HSC->SetOnFinalDataRead(OnFinalDataRead);
	HSC->SetOnDataSent(OnDataSent);
	HSC->SetOnConfigRead(OnConfigRead);
	HSC->SetOnChargeRead(OnChargeRead);
	HSC->SetOnImpDataRead(OnImpDataRead);
	HSC->SetOnWidthRead(OnWidthRead);
	// Назначение обработчиков событий
	// HSC->OnDiscoverEnd(Bluetooth1DiscoveryEnd);          //< При завершении сканирования
	// HSC->OnWdtOverflow(DiscoverWdtOverflow);             //< При превышении времени ожидания сканирования
	HSC->SetOnBluetoothConnectMain(OnBluetoothConnectMain);
	// < При подключении к устройству Bluetooth
	HSC->SetOnBluetoothDisconnect(OnBluetoothDisconnect);
	// HSC->SetOnBluetoothDisconnect(OnBluetoothDisconnect);   //< При отключении Bluetooth устройства
	HSC->SetOnChangeState(OnHscDeviceChangeState);
	// < При смене состояния устройства
	// HSC->SetOnMeasureDataRecv(OnDataReceive);     //< Когда прилетела очередная порция данных измерения

	TFlowMeter::StaticInit(HSC);

	#ifdef __ANDROID__
   //	CheckStoragePermission();
	#endif



	try {
	TFlowMeterType::LoadTypesFromFile();
	} catch (...) {
	}

		try {
	TestMeter=TFlowMeter::LoadFromFile();
    	} catch (...) {
	}

	EtalonMeter = new TFlowMeter(HSC, true);
	EtalonMeter->SetChannel(3);
	EtalonMeter->Name = "Etalon";
	EtalonMeter->IsEtalon = true;

	EtalonMeter->SetKoef(100);



	if (TestMeter == nullptr) {
		TestMeter = new TFlowMeter(HSC, false);
		TestMeter->SetChannel(1);
		TestMeter->Name = "Test";

	}

	TestMeter->SetEtalon(EtalonMeter);


	UpdateDeviceChange();

	inte = 0;
	// ListView1->add

	Anim1->Enabled = false;
	Anim2->Enabled = true;
	TestButton->Text = L"Измерение";


	RefreshCond(0);
}

// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::OnDataSent(uint8_t callback) {
	if (TestMeter->CheckType == 0) {
		// Отправлен запуск измерения
		if (callback == 1) {
			PhotoCaptureForm->ChangeState(2);

		}
		// Отправоен остановка измерения
		else if (callback == 2) {
			PhotoCaptureForm->ChangeState(0);
		}
	}
}

// ---------------------------------------------------------------------------
void __fastcall TMainTabbedForm::FormCreate(TObject *Sender) {
	// This defines the default active tab at runtime
	if (!(TestMeter == nullptr)) {
		TestMeterKoefEdit->Text = FloatToStr(TestMeter->GetKoef());
	}

		RefreshCond(0);
}
// ---------------------------------------------------------------------------

	void __fastcall TMainTabbedForm::ConnectionUp(TObject * Sender)
{
	  SwitchConnection(true);
}

	void TMainTabbedForm::SwitchConnection(bool state)
{
Integer status;



		if (state) {

		DeviceSelectionForm->Init(HSC);
		status = HSC->Connect();

		if (status == 1) {
			MainTabbedForm->ActiveBlueToothLabel->Text = L"Подключение";
		  //	MainTabbedForm->ActiveBlueToothLabel->TextSettings->FontColor =
		  //		claGray;
		}
		else if (status == 2) {
			MainTabbedForm->ActiveBlueToothLabel->Text =
				L"Нет Устройств";// + IntToStr(status);
		  //	MainTabbedForm->ActiveBlueToothLabel->TextSettings->FontColor
		   //		= claRed;
		}
		else if (status == 3) {
			MainTabbedForm->ActiveBlueToothLabel->Text =
				L"Нет HSC Устр-в ";// + IntToStr(status);
		   //	MainTabbedForm->ActiveBlueToothLabel->TextSettings->FontColor
		   //		= claRed;
		}
		else if (status == 0) {
			MainTabbedForm->ActiveBlueToothLabel->Text =
				L"Выключен BlueTooth" ;//+ IntToStr(status);
		   //	MainTabbedForm->ActiveBlueToothLabel->TextSettings->FontColor
			//	= claRed;
		}

	}
	else {
		MainTabbedForm->ActiveBlueToothLabel->Text = L"Отключение";
		MainTabbedForm->ActiveBlueToothLabel->TextSettings->FontColor =
			claWhite;
		HSC->Disconnect();
	}

}


void __fastcall TMainTabbedForm::SwitchConnectedClick(TObject *Sender) {

		SwitchConnection(SwitchConnected->IsChecked);

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Button11Click(TObject *Sender) {

	TListViewItem *Item;
	Item = ListView1->Items->Add();
	// ListView1->ItemIndex = ListView1->Items->Count-1;
	// Item = ListView1->Items[ListView1->ItemIndex];
	Item->Text = L"Имя Фамилия";
	Item->Detail = L"адрес";

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::AddTypeButtonClick(TObject *Sender) {

	TypeDeviceUpdate();

	float f;
	TFlowMeterType *MeterType;

	MeterType = new TFlowMeterType();

	MeterType->DeviceType = EditDeviceType->Text;
	MeterType->Modification = EditModification->Text;
	//
	// Серийный номер поверяемого устройства
	MeterType->SerialNum = SerialNumEdit->Text;
	//
	// Номер ГРСИ поверяемого устройства
	MeterType->CertificateNum = CertificateNumEdit->Text;

	// Коэффициент преобразования
	if (TryStrToFloat_(KpEdit1->Text, f)) {
		MeterType->Kp = f;
	}
	else {
		KpEdit1->TextSettings->FontColor = 256;
	}

	if (TryStrToFloat_(QmaxEdit->Text, f)) {
		MeterType->Qmax = f;
	}
	else {
		KpEdit1->TextSettings->FontColor = 246;
	}

	MeterType->AddPoints(TestMeter->Points);

	MeterType->AddTypeToList();

	MeterType->SaveTypesToFile();

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Button3Click(TObject *Sender) {
	// HSCSettingsForm->
	// DebugForm->Debug->AddMSG (" Open Settings");
	Debug->AddMSG(L"Open Settings");
	HSCSettingsForm->SetHscDevice(HSC);
	HSCSettingsForm->SetFlowMeter(EtalonMeter);
	HSCSettingsForm->SetTestMeter(TestMeter);
	HSCSettingsForm->Show();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::DevicesButtonClick(TObject *Sender) {
	// TDeviceSelectionForm DeviceSelectionForm = new TDeviceSelectionForm(TObject *Sender, );
	DeviceSelectionForm->Init(HSC);
	DeviceSelectionForm->Show();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::OnBluetoothDisconnect(TObject *Sender) {

	if (DeviceSelectionForm->StatusLabel != NULL) {
		DeviceSelectionForm->StatusLabel->Text = L"Отключено устройство";
		DeviceSelectionForm->Spinner->Enabled = false;
		DeviceSelectionForm->Spinner->Visible = false;
		DeviceSelectionForm->DiscoverDevicesList->Enabled = true;
		DeviceSelectionForm->ButtonConnect->Text = L"Подключить";


		ActiveBlueToothLabel->Text = L"Не подключено";
		ActiveBlueToothLabel->TextSettings->FontColor = claGrey;
		SwitchConnected->IsChecked = False;
	 }


}

void __fastcall TMainTabbedForm::OnBluetoothConnectMain(TObject *Sender,
	bool connected) {
	if (connected) {
		SwitchConnected->IsChecked = True;
		ActiveBlueToothLabel->Text = L"Активно";
		TestButton->Tag = 1;

		TestButton->Enabled = true;
		ButtonMonitor->Enabled = true;

		PhotoCaptureForm->TestButton->Tag = 1;
		PhotoCaptureForm->ButtonMonitor->Enabled = true;

	}
	else {
		 SwitchConnected->IsChecked = False;
		ActiveBlueToothLabel->Text = L"Отключено";
		ActiveBlueToothLabel->TextSettings->FontColor = claGrey;
		TestButton->Tag = 0;

		TestButton->Enabled = false;
		ButtonMonitor->Enabled = false;

		PhotoCaptureForm->TestButton->Tag = 0;
	}
}

void __fastcall TMainTabbedForm::StopMesurment() {

	TimeEdit->Enabled = True;
	VolumeEdit->Enabled = True;
	 TestButton->Tag = 1;
	PreviousPointButton->Enabled = True;
	NextPointButton->Enabled = True;

	TestButton->Text = L"Остановка";
	PhotoCaptureForm->TestButton->Text = L"Остановка";
	PhotoCaptureForm->TestButton->Tag = 3;
	PhotoCaptureForm->TimeEdit->Enabled = True;

	HSC->StopMeasure();
}

void __fastcall TMainTabbedForm::StartMesurment() {

	// if (TryStrToInt())
	avrflowEtalon = 0;

	TestMeter->ResetTest();
	EtalonMeter->ResetTest();
    TestMeter->SetEtalon(EtalonMeter);


	EditAfter->Text = "";
    LabelTime->Text = "";
	TestButton->Text = L"Запуск";
	TimeEdit->Enabled = False;
	VolumeEdit->Enabled = False;
//	TestButton->Tag = 2;

	PreviousPointButton->Enabled = False;
	NextPointButton->Enabled = False;

	ProgressBar->Value = 0;
	ProgressBar->Max = StrToFloat_(TimeEdit->Text);



	if (TestMeter->CheckType == 0) {
		HSC->StartMeasureInstant(StrToInt_(TimeEdit->Text));
	}

	else if (TestMeter->CheckType == 1) {
		HSC->StartMeasure(StrToInt_(TimeEdit->Text));
	}
	else if (TestMeter->CheckType == 3) {
		HSC->StartMeasure(StrToInt_(TimeEdit->Text));
	}
	else if (TestMeter->CheckType == 4) {

	}

	else

	{
		HSC->StartMeasureInstant(StrToInt_(TimeEdit->Text));
	}

	inte = 0;

	PhotoCaptureForm->TestMeter = TestMeter;
	PhotoCaptureForm->EtalonMeter = EtalonMeter;

	PhotoCaptureForm->TestButton->Text = L"Запуск";
	PhotoCaptureForm->TimeEdit->Enabled = False;
	// VolumeEdit -> Enabled 	= False;

	// state = 0;

	PhotoCaptureForm->ProgressBar->Value = 0;
	PhotoCaptureForm->ProgressBar->Max = StrToFloat(TimeEdit->Text);
	PhotoCaptureForm->SpillTimer->Interval = StrToInt(TimeEdit->Text) * 1000;

	PhotoCaptureForm->StringGridTest->Cells[2][0] = "";
	PhotoCaptureForm->StringGridTest->Cells[3][0] = "";

	PhotoCaptureForm->EditBefore->Text = "";
	PhotoCaptureForm->EditAfter->Text = "";

}



	void __fastcall TMainTabbedForm::StopMonitor()
{
		ButtonMonitor->Tag = 0;
		Anim2->Enabled = true;
		Anim1->Enabled = false;
		Glow1->Enabled = false;
		HSC->StopMonitor();
}
	void __fastcall TMainTabbedForm::StartMonitor()
{
			ButtonMonitor->Tag = 1;

		TestMeter->ResetTest();
		EtalonMeter->ResetTest();
		TestMeter->SetEtalon(EtalonMeter);

		EtalonMeter->FlowValue->SetFilter(8);

		Anim2->Enabled = false;
		Anim1->Enabled = true;

		HSC->StartMonitor();
}

void __fastcall TMainTabbedForm::TestButtonClick(TObject *Sender) {
	//
	int tag;

	tag = TestButton->Tag;

	if ((TestMeter->CheckType > 0)&&(TestMeter->CheckType < 3)) {

		if (tag == 1) {
			if (TestMeter->CheckType == 2) {
			  	TestMeter->Write_Channel_State(0);
			}
			// MainTimer->	Interval = 1000;
			// MainTimer-> Enabled = True;


			StartMesurment();
			PhotoCaptureForm->TestButton->Tag = 2;
			TestButton->Tag = 2;
		}
		else if (tag == 2) {
			StopMesurment();
			TestButton->Tag = 1;
		}

		// Сохранение точки
		else if (tag == 3) {
			AfterSpillage();
			TestMeter->SaveDataPoint(EtalonMeter);

			TestButton->Text = L"Измерение";

			TimeEdit->Enabled = True;
			VolumeEdit->Enabled = True;
			TestButton->Tag = 1;

			TThread::Synchronize(NULL, TestButton->Repaint);
			TThread::Synchronize(NULL, StringGridTest->Repaint);

			CancelButton->Visible = false;

			UpdateDeviceChange();

			// Calibration
			if (HSC->CalibrState) {

				EtalonMeter->AddCalibrData(volumeTest, time, impEtalon,
					EtalonMeter->GetImpCoef());

			}
			TFlowMeter::SaveToFile(TestMeter);
		}

	}
	 // Ручной ввод
	else if (TestMeter->CheckType == 3) {

	if (TestButton->Tag == 1) {
			StartMesurment();
			PhotoCaptureForm->TestButton->Tag = 2;
			TestButton->Tag = 2;
		}
		else if (TestButton->Tag == 2) {
			StopMesurment();
			TestButton->Tag = 1;
		}

		// Сохранение точки
		else if (TestButton->Tag == 3) {

			AfterSpillage();

			TestMeter->SaveDataPoint(EtalonMeter);

			TestButton->Text = L"Измерение";

			TimeEdit->Enabled = True;
			VolumeEdit->Enabled = True;
			TestButton->Tag = 1;

			TThread::Synchronize(NULL, TestButton->Repaint);
			TThread::Synchronize(NULL, StringGridTest->Repaint);

			CancelButton->Visible = false;

			UpdateDeviceChange();

			// Calibration
			if (HSC->CalibrState) {

				EtalonMeter->AddCalibrData(volumeTest, time, impEtalon,
					EtalonMeter->GetImpCoef());

			}

		}

	}

	 // Фотофиксация
	else if (TestMeter->CheckType == 0) {
		PhotoCaptureForm->SetHscDevice(HSC);
		PhotoCaptureForm->TestMeter = TestMeter;
		PhotoCaptureForm->EtalonMeter = EtalonMeter;
		PhotoCaptureForm->TimeEdit->Text = TimeEdit->Text;

		TestMeter->temperature = TempEdit->Text + L" град. С";
		TestMeter->tempWater = TempWaterEdit->Text + L" град. С";



		if (TryStrToFloat_(PressureTemp->Text,f)) {
			f = f /10;

		 TestMeter->pressure =FloatToStrF(f,
					ffNumber, 10, 1) + L" кПа";

		}  else
		{

		   TestMeter->pressure =L"101,7 кПа";
		}

		TestMeter->hymidity = HumidityEdit->Text+ L"%";



		PhotoCaptureForm->Show();
	}
	 // Фото сличение
	else if (TestMeter->CheckType == 4) {
		if (ButtonMonitor->Tag==1 )
		{
		ButtonMonitor->Tag = 0;
		Anim2->Enabled = true;
		Anim1->Enabled = false;
		Glow1->Enabled = false;
		}
		SwitchConnection(false);
		PhotoFixForm->SetOnTurnUpConnection(ConnectionUp);
	   //	PhotoFixForm->SetHscDevice(nullptr);
		PhotoFixForm->TestMeter = TestMeter;
		PhotoFixForm->EtalonMeter = EtalonMeter;
		PhotoFixForm->TimeEdit->Text = TimeEdit->Text;
		PhotoFixForm->TestButton->Tag = 1;

		TestMeter->temperature = TempEdit->Text + L" град. С";
		TestMeter->tempWater = TempWaterEdit->Text + L" град. С";



		if (TryStrToFloat_(PressureTemp->Text,f)) {
			f = f /10;

		 TestMeter->pressure =FloatToStrF(f,
					ffNumber, 10, 1) + L" кПа";

		}  else
		{

		   TestMeter->pressure =L"101,7 кПа";
		}

		TestMeter->hymidity = HumidityEdit->Text+ L"%";



		PhotoFixForm->Show();
	}


}

// ---------------------------------------------------------------------------
void __fastcall TMainTabbedForm::OnHscDeviceChangeState(TObject *Sender) {
	// THscDevice *HSC = Sender;

	switch (HSC->hscState) {
	case HSC_STATE_STANDBY: // < Хуем груши околачиваем.... :-)
		break;

	case HSC_STATE_CONFIG: // < Команда остановки измерении
		ActiveBlueToothLabel->Text = L"Конфигурация";
		// ActiveBlueToothLabel->TextSettings->FontColor=claGreen;
		break;

	case HSC_STATE_PING
			: // < Подключились, мониторим подключение, ждем команды юзера
		{
			ActiveBlueToothLabel->Text = L"Подключено";
			ActiveBlueToothLabel->TextSettings->FontColor = claGreen;
		} break;

	case HSC_STATE_STARTTEST: // < Запуск измерения
		{
			ActiveBlueToothLabel->Text = L"Измерение";

		} break;

	case HSC_STATE_EXECUTE: // < Измерение идёт
		{
			TestButton->Text = L"СТОП";

		} break;

	case HSC_STATE_STOPTEST: // < Команда остановки измерении
		break;

	case HSC_STATE_COMPLETE: // < Измерение закончено
		{

			if (TestMeter->CheckType == 3) {
		TestButton->Text = L"Введите данные";
		TestButton->Tag = 5;
		CancelButton->Visible = true;
									 }
			else if (TestMeter->CheckType == 0)
				{
		   TestButton->Text = L"Измерение";
				}

		   else{
		   TestButton->Text = L"Обработка";
				}

			  {


			PhotoCaptureForm->ChangeState(0);
					   }
			HSC->Read_FinalTime();
			HSC->Read_FinalData();

		} break;

	case HSC_STATE_FINALREAD: // < Измерение закончено
		{

			tDataPoint DataPoint;

			DataPoint = TestMeter->GetDataPoint();

			time = HSC->GetFinalTime();
			flowEtalon = EtalonMeter->GetFinalFlow();
			volumeEtalon = EtalonMeter->GetFinalVolume();

			imp = EtalonMeter->GetFinalImp();
			impEtalon = imp;
			impTest = TestMeter->GetFinalImp();

			flowTest = TestMeter->GetFinalFlow();
			volumeTest = TestMeter->GetFinalVolume();
			error = (volumeTest - volumeEtalon) / volumeEtalon;

			TThread::Synchronize(NULL, DataReadFormsFinalUpdate);

			PhotoCaptureForm->time = time;
			PhotoCaptureForm->flowEtalon = flowEtalon;
			PhotoCaptureForm->volumeEtalon = volumeEtalon;
			PhotoCaptureForm->volumeTest = volumeTest;
			PhotoCaptureForm->flowTest = flowTest;
			PhotoCaptureForm->error = error;

			if (TestMeter->CheckType== 0)
				TThread::Synchronize(NULL,
				PhotoCaptureForm->DataReadFormsFinalUpdate);

			if (TestMeter->CheckType == 2)
				TThread::Synchronize(NULL,
				PhotoCaptureForm->DataReadFormsFinalUpdate);

			// TThread::Synchronize(NULL,PhotoCaptureForm->DataReadFormsUpdate);

			AfterSpillage();

			HSC->ChangeState(HSC_STATE_PING);

		} break;

	case HSC_STATE_FAILURE
			: // < Произошел сбой, обработка ошибки работы устройства
		{
			ActiveBlueToothLabel->Text = L"Нет связи";
			ActiveBlueToothLabel->TextSettings->FontColor = claRed;

			TestButton->Text = L"Измерение";

			TimeEdit->Enabled = true;
			VolumeEdit->Enabled = true;

			//AfterSpillage();

		} break;

	default:
		break;
	}
}

void TMainTabbedForm::AfterSpillage(void) {
	// Сличение
	if (TestMeter->CheckType == 3)
  {
	if ((TestButton->Tag == 5)||(TestButton->Tag == 3))
		{
		float f, fa, fb, q, v;

		if ((TryStrToFloat_(EditBefore->Text, fb)) && (EditBefore->Text != ""))
		{
			if ((TryStrToFloat_(EditAfter->Text, fa)) &&
				(EditAfter->Text != ""))

			{
			   if (ComboBoxUnits->ItemIndex==0) {
			   f = (fa - fb);
		   }
				else if (ComboBoxUnits->ItemIndex==1)
		   {
			  f = (fa*(1000) - fb*(1000));
		   }
				else
		   {
			   f = (fa - fb);
		   }



				v = (f);

				TestMeter->DataPoint.Volume = v;
				volume = v;
				volumeTest = v;

				StringGridTest->Cells[2][0] = FloatToStrF(v, ffNumber, 10, 2);

				error = ((v - volumeEtalon) * 100) / v;

				StringGridTest->Cells[3][0] = FloatToStrF(error, ffNumber, 10, 2);

				q = (TestMeter->DataPoint.Volume * 3600) / time;

				StringGridTest->Cells[1][0] = FloatToStrF(q, ffNumber, 10, 2);
				/*
				 StringGridTest->Cells[0][0] 	= TestMeter->Name;
				 StringGridTest->Cells[1][0] 	= FloatToStrF(TestMeter->GetFinalFlow(),10,2);
				 StringGridTest->Cells[2][0] 	= FloatToStrF(TestMeter->GetFinalVolume(),10,2);
				 StringGridTest->Cells[3][0] 	= FloatToStrF(100*(TestMeter->GetFinalVolume()-
				 EtalonMeter->GetFinalVolume())/TestMeter->GetFinalVolume(),10,2)+L"%";
				 */

				TestButton->Text = L"Сохранить";
				TestButton->Tag = 3;
				CancelButton->Visible = true;



				TestMeter->DataPoint.Error= error;
				TestMeter->DataPoint.VolumeBefore=fb;
				TestMeter->DataPoint.VolumeAfter=fa;
				TestMeter->DataPoint.Volume = v;

				TestMeter->temperature = TempEdit->Text + L" град. С";
				TestMeter->tempWater = TempWaterEdit->Text + L" град. С";

				if (TryStrToFloat_(PressureTemp->Text,f)) {
				f = f /10;
				TestMeter->pressure =FloatToStrF(f,ffNumber, 10, 1) + L" кПа";
				}  else
				{
					 TestMeter->pressure =L"101,7 кПа";
				}

				TestMeter->hymidity = HumidityEdit->Text+ L"%";





				TThread::Synchronize(NULL, TestButton->Repaint);
				TThread::Synchronize(NULL, StringGridTest->Repaint);

			}
			else

			{
				// ShowMessage(L"Ошибка заполнения данных");
				// EditAfter->TextSettings->FontColor=claRed;
				// EditAfter->Repaint();
			}

		}
		else {
			// EditBefore->TextSettings->FontColor=claRed;
			// EditBefore->Repaint();
		}
	 }
	}

	else if (TestMeter->CheckType == 1) {
		TestButton->Text = L"Сохранить";
		TestButton->Tag = 3;
		CancelButton->Visible = true;
	}
	else if (TestMeter->CheckType == 2) {
		TestButton->Text = L"Сохранить";
		TestButton->Tag = 3;
		CancelButton->Visible = true;
	}
	else if (TestMeter->CheckType == 0) {
		TestButton->Text = L"Измерение";
		TestButton->Tag = 0;
		CancelButton->Visible = false;
	}

	}


void TMainTabbedForm::AfterHandSpillage(void) {
	TestButton->Text = L"Сохранить";

	TimeEdit->Enabled = True;
	VolumeEdit->Enabled = True;
	TestButton->Tag = 3;

	TThread::Synchronize(NULL, TestButton->Repaint);
	TThread::Synchronize(NULL, StringGridTest->Repaint);

	UpdateDeviceChange();
}

void TMainTabbedForm::MesureDataUpdate(void) {

}

void TMainTabbedForm::OnVersionRead(TObject * Sender) {

	if (HSC->Version.Protocol>43537) {

	EtalonMeter->SetChannel(1);

	if (TestMeter != nullptr) {
		TestMeter->SetChannel(2);
   }

   }
}

void TMainTabbedForm::OnConfigRead(TObject * Sender) {

#ifdef __ANDROID__
	if (!IsInfinite(HSC->Kp)) {

		if (HSC->Kp > 0) {
			// EtalonImpCoefEdit->Text = FloatToStr(HSC->Kp);

			EtalonMeter->SetKoef(HSC->Kp);
		}
	}

#endif

#ifndef __ANDROID__
	if (_finite(HSC->Kp)) {

		if (HSC->Kp > 0) {
			// EtalonImpCoefEdit->Text = FloatToStr(HSC->Kp);
			EtalonMeter->SetKoef(HSC->Kp);
		}
	}

#endif

	EtalonMeter->ClearCalibraion();

	// CoefStringGrid->Cells[6][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Error,ffNumber,10,1);

	for (int i = 0; i < 20; i++) {
		EtalonMeter->AddCalibrData(HSC->cEtlVolume[i], HSC->cTime[i],
			HSC->cImp[i], HSC->cCoef[i]);
	}

	EtalonMeter->UpdateCoefs();


}

void __fastcall TMainTabbedForm::OnSecDataRead(TObject *Sender) {
	// LabelTime->Text = IntToStr(SecData.Time);
}

void __fastcall TMainTabbedForm::OnImpDataRead(TObject *Sender) {


	EtalonMeter->SetMonitorValues();
	TestMeter->SetMonitorValues();


	flowEtalon = EtalonMeter->GetFlow();
	flowTest = TestMeter->GetFlow();

	// if (avrflowEtalon==0) {
	//avrflowEtalon = flowEtalon;
	// } else
	// {
	// avrflowEtalon = (avrflowEtalon+ flowEtalon)/2;
	// }
	//
	volumeEtalon = EtalonMeter->GetFlowVolume();
	volume = EtalonMeter->GetFlowVolume();
	volumeTest = TestMeter->GetFlowVolume();



	TThread::Synchronize(NULL, DataReadFormsUpdate);

	if (TestMeter->CheckType == 0) {

	PhotoCaptureForm->flowEtalon = flowEtalon;
	PhotoCaptureForm->flowTest = flowTest;
	PhotoCaptureForm->time = PhotoCaptureForm->time + 1;
	PhotoCaptureForm->volume = volume;
	PhotoCaptureForm->error = error;

	TThread::Synchronize(NULL, PhotoCaptureForm->DataReadFormsUpdate);
	PhotoCaptureForm->Glow1->Enabled = true;
	}

	 else if (TestMeter->CheckType == 4) {

	   if ((EtalonMeter->FlowValue->IsStable(10))&&(ShowStableFlow == 1))
		{
			//  ShowMessage(L"Расход зафиксирован")
		  // TThread::Synchronize(NULL, 	);

		 ShowStableFlow = 2;

	//	}
	  //		);



			 TestButton->Enabled = true;
		}   else
		{
			TestButton->Enabled = false;

			if (time>120) {
			   ShowStableFlow = 3;
			 // TThread::Synchronize(NULL, 	);
			  TestButton->Enabled = true;
			}

		}



		}


		if (ButtonMonitor->Tag ==1) {
		Glow1->Enabled = true;
		//Glow1->Enabled = false;
		//Anim1->Enabled = true;
		//Anim2->Enabled = false;
	   //ButtonMonitor->Tag = 2;
	}


      	time = time + 1;
}

void __fastcall TMainTabbedForm::OnChargeRead(TObject *Sender) {
	ChargeLabel->Text = HSC->GetCharge();
	ChargeProgressBar->Value = HSC->GetValueCharge();
}

void __fastcall TMainTabbedForm::OnWidthRead(TObject *Sender) {



	 /*

	time = time + 1;

	flowEtalon = EtalonMeter->GetWidthFlow();
	flowTest = TestMeter->GetWidthFlow();

	// if (avrflowEtalon==0) {
	avrflowEtalon = flowEtalon;
	// } else
	// {
	// avrflowEtalon = (avrflowEtalon+ flowEtalon)/2;
	// }
	//
	volumeEtalon = EtalonMeter->GetWidthFlow() * time;
	volume = EtalonMeter->GetWidthFlow() * time;
	volumeTest = TestMeter->GetWidthFlow() * time;

	PhotoCaptureForm->flowEtalon = flowEtalon;
	PhotoCaptureForm->flowTest = flowTest;
	PhotoCaptureForm->time = PhotoCaptureForm->time + 1;
	PhotoCaptureForm->volume = volume;
	PhotoCaptureForm->error = error;

	TThread::Synchronize(NULL, DataReadFormsUpdate);

	if (SpillageTypeComboBox->ItemIndex == 0) {
		TThread::Synchronize(NULL, PhotoCaptureForm->DataReadFormsUpdate);
	}

	Glow1->Enabled = false;
	Anim1->Enabled = true;
	Anim2->Enabled = false;

	PhotoCaptureForm->Glow1->Enabled = false;
	PhotoCaptureForm->Anim1->Enabled = true;
	PhotoCaptureForm->Anim2->Enabled = false;

	*/
}

void __fastcall TMainTabbedForm::OnDataRead(TObject *Sender) {
	float f, fa;

	 if ( HSC->hscState == HSC_STATE_EXECUTE) {

	time = HSC->GetTime();
	PhotoCaptureForm->time = time;

	EtalonMeter->SetValues();
	TestMeter->SetValues();

	volumeEtalon = EtalonMeter->GetVolume();

	   //Values Migration

/*
flowEtalon = EtalonMeter->GetFlow();

	if (avrflowEtalon == 0) {
		avrflowEtalon = flowEtalon;
	}
	else {
		avrflowEtalon = (avrflowEtalon + flowEtalon) / 2;
}



	flowTest = TestMeter->GetFlow();

	volume = TestMeter->GetVolume();
	volumeTest = TestMeter->GetVolume();

	imp = TestMeter->GetTotalImp();
	impTest = imp;
	impEtalon = EtalonMeter->GetTotalImp();

	PhotoCaptureForm->time = time;
	PhotoCaptureForm->flowEtalon = flowEtalon;
	PhotoCaptureForm->volume = volumeEtalon;
	PhotoCaptureForm->flowTest = flowTest;
	PhotoCaptureForm->error = error;
	*/
	TThread::Synchronize(NULL, DataReadFormsUpdate);

	if (TestMeter->CheckType == 0) {
		TThread::Synchronize(NULL, PhotoCaptureForm->DataReadFormsUpdate);
	}

	int add_time = EtalonMeter->TimeToEndVolumeLimit(TestMeter->Point.Volume, 1.6, 1.7);

	{
		if (add_time) {
			TimerStop->Interval = add_time;
			TThread::Synchronize(NULL,StopTimerEnabled);
		}

		if ((EtalonMeter->GetTotalImp() > TestMeter->Point.Imp) &&
			(TestMeter->Point.Imp > 0))
		{
			TThread::Synchronize(NULL, StopMesurment);
		}

		  if (time == TestMeter->Point.Time-10) {


		if (TSettingsClass::StopSoundFile.Length()>4)
		{
			try {

		UnicodeString 	fname;

			fname = TSettingsClass::Dir +
					System::Ioutils::TPath::DirectorySeparatorChar + TSettingsClass::StopSoundFile;

			MediaPlayer1->FileName = fname;

			if(MediaPlayer1->Media!=NULL)
			  {
				 MediaPlayer1->Play();
			  }
					} catch (...) {
			}
		}
		   }

	}
  }

   /*	if (TestMeter->CheckType == 3)
	{
		 if (TestMeter->FlowValue-> ) {

		 }
	 }   */
}

void __fastcall  TMainTabbedForm::StopTimerEnabled()
{
	 TimerStop->Enabled = True;
}


void __fastcall TMainTabbedForm::DataReadFormsFinalUpdate(void) {

	LabelTime->Text = FloatToStrF(time, ffNumber, 10, 2);

	LabelFlow->Text = EtalonMeter->FlowValue->GetStringValue();//FloatToStrF(flowEtalon, ffNumber, 10, 2);

	if (LabelLimit->Tag == 1) {
		LabelVolume->Text = EtalonMeter->ImpValue->GetStringValue();//FloatToStrF(impEtalon, ffNumber, 10, 3);
	}
	else {
		LabelVolume->Text = EtalonMeter->VolumeValue->GetStringValue();//FloatToStrF(volumeEtalon, ffNumber, 10, 3);
	}
	// FlowEdit->Text   =   FloatToStrF(imp,ffNumber,10,2);

	StringGridTest->Cells[0][0] = TestMeter->SerialNum;
	StringGridTest->Cells[1][0] = TestMeter->FlowValue->GetStringValue();//FloatToStrF(flowTest, ffNumber, 10, 2);
	StringGridTest->Cells[2][0] = TestMeter->VolumeValue->GetStringValue();//FloatToStrF(volume, ffNumber, 10, 2);

	StringGridTest->Cells[3][0] = FloatToStrF(100 * (error), ffNumber, 10, 2);

	ProgressBar->Value = (int)time; // (HSC->GetTime());
	 /*
	StringGridTest->Repaint();
	LabelFlow->Repaint();
	LabelTime->Repaint();
	LabelVolume->Repaint();
	*/
	TThread::Synchronize(NULL,StringGridTest->Repaint);
	TThread::Synchronize(NULL,LabelTime->Repaint);
	TThread::Synchronize(NULL,TestButton->Repaint);
	TThread::Synchronize(NULL,CancelButton->Repaint);
	TThread::Synchronize(NULL,LabelFlow->Repaint);
	TThread::Synchronize(NULL,LabelVolume->Repaint);


}

void __fastcall TMainTabbedForm::DataReadFormsUpdate(void) {

	LabelTime->Text = FloatToStrF(time, ffNumber, 10, 0);
	LabelFlow->Text = EtalonMeter->FlowValue->GetStringValue();  //FloatToStrF(avrflowEtalon, ffNumber, 10, 2);

	if (LabelLimit->Tag == 1) {
		LabelVolume->Text = EtalonMeter->ImpValue->GetStringValue();//FloatToStrF(impEtalon, ffNumber, 10, 2);
	}
	else {
		LabelVolume->Text = EtalonMeter->VolumeValue->GetStringValue();   //FloatToStrF(volumeEtalon, ffNumber, 10, 2);
	}
	// str[inte++] = LabelFlow->Text ;
	// temp[inte][1] = volume;
	// temp[inte][2] = flowEtalon;

	StringGridTest->Cells[0][0] = TestMeter->SerialNum;
	StringGridTest->Cells[1][0] = TestMeter->FlowValue->GetStringValue();     //FloatToStr(flowTest);
	StringGridTest->Cells[2][0] = TestMeter->VolumeValue->GetStringValue();//FloatToStrF(volumeTest, ffNumber, 10, 2);
	// temp[inte][0] = time;
	// temp[inte][3] = volume;
	// temp[inte++][4] = flowTest;

	StringGridTest->Cells[3][0] = FloatToStrF(100 * (error), ffNumber, 10, 2);

	ProgressBar->Value = (int)time; // (HSC->GetTime());




	StringGridTest->Repaint();
	LabelFlow->Repaint();
	LabelTime->Repaint();
	LabelVolume->Repaint();


	 if (ShowStableFlow==2) {

	  StopMonitor();
	  ShowStableFlow = 0;

	 TDialogServiceAsync::MessageDialog(L"Расход зафиксирован. Начать измерение?",
	 TMsgDlgType::mtInformation, mbYesNo, TMsgDlgBtn::mbYes, 0,

			[this](const System::Uitypes::TModalResult AResult)
		{Result = AResult;

			switch (Result) {

				case mrYes : {

		SwitchConnection(false);
		PhotoFixForm->SetOnTurnUpConnection(ConnectionUp);
		//PhotoFixForm->SetHscDevice(nullptr);
		PhotoFixForm->TestMeter = TestMeter;
		PhotoFixForm->EtalonMeter = EtalonMeter;
		PhotoFixForm->TimeEdit->Text = TimeEdit->Text;
		PhotoFixForm->TestButton->Tag = 1;
		PhotoFixForm->Show();



					}; break;

				case mrNo : {



					}; break;
			default :
				{

				}

				}



		}
	);
	 }
}

// ---------------------------------------------------------------------------
void __fastcall TMainTabbedForm::OnFinalDataRead(TObject *Sender) {
	// LabelTime->Text = IntToStr(SecData.Time);
	/* Before MeterValues integration       */
	/*
	LabelTime->Text = FloatToStrF(HSC->GetFinalTime(), ffNumber, 10, 2);
	LabelVolume->Text = FloatToStrF(EtalonMeter->GetFinalVolume(),
		ffNumber, 10, 2);
	LabelFlow->Text = FloatToStrF(EtalonMeter->GetFinalFlow(), ffNumber, 10, 2);

	StringGridTest->Cells[0][0] = TestMeter->SerialNum;
	StringGridTest->Cells[1][0] = FloatToStrF(TestMeter->GetFinalFlow(),
		ffNumber, 10, 2);
	StringGridTest->Cells[2][0] = FloatToStrF(TestMeter->GetFinalVolume(),
		ffNumber, 10, 2);
	StringGridTest->Cells[3][0] =
		FloatToStrF(100 * (TestMeter->GetFinalVolume() -
		EtalonMeter->GetFinalVolume()) / TestMeter->GetFinalVolume(), ffNumber,
		10, 2) + L"%";

	ProgressBar->Value = (int)(HSC->GetTime());

	if (TestMeter->CheckType != 0) {
		TestButton->Text = L"Сохранить";
		CancelButton->Visible = true;
		TestButton->Tag = 3;
	}
	 */
	EtalonMeter->SetFinalValues();
	TestMeter->SetFinalValues();
      /*
	TThread::Synchronize(NULL, DataReadFormsFinalUpdate);    */
}


void __fastcall TMainTabbedForm::TimeEditKeyDown(TObject *Sender, WORD &Key,
	System::WideChar &KeyChar, TShiftState Shift) {
	KeyDownCheck(Key, KeyChar);

	if (Key == 13U) {
		TimeEdit->ResetFocus();
	}

}
// ---------------------------------------------------------------------------
// bottom part of Check tab
void __fastcall TMainTabbedForm::UpdatePointChange() {

	tPoint Point;

	if (TestMeter!=nullptr) {



	Point = TestMeter->Point;

	TimeEdit->Text = IntToStr(Point.Time);
	VolumeEdit->Text = FloatToStrF(Point.Volume, ffNumber, 10, 2);
	FlowEdit->Text = FloatToStrF(Point.Q, ffNumber, 10, 2);
	PointLabel->Text = L"Поверочная точка: " + Point.Name;

	if (LabelLimit->Tag == 1) {
		LabelLimit->Text = L"Импульсы";
		VolumeEdit->Text = TestMeter->Point.Imp;
		LabelVolume->Text = EtalonMeter->ImpValue->GetStringValue();
	}
	else {
		LabelLimit->Text = L"Объем, л";
		VolumeEdit->Text = TestMeter->Point.Volume;
		LabelVolume->Text = EtalonMeter->VolumeValue->GetStringValue();
	}

	} else
{
        VolumeEdit->Text =  L"";
		LabelVolume->Text =  L"";
}

}

void __fastcall TMainTabbedForm::NextPointButtonClick(TObject *Sender) {
	if (TestMeter != NULL) {
		TestMeter->SetNextPoint();
	}

	UpdatePointChange();

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::RefreshDeviceButtonClick(TObject *Sender) {
	tPoint Point;
	bool empty;

	if (TestMeter->Type != NULL) {
		TestMeter->SetType(TestMeter->Type);
	}

	UpdateTypeLayOut();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::DebugButtonClick(TObject *Sender) {
	DebugForm->SetHscDevice(HSC);
	DebugForm->Show();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::SwitchConnectedSwitch(TObject *Sender) {
	if (SwitchConnected->IsChecked == false) {

	}
	else {

	}

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::FormClose(TObject *Sender,
	TCloseAction &Action)

{
	TFlowMeter::SaveToFile(TestMeter);
	TOrderClass::SaveToFile();
	HSC->Disconnect();

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::CameraButtonClick(TObject *Sender) {
	PhotoCaptureForm->SetHscDevice(HSC);
	PhotoCaptureForm->TestMeter = TestMeter;
	PhotoCaptureForm->EtalonMeter = EtalonMeter;

	PhotoCaptureForm->TimeEdit->Text = TimeEdit->Text;

	PhotoCaptureForm->Show();

}

// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Button23Click(TObject *Sender) {

  /*	TestMeter->CalibrPoint.State = 1;
	TestMeter->CalibrPoint.Index = 1;
	TestMeter->CalibrPoints.push_back(TestMeter->CalibrPoint);

	TestMeter->CalibrPoint.State = 2;
	TestMeter->CalibrPoint.Index = 2;
	TestMeter->CalibrPoints.push_back(TestMeter->CalibrPoint);  */

		Anim2->Enabled = true;
		Anim1->Enabled = false;


/*
	if (ButtonMonitor->Tag == 0) {
		Glow1->Enabled = true;
		// ButtonMonitor->IsPressed = true;
		ButtonMonitor->Tag = 1;
		TestMeter->ResetTest();
		EtalonMeter->ResetTest();

	  //	HSC->StartMonitor();
		// RadioButton1->IsChecked = true;

	}
	else {
		// ButtonMonitor->IsPressed = false;
		ButtonMonitor->Tag = 0;
		Anim2->Enabled = true;
		Anim1->Enabled = false;
	//	HSC->StopMonitor();
		// RadioButton1->IsChecked = false;
	}

   */

	// TestMeter->CalibrPoints.insert(2,TestMeter->CalibrPoint);
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::ButtonClick(TObject *Sender) {


	PointsStringGrid->RowCount = TestMeter->CalibrPoints.size();

	for (int i = 0; i < TestMeter->CalibrPoints.size(); i++) {
		PointsStringGrid->Cells[0][i] =
			IntToStr(TestMeter->CalibrPoints[i].Index);
		PointsStringGrid->Cells[1][i] =
			IntToStr(TestMeter->CalibrPoints[i].State);
	}

	/* for (int i=0;i<PointsStringGrid->RowCount;i++)
	 {

	 for (int j=0; j<PointsStringGrid->ColumnCount;j++)
	 {
	 if (PointsStringGrid->Cells[j][i]=="")
	 {
	 empty = true;
	 }
	 }
	 */

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::TempEditKeyDown(TObject *Sender, WORD &Key,
	System::WideChar &KeyChar, TShiftState Shift) {
	float f;

	KeyDownCheck(Key, KeyChar);

	if (Key == 13U) {
		if (TryStrToFloat(TempEdit->Text, f)) {
			TempEdit->ResetFocus();
		}
		else {
			TempEdit->ResetFocus();
			TempEdit->Text = L"20,1";
		}
	}
}

void TMainTabbedForm::KeyDownCheck(WORD &Key, System::WideChar &KeyChar) {
	UnicodeString M;

	if (((KeyChar < L'0') || (KeyChar > L'9')) && /* (KeyChar!=L'.')&& */
		(KeyChar != L',') && (Key != 8) && (Key != 46))

	{
		if (KeyChar == L'.') {
			KeyChar = L',';
		}
		else {
			KeyChar = NULL;
		}

	}
	/* if((KeyChar==L'.')||(KeyChar==L','))
	 {
	 M=FloatToStr(0.1);
	 KeyChar=M[2];
	 } */

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::PressureTempKeyDown(TObject *Sender, WORD &Key,
	System::WideChar &KeyChar, TShiftState Shift) {
	float f;

	KeyDownCheck(Key, KeyChar);

	if (Key == 13U) {
		PressureTemp->ResetFocus();

	}

	if (Key == 13U) {
		if (TryStrToFloat(TempEdit->Text, f)) {
			PressureTemp->ResetFocus();
		}
		else {
			PressureTemp->ResetFocus();
			PressureTemp->Text = "20,1";
		}
	}

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::HumidityEditKeyDown(TObject *Sender, WORD &Key,
	System::WideChar &KeyChar, TShiftState Shift) {
	KeyDownCheck(Key, KeyChar);

	if (Key == 13U) {
		HumidityEdit->ResetFocus();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::TempWaterEditKeyDown(TObject *Sender,
	WORD &Key, System::WideChar &KeyChar, TShiftState Shift) {
	KeyDownCheck(Key, KeyChar);

	if (Key == 13U) {
		TempWaterEdit->ResetFocus();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::PresWaterEditKeyDown(TObject *Sender,
	WORD &Key, System::WideChar &KeyChar, TShiftState Shift) {
	KeyDownCheck(Key, KeyChar);

	if (Key == 13U) {
		PresWaterEdit->ResetFocus();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::DensityEditKeyDown(TObject *Sender, WORD &Key,
	System::WideChar &KeyChar, TShiftState Shift) {
	KeyDownCheck(Key, KeyChar);

	if (Key == 13U) {
		DensityEdit->ResetFocus();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::ButtonMonitorClick(TObject *Sender) {

	// if (ButtonMonitor->IsPressed)
	{
		// ButtonMonitor->StartTriggerAnimation(ButtonMonitor, 'IsVisible=true');
		// ButtonMonitor->ApplyTriggerEffect(ButtonMonitor, 'IsVisible=true');

	}
	// else
	{

	}

	if (HSC->IsBluetoothConnected())
{
		if (TestMeter->CheckType == 4)
 {
	  time = 0;
	  if (ButtonMonitor->Tag == 0) {

	StartMonitor();
	ShowStableFlow = 1;

	}
	else {
		// ButtonMonitor->IsPressed = false;
  StopMonitor();
	ShowStableFlow = 0;
		// RadioButton1->IsChecked = false;
	   }
 }    else
  {
	time = 0;

	if (ButtonMonitor->Tag == 0) {
	//Glow1->Enabled = true;
		// ButtonMonitor->IsPressed = true;
		ButtonMonitor->Tag = 1;

		TestMeter->ResetTest();
		EtalonMeter->ResetTest();
		TestMeter->SetEtalon(EtalonMeter);


		Anim2->Enabled = false;
		Anim1->Enabled = true;

		HSC->StartMonitor();
		// RadioButton1->IsChecked = true;
		PhotoCaptureForm->TestMeter = TestMeter;
		PhotoCaptureForm->EtalonMeter = EtalonMeter;


	}
	else {
		// ButtonMonitor->IsPressed = false;
		ButtonMonitor->Tag = 0;
		Anim2->Enabled = true;
		Anim1->Enabled = false;
		Glow1->Enabled = false;
		HSC->StopMonitor();
		// RadioButton1->IsChecked = false;


	}
   }
	// ButtonMonitor->IsChecked = true;
		  }
	// ButtonMonitor->Effects
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------

//// Перенос поверочных точек из таблицы в запись текущего прибора

void __fastcall TMainTabbedForm::RefreshPointsTable(void) {

	String str1 = "", str2 = "";
	float Q2, Qetl, error;
	float Qrate, Flow, Volume, Time, Imp, Error;
	float f;
	bool succes = true;
	char chr = 160U;

	TestMeter->ClearPoints();

	// CoefStringGrid->Cells[6][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Error,ffNumber,10,1);

	for (int i = 0; i < PointsStringGrid->RowCount; i++) {
		str1 = StrTrim_(PointsStringGrid->Cells[1][i]);
		if (TryStrToFloat(str1, f)) {
			Qrate = f;
		}
		else {
			PointsStringGrid->Cells[1][i] = "";
			if (str1 != "") {
				succes = false;
			}
			break;
		}

		str1 = StrTrim_(PointsStringGrid->Cells[2][i]);
		if (TryStrToFloat(str1, f)) {
			Flow = f;
		}
		else {
			PointsStringGrid->Cells[2][i] = "";
			if (str1 != "") {
				succes = false;
			}
			break;
		}

		str1 = StrTrim_(PointsStringGrid->Cells[3][i]);
		if (TryStrToFloat(str1, f)) {
			Volume = f;
		}
		else {
			PointsStringGrid->Cells[3][i] = "";
			if (str1 != "") {
				succes = false;
			}
			break;
		}

		str1 = StrTrim_(PointsStringGrid->Cells[4][i]);
		if (TryStrToFloat(str1, f)) {
			Time = f;
		}
		else {
			PointsStringGrid->Cells[4][i] = "";
			if (str1 != "") {
				succes = false;
			}

			break;
		}

		str1 = StrTrim_(PointsStringGrid->Cells[5][i]);
		if (TryStrToFloat(str1, f)) {
			Error = f;
		}
		else {
			PointsStringGrid->Cells[5][i] = "";
			if (str1 != "") {
				succes = false;
			}

			break;
		}

		TestMeter->AddPointData(PointsStringGrid->Cells[0][i], Qrate, Flow,
			Volume, Time, Error);

	}

	if (succes) {
		UpdatePointsGrid();
		// RadioButton2->IsChecked = true;
	}

}

void __fastcall TMainTabbedForm::RefreshButtonClick(TObject *Sender) {
	RefreshPointsTable();
}

void __fastcall TMainTabbedForm::UpdatePointsGrid() {
	PointsStringGrid->RowCount = 0;
	/* PointsStringGrid->RowCount = 10;

	 for (int i=0;i<PointsStringGrid->RowCount;i++)
	 {
	 for (int ii=0;ii<PointsStringGrid->ColumnCount;ii++)
	 { PointsStringGrid->Cells[ii][i]= "";    }

	 }
	 */
	if (!(TestMeter == nullptr))
		if (!TestMeter->Points.empty()) {
			for (int i = 0; i < TestMeter->Points.size(); i++) {
				PointsStringGrid->RowCount = PointsStringGrid->RowCount + 1;
				PointsStringGrid->Cells[0][i] = TestMeter->Points[i].Name;
				PointsStringGrid->Cells[1][i] =
					FloatToStrF(TestMeter->Points[i].Qrate, ffNumber, 10, 2);
				PointsStringGrid->Cells[2][i] =
					FloatToStrF(TestMeter->Points[i].Q, ffNumber, 10, 2);

				if (!(TestMeter->Points[i].Volume == 0)) {
					PointsStringGrid->Cells[3][i] =
						FloatToStrF(TestMeter->Points[i].Volume,
					ffNumber, 10, 2);
				}
				else {
					PointsStringGrid->Cells[3][i] = L"-";
				}

				PointsStringGrid->Cells[4][i] =
					FloatToStrF(TestMeter->Points[i].Time, ffNumber, 10, 2);

				PointsStringGrid->Cells[5][i] =
					FloatToStrF(TestMeter->Points[i].Error, ffNumber, 10, 2);

			}
		}
		else {

		}

}

// ---------------------------------------------------------------------------

void TMainTabbedForm::SavePointsToFile(void) {
	/*
	 bool success = true;
	 UnicodeString dirName = L"";
	 TXMLDocument *XmlDoc = nullptr;
	 _di_IXMLNode rootNode = nullptr;
	 _di_IXMLNode sampleNode = nullptr;

	 System::UnicodeString fname;


	 XmlDoc = new TXMLDocument(NULL);

	 XmlDoc->DOMVendor = DOMVendors->Vendors[0]; //< OMNI XML кроссплатформенный вендор
	 XmlDoc->XML->Clear();
	 XmlDoc->FileName = "";
	 XmlDoc->Active = true;
	 //
	 // Создадим главную ветку и добавим узел об устройстве
	 rootNode   = XmlDoc->AddChild(L"Device");
	 sampleNode = rootNode->AddChild(L"Main");

	 sampleNode->SetAttribute(L"Serial", TestMeter->SerialNum);
	 sampleNode->SetAttribute(L"DeviceType", TestMeter->DeviceType);
	 sampleNode->SetAttribute(L"CertificateNum", TestMeter->CertificateNum);

	 fname=
	 System::Ioutils::TPath::GetDocumentsPath()+
	 System::Ioutils::TPath::DirectorySeparatorChar+
	 "TestMeter"+
	 System::Ioutils::TPath::ExtensionSeparatorChar+"xml";


	 // XmlDoc->SaveToFile(dirName + L"/Report.xml");

	 XmlDoc->SaveToFile(fname);

	 //XmlDoc->L
	 XmlDoc->Active = false;

	 */

}

// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::SavePointsButtonClick(TObject *Sender) {
	SavePointsToFile();

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::DelPointsButtonClick(TObject *Sender) {
	TestMeter->ClearPoints();
	UpdatePointsGrid();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::QmaxEditKeyDown(TObject *Sender, WORD &Key,
	System::WideChar &KeyChar, TShiftState Shift) {
	UnicodeString M;

	if (((KeyChar < L'0') || (KeyChar > L'9')) && /* (KeyChar!=L'.')&& */
		(KeyChar != L',') && (Key != 8) && (Key != 46))
		KeyChar = NULL;
	/* if((KeyChar==L'.')||(KeyChar==L','))
	 {
	 M=FloatToStr(0.1);
	 KeyChar=M[2];
	 } */

	if (KeyChar == L'.')
		KeyChar = L',';

	if (Key == 13U) {
		QmaxEdit->ResetFocus();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::KpEdit1KeyDown(TObject *Sender, WORD &Key,
	System::WideChar &KeyChar, TShiftState Shift) {
	UnicodeString M;
	if (((KeyChar < L'0') || (KeyChar > L'9')) && /* (KeyChar!=L'.')&& */
		(KeyChar != L',') && (Key != 8) && (Key != 46))
		KeyChar = NULL;
	/* if((KeyChar==L'.')||(KeyChar==L','))
	 {
	 M=FloatToStr(0.1);
	 KeyChar=M[2];
	 } */

	if (KeyChar == L'.')
		KeyChar = L',';

	if (Key == 13U) {
		KpEdit1->ResetFocus();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::QmaxEditChange(TObject *Sender) {
	float f;
	if (TryStrToFloat(QmaxEdit->Text, f)) {
		TestMeter->Qmax = f;
		// RadioButton3->IsChecked = true;
	}
	else {
		// RadioButton3->IsChecked = false;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::PointsStringGridEditingDone(TObject *Sender,
	const int ACol, const int ARow) {
	// RadioButton2->IsChecked = false;
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::ButtonDelPointClick(TObject *Sender) {
	if ((!TestMeter->Points.empty()) && (PointsStringGrid->Row != -1)) {

		TestMeter->Points.erase(TestMeter->Points.begin() +
			PointsStringGrid->Row);

		UpdateTypeLayOut();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::DeleteRowDataButtonClick(TObject *Sender) {
	/* TypeSelectionForm->SetFlowMeter(TestMeter);
	 TypeSelectionForm->SetOnUpdate(OnTypeUpdate);
	 TypeSelectionForm->Show(); */

	if ((!(TestMeter == nullptr)) && (DevicesGrid->Row != -1))
		if (!(TestMeter->DataPoints.size() < DataPointsGrid->Row)) {

			TDialogServiceAsync::MessageDialog(L"Удалить данные измерения?",
				TMsgDlgType::mtWarning, mbYesNo, TMsgDlgBtn::mbNo, 0,
				[this](const System::Uitypes::TModalResult AResult) {

				switch (AResult)

				{

					case mrYes :
					{TestMeter->DataPoints.erase(TestMeter->DataPoints.begin() +
							DataPointsGrid->Row); UpdateDevicesGrid(); break;
					}; // пользователь нажал Yes

					case mrNo : {

						break;};

				}});

		}

}
// ---------------------------------------------------------------------------
// Обновить данные Прибора из данных формв

void __fastcall TMainTabbedForm::TypeDeviceUpdate() {
	tPoint Point;
	bool empty;
	float f;
	int i;

	TestMeter->Name = 				ComboEditTypeName->Text;
	TestMeter->DeviceType = 		EditDeviceType->Text;
	TestMeter->Modification =  		EditModification->Text;
 	TestMeter->SerialNum = 			SerialNumEdit->Text;
	TestMeter->CertificateNum = 	CertificateNumEdit->Text;
	TestMeter->CheckType = 			TypeComboBox->ItemIndex;
	TestMeter->year_production= 	EditYear->Text;

	  if (TryStrToInt_(EditVerificationInterval->Text,i)) {
		TestMeter->VerificationInterval =  i;
	}


	if (TryStrToInt(EditDN->Text, i)) {
	  if (i!=0) {
		 TestMeter->DN		=		EditDN->Text;
	  }


}  else
	{   TestMeter->DN		= "15";
		EditDN->Text =             "15";

	}


	if (TryStrToFloat_(KpEdit1->Text, f)) {
		TestMeter->Kp = f;
	}
	else {
		KpEdit1->TextSettings->FontColor = 256;
	}

	if (TryStrToFloat_(QmaxEdit->Text, f)) {
		TestMeter->Qmax = f;
	}
	else {
		QmaxEdit->TextSettings->FontColor = 246;
	}

	RefreshPointsTable();

	// RadioButton3->IsChecked = true;
	// RadioButton2->IsChecked = true;

	UpdatePointsGrid();
}

void __fastcall TMainTabbedForm::UpdateTypeLayOut() {
	if (!(TestMeter == nullptr)) {

		 UpdateMesurmentMean();
		/*
		TypeDeviceEdit->Text = TestMeter->DeviceType;
		CertificateNumEdit->Text = TestMeter->CertificateNum;
		SerialNumEdit->Text = TestMeter->SerialNum;
		*/
		TypeComboBox->ItemIndex = TestMeter->CheckType;


		QmaxEdit->Text = FloatToStrF(TestMeter->Qmax, ffNumber, 10, 2);
		KpEdit1->Tag = 1;
		KpEdit1->Text = FloatToStrF(TestMeter->Kp, ffNumber, 10, 4);
	}
	else {
		ComboEditTypeName->Text="";
		EditDeviceType->Text="";
		EditModification->Text="";
		CertificateNumEdit->Text = "";
		SerialNumEdit->Text = "";
		EditYear->Text ="";
        EditDN->Text ="";
		QmaxEdit->Text = "";
		KpEdit1->Text = "";
	}

	UpdatePointsGrid();

	// RadioButton2->IsChecked = true;
	// RadioButton3->IsChecked = true;
}

void __fastcall TMainTabbedForm::OnTypeUpdate(TObject * Sender) {
		TFlowMeterType::filter=0;

	 ComboEditTypeName->Tag=1;
	 ComboEditTypeName->Text = TestMeter->Modification;

	UpdateDeviceChange();

}

void __fastcall TMainTabbedForm::TypeSelectButton2Click(TObject *Sender) {
	TypeSelectionForm->SetFlowMeter(TestMeter);
	TypeSelectionForm->SetOnUpdate(OnTypeUpdate);
	TypeSelectionForm->Show();

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Button7Click(TObject *Sender) {
	PointsStringGrid->RowCount = PointsStringGrid->RowCount + 1;
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::FormShow(TObject *Sender) {
	UpdateDeviceChange();
	HSC->SetOnVersionRead(OnVersionRead);


	//TabItem2->Visible = false;
}
// ---------------------------------------------------------------------------
  //Обновдение типа по текущим данным
void TMainTabbedForm::TypeUpdate() {
	float f;
	TFlowMeterType *MeterType;

	if (TestMeter->Type == nullptr) {
		float f;
		TFlowMeterType *MeterType;

		MeterType = new TFlowMeterType();


		MeterType->Name = 			ComboEditTypeName->Text;
		MeterType->DeviceType = 	EditDeviceType->Text;
		MeterType->Modification =  	EditModification->Text;
		//
		// Серийный номер поверяемого устройства
		MeterType->SerialNum = SerialNumEdit->Text;
		//
		// Номер ГРСИ поверяемого устройства
		MeterType->CertificateNum = CertificateNumEdit->Text;

		MeterType->CheckType = TypeComboBox->ItemIndex;

		MeterType->Date1 = EditYears->Text;

		// Коэффициент преобразования
		if (TryStrToFloat_(KpEdit1->Text, f)) {
			MeterType->Kp = f;
		}
		else {
			KpEdit1->TextSettings->FontColor = 256;
		}

		if (TryStrToFloat_(QmaxEdit->Text, f)) {
			MeterType->Qmax = f;
		}
		else {
			KpEdit1->TextSettings->FontColor = 246;
		}

		RefreshPointsTable();

		MeterType->AddPoints(TestMeter->Points);

		MeterType->AddTypeToList();

		MeterType->SaveTypesToFile();
		return;
	}

	MeterType = TestMeter->Type;

	MeterType->DeviceName = EditDeviceType->Text;
	//
	// Серийный номер поверяемого устройства
	MeterType->SerialNum = SerialNumEdit->Text;
	//
	// Номер ГРСИ поверяемого устройства
	MeterType->CertificateNum = CertificateNumEdit->Text;

	// Коэффициент преобразования
	if (TryStrToFloat_(KpEdit1->Text, f)) {
		MeterType->Kp = f;
	}
	else {
		KpEdit1->TextSettings->FontColor = 256;
	}

	if (TryStrToFloat_(QmaxEdit->Text, f)) {
		MeterType->Qmax = f;
	}
	else {
		KpEdit1->TextSettings->FontColor = 256;
	}

	MeterType->AddPoints(TestMeter->Points);

}

void __fastcall TMainTabbedForm::ModifyTypeButtonClick(TObject *Sender) {
	TypeUpdate();
	TFlowMeterType::SaveTypesToFile();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::SpillageTypeComboBoxChange(TObject *Sender) {
	if (TestMeter->CheckType == 1) {
		Layout23->Visible = true;

	}
	else {
		Layout23->Visible = false;
	}

	if (TypeComboBox->ItemIndex == 1) {

	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::PointsStringGridSelectCell(TObject *Sender,
	const int ACol, const int ARow, bool &CanSelect) {
	if ((TestMeter->Points.size() > ARow)) {
		TestMeter->Point = TestMeter->Points[ARow];
		UpdatePointChange();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::PreviousPointButtonClick(TObject *Sender) {
	if (TestMeter != NULL) {
		TestMeter->SetPreviousPoint();
	}

	UpdatePointChange();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::LoadPointsButtonClick(TObject *Sender) {
	TestMeter->RestoreTypePoints();
	UpdateTypeLayOut();

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::AddTypeMenuItemClick(TObject *Sender) {
	AddDeviceType();

}
// ---------------------------------------------------------------------------

void TMainTabbedForm::ModifyDeviceType() {
	float f;
	TFlowMeterType *MeterType;

	if (TestMeter->Type == nullptr) {
		float f;

		MeterType = new TFlowMeterType();

		MeterType->Name = 			ComboEditTypeName->Text;
		MeterType->DeviceName= 	EditDeviceType->Text;
		MeterType->Modification =  	EditModification->Text;
		//
		// Серийный номер поверяемого устройства
		MeterType->SerialNum = SerialNumEdit->Text;
		//
		// Номер ГРСИ поверяемого устройства
		MeterType->CertificateNum = CertificateNumEdit->Text;

		// Коэффициент преобразования
		if (TryStrToFloat_(KpEdit1->Text, f)) {
			MeterType->Kp = f;
		}
		else {
			KpEdit1->TextSettings->FontColor = 256;
		}

		if (TryStrToFloat_(QmaxEdit->Text, f)) {
			MeterType->Qmax = f;
		}
		else {
			QmaxEdit->TextSettings->FontColor = 246;
		}

		RefreshPointsTable();

		MeterType->AddPoints(TestMeter->Points);

		MeterType->AddTypeToList();

		MeterType->SaveTypesToFile();
		return;
	}

	MeterType = TestMeter->Type;

	MeterType->Name = 			ComboEditTypeName->Text;
	MeterType->DeviceName = 	EditDeviceType->Text;
	MeterType->Modification =  	EditModification->Text;

	//
	// Серийный номер поверяемого устройства
	MeterType->SerialNum = SerialNumEdit->Text;
	//
	// Номер ГРСИ поверяемого устройства
	MeterType->CertificateNum = CertificateNumEdit->Text;

	// Коэффициент преобразования
	if (TryStrToFloat_(KpEdit1->Text, f)) {
		MeterType->Kp = f;
	}
	else {
		KpEdit1->TextSettings->FontColor = 256;
	}

	if (TryStrToFloat_(QmaxEdit->Text, f)) {
		MeterType->Qmax = f;
	}
	else {
		QmaxEdit->TextSettings->FontColor = 256;
	}

	RefreshPointsTable();

	MeterType->AddPoints(TestMeter->Points);

	// MeterType->AddTypeToList();

	MeterType->SaveTypesToFile();

	UpdateDevicesGrid();
}

void __fastcall TMainTabbedForm::ModifyTypeMenuItemClick(TObject *Sender) {
	ModifyDeviceType();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::RefreshDeviceMenuItemClick(TObject *Sender) {
	tPoint Point;
	bool empty;

	if (TestMeter->Type != NULL) {
		TestMeter->SetType(TestMeter->Type);
	}

	UpdateTypeLayOut();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::SerialNumEditKeyDown(TObject *Sender,
	WORD &Key, System::WideChar &KeyChar, TShiftState Shift) {
	if (Key == 13U) {
		SerialNumEdit->ResetFocus();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::SerialNumEditExit(TObject *Sender) {

	// System::Uitypes::TModalResult Result;
	if ((TestMeter->SerialNum == "") || (SerialNumEdit->Tag == 1)) {
		TestMeter->SerialNum = SerialNumEdit->Text;
		SerialNumEdit->Tag = 0;
	}
	else if (TestMeter->SerialNum == SerialNumEdit->Text) {

	}
	else {
		TDialogServiceAsync::MessageDialog(L"Создать новый прибор?",
			TMsgDlgType::mtInformation, mbYesNo, TMsgDlgBtn::mbYes, 0,

			[this](const System::Uitypes::TModalResult AResult)
		{Result = AResult;
			switch (Result) {

				case mrYes : {

					TFlowMeterType *Type = TestMeter->Type;
					TestMeter = new TFlowMeter(HSC, false);

					TestMeter->Type = Type;

					TypeDeviceUpdate();

					break;}; // пользователь нажал Yes

				case mrNo : {TestMeter->SerialNum = SerialNumEdit->Text; break;
				}; default : {} // если нажата другая кнопка
			}

			// ---------------------------------------------------------------------------

		});
	}
}

void TMainTabbedForm::UpdateDevicesGrid() {
	int size = TFlowMeter::FlowMeters.size();
	int ARow = -1;
	float f;
	int row = 0;

	TFlowMeter *Meter;
	DevicesGrid->Tag = 1;

	DevicesGrid->RowCount = 0;




	if (!TFlowMeter::FlowMeters.empty())
	{

		for (int j = 0; j < size; j++) {

			 Meter = TFlowMeter::FlowMeters[j];

			if ((TSettingsClass::IsOrdersEnabled==0 )||
					(Meter->ID_Order==TOrderClass::ActiveOrder->ID_Order))

		  //	if (TFlowMeter::FlowMeters[j]->IsEtalon == false)
			{

				DevicesGrid->RowCount = DevicesGrid->RowCount + 1;
				DevicesGrid->Cells[0][row] =
					TFlowMeter::FlowMeters[j]->Modification;
				DevicesGrid->Cells[1][row] =  TFlowMeter::FlowMeters[j]->ID_Order;
				  //	TFlowMeter::FlowMeters[j]->CertificateNum;
				DevicesGrid->Cells[2][row] = TFlowMeter::FlowMeters[j]->SerialNum;

				TFlowMeter::FlowMeters[j]->CheckStatus();
				DevicesGrid->Cells[3][row] = TFlowMeter::FlowMeters[j]->Result;

				DevicesGrid->Cells[4][row] = TFlowMeter::FlowMeters[j]->GetSendStatus();

				if (!TFlowMeter::FlowMeters[j]->DataPoints.empty()) {
					DevicesGrid->Cells[5][row] =
						(TFlowMeter::FlowMeters[j]->DataPoints
						[TFlowMeter::FlowMeters[j]->DataPoints.size() -
						1].Date);
				}

				DevicesGrid->Cells[6][row] = TFlowMeter::FlowMeters[j]->Hash;

				if (!(TestMeter == nullptr))
					if (TFlowMeter::FlowMeters[j]->Hash == TestMeter->Hash) {
						ARow = row;
					}



			   row++;
			}

		}
		DevicesGrid->Row = ARow;
	} else
	{
	  TestMeter = nullptr;
    }


	///////

	size = 0;
	tPoint *pnt;

	DataPointsGrid->RowCount = 0;
	if (!(TestMeter == nullptr))
{
		if (!TestMeter->DataPoints.empty()) {
			size = TestMeter->DataPoints.size();

			for (int i = 0; i < TestMeter->DataPoints.size(); i++) {
				DataPointsGrid->RowCount = DataPointsGrid->RowCount + 1;

				pnt = TestMeter->DataPoints[i].Point;
				/*
				if (!(TestMeter->DataPoints[i].Point == NULL)) {
					if (TestMeter->DataPoints[i].Point != nullptr) {

						if (TestMeter->DataPoints[i].Point->Name != NULL) {
							DataPointsGrid->Cells[0][i] =
								TestMeter->DataPoints[i].Point->Name;
						}
					}
				}
				*/
				//if (TestMeter->DataPoints[i].Point->Name != NULL) {
							DataPointsGrid->Cells[0][i] =
								TestMeter->DataPoints[i].Name;
				   //		}

				// DataPointsGrid->Cells[1][i]= FloatToStrF(TestMeter->Points[i].Qrate,ffNumber,10,2);
				DataPointsGrid->Cells[1][i] =
					FloatToStrF(TestMeter->DataPoints[i].Q, ffNumber, 10, 2);
				DataPointsGrid->Cells[2][i] =
					FloatToStrF(TestMeter->DataPoints[i].Volume,
					ffNumber, 10, 2);
				DataPointsGrid->Cells[3][i] =
					FloatToStrF(TestMeter->DataPoints[i].Time, ffNumber, 10, 2);
				f = TestMeter->DataPoints[i].Error;

				DataPointsGrid->Cells[4][i] =
					FloatToStrF(TestMeter->DataPoints[i].Error, ffNumber,
					10, 2);

			}
		}
		else {

		}
}   else
{


}



	////

	DevicesGrid->Tag = 0;
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::AnalyzeTabItemClick(TObject *Sender) {

    if (TSettingsClass::IsOrdersEnabled==1) {
		  OrdersExpander->IsExpanded = true;
	  }  else
	   {
		  OrdersExpander->IsExpanded = false;
	   }

	UpdateDevicesGrid();
	UpdateOrdersGrid();



}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::DataPointsGridSelectCell(TObject *Sender,
	const int ACol, const int ARow, bool &CanSelect) {
	if (!TestMeter->DataPoints.empty()) {

		if (ARow > TestMeter->DataPoints.size()) {
			TestMeter->DataPoint = TestMeter->DataPoints[ARow];
		}

		// TestMeter->Point =  TestMeter->Points[ARow];
		// UpdatePointChange();
	}
}
// ---------------------------------------------------------------------------


//Обновление нижней вкладки Поверка
void TMainTabbedForm::UpdateCheckType() {
	float flt;
	if (!(TestMeter == nullptr)) {

		TestMeterKoefEdit->Text = FloatToStr(TestMeter->GetKoef());



		 LayoutCoef->Visible = false;
		LabelBefore->Visible = false;
		LabelAfter->Visible = false;
		EditBefore->Visible = false;
		EditAfter->Visible = false;
		Label10->Text = L"Монитор";
		TestButton->Enabled = true;
		TypeComboBox->ItemIndex = TestMeter->CheckType;
		ComboBoxUnits->Visible = false;
		Layout23->Visible = false;

	if (TestMeter->CheckType == 1) {
		LayoutCoef->Visible = true;
		LabelDevice->Text = L"Поверяемое СИ: метод сличения";
		LabelCoef->Text = L"Коэффициент, имп/л";
		KpEdit1->Tag = 1;
		KpEdit1->Text = FloatToStrF(TestMeter->Kp, ffNumber, 10, 4);

	}

	else if (TestMeter->CheckType == 0) {

		LabelDevice->Text = L"Поверяемое СИ: метод фотофиксации";
	}

	else if (TestMeter->CheckType == 2) {

		LayoutCoef->Visible = true;

		LabelDevice->Text = L"Поверяемое СИ: оптосчитыватель";
		LabelCoef->Text = L"Коэффициент, л/имп";
		flt = 1 / TestMeter->Kp;
		KpEdit1->Tag = 1;
		KpEdit1->Text = FloatToStrF(flt, ffNumber, 10, 4);

	}

	else if (TestMeter->CheckType == 3) {

		LabelDevice->Text = L"Поверяемое СИ: метод ручной ввод";

		LabelBefore->Visible = true;
		LabelAfter->Visible = true;
		EditBefore->Visible = true;
		EditAfter->Visible = true;
		ComboBoxUnits->Visible = true;

		int units;

	   if (TryStrToInt(TSettingsClass::Units, units))
	{
	  ComboBoxUnits->ItemIndex=units;
	}

		if (ComboBoxUnits->ItemIndex==0) {
	  LabelBefore->Text = L"Объем до, л";
	  LabelAfter->Text = L"Объем после, л";
	 }
	else if (ComboBoxUnits->ItemIndex==1) {
	  LabelBefore->Text = L"Объем до, м3";
	  LabelAfter->Text = L"Объем после, м3";
		 }

	}

		else if (TestMeter->CheckType == 4) {

		LabelDevice->Text = L"Поверяемое СИ: Фото сличение";
		Label10->Text = L"Фиксировать расход";
		TestButton->Enabled = false;

	}
 }

}

void TMainTabbedForm::UpdateOrdersGrid() {

	if (TSettingsClass::IsOrdersEnabled==1) {
	   OrdersExpander->IsExpanded = true;



	int size = TOrderClass::Orders.size();
	int ARow = OrdersStringGrid->Row;
	float f;

	TOrderClass *Order;
	OrdersStringGrid->Tag = 1;

	OrdersStringGrid->RowCount = 0;

	if (!TOrderClass::Orders.empty()) {

		for (int j = 0; j < size; j++) {
			// if (TFlowMeter::FlowMeters[j]->IsEtalon == false)
			{
				Order = TOrderClass::Orders[j];
				OrdersStringGrid->RowCount = OrdersStringGrid->RowCount + 1;
				OrdersStringGrid->Cells[0][j] =
					IntToStr(TOrderClass::Orders[j]->ID_Order);
				OrdersStringGrid->Cells[1][j] = TOrderClass::Orders[j]->GetStatus();
				OrdersStringGrid->Cells[2][j] = TOrderClass::Orders[j]->Name;
				OrdersStringGrid->Cells[3][j] =
					TOrderClass::Orders[j]->TelNumber;
				OrdersStringGrid->Cells[4][j] = TOrderClass::Orders[j]->Adress;
				OrdersStringGrid->Cells[5][j] = TOrderClass::Orders[j]->Time1 +
					" - " + TOrderClass::Orders[j]->Time2;
				OrdersStringGrid->Cells[6][j] = TOrderClass::Orders[j]->Date;
				OrdersStringGrid->Cells[7][j] = TOrderClass::Orders[j]->Work;


				OrdersStringGrid->Cells[8][j] = TOrderClass::Orders[j]->Comment;

				if (TOrderClass::ActiveOrder != nullptr) {
					if (TOrderClass::Orders[j]
						->ID_Order == TOrderClass::ActiveOrder->ID_Order) {
						ARow = j;
					}
				} else
				{
					   ARow = -1;
				}

			}

		}
		OrdersStringGrid->Row = ARow;

	}
		OrdersStringGrid->Tag = 0;

		UpdateDevicesGrid();

	} else
	{
	   OrdersExpander->IsExpanded = false;
	}

}

void TMainTabbedForm::UpdateCheckLayOut() {
	 //верхняя часть
	UpdatePointChange();
	//нижняя часть
	UpdateCheckType();
	//StringGridTest->Cells[0][0] = TestMeter->SerialNum;

}
 //Изменение выбора прибора
void TMainTabbedForm::UpdateDeviceChange() {
	//Тиа Устройства
	UpdateMesurmentMean();
	UpdateMesurmentTool();
	//Поверяемое СИ
	UpdateDevicesGrid();
	//Вкладка ПОВЕРКА
	UpdateTypeLayOut();
	UpdateCheckLayOut();
}

void __fastcall TMainTabbedForm::AddDataPointClick(TObject *Sender) {
	if (!(TestMeter == NULL)) {
		// #ifndef __ANDROID__
		// randomize();

		TestMeter->DataPoint.Time = 52;
		TestMeter->DataPoint.Q = rand() % 100;
		TestMeter->DataPoint.Volume = rand() % 500;
		TestMeter->DataPoint.Imp = rand() % 13000;
		TestMeter->DataPoint.Temp = 20;
		TestMeter->DataPoint.Pres = 0.1;
		TestMeter->DataPoint.DateTime = Today();
		TestMeter->DataPoint.EtlVolume = rand() % 500;
		TestMeter->DataPoint.Error =
			(TestMeter->DataPoint.Volume - TestMeter->DataPoint.EtlVolume)
			/ TestMeter->DataPoint.EtlVolume;

		TestMeter->DateTime = Today();

		TestMeter->DataPoints.push_back(TestMeter->DataPoint);

		AfterSpillage();
		// #endif
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::DevicesGridSelChanged(TObject *Sender) {

	int size= TFlowMeter::FlowMeters.size();
	int ARow = DevicesGrid->Row;
	int hash=0;
	TFlowMeter *Device;

	if (DevicesGrid->Tag == 0) {
		if (!TFlowMeter::FlowMeters.empty()) {
			if (ARow != -1) {
				hash = StrToInt(DevicesGrid->Cells[6][ARow]);
				Device =TFlowMeter::GetDevice(hash);
			   if (Device!=nullptr) {
				  TestMeter =  Device;
					TFlowMeter::ActiveFlowMeter = TestMeter;
					UpdateDeviceChange();
			   }

			}
			else {
				TestMeter = nullptr;
				UpdateDeviceChange();
			}

			// TestMeter->Point =  TestMeter->Points[ARow];

		}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::SaveDevicesButtonClick(TObject *Sender) {
	TFlowMeter::SaveToFile(TestMeter);
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::AddDeviceButtonClick(TObject *Sender) {

  //	if ((!(TestMeter == nullptr)) && (DevicesGrid->Row != -1))
	{

		// MessageDialog (L"Создать новый прибор?",TMsgDlgType :: mtWarning,
		// TMsgDlgButtons() << TMsgDlgBtn::mbYes<< TMsgDlgBtn::mbNo,TMsgDlgButtons() << TMsgDlgBtn::mbYes, 0);

		bool UserResponse;
		TDialogServiceAsync::MessageDialog(L"Создать новый прибор?",
			TMsgDlgType::mtInformation, mbYesNo, TMsgDlgBtn::mbNo, 0,
			[this](const System::Uitypes::TModalResult AResult) {

			switch (AResult) {
				// MessageDlg
				case mrYes : {


					TFlowMeterType *MeterType = TestMeter->Type;

					TestMeter = new TFlowMeter(HSC, false);

					TestMeter->SetType(MeterType);

					TestMeter->SerialNum = SerialNumEdit->Text;
					SerialNumEdit->Tag = 1;

					TypeDeviceUpdate();

					break;}; // пользователь нажал Yes

				case mrNo : {

					break;};

			}

		});

	}

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::LoadDevicesButtonClick(TObject *Sender) {
	TFlowMeter::LoadFromFile();
	UpdateDevicesGrid();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::DeleteRowButtonClick(TObject *Sender) {
	if ((!(TestMeter == nullptr)) && (DevicesGrid->Row != -1)) {
		TDialogServiceAsync::MessageDialog(L"Удалить прибор?",
			TMsgDlgType::mtInformation, mbYesNo, TMsgDlgBtn::mbNo, 0,
			[this](const System::Uitypes::TModalResult AResult) {

			switch (AResult)

			{

				case mrYes :
				{TFlowMeter::FlowMeters.erase(TFlowMeter::FlowMeters.begin() +
						DevicesGrid->Row);

				UpdateDevicesGrid();



				break;
				}; // пользователь нажал Yes

				case mrNo : {

					break;};

			}

		});

	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::DeleteConfigButtonClick(TObject *Sender) {
	//if ((!(TestMeter == nullptr)) && (DevicesGrid->Row != -1)) {

		TDialogServiceAsync::MessageDialog(L"Очистить приборы?",
			TMsgDlgType::mtInformation, mbYesNo, TMsgDlgBtn::mbNo, 0,
			[this](const System::Uitypes::TModalResult AResult) {

			switch (AResult)

			{

				case mrYes : {TFlowMeter::FlowMeters.clear();
					UpdateDevicesGrid(); break;}; // пользователь нажал Yes

				case mrNo : {

					break;};

			}});

   //	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::RefreshDevicesButtonClick(TObject *Sender) {
	UpdateDevicesGrid();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Button1Click(TObject *Sender) {
	UpdateDevicesGrid();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::ClearDataButtonClick(TObject *Sender) {
	if ((!(TestMeter == nullptr)) && (DevicesGrid->Row != -1)) {
		TDialogServiceAsync::MessageDialog(L"Очистить данные измерений?",
			TMsgDlgType::mtInformation, mbYesNo, TMsgDlgBtn::mbNo, 0,
			[this](const System::Uitypes::TModalResult AResult) {

			switch (AResult) {

				case mrYes : {TestMeter->DataPoints.clear();
					UpdateDevicesGrid(); break;}; // пользователь нажал Yes

				case mrNo : {

					break;};

			}});

	}
	// ---------------------------------------------------------------------------
}

void __fastcall TMainTabbedForm::Image5DblClick(TObject *Sender) {

	if (LabelLimit->Tag == 0) {
		LabelLimit->Text = L"Импульсы";
		LabelLimit->Tag = 1;
		VolumeEdit->Text = TestMeter->Point.Imp;
		LabelVolume->Text = EtalonMeter->ImpValue->GetStringValue();

	}
	else {
		LabelLimit->Text = L"Объем, л";
		VolumeEdit->Text = TestMeter->Point.Volume;
		LabelLimit->Tag = 0;

		LabelVolume->Text = EtalonMeter->VolumeValue->GetStringValue();
	}

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::VolumeEditValidate(TObject *Sender,
	UnicodeString &Text)

{

	if (TestMeter!=nullptr) {


	float fa;
	if (TryStrToFloat_(VolumeEdit->Text, fa)) {
		TestMeter->Point.Volume = fa;
		VolumeEdit->Text = FloatToStrF(TestMeter->Point.Volume, ffNumber,
			10, 2);
		VolumeEdit->TextSettings->FontColor = claGreen;

	}
	else

	{
		VolumeEdit->TextSettings->FontColor = claRed;
	}
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::VolumeEditKeyDown(TObject *Sender, WORD &Key,
	System::WideChar &KeyChar, TShiftState Shift) {
	float fa;
	KeyDownCheck(Key, KeyChar);

	if (Key == 13U) {
		if (TryStrToFloat_(VolumeEdit->Text, fa)) {
			TestMeter->Point.Volume = fa;
		}
		VolumeEdit->ResetFocus();

	}

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::TypeComboBoxChange(TObject *Sender) {
	TestMeter->CheckType = TypeComboBox->ItemIndex;
	UpdateCheckType();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::DeviceSelectButtonClick(TObject *Sender) {
	TFlowMeterType *MeterType = TestMeter->Type;
	TestMeter = new TFlowMeter(HSC, false);

	TestMeter->SetType(MeterType);
	TypeDeviceUpdate();

	SerialNumEdit->Tag = 1;
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::TimeEditExit(TObject *Sender) {

	 int i;

	if (TryStrToInt_(TimeEdit->Text,i) ) {
	  TestMeter->Point.Time = i;
	} else
{
	TimeEdit->Text = "";
}


}
// ---------------------------------------------------------------------------

void TMainTabbedForm::RefreshCond(int delta) {

	float temp;

	temp = ((rand() % 100) - 50);
	temp = temp / 100;
	temp = StrToFloat_(TempEdit->Text) + temp;
	if (temp > 25) {
		temp = temp - 5;
	}

	if (temp < 15) {
		temp = temp + 5;
	}

	TempEdit->Text = FloatToStrF(temp, ffNumber, 10, 1);

	temp = ((rand() % 100) - 50);
	temp = temp / 100;
	temp = StrToFloat_(TempWaterEdit->Text) + temp;
	if (temp > 30) {
		temp = temp - 5;
	}

	if (temp < 15) {
		temp = temp + 5;
	}

	TempWaterEdit->Text = FloatToStrF(temp, ffNumber, 10, 1);



	temp = ((rand() % 10) - 5);
	temp = StrToFloat_(PressureTemp->Text) + temp;
	if (temp > 1020) {
		temp = temp - 5;
	}

	if (temp < 1010) {
		temp = temp + 5;
	}

	PressureTemp->Text = FloatToStrF(temp, ffNumber, 10, 1);

	temp = ((rand() % 100) - 50);
	temp = temp / 100;
	temp = StrToFloat_(HumidityEdit->Text) + temp;
	if (temp > 60) {
		temp = temp - 5;
	}

	if (temp < 20) {
		temp = temp + 5;
	}

	HumidityEdit->Text = FloatToStrF(temp, ffNumber, 10, 1);

}

void __fastcall TMainTabbedForm::Button22Click(TObject *Sender) {
	/*
	 float temp;

	 temp =  ((rand()% 100) - 50);
	 temp = temp /100;
	 temp = StrToFloat_(TempEdit->Text)+temp;
	 if (temp>25)
	 {
	 temp = temp -5;
	 }

	 TempEdit->Text = FloatToStrF(temp,ffNumber,10,2);
	 */

	RefreshCond(0);

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Button12Click(TObject *Sender) {
	ShowMessage(L"FlowService Версия 1.005 (27.09.22)");

  //	Memo1->Visible = ~Memo1->Visible;
	/*
	 if (Memo1->Visible) {
	 Memo1->Visible = false;

	 } else
	 {
	 Memo1->Visible = true;
	 }
	 */
}
// ---------------------------------------------------------------------------
void TMainTabbedForm::AutoFill()
{
	TDialogServiceAsync::MessageDialog(L"Добавить точки измерения?",
		TMsgDlgType::mtWarning, mbYesNo, TMsgDlgBtn::mbNo, 0,
		[this](const System::Uitypes::TModalResult AResult) {

		float f, fa, Q;
		float VolumeBefore=0;
		switch (AResult)

		{

			case mrYes : {

				if (!(TestMeter == NULL)) {
					// #ifndef __ANDROID__
					// randomize();

					   RefreshCond(0);

					if (TestMeter->DataPoints.size()>0) {
							  VolumeBefore=TestMeter->DataPoints[TestMeter->DataPoints.size()-1].VolumeAfter;
							if (VolumeBefore!=0) {
							   VolumeBefore = VolumeBefore+0.002;
							}

					} else if (TryStrToFloat(EditBefore->Text,VolumeBefore)) {

                               if (ComboBoxUnits->ItemIndex==0) {
					  VolumeBefore =  VolumeBefore/1000;
		   }
			else if (ComboBoxUnits->ItemIndex==1)
				{

				 }
		  else
			{

			}


								if (VolumeBefore!=0) {
							   VolumeBefore = (VolumeBefore)+0.002;
							}
						   }



					for (int i = 0; i < TestMeter->Points.size(); i++) {

					TestMeter->DataPoint.Name= TestMeter->Points[i].Name;
					TestMeter->DataPoint.Time = TestMeter->Points[i].Time;

					f = rand() % 50 + 50;
					f = f / 1000;
					Q = TestMeter->Points[i].Q;
					Q = TestMeter->Points[i].Q * f;
					TestMeter->DataPoint.Q =
						TestMeter->Points[i].Q + Q;

					int lo = TestMeter->Points[i].Error * 100;
					fa =  1;
					int l = (lo-fa)*2;




					if (l == 0) {l = 20;}

					 int r = (rand() % l);

					f = lo - r;
					f = f / 10000;

					TestMeter->DataPoint.EtlVolume =
						TestMeter->Points[i].Time * (TestMeter->DataPoint.Q/3600);
					TestMeter->DataPoint.Volume =
						TestMeter->DataPoint.EtlVolume +
						TestMeter->DataPoint.EtlVolume * f;

					TestMeter->DataPoint.VolumeBefore= VolumeBefore;

					TestMeter->DataPoint.VolumeAfter =  VolumeBefore+ (TestMeter->DataPoint.Volume)/1000;

					VolumeBefore=   TestMeter->DataPoint.VolumeAfter + 0.001;

					TestMeter->DataPoint.Imp = rand() % 13000;
					TestMeter->DataPoint.Temp = StrToFloat_(TempWaterEdit->Text);
					TestMeter->DataPoint.Pres = StrToFloat_(PresWaterEdit->Text);
					TestMeter->DataPoint.DateTime = Today();

					TestMeter->DataPoint.Error =
						(TestMeter->DataPoint.Volume -
						TestMeter->DataPoint.EtlVolume)
						* 100 / TestMeter->DataPoint.EtlVolume;

					TestMeter->DateTime = Today();

					TestMeter->DataPoints.push_back(TestMeter->DataPoint);

					// AfterSpillage();
					// TestMeter->SaveDataPoint(EtalonMeter);
					// #endif
				}
					}

		TestMeter->temperature = TempEdit->Text + L" град. С";
		TestMeter->tempWater = TempWaterEdit->Text + L" град. С";



		if (TryStrToFloat_(PressureTemp->Text,f)) {
			f = f /10;

		 TestMeter->pressure =FloatToStrF(f,
					ffNumber, 10, 1) + L" кПа";

		}  else
		{

		   TestMeter->pressure =L"101,7 кПа";
		}

		TestMeter->hymidity = HumidityEdit->Text+ L"%";

				UpdateDevicesGrid();

			TFlowMeter::SaveToFile(TestMeter);


			; break;}; // пользователь нажал Yes

			case mrNo : {

				break;};

		}});

}


void __fastcall TMainTabbedForm::PointLabelDblClick(TObject *Sender) {
			  AutoFill();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::PointLabelGesture(TObject *Sender,
	const TGestureEventInfo &EventInfo, bool &Handled) {
		 AutoFill();

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::TypeMenuButtonClick(TObject *Sender) {
	// Form1->ShowModal();

	if (AddTypeButn->Visible) {
		AddTypeButn->Visible = false;
		ModifyTypeButn->Visible = false;
	}
	else {
		AddTypeButn->Visible = true;
		ModifyTypeButn->Visible = true;
	}

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditNameKeyDown(TObject *Sender,
	WORD &Key, System::WideChar &KeyChar, TShiftState Shift) {
	if (Key == 13U) {

	}
}
// ---------------------------------------------------------------------------

void TMainTabbedForm::AddDeviceType() {
	TypeDeviceUpdate();

	float f;
	TFlowMeterType *MeterType;

	MeterType = new TFlowMeterType();

	MeterType->Name = 			ComboEditTypeName->Text;
	MeterType->DeviceName = 	EditDeviceType->Text;
	MeterType->Modification =  	EditModification->Text;
	//
	// Серийный номер поверяемого устройства
	MeterType->SerialNum = SerialNumEdit->Text;
	//
	// Номер ГРСИ поверяемого устройства
	MeterType->CertificateNum = CertificateNumEdit->Text;

	MeterType->CheckType = TypeComboBox->ItemIndex;

	// Коэффициент преобразования
	if (TryStrToFloat_(KpEdit1->Text, f)) {
		MeterType->Kp = f;
	}
	else {
		KpEdit1->TextSettings->FontColor = 256;
	}

	if (TryStrToFloat_(QmaxEdit->Text, f)) {
		MeterType->Qmax = f;
	}
	else {
		QmaxEdit->TextSettings->FontColor = 246;
	}

	MeterType->AddPoints(TestMeter->Points);

	MeterType->AddTypeToList();

	MeterType->SaveTypesToFile();

	TestMeter->Type = MeterType;
	TestMeter->CertificateNum = MeterType->CertificateNum;
	TestMeter->DeviceType = MeterType->DeviceName;

	UpdateDevicesGrid();

}

void __fastcall TMainTabbedForm::EditNameExit(TObject *Sender) {
	TFlowMeterType *MeterType = TestMeter->Type;

	//

	TestMeter->DeviceType = EditModification->Text;

	// Вариант 1: изменениеz самого хранимого типа

	// System::Uitypes::TModalResult Result;
	/*
	 if ( TestMeter->Type== nullptr)
	 {
	 AddDeviceType();
	 }
	 else if ((MeterType->DeviceType == TypeDeviceEdit->Text))
	 {
	 //AddDeviceType();

	 } else
	 {
	 TDialogServiceAsync::MessageDialog(L"Создать новый тип?",
	 TMsgDlgType::mtInformation,
	 mbYesNo,
	 TMsgDlgBtn::mbYes, 0,


	 [this](const System::Uitypes::TModalResult AResult)
	 {
	 Result = AResult;

	 switch (Result)
	 {

	 case mrYes:
	 {
	 AddDeviceType();

	 break;
	 };//пользователь нажал Yes

	 case mrNo :
	 {
	 ModifyDeviceType();
	 break;
	 };
	 default : {} //если нажата другая кнопка
	 }


	 });





	 //---------------------------------------------------------------------------

	 }
	 */
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::CertificateNumEditKeyDown(TObject *Sender,
	WORD &Key, System::WideChar &KeyChar, TShiftState Shift) {
	if (Key == 13U) {
		CertificateNumEdit->ResetFocus();
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::CertificateNumEditExit(TObject *Sender) {

	//TFlowMeterType *MeterType = TestMeter->Type;

	TestMeter->CertificateNum = CertificateNumEdit->Text;

	// Вариант 1
	/*
	 //System::Uitypes::TModalResult Result;
	 if ( TestMeter->Type== nullptr)
	 {
	 AddDeviceType();
	 }
	 else if ((MeterType->CertificateNum == CertificateNumEdit->Text))
	 {
	 //AddDeviceType();

	 } else
	 {

	 ModifyDeviceType();

	 };

	 */

	// ---------------------------------------------------------------------------

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::ModifyTypeButnClick(TObject *Sender) {
	ModifyDeviceType();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::LabelLimitGesture(TObject *Sender,
	const TGestureEventInfo &EventInfo, bool &Handled) {


		if (LabelLimit->Tag == 0) {
		LabelLimit->Tag = 1;
	}
	else {
		LabelLimit->Tag = 0;
	}

	 UpdatePointChange();

   /*
	if (LabelLimit->Tag == 0) {
		LabelLimit->Text = L"Импульсы";
		LabelLimit->Tag = 1;
		VolumeEdit->Text = TestMeter->Point.Imp;
	}
	else {
		LabelLimit->Text = L"Объем, л";
		VolumeEdit->Text = TestMeter->Point.Volume;
		LabelLimit->Tag = 0;
	}

	if (LabelLimit->Tag == 1) {
		LabelVolume->Text = FloatToStrF(impEtalon, ffNumber, 10, 3);
	}
	else {
		LabelVolume->Text = FloatToStrF(volumeEtalon, ffNumber, 10, 3);
	}
	   */
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::VolumeEditExit(TObject *Sender) {
	float fa;
	if (TryStrToFloat_(VolumeEdit->Text, fa)) {
		TestMeter->Point.Volume = fa;
	}
	else {

	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditBeforeKeyDown(TObject *Sender, WORD &Key,
	System::WideChar &KeyChar, TShiftState Shift) {

	KeyDownCheck(Key, KeyChar);

	if (Key == 13U) {
		EditBefore->ResetFocus();
	}

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditAfterKeyDown(TObject *Sender, WORD &Key,
	System::WideChar &KeyChar, TShiftState Shift) {

   KeyDownCheck(Key, KeyChar);

	if (Key == 13U) {
		EditAfter->ResetFocus();
	}

}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditBeforeExit(TObject *Sender) {

	float fa;
	if (TryStrToFloat_(EditBefore->Text, fa)) {
		TestMeter->Point.Volume = fa * 1000;
	}
	else {

	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditBeforeValidate(TObject *Sender,
	UnicodeString &Text)

{
	AfterSpillage();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditAfterValidate(TObject *Sender,
	UnicodeString &Text)

{
	AfterSpillage();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::CancelButtonClick(TObject *Sender) {
	CancelButton->Visible = false;

	TestButton->Text = L"Измерение";
	// state = 1;
	TestButton->Tag = 1;
	TimeEdit->Enabled = True;
	// BeforeTabItem->IsSelected = true;
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::CheckTabItemClick(TObject *Sender) {
	UpdateCheckLayOut();
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditAfterExit(TObject *Sender) {
	float f, fa, fb;
	if (TryStrToFloat_(EditAfter->Text, fa)) {
		if (TryStrToFloat_(EditBefore->Text, fb)) {
		   if (ComboBoxUnits->ItemIndex==0) {
			   f = (fa - fb);
		   }
			else if (ComboBoxUnits->ItemIndex==1)
				{
			  f = (fa*(1000) - fb*(1000));
				 }
		  else
			{
			   f = (fa - fb);
			}

			TestMeter->DataPoint.Volume = f;
		}
	}
	else {

	}
}

void  TMainTabbedForm::SetUpDataFlowMeters()
	  {

	TDateTime Date;
	float f;


	for (int j = 0; j < TFlowMeter::FlowMeters.size(); j++) {


	TFlowMeter::FlowMeters[j]->ID_Order = 800;//8763;
	TFlowMeter::FlowMeters[j]->miOwner = L"Физ. лицо";

	//TFlowMeter::FlowMeters[j]->docTitle);
	TFlowMeter::FlowMeters[j]->means = TSettingsClass::Means;
	//jDevice->AddPair(L"modification", FlowMeters[j]->Modification);
	//jDevice->AddPair(new TJSONPair(L"dn", new TJSONNumber(StrToInt_(FlowMeters[j]->DN))));
	TFlowMeter::FlowMeters[j]->vrfDate =  DateToISO8601(Today(),false);

		TFlowMeter::FlowMeters[j]->vrfDate =  TFlowMeter::FlowMeters[j]->vrfDate.SubString(0,10)+ TFlowMeter::FlowMeters[j]->vrfDate.SubString(24,29);



		Date =  IncYear(Today(),TFlowMeter::FlowMeters[j]->VerificationInterval);

		TFlowMeter::FlowMeters[j]->validDate = DateToISO8601(Date,false);  //IncYear(vrfDate, 6);// IncDateFGISFormat(6);

		TFlowMeter::FlowMeters[j]->validDate = TFlowMeter::FlowMeters[j]->validDate.SubString(0,10)+ TFlowMeter::FlowMeters[j]->validDate.SubString(24,29);



		//TFlowMeter::FlowMeters[j]->year_production = L"-";
		TFlowMeter::FlowMeters[j]->doc_number=L"-";
		TFlowMeter::sign_cipher = TSettingsClass::SignCipher;
		TFlowMeter::porveritel_fio = TSettingsClass::Performer;
		//TFlowMeter::email = TSettingsClass::eMail;
	  }


}


// ---------------------------------------------------------------------------


void __fastcall TMainTabbedForm::ButtonSendDataClick(TObject *Sender) {

	UnicodeString text;

	abort=false;

	if (TFlowMeter::ApiCheckResult()==1)
	{
	   text = L"Есть НЕ ГОДНЫЕ приборы. Всё равно отправить?";
	}  	else if (TFlowMeter::ApiCheckResult()==0)
{
	   text = L"Отправить данные?";
}
		else if (TFlowMeter::ApiCheckResult()==-1)
	{
	   text = L"Нет данных для отправки.";
	} else
	{
		text = L"Нет данных для отправки.";
    }


	TDialogServiceAsync::MessageDialog(text,
			TMsgDlgType::mtInformation, mbYesNo, TMsgDlgBtn::mbYes, 0,

			[this](const System::Uitypes::TModalResult AResult)
		{Result = AResult;
			switch (Result) {

				case mrYes : {

			 //TStringList *str = new TStringList();
				   SetUpDataFlowMeters();
				   UnicodeString str;
			   //	str->Text
					str = TFlowMeter::ApiConvert();

				  if (str==NULL) {
			 abort=true;
				  }   else
					  {
				  TNameValuePair  NameValuePair [4];

				   NameValuePair[0].Name = "Content-Type";
				NameValuePair[0].Value = "application/json";

					NameValuePair[1].Name = "Accept";
				NameValuePair[1].Value = "*/*";

				   NameValuePair[2].Name = "Content-Length";
				NameValuePair[2].Value = "<calculated when request is sent>";

					NameValuePair[3].Name = "Host";
				NameValuePair[3].Value = "<calculated when request is sent>";


				 TStringStream *aBodyStream = new TStringStream(str, TEncoding::UTF8,true);

				Memo3->Text = str;
				try                                                    //str
				{
				NetHTTPClient1->Post("https://api.routefinder.ru/api/write-data", aBodyStream); // , NULL, TEncoding::Default,NameValuePair);
			   //	NetHTTPRequest1->Post("http://postman-echo.com/post", (System::Classes::TStream*)aBodyStream, NULL, TEncoding::Default,NameValuePair);
				 } catch(...)
				 {
					ShowMessage(L"Проблемы соединения. Данные не отправлены!");
				 }



					   //
					   }
					break;}; // пользователь нажал Yes

				case mrNo : { break;
				}; default : {} // если нажата другая кнопка
			}
		}
			);

	if (abort) {
	ShowMessage("Не корректные данные для отправки");
	}
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::KpEdit1Change(TObject *Sender) {

	if (KpEdit1->Tag == 1) {
		KpEdit1->Tag = 0;
	}
	else {
		float f = 1, f1 = 0;
		if (TryStrToFloat_(KpEdit1->Text, f)) {

			if (TestMeter->CheckType == 1) {
				TestMeter->Kp = f;
				KpEdit1->Tag = 1;

				KpEdit1->Text = FloatToStrF(TestMeter->Kp, ffNumber, 10, 4);
			}
			else if (TestMeter->CheckType == 2) {
				f1 = 1 / f;
				TestMeter->Kp = f1;
				KpEdit1->Tag = 1;
				KpEdit1->Text = FloatToStrF((1 / TestMeter->Kp),
					ffNumber, 10, 4);
			}
			else {
				TestMeter->Kp = f;
			}

		}
		else {
			KpEdit1->Text = "";
		}

	}
	// ---------------------------------------------------------------------------

}

void __fastcall TMainTabbedForm::Memo1DblClick(TObject *Sender) {
 //	Memo1->Lines->Text = TFlowMeter::JSonConvert(TestMeter);
}
// ---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::AddOrderClick(TObject *Sender) {

	TOrderClass *Order = new TOrderClass();
	TOrderClass::ActiveOrder = Order;
	UpdateOrdersGrid();

}
// ---------------------------------------------------------------------------

void TMainTabbedForm::Refresh_Orders(void) {

	UnicodeString  str;
	int id, st;
	int size = TOrderClass::Orders.size();
	float f;
	bool succes = true;
	char chr = 160U;
	TOrderClass *Order;


	OrdersStringGrid->Tag = 1;

	TOrderClass::Orders.clear(); ;

	// CoefStringGrid->Cells[6][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Error,ffNumber,10,1);

	for (int j = 0; j < OrdersStringGrid->RowCount; j++) {
		if (TryStrToInt(OrdersStringGrid->Cells[0][j], id)) {
			Order = new TOrderClass();

			Order->ID_Order = id;

			if (TryStrToInt(OrdersStringGrid->Cells[1][j],st)) {
			   Order->Status = st;
			} else
			{
			Order->SetStatus(OrdersStringGrid->Cells[1][j]);
			}
			Order->Name = OrdersStringGrid->Cells[2][j];



			Order->TelNumber = OrdersStringGrid->Cells[3][j];
			Order->Adress = OrdersStringGrid->Cells[4][j];
			Order->Time1 = OrdersStringGrid->Cells[5][j];
			Order->Time2 = OrdersStringGrid->Cells[5][j];
			Order->Date = OrdersStringGrid->Cells[6][j];
			Order->Work = OrdersStringGrid->Cells[7][j];

			Order->Comment = OrdersStringGrid->Cells[8][j];

		}
	}

	OrdersStringGrid->Tag = 0;

	TOrderClass::SaveToFile();

	UpdateOrdersGrid();

}

void __fastcall TMainTabbedForm::RefreshOrdersClick(TObject *Sender) {
	Refresh_Orders();
}
// ---------------------------------------------------------------------------
void __fastcall TMainTabbedForm::OrdersStringGridSelChanged(TObject *Sender)
{
	int size;
	int ARow = OrdersStringGrid->Row;
	size = TOrderClass::Orders.size();

	if (OrdersStringGrid->Tag == 0) {
		if (!TOrderClass::Orders.empty()) {
			if (ARow != -1) {
				if (ARow < TOrderClass::Orders.size()) {
					TOrderClass::SetActive(ARow);
					UpdateDeviceChange();
				}
			}
			else {
				TOrderClass::ActiveOrder = nullptr;
				UpdateDeviceChange();
			}

			// TestMeter->Point =  TestMeter->Points[ARow];

		}
	}
}
//---------------------------------------------------------------------------


void __fastcall TMainTabbedForm::OrdersExpanderExpandedChanged(TObject *Sender)
{
	if (TSettingsClass::status==0) {
	  if (TSettingsClass::IsOrdersEnabled==1) {
		  OrdersExpander->IsExpanded = true;
	  }  else
	   {
		  OrdersExpander->IsExpanded = false;
	   }


	}  else
	{
	if (OrdersExpander->IsExpanded) {
		 TSettingsClass::IsOrdersEnabled = 1;
		 TSettingsClass::SaveToFile();
		 UpdateOrdersGrid();
	}   else
	{
		TSettingsClass::IsOrdersEnabled = 0;
		TSettingsClass::SaveToFile();
	}

	}

}
//---------------------------------------------------------------------------


void __fastcall TMainTabbedForm::Button27Click(TObject *Sender)
{

	int size = TOrderClass::Orders.size();;
	int ARow = OrdersStringGrid->Row;

	if (TOrderClass::ActiveOrder!=nullptr) {
		TOrderClass::ActiveOrder->Status =  4;
	}


}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Button26Click(TObject *Sender)
{
		if (TOrderClass::ActiveOrder!=nullptr) {
		TOrderClass::ActiveOrder->Status =  1;
	}
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::BindButtonClick(TObject *Sender)
{
	TestMeter->ID_Order=TOrderClass::ActiveOrder->ID_Order;
	TFlowMeter::SaveToFile(TestMeter);
	UpdateDevicesGrid();

}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Edit1Exit(TObject *Sender)
{

	Value->SetValue(Edit1->Text);

	 Label7->Text = Value->GetStringValue();
	 Memo2->Lines->Clear();

	for (int i = 0; i < Value->values.size(); i++) {
	Memo2->Lines->Add( FloatToStr(Value->values[i]));
	}

}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Edit2Exit(TObject *Sender)
{
Value->SetFilter(StrToInt(Edit2->Text));
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Button28Click(TObject *Sender)
{
	Glow1->Enabled = false;
	Anim1->Enabled = true;
	Anim2->Enabled = false;

    MediaPlayer1->FileName = L"C:\1.wav";
	MediaPlayer1->Play();
}
//---------------------------------------------------------------------------


void __fastcall TMainTabbedForm::Button20Click(TObject *Sender)
{

		Value->SetValue(Edit1->Text);

	 Label7->Text = Value->GetStringValue();
	 Memo2->Lines->Clear();

	for (int i = 0; i < Value->values.size(); i++) {
	Memo2->Lines->Add( FloatToStr(Value->values[i]));
	}

}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Edit1KeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
		  TShiftState Shift)
{

	if (Key == 13U) {
				Value->SetValue(Edit1->Text);

	 Label7->Text = Value->GetStringValue();
	 Memo2->Lines->Clear();

	for (int i = 0; i < Value->values.size(); i++) {
	Memo2->Lines->Add( FloatToStr(Value->values[i]));
	}
	}

}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Image5Click(TObject *Sender)
{
		if (LabelLimit->Tag == 0) {
		LabelLimit->Tag = 1;
	}
	else {
		LabelLimit->Tag = 0;
	}

UpdatePointChange();
}
//---------------------------------------------------------------------------


void __fastcall TMainTabbedForm::Edit3Exit(TObject *Sender)
{
Edit4->Text = IntToStr((int)sizeof(Edit3->Text));
Edit5->Text = IntToStr((int)Edit3->Text.Length());

}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Edit5Exit(TObject *Sender)
{
   if ((bool)StrToInt(Edit5->Text)) {
		 Label7->Text = "true";
   } else
{
	  Label7->Text = "false";
}

}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::TimerStopTimer(TObject *Sender)
{
	TimerStop->Enabled = false;
	StopMesurment();
	//TThread::Synchronize(NULL, StopMesurment);


}
//---------------------------------------------------------------------------
	void TMainTabbedForm::UpdateCommon()
{
	int hight=0;

	if (TSettingsClass::IsDocNumIndividual==1) {
	 LayoutDoc->Visible=true;
     EditDoc->Text = 	TSettingsClass::DocNum ;
	 hight=hight+50;
	}
	else
	{
	 LayoutDoc->Visible=false;
	}

	if (TSettingsClass::IsAdressIndividual==1) {
	 LayoutAdress->Visible=true;
	 hight=hight+100;
	}
	else
	{
	 LayoutAdress->Visible=false;
	}

	LayoutPrice->Visible=false;

	if (hight==0) {
	   LayoutCommon->Visible=false;
	}   else
	{
	   LayoutCommon->Visible=true;
	   LayoutCommon->Height =hight + 44;
    }

}
   void TMainTabbedForm::UpdateMesurmentTool()
{

	int i;
		if (TestMeter!=nullptr)
  {


		SerialNumEdit->Text = TestMeter->SerialNum;
		EditYear->Text = TestMeter->year_production;

	if (TryStrToInt(TestMeter->DN, i)) {
	  if (i!=0) {
		 TestMeter->DN		=		EditDN->Text;
	  }


}  else
	{   TestMeter->DN		= "15";
		EditDN->Text =             "15";
	}

		if (TestMeter->VerificationInterval==0) {
		  EditVerificationInterval->Text = "-";
		}  else
		{
		EditVerificationInterval->Text = IntToStr(TestMeter->VerificationInterval);
		 }
		int i1,i2;

		if (TestMeter->Type!=nullptr) {



		 if (TryStrToInt_(TestMeter->Type->VerificationInterval1,i1)) {
		  if (TryStrToInt_(TestMeter->Type->VerificationInterval2,i2)) {
			ButtonInterval1->Text = L"ХВ - "+ TestMeter->Type->VerificationInterval1;
			ButtonInterval2->Text = L"ГВ - "+ TestMeter->Type->VerificationInterval2;

			if (i1!=i2) {
			if (TestMeter->VerificationInterval==i1)
			{
			  ButtonInterval1->IsPressed = true;
			  ButtonInterval2->IsPressed = false;
			}
				else if (TestMeter->VerificationInterval==i2)
			{
			  ButtonInterval1->IsPressed = false;
			  ButtonInterval2->IsPressed = true;
			}
				else
			{
			  ButtonInterval2->IsPressed = false;
			  ButtonInterval1->IsPressed = false;
			}
		   } else
			{
			  TestMeter->VerificationInterval = i1;
			  ButtonInterval2->IsPressed = true;
			  ButtonInterval1->IsPressed = true;
			}
		  }  else
		{
			ButtonInterval1->Text = L"ХВ - "+ TestMeter->Type->VerificationInterval1;
			ButtonInterval2->Text = L"ГВ - "+ TestMeter->Type->VerificationInterval1;
			ButtonInterval2->IsPressed = true;
			ButtonInterval1->IsPressed = true;
		}
		 }  else
		{
			ButtonInterval1->Text = L"-";
			ButtonInterval2->Text = L"-";
		}


		}


}    else
{
		SerialNumEdit->Text = "";
		EditYear->Text = "";

		TestMeter->DN		= "";
		EditDN->Text =             "";
		EditVerificationInterval->Text = "";
}

}

   void TMainTabbedForm::DeviceUpdate()
   {

		/*ComboEditTypeName->Text = TestMeter->DeviceType +" "+TestMeter->Modification+" "+TestMeter->DN;

		EditDeviceType->Text = TestMeter->DeviceType;
		EditModification->Text = TestMeter->Modification;
		CertificateNumEdit->Text = TestMeter->CertificateNum;
		if (TestMeter->Type!=nullptr) {
		 EditYears->Text =  TestMeter->Type->Date1+"-"+TestMeter->Type->Date2;
		}  else   {
		 EditYears->Text = "";
			  }    */



  }

   void TMainTabbedForm::UpdateMesurmentMean(){

	int hash;
	TFlowMeterType *Type;
	UnicodeString M, text, S;
	bool IsContain;

	if (TestMeter!=nullptr) {

	  if (TFlowMeterType::filter==0)
	  {
		//ComboEditTypeName->Tag=1;
		//ComboEditTypeName->Items->Clear();
		//ComboEditTypeName->Text = TestMeter->Modification;//TestMeter->DeviceType +" "++" "+TestMeter->DN;

		EditDeviceType->Text = TestMeter->DeviceType;
		EditModification->Text = TestMeter->Modification;
		CertificateNumEdit->Text = TestMeter->CertificateNum;


		 EditYears->Text =  TestMeter->Date1+"-"+TestMeter->Date2;

				   if (IsInInterval(TestMeter->Date1,
						  TestMeter->Date2,
							 EditYear->Text)==-1)
			{
				EditYear-> TextSettings-> FontColor = claRed;
			 } else
			{
			   EditYear-> TextSettings-> FontColor = claWhite;
			}


			EditDN->Text = 	TestMeter->DN;

	 }
	  else
	  {

		ComboEditTypeName->ItemIndex = -1;
		ComboEditTypeName->Items->Clear();


		 for (int j=0;j<TFlowMeterType::FilterList.size();j++)   //StringGrid1->RowCount
		 {
			Type = TFlowMeterType::MeterTypes[TFlowMeterType::FilterList[j]];

			if (Type -> Visible == true) {
			  ComboEditTypeName->Items->Add(Type->DeviceName +" "+Type->Modification/*+" "+Type->DN*/);
			}
		 }

		 if (TFlowMeterType::FilterList.size()>0)
	{

			  //if (TFlowMeterType::filter==2)

	//		if (index!=-1)
	 {
	 // if (index<TFlowMeterType::FilterList.size()) {
	//	int filter =   TFlowMeterType::FilterList[index];
	  //	Type = TFlowMeterType::MeterTypes[TFlowMeterType::FilterList[filter]];
	  }

		//else
		{
		Type = TFlowMeterType::MeterTypes[TFlowMeterType::FilterList[0]];
		}

		TestMeter->SetType(Type);
		EditDeviceType->Text = Type->DeviceName;
		EditModification->Text = Type->Modification;
		CertificateNumEdit->Text = Type->CertificateNum;
		EditYears->Text =  Type->Date1+"-"+Type->Date2;

		//SerialNumEdit->Text = Type->SerialNum;



			}   else
		{

		 if (TimerFilter->Tag==1) {
 		EditDeviceType->Text = TestMeter->DeviceType;
		EditModification->Text = TestMeter->Modification;
		CertificateNumEdit->Text = TestMeter->CertificateNum;

			  /*	EditDeviceType->Text = Type->DeviceName;
				EditModification->Text = Type->Modification;
				CertificateNumEdit->Text = Type->CertificateNum;
				EditYears->Text =  Type->Date1+"-"+Type->Date2;*/
				TimerFilter->Tag=0;
		 }
		 else
		{
		ComboEditTypeName->Items->Clear();

		EditDeviceType->Text = "";
		EditModification->Text = "";
		CertificateNumEdit->Text = "";
		EditYears->Text = "";
		}
	}



	   }
	  if (TFlowMeterType::FilterList.size()>1) {
				   ButtonComboDown->TextSettings->FontColor = claWhite;
		   ButtonComboDown->Enabled = true;
	  }   else
	   {
			ButtonComboDown->TextSettings->FontColor = claGray;
		   ButtonComboDown->Enabled = false;

	   }
	  } else
{
     		ComboEditTypeName->Items->Clear();

		EditDeviceType->Text = "";
		EditModification->Text = "";
		CertificateNumEdit->Text = "";
		EditYears->Text = "";
}

}

void __fastcall TMainTabbedForm::EditDeviceTypeExit(TObject *Sender)
{
	  TestMeter->DeviceType = EditDeviceType->Text;
	  UpdateMesurmentMean();
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditModificationExit(TObject *Sender)
{
		TestMeter->Modification = EditModification->Text;
        UpdateMesurmentMean();
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::ComboEditTypeNameExit(TObject *Sender)
{
		TimerFilter->Enabled = false;

		 if (ComboEditTypeName->Text == "") {
			TFlowMeterType::filter = 0;
		 }

	   /*
		UpdateMesurmentMean(); */
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::ComboEditTypeNameChange(TObject *Sender)
{
   if (ComboEditTypeName->Tag==0) {

    			TimerFilter->Enabled = true;

	int index =  ComboEditTypeName->ItemIndex;

	if (index!=-1)
	{
	  if (index<TFlowMeterType::FilterList.size()) {
		int filter =   TFlowMeterType::FilterList[index];

		if (filter<TFlowMeterType::MeterTypes.size()) {
		    //ComboEditTypeName->Tag=1;
			TFlowMeterType::filter = 0;
			TimerFilter->Tag = 1;
			//TimerFilter->Enabled = false;
			TestMeter->SetType(TFlowMeterType::MeterTypes[filter]);

			UpdateMesurmentMean();
            	UpdateMesurmentTool();
		}
	  }
	}


   } else
	{
	   ComboEditTypeName->Tag=0;
	}

}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::ComboEditTypeNameKeyDown(TObject *Sender, WORD &Key,
          System::WideChar &KeyChar, TShiftState Shift)
{
		  UpdateMesurmentMean();
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::ComboEditTypeNameEnter(TObject *Sender)
{
			TimerFilter->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::TimerFilterTimer(TObject *Sender)
{

	int hash;
	TFlowMeterType *Type;
	UnicodeString M, text, S;
	bool IsContain;

	text = ComboEditTypeName->Text;
	text = text.Trim();

	 if (text=="") {
		TFlowMeterType::filter = 0;
		TFlowMeterType::FilterList.clear();
		UpdateMesurmentMean();
		//TestMeter->Type = NULL;
	 }

  else if  ( TimerFilter->Tag == 1)
	{
	  TimerFilter->Tag = 0;
	}


	else if (text!=Str)
  {

  /*	if (text =="") {
				   TFlowMeterType::filter = 0;
				  // EditName->ResetFocus();
	}    */


  //	else if ((System::Character::IsLetter((System::WideChar)KeyChar))||(System::Character::IsNumber((System::WideChar)KeyChar)))
  // else




	TFlowMeterType::filter = 1;

	TFlowMeterType::FilterList.clear();
	  IsContain = false;
	  for (int j=0;j<TFlowMeterType::MeterTypes.size();j++)   //StringGrid1->RowCount
  {

	  Type = TFlowMeterType::MeterTypes[j];
	  hash = Type->Hash;// StrToInt(StringGrid1->Cells[8][j]);


	  if (Type!=nullptr) {

	   //	if(j< TFlowMeterType::MeterTypes.size())

	  S = TFlowMeterType::MeterTypes[j]-> CertificateNum;

	  S = S+ TFlowMeterType::MeterTypes[j]-> DeviceType;

	  S = S+ TFlowMeterType::MeterTypes[j]-> DeviceName;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Modification;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Description;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Manufacturer;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Data1;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Data2;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Data3;


		if (StrPos(S.c_str(),text.c_str()))
		 {
		   if (IsInInterval(TFlowMeterType::MeterTypes[j]-> Date1,
						  TFlowMeterType::MeterTypes[j]-> Date2,
							 EditYear->Text)!=-1)
			{
				IsContain = true;
				Type -> Visible = true;
				TFlowMeterType::FilterList.push_back(j);
				ButtonComboDown->TextSettings->FontColor = claWhite;
				ButtonComboDown->Enabled = true;
			}  else
			  {
				Type -> Visible = false;
			  }

		 }   else
		 {
				Type -> Visible = false;
		 }



	}




  }

	if (!IsContain) {
	   ButtonComboDown->TextSettings->FontColor = claGrey;
	   //ButtonComboDown->Enabled = false;
	 }

  UpdateMesurmentMean();
	UpdateMesurmentTool();

 }
	Str = text;
//---------------------------------------------------------------------------
	}
void __fastcall TMainTabbedForm::ButtonComboDownClick(TObject *Sender)
{
	  ComboEditTypeName->DropDown();
}
//---------------------------------------------------------------------------


void __fastcall TMainTabbedForm::ButtonDeleteOrderClick(TObject *Sender)
{

	int size;
	int ARow = OrdersStringGrid->Row;
	size = TOrderClass::Orders.size();


	if (OrdersStringGrid->Tag == 0) {
		if (!TOrderClass::Orders.empty()) {
			if (ARow >= 0) {
				if (ARow < TOrderClass::Orders.size()) {
					TOrderClass::Delete(ARow);
					UpdateOrdersGrid();
					UpdateDevicesGrid();
				}
			}
			else {

			}
		}

}




}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::AddButtonClick(TObject *Sender)
{
		TDialogServiceAsync::MessageDialog(L"Создать новый прибор?",
			TMsgDlgType::mtInformation, mbYesNo, TMsgDlgBtn::mbYes, 0,

			[this](const System::Uitypes::TModalResult AResult)
		{Result = AResult;
			switch (Result) {

				case mrYes : {

					TFlowMeterType *Type = TestMeter->Type;
					TestMeter = new TFlowMeter(HSC, false);

					TestMeter->Type = Type;

					TypeDeviceUpdate();

					break;}; // пользователь нажал Yes

				case mrNo : {TestMeter->SerialNum = SerialNumEdit->Text; break;
				}; default : {} // если нажата другая кнопка
			}
		}
			);
}
//---------------------------------------------------------------------------





void __fastcall TMainTabbedForm::NetHTTPClient1RequestCompleted(TObject * const Sender,
		  IHTTPResponse * const AResponse)
{
	UnicodeString text, Str;
	int i;
	 text =  AResponse->ContentAsString();
	 Str = "OK";
	// Str='"';

	   i =  (int)StrPos(text.c_str(),Str.c_str());
		Memo3->Text =Memo3->Text + " Resp:"+text;

	if (i!=0)
   {
	  TFlowMeter::ApiSent();
	  TFlowMeter::SaveToFile(TestMeter);
	  TSettingsClass::SaveToFile();
	  UpdateDevicesGrid();
	  ShowMessage(L"Данные успешно отправлены.");

   }  else
	{
		text =  "Внимание! Данные не отправлены: " + text;
		ShowMessage(L"Внимание! Данные не отправлены.");
	}

}
//---------------------------------------------------------------------------


void __fastcall TMainTabbedForm::Button24Click(TObject *Sender)
{
	DateTime = Time();
	EtalonMeter->FlowValue -> Reset();

}
//---------------------------------------------------------------------------


void __fastcall TMainTabbedForm::Edit6KeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
	if (Key == 13U) {

			EtalonMeter->FlowValue -> SetValue((float)StrToFloat(Edit6->Text));

			Label28->Text = EtalonMeter->FlowValue->GetStringValue();
			Label29->Text = IntToStr(EtalonMeter->FlowValue->temp_delta);
			Label32->Text = EtalonMeter->FlowValue->GetStringMeanValue();
			  if (EtalonMeter->FlowValue->IsStable(5)) {
				 Label34->Text =  "Stable";
			  }                            else
			{
				  Label34->Text =  "InStable";
            }

	}
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::Button30Click(TObject *Sender)
{

		TDateTime NowDateTime = Time();

   float flt=0;
	Memo4->Lines->Clear();
   for (int i = 0; i < EtalonMeter->FlowValue ->values.size(); i++) {
	  flt = EtalonMeter->FlowValue -> values[i];

	 Memo4->Lines->Add(FloatToStr(flt));

}
	  Label33->Text =  FloatToStr(MilliSecondsBetween(NowDateTime,DateTime)); //IntToStr(SecondOf(TimeOf(DateTime)))+"."+ IntToStr(MilliSecondOf(TimeOf(DateTime)));
}
//---------------------------------------------------------------------------





void __fastcall TMainTabbedForm::ButtonGetDataClick(TObject *Sender)
{

					   UnicodeString str;
			   //	str->Text
					str = TFlowMeter::ApiConvert();

				  TNameValuePair  NameValuePair [4];

				   NameValuePair[0].Name = "Content-Type";
				NameValuePair[0].Value = "application/json";

					NameValuePair[1].Name = "Accept";
				NameValuePair[1].Value = "*/*";

				   NameValuePair[2].Name = "Content-Length";
				NameValuePair[2].Value = "<calculated when request is sent>";

					NameValuePair[3].Name = "Host";
				NameValuePair[3].Value = "<calculated when request is sent>";


				 TStringStream *aBodyStream = new TStringStream(str, TEncoding::UTF8,true);

			   	Memo3->Text = str;
				try                                                    //str
				{
				Memo2->Lines->LoadFromStream( NetHTTPRequest1->Get("https://api.routefinder.ru/order/list-by-date-and-verifier?verifier_id=12&date=2022-07-16")->ContentStream ); // , NULL, TEncoding::Default,NameValuePair);
			   //	NetHTTPRequest1->Post("http://postman-echo.com/post", (System::Classes::TStream*)aBodyStream, NULL, TEncoding::Default,NameValuePair);

				 } catch(...)
						   {

						   }





}
//---------------------------------------------------------------------------





void __fastcall TMainTabbedForm::ButtonJSONClick(TObject *Sender)
{
   System::UnicodeString str, s, s2,str1;
   TJSONValue * JSONValue;
   TJSONObject * JSONObject;
 Memo3->Text="";
   str1 = TOrderClass::JSonConvert(nullptr);   //   {\"colors\":[{\"name\":\"red\", \"hex\":\"#f00\"}]}";
Memo3->Lines->Add(str1);
JSONValue = TJSONObject::ParseJSONValue(str1,false,true);

	  Memo3->Lines->Add(JSONValue->ToString());
Memo3->Lines->Add("READER:");
	s = Edit1->Text;  s2 = "23";
 // str= (UnicodeString)JSONValue->GetValue(s,s2);
  Memo3->Lines->Add("orders");
  Memo3->Lines->Add("ID: "+ str);
  //->Lines->Add("hex: "+ JSONValue->GetValue("colors[0].hex","23"));

}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditYearExit(TObject *Sender)
{
   int i;

	if (TryStrToInt(EditYear->Text, i)) {
	 //	TestMeter->year_production = i;
		// RadioButton3->IsChecked = true;
	}
	else {
		// RadioButton3->IsChecked = false;
	}

	 TestMeter->year_production =  EditYear->Text;

	 UpdateMesurmentMean();
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::DeviceTabItemClick(TObject *Sender)
{
				TFlowMeterType::filter = 0;
                UpdateCommon();
				UpdateMesurmentMean();
				UpdateMesurmentTool();

}
//---------------------------------------------------------------------------



void __fastcall TMainTabbedForm::ButtonInterval1Click(TObject *Sender)
{
	int i;
	if(TestMeter->Type!=nullptr)
{
	if (TryStrToInt(TestMeter->Type->VerificationInterval1,i)) {
	TestMeter->VerificationInterval =  i;
	EditVerificationInterval->Text =TestMeter->Type->VerificationInterval1;
	}
}
}


//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditVerificationIntervalExit(TObject *Sender)
{
   int i;

   if (TryStrToInt_(EditVerificationInterval->Text,i)) {
	TestMeter->VerificationInterval =  i;
	}    else
	 {
		if (TestMeter->Type!=NULL) {
		  EditVerificationInterval->Text =TestMeter->Type->VerificationInterval1;
		}
	 }
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::ButtonInterval2Click(TObject *Sender)
{
	int i;
	if(TestMeter->Type!=nullptr)
{
	if (TryStrToInt_(TestMeter->Type->VerificationInterval2,i)) {
	TestMeter->VerificationInterval =  i;
	EditVerificationInterval->Text =TestMeter->Type->VerificationInterval2;
	}
}
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditDNExit(TObject *Sender)
{
	  int i;

	if (TryStrToInt(EditDN->Text, i)) {
		//TestMeter->DN= i;
		// RadioButton3->IsChecked = true;
	}
	else {
		// RadioButton3->IsChecked = false;
	}

	 TestMeter->DN =  EditDN->Text;
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::ComboBoxUnitsChange(TObject *Sender)
{
	 if (ComboBoxUnits->ItemIndex==0) {
	  LabelBefore->Text = L"Объем до, л";
	  LabelAfter->Text = L"Объем после, л";
	 }
	else if (ComboBoxUnits->ItemIndex==1) {
	  LabelBefore->Text = L"Объем до, м3";
	  LabelAfter->Text = L"Объем после, м3";
		 }

     TSettingsClass::Units = IntToStr(ComboBoxUnits->ItemIndex);
	TSettingsClass::SaveToFile();

	 AfterSpillage();
}
//---------------------------------------------------------------------------



void __fastcall TMainTabbedForm::Button32Click(TObject *Sender)
{
      // alternatively clear only styled font color setting
	 Edit8->StyledSettings = Edit8->StyledSettings >> TStyledSetting::FontColor;

	// and styled size setting
   //	text->StyledSettings = text->StyledSettings >> TStyledSetting::Size;

	// Firemonkey uses TAlphaColor colors
	Edit8->TextSettings->FontColor = TAlphaColor(claRed);


  //Edit8-> TextSettings-> FontColor = claRed;


 if (IsInInterval(Edit8->Text, Edit7->Text, Edit6->Text)) {
   Label42->Text = "TRUE";
 } else
	{
   Label42->Text = "FALSE";
    }


}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditYearsExit(TObject *Sender)
{
		 TestMeter->Date1 = EditYears->Text;
		 TestMeter->Date2 = "";
		if (EditYears->Text == "") {
			TestMeter->Date1 = "";

		}
}
//---------------------------------------------------------------------------




void __fastcall TMainTabbedForm::ButtonCancelStatusClick(TObject *Sender)
{
	if ((!(TestMeter == nullptr)) && (DevicesGrid->Row != -1)) {
		TDialogServiceAsync::MessageDialog(L"Отменить статус прибора?",
			TMsgDlgType::mtInformation, mbYesNo, TMsgDlgBtn::mbNo, 0,
			[this](const System::Uitypes::TModalResult AResult) {

			switch (AResult)

			{

				case mrYes :
				{
				TFlowMeter::FlowMeters[DevicesGrid->Row]->SetSendStatus("-");

				UpdateDevicesGrid();



				break;
				}; // пользователь нажал Yes

				case mrNo : {

					break;};

			}

		});

	}
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::EditDocExit(TObject *Sender)
{
   TestMeter->doc_number =  EditDoc->Text;
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::MemoAdressExit(TObject *Sender)
{
   TestMeter->Adress  =   MemoAdress ->Text;
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::MainTabsChange(TObject *Sender)
{
	 if  (TSettingsClass::IsCheckFill==1) {
	 if (DeviceTabItem->IsSelected != true) {


	if (EditVerificationInterval->Text=="-") {
		ShowMessage(L"Не указан МПИ");
		DeviceTabItem->IsSelected = true;
	}
}
	 }
}
//---------------------------------------------------------------------------

void __fastcall TMainTabbedForm::NetHTTPRequest1RequestError(TObject * const Sender,
          const UnicodeString AError)
{
	ShowMessage(L"Не указан МПИ");
}
//---------------------------------------------------------------------------

