//---------------------------------------------------------------------------

#include <fmx.h>
#include <System.DateUtils.hpp>
#pragma hdrstop

#include "THscDevice.h"

#include <System.IOUtils.hpp>

//using namespace std;

#include <System.SysUtils.hpp>

#include "FormDebug.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"


//extern static TDebug Debug;
TDebugForm *DebugForm;
 /*
//---------------------------------------------------------------------------
__fastcall TDebug::TDebug()
{


}

void TDebug::AddMSG (String LocData)
{
      //MSG = new TMSG();

      MSG.Time = Time();
      MSG.Data =  LocData;

      MSGS.push_back(MSG);

      OnAddMsg(this);
}

void TDebug::SetOnAddMSG (TOnAddMSG OnAddMSG)
{
        OnAddMsg =  OnAddMSG;
}

TMSG::TMSG()
{


}

String TMSG::msg ()
{
      String temp;
      temp = Direction+" "+DateTimeToStr(Time)+"::"+Data+" **"+Note;
}

*/

//---------------------------------------------------------------------------
__fastcall TDebugForm::TDebugForm(TComponent* Owner)
	: TForm(Owner)
{
   	Debug = new TDebug;
	Debug->SetOnAddMSG(OnAddMSGForm);
	Debug->OnTransfer = OnTransfer;
	Debug->OnTransferCmplt = OnTransferCmplt;

	Debug->OnRecieve = OnRecieve;
	Debug->OnRecieveCmplt = OnRecieveCmplt;

	Debug->OnError = OnError;

   //SetOnCheckBox->IsChecked = true;
	Debug->active =	  CheckBoxActive->IsChecked ;
}





//---------------------------------------------------------------------------

void __fastcall TDebugForm::OnAddMSGForm()
{

		if ( SetOnCheckBox->IsChecked == true )
	{
       DisplayMSG(Debug->MSG);
	}


	if (Debug->state == 1)
	{
	 if   (DataReadComboBox->ItemIndex==0)
{
	DataReadEdit->Text =   IntToStr(Debug->Int);
}
else if (DataReadComboBox->ItemIndex==1)
{
	DataReadEdit->Text =  IntToHex(Debug->Int);
}
else if (DataReadComboBox->ItemIndex==2)
{
	 DataReadEdit->Text =  FloatToStr(Debug->Float);
}
else if (DataReadComboBox->ItemIndex==3)
{
		DataReadEdit->Text =  Debug->Str;
}
	   Debug->state = 0;
	}


}
//
void __fastcall TDebugForm::FormShow(TObject *Sender)
{
  int c =    Debug->MSGS.size();

  for(int i=0;i<c;i++)
  {
	//  DebugMemo->Lines->Append(Debug->MSGS[i].msg);

  }




}
//---------------------------------------------------------------------------


void __fastcall TDebugForm::CleanButtonClick(TObject *Sender)
{
DebugMemo->Text = "";
DebugMemo->ClearContent();
DebugMemo->Lines->Clear();
}
//---------------------------------------------------------------------------








void __fastcall TDebugForm::PauseButtonClick(TObject *Sender)
{

         PauseButton->Enabled = false;
         PlayButton->Enabled = true;



}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::PlayButtonClick(TObject *Sender)
{
  unsigned  Reg;
  unsigned  Data;
  float FData;
  String str="123";


         // PauseButton->Enabled = true;
         // PlayButton->Enabled = false;
         str =  AdrEdit->Text;
         if (TryStrToUInt(str,Reg))
  {
         // Запись
         if (DirectionComboBox->ItemIndex==0)
   {
         switch (DataWriteComboBox->ItemIndex)
    {
         //DEC
         case 0:
         {
           if (TryStrToUInt(DataWriteEdit->Text,Data))
           {
			   HSC->WriteReg(Reg, (uint16_t)Data);
           }

		 };break;

         //HEX
         case 1:
         {

           if (TryStrToUInt(DataWriteEdit->Text,Data))
           {
               HSC->WriteReg(Reg, (uint16_t)Data);
           }

         };break;

         //Float
         case 2:
         {
           if (TryStrToFloat(DataWriteEdit->Text,FData))
           {
               HSC->WriteReg(81, Reg, FData);
           }
         };break;

    }
   }
       //Чтение
        if (DirectionComboBox->ItemIndex==1)
        {
        if (TryStrToUInt(DataWriteEdit->Text,Data))
        {
         HSC->ReadDataFromDevice(Reg, Data);
        }
        else
        {
          HSC->ReadDataFromDevice(Reg, 1);
        }

        }



  }


		  //HSC->Read;

}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::SetOnCheckBoxChange(TObject *Sender)
{
 /*if (SetOnCheckBox->IsChecked == false )
	 {
	   //	SetOnCheckBox->IsChecked = true;
			TDebug::active = false;
	 }    else
	 {
	   //	SetOnCheckBox->IsChecked = false;
		   TDebug::active = true;
	 }
  */

}
//---------------------------------------------------------------------------

