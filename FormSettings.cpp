// ---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "FormSettings.h"

#include "UserRoutines.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
#pragma resource ("*.NmXhdpiPh.fmx", _PLAT_ANDROID)
#pragma resource ("*.Windows.fmx", _PLAT_MSWINDOWS)
#pragma resource ("*.iPhone4in.fmx", _PLAT_IOS)

THSCSettingsForm *HSCSettingsForm;

// ---------------------------------------------------------------------------
__fastcall THSCSettingsForm::THSCSettingsForm(TComponent* Owner) : TForm(Owner)
{
	 HSC=nullptr;




}

// ---------------------------------------------------------------------------
void __fastcall THSCSettingsForm::Button1Click(TObject *Sender) {
	HSCSettingsForm->Close();
}
// ---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::OnVersionRead(TObject *Sender) {

	//VersionEdit	->Text = HSC->Version.Version;
	ProtocolEdit->Text = IntToHex(HSC->Version.Protocol);
	VersionCheckBox->Tag=1;

	 if (TSettingsClass::EtalonCHNum>10)
	 {
		EtalonMeter->SetChannel(TSettingsClass::EtalonCHNum-10);
	 }    else
	 {
	 if (HSC->Version.Protocol>43536) {
	  //  HSC->PeriodState = true;
	 }

	 if (HSC->Version.Protocol>43537) {

		EtalonMeter->SetChannel(1);

	if (TestMeter != nullptr) {
		TestMeter->SetChannel(2);
   }

   } else
	{
		 EtalonMeter->SetChannel(3);
    }
	   }
   // VersionCheckBox->IsChecked = true;
}

void __fastcall THSCSettingsForm::OnSettingsRead(TObject *Sender) {


	Settings = HSC->Settings;
	THSCSettingsForm::SettingsCheckBox->Tag=1;

	THSCSettingsForm::SimComboBox->ItemIndex =
		static_cast<uint8_t>(Settings.Simulator);

	THSCSettingsForm::StartSpillComboBox->ItemIndex =
        static_cast<uint8_t>(Settings.StartSettings);

	THSCSettingsForm::StopSpillComboBox->ItemIndex=
        static_cast<uint8_t>(Settings.StopSettings);

	THSCSettingsForm::ExtSynchComboBox->ItemIndex =
		static_cast<uint8_t>(Settings.ExtSyn);

	THSCSettingsForm::ImpLimitEdit->Text = IntToStr ((unsigned short)Settings.ImpLimit);  //  IntToStr(Settings.ImpLimit);
	 THSCSettingsForm::TimeLimitEdit->Text = IntToStr((unsigned short)Settings.TimeLimit);
	 EditInputSynch->Text = " - ";
	 EditOutSynch->Text = IntToStr(Settings.OutSynCH);

	 EditStopCHNum->Text = IntToStr(Settings.StopCHNum);
	 EditStartCHNum->Text = IntToStr(Settings.StartCHNum);

	   //	THSCSettingsForm::SettingsCheckBox->IsChecked = True;

	   Anim2->Enabled = false;
	   Anim1->Enabled = true;

 //   THSCSettingsForm::
	  SettingsCheckBox->Tag = 0;


        // И так далее

	// Edit1->Text = HSC->Settings.StartSettings;

}

void __fastcall THSCSettingsForm::OnAccessRead(TObject *Sender) {

    if (HSC->Access == 0)
    {
    ValueLabel->Text = L"не принят";
    }
    else if (HSC->Access == 1)
    {
	ValueLabel->Text = L"принят";
	TSettingsClass::status = 1;
    }

    else if (HSC->Access == 2)
	{
	ValueLabel->Text = L"заводской";
	EditEtalonImpChannel->ReadOnly = false;
	EditEtalonImpChannel->Enabled = true;

	EditTestCHNum->ReadOnly = false;
	EditTestCHNum->Enabled = true;

	EtalonImpCoefEdit->ReadOnly = false;
	EtalonImpCoefEdit->Enabled = true;
	TSettingsClass::status = 2;
	}

    else
    {

    ValueLabel->Text = L"другой";
    }




}

