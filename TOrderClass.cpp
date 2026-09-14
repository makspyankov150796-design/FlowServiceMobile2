//---------------------------------------------------------------------------

#pragma hdrstop

#include "TOrderClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma explicit_rtti


 vector<TOrderClass*>TOrderClass::Orders;
  TOrderClass *TOrderClass::ActiveOrder = new TOrderClass() ;


 TXMLDocument 			*TOrderClass::XmlDoc = nullptr;
_di_IXMLNode 			TOrderClass::rootNode = nullptr;
_di_IXMLNode 			TOrderClass::sampleNode = nullptr;
_di_IXMLNode 			TOrderClass::sampleNode2 = nullptr;
_di_IXMLNode 			TOrderClass::sampleNode3 = nullptr;

 UnicodeString 	TOrderClass::dirName = L"";
 UnicodeString 	TOrderClass::fname = L"";



 TOrderClass ::TOrderClass()
{

	Status = "1";

	 Orders.push_back(this);

}

__fastcall TOrderClass ::~TOrderClass ()
{

}

void TOrderClass ::AddCurrOrder(tPoint Pnt)
{
 // Point = Pnt;
 // Points.push_back(Point);
}

void TOrderClass ::AddOrder(TOrderClass *Order)
{
	 Orders.push_back(Order);

}

