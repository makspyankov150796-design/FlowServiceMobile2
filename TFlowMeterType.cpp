// ---------------------------------------------------------------------------

#pragma hdrstop

#include "TFlowMeterType.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

TXMLDocument *TFlowMeterType::XmlDoc = nullptr;
_di_IXMLNode TFlowMeterType::rootNode = nullptr;
_di_IXMLNode TFlowMeterType::sampleNode = nullptr;
_di_IXMLNode TFlowMeterType::sampleNode2 = nullptr;
_di_IXMLNode TFlowMeterType::sampleNode3 = nullptr;
UnicodeString TFlowMeterType::dirName = "";
System::UnicodeString TFlowMeterType::fname = "";

vector<TFlowMeterType*>TFlowMeterType::MeterTypes;
std::vector<tPoint>::iterator TFlowMeterType::it;

vector <int> TFlowMeterType::FilterList;

bool TFlowMeterType::filter = false;

__fastcall TFlowMeterType::~TFlowMeterType() {

}

TFlowMeterType::TFlowMeterType(){
	 Hash = this->GetHashCode();
	 Visible=true;
}

void TFlowMeterType::AddCurrPoint(tPoint Pnt) {
	Point = Pnt;
	Points.push_back(Point);
}

void TFlowMeterType::AddPoint(tPoint Pnt) {
	tPoint temp, temp1;
	float Q1, Q2;

	std::vector<tPoint>::iterator it;

	it = Points.begin();

	if (!Points.empty()) {
		temp = Pnt;

		for (int i = 0; i < Points.size(); i++) {
			// Вычисляем расход для данной калибровочной точки
			temp1 = Points[i];
			if (Points[i].Time > 0) {
				Q1 = Points[i].Q;
			}
			else {
				return;
			}

			if (Pnt.Time > 0) {
				Q2 = Pnt.Q;
			}
			else {
				return;
			}

			if (Q2 <= Q1) {
				Points.insert(it + i, Pnt);
				return;
			}
		}

		Points.push_back(Pnt);
		return;

	}
	else {
		Points.push_back(Pnt);
		return;
	}
}

void TFlowMeterType::ClearPoints(void) {
	Points.clear();
}

uint8_t TFlowMeterType::AddPointData(float Qrate, float Q, float Volume,
	float vTime, float Error, float RagePlus, float RageMinus) {
	tPoint Pnt;
	uint8_t error;

	/*
	 if (vTime == 0) { return 2; }
	 if (vTime < 1) { return 3; }
	 if (vTime > 100000) { return 4; }

	 #ifndef __ANDROID__
	 if (!_finite(vTime)) { return 5; }

	 #endif
	 */
	if ((!IsNan(Qrate)) && (!IsInfinite(Qrate))) {

		if ((Qrate > 0) && (Qmax > 0)) {
			Pnt.Q = Qmax * Qrate;
			Pnt.Qrate = Qrate;
		}
		else if (Q > 0) {
			Pnt.Q = Q;
			if (Qmax > 0) {
				Pnt.Qrate = Pnt.Q / Qmax;
			}

		}
		else {
			return 6;
		}

	}
	else {
		return 7;
	}

	if ((!IsNan(Volume)) && (!IsInfinite(vTime))) {

		if (Volume > 0) {
			Pnt.Volume = Volume;

		}

		if (vTime > 1) {
			Pnt.Time = vTime;
		}

	}
	else {
		return 8;
	}

	if (!IsNan(Error)) {
		Pnt.Error = Error;
	}

   		 if (!IsNan(RageMinus))
	{
	  Pnt.RageMinus = RageMinus;
	}

		 if (!IsNan(RagePlus))
	{
	  Pnt.RagePlus = RagePlus;
	}

	/* Pnt.Volume = 	Pnt.Imp / Pnt.Coef;
	 Pnt.Q = 		(Pnt.EtlVolume/Pnt.Time)*3600;
	 Pnt.Qtest  =  	(Pnt.Volume/Pnt.Time)*3600;

	 Pnt.Error =   	((Pnt.Volume - Pnt.EtlVolume)*100)/Pnt.EtlVolume;
	 Pnt.Rate  =   	Pnt.Volume/Pnt.EtlVolume; */

	AddCurrPoint(Pnt);

	return 1;

}

