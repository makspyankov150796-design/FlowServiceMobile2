//---------------------------------------------------------------------------

#include <fmx.h>


#ifdef _ANDROID_
#include <Androidapi.Helpers.hpp>
#endif

#include <System.Permissions.HPP>
#pragma hdrstop

#include "FormDeviceSlection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TDeviceSelectionForm *DeviceSelectionForm;
uint32_t  temp;
//---------------------------------------------------------------------------
__fastcall TDeviceSelectionForm::TDeviceSelectionForm(TComponent* Owner, THscDevice *HSC)
	: TForm(Owner)
{
this->HSC = HSC;
Spinner->Visible    = false;

  HSC->btThread->btPermissionGranted         = false;

  camPermissionGranted        = false;
  storagePremissionGranted    = false;

HSC->SetOnDiscoverEnd(Bluetooth1DiscoveryEnd);
HSC->SetOnBluetoothConnect(OnBluetoothConnect);
HSC->SetOnPairedDiscoverEnd(BluetoothPairedDiscoveryEnd);


}

void __fastcall TDeviceSelectionForm::Init(THscDevice *HSC)
{
this->HSC = HSC;
Spinner->Visible    = false;

  btPermissionGranted         = false;
  camPermissionGranted        = false;
  storagePremissionGranted    = false;

HSC->SetOnDiscoverEnd(Bluetooth1DiscoveryEnd);
HSC->SetOnBluetoothConnect(OnBluetoothConnect);
HSC->SetOnPairedDiscoverEnd(BluetoothPairedDiscoveryEnd);
//HSC->SetOnDiscoverEndPaired(BluetoothPairedDiscoveryEnd);
}
 //---------------------------------------------------------------------------
// Проверка наличия доступа к хранилищу
void TDeviceSelectionForm::CheckStoragePermission(void)
{
#ifdef _ANDROID_

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

#ifndef _ANDROID_
   storagePremissionGranted = true;
#endif

	}





void __fastcall TDeviceSelectionForm::Bluetooth1DiscoveryEnd(TObject * const Sender,
																TBluetoothDeviceList * const ADeviceList)

{
  TListBox *ListBox = DiscoverDevicesList;
  int32_t deviceNum = 0;

  ButtonScan->Enabled = true;
  Spinner->Enabled    = false;
  Spinner->Visible    = false;

  do
  {
    if(ADeviceList->Count <= 0)
    {
      ShowMessage(L"Bluetooth устройств не обнаружено");
      break;
    }

    ButtonScan->Enabled = true;
    ListBox->Clear();

    for(int32_t i = 0; i < ADeviceList->Count; i++)
    {
      if(ADeviceList->Items[i]->DeviceName.Length())
      {
        ListBox->Items->Add(IntToStr(++deviceNum) + ". " +
                            ADeviceList->Items[i]->DeviceName);
        //
        // Сохраним в свойстве Tag номер BT устройства в списке доступных
        // BT устройств.
        ListBox->ItemByIndex(ListBox->Count-1)->Tag = i;
      }
    }

    ListBox->ItemIndex = (ListBox->Count > 0) ? 0 : -1;

    if(ListBox->ItemIndex > -1)
      ButtonConnect->Enabled = true;

  } while(0);
}


void __fastcall TDeviceSelectionForm::PairedDevicesUpdate(void)
{
  TListBox *ListBox = DiscoverDevicesList;
    int32_t deviceNum = 0;
  TBluetoothDeviceList* PairedDeviceList;
   try{
  PairedDeviceList=   HSC->btThread->PairedDevices();
      }
      catch(const Exception& e)
        {
           // printf("Caught C++ Exception: %s :\n", e.msg());
        }





       if(PairedDeviceList->Count > 0)
    {
    ListBox->Items->Add(L"-----PAIRED DEVICES-----");

    for(int32_t i = 0; i < PairedDeviceList->Count; i++)
     {
      if(PairedDeviceList->Items[i]->DeviceName.Length())
      {
        ListBox->Items->Add(IntToStr(++deviceNum) + L". " +
                            PairedDeviceList->Items[i]->DeviceName);
        //
        // Сохраним в свойстве Tag номер BT устройства в списке доступных
        // BT устройств.
        ListBox->ItemByIndex(ListBox->Count-1)->Tag = i+100;
      }
     }
    }



    ListBox->ItemIndex = (ListBox->Count > 0) ? 0 : -1;

    if(ListBox->ItemIndex > -1)
      ButtonConnect->Enabled = true;
}


void __fastcall TDeviceSelectionForm::BluetoothPairedDiscoveryEnd(TObject * const Sender,
                                                                TBluetoothDeviceList * const ADeviceList,
																TBluetoothDeviceList * const PairedDeviceList)