void TOrderClass ::SaveToFile()
{

	bool success = true;
	int k=0, l = 0;
	rootNode = nullptr;
	sampleNode = nullptr;

	XmlDoc = new TXMLDocument(NULL);

	XmlDoc->DOMVendor = DOMVendors->Vendors[0];
	// < OMNI XML кроссплатформенный вендор
	XmlDoc->XML->Clear();
	XmlDoc->FileName = "";
	XmlDoc->Active = true;
	//
	// Создадим главную ветку и добавим узел об устройстве

	rootNode = XmlDoc->AddChild(L"Orders");

	// CheckStoragePermission_(this);

	if (!Orders.empty()) {

		rootNode->SetAttribute(L"VER", XMLVER);

			k=  Orders.size();
			while (l < Orders.size())
            {
			if ((Orders[l]->ID_Order==0)&&(Orders[l]->Name==""))
			{
				  TOrderClass::Orders.erase(TOrderClass::Orders.begin()+l);

            }   else
            {
                 l=l+1;
            }

			}



		rootNode->SetAttribute(L"OrderCount", IntToStr((int) Orders.size()));

		for (int j = 0; j < Orders.size(); j++) {


			sampleNode = rootNode->AddChild(L"Order" + IntToStr(j));

			 {
			sampleNode->SetAttribute(L"ID_Order", IntToStr(Orders[j]->ID_Order));

			if (ActiveOrder==nullptr)
			{
			   Orders[j]->Active = 0;
            }
			else if (ActiveOrder->ID_Order==Orders[j]->ID_Order)
			{
				 Orders[j]->Active = 1;
            }
			else
			{
				Orders[j]->Active = 0;
            }


			sampleNode->SetAttribute(L"Active", IntToStr(Orders[j]->Active));
			sampleNode->SetAttribute(L"Performer", Orders[j]->Performer);
			sampleNode->SetAttribute(L"miOwner", Orders[j]->miOwner);
			sampleNode->SetAttribute(L"Status", Orders[j]->Status);
			sampleNode->SetAttribute(L"TelNumber", Orders[j]->TelNumber);
			sampleNode->SetAttribute(L"Name", Orders[j]->Name);
			sampleNode->SetAttribute(L"Adress", Orders[j]->Adress);
			sampleNode->SetAttribute(L"Date", Orders[j]->Date);
			sampleNode->SetAttribute(L"Time1", Orders[j]->Time1);
			sampleNode->SetAttribute(L"Time2", Orders[j]->Time2);

			sampleNode->SetAttribute(L"Geo1", Orders[j]->Geo1);
			sampleNode->SetAttribute(L"Geo2", Orders[j]->Geo2);
			sampleNode->SetAttribute(L"Data1", Orders[j]->Data1);
			sampleNode->SetAttribute(L"Data2", Orders[j]->Data2);
			sampleNode->SetAttribute(L"Data3", Orders[j]->Data3);
			sampleNode->SetAttribute(L"Comment", Orders[j]->Comment);

			sampleNode->SetAttribute(L"Work", Orders[j]->Work);
			sampleNode->SetAttribute(L"Hash", Orders[j]->Hash);

			sampleNode->SetAttribute(L"AdTelNumber", Orders[j]->AdTelNumber);
			sampleNode->SetAttribute(L"AdressShrt", Orders[j]->AdressShrt);
			sampleNode->SetAttribute(L"Enter", Orders[j]->Enter);
			sampleNode->SetAttribute(L"Floor", Orders[j]->Floor);
			sampleNode->SetAttribute(L"Flat", Orders[j]->Flat);

			sampleNode->SetAttribute(L"Price", Orders[j]->Price);

			sampleNode->SetAttribute(L"meter_hot", 		Orders[j]->meter_hot);
			sampleNode->SetAttribute(L"meter_cold", 	Orders[j]->meter_cold);
			sampleNode->SetAttribute(L"meter_warm", 	Orders[j]->meter_warm);
			sampleNode->SetAttribute(L"meter_replace", 	Orders[j]->meter_replace);
			sampleNode->SetAttribute(L"is_moved", 		Orders[j]->is_moved);
			sampleNode->SetAttribute(L"is_cancelled", 	Orders[j]->is_cancelled);





		  /*	if (!FlowMeters[j]->Points.empty()) {
				for (int i = 0; i < FlowMeters[j]->Points.size(); i++) {
					sampleNode2 = sampleNode->AddChild(L"Point" + IntToStr(i));

					sampleNode2->SetAttribute(L"Name",
						FlowMeters[j]->Points[i].Name);
					sampleNode2->SetAttribute(L"Qrate",
						FloatToStrF(FlowMeters[j]->Points[i].Qrate, ffNumber,
						10, 2));
					sampleNode2->SetAttribute(L"Q",
						FloatToStrF(FlowMeters[j]->Points[i].Q, ffNumber,
						10, 2));
					sampleNode2->SetAttribute(L"Volume",
						FloatToStrF(FlowMeters[j]->Points[i].Volume,
						ffNumber, 10, 2));
					sampleNode2->SetAttribute(L"Time",
						FloatToStrF(FlowMeters[j]->Points[i].Time, ffNumber,
						10, 0));
					sampleNode2->SetAttribute(L"Error",
						FloatToStrF(FlowMeters[j]->Points[i].Error, ffNumber,
						10, 2));

				}

			}   */



		  }




		}
	}  else
	{
		rootNode->SetAttribute(L"VER", XMLVER);
		rootNode->SetAttribute(L"OrderCount", "0");
	}

	// XmlDoc->SaveToFile(dirName + L"/Report.xml");

	fname = TSettingsClass::Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "Orders" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";



	XmlDoc->SaveToFile(fname);

	// XmlDoc->L
	XmlDoc->Active = false;

}

