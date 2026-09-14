//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
    TJSONValue *jValue;
    RESTRequest1->Execute();
    jValue = RESTResponse1->JSONValue;
    MemoContent->Text = jValue->ToString();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NetHTTPRequest1RequestCompleted(TObject * const Sender, IHTTPResponse * const AResponse)

{
Memo2->Text =Memo2->Text+ " Resp:"+AResponse->ContentAsString();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
  TStringList *str = new TStringList();

 // str->Add("{}");

  str->Add("{");
  str->Add("\"number_si\":\"d290f1ee-6c54-4b01-90e6-d701748f0851\",");
  str->Add("\"number_grsi\": \"3452345\",");
   str->Add("\"modification\": \"2016-08-29T09:12:33.001Z\",");
  str->Add("\"mp\": \"2452\",");
  str->Add("\"next_verification\": \"12.23.34\", ");
  str->Add("\"pouring_volume_fact\": 123,");
  str->Add("\"pouring_volume_ref\": 143, ");
  str->Add("\"photo1\": \"Widget Adapter\",");
  str->Add("\"photo2\": \"Widget Adapter\",");
  str->Add("\"order_id\": 1");
  str->Add("}");



  NetHTTPRequest1->Post("https://api.routefinder.ru/api/write-data", str, NULL, TEncoding::Default);

  Memo2->Text = Memo2->Text+"Reqst"+str->Text;


}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
  TStringList *str = new TStringList();
  TNameValuePair  NameValuePair [2];
  UnicodeString str1;
 /*
	str->Add("{");
  str->Add("\"order_id\": "Widget Adapter",");
  str->Add("\"mi_owner\": \""Widget Adapter"\",");
   str->Add("\"active\"\": \""Widget Adapter"\",");
  str->Add("\"device_type\": \""Widget Adapter"\",");
  str->Add("\"manufacture_num\": \""Widget Adapter"\",");
  str->Add("\"mitype_number\": \""Widget Adapter"\",");
  str->Add("\"doc_title\": \""Widget Adapter"\",");
  str->Add("\"means\": \""Widget Adapter"\",");
  str->Add("\"modification\": \""Widget Adapter"\",");
  str->Add("\"dn\"\""Widget Adapter"\",");
  str->Add("\"next_verification\"\""Widget Adapter"\",");
  str->Add("\"vrf_date\"\""Widget Adapter"\",");
  str->Add("\"check_type\"\""Widget Adapter"\",");
  str->Add("\"kp\"\""Widget Adapter"\",");
  str->Add("\"q_max\"\""Widget Adapter"\",");
  str->Add("\"temperature\"\""Widget Adapter"\",");
  str->Add("\"temp_water\"\""Widget Adapter"\",");
  str->Add("\"pressure\"\""Widget Adapter"\",");
  str->Add("\"hymidity\"\""Widget Adapter"\",");;
  str->Add("\"result\"\""Widget Adapter"\",");
  str->Add("\"year_production\"\""Widget Adapter"\",");
  str->Add("\"data1\"\""Widget Adapter"\",");
  str->Add("\"data2\"\""Widget Adapter"\",");
  str->Add("\"data3\"\""Widget Adapter"\",");
  str->Add("}");

  //str->Add("{}");

  str1 = str->Text;
   */
  TStringStream *REST_parameters   = new TStringStream(TFlowMeter::ActiveFlowMeter->JSonConvert()) ;

	Memo3->Text = TFlowMeter::ActiveFlowMeter->JSonConvert();

   NameValuePair[0].Name = "Content-Type";
	NameValuePair[0].Value = "application/json";

	NameValuePair[1].Name = "Accept";
	NameValuePair[1].Value = "*/*";





   //	  Memo2->Lines->LoadFromStream(NetHTTPClient1->Post("http://postman-echo.com/post",REST_parameters,0x00, NameValuePair)->ContentStream);
	 Memo2->Lines->LoadFromStream(NetHTTPClient1->Post("https://api.routefinder.ru/api/write-data",REST_parameters,0x00, NameValuePair)->ContentStream);



}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
   TStringList *str = new TStringList();
  TNameValuePair  NameValuePair [2];
  UnicodeString STR;

  STR ="{}";
 // STR=STR+char(34)+ "id"+char(34)+"d290f1ee-6c54-4b01-90e6-d701748f0851"+char(34 )+",";

  TStringStream *REST_parameters   = new TStringStream(STR);

   NameValuePair[0].Name = "Content-Type";
	NameValuePair[0].Value = "application/json";

	NameValuePair[1].Name = "Accept";
	NameValuePair[1].Value = "*/*";


   //	Memo2->Lines->LoadFromStream(NetHTTPClient1->Get("https://api.routefinder.ru/api/get-data",REST_parameters, NameValuePair)->ContentStream);


   STR=	 NetHTTPClient1->Get("https://api.routefinder.ru/api/get-data",REST_parameters, NameValuePair)->ContentAsString();


	  TJSONObject *o = (TJSONObject*) TJSONObject::ParseJSONValue(TEncoding::ASCII->GetBytes(STR),0);



  __try {
	TJSONObject *data =  (TJSONObject*) o->Get("data")->JsonValue;
	TJSONArray *items = (TJSONArray*) data->Get("items")->JsonValue;


for (int idx = 0; idx < items->Size(); idx++) {
	  TJSONObject *item = (TJSONObject*) items->Get(idx);

		for (int idy = 0; idy < item->Count; idy++) {


		/*Memo2->Lines->Text = Memo2->Lines->Text + item->Pairs[idy]->JsonString->ToString() + ':' +
						  item->Pairs[idy]->JsonValue->ToString();  */

/*	ShowMessage( item->Pairs[idx]->JsonString->ToString() + ':' +
					 item->Pairs[idx]->JsonValue->ToString() );     */
	  }
		Memo2->Lines->Text =  Memo2->Lines->Text + item->Get("id")->JsonValue->ToString();
	}


  }
  __finally {
	o->Free();
  }

}
//---------------------------------------------------------------------------