{
  TListBox *ListBox = DiscoverDevicesList;
    ListBox->Clear();
  int32_t deviceNum = 0;

  ButtonScan->Enabled = true;
  Spinner->Enabled    = false;
  Spinner->Visible    = false;



  do
  {
    if (ADeviceList == nullptr)
    {
       StatusLabel->Text =L"Поиск новых устройств не выполнен";

    }
    else if(ADeviceList->Count <= 0)
    {
       StatusLabel->Text = L"Новых Bluetooth устройств не обнаружено";
       ButtonScan->Enabled = true;
    }
    else
    {

    ButtonScan->Enabled = true;


    for(int32_t i = 0; i < ADeviceList->Count; i++)
    {
      if(ADeviceList->Items[i]->DeviceName.Length())
      {
        ListBox->Items->Add(IntToStr(++deviceNum) + L". " +
                            ADeviceList->Items[i]->DeviceName);
        //
        // Сохраним в свойстве Tag номер BT устройства в списке доступных
        // BT устройств.
        ListBox->ItemByIndex(ListBox->Count-1)->Tag = i;
      }
           StatusLabel->Text =L"Поиск новых устройств выполнен";
    }
   }

   if(PairedDeviceList->Count > 0)
	{
   ListBox->Items->Add(L"-----PAIRED DEVICES-----");

    for(int32_t i = 0; i < PairedDeviceList->Count; i++)
     {
      if(PairedDeviceList->Items[i]->DeviceName.Length())
      {
        ListBox->Items->Add(IntToStr(++deviceNum) + L". " +
                            PairedDeviceList->Items[i]->DeviceName);
        //
        // Сохраним в свойстве Tag номер BT устройства в списке доступных
        // BT устройств.
        ListBox->ItemByIndex(ListBox->Count-1)->Tag = i+100;
      }
     }
    }



    ListBox->ItemIndex = (ListBox->Count > 0) ? 0 : -1;

    if(ListBox->ItemIndex > -1)
      ButtonConnect->Enabled = true;

  } while(0);
}

//---------------------------------------------------------------------------
void __fastcall TDeviceSelectionForm::ButtonScanClick(TObject *Sender)
{
  need = false;
    StatusLabel->Text =L"Поиск новых устройств";
  HSC->CheckBtPermission();
							 /*
  TDialogServiceSync::MessageDialog(L"А оно тебе надо?",
                                TMsgDlgType::mtError,
                                mbYesNo,
                                TMsgDlgBtn::mbNo, 0,
                                [this](const System::Uitypes::TModalResult AResult)
                                {
                                  if(AResult == mrYes)
                                    need = true;
                                });    */
  /*
  TDialogService::MessageDialog(L"А оно тебе надо?",
                                TMsgDlgType::mtError,
                                mbYesNo,
                                TMsgDlgBtn::mbNo, 0,
                                [this](const System::Uitypes::TModalResult AResult)
                                {
                                  if(AResult == mrYes)
                                    need = true;
                                });
    */


  HSC->DiscoverBluetoothDevices();

  ButtonScan->Enabled    = false;
  ButtonConnect->Enabled = false;
  Spinner->Enabled       = true;
  Spinner->Visible       = true;
}
//---------------------------------------------------------------------------
void __fastcall TDeviceSelectionForm::ButtonConnectClick(TObject *Sender)
{
  uint32_t itemIndex = DiscoverDevicesList->ItemIndex;


 // CheckStoragePermission();


  do
  {
    if(!HSC->GetBtPermission())
    {
      ShowMessage(L"Нет доступа к Bluetooth!");
      break;
    }

	if(!HSC->IsBluetoothConnected())
	{
     /*   if (DiscoverDevicesList->ItemByIndex(itemIndex)->Tag < 10)
        {
	  HSC->Connect(DiscoverDevicesList->ItemByIndex(itemIndex)->Tag);
	  temp =  DiscoverDevicesList->ItemByIndex(itemIndex)->Tag;
       } else   */
       StatusLabel->Text =L"Подключение устройства";

	   {
		HSC->Connect(DiscoverDevicesList->ItemByIndex(itemIndex)->Tag);
		temp =  DiscoverDevicesList->ItemByIndex(itemIndex)->Tag-100;
       }

	  ButtonConnect->Enabled       = false;
	  ButtonScan->Enabled          = false;
	  DiscoverDevicesList->Enabled = false;

	}
    else
	{
      StatusLabel->Text =L"Отключение устройства";
      HSC->Disconnect();
    }

	Spinner->Enabled = true;
    Spinner->Visible = true;
  } while(0);
}

void __fastcall TDeviceSelectionForm::OnBluetoothConnect(TObject *Sender, bool connected)
{
  Spinner->Enabled = false;
  Spinner->Visible = false;

  if(connected)
  {
	ButtonConnect->Text        = L"Отключить";
  }
  else
  {
	ShowMessage(L"Подключиться не удалось!");
    StatusLabel->Text =L"Устройство не подключено";
	ButtonScan->Enabled          = true;
	DiscoverDevicesList->Enabled = true;
  }
  ButtonConnect->Enabled = true;
}

//---------------------------------------------------------------------------


void __fastcall TDeviceSelectionForm::Button1Click(TObject *Sender)
{
		DeviceSelectionForm->Close();
}
//---------------------------------------------------------------------------
void __fastcall TDeviceSelectionForm::FormShow(TObject *Sender)
{
  TListBox *ListBox = DiscoverDevicesList;
    ListBox->Clear();
PairedDevicesUpdate();
StatusLabel->Text =L"Ранее подключенные устройства";
}
//---------------------------------------------------------------------------

void __fastcall TDeviceSelectionForm::DiscoverDevicesListChangeCheck(TObject *Sender)

{


	 //	DiscoverDevicesList->ItemByIndex(itemIndex)->Tag
}
//---------------------------------------------------------------------------

void __fastcall TDeviceSelectionForm::DiscoverDevicesListChange(TObject *Sender)
{
   	   int i=DiscoverDevicesList->ItemIndex;
	   UnicodeString Str = DiscoverDevicesList->ItemByIndex(i)->Text;


		if (Str ==L"-----PAIRED DEVICES-----" ) {

			if (i+1> DiscoverDevicesList->Items->Count)
			{
			   DiscoverDevicesList->ItemIndex=-1;
			}   else
			{
			   DiscoverDevicesList->ItemIndex++;
			}
		}

}
//---------------------------------------------------------------------------