void __fastcall THSCSettingsForm::OnConfigRead(TObject *Sender) {


	EditCharge->Text = HSC->GetCharge();

	if (HSC->Access == 0)
	{
    ValueLabel->Text = L"не принят";
    }
	else if (HSC->Access == 1)
	{
	ValueLabel->Text = L"принят";
	TSettingsClass::status = 1;
	}

	else if (HSC->Access == 2)
    {
	ValueLabel->Text = L"заводской";
	TSettingsClass::status = 2;

	EditEtalonImpChannel->ReadOnly = false;
	EditEtalonImpChannel->Enabled = true;

	EditTestCHNum->ReadOnly = false;
	EditTestCHNum->Enabled = true;

	EtalonImpCoefEdit->ReadOnly = false;
	EtalonImpCoefEdit->Enabled = true;


	TSettingsClass::status = 1;

    }

    else
    {

    ValueLabel->Text = L"другой";
    }




    #ifdef __ANDROID__
	 if (!IsInfinite(HSC->Kp) )
    {

	if ( HSC->Kp>0)
    {
    	EtalonImpCoefEdit->Text = FloatToStr(HSC->Kp);
		EtalonMeter->SetKoef(HSC->Kp);
	}

	}

     if (( HSC->SYNCH>=0)&&( HSC->SYNCH<3))
    {
        SynchComboBox->Tag = 1;
		SynchComboBox->ItemIndex =  HSC->SYNCH;
    }

    #endif

        #ifndef __ANDROID__
	if ( _finite(HSC->Kp) )
	{

    if ( HSC->Kp>0)
    {
    EtalonImpCoefEdit->Text = FloatToStr(HSC->Kp);
    EtalonMeter->SetKoef(HSC->Kp);
     }
    }

		if (( HSC->SYNCH>=0)&&( HSC->SYNCH<3))
    {
        SynchComboBox->Tag = 1;
		SynchComboBox->ItemIndex =  HSC->SYNCH;
    }

    #endif

    EtalonMeter->ClearCalibraion();

      // CoefStringGrid->Cells[6][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Error,ffNumber,10,1);

   for (int i=0;i<20;i++)
   {
     EtalonMeter->AddCalibrData(HSC->cEtlVolume[i] , HSC->cTime[i], HSC->cImp[i], HSC->cCoef[i]);
   }

	   Animation2->Enabled = false;
	   Animation1->Enabled = true;

     UpdateCalibrGrid();

     EtalonMeter->UpdateCoefs();

     UpdateChart();

}





void __fastcall THSCSettingsForm::RefreshButtonClick(TObject *Sender) {


	VersionCheckBox->Tag=1;

	Anim2->Enabled = true;
	Anim1->Enabled = false;


	HSC->Read_ConfigData();
	HSC->Read_Version();
	HSC->Read_Settings();
	UpdateUserSettings();

}
// ---------------------------------------------------------------------------

void 	THSCSettingsForm::UpdateUserSettings(void )
{
			IDOrgEdit->Text = IntToStr(TSettingsClass::IDOrg);
			OrgNameEdit->Text = TSettingsClass::OrgName;
			IDPerformerEdit->Text = IntToStr(TSettingsClass::IDPerformer);
			PerformerEdit->Text = TSettingsClass::Performer;
			EditStopSoundFile ->Text =   TSettingsClass::StopSoundFile;
			EditDir ->Text =   TSettingsClass::Dir;
			EditEmail->Text = TSettingsClass::eMail;
			EditSignCipher->Text =  TSettingsClass::SignCipher;
			EditDocNum->Text = TSettingsClass::DocNum ;
			EditMeans->Text = TSettingsClass::Means ;

			ComboBoxDestination->ItemIndex = TSettingsClass::DataDestination;
			ComboBoxOrders->ItemIndex = TSettingsClass::IsOrdersEnabled;

			EditProtocolNumTitle->Text = TSettingsClass::ProtocolNumTitle ;
			EditProtocolNum->Text = IntToStr(TSettingsClass::ProtocolNum) ;

			EditEtalonImpChannel->Text =  IntToStr(EtalonMeter->GetChannel());
			EditTestCHNum->Text = IntToStr(TestMeter->GetChannel());

			ComboBoxDoc->ItemIndex =  TSettingsClass::IsDocNumIndividual;
			ComboBoxAdress->ItemIndex  = TSettingsClass::IsAdressIndividual;
			MemoTitle->Text= TSettingsClass::AllMeans;
			MemoMeans->Text =TSettingsClass::Title ;

			FlowCalcComboBox->ItemIndex = (int)TSettingsClass::IsUsePeriod;

			EditTestCHNum->Text = IntToStr(TSettingsClass::TestCHNum);

		#ifdef __ANDROID__
		  EditPackage->Text =   TSettingsClass::Package;
		#endif

}

