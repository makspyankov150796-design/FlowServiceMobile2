//---------------------------------------------------------------------------

//#pragma hdrstop

#include "TSettingsClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)



 TXMLDocument *TSettingsClass::XmlDoc = nullptr;
_di_IXMLNode TSettingsClass::rootNode = nullptr;
_di_IXMLNode TSettingsClass::sampleNode = nullptr;
_di_IXMLNode TSettingsClass::sampleNode2 = nullptr;
_di_IXMLNode TSettingsClass::sampleNode3 = nullptr;



UnicodeString TSettingsClass::Performer = L" ";
UnicodeString TSettingsClass::OrgName = L" ";

UnicodeString TSettingsClass::SignCipher = L" ";;

UnicodeString TSettingsClass::eMail = L" ";

UnicodeString TSettingsClass::StopSoundFile = L" ";

UnicodeString TSettingsClass::DocNum = L" ";

UnicodeString TSettingsClass::Means = L" ";

UnicodeString TSettingsClass::Dir= L" ";
UnicodeString TSettingsClass::Units= L" ";

UnicodeString TSettingsClass::AllMeans= L" ";
UnicodeString TSettingsClass::Title= L" ";

UnicodeString TSettingsClass::TempData= L"";

UnicodeString TSettingsClass::Package= L"";

int TSettingsClass::IsCheckFill= 0;

int TSettingsClass::IsDocNumIndividual=0;
int TSettingsClass::IsAdressIndividual=0;

int TSettingsClass::status=0;
int TSettingsClass::IDOrg=0;
int TSettingsClass::IDPerformer=0;

int TSettingsClass::DataDestination =1;

int TSettingsClass::IsOrdersEnabled = 0;
int TSettingsClass::IsUsePeriod = 0;

int TSettingsClass::EtalonCHNum= 1;
int TSettingsClass::TestCHNum = 2;

UnicodeString TSettingsClass::ProtocolNumTitle=L" ";
int	TSettingsClass::ProtocolNum=0;