TOrderClass *TOrderClass::LoadFromFile(void) {

	bool success = true;

	XmlDoc = nullptr;
	rootNode = nullptr;
	sampleNode = nullptr;

	TOrderClass *Order;

	String str;

	int Size;
	int PointsSize;
	int index1 = 0;
	int index2 = 0;
	 int  size, id;

	_di_IXMLNode rootNode1;

	_di_IXMLDocument document = interface_cast<Xmlintf::IXMLDocument>
		(new TXMLDocument(NULL));

	UnicodeString SerialNum;
	UnicodeString dDir, FileName;
	UnicodeString VER;

	UnicodeString Result;

	_di_IXMLNodeList nodeList;

	// CheckStoragePermission_(this);

	XmlDoc = new TXMLDocument(NULL);



	fname = TSettingsClass::Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "Orders" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";


	if (!FileExists(fname)) {

		Result = L"Файл заказов не существует";
	   //	throw Exception(L"Файл типов не существует");
		ShowMessage(Format(L"Файл '%s' не существует. Создан новый файл.",
	 ARRAYOFCONST((fname))));
		 SaveToFile();

	}

	document = LoadXMLDocument(fname);

	if (document == NULL) {

		Result = L"Файл пустой или поврежден";
		throw Exception(Result);

	}

	TOrderClass::Orders.clear();

	rootNode = document->ChildNodes->FindNode(L"Orders");

	if (rootNode == NULL) {
		Result = L"Файл пустой или поврежден";
		throw Exception(Result);
	}


	// САМАЯ РАНЯЯ ВЕРСИЯ !


	 VER = rootNode->Attributes[L"VER"];


	if (VER == "4.0") {

		Size = rootNode->Attributes[L"OrderCount"];

		for (int j = 0; j < Size; j++) {

			sampleNode = rootNode->ChildNodes->FindNode(L"Order" + IntToStr(j));

			if (sampleNode != NULL) {

				 if (TryStrToInt(sampleNode->Attributes[L"ID_Order"],id))
				 {

				 Order = new TOrderClass() ;

				Order->ID_Order = id;
				Order->Active = StrToInt(sampleNode->Attributes[L"Active"]);

				if (Order->Active == 1)
				 {
					ActiveOrder = Order;
				 }

				 //try
				 {
			Order->Performer = sampleNode->Attributes[L"Performer"];
			Order->miOwner = sampleNode->Attributes[L"miOwner"];
			Order->Status = sampleNode->Attributes[L"Status"];
			Order->TelNumber = sampleNode->Attributes[L"TelNumber"];
			Order->Name = sampleNode->Attributes[L"Name"];
			Order->Adress = sampleNode->Attributes[L"Adress"];
			Order->Date = sampleNode->Attributes[L"Date"];
			Order->Time1 = sampleNode->Attributes[L"Time1"];
			Order->Time2 = sampleNode->Attributes[L"Time2"];
			Order->Geo1 = sampleNode->Attributes[L"Geo1"];
			Order->Geo2 = sampleNode->Attributes[L"Geo2"];
			Order->Data1 = sampleNode->Attributes[L"Data1"];
			Order->Data2= sampleNode->Attributes[L"Data2"];
			Order->Data3 = sampleNode->Attributes[L"Data3"];
			Order->Comment = sampleNode->Attributes[L"Comment"];
			Order->Hash  = StrToInt (GetIntAttribute(L"Hash",sampleNode));



			Order->AdTelNumber= GetAttribute(L"AdTelNumber",sampleNode);
			Order->AdressShrt= GetAttribute(L"AdressShrt",sampleNode);
			Order->Enter= GetAttribute(L"Enter",sampleNode);
			Order->Floor= GetAttribute(L"Floor",sampleNode);
			Order->Flat= GetAttribute(L"Flat",sampleNode);

			Order->Price= GetAttribute(L"Price",sampleNode);

			Order->meter_hot= GetAttribute(L"meter_hot",sampleNode);
			Order->meter_cold= GetAttribute(L"meter_cold",sampleNode);
			Order->meter_warm= GetAttribute(L"meter_warm",sampleNode);
			Order->meter_replace= GetAttribute(L"meter_replace",sampleNode);
			Order->is_moved= GetAttribute(L"is_moved",sampleNode);
			Order->is_cancelled= GetAttribute(L"is_cancelled",sampleNode);

			  }
				 }

			}
			else
			{
				Result = L"Файл пустой или поврежден";
				throw Exception(Result);
			}


		}


	}

	else if (VER == XMLVER) {


		Size = rootNode->Attributes[L"OrderCount"];

		for (int j = 0; j < Size; j++) {

			sampleNode = rootNode->ChildNodes->FindNode(L"Order" + IntToStr(j));

			if (sampleNode != NULL) {

				Order = new TOrderClass() ;

				Order->ID_Order = StrToInt(sampleNode->Attributes[L"ID_Order"]);
				Order->Active = StrToInt(sampleNode->Attributes[L"Active"]);

				if (Order->Active == 1)
				 {
					ActiveOrder = Order;
				 }

				 //try
				 {
			Order->Performer = sampleNode->Attributes[L"Performer"];
			Order->miOwner = sampleNode->Attributes[L"miOwner"];
			Order->Status = sampleNode->Attributes[L"Status"];
			Order->TelNumber = sampleNode->Attributes[L"TelNumber"];
			Order->Name = sampleNode->Attributes[L"Name"];
			Order->Adress = sampleNode->Attributes[L"Adress"];
			Order->Date = sampleNode->Attributes[L"Date"];
			Order->Time1 = sampleNode->Attributes[L"Time1"];
			Order->Time2 = sampleNode->Attributes[L"Time2"];
			Order->Geo1 = sampleNode->Attributes[L"Geo1"];
			Order->Geo2 = sampleNode->Attributes[L"Geo2"];
			Order->Data1 = sampleNode->Attributes[L"Data1"];
			Order->Data2= sampleNode->Attributes[L"Data2"];
			Order->Data3 = sampleNode->Attributes[L"Data3"];
			Order->Comment = sampleNode->Attributes[L"Comment"];
			Order->Hash  = StrToInt (GetIntAttribute(L"Hash",sampleNode));

			Order->AdTelNumber= GetAttribute(L"AdTelNumber",rootNode);
			Order->AdressShrt= GetAttribute(L"AdressShrt",rootNode);
			Order->Enter= GetAttribute(L"Enter",rootNode);
			Order->Floor= GetAttribute(L"Floor",rootNode);
			Order->Flat= GetAttribute(L"Flat",rootNode);

			Order->Price= GetAttribute(L"Price",rootNode);

			Order->meter_hot= GetAttribute(L"meter_hot",rootNode);
			Order->meter_cold= GetAttribute(L"meter_cold",rootNode);
			Order->meter_warm= GetAttribute(L"meter_warm",rootNode);
			Order->meter_replace= GetAttribute(L"meter_replace",rootNode);
			Order->is_moved= GetAttribute(L"is_moved",rootNode);
			Order->is_cancelled= GetAttribute(L"is_cancelled",rootNode);

				  }

			}
			else
			{
				Result = L"Файл пустой или поврежден";
				throw Exception(Result);
			}


		}


	}

	else
	{
	Result = L"Версия не может быть обработана";
	throw Exception(Result);
	}





	return  ActiveOrder;
}