void __fastcall THSCSettingsForm::FormShow(TObject *Sender) {

	HSC->SetOnVersionRead(OnVersionRead);
	HSC->SetOnSettingsRead(OnSettingsRead);
    HSC->SetOnAccessRead(OnAccessRead);
    HSC->SetOnConfigRead(OnConfigRead);
    /*
	VersionCheckBox->Tag=1;
    VersionCheckBox->IsChecked = false;
    HSC->Read_ConfigData();
	HSC->Read_Version();
	HSC->Read_Settings();
    */


	EditEtalonImpChannel->Text = IntToStr(EtalonMeter->GetChannel());
	EtalonImpCoefEdit->Text = FloatToStr(EtalonMeter->GetImpKoef());
	EditTestCHNum->Text = IntToStr(TestMeter->GetChannel());

	UpdateUserSettings();

   // UpdateCalibrGrid();
	UpdateChart();
     ////////////
   //   EtalonMeter->ClearCalibraion();
    //AddCalibrData(float vEtlVolume, float vTime, float vImp, float vCoef)
 /*   EtalonMeter->AddCalibrData(20, 		72, 1600, 	83.6);
    EtalonMeter->AddCalibrData(30.1, 	72, 3000, 	83.6);
    EtalonMeter->AddCalibrData(80, 		72, 7000, 	83.6);
    EtalonMeter->AddCalibrData(0, 		72, 0, 		83.6);
	EtalonMeter->AddCalibrData(100, 	72, 10000, 	83.6);
                           */
    UpdateCalibrGrid();

    /*
    EtalonMeter->CalibrPoint.Index = 1;
    EtalonMeter->CalibrPoint.Q = 0;
    EtalonMeter->CalibrPoint.EtlVolume = 0;
    EtalonMeter->CalibrPoint.Volume = 0;
    EtalonMeter->CalibrPoint.Time= 72;
    EtalonMeter->CalibrPoint.Imp = 0;
    EtalonMeter->CalibrPoint.Coef = 83.6;

    EtalonMeter->AddCurrentCalibrPoint();

    EtalonMeter->CalibrPoint.Index = 2;
    EtalonMeter->CalibrPoint.Q = 1000;
    EtalonMeter->CalibrPoint.EtlVolume = 20;
    EtalonMeter->CalibrPoint.Volume = 20.5;
    EtalonMeter->CalibrPoint.Time= 72;
    EtalonMeter->CalibrPoint.Imp = 1600;
    EtalonMeter->CalibrPoint.Coef = 83.6;

     EtalonMeter->AddCurrentCalibrPoint();

    EtalonMeter->CalibrPoint.Index = 2;
    EtalonMeter->CalibrPoint.Q = 1505;
    EtalonMeter->CalibrPoint.EtlVolume = 30.1;
    EtalonMeter->CalibrPoint.Volume = 30.3;
    EtalonMeter->CalibrPoint.Time= 72;
    EtalonMeter->CalibrPoint.Imp = 3000;
    EtalonMeter->CalibrPoint.Coef = 83.6;

     EtalonMeter->AddCurrentCalibrPoint();


         EtalonMeter->CalibrPoint.Index = 2;
    EtalonMeter->CalibrPoint.Q = 4000;
    EtalonMeter->CalibrPoint.EtlVolume = 80;
    EtalonMeter->CalibrPoint.Volume = 80.4;
    EtalonMeter->CalibrPoint.Time= 72;
    EtalonMeter->CalibrPoint.Imp = 7000;
    EtalonMeter->CalibrPoint.Coef = 83.6;

     EtalonMeter->AddCurrentCalibrPoint();


    EtalonMeter->CalibrPoint.Index = 2;
    EtalonMeter->CalibrPoint.Q = 5000;
    EtalonMeter->CalibrPoint.EtlVolume = 100;
    EtalonMeter->CalibrPoint.Volume = 100.3;
    EtalonMeter->CalibrPoint.Time= 72;
    EtalonMeter->CalibrPoint.Imp = 10000;
    EtalonMeter->CalibrPoint.Coef = 83.6;

     EtalonMeter->AddCurrentCalibrPoint();
     */


    ///////////////

}
// ---------------------------------------------------------------------------
void THSCSettingsForm::UpdateChart()
{
   float Q2,Qetl,error;

   Series1->Clear();

   if (!EtalonMeter->Coefs.empty())
  {


     Q2 = EtalonMeter->Coefs[EtalonMeter->Coefs.size()-1].Q2;

  for (int Q=0;Q<Q2*2;Q=Q+100)
   {

    Qetl=Q*EtalonMeter->Rate(Q);

    if (Qetl!=0)
    {
    error = ((Q-Qetl)*100)/Qetl;
    Series1->Add(error,Q,claBlue);
    }


   }

 //Chart1->AddSeries(LineSeries);

  }

}

void THSCSettingsForm::UpdateCalibrGrid()
{
    //  CoefStringGrid->RowCount = EtalonMeter->CalibrPoints.size();
        CoefStringGrid->RowCount = 20;

      for (int i=0;i<CoefStringGrid->RowCount;i++)
    {
       CoefStringGrid->Cells[0][i]= "";
       CoefStringGrid->Cells[1][i]= "";
       CoefStringGrid->Cells[2][i]= "";
       CoefStringGrid->Cells[3][i]= "";
       CoefStringGrid->Cells[4][i]= "";
       CoefStringGrid->Cells[5][i]= "";
       CoefStringGrid->Cells[6][i]= "";
    }

    if (!EtalonMeter->CalibrPoints.empty())
     {
        for (int i=0;i<EtalonMeter->CalibrPoints.size();i++)
    {

	   CoefStringGrid->Cells[0][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Q,ffNumber,10,2);
       CoefStringGrid->Cells[1][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].EtlVolume,ffNumber,10,2);
       CoefStringGrid->Cells[2][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Volume,ffNumber,10,2);
       CoefStringGrid->Cells[3][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Time,ffNumber,10,0);
       CoefStringGrid->Cells[4][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Imp,ffNumber,10,0);
       CoefStringGrid->Cells[5][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Coef,ffNumber,10,3);
       CoefStringGrid->Cells[6][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Error,ffNumber,10,1);
    }
      } else
      {

      }

}