void TFlowMeterType::SaveTypesToFile(void) {

	bool success = true;

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

	rootNode = XmlDoc->AddChild(L"DeviceTypes");

	// CheckStoragePermission_(this);

	if (!MeterTypes.empty()) {

		rootNode->SetAttribute(L"VER", XMLVER);
		rootNode->SetAttribute(L"TypeCount", IntToStr((int) MeterTypes.size()));

		for (int j = 0; j < MeterTypes.size(); j++) {
			sampleNode = rootNode->AddChild(L"Type" + IntToStr(j));

			/*
			 Int Hash;
			 //
			 // Тип поверяемого устройства
			 UnicodeString DeviceType;
			 //
			 // Серийный номер поверяемого устройства
			 UnicodeString SerialNum;
			 //
			 // Номер ГРСИ поверяемого устройства
			 UnicodeString CertificateNum;

			 // Коэффициент преобразования
			 float Kp;

			 float Qmax;

			 tPoint 					Point;
			 vector <tPoint> 		Points;
			 */

			sampleNode->SetAttribute(L"Hash",
            	MeterTypes[j]->Hash);
			sampleNode->SetAttribute(L"DeviceType",
				MeterTypes[j]->DeviceType);

			sampleNode->SetAttribute(L"DeviceName",
				MeterTypes[j]->DeviceName);

			sampleNode->SetAttribute(L"Serial",
				MeterTypes[j]->SerialNum);
			sampleNode->SetAttribute(L"CertificateNum",
				MeterTypes[j]->CertificateNum);
            sampleNode->SetAttribute(L"Modification",
				MeterTypes[j]->Modification);
            sampleNode->SetAttribute(L"VerificationNum",
				MeterTypes[j]->VerificationNum);
            sampleNode->SetAttribute(L"Description",
				MeterTypes[j]->Description);
			sampleNode->SetAttribute(L"VerificationInterval1",
				MeterTypes[j]->VerificationInterval1);
			sampleNode->SetAttribute(L"VerificationInterval2",
				MeterTypes[j]->VerificationInterval2);


			sampleNode->SetAttribute(L"Date1",
				MeterTypes[j]->Date1);
			sampleNode->SetAttribute(L"Date2",
				MeterTypes[j]->Date2);

			sampleNode->SetAttribute(L"Manufacturer",
				MeterTypes[j]->Manufacturer);
			sampleNode->SetAttribute(L"Data1",
				MeterTypes[j]->Data1);
			sampleNode->SetAttribute(L"Data2",
				MeterTypes[j]->Data2);
			sampleNode->SetAttribute(L"Data3",
				MeterTypes[j]->Data3);


			sampleNode->SetAttribute(L"CheckType", IntToStr(MeterTypes[j]->CheckType));

			sampleNode->SetAttribute(L"Kp", FloatToStr(MeterTypes[j]->Kp));
			sampleNode->SetAttribute(L"Qmax", FloatToStr(MeterTypes[j]->Qmax));

			sampleNode2 = sampleNode->AddChild(L"Points");

			sampleNode2->SetAttribute(L"PointsCount",
				IntToStr((int) MeterTypes[j]->Points.size()));

			if (!MeterTypes[j]->Points.empty()) {
				for (int i = 0; i < MeterTypes[j]->Points.size(); i++) {
					sampleNode2 = sampleNode->AddChild(L"Point" + IntToStr(i));

					sampleNode2->SetAttribute(L"Name",
						MeterTypes[j]->Points[i].Name);

					sampleNode2->SetAttribute(L"Qrate",
						FloatToStrF(MeterTypes[j]->Points[i].Qrate, ffNumber,
						10, 3));
					sampleNode2->SetAttribute(L"Q",
						FloatToStrF(MeterTypes[j]->Points[i].Q, ffNumber,
						10, 3));
					sampleNode2->SetAttribute(L"Volume",
						FloatToStrF(MeterTypes[j]->Points[i].Volume,
						ffNumber, 10, 2));
					sampleNode2->SetAttribute(L"Time",
						FloatToStrF(MeterTypes[j]->Points[i].Time, ffNumber,
						10, 0));
					sampleNode2->SetAttribute(L"Error",
						FloatToStrF(MeterTypes[j]->Points[i].Error, ffNumber,
						10, 2));
					sampleNode2->SetAttribute(L"RagePlus",
						FloatToStrF(MeterTypes[j]->Points[i].RagePlus, ffNumber,
						3, 1));
					sampleNode2->SetAttribute(L"RageMinus",
						FloatToStrF(MeterTypes[j]->Points[i].RageMinus, ffNumber,
						3, 1));
					sampleNode2->SetAttribute(L"IsRageFree",
						IntToStr(MeterTypes[j]->Points[i].IsRageFree));

					sampleNode2->SetAttribute(L"Accuracy",
						FloatToStrF(MeterTypes[j]->Points[i].Accuracy, ffNumber,
						3, 1));
				}

			}

		}
	}  else
    {
		rootNode->SetAttribute(L"VER", XMLVER);
		rootNode->SetAttribute(L"TypeCount", "0");
    }

	// XmlDoc->SaveToFile(dirName + L"/Report.xml");


	fname = TSettingsClass::Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "MeterTypes" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";


	XmlDoc->SaveToFile(fname);

	// XmlDoc->L
	XmlDoc->Active = false;

}