UnicodeString TOrderClass::GetJsonValue(TJSONValue * JSONValue, UnicodeString Group,UnicodeString Class, int i,UnicodeString string)
{
	UnicodeString str, s, err, test;
	test =  JSONValue->ToString();
	err = L"not exist";
	s = Group+"."+Class+L"["+IntToStr(i)+L"]"+L"." + string;

	#ifdef __ANDROID__
	try {
	  str= (UnicodeString)JSONValue->GetValue<UnicodeString>((UnicodeString)s);
	} catch (...) {
	  str= err ;
	}

	#endif

	#ifndef __ANDROID__
		try {

	  if (JSONValue->FindValue((UnicodeString)s)!=nullptr) {
		  str= (UnicodeString)JSONValue->GetValue<UnicodeString>((UnicodeString)s);
	  }    else
	{
       str= err ;
    }

	} catch (...) {
	  str= err ;
	}
	#endif


	return str;

}

void TOrderClass::StringConvert(UnicodeString string)
{
	bool success = true, end = false;
	int i=0 ,k=string.Length(), l = 0;
	XmlDoc = nullptr;
	rootNode = nullptr;
	sampleNode = nullptr;

	TOrderClass *Order;

	TJSONValue * JSONValue;
	UnicodeString str;

	 TJSONObject *jObject;
	 TJSONArray *jOrders;
	 TJSONObject * jOrder;


	// TJSONObject *jObject = new TJSONObject();
	 jObject = new TJSONObject();

	 try
	{
	  JSONValue = TJSONObject::ParseJSONValue(string,false,true);


 	 } catch(...)
	{
	   return;
    }


	while (!end)
   {
		int ID, ii;

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"id");

		if ((str!=L"not exist")&&(TryStrToInt_(str,ID))) {

		Order = new TOrderClass();

		Order->ID_Order = ID;

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"latitude");
		Order->Geo1=str;
		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"longitude");
		Order->Geo2=str;
		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"address");
		Order->Adress=str;
		Order->AdressShrt=str;

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"meter_hot");
		Order->meter_hot=str;
		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"meter_cold");
		Order->meter_cold=str;
		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"meter_warm");
		Order->meter_warm=str;
		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"meter_replace");
		Order->meter_replace=str;
		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"flat");
		if (TryStrToInt(str,ii)) {
		  if (ii>0) {
		   Order->Adress=Order->Adress+L" кв: "+str;
		  }
		}

		Order->Flat=str;

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"entrance");
		if (TryStrToInt(str,ii)) {
		  if (ii>0) {
		   Order->Adress=Order->Adress+L" вх: "+str;
		  }
		}

		Order->Enter=str;

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"name");
		Order->Name=str;

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"phone");
		Order->TelNumber=str;

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"comment");
		Order->Comment=str;

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"is_preliminary");

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"time_start");
		str = str.SubString(1,5)+" ";
		Order->Time1=str;
		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"time_finish");
        str = " " + str.SubString(1,5);
		Order->Time2=str;
		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"price");

		Order->Price=str;

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"floor");
		if (TryStrToInt(str,ii)) {
		  if (ii>0) {
		   Order->Adress=Order->Adress+L" эт: "+str;
		  }
		}

		Order->Floor=str;

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"created_at");

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"is_cancelled");
		Order->is_cancelled=str;


		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"is_moved");
		Order->is_moved=str;
		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"route_id");

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"additional_number");
		Order->AdTelNumber=str;



		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"date");

		str = str.SubString(9,2)+"."+str.SubString(6,2)+"."+str.SubString(1,4);
		Order->Date=str;

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"status");
		//Order->Status=str;
		Order->SetStatus(str);

		str = GetJsonValue(JSONValue,L"data",L"orders", i,L"fixed_time");

		if (str.Length()>3) {
			str = str.SubString(1,5)+" ";
			Order->Time1=str;
			Order->Time2="";
		}


		Order->Hash = Order->GetHashCode();

		 i++;

		}  else
		{
			end = true;
		}
   }

	 return;

}