void __fastcall THSCSettingsForm::SimComboBoxChange(TObject *Sender) {
	// SimRadioButtton->IsChecked = False;
	if (THSCSettingsForm::SettingsCheckBox->Tag!=1)
	{
	THSCSettingsForm::SettingsCheckBox->Tag=1;
	THSCSettingsForm::SettingsCheckBox->IsChecked = False;
	Settings.Simulator =  static_cast<bool>(SimComboBox->ItemIndex);
	}
}
// ---------------------------------------------------------------------------
	void __fastcall THSCSettingsForm::SetHscDevice (THscDevice *THSC)
    {
    HSC= THSC;
    }

	void __fastcall THSCSettingsForm::SetFlowMeter (TFlowMeter *TMeter)
    {
    EtalonMeter= TMeter;
    }

	void __fastcall THSCSettingsForm::SetTestMeter (TFlowMeter *TMeter)
    {
    TestMeter= TMeter;
	}
	//--------------------------------------------------------------------------
void __fastcall THSCSettingsForm::SettingsCheckBoxChange(TObject *Sender)
{
		if (SettingsCheckBox->Tag!=1)
	 {
	   SettingsCheckBox->OnChange = nullptr;
	   SettingsCheckBox->Tag = 3;
		if (SettingsCheckBox->IsChecked)
		{
		SettingsCheckBox->IsChecked = False;
		}
		HSC->Write_Settings(&Settings);
		HSC->Read_Settings();
		SettingsCheckBox->OnChange=SettingsCheckBoxChange;
	  }

		if (SettingsCheckBox->Tag==1)
	  {
		SettingsCheckBox->Tag=0;
	  }


}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::VersionCheckBoxChange(TObject *Sender)
{
    if (VersionCheckBox->Tag!=1)
    {
	VersionCheckBox->IsChecked = False;
    HSC->Read_Version();
    }  else
    {
      VersionCheckBox->Tag=0;
    }
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::FormTimerTimer(TObject *Sender)
{
  if  (SettingsCheckBox->Tag == 1){SettingsCheckBox->IsChecked = true;}
 // else {SettingsCheckBox->IsChecked = false;}

   if  (VersionCheckBox->Tag == 1){VersionCheckBox->IsChecked = true;}
//  else {VersionCheckBox->IsChecked = false;}

}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::SettingsChange(TObject *Sender)
{
Integer value;

     SettingsCheckBox->Tag = 0;
     SettingsCheckBox->IsChecked = false;

	   Settings.Simulator  =  static_cast<uint16_t>(THSCSettingsForm::SimComboBox->ItemIndex);

	   Settings.StartSettings = static_cast<EStartSettings>(THSCSettingsForm::StartSpillComboBox->ItemIndex);

	   Settings.StopSettings =  static_cast<EStopSettings>(THSCSettingsForm::StopSpillComboBox->ItemIndex);

	   Settings.ExtSyn =  static_cast<EExtSyn>(THSCSettingsForm::ExtSynchComboBox->ItemIndex);




	   try
	   {
		 if (TryStrToInt(EditInputSynch->Text,value) )
	   {Settings.StartCHNum =  static_cast<uint64_t>(value);
		Settings.StopCHNum =  static_cast<uint64_t>(value);
		}

		if (TryStrToInt(EditStopCHNum->Text,value) )
	   {Settings.StopCHNum =  static_cast<uint64_t>(value);}

		if (TryStrToInt(EditStartCHNum->Text,value) )
	   {Settings.StartCHNum =  static_cast<uint64_t>(value);}


		 if (TryStrToInt(EditOutSynch->Text,value) )
	   {Settings.OutSynCH =  static_cast<uint64_t>(value); }


	   if (TryStrToInt(THSCSettingsForm::ImpLimitEdit->Text,value) )
	   {Settings.ImpLimit =  static_cast<uint64_t>(value); }

       if (TryStrToInt(THSCSettingsForm::TimeLimitEdit->Text,value) )
       {Settings.TimeLimit = static_cast<uint64_t>(value); }
       } catch  (EConvertError &E)
       {

       }

}
//---------------------------------------------------------------------------


void __fastcall THSCSettingsForm::Button2Click(TObject *Sender)
{
     HSC->Write_TimeLimit(StrToInt(TimeLimitEdit->Text));
}
//---------------------------------------------------------------------------


void __fastcall THSCSettingsForm::RefreshConfigButtonClick(TObject *Sender)
{

   String str1="",str2="";
   float Q2,Qetl,error;
   float EtlVolume, Time, Imp, Coef;
   float f;
   bool succes=true;
   char chr = 160U;

    EtalonMeter->ClearCalibraion();


      // CoefStringGrid->Cells[6][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Error,ffNumber,10,1);

   for (int i=0;i<CoefStringGrid->RowCount;i++)
   {

		str1 = StrTrim_(CoefStringGrid->Cells[1][i]);
        if (TryStrToFloat(str1, f))
        {
   	   		EtlVolume = f;
        } else
        {
			CoefStringGrid->Cells[1][i]="";
            if (str1!="")    {succes=false;}
            break;
        }

        str1 = StrTrim_(CoefStringGrid->Cells[3][i]);
        if (TryStrToFloat(str1, f))
        {
   	   		Time = f;
        } else
        {
            CoefStringGrid->Cells[3][i]="";
            if (str1!="")    {succes=false;}
            break;
        }

        str1 = StrTrim_(CoefStringGrid->Cells[4][i]);
        if (TryStrToFloat(str1, f))
        {
   	   		Imp = f;
        } else
        {
            CoefStringGrid->Cells[4][i]="";
            if (str1!="")    {succes=false;}
            break;
        }

        str1 = StrTrim_(CoefStringGrid->Cells[5][i]);
        if (TryStrToFloat(str1, f))
        {
           if (f==0)
           {
				  Coef = EtalonMeter->GetKoef();
           } else
		   {
			Coef = f;
		   }

        } else
        {
			Coef = EtalonMeter->GetKoef();
        }

     EtalonMeter->AddCalibrData(EtlVolume , Time, Imp, Coef);
   }

	 if (succes)
     {
     UpdateCalibrGrid();

     EtalonMeter->UpdateCoefs();

     UpdateChart();

     ShowMessage(L"Коэффициенты пересчитаны");
     }
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EtalonImpCoefEditExit(TObject *Sender)
{
   float f=100;
	if (HSC->Access == 2)
		{
	if (TryStrToFloat(EtalonImpCoefEdit->Text, f))
	{

		Animation1->Enabled=false;
		Animation2->Enabled=true;

		EtalonMeter->SetKoef(f);
		HSC->Write_Coef(f);
		HSC->Save_Flash();
	}


	  }

	 else
	{ EtalonImpCoefEdit->Text =  EtalonMeter->GetKoefStr(); }

}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::ValueEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
		  TShiftState Shift)
{

float temp,f,Q;


   if (Key==13U)
   {


		 ValueEdit->ResetFocus();

   /* for (int i=0;i<CoefStringGrid->RowCount;i++)
   {

   //for (int j=0; j<CoefStringGrid->ColumnCount;j++)
   //{
	 if (CoefStringGrid->Cells[0][i]!="")
	 {



	  // empty = true;
	 }
   //}
	 }   */
   /*
	 EtalonMeter->UpdateCoefs();

      if (TryStrToFloat(ValueEdit->Text, f))
    {
     temp =  EtalonMeter->Rate(f);

     Q = temp * f;

	 ValueLabel->Text= FloatToStrF(Q,ffNumber,10,3);
	  }
	 ValueEdit->ResetFocus();
   }
   */
   }
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::SaveConfigButtonClick(TObject *Sender)
{
 if (HSC->Access > 0)
 {
    EtalonMeter->SaveCalibrData();
    HSC->Save_Flash();
    ShowMessage(L"Коэфициенты сохранены!");
 } else
 {

    ShowMessage(L"Введите пароль!");
 }
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::LoadConfigButtonClick(TObject *Sender)
{
    HSC->Read_ConfigData();
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EtalonCheckBoxChange(TObject *Sender)
{
		 if ((EtalonCheckBox->Tag!=1)&&(EtalonCheckBox->IsChecked == true))
     {
        EtalonCheckBox->Tag = 3;
		EtalonCheckBox->IsChecked = False;
		HSC->Write_SYNCH(SynchComboBox->ItemIndex);

      }

        if (EtalonCheckBox->Tag==1)
      {
        EtalonCheckBox->Tag=0;
      }
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::SynchComboBoxChange(TObject *Sender)
{


		if (SynchComboBox->Tag == 0)
		{
			Animation1->Enabled=false;
			Animation2->Enabled=true;

			HSC->Write_SYNCH(SynchComboBox->ItemIndex);
       		HSC->Read_ConfigData();
        }  else
    {
      SynchComboBox->Tag= 0;
    }

}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::StartSpillComboBoxChange(TObject *Sender)
{
	if (THSCSettingsForm::SettingsCheckBox->Tag!=1)
	{
   //	THSCSettingsForm::SettingsCheckBox->Tag=1;
	 //	SettingsCheckBox->OnChange=nullptr;
	//	if (THSCSettingsForm::SettingsCheckBox->IsChecked)
	{
	   Anim2->Enabled = true;
	   Anim1->Enabled = false;

	}
   //	SettingsCheckBox->OnChange=SettingsCheckBoxChange;
	Settings.StartSettings = static_cast<EStartSettings>(THSCSettingsForm::StartSpillComboBox->ItemIndex);
   //	Settings.StartCHNum = TestMeter->GetChannel();

	//HSC->StartType = THSCSettingsForm::StartSpillComboBox->ItemIndex;

	HSC->Write_Settings(&Settings);
	HSC->Read_Settings();

}

}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::StopSpillComboBoxChange(TObject *Sender)
{
	if (THSCSettingsForm::SettingsCheckBox->Tag!=1)
	{

	   Anim2->Enabled = true;
	   Anim1->Enabled = false;
   //	THSCSettingsForm::SettingsCheckBox->Tag=1;
   //		SettingsCheckBox->OnChange=nullptr;
  //	if (THSCSettingsForm::SettingsCheckBox->IsChecked)
	{
  //	THSCSettingsForm::SettingsCheckBox->IsChecked = False;
	}
   //	SettingsCheckBox->OnChange=SettingsCheckBoxChange;
	Settings.StopSettings =  static_cast<EStopSettings>(THSCSettingsForm::StopSpillComboBox->ItemIndex);
   //	Settings.StopCHNum = TestMeter->GetChannel();
	HSC->StopType = THSCSettingsForm::StopSpillComboBox->ItemIndex;

	HSC->Write_Settings(&Settings);
	HSC->Read_Settings();

	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::ExtSynchComboBoxChange(TObject *Sender)
{


	if (THSCSettingsForm::SettingsCheckBox->Tag!=1)
	{
	//THSCSettingsForm::SettingsCheckBox->Tag=1;
	//THSCSettingsForm::SettingsCheckBox->IsChecked = False;
	Anim2->Enabled = true;
	Anim1->Enabled = false;

	Settings.OutSynCH =  TestMeter->GetChannel();
	Settings.ExtSyn =  static_cast<EExtSyn>(THSCSettingsForm::ExtSynchComboBox->ItemIndex);

	HSC->Write_Settings(&Settings);
	HSC->Read_Settings();
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::Button3Click(TObject *Sender)
{
					   if (CalibrCheckBox->Visible) {
						 CalibrCheckBox->Visible = false;

					   } else
					   {

                       CalibrCheckBox->Visible = true;
                       }


}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::CalibrCheckBoxChange(TObject *Sender)
{
if (CalibrCheckBox->IsChecked)
	{
if (HSC->Access > 0) {
	  HSC->CalibrState = true;
}     else
	 {CalibrCheckBox->IsChecked = false; }



	}
	else
	{
	 HSC->CalibrState = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::DeleteRowButtonClick(TObject *Sender)
{
	if  (CoefStringGrid->Row!=-1)
   {
		CoefStringGrid->Cells[0][CoefStringGrid->Row] = "";
		CoefStringGrid->Cells[1][CoefStringGrid->Row] = "";
		CoefStringGrid->Cells[2][CoefStringGrid->Row] = "";
		CoefStringGrid->Cells[3][CoefStringGrid->Row] = "";
		CoefStringGrid->Cells[4][CoefStringGrid->Row] = "";
		CoefStringGrid->Cells[5][CoefStringGrid->Row] = "";

	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::FlowCalcComboBoxChange(TObject *Sender)
{

	   HSC->PeriodState = (bool) FlowCalcComboBox->ItemIndex;

	if (TSettingsClass::status!=0) {
	TSettingsClass::IsUsePeriod = (bool) FlowCalcComboBox->ItemIndex;
	TSettingsClass::SaveToFile();
	}


}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::Imp1StateComboBoxChange(TObject *Sender)
{
	 TestMeter->Write_Channel_State(static_cast<EStopSettings>(Imp1StateComboBox->ItemIndex));
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::PerformerEditExit(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::Performer = PerformerEdit->Text;
	TSettingsClass::ProtocolNumTitle = TSettingsClass::Performer;
	TSettingsClass::SaveToFile();
	}

}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::ValueEditExit(TObject *Sender)
{
   int pin;

		if (TryStrToInt(ValueEdit->Text, pin))
	{

	   if (pin==0) {

		 TSettingsClass::status = 1;
		 ValueLabel->Text = L"Локальные настройки";

			IDOrgEdit->ReadOnly = false;
			OrgNameEdit->ReadOnly = false;
			IDPerformerEdit->ReadOnly = false;
			PerformerEdit->ReadOnly = false;
			EditStopSoundFile->ReadOnly = false;
			EditDir->ReadOnly = false;
			EditEmail->ReadOnly = false;
			EditSignCipher->ReadOnly = false;
			EditDocNum->ReadOnly = false;
			EditMeans->ReadOnly = false;
			ComboBoxDoc->Enabled = true;
			ComboBoxOrders->Enabled = true;
			ComboBoxDestination->Enabled = true;
			ComboBoxAdress->Enabled = true;
			MemoTitle->ReadOnly = false;
			MemoMeans->ReadOnly = false;

		    EditProtocolNumTitle->ReadOnly = false;
			EditProtocolNum->ReadOnly = false;

		   ButtonDownload->Visible = true;
	   }
	else
		{

	  ValueLabel->Text = "";
	   HSC->Write_PIN(pin);
	   HSC->Read_ConfigAccess();

		ButtonDownload->Visible = false;

		 }
	}

}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::PerformerEditKeyDown(TObject *Sender, WORD &Key,
		  System::WideChar &KeyChar, TShiftState Shift)
{
  if (Key==13U)
   {
		 PerformerEdit->ResetFocus();
   }
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::IDPerformerEditKeyDown(TObject *Sender, WORD &Key,
		  System::WideChar &KeyChar, TShiftState Shift)
{
   if (Key==13U)
   {
		 IDPerformerEdit->ResetFocus();
   }
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::NameOrgEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
   if (Key==13U)
   {
		 OrgNameEdit->ResetFocus();
   }
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::IDOrgEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift)
{
     if (Key==13U)
   {
		 IDOrgEdit->ResetFocus();
   }
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::IDPerformerEditExit(TObject *Sender)
{
	int i;

		if (TryStrToInt(IDPerformerEdit->Text, i))
   {
	if (TSettingsClass::status!=0) {
	TSettingsClass::IDPerformer = i;
	TSettingsClass::SaveToFile();
	}
   }

}
//---------------------------------------------------------------------------


void __fastcall THSCSettingsForm::IDOrgEditExit(TObject *Sender)
{
	int i;

		if (TryStrToInt(IDOrgEdit->Text, i))
   {
	if (TSettingsClass::status!=0) {
	TSettingsClass::IDOrg = i;
	TSettingsClass::SaveToFile();
	}
   }

}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::OrgNameEditExit(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::OrgName = OrgNameEdit->Text;
	TSettingsClass::SaveToFile();
}

}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EditStopSoundFileExit(TObject *Sender)
{
		int i;
	UnicodeString fname;

	if (EditStopSoundFile->Text.Length()>3) {




	fname = TSettingsClass::Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + EditStopSoundFile->Text;




	if (!FileExists(fname)) {

		ShowMessage(Format(L"Файл '%s' не существует.",
	 ARRAYOFCONST((fname))));

	 TSettingsClass::StopSoundFile = "";
   //	TSettingsClass::SaveToFile();

	} else
   {
	TSettingsClass::StopSoundFile = EditStopSoundFile->Text;
	TSettingsClass::SaveToFile();
	}
	}  else
	{
		TSettingsClass::StopSoundFile = " ";
		TSettingsClass::SaveToFile();
	}

}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EditEmailExit(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::eMail = EditEmail->Text;
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EditSignCipherExit(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::SignCipher = EditSignCipher->Text;
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EditDocNumExit(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::DocNum = EditDocNum->Text;
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EditMeansExit(TObject *Sender)
{
    	if (TSettingsClass::status!=0) {
	TSettingsClass::Means = EditMeans->Text;
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------



void __fastcall THSCSettingsForm::EditDirExit(TObject *Sender)
{
		int i;
	UnicodeString fname;

	if (EditDir->Text.Length()>3) {


	fname = TSettingsClass::Dir;

	if (!DirectoryExists(fname,false)) {

		ShowMessage(Format(L"Директория '%s' не существует.",
	 ARRAYOFCONST((fname))));


	fname = TSettingsClass::Dir;



	 TSettingsClass::Dir = fname;
	TSettingsClass::SaveToFile();

	} else
   {
	//TSettingsClass::Dir = fname;
	//TSettingsClass::SaveToFile();
	}
	}  else
	{
	#ifdef __ANDROID__
   //	fname = System::Ioutils::TPath::GetPublicPath();
	#endif

	#ifndef __ANDROID__
	//fname = System::Ioutils::TPath::GetDocumentsPath();
	#endif


	 TSettingsClass::Dir = fname;
	 TSettingsClass::SaveToFile();
	}

}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::ComboBoxDocChange(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::IsDocNumIndividual = ComboBoxDoc->ItemIndex;
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::ComboBoxAdressChange(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::IsAdressIndividual = ComboBoxAdress->ItemIndex;
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::MemoTitleExit(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::Title = MemoTitle->Text;
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::MemoMeansExit(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::AllMeans = MemoMeans->Text;
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EditProtocolNumTitleExit(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::ProtocolNumTitle = EditProtocolNumTitle->Text;
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EditProtocolNumExit(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::ProtocolNum = StrToInt_(EditProtocolNum->Text);
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::ButtonDownloadClick(TObject *Sender)
{

		TDialogServiceAsync::MessageDialog(L"Загрузить базу данных с сервера?",
			TMsgDlgType::mtInformation, mbYesNo, TMsgDlgBtn::mbNo, 0,
			[this](const System::Uitypes::TModalResult AResult) {

			switch (AResult) {

				case mrYes : {


               	UnicodeString Dir = L" ";
	UnicodeString fname = L" ";
	TMemoryStream * MS= new TMemoryStream;
	TFileStream *fs;





	#ifdef __ANDROID__
		Dir =  System::Ioutils::TPath::GetPublicPath();
	#endif

	#ifndef __ANDROID__
		Dir =  System::Ioutils::TPath::GetDocumentsPath();
	#endif

	__try
	{
	__try
	{
		/*
		fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "Settings" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";

		if (FileExists(fname))
		{
		 fs = new TFileStream (fname,fmOpenWrite);
		}  else
		{
		fs = new TFileStream (fname,fmCreate);
		}
		TDownloadURL::DownloadRawBytes("https://getfile.dokpub.com/yandex/get/https://disk.yandex.ru/d/1bDyWrjWBMgtOw",fs);
		 */
		fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "MeterTypes" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";
		 if (FileExists(fname))
		{
		 fs = new TFileStream (fname,fmOpenWrite);
		}  else
		{
		fs = new TFileStream (fname,fmCreate);
		}

	   //	TDownloadURL::DownloadRawBytes("https://getfile.dokpub.com/yandex/get/https://disk.yandex.ru/d/HliWcqGqtaENog",fs);
		NetHTTPClient1->Get("https://getfile.dokpub.com/yandex/get/https://disk.yandex.ru/d/oeHjuOwLO3LPqw",fs);
/*
		fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "1" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "wav";

        		if (FileExists(fname))
		{
		 fs = new TFileStream (fname,fmOpenWrite);
		}  else
		{
		fs = new TFileStream (fname,fmCreate);
		}

		TDownloadURL::DownloadRawBytes("https://getfile.dokpub.com/yandex/get/https://disk.yandex.ru/d/ZZcjDjCsWkW9Bg",fs);
        */

	}
	__finally
	{
        fs->Free();
		delete MS;
	}
	}
	__except(true)//(GetExceptionCode())
	{
		ShowMessage(L"Файлы не загружены с сервера.");
	}


  }
; // пользователь нажал Yes

				case mrNo : {

					break;};


	}
		});

TSettingsClass::LoadFromFile();
 UpdateUserSettings();



}


//---------------------------------------------------------------------------


void __fastcall THSCSettingsForm::EtalonImpChannelEditChange(TObject *Sender)
{
	int i;
	 if (TryStrToInt_(EditEtalonImpChannel->Text, i)) {

		if (TSettingsClass::status!=0) {



		TSettingsClass::EtalonCHNum =  i;
		EtalonMeter->SetChannel(i);
		TSettingsClass::SaveToFile();
	}   else
	{
		EditEtalonImpChannel->Text =  IntToStr(EtalonMeter->GetChannel());
	}
	 } else
	{
		EditEtalonImpChannel->Text =  IntToStr(EtalonMeter->GetChannel());
	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::ComboBoxOrdersChange(TObject *Sender)
{

	if (TSettingsClass::status!=0) {
	TSettingsClass::IsOrdersEnabled = ComboBoxOrders->ItemIndex;
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------


void __fastcall THSCSettingsForm::EditInputSynchExit(TObject *Sender)
{
	if (THSCSettingsForm::SettingsCheckBox->Tag!=1)
	{

	   Anim2->Enabled = true;
	   Anim1->Enabled = false;

		HSC->Write_Settings(&Settings);
		HSC->Read_Settings();

	}


}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EditStopCHNumExit(TObject *Sender)
{
		if (THSCSettingsForm::SettingsCheckBox->Tag!=1)
	{

	   Anim2->Enabled = true;
	   Anim1->Enabled = false;

		HSC->Write_Settings(&Settings);
		HSC->Read_Settings();

	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EditStartCHNumExit(TObject *Sender)
{
		if (THSCSettingsForm::SettingsCheckBox->Tag!=1)
	{

	   Anim2->Enabled = true;
	   Anim1->Enabled = false;

		HSC->Write_Settings(&Settings);
		HSC->Read_Settings();

	}
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EditOutSynchExit(TObject *Sender)
{
		if (THSCSettingsForm::SettingsCheckBox->Tag!=1)
	{

	   Anim2->Enabled = true;
	   Anim1->Enabled = false;

		HSC->Write_Settings(&Settings);
		HSC->Read_Settings();

	}
}
//---------------------------------------------------------------------------


void __fastcall THSCSettingsForm::EditTestCHNumExit(TObject *Sender)
{
	int value;

	if (TSettingsClass::status!=0) {
   if (TryStrToInt(EditTestCHNum->Text,value) )
   {
      	int i;
	 if (TryStrToInt_(EditTestCHNum->Text, i)) {

		TSettingsClass::TestCHNum = i;
		TestMeter->SetChannel(TSettingsClass::TestCHNum);
						}

	}
	TSettingsClass::SaveToFile();
	}

	 EditTestCHNum->Text = IntToStr(TSettingsClass::TestCHNum);
}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::EditInputSynchChange(TObject *Sender)
{
    int value;

		 if (TryStrToInt(EditInputSynch->Text,value) )
	   {
		THSCSettingsForm::SettingsCheckBox->Tag=1;
		Settings.StartCHNum =  static_cast<uint64_t>(value);
		Settings.StopCHNum =  static_cast<uint64_t>(value);
		EditStopCHNum->Text =   IntToStr (Settings.StopCHNum);
		EditStartCHNum->Text  =   IntToStr (Settings.StartCHNum);
		THSCSettingsForm::SettingsCheckBox->Tag=0;
		}

}
//---------------------------------------------------------------------------

void __fastcall THSCSettingsForm::ComboBoxDestinationChange(TObject *Sender)
{
	if (TSettingsClass::status!=0) {
	TSettingsClass::DataDestination = ComboBoxDestination->ItemIndex;
	TSettingsClass::SaveToFile();
	}
}
//---------------------------------------------------------------------------