void __fastcall  TDebugForm::DisplayMSG (TMSG MSG)
{
	UnicodeString String;

	do
	{




	if (CheckBoxDataOnly->IsChecked == false )
	{
	// Получаемые данные
	   if ((MSG.Type=="RD")&&(CheckBoxReadData->IsChecked == false ))
	{
	   break;
	}
	// Отправляемые данне данные
	if ((MSG.Type=="WD")&&(CheckBoxWriteData->IsChecked == false ))
	{
	   break;
	}
	// Системные данные
	if ((MSG.Type=="SYS")&&(CheckBoxSys->IsChecked == false ))
	{
	   break;
	}

	 //Подготовка пакета
	if ((MSG.Type=="ENT")&&(CheckBoxPreFrame->IsChecked == false ))
	{
	   break;
	}
	 //Только данные
	 if (((MSG.Type=="RWD")||(MSG.Type=="RRD")||(MSG.Type=="PHT"))&&(CheckBoxDataOnly->IsChecked == false ))
	{
	   break;
	}


	 }
	 else

	 {
		if ((MSG.Type=="RWD")||(MSG.Type=="RRD")||(MSG.Type=="PHT")||(MSG.Type=="ERR"))
	{

	}    else

	{
		break;
	}
	}

	String =  MSG.Type+" : "+MSG.Direction+" :: "+MSG.Time+ ":"+IntToStr(MilliSecondOf(TimeOf(MSG.Time)))+" ::     "+MSG.msg+" **"+MSG.Data;
   //  ///DebugMemo->Lines->Append(Debug->MSG->msg);
	DebugMemo->Lines->Append(" " );
	  DebugMemo->Lines->Append(String);

	if (Debug->state == 1)
	{
	 if   (DataReadComboBox->ItemIndex==0)
{
	DataReadEdit->Text =   IntToStr(Debug->Int);
}
else if (DataReadComboBox->ItemIndex==1)
{
	DataReadEdit->Text =  IntToHex(Debug->Int);
}
else if (DataReadComboBox->ItemIndex==2)
{
	 DataReadEdit->Text =  FloatToStr(Debug->Float);
}
else if (DataReadComboBox->ItemIndex==3)
{
		DataReadEdit->Text =  Debug->Str;
}
	   Debug->state = 0;
	}


	} while (0);

}


void __fastcall TDebugForm::ButtonRefreshClick(TObject *Sender)
{

	int cnt = 0;
	TMSG MSG;

	UnicodeString String;
	cnt = Debug->MSGS.size();
   //	while (!(Debug->MSGS.empty()))

	DebugMemo->Text = "";
	DebugMemo->ClearContent();
	DebugMemo->Lines->Clear();

	for(int i=0;i<cnt;i++)
	{
		DisplayMSG(Debug->MSGS[i]);
	}



}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::Button1Click(TObject *Sender)
{
Close();
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::DirectionComboBoxChange(TObject *Sender)
{
		switch (DirectionComboBox->ItemIndex)
	{
	 case 0:
	 {
	   DataWriteComboBox->Enabled = true;
	 }; break;

	 case 1:
	 {
		DataWriteComboBox->Enabled = false;
	 }; break;

	}
}
//---------------------------------------------------------------------------


void __fastcall TDebugForm::Button3Click(TObject *Sender)
{
if (HSC->PingEnable == True)
{
   HSC->PingOff();
} else
{
   HSC->PingOn();
}
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::DataReadComboBoxChange(TObject *Sender)
{
if   (DataReadComboBox->ItemIndex==0)
{
    DataReadEdit->Text =   IntToStr(Debug->Int);
}
else if (DataReadComboBox->ItemIndex==1)
{
    DataReadEdit->Text =  IntToHex(Debug->Int);
}
else if (DataReadComboBox->ItemIndex==2)
{
     DataReadEdit->Text =  FloatToStr(Debug->Float);
}
else if (DataReadComboBox->ItemIndex==3)
{
        DataReadEdit->Text =  Debug->Str;
}


}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::Button5Click(TObject *Sender)
{
   /*if (Button5->Tag==0)
	{
        HSC->
		 Button5->Tag=1;
	}   else
{

	Button5->Tag=1 ;
}   */
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::Label6Click(TObject *Sender)
{

    OnRecieveCmplt();
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::Label8Click(TObject *Sender)
{

	OnTransferCmplt();
}


void __fastcall TDebugForm::OnRecieve()
  {
	//  GlowGreen1->Enabled=true;
  }

   void __fastcall TDebugForm::OnRecieveCmplt()
  {
   //		AnimGreen2->Start();
	// GlowGreen1->Enabled=false;
  }

   void __fastcall TDebugForm::OnTransfer()
  {
   //	 GlowYellow1->Enabled=true;
  }

   void __fastcall TDebugForm::OnTransferCmplt()
  {
   //	AnimYellow2->Start();
   //	 GlowYellow1->Enabled=false;
  }

   void __fastcall TDebugForm::OnError()
  {
   //  AnimRed2->Start();
  }

//---------------------------------------------------------------------------

void __fastcall TDebugForm::Label9Click(TObject *Sender)
{
   OnError();
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::Button2Click(TObject *Sender)
{
   Debug->MSGS.clear();
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::CheckBoxActiveChange(TObject *Sender)
{
		   Debug->active =	  CheckBoxActive->IsChecked ;
}
//---------------------------------------------------------------------------