UnicodeString TOrderClass::JSonConvert(TOrderClass *AOrder)
{

	bool success = true;
    int k=0, l = 0;
	XmlDoc = nullptr;
	rootNode = nullptr;
	sampleNode = nullptr;

     TJSONObject *jObject;

	 TJSONArray *jOrders;


	 TJSONObject * jOrder;


	// TJSONObject *jObject = new TJSONObject();
	 jObject = new TJSONObject();


	//
	// Создадим главную ветку и добавим узел об устройстве

	jObject->AddPair(L"VER", XMLVER);




	// CheckStoragePermission_(this);

	if (!Orders.empty()) {

			k=  Orders.size();
			while (l < Orders.size())
			{
			if ((Orders[l]->ID_Order==0)&&(Orders[l]->Name==""))
			{

				  Orders.erase(Orders.begin()+l);

			}   else
			{
                 l=l+1;
			}

			}



		jObject->AddPair( new TJSONPair(L"OrderCount", IntToStr((int) Orders.size())));

		jOrders = new TJSONArray();
		jObject->AddPair(L"Orders",jOrders);


		for (int j = 0; j < Orders.size(); j++) {

			jOrder = new TJSONObject();
			jOrder->AddPair( new TJSONPair( L"Order Num" , IntToStr(j)));

			 {
			jOrder->AddPair( new TJSONPair(L"ID_Order", Orders[j]->ID_Order));
			jOrder->AddPair( new TJSONPair(L"ID_Order", Orders[j]->ID_Order));
			jOrder->AddPair( new TJSONPair(L"miOwner", Orders[j]->miOwner));
			jOrder->AddPair( new TJSONPair(L"Status", Orders[j]->Status));
			jOrder->AddPair( new TJSONPair(L"TelNumber", Orders[j]->TelNumber));
			jOrder->AddPair( new TJSONPair(L"Name", Orders[j]->Name));
			jOrder->AddPair( new TJSONPair(L"Adress", Orders[j]->Adress));
			jOrder->AddPair( new TJSONPair(L"Date", Orders[j]->Date));
			jOrder->AddPair( new TJSONPair(L"Time1", Orders[j]->Time1));
			jOrder->AddPair( new TJSONPair(L"Time2", Orders[j]->Time2));
			jOrder->AddPair( new TJSONPair(L"Geo1", Orders[j]->miOwner));
			jOrder->AddPair( new TJSONPair(L"Geo2", Orders[j]->miOwner));
			jOrder->AddPair( new TJSONPair(L"Data1", Orders[j]->miOwner));
			jOrder->AddPair( new TJSONPair(L"Data2", Orders[j]->miOwner));
			jOrder->AddPair( new TJSONPair(L"Data3", Orders[j]->miOwner));
			jOrder->AddPair( new TJSONPair(L"Comment", Orders[j]->miOwner));

		   jOrders->AddElement(jOrder);

			 }

			}



	}  else
	{

	}

	 return jObject->ToString();

}