void TFlowMeterType::LoadTypesFromFile(void) {

	bool success = true;

	XmlDoc = nullptr;
	rootNode = nullptr;
	sampleNode = nullptr;
	UnicodeString  Dir, fname;
	UnicodeString str;
	TFlowMeterType *MeterType;

	int MetersTypeSize;
	int MetersTypePointsSize;
	int index1 = 0;
	int index2 = 0;

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


    #ifdef __ANDROID__
		Dir =  System::Ioutils::TPath::GetPublicPath();
	#endif

	#ifndef __ANDROID__
		Dir =  System::Ioutils::TPath::GetDocumentsPath();
	#endif

	fname = Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "MeterTypes" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";


	if (!DirectoryExists(TSettingsClass::Dir)) {
		Result = L"Файл типов не существует";
		throw Exception(L"Файл типов не существует");
	}

	if (!FileExists(fname)) {

		Result = L"Файл типов не существует";
	   //	throw Exception(L"Файл типов не существует");
        ShowMessage(Format(L"Файл '%s' не существует. Создан новый файл.",
	 ARRAYOFCONST((fname))));
         SaveTypesToFile();

	}

	document = LoadXMLDocument(fname);

	if (document == NULL) {

		Result = L"Файл пустой или поврежден";
		throw Exception(Result);

	}

	TFlowMeterType::MeterTypes.clear();

	rootNode = document->ChildNodes->FindNode(L"DeviceTypes");

	if (rootNode == NULL) {
		Result = L"Файл пустой или поврежден";
		throw Exception(Result);
	}


	// САМАЯ РАНЯЯ ВЕРСИЯ !


     VER = rootNode->Attributes[L"VER"];


	if (VER == "4.0") {

		MetersTypeSize = rootNode->Attributes[L"TypeCount"];

		for (int j = 0; j < MetersTypeSize; j++) {

			sampleNode = rootNode->ChildNodes->FindNode(L"Type" + IntToStr(j));

			if (sampleNode != NULL) {

				MeterType = new TFlowMeterType();

				MeterType->Hash = sampleNode->Attributes[L"Hash"];
				MeterType->DeviceType = sampleNode->Attributes[L"DeviceType"];

				MeterType->DeviceName = sampleNode->Attributes[L"DeviceName"];

				MeterType->SerialNum = sampleNode->Attributes[L"Serial"];
				MeterType->CertificateNum =
					sampleNode->Attributes[L"CertificateNum"];
				MeterType->Modification =
					sampleNode->Attributes[L"Modification"];
				MeterType->VerificationNum =
					sampleNode->Attributes[L"VerificationNum"];
				MeterType->Description =
					sampleNode->Attributes[L"Description"];
				MeterType->VerificationInterval1 =
					sampleNode->Attributes[L"VerificationInterval1"];
				MeterType->VerificationInterval2 =
					sampleNode->Attributes[L"VerificationInterval2"];
				MeterType->CheckType = StrToInt( sampleNode->Attributes[L"CheckType"]);
				MeterType->Kp = sampleNode->Attributes[L"Kp"];
				MeterType->Qmax =  StrToFloat(GetAttribute(L"Qmax",sampleNode));

				MeterType->Manufacturer =
					sampleNode->Attributes[L"Manufacturer"];
				MeterType->Date1 =
					sampleNode->Attributes[L"Date1"];
				MeterType->Date2 =
					sampleNode->Attributes[L"Date2"];

				MeterType->Data1 =
					sampleNode->Attributes[L"Data1"];
				MeterType->Data2 =
					sampleNode->Attributes[L"Data2"];
				MeterType->Data3 =
					sampleNode->Attributes[L"Data3"];

			}
            else
            {
				Result = L"Файл пустой или поврежден";
				throw Exception(Result);
            }

			sampleNode2 = sampleNode->ChildNodes->FindNode(L"Points");

			MetersTypePointsSize = sampleNode2->Attributes[L"PointsCount"];

			if (MetersTypePointsSize > 0) {
				MeterType->Points.clear();

				for (int i = 0; i < MetersTypePointsSize; i++) {

					sampleNode2 = sampleNode->ChildNodes->FindNode
						(L"Point" + IntToStr(i));

					if (sampleNode != NULL) {
						/*MeterType->Point.Name =
							sampleNode2->Attributes[L"Name"];*/
						try{

						MeterType->Point.Name =
							sampleNode2->Attributes[L"Name"];

						MeterType->Point.Qrate =
							StrToFloat_(sampleNode2->Attributes[L"Qrate"]);
						MeterType->Point.Q =
							StrToFloat_(sampleNode2->Attributes[L"Q"]);
						MeterType->Point.Volume =
							StrToFloat_(sampleNode2->Attributes[L"Volume"]);
						MeterType->Point.Time =
							StrToFloat_(sampleNode2->Attributes[L"Time"]);
						MeterType->Point.Error =
							StrToFloat_(sampleNode2->Attributes[L"Error"]);



						str =  GetAttribute(L"Accuracy",sampleNode2);

						MeterType->Point.Accuracy =
							StrToFloat_(GetAttribute(L"Accuracy",sampleNode2));



						MeterType->Point.Accuracy =
							StrToFloat_(GetAttribute(L"Accuracy",sampleNode2,L"10"));
						MeterType->Point.RagePlus =
							StrToFloat_(GetAttribute(L"RagePlus",sampleNode2,L"10"));

						MeterType->Point.RageMinus =
							StrToFloat_(GetAttribute(L"RageMinus",sampleNode2,L"10"));

						if (MeterType->Point.Name == L"Qmax") {

						//FlowMeter->Point.RageMinus =
						 //	StrToFloat_(GetAttribute(L"RageMinus",sampleNode2,L"90"));
						MeterType->Point.RageMinus = 90;

					   }   else
					   {
						MeterType->Point.RageMinus =
							StrToFloat_(GetAttribute(L"RageMinus",sampleNode2,L"10"));
					   }

						MeterType->Point.IsRageFree =
							StrToInt(GetAttribute(L"IsRageFree",sampleNode2,L"0"));

						MeterType->Points.push_back(MeterType->Point);
						} catch (...)
						{

						}
					}
				}

			}

            TFlowMeterType::MeterTypes.push_back(MeterType);
		}


	}

  	else if (VER == XMLVER) {

		MetersTypeSize = rootNode->Attributes[L"TypeCount"];

		for (int j = 0; j < MetersTypeSize; j++) {

			sampleNode = rootNode->ChildNodes->FindNode(L"Type" + IntToStr(j));

			if (sampleNode != NULL) {

				MeterType = new TFlowMeterType();

				MeterType->Hash = sampleNode->Attributes[L"Hash"];
				MeterType->DeviceType = sampleNode->Attributes[L"DeviceType"];

				MeterType->DeviceName = sampleNode->Attributes[L"DeviceName"];

				MeterType->SerialNum = sampleNode->Attributes[L"Serial"];
				MeterType->CertificateNum =
					sampleNode->Attributes[L"CertificateNum"];
				MeterType->Modification =
					sampleNode->Attributes[L"Modification"];
				MeterType->VerificationNum =
					sampleNode->Attributes[L"VerificationNum"];
				MeterType->Description =
					sampleNode->Attributes[L"Description"];
				MeterType->VerificationInterval1 =
					sampleNode->Attributes[L"VerificationInterval1"];
								MeterType->VerificationInterval2 =
					sampleNode->Attributes[L"VerificationInterval2"];
                MeterType->CheckType = StrToInt( sampleNode->Attributes[L"CheckType"]);
				MeterType->Kp = sampleNode->Attributes[L"Kp"];
				MeterType->Qmax = sampleNode->Attributes[L"Qmax"];

				MeterType->Manufacturer =
					sampleNode->Attributes[L"Manufacturer"];
				MeterType->Date1 =
					sampleNode->Attributes[L"Date1"];
				MeterType->Date2 =
					sampleNode->Attributes[L"Date2"];

				MeterType->Data1 =
					sampleNode->Attributes[L"Data1"];
				MeterType->Data2 =
					sampleNode->Attributes[L"Data2"];
				MeterType->Data3 =
					sampleNode->Attributes[L"Data3"];

			}
            else
            {
				Result = L"Файл пустой или поврежден";
				throw Exception(Result);
            }

			sampleNode2 = sampleNode->ChildNodes->FindNode(L"Points");

			MetersTypePointsSize = sampleNode2->Attributes[L"PointsCount"];

			if (MetersTypePointsSize > 0) {
				MeterType->Points.clear();

				for (int i = 0; i < MetersTypePointsSize; i++) {

					sampleNode2 = sampleNode->ChildNodes->FindNode
						(L"Point" + IntToStr(i));

					if (sampleNode != NULL) {
						MeterType->Point.Name =
							sampleNode2->Attributes[L"Name"];
						MeterType->Point.Qrate =
							StrToFloat_(sampleNode2->Attributes[L"Qrate"]);
						MeterType->Point.Q =
							StrToFloat_(sampleNode2->Attributes[L"Q"]);
						MeterType->Point.Volume =
							StrToFloat_(sampleNode2->Attributes[L"Volume"]);
						MeterType->Point.Time =
							StrToFloat_(sampleNode2->Attributes[L"Time"]);
						MeterType->Point.Error =
							StrToFloat_(sampleNode2->Attributes[L"Error"]);

						MeterType->Points.push_back(MeterType->Point);
					}
				}

			}

            TFlowMeterType::MeterTypes.push_back(MeterType);
		}


	}

    else
    {

				Result = L"Версия не может быть обработана";
				throw Exception(Result);

    }


}