void TSettingsClass::SaveToFile(void)
{
	UnicodeString dirName;
	UnicodeString fname;

	bool success = true;
	int k=0, l = 0;
	XmlDoc = nullptr;
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

	rootNode = XmlDoc->AddChild(L"Settings");

	// CheckStoragePermission_(this);

	if (status != 5) {

		rootNode->SetAttribute(L"VER", XMLVER);

		rootNode->SetAttribute(L"DataDestination", DataDestination);
		rootNode->SetAttribute(L"Performer", Performer);
		rootNode->SetAttribute(L"IDPerformer", IDPerformer);
		rootNode->SetAttribute(L"OrgName", OrgName);
		rootNode->SetAttribute(L"IDOrg", IDOrg);

		rootNode->SetAttribute(L"Units", Units);
		rootNode->SetAttribute(L"StopSoundFile", StopSoundFile);
		rootNode->SetAttribute(L"IsOrdersEnabled", (IsOrdersEnabled));
		rootNode->SetAttribute(L"IsUsePeriod", (IsUsePeriod));
		rootNode->SetAttribute(L"eMail", eMail);
		rootNode->SetAttribute(L"SignCipher", SignCipher);
		rootNode->SetAttribute(L"DocNum", DocNum);
		rootNode->SetAttribute(L"Means", Means);

		rootNode->SetAttribute(L"AllMeans", AllMeans);
		rootNode->SetAttribute(L"Title", Title);

		rootNode->SetAttribute(L"IsDocNumIndividual", (IsDocNumIndividual));
		rootNode->SetAttribute(L"IsAdressIndividual", (IsAdressIndividual));
		rootNode->SetAttribute(L"IsCheckFill", (IsCheckFill));

		rootNode->SetAttribute(L"ProtocolNumTitle", (ProtocolNumTitle));
		rootNode->SetAttribute(L"ProtocolNum", (ProtocolNum));

		rootNode->SetAttribute(L"EtalonCHNum", (EtalonCHNum));
		rootNode->SetAttribute(L"TestCHNum", (TestCHNum));


	// XmlDoc->SaveToFile(dirName + L"/Report.xml");

	 if ((Dir==" ")||(!DirectoryExists(Dir))) {


		#ifdef __ANDROID__
		Dir =  System::Ioutils::TPath::GetTempPath();
		#endif

		#ifndef __ANDROID__
		Dir =  System::Ioutils::TPath::GetDocumentsPath();
		#endif

	 }

			rootNode->SetAttribute(L"Dir", Dir);


	fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "Settings" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";


	XmlDoc->SaveToFile(fname);

	// XmlDoc->L
	XmlDoc->Active = false;
   }
}


 void TSettingsClass::LoadFromFile(void) {

	bool success = true;

	XmlDoc = nullptr;
	rootNode = nullptr;
	sampleNode = nullptr;

	UnicodeString dirName;
	UnicodeString fname;

   //	TFlowMeter *FlowMeter;

	String str;

	int Size;
	int PointsSize;
	int index1 = 0;
	int index2 = 0;
	 int  size;

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

	 //Ищем пользовательские файлы


		fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "Settings" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";


	// Если пользовательского не существует.
	if (!FileExists(fname)) {
	 // Создаем хотя бы директорию
		if (!DirectoryExists(Dir)) {

		 if (!CreateDir(Dir))
		 {
		 Result = L"Cannot create USER directory.";
		 throw Exception(L"Cannot create USER directory.");
		 return;
		 }
		}
	/*
	  //Используем временные файлы
	#ifdef __ANDROID__
	Dir =  System::Ioutils::TPath::GetTempPath();
	#endif

	#ifndef __ANDROID__
	Dir =  System::Ioutils::TPath::GetDocumentsPath();
	#endif

	if (!DirectoryExists(Dir)) {
		Result = L"Папки не существует";
		throw Exception(L"Папки не существует");

		 if (!CreateDir(Dir))
		 {
		 Result = L"Cannot create directory.";
		 throw Exception(L"Cannot create directory.");
		 return;
		 }

		}

		fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "Settings" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";

	 */

	if (!FileExists(fname)) {

		Result = L"Файл настроек не существует";
	   //	throw Exception(L"Файл типов не существует");
		ShowMessage(Format(L"Файл '%s' не существует. Создан новый файл.",
	 ARRAYOFCONST((fname))));
		 SaveToFile();

	}


	}

	FileName = ExtractFileName(fname);
	document = LoadXMLDocument(fname);

	if (document == NULL) {

		Result = L"Файл пустой или поврежден";
		throw Exception(Result);

	}


	rootNode = document->ChildNodes->FindNode(L"Settings");

	if (rootNode == NULL) {
		Result = L"Файл пустой или поврежден";
		throw Exception(Result);
	}


	 VER = rootNode->Attributes[L"VER"];


//	if (VER == "4.0") {
		 DataDestination = GetIntAttribute(L"DataDestination",rootNode, 1);
		Performer = rootNode->Attributes[L"Performer"];
		IDPerformer = rootNode->Attributes[L"IDPerformer"];
		OrgName = rootNode->Attributes[L"OrgName"];
		IDOrg = rootNode->Attributes[L"IDOrg"];
		IsOrdersEnabled = (rootNode->Attributes[L"IsOrdersEnabled"]);
		IsUsePeriod = (rootNode->Attributes[L"IsUsePeriod"]);
		StopSoundFile  = rootNode->Attributes[L"StopSoundFile"];
		eMail  = rootNode->Attributes[L"eMail"];
		SignCipher  = rootNode->Attributes[L"SignCipher"];
		DocNum  = rootNode->Attributes[L"DocNum"];
		Means  = rootNode->Attributes[L"Means"];
		//Dir  = rootNode->Attributes[L"Dir"];
		Units  = rootNode->Attributes[L"Units"];

		AllMeans  = GetAttribute(L"AllMeans",rootNode);
		Title  = GetAttribute(L"Title",rootNode);
		IsDocNumIndividual  = StrToInt (GetIntAttribute(L"IsDocNumIndividual",rootNode));
		IsAdressIndividual  = StrToInt (GetIntAttribute(L"IsAdressIndividual",rootNode));
		IsCheckFill = 		StrToInt (GetIntAttribute(L"IsCheckFill",rootNode));


		ProtocolNumTitle = GetAttribute(L"ProtocolNumTitle",rootNode);
		ProtocolNum = StrToInt (GetIntAttribute(L"ProtocolNum",rootNode));


		EtalonCHNum  = GetIntAttribute(L"EtalonCHNum",rootNode);
		TestCHNum  = GetIntAttribute(L"TestCHNum",rootNode);

  //	}



//	else
	{

  //				Result = L"Версия не может быть обработана";
	//			throw Exception(Result);
	}





//	return ActiveFlowMeter;
}