UnicodeString TOrderClass::GetStatus()
{
  UnicodeString status=L"";
  int st;
   if (TryStrToInt(Status,st)) {

	switch (st) {

	case 0:
	{
	   status = L"не активна";
	}; break;

	case 1:
	{
	   status = L"сформирована";
	}; break;

	case 2:
	{
	   status = L"подтверждена";
	}; break;

	case 3:
	{
	   status = L"начато исполнение";
	}; break;

	case 4:
	{
	   status = L"отправлены результаты";
	}; break;

	case 5:
	{
	   status = L"";
	}; break;

	case 6:
	{
	   status = L"-";
	}; break;

	case 7:
	{
	   status = L"исполнена";
	}; break;

	case 8:
	{
	   status = L"отказ";
	}; break;

	default:
		;
	}
	 }

 return status;
}

void TOrderClass::SetStatus( UnicodeString status)
{

	//int st = StrToInt(status);

	if (status==L"не активна") {

		   Status =L"0";
		  }
	else if (status==L"сформирована") {
		   Status =L"1";
		 }
	else if (status==L"подтверждена") {
		   Status =L"2";
		 }
	else if (status==L"начато исполнение") {
		   Status =L"3";
		 }
	else if (status==L"отправлены результаты") {
			Status =L"4";
		 }

	else if (status==L"-") {
			Status =L"6";
		 }

	else if (status==L"исполнена") {
			Status =L"7";
		 }

	else if (status==L"отказ") {
			Status =L"8";
		 }

	 else {
		  Status =L"0";
     }

}

bool TOrderClass::SetActive(TOrderClass *AOrder)
{
	ActiveOrder = AOrder;


		if (ActiveOrder!=NULL) {
       ActiveOrder->Active = 1;
	   return true;
}
	   return false;

}

TOrderClass * TOrderClass::GetOrder (int Hash)
{
	for (int j = 0; j < Orders.size(); j++)
	{
	  if (Orders[j]->Hash == Hash) {
			return  Orders[j];
	  }
	}

	return nullptr;
}

TOrderClass * TOrderClass::GetActiveOrder ()
{

	  if (ActiveOrder!= NULL)
	  {
		return  ActiveOrder;
	  }

	return nullptr;
}

bool TOrderClass::SetActiveHash(int Hash)
{
	ActiveOrder = GetOrder(Hash);
	ActiveOrder->Active = 1;
	if (ActiveOrder!=NULL) {
	   return true;
}
	   return false;
}

bool TOrderClass::SetActive(int index)
{
	ActiveOrder = Orders[index];
	ActiveOrder->Active = 1;
if (ActiveOrder!=NULL) {
	   return true;
}
	   return false;
}

void TOrderClass::Delete(int index)
{

	/*	for (int j = 0; j < MeterTypes.size(); j++)
	{
	  if (MeterTypes[j]->Hash == Hash) {


			return  ;
	  }
	}

	return ;  */

	TOrderClass::Orders.erase(TOrderClass::Orders.begin()+index);
}

UnicodeString TOrderClass::ApiConvert_editStatus()
{
	TJSONObject *jObject;

	jObject = new TJSONObject();

	jObject->AddPair(L"id",  TOrderClass::ID_Order);
	jObject->AddPair(L"status", GetStatus());

	return jObject->ToString();

}

