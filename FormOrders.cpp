//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "FormOrders.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TOrdersForm *OrdersForm;

//int TOrdersForm::HashTag = 0;
//---------------------------------------------------------------------------
__fastcall TOrdersForm::TOrdersForm(TComponent* Owner)
	: TForm(Owner)
{


}
//---------------------------------------------------------------------------
void __fastcall TOrdersForm::SpeedButton2Click(TObject *Sender)
{
this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TOrdersForm::Button2Click(TObject *Sender)
{
this->Close();
}
//---------------------------------------------------------------------------


void __fastcall TOrdersForm::ButtonCloseClick(TObject *Sender)
{
this->Close();

}
//---------------------------------------------------------------------------

 void TOrdersForm::Refresh()
{
   				   UnicodeString str;
				   UnicodeString STR;
				   UnicodeString STR1;
				   UnicodeString NowDay;
				   UnicodeString TomorrowDay;
				   UnicodeString URL;

					TDateTime nd = Now();
					TDateTime td = Now();
					td = IncDay(td, 1);

					Word Year, Month, Day, Hour, Min, Sec, MSec;

		DecodeDate(td, Year, Month, Day);
		TomorrowDay = IntToStr(Year)+"-"+IntToStr(Month)+"-"+IntToStr(Day);

	  // TomorrowDay = L"2022-06-23";
					  //	Memo1->Lines->Append("Date: "+td);
	   //	+"+0"+		IntToStr(TTimeZone::Local->UtcOffset.Hours)+":00";

	   DecodeDate(nd, Year, Month, Day);
	   NowDay = IntToStr(Year)+"-"+IntToStr(Month)+"-"+IntToStr(Day);

	  //NowDay = L"2022-06-27";
					   //		Memo1->Lines->Append("Date: "+nd);
  //	Label11->Text = TTimeZone::Local->UtcOffset.;
					//Label12->Text = TTimeZone::Local->UtcOffset.Hours;



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

				 _di_IHTTPResponse Resp;

			   TOrderClass::Orders.clear();

				URL = "https://api.routefinder.ru/order/list-by-date-and-verifier?verifier_id="+IntToStr(TSettingsClass::IDPerformer)+"&date="+NowDay;
					Memo1->Lines->Append("Request:"+URL);
			Resp = NetHTTPClientOrders->Get(URL, aBodyStream);

			STR = Resp->ContentAsString();
				Memo1->Lines->Append("Response1:"+STR);
			 	TOrderClass::StringConvert(STR);


				URL = "https://api.routefinder.ru/order/list-by-date-and-verifier?verifier_id="+IntToStr(TSettingsClass::IDPerformer)+"&date="+TomorrowDay;

				 Memo1->Lines->Append("URL: "+URL);

					Resp = NetHTTPClientOrders->Get(URL, aBodyStream);

				STR1 = Resp->ContentAsString();
				 Memo1->Lines->Append("Responses:"+STR1);

				STR1 = STR1.SubString(STR.Length()+1,STR1.Length());

				Memo1->Lines->Append("Response2:"+STR1);
				TOrderClass::StringConvert(STR1);

				UpdateList();

				TOrderClass::SaveToFile();
}


void __fastcall TOrdersForm::ButtonRefreshClick(TObject *Sender)
{
	  Refresh();
}


void TOrdersForm::UpdateList()
{
   int index=0;

	UnicodeString S;
	UnicodeString str;
	TValue value;
	 TOrderClass::SortOrders();
	int size = TOrderClass::Orders.size();

	TOrderClass *Order;

	if (TSettingsClass::IsOrdersEnabled==0)
	{
		ListViewOrders_->Visible = false;
		return;
	}  else
	{
		ListViewOrders_->Visible = true;
	}


	ListViewOrders_->BeginUpdate();
try {

	ListViewOrders_->Items->Clear();

	if (!TOrderClass::Orders.empty())
	{
	 for (int j = 0; j < size; j++)
	 {
			// if (TFlowMeter::FlowMeters[j]->IsEtalon == false)
			{
		Order = TOrderClass::Orders[j];
		TListViewItem* item = ListViewOrders_->Items->Add();
		item->Tag =  TOrderClass::Orders[j]-> Hash;

		str = 	IntToStr(TOrderClass::Orders[j]->ID_Order);
		value = str;
		item->Data["IdOrder"] = value ;
	  if (TOrderClass::Orders[j]-> Time2.Length()>3) {
		  str = TOrderClass::Orders[j]-> Date +", "+ TOrderClass::Orders[j]->
			 Time1 +"-"+ TOrderClass::Orders[j]-> Time2 ;
		 value = str;
		 item->Data["Date"] = value ;

		 
	  } else
	  {
		  str = TOrderClass::Orders[j]-> Date +", "+ TOrderClass::Orders[j]->
		  Time1;
		 value = str;
		 item->Data["Date"] = value ;
	  }

		str = TOrderClass::Orders[j]-> Date +"  "+ TOrderClass::Orders[j]->
			 Time1;

		//Memo1->Lines->Append("Date final: "+str);

		str = TOrderClass::Orders[j]-> Date +":"+ TOrderClass::Orders[j]->
			 Time1;

		DTime = StrToDateTime(str);
		str =  DateTimeToStr(DTime);
	   //	Memo1->Lines->Append("Date final2: "+str);

		str = TOrderClass::Orders[j]-> Name;
		value = str;
		item->Data["Name"] = value ;

		str = TOrderClass::Orders[j]-> Adress;
		value = str;
		item->Data["Adress"] = value ;

		str = TOrderClass::Orders[j]-> TelNumber;
		value = str;
		item->Data["Tel"] = value ;

		str = TOrderClass::Orders[j]-> Comment;
		value = str;
		item->Data["Comment"] = value ;

		str =  TOrderClass::Orders[j]->GetStatus();
		value = str;
		item->Data["Status"] = value ;

		str = TOrderClass::Orders[j]-> Hash;
		value = str;
		item->Data["Tag"] = value ;

	  //	item-> Purpose =     TListItemPurpose.Footer ;


	  if (HashTag==TOrderClass::Orders[j]-> Hash)
	  {
			 ListViewOrders_->ItemIndex=index;
	  }

	  index = index+1;
	}
   }
 }
}     catch (...)
{

}
	ListViewOrders_->EndUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TOrdersForm::FormShow(TObject *Sender)
{
	TabControl->ActiveTab = TabItem1;
	Refresh();
	UpdateList();
	WebBrowser ->Reload();
}
//---------------------------------------------------------------------------


void __fastcall TOrdersForm::ListViewOrders_ItemClick(TObject * const Sender, TListViewItem * const AItem)

{
	TListViewItem* item = AItem;

		UnicodeString Tel, Hash, str;

		int i;
		TelNumber = item->Data[L"Tel"].AsString();

		 str = 	item->Data["Tag"].AsString();

		TelNumber = str;

	if (TryStrToInt(str, i)) {
		HashTag = i;
		UpdateListBox();
		TabControl->ActiveTab = TabItem3;
	}  else
	{
		HashTag =  0;
	}



//CheckPhonePermission();


}
//---------------------------------------------------------------------------


void __fastcall TOrdersForm::ButtonCallClick(TObject *Sender)
{
   _di_IFMXPhoneDialerService PDService;

if (TPlatformServices::Current->SupportsPlatformService(
__uuidof(IFMXPhoneDialerService)) &&
(PDService = TPlatformServices::Current->GetPlatformService(
__uuidof(IFMXPhoneDialerService))))
{

if ( TelNumber!= "")
{
PDService->Call(TelNumber);
}
else
{
ShowMessage(L"Телефонный номер пуст!");
}
} else {
ShowMessage(L"Нет доступа к службе телефонии!");
}

}
//---------------------------------------------------------------------------

void __fastcall TOrdersForm::ListViewOrders_Paint(TObject *Sender, TCanvas *Canvas,
		  const TRectF &ARect)
{
   //	Canvas->Font->Color = clRed;//TColor(clRed);
}
//---------------------------------------------------------------------------

void __fastcall TOrdersForm::ButtonEnterClick(TObject *Sender)
{
 if (HashTag!=0) {
	TOrderClass::SetActiveHash(HashTag);



	//TestMeter->ID_Order = 0;//TOrderClass::ActiveOrder->ID_Order ;

	MainTabbedForm->SetTestMeterOrder();
	MainTabbedForm->UpdateDeviceChange();
	this->Close();
 }

}
//---------------------------------------------------------------------------

void __fastcall TOrdersForm::ButtonCancelClick(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TOrdersForm::CancelButtonClick(TObject *Sender)
{
  if (HashTag>0) {
	 TOrderClass *Order = TOrderClass::GetOrder(TOrdersForm::HashTag);

	 Order->SetStatus(L"отказ");

				if (Order != nullptr) {
	UnicodeString  text = L"Отменить закаявку?";

	TDialogServiceAsync::MessageDialog(text,
			TMsgDlgType::mtInformation, mbYesNo, TMsgDlgBtn::mbYes, 0,

			[this](const System::Uitypes::TModalResult AResult)
		{
			switch (AResult) {

				case mrYes : {

				   UnicodeString str;
			   //	str->Text

			   if (HashTag>0) {
					TOrderClass *Order = TOrderClass::GetOrder(TOrdersForm::HashTag);

				if (Order != nullptr) {

					str = Order->ApiConvert_editStatus();
							  }



				  if (str==NULL) {
		   //	 abort=true;
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

			   //	Memo3->Text = str;
				try                                                    //str
				{
				NetHTTPClientOrders->Put("https://api.routefinder.ru/order/edit-status", aBodyStream); // , NULL, TEncoding::Default,NameValuePair);
			   //	NetHTTPRequest1->Post("http://postman-echo.com/post", (System::Classes::TStream*)aBodyStream, NULL, TEncoding::Default,NameValuePair);
				 } catch(...)
				 {
					ShowMessage(L"Проблемы соединения. Данные не отправлены!");
				 }



					   //
					   }
			   }
					break;}; // пользователь нажал Yes

				case mrNo : { break;
				}; default : {} // если нажата другая кнопка
			}
		}
			);

 }
	}
}
//---------------------------------------------------------------------------

void __fastcall TOrdersForm::NetHTTPClientOrdersRequestCompleted(TObject * const Sender,
		  IHTTPResponse * const AResponse)
{
	 //ShowMessage(L"Данные отправлены.");
}
//---------------------------------------------------------------------------

 void TOrdersForm::UpdateListBox()
{

   if (HashTag!=0) {
	TOrderClass *Order = TOrderClass::GetOrder(HashTag);

	ListBoxItemName->Text =   Order-> Name;

	UnicodeString status =  Order->GetStatus();

	if (status=="-") {
	  status = L"АКТИВНА";
	}

	LabelDate->Text = L"Заявка №" + IntToStr(Order->ID_Order)+" " + status+" : "
/*ListBoxItemDate->Text = */
				+  Order-> Date +", "+ Order->Time1 +" : "+ Order->Time2;
	ListBoxItemAdress->Text =Order-> AdressShrt;
	ListBoxItemTel->Text =Order-> TelNumber;
	/*
	ListBoxItemEnter->Text = L"Подъезд: 	" + Order-> Enter;//->ItemData->Detail =  Order-> Enter;
	ListBoxItemFloor->Text = L"Этаж: 		" + Order-> Floor;//->ItemData->Detail =  Order-> Floor;
	ListBoxItemApart->Text = L"Квартира: 	" + Order-> Flat;//->ItemData->Detail =  Order-> Flat;

	ListBoxItemPrice->Text = L"Сумма заявки:   " + Order-> Price;//->ItemData->Detail =  Order-> Price;

	ListBoxItem1->Text = 	 L"Гоярчая: 	" + Order-> meter_hot;//->ItemData->Detail =  Order-> meter_hot;
	ListBoxItem2->Text = 	 L"Холодная: 	" + Order-> meter_cold;//->ItemData->Detail =  Order-> meter_cold;
	ListBoxItem4->Text = 	 L"Прочие: 		" + Order-> meter_warm;//->ItemData->Detail =  Order-> meter_warm;
	ListBoxItem3->Text = 	 L"Замена: 		" + Order-> meter_replace;//->ItemData->Detail =  Order-> meter_replace;
	ListBoxItemMoved->ItemData->Detail =  Order-> is_moved;
	ListBoxItemCancelled->ItemData->Detail =  Order-> is_cancelled;
	*/
	LabelEntrance->Text = /*L"Подъезд: 	" + */Order-> Enter;//->ItemData->Detail =  Order-> Enter;
	LabelFloor->Text = /*L"Этаж: 		" +*/ Order-> Floor;//->ItemData->Detail =  Order-> Floor;
	LabelFlat->Text = /*L"Квартира: 	" + */Order-> Flat;//->ItemData->Detail =  Order-> Flat;

	LabelPrice->Text = /*L"Сумма заявки:   " +*/ Order-> Price;//->ItemData->Detail =  Order-> Price;

	LabelHotMeter->Text =  /*	 L"Гоярчая: 	" + */Order-> meter_hot;//->ItemData->Detail =  Order-> meter_hot;
	LabelColdMeter->Text = 	/* L"Холодная: 	" + */Order-> meter_cold;//->ItemData->Detail =  Order-> meter_cold;
	LabelWarm->Text = 	 /*L"Прочие: 		" + */Order-> meter_warm;//->ItemData->Detail =  Order-> meter_warm;
	LabelReplace->Text =   /*	 L"Замена: 		" + */Order-> meter_replace;//->ItemData->Detail =  Order-> meter_replace;

	 if (Order-> is_moved == "true") {
	   LabelMoved->Text =  L"ДА";
	 } else
	 {
	   LabelMoved->Text =  L"НЕТ";
	 }

		 if (Order-> is_cancelled == "true") {
	   LabelCancelled->Text =  L"ДА";
	 } else
	 {
	   LabelCancelled->Text =  L"НЕТ";
	 }


	MemoComment->Text =  Order-> Comment;

}
}




void __fastcall TOrdersForm::SpeedButton3Click(TObject *Sender)
{
	TabControl->ActiveTab = TabItem1;
}
//---------------------------------------------------------------------------

void __fastcall TOrdersForm::SpeedButton4Click(TObject *Sender)
{
TabControl->ActiveTab = TabItem1;
}
//---------------------------------------------------------------------------

void __fastcall TOrdersForm::ListViewOrders_ButtonClick(TObject * const Sender, TListItem * const AItem,
		  TListItemSimpleControl * const AObject)
{
	TabControl->ActiveTab = TabItem2;
}
//---------------------------------------------------------------------------