void TFlowMeterType::AddTypeToList() {
MeterTypes.push_back(this);
}

void TFlowMeterType::AddPoints(vector<tPoint>points) {
Points.clear();

if (!points.empty()) {
	for (int i = 0; i < points.size(); i++) {
		Points.push_back(points[i]);
	}
}

}

void TFlowMeterType::DuplicateType(TFlowMeterType* MeterType)
{

       TFlowMeterType	*MeterTypes = new TFlowMeterType();

				MeterTypes->DeviceType = MeterType->DeviceType;
				MeterTypes->DeviceName = MeterType->DeviceName;
				MeterTypes->SerialNum = MeterType->SerialNum;
                MeterTypes->CertificateNum = MeterType->CertificateNum;
                MeterTypes->Modification = MeterType->Modification;
                MeterTypes->VerificationNum = MeterType->VerificationNum;
				MeterTypes->Description = MeterType->Description;
				MeterTypes->VerificationInterval1 = MeterType->VerificationInterval1;
				MeterTypes->VerificationInterval2 = MeterType->VerificationInterval2;
				MeterTypes->CheckType = MeterType->CheckType;
				MeterTypes->Kp = MeterType->Kp;
				MeterTypes->Qmax = MeterType->Qmax;

				MeterTypes->Manufacturer =
					MeterType->Manufacturer;
				MeterTypes->Date1 =
					MeterType->Date1;
				MeterTypes->Date2 =
					MeterType->Date2;
				MeterTypes->Data1 =
					MeterType->Data1;
				MeterTypes->Data2 =
					MeterType->Data2;
				MeterTypes ->Data3 =
					MeterType->Data3;



			if (!MeterType->Points.empty()) {
				for (int i = 0; i < MeterType->Points.size(); i++)
                {
                tPoint Point;

                Point.Name 		= MeterType->Points[i].Name;

				Point.Qrate 	= MeterType->Points[i].Qrate;
                Point.Q 		= MeterType->Points[i].Q;
                Point.Volume 	= MeterType->Points[i].Volume;
                Point.Time 		= MeterType->Points[i].Time;
                Point.Error 	= MeterType->Points[i].Error;

                MeterTypes->Points.push_back(Point);
				}

			}


             MeterTypes->AddTypeToList();

		}