void TOrderClass::SortOrders()
{
	 vector <TOrderClass*> TempOrders;
	 int cmp1=0;
	  int j=0;
	 UnicodeString  Str, Str1, Str2;
	 TOrderClass*Order;
	 int cmp, st, pos, pos1,pos2,pos3;
	 std::vector<TOrderClass*>::iterator it,it1,it2,it3;
	 TDateTime time1, time2;
	 /*

   for (int i = 1; i < Orders.size(); i++)
 {
	for (int j = 0; j < Orders.size(); j++)
	{

		time1 = StrToTime(Orders[i]->Time1);
		time2 = StrToTime(Orders[j]->Time1);
		  if (Orders[i]->GetStatus()==L"отказ")
		{
		 if (CompareTime(time1, time2))
	  {
		   Order = Orders[i];
	  }

	  else if (j==TempOrders.size()-1)
	  {
			 TempOrders.insert(TempOrders.end(), Orders[i]);
			 break ;
	  }
	   it++;
	}



 }
	   */

  //	TempOrders.resize ( Orders.size());

  if (TempOrders.size()==0) {
	  //	 TempOrders.insert(TempOrders.end(), Orders[0]);
	}
		it1 = TempOrders.begin();
		it2 = TempOrders.begin();
		it3 = TempOrders.begin();
		pos1 = 0;
		pos2 = 0;
		pos3 = 0;

	//  it1++;

	//  TempOrders.insert(it1, Orders[0]);
   //	  it1++;
	//	  TempOrders.insert(it1, Orders[0]);



  //	 Str1 =   Orders[4]->Date+":"+Orders[4]->Time1 ;
   //	 Str2 =   Orders[2]->Date+":"+Orders[2]->Time1 ;

  //	 time1 =  StrToDateTime(Str1);
  //	 time2 =  StrToDateTime(Str2);

  //	 j =  CompareDateTime(time2, time1);

  for (int i = 0; i < Orders.size(); i++)
 {
	  st=0;
	  Order = Orders[i];
	  time1 = StrToDateTime(Orders[i]->Date+":"+Orders[i]->Time1);

	  it = TempOrders.begin();
	//for (int j = 0; ; j++)
	   /*	if (Order->GetStatus()==L"-")
		{
		  it = it1;
		  it1++;
		}
	   */


	  for (int j = 0; j < TempOrders.size(); j++)
	{
		time2 = StrToDateTime(TempOrders[j]->Date+":"+TempOrders[j]->Time1);

	  if (TempOrders[j]->GetStatus()==Order->GetStatus())
	  {
		cmp1 =  CompareDateTime(time1, time2);
		if (cmp1<1)
	   {

		 if (Order->GetStatus()==L"-") {

			 st=1;
		   //	it = it1;
			 pos1++;
			 pos2++;
			 pos3++;

			break;
		 }
		 else if (Order->GetStatus()==L"исполнена")
		 {
			 st=1;
		  //	 it = it2;
			 pos2++;
			 pos3++;
			 break;
		 }
		 else if (Order->GetStatus()==L"отказ")
		 {
			st=1;
		   //	 it = it3;
			 pos3++;

			 break;
		 }


	   } else
		{

		}
	  }


	  /*
		if (Orders[i]->GetStatus()==L"-") {
		   //
	   //	break;
	   cmp = CompareTime(time1, time2);



	  else if (j==TempOrders.size()-1)
	  {
			 TempOrders.insert(TempOrders.end(), Orders[i]);
			 break ;
	  }
	 }   */
	   it++;
	}

   if (st==1) {
	  TempOrders.insert(it, Order);
   }

   else if (st==0)
   {

   		 if (Order->GetStatus()==L"-") {

			it = TempOrders.begin();
			it += pos1;
			TempOrders.insert(it, Order);
			 pos1++;
			 pos2++;
			 pos3++;


		 }
		 else if (Order->GetStatus()==L"исполнена")
		 {
			it = TempOrders.begin();
			it += pos2;
			 TempOrders.insert(it, Order);
			 pos2++;
			 pos3++;


		 }
		 else if (Order->GetStatus()==L"отказ")
		 {
			it = TempOrders.begin();
			it += pos3;
			 TempOrders.insert(it, Order);
			 pos3++;

		 }
		 else
		 {
			it = TempOrders.begin();
			it += pos3;
			 TempOrders.insert(it, Order);
			 pos3++;

		 }

   }



 }
   Orders = TempOrders;
}