TFlowMeterType* TFlowMeterType::Get (int hash)
{
		for (int j = 0; j < MeterTypes.size(); j++)
	{
	  if (MeterTypes[j]->Hash == hash) {
			return  MeterTypes[j];
	  }
	}

	return nullptr;
}

TFlowMeterType* TFlowMeterType::Get(UnicodeString hash)
{
	 int h;

	  if (TryStrToInt(hash,h)) {

		for (int j = 0; j < MeterTypes.size(); j++)
	{
	  if (MeterTypes[j]->Hash == h) {
			return  MeterTypes[j];
	  }
	}
	}
	return nullptr;
}


void TFlowMeterType::Erase (int Hash)
{


		for (int j = 0; j < MeterTypes.size(); j++)
	{
	  if (MeterTypes[j]->Hash == Hash) {

		  TFlowMeterType::MeterTypes.erase(TFlowMeterType::MeterTypes.begin()+j);
			return  ;
	  }
	}

	return ;
}

void TFlowMeterType::Erase (UnicodeString hash)
{
	  int h;
	  if (TryStrToInt(hash,h)) {
		for (int j = 0; j < MeterTypes.size(); j++)
	{
	  if (MeterTypes[j]->Hash == hash) {

		  TFlowMeterType::MeterTypes.erase(TFlowMeterType::MeterTypes.begin()+j);
			return  ;
	  }
	}
	   }
	return ;
}
