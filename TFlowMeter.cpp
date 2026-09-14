//---------------------------------------------------------------------------

#pragma hdrstop

#include "TFlowMeter.h"



 TXMLDocument *TFlowMeter::XmlDoc = nullptr;
_di_IXMLNode TFlowMeter::rootNode = nullptr;
_di_IXMLNode TFlowMeter::sampleNode = nullptr;
_di_IXMLNode TFlowMeter::sampleNode2 = nullptr;
_di_IXMLNode TFlowMeter::sampleNode3 = nullptr;
UnicodeString TFlowMeter::dirName = "";
UnicodeString TFlowMeter::fname = "";


 //UnicodeString TFlowMeter::email = " ";
 UnicodeString TFlowMeter::sign_cipher  = " ";
 UnicodeString TFlowMeter::porveritel_fio = " ";


vector<TFlowMeter*>TFlowMeter::FlowMeters;
std::vector<tPoint>::iterator TFlowMeter::it;
std::vector<TFlowMeter*>::iterator TFlowMeter::fit;

THscDevice *TFlowMeter::sHSC= nullptr;
TFlowMeter *TFlowMeter::ActiveFlowMeter= nullptr;


TJSONArray *TFlowMeter::jArray = new TJSONArray();
TJSONObject *TFlowMeter::jObject = new TJSONObject();

//---------------------------------------------------------------------------



TFlowMeter::TFlowMeter(THscDevice *HSCDev, int id_Order, bool IsEtalon)
{
	HSCDevice = HSCDev;
   HSC = HSCDevice;

   Init(IsEtalon);

	ID_Order = id_Order;

}

TFlowMeter::TFlowMeter(THscDevice *HSCDev, bool IsEtalon)
{
   HSCDevice = HSCDev;
   HSC = HSCDevice;
  // HSC->SetOnConfigRead(OnConfigRead);

   Init(IsEtalon);

		 if (TOrderClass::ActiveOrder!=nullptr) {
		ID_Order = TOrderClass::ActiveOrder->ID_Order;
	 } else
	  {
	   ID_Order = 800;
	  }

}

TFlowMeter::TFlowMeter(bool IsEtalon)
{
     HSCDevice = sHSC;
	 HSC = HSCDevice;
   Init(IsEtalon);

     		 if (TOrderClass::ActiveOrder!=nullptr) {
		ID_Order = TOrderClass::ActiveOrder->ID_Order;
	 } else
	  {
	   ID_Order = 800;
	  }
}




void TFlowMeter::Init(bool IsEtalon)
{

	TDateTime Date;

	TDateTime dt;
     /*Point;
     tDataPoint 			DataPoint;
     tCalibrPoint           CalibrPoint;
     tCoef                  Coef; */

	tPoint Pnt;

	Name= IntToStr(this->GetHashCode());
	 Kp=100;
	 Qmax = 3000;
	 Points.clear();

	Pnt.Qrate= 0.02;		   //Часть от Qmax
	Pnt.Q = 0.06;


	Pnt.Name = L"Qнаим";
	Pnt.Q = 60;
	Pnt.Num = 1;                 //Кол-во измерений
	Pnt.State=0;         //<Состоние
	Pnt.Time=720;          //Время измерения
	Pnt.Volume = 0;
	Pnt.Imp = 0;
	Pnt.Accuracy = 10;   //Точность выхода на расход
	Pnt.Error= 5;      //Погрешность допустимая измерениы
	Pnt.RagePlus = 10;
	Pnt.RageMinus = 10;
	Pnt.IsRageFree = 0;


	 AddPoint(Pnt);


	Pnt.Qrate= 0.044;            //Часть от Qmax
	Pnt.Q = 132;
	Pnt.Name = L"1.1 Qп";
	Pnt.Num = 1;                 //Кол-во измерений
	Pnt.State=0;         //<Состоние
	Pnt.Time=360;          //Время измерения
	Pnt.Volume = 0;
	Pnt.Imp = 0;
	Pnt.Accuracy = 10;   //Точность выхода на расход
	Pnt.Error= 2;      //Погрешность допустимая измерениы
	Pnt.RagePlus = 10;
	Pnt.RageMinus = 10;
	Pnt.IsRageFree = 0;

	 AddPoint(Pnt);


	Pnt.Qrate= 0.3;            //Часть от Qmax
	Pnt.Q = 1500;
	Pnt.Name = L"Qнаиб";
	Pnt.Num = 1;                 //Кол-во измерений
	Pnt.State=0;         //<Состоние
	Pnt.Time=120;          //Время измерения
	Pnt.Volume = 0;
	Pnt.Imp = 0;
	Pnt.Accuracy = 10;   //Точность выхода на расход
	Pnt.Error= 2;      //Погрешность допустимая измерениы
	Pnt.RagePlus = 10;
	Pnt.RageMinus = 90;
	Pnt.IsRageFree = 1;

	 AddPoint(Pnt);



	 DataPoints.clear();
	 CalibrPoints.clear();
	 Coefs.clear();
	// IsEtalon = false;
	 SetChannel(CHANNEL);
	 Hash = this->GetHashCode();






		FlowValue = new TMeterValue;
		FlowValue->SetAsFlowRate();
		FlowValue->SetFilter(8);
		FlowValue->MaxValue = Qmax;
		FlowValue->MinValue = Qmin;
		FlowValue->Accuracy = 4;
		FlowValue->Name = L"Расход";


		VolumeValue = new TMeterValue;
		VolumeValue->SetAsVolume();
		VolumeValue->SetFilter(-1);
		VolumeValue->Accuracy = 5;
		VolumeValue->Name = L"Объем";



		ImpValue = new TMeterValue;
		ImpValue->SetAsImp();
		ImpValue->SetFilter(-1);
		ImpValue->Accuracy = 4;
		ImpValue->Name = L"Импульсы";

		VolumeErrorValue = new TMeterValue;
		VolumeErrorValue->SetAsError();
		VolumeErrorValue->SetFilter(-1);
		VolumeErrorValue->Accuracy = 2;
		VolumeErrorValue->Name = L"Погрешность";

	  AddToList();

		  if (!IsEtalon)
   {


		FlowValue->Accuracy = 4;
		FlowValue->Error = 5;
   }    else
	{
		FlowValue->Accuracy = 4;
		FlowValue->Error = 2;
    }




		miOwner = L"Физ. лицо";
	//
	// Серийный номер поверяемого устройства
		SerialNum="";
	//
	// Номер ГРСИ поверяемого устройства
		CertificateNum="";

		DN=L"15";
		docTitle="1592-2015";

		means=L"60661.15.3Р.00540801";

		year_production=L"";

		K1=""; P1=""; K2= ""; P2="";

		 tempWater = L"18,75";
		 temperature = L"22,4 град. С";
		 pressure = L"99,2 кПа";
		 hymidity = L"51,30%";



		vrfDate =   DateToISO8601(Today(),false);//DateToStr();//ToDayFGISFormat();

		vrfDate = vrfDate.SubString(0,9)+ vrfDate.SubString(23,29);

		Date =  IncYear(Today(), 6);

		validDate = DateToISO8601(Date,false);  //IncYear(vrfDate, 6);// IncDateFGISFormat(6);

		validDate = validDate.SubString(0,9)+ validDate.SubString(23,29);

		Result = L"Годен";

		VerificationInterval = 6;
}

void  __fastcall TFlowMeter::OnConfigRead(TObject * Sender)
{
    //Kp =

   for(int i=0; i<20;i++)
   {
   AddCalibrData(HSCDevice->cEtlVolume[i], HSCDevice->cTime[i], HSCDevice->cImp[i], HSCDevice->cCoef[i]);
   }
}

void TFlowMeter::ReadPIN(void)
    {


    }

void TFlowMeter::SavePIN(uint8_t pin)
    {
	   HSCDevice->Save_PIN(pin);

    }

void  TFlowMeter::Write_Channel_State(uint16_t State)
{
	HSCDevice->Write_Channel_State(Channel, State);
}

void TFlowMeter::WritePIN(uint8_t pin)
    {
       HSCDevice->Write_PIN(pin);

    }

 __fastcall TFlowMeter::~TFlowMeter()
{


}

float TFlowMeter::Flow(void)
{
	float flw, K, P;


   	flw = HSCDevice->GetAvrImpulses(GetChannel())/Kp;

  // 	K = koef_K(flw);
  //  P = koef_K(flw);

  //  Q = flw*K + P;

	return flw;

}
	UnicodeString TFlowMeter::GetSendStatus(){

	   UnicodeString text= L"-";

	   if (SendStatus==0) {
			text = L"-";

	   } else if (SendStatus==1) {
			text = L"Отправляется";
			  }
		else {
			text = L"Отправлен";
			}


	 return text;
	}


   void	TFlowMeter::SetSendStatus(UnicodeString text){

	   if (text== L"-")
		{
		  SendStatus=0;
		}
		 else if (text == L"Отправляется")
		{SendStatus=1;}

		 else if (text == L"Отправлен")
		{SendStatus=2;}

		else {
			SendStatus=0;
			}


	}


	UnicodeString TFlowMeter::GetStatus(){
	UnicodeString text= L"-";

	   if (Status==0) {

	   text= L"-";
	   } else if (Status==1) {
	   text= L"Не Годен";
			  }
		else if (Status==2) {
		text= L"Годен";
			  }
		else {}
	  return text;
	}

uint8_t TFlowMeter::GetChannel(void)
{

    return Channel;

}

void TFlowMeter::SetChannel(uint8_t CH)
{
    //if (CH < HSCDevice->GetMaxChannel())
    {
		Channel = CH;
    }
}

void TFlowMeter::ResetTest(void)
{
   VolSum = 0;
   ImpSum = 0;

 //  FlowValue -> Reset();
   FlowValue -> ARRAY_SIZE = 100;
   FlowValue -> SetFilter(8);

   VolumeValue -> Reset();
   ImpValue -> Reset();
}

void  TFlowMeter::IncImpSumMonitor(void)
{

	 ImpSum =  ImpSum + (HSCDevice-> GetSecImp(Channel));



}


void  TFlowMeter::SetMonitorValues(void)
{

	 FlowValue -> SetValue((float)GetFlow());
	 VolumeValue -> SetValue((float)GetFlowVolume());
	 ImpValue -> SetValue((float)GetTotalImp());


}

void  TFlowMeter::SetValues(void)
{
	 FlowValue -> SetValue((float)GetFlow());
	 VolumeValue -> SetValue((float)GetVolume());
	 ImpValue -> SetValue((float)GetTotalImp());
	 VolumeErrorValue -> SetValue((float)GetVolumeError());

}

void  TFlowMeter::SetFinalValues(void)
{
	 FlowValue -> SetFilter(-1);
	 FlowValue -> SetValue((float)GetFinalFlow());
	 VolumeValue -> SetValue((float)GetFinalVolume());
	 ImpValue -> SetValue((float)GetFinalImp());

	 VolumeErrorValue -> SetValue((float)GetVolumeError());
}

float TFlowMeter::GetFlowVolume(void)
{

	float Vol, flow, CalibrVol;
	float calibr_flow, rate;



	Vol =   ImpSum/Kp;

	flow =  GetRawFlow();
	rate = Rate(flow);

	CalibrVol =  Rate(flow) * Vol;


	return CalibrVol;
}

float TFlowMeter::GetWidthFlowVolume(void)
{

	float Vol, flow, CalibrVol;
	float calibr_flow, rate;


	CalibrVol = 0;//GetWidthFlow() * time;


	return CalibrVol;
}

float TFlowMeter::GetTotalImp(void)
{

    return HSCDevice->GetTotalImp(Channel);
}

float TFlowMeter::GetVolume(void)
{   float Vol, flow, CalibrVol;

    Vol =  HSCDevice->GetTotalImp(Channel)/Kp;
    flow =  GetRawFlow();
    CalibrVol = Rate(flow) * Vol;

    return CalibrVol;
}

float TFlowMeter::GetFinalRawVolume(void)
{   float Vol;

    Vol =  HSCDevice->GetFinalImp(Channel)/Kp;

    return Vol;
}

float TFlowMeter::GetFinalVolume(void)
{
    float Vol, flow, CalibrVol;

    Vol =  HSCDevice->GetFinalImp(Channel)/Kp;

    flow =  GetFinalRawFlow();
    CalibrVol = Rate(flow) * Vol;

    return CalibrVol;
}

float TFlowMeter::GetRawFlow(void)
{
	float flow, calibr_flow, rate;

	flow =  (HSCDevice-> GetSecImp(Channel)*3600)/Kp;

	return flow;
}

int TFlowMeter::GetRawSecImp(void)
{
	return HSCDevice-> GetSecImp(Channel);
}


float TFlowMeter::GetRawWidthFlow(void)
{
	float flow, calibr_flow, rate;

	flow =  3.6/(Kp*HSCDevice-> GetWidth(Channel));

	return flow;
}

float TFlowMeter::GetWidthFlow(void)
{
	float flow, calibr_flow, rate;

	flow =  GetRawWidthFlow();
    rate = Rate(flow);

	calibr_flow = flow * rate;

	return calibr_flow;
}

float TFlowMeter::GetFlow(void)
{
	float flow, calibr_flow, rate;

	flow =  GetRawFlow();
    rate = Rate(flow);

	calibr_flow = flow * rate;

	return calibr_flow;
}

float TFlowMeter::GetFinalRawFlow(void)
{
	float flow;

    flow =  ((HSCDevice->GetFinalImp(Channel)*3600) /Kp)/HSCDevice-> GetFinalTime();

	return flow;
}

float TFlowMeter::GetFinalImp(void)
{

	return (float)HSCDevice->GetFinalImp(Channel);
}

float TFlowMeter::GetFinalFlow(void)
{
	float flow, calibr_flow, rate;

	flow =  ((HSCDevice->GetFinalImp(Channel)*3600) /Kp)/HSCDevice-> GetFinalTime();
	rate = Rate(flow);
	calibr_flow = flow * rate;

	return calibr_flow;
}

//работа с да

void TFlowMeter::SaveDataPoint(TFlowMeter *Etalon)
{

	 if (CheckType ==1)
	{

	DataPoint.Time 		= 	HSCDevice->GetFinalTime();
	DataPoint.Q 		= 	Etalon-> GetFinalFlow();
	DataPoint.Volume 	=   GetFinalVolume();
	DataPoint.Imp       =   HSCDevice->GetFinalImp(Channel);
	DataPoint.Date      =  	DateTimeToStr(Today());
	DataPoint.DateTime  =  Today();
	DataPoint.EtlVolume = 	Etalon-> GetFinalVolume();
	DataPoint.Error     =   ((DataPoint.Volume-DataPoint.EtlVolume)*100)/DataPoint.EtlVolume;

	}
	else if (CheckType ==0)
	{
	DataPoint.Time 		= 	HSCDevice->GetFinalTime();
	DataPoint.EtlVolume = 	Etalon-> GetFinalVolume();

	DataPoint.Date      =  	DateTimeToStr(Today());
	DataPoint.DateTime      	=  Today();

	DataPoint.Q 		=   (DataPoint.EtlVolume*3600)/DataPoint.Time;

	DataPoint.Error     =   ((DataPoint.Volume-DataPoint.EtlVolume)*100)/DataPoint.EtlVolume;

	}
	 else if (CheckType ==2)
	{

	DataPoint.Time 		= 	HSCDevice->GetFinalTime();
	DataPoint.Q 		= 	Etalon-> GetFinalFlow();
	DataPoint.Volume 	=   GetFinalVolume();
	DataPoint.Imp       =   HSCDevice->GetFinalImp(Channel);
	DataPoint.Date      =  	DateTimeToStr(Today());
	DataPoint.DateTime  =   Today();
	DataPoint.EtlVolume = 	Etalon-> GetFinalVolume();
	DataPoint.Error     =   ((DataPoint.Volume-DataPoint.EtlVolume)*100)/DataPoint.EtlVolume;

	}
	else if (CheckType ==3)
	{

	DataPoint.Time 		= 	HSCDevice->GetFinalTime();
	DataPoint.EtlVolume = 	Etalon-> GetFinalVolume();

	DataPoint.Date      =  	DateTimeToStr(Today());
	DataPoint.DateTime      	=  Today();

	DataPoint.Q 		=   (DataPoint.EtlVolume*3600)/DataPoint.Time;

	DataPoint.Error     =   ((DataPoint.Volume-DataPoint.EtlVolume)*100)/DataPoint.EtlVolume;

	}
	 else if (CheckType ==4)
	{
	DataPoint.Date      =  	DateTimeToStr(Today());
	DataPoint.DateTime  =   Today();
	}

   DataPoints.push_back(DataPoint);
   CheckFullStatus();
   SaveToFile(this,0);



}

bool TFlowMeter::GetPointUseFlow(float Q, tPoint &point)
{

	float Qp;
	tPoint pnt;

	 for (int i=0;i<Points.size();i++)
	{
	  // Вычисляем расход для данной калибровочной точки
	   if (Points[i].Time>0)
		{
		Qp = Points[i].Q;
		 pnt = Points[i];

	if  (IsFlowInPoint( Q, pnt))
	 {
		 point = pnt;
		 return true;
	 }
		}
	}

	return false;

}


bool  TFlowMeter::IsDataPointGood(tDataPoint &dataPoint, tPoint point)
{
	float e1,e2;
	float Q = dataPoint.Q;

	float Qp = point.Q;

   // проверка расхода
  if  (IsFlowInPoint( Q, point))
	{
			dataPoint.Name = point.Name;
			e1 = abs (point.Error);
			e2 = abs (dataPoint.Error);
   // проверка времени
			if (dataPoint.Time >= point.Time )
		  {
			 // dataPoint.IsUsed = 1;
	   // проверка погрешности
				if (e1>e2)

		 {

				  dataPoint.State = 4;
				//return 4;
				return true;
			}  else
			{
			  dataPoint.State = 3;
			 // return 3;
				return false;
			}




		  }   else
		  {
			   dataPoint.State = 2;
			   return false;
		  }
		}
	else
	 {
		 dataPoint.Name = L"-";
	 }


 return false;

}






int TFlowMeter::CheckStatus()
{
	tPoint temp,temp1;
	float Q,Q1,Q2,Qd,e1,e2;

	int status = 0;
	int statusPoint = 0;
	int statusPoints = 1;
	int previousStatus = Status;

	tPoint point;

	UnicodeString text=L"";;

	std::vector<tPoint>::iterator it;

	it =  Points.begin();

	if  ( !Points.empty())
	{

	 for (int i=0;i<Points.size();i++)
	{
	  // Вычисляем расход для данной калибровочной точки
	   if (Points[i].Time>0)
		{Q = Points[i].Q;
		 point = Points[i];
		}
		else
	   {
			Status = 0;
			Result = "-";
			return 0;
	   }

		statusPoint = 0;

		 for (int j=0;j<DataPoints.size();j++)
	{
		Qd = DataPoints[j].Q;
		Q1 = Q*1.1;
		Q2 = Q*0.90;

		if ((Q2<Qd)&&(Qd<Q1))
		{
			DataPoints[j].Name =  Points[i].Name;
			e1 = Points[i].Error ;
			e2 = DataPoints[j].Error;
			if (Points[i].Error>DataPoints[j].Error)
		  {
			  statusPoint = 1;
			  DataPoints[j].IsUsed = 1;
			  j = DataPoints.size();
		  }   else
			{
			   Status = 1;
			   Result = L"Не годен";
			   return 1; //не годен
			}
		}
	}

	  if (statusPoint==1) {
				statusPoints = statusPoints;
	  } else
	   {
			 statusPoints = 0;
	   }

	}

	  if (statusPoints==1) {
		 Status = 2;
		 Result = L"Годен";
		 Status = 2;
		return 2;   //годен

	  }  else
	  {
		Status = 0;
		Result = L"-";
		return 0;   // не достаточно данных
	  }

   }
 Result = L"-";

 Status = 0;
 return 0;
}


int TFlowMeter::CheckFullStatus()
{

	float Q,Q1,Q2,Qd,e1,e2;

	int status = 0;
	int statusPoint = 0;
	int statusPoints = 5;
	int previousStatus = Status;


	UnicodeString text=L"";

	UsedDataPoints.clear();


	std::vector<tPoint>::iterator it;

	  for (int j=0;j<DataPoints.size();j++)
	   {       DataPoints[j].State = 0;            }

		if  ( (!Points.empty())&&(!DataPoints.empty()))
	{

	 for (int i=0;i<Points.size();i++)
	{

	  UsedDataPoints.push_back(DataPoints[0]);
	  // Вычисляем расход для данной калибровочной точки
	   if (Points[i].Time>0)
		{Q = Points[i].Q;
		 Points[i].State = 0; //Обнуляем состояние
		}
		else
	   {
			Status = 0;
			Result = "-";
			return 0;
	   }

	   statusPoint = 0;


		 for (int j=0;j<DataPoints.size();j++)
	{


	   if (DataPoints[j].State<2)
		  {
		if  (IsDataPointGood(DataPoints[j], Points[i]))
		{
		  if ((Points[i].State == 0)||(Points[i].State == 3)) {
			Points[i].State = 5;
			DataPoints[j].State = 5;
			DataPoints[j].Point = &Points[i];
			UsedDataPoints[i] = DataPoints[j];

		  }
		 // j = DataPoints.size();
		}
		else
		{
			if (DataPoints[j].State == 3) {
			   Status = 1;
			 if (Points[i].State != 5) {
				 Points[i].State = 3;
				 DataPoints[j].Point = &Points[i];
				 UsedDataPoints[i] = DataPoints[j];
			 }


			  // Result = L"Не годен";
			  // return 1; //не годен
			}

		}
		  }
		}

	   if (Points[i].State !=5) {

		if (Points[i].State ==3) {
			 statusPoints = 3;
		}  else
		{
			 statusPoints = 0;
		}
	  } else
	   {



	   }

	}

	  if (statusPoints==5) {
		 Status = 2;
		 Result = L"Годен";
		return 2;   //годен

	  }  else
	  {

	  if (statusPoints==3) {
		Result = L"Не годен";
		return 1; //не годен
	  }

		Status = 0;
		Result = L"Нет всех измерений";
		return 0;   // не достаточно данных
	  }

   }
 Result = L"Нет всех измерений";

 Status = 0;
 return 0;
	}


vector <tDataPoint> TFlowMeter::SortDataVector(vector <tDataPoint> vect, int State,int State2, int maxmin=0 )
 {
	float Q= 		MAXFLOAT ;
	float Qmax = 	0;

	int index  = 	0;
	vector <tDataPoint> 	dataPoints;
	vector <tDataPoint> 	dataPoints1;

	dataPoints.clear();

	  while (index!=-1)

  {
		index = -1;

		 for (int i=0;i<vect.size();i++)
	{
	  if ((vect[i].State==State2)||(vect[i].State==State)||(State==-1))
	   if ((Qmax <= vect[i].Q)&&(vect[i].Q<Q))
	   {
		index = i;
		Qmax = vect[i].Q;
	   }
	}



	 if (index!=-1)
	 {
	  for (int i=0;i<vect.size();i++)
	{
	   if ((vect[i].State==State)||(State==-1))
	   if (Qmax == vect[i].Q)
		{
		 dataPoints.push_back(vect[i]);
		}
	}
		Q = Qmax;
		Qmax = 0;
	 }
   }


	dataPoints1.clear();

	if (maxmin==1) {
	for (int i=0;i<dataPoints.size();i++)
	{
	  //if (DataPoints[i].State==5)

		 dataPoints1.push_back(dataPoints[i]);

	}
	} else
	{
	for (int i=dataPoints.size()-1;i>=0;i--)
	{
	  //if (DataPoints[i].State==5)

		 dataPoints1.push_back(dataPoints[i]);

	}
	}






   return  dataPoints1;

}


void TFlowMeter::SortDataPoints(int maxmin)
{
	float Q= 		FLT_MAX ;
	float Qmax = 	0;

	int index  = 	0;

	vector <tDataPoint> 	dataPoints;

	dataPoints.clear();

	  while (index!=-1)

  {
		index = -1;

		 for (int i=0;i<DataPoints.size();i++)
	{
	  //if (DataPoints[i].State==5)
	   if ((Qmax <= DataPoints[i].Q)&&(DataPoints[i].Q<Q))
	   {
		index = i;
		Qmax = DataPoints[i].Q;
	   }
	}



	 if (index!=-1)
	 {
	  for (int i=0;i<DataPoints.size();i++)
	{
	  //if (DataPoints[i].State==5)
	   if (Qmax == DataPoints[i].Q)
		{
		 dataPoints.push_back(DataPoints[i]);
	   }
	}
		Q = Qmax;
		Qmax = 0;
	 }
   }

	DataPoints.clear();
	if (maxmin==1) {
	for (int i=0;i<dataPoints.size();i++)
	{
	  //if (DataPoints[i].State==5)

		 DataPoints.push_back(dataPoints[i]);

	}
	} else
	{
	for (int i=dataPoints.size()-1;i>=0;i--)
	{
	  //if (DataPoints[i].State==5)

		 DataPoints.push_back(dataPoints[i]);

	}
	}


}





uint8_t TFlowMeter::AddPointData (UnicodeString Name, float Qrate, float Q, float Volume, float vTime, float Error,float RagePlus, float RageMinus )
{
	tPoint Pnt;
	uint8_t error;

	Pnt.Name = Name;
	/*
	if (vTime == 0) { return 2; }
	if (vTime < 1) { return 3; }
	if (vTime > 100000) { return 4; }

     #ifndef __ANDROID__
    if (!_finite(vTime)) { return 5; }

        #endif
    */
    if ((!IsNan(Qrate))&&(!IsInfinite(Qrate))  )
    {

    if ((Qrate>0)&&(Qmax>0))
    {
	    Pnt.Q =  Qmax * Qrate;
        Pnt.Qrate = Qrate;
    }
    else if (Q>0)
    {
        Pnt.Q=  Q;
    if ( Qmax > 0)
    {
     Pnt.Qrate = Pnt.Q/Qmax;
    }

    }
    else
    {
     return 6;
    }

    } else
    {
     return 7;
    }


        if ((!IsNan(Volume))&&(!IsInfinite(vTime)))
    {

	if (Volume>0)
	{
		Pnt.Volume =  Volume;
	}  else
    {
		Pnt.Volume =  0;
	}

    if (vTime>1)
    {
        Pnt.Time =  vTime;
    } else
    {
        Pnt.Time = 3600;
    }

    } else
    {
     return 8;
    }


	 if (!IsNan(Error))
	{
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


    /*Pnt.Volume = 	Pnt.Imp / Pnt.Coef;
    Pnt.Q = 		(Pnt.EtlVolume/Pnt.Time)*3600;
    Pnt.Qtest  =  	(Pnt.Volume/Pnt.Time)*3600;

    Pnt.Error =   	((Pnt.Volume - Pnt.EtlVolume)*100)/Pnt.EtlVolume;
    Pnt.Rate  =   	Pnt.Volume/Pnt.EtlVolume; */

    AddCurrPoint (Pnt);

    return 1;

}

void TFlowMeter::AddCurrPoint(tPoint Pnt)
{
  Point = Pnt;
  Points.push_back(Point);
}


	void TFlowMeter::SetType (TFlowMeterType *type)
{

	int i,i1,i2;

	Type = type;
	if (Type!=nullptr) {

	TypeHash =  Type->Hash;
	DeviceType = Type->DeviceName;
   //	SerialNum= Type->SerialNum;
	CertificateNum= Type->CertificateNum;
	Modifications =  Type->Modification ;
    Modification =   Type->Modification ;
	docTitle = Type->VerificationNum;
	DN = Type->DN;

	Date1 = Type->Date1;
	Date2 = Type->Date2;

	if (TryStrToInt_(Type->VerificationInterval1,i1)) {
	if (TryStrToInt_(Type->VerificationInterval2,i2)) {
	 if (i1==i2) {
	  VerificationInterval = i1;
	 } else
	 {
	  VerificationInterval = 0;
	 }
	} else
	{
	  VerificationInterval = i1;
	}

	} else
	{
	  VerificationInterval = 0;
	}

	Kp= Type->Kp;
	Qmax= Type->Qmax;

	Points.clear();

	for (int i=0;i<Type->Points.size();i++)
	{
	Points.push_back(Type->Points[i]);
	}
	}  else
	{

	}
}


bool TFlowMeter::InitType(TFlowMeter *FlowMeter)
{
  if (FlowMeter->TypeHash!=0) {

	   FlowMeter->Type = (TFlowMeterType::Get(FlowMeter->TypeHash));

		if (FlowMeter->Type!=nullptr) {
		   FlowMeter->SetType(FlowMeter->Type);
		   return true;
		}
  } else
	{
 for (int j = 0; j < TFlowMeterType::MeterTypes.size(); j++)
	{
	  if (FlowMeter->DeviceType==TFlowMeterType::MeterTypes[j]->DeviceType)
	  {

	   FlowMeter->Type = (TFlowMeterType::MeterTypes[j]);

		  if (FlowMeter->Type!=nullptr) {
		   FlowMeter->SetType(FlowMeter->Type);
		   return true;
		}
	  }
	}
	}
  return false;
}

bool TFlowMeter::SetType(int typeHash)
{
  SetType(TFlowMeterType::Get(typeHash));

	return true;


}


void TFlowMeter::AddPoint(tPoint Pnt)
{
	tPoint temp,temp1;
	float Q1,Q2;

	std::vector<tPoint>::iterator it;

	it =  Points.begin();

	if  ( !Points.empty())
	{
	 temp = Pnt;

	 for (int i=0;i<Points.size();i++)
	{
	  // Вычисляем расход для данной калибровочной точки
	   temp1 =  Points[i];
	   if (Points[i].Time>0)
	   {Q1 = Points[i].Q;}
		else
	   {return ;}

	   if (Pnt.Time>0)
	   {Q2 = Pnt.Q;}
		else
	   {return ;}

	   if (Q2>=Q1)
	   {
		  Points.insert(it+i,Pnt);
		  return ;
	   }
	}

	  Points.push_back(Pnt);
	  return ;

	} else
	{
	 Points.push_back(Pnt);
	 return ;
	}
}

void TFlowMeter::AddDataPoint(tPoint Pnt)
{
  Point = Pnt;
  Points.push_back(Point);
}

void TFlowMeter::AddDataCalibrPoint(void)
{
	AddCalibrPoint(CalibrPoint);

}

void TFlowMeter::LoadCalibrData(void)
{
HSC->Read_ConfigData();

}

void TFlowMeter::SaveCalibrData(void)
{

   tCalibrPoint Pnt;
   float cEtlVolume[20], cTime[20],cImp [20], cCoefs[20];
   int size;


    if  ( !CalibrPoints.empty())
    {
     size = CalibrPoints.size();
    }  else
    {
     size = 0;
    }

     for (int i=0;i<20;i++)
    {
      // Вычисляем расход для данной калибровочной точки
      if (i<size)
      {

      Pnt = CalibrPoints[i];

    cEtlVolume[i] = Pnt.EtlVolume;
    cTime[i] = Pnt.Time;
    cImp [i] = Pnt.Imp;
    cCoefs[i] = Pnt.Coef;
    }else
    {
    cEtlVolume[i] = 0;
    cTime[i] = 0;
    cImp [i] = 0;
    cCoefs[i] = 0;
    }
    }


       HSC->Save_CalibrData(cEtlVolume,  cTime,  cImp , cCoefs) ;

   }

 void TFlowMeter::SaveCoef()
 {

   HSC->Write_Coef(Kp);


 }

int8_t TFlowMeter::AddCalibrData(float vEtlVolume, float vTime, float vImp, float vCoef)
 {
    tCalibrPoint Pnt;

	if (vCoef == 0) { return -1; }
    if (vTime == 0) { return -2; }
    if (vTime < 1) { return -3; }
    if (vTime > 100000) { return -4; }


    if (IsInfinite(vTime)) { return -5; }
    if (IsInfinite(vCoef)) { return -5; }
    if (IsInfinite(vImp)) { return -5; }

 	if (IsNan(vTime)) { return -5; }
    if (IsNan(vCoef)) { return -5; }
    if (IsNan(vImp)) { return -5; }


    Pnt.EtlVolume = vEtlVolume;
    Pnt.Time= vTime;
    Pnt.Imp = vImp;
    Pnt.Coef = vCoef;

    Pnt.Volume = 	Pnt.Imp / Pnt.Coef;
    Pnt.Q = 		(Pnt.EtlVolume/Pnt.Time)*3600;
    Pnt.Qtest  =  	(Pnt.Volume/Pnt.Time)*3600;

    Pnt.Error =   	((Pnt.Volume - Pnt.EtlVolume)*100)/Pnt.EtlVolume;
    Pnt.Rate  =   	Pnt.Volume/Pnt.EtlVolume;

    AddCalibrPoint(Pnt);

    return 1;

 }

int8_t TFlowMeter::UpdateCoefs()
 {

 tCoef coef;

 float Qtest1, Qetl1, Qtest2, Qetl2;
 float k,b;

    Coefs.clear();
    if (!CalibrPoints.empty())
    {
       if ( CalibrPoints[0].Qtest !=0)
       {
       Qtest1 =  	CalibrPoints[0].Qtest;
       Qetl1 =  	CalibrPoints[0].Q;


       k = Qetl1 / Qtest1;

       coef.Q1 =  0;
       coef.Q2 =  Qtest1;
       coef.K =   k;
       coef.b =   0;



       Coefs.push_back(coef);
       }


    for (int i=1;i<CalibrPoints.size();i++)
    {

       Qtest1 =  	CalibrPoints[i-1].Qtest;
       Qetl1 =  	CalibrPoints[i-1].Q;
       Qtest2 =  	CalibrPoints[i].Qtest;
       Qetl2 =  	CalibrPoints[i].Q;

       if ((Qtest1- Qtest2)!=0)
       {
       k =  (Qetl1- Qetl2) / (Qtest1- Qtest2);
       } else
       {
       k = 0;
       }

       b =  Qetl1- k*Qtest1;

       coef.Q1 =  Qtest1;
       coef.Q2 =  Qtest2;
       coef.K =  k;
       coef.b =  b;

       Coefs.push_back(coef);
    }

    }

    else
    {

      return 0;
    }

    return 1;
 }

void TFlowMeter::ClearPoints(void)
{
	Points.clear();
}

void TFlowMeter::ClearCalibraion(void)
{
  CalibrPoints.clear();
  Coefs.clear();
}

float  TFlowMeter::Rate(float Q)
{
	float temp,k,b;
    float Qetl=0;
    float rate;

    if (Q == 0) return 1;


    if (!Coefs.empty())
    {
    for (int i=0;i<Coefs.size();i++)
    {
    k = Coefs[i].K;
    b = Coefs[i].b;

     if ((Q>= Coefs[i].Q1)&&(Q< Coefs[i].Q2))
     {

        Qetl = k*Q+ b;
        rate=Qetl/Q;
        return rate;
     }
    }

    if (Qetl == 0)
    {
        Qetl= k*Q+ b;
        rate=Qetl/Q;
        return rate;
    }


    }
    else
    {
     return 1;
    }
    return 1;
}

void TFlowMeter::AddCurrentCalibrPoint(void)
{
	AddCalibrPoint(CalibrPoint);
}

int8_t TFlowMeter::AddCalibrPoint(tCalibrPoint Pnt)
{

tCalibrPoint temp,temp1;
float Q1,Q2;
std::vector<tCalibrPoint>::iterator it;

    it =  CalibrPoints.begin();

    if  ( !CalibrPoints.empty())
    {
     temp = Pnt;

     for (int i=0;i<CalibrPoints.size();i++)
    {
      // Вычисляем расход для данной калибровочной точки
       temp1 =  CalibrPoints[i];
       if (CalibrPoints[i].Time>0)
        {Q1 = CalibrPoints[i].EtlVolume/CalibrPoints[i].Time;}
        else
        {return -1;}

       if (Pnt.Time>0)
        {Q2 = Pnt.EtlVolume/Pnt.Time;}
        else
        {return -1;}

       if (Q2<=Q1)
       {
          CalibrPoints.insert(it+i,Pnt);
          return 1;
       }
    }

      CalibrPoints.push_back(Pnt);
      return 2;

    } else
    {
     CalibrPoints.push_back(Pnt);
     return 3;
    }

}

tPoint TFlowMeter::GetCurrentPoint()
{
return Point;
}

tPoint TFlowMeter::SetNextPoint()
{
    int index=-1;
    float eps=0.01;

	for (int i=0;i<Points.size();i++)
{
    if (SameValue(Point.Q,Points[i].Q,eps))
    {
      index = i;

    }

}


if (index>=0)
{
    if (index+1<Points.size())
    {
    Point = Points[index+1];
     } else
     {
    // Point = Points[0];
     }

} else
{
  index = Point.Index;
}

return Point;
}

tPoint TFlowMeter::SetPreviousPoint()
{
    int index = -1;
    float eps=0.001;


	for (int i=0;i<Points.size();i++)
{
	if (SameValue(Point.Q,Points[i].Q,eps))
    {
      index = i;
    }

}

 if (index!=-1)
 {
 if ((index)>0)
    {
	Point = Points[index-1];
	 } else
     {
	 Point = Points[0];
     }
   }


return Point;
}

	void TFlowMeter::RestoreTypePoints(void)
{
	Points.clear();

	for (int i=0;i<Type->Points.size();i++)
	{
	Points.push_back(Type->Points[i]);
	}

}


void TFlowMeter::SaveToFile(TFlowMeter *AFlowMeter, int IsBackUp)
{

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

	rootNode = XmlDoc->AddChild(L"Devices");

	// CheckStoragePermission_(this);

	if (!FlowMeters.empty()) {

		rootNode->SetAttribute(L"VER", XMLVERFLOWMETERS);

			k=  FlowMeters.size();
            while (l < FlowMeters.size())
            {
			if ((FlowMeters[l]->SerialNum=="")&&(FlowMeters[l]->DataPoints.empty()))
			{

                  TFlowMeter::FlowMeters.erase(TFlowMeter::FlowMeters.begin()+l);

			}   else
			{
				 l=l+1;
			}

			}



		rootNode->SetAttribute(L"DeviceCount", IntToStr((int) FlowMeters.size()));

		for (int j = 0; j < FlowMeters.size(); j++) {


			sampleNode = rootNode->AddChild(L"Device" + IntToStr(j));

			 {
			sampleNode->SetAttribute(L"Hash", FlowMeters[j]->Hash);

					sampleNode->SetAttribute(L"ID_Order", FlowMeters[j]->ID_Order);

            if (AFlowMeter==nullptr)
            {
			   FlowMeters[j]->Active = 0;
            }
			else if (AFlowMeter->Hash==FlowMeters[j]->Hash)
			{
                 FlowMeters[j]->Active = 1;
            }
            else
            {
				FlowMeters[j]->Active = 0;
            }


			sampleNode->SetAttribute(L"Active", IntToStr(FlowMeters[j]->Active));
			sampleNode->SetAttribute(L"DeviceType", FlowMeters[j]->DeviceType);
			sampleNode->SetAttribute(L"Modification", FlowMeters[j]->Modification);

			sampleNode->SetAttribute(L"Modifications", FlowMeters[j]->Modifications);

			sampleNode->SetAttribute(L"TypeHash", IntToStr(FlowMeters[j]->TypeHash));
			sampleNode->SetAttribute(L"Serial", FlowMeters[j]->SerialNum);
			sampleNode->SetAttribute(L"YearOfProduction", FlowMeters[j]->year_production);

			sampleNode->SetAttribute(L"CertificateNum",
				FlowMeters[j]->CertificateNum);

			sampleNode->SetAttribute(L"CheckType", IntToStr(FlowMeters[j]->CheckType));
			sampleNode->SetAttribute(L"VerificationInterval", IntToStr(FlowMeters[j]->VerificationInterval));
			sampleNode->SetAttribute(L"Kp", FloatToStr(FlowMeters[j]->Kp));
			sampleNode->SetAttribute(L"Qmax", FloatToStr(FlowMeters[j]->Qmax));

			sampleNode->SetAttribute(L"Status", IntToStr(FlowMeters[j]->Status ));
			sampleNode->SetAttribute(L"SendStatus", IntToStr(FlowMeters[j]->SendStatus));


			sampleNode->SetAttribute(L"Adress", FlowMeters[j]->Adress);
			sampleNode->SetAttribute(L"doc_number", FlowMeters[j]->doc_number);


			sampleNode2 = sampleNode->AddChild(L"Points");

			sampleNode2->SetAttribute(L"PointsCount",
				IntToStr((int) FlowMeters[j]->Points.size()));

			if (!FlowMeters[j]->Points.empty()) {
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
						10, 2));
					sampleNode2->SetAttribute(L"Error",
						FloatToStrF(FlowMeters[j]->Points[i].Error, ffNumber,
						10, 2));
					sampleNode2->SetAttribute(L"RagePlus",
						FloatToStrF(FlowMeters[j]->Points[i].RagePlus, ffNumber,
						3, 1));
					sampleNode2->SetAttribute(L"RageMinus",
						FloatToStrF(FlowMeters[j]->Points[i].RageMinus, ffNumber,
						3, 1));
					sampleNode2->SetAttribute(L"IsRageFree",
						IntToStr(FlowMeters[j]->Points[i].IsRageFree));
				}

			}

			sampleNode2 = sampleNode->AddChild(L"DataPoints");

			sampleNode2->SetAttribute(L"DataPointsCount",
				IntToStr((int) FlowMeters[j]->DataPoints.size()));

			if (!FlowMeters[j]->DataPoints.empty()) {
				for (int i = 0; i < FlowMeters[j]->DataPoints.size(); i++) {
					sampleNode2 = sampleNode->AddChild(L"DataPoint" + IntToStr(i));
					sampleNode2->SetAttribute(L"Name",
						FlowMeters[j]->DataPoints[i].Name);
					sampleNode2->SetAttribute(L"Comment",
						FlowMeters[j]->DataPoints[i].Comment);
				   /*	sampleNode2->SetAttribute(L"Qrate",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Q, ffNumber,
						10, 2));   */
					sampleNode2->SetAttribute(L"Q",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Q, ffNumber,
						10, 2));
					sampleNode2->SetAttribute(L"Time",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Time, ffNumber,
						10, 2));
					sampleNode2->SetAttribute(L"Imp",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Imp, ffNumber,
						10, 2));
					sampleNode2->SetAttribute(L"EtlVolume",
						FloatToStrF(FlowMeters[j]->DataPoints[i].EtlVolume,
						ffNumber, 10, 2));
					sampleNode2->SetAttribute(L"Volume",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Volume,
						ffNumber, 10, 2));
					sampleNode2->SetAttribute(L"VolumeBefore",
						FloatToStrF((float)FlowMeters[j]->DataPoints[i].VolumeBefore,
						ffNumber, 10,5));
					sampleNode2->SetAttribute(L"VolumeAfter",
						FloatToStrF((float)FlowMeters[j]->DataPoints[i].VolumeAfter,
						ffNumber, 10, 5));


					sampleNode2->SetAttribute(L"Temp",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Temp, ffNumber,
						10, 2));

					 sampleNode2->SetAttribute(L"Pres",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Pres, ffNumber,
						10, 2));

					 sampleNode2->SetAttribute(L"Data",
						FlowMeters[j]->DataPoints[i].Date);
						//FlowMeters[j]->

					sampleNode2->SetAttribute(L"Error",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Error, ffNumber,
						10, 2));

				}

			}


          }




		}

		// Если все номера были пустые и мы всё удадиди, то надо добавить пустой прибор


	}  else
	{
		rootNode->SetAttribute(L"VER", XMLVER);
		rootNode->SetAttribute(L"DeviceCount", "0");
	}

	if (IsBackUp==0) {
	fname = TSettingsClass::Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + L"TestMeters" +
		System::Ioutils::TPath::ExtensionSeparatorChar + L"xml";
		XmlDoc->SaveToFile(fname);

	}   else
	{
	fname = TSettingsClass::Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + L"TestMetersBackUp"+IntToStr(IsBackUp) +
		System::Ioutils::TPath::ExtensionSeparatorChar + L"xml";
	}





	// XmlDoc->L
	XmlDoc->Active = false;

}



  UnicodeString TFlowMeter::StringStreamConvert()
{
	 TStringList *str = new TStringList();
	UnicodeString STR;
	 TStringStream *REST_parameters;  // =  new TStringStream(str1);

   str->Add("{");
  str->Add("\"order_id\": "+IntToStr(ID_Order)+",");
  str->Add("\"mi_owner\": \""+miOwner+"\",");
   str->Add("\"active\": \""+IntToStr(Active)+"\",");
  str->Add("\"device_type\": \""+DeviceType+"\",");
  str->Add("\"manufacture_num\": \""+SerialNum+"\",");
  str->Add("\"mitype_number\": \""+CertificateNum+"\",");
  str->Add("\"doc_title\": \""+docTitle+"\",");
  str->Add("\"means\": \""+means+"\",");
  str->Add("\"modification\": \""+Modification+"\",");
  str->Add("\"dn\":\""+DN+"\",");
  str->Add("\"next_verification\":\""+validDate+"\",");
  str->Add("\"vrf_date\":\""+vrfDate+"\",");
  str->Add("\"check_type\":\""+IntToStr(CheckType)+"\",");
  str->Add("\"kp\":\""+FloatToStr(Kp)+"\",");
  str->Add("\"q_max\":\""+FloatToStr(Qmax)+"\",");
  str->Add("\"temperature\":\""+temperature+"\",");
  str->Add("\"temp_water\":\""+tempWater+"\",");
  str->Add("\"pressure\":\""+pressure+"\",");
  str->Add("\"hymidity\":\""+hymidity+"\",");;
  str->Add("\"result\":\""+Result+"\",");
  str->Add("\"year_production\":\""+year_production+"\",");
  str->Add("\"data1\":\"\",");
  str->Add("\"data2\":\"\",");
  str->Add("\"data3\":\"\",");
  str->Add(L"\"data_points\": [");

	// CheckStoragePermission_(this);



			if (!DataPoints.empty()) {
				for (int i = 0; i < DataPoints.size(); i++) {


					if (i==0) {	 str->Add("{"); } else
					{str->Add(","); str->Add("{"); }

					  str->Add("\"q\":\""+FloatToStrF(DataPoints[i].Q, ffNumber,
						10, 2)+"\",");
					  str->Add("\"time\":\""+FloatToStrF(DataPoints[i].Time, ffNumber,
						10, 0)+"\",");
					  str->Add("\"imp\":\""+FloatToStrF(DataPoints[i].Imp, ffNumber,
						10, 0)+"\",");
					  str->Add("\"etl_volume\":\""+FloatToStrF(DataPoints[i].EtlVolume, ffNumber,
						10, 0)+"\",");
					  str->Add("\"volume\":\""+FloatToStrF(DataPoints[i].Volume, ffNumber,
						10, 0)+"\",");
					  str->Add("\"volume_before\":\""+FloatToStrF(DataPoints[i].VolumeBefore, ffNumber,
						10, 0)+"\",");
					  str->Add("\"volume_after\":\""+FloatToStrF(DataPoints[i].VolumeAfter, ffNumber,
						10, 0)+"\",");

					  str->Add("\"temp\":\""+FloatToStrF(DataPoints[i].Temp, ffNumber,
						10, 0)+"\",");

						str->Add("\"pres\":\""+FloatToStrF(DataPoints[i].Pres, ffNumber,
						10, 0)+"\",");
						str->Add("\"point_humidity\":\""+FloatToStrF(DataPoints[i].Humidity, ffNumber,
						10, 0)+"\",");
						str->Add("\"temp_air\":\""+FloatToStrF(DataPoints[i].TempAir, ffNumber,
						10, 0)+"\",");
						str->Add("\"data\":\"\",");
						str->Add("\"error\":\""+FloatToStrF(DataPoints[i].Error, ffNumber,
						10, 0)+"\"");

						str->Add("}");

				}

			}
			str->Add("]");

str->Add("}");
	STR = str->Text;

	 return STR ;

}



TFlowMeter *TFlowMeter::LoadFromFile(void) {

	bool success = true;

	XmlDoc = nullptr;
	rootNode = nullptr;
	sampleNode = nullptr;

	TFlowMeter *FlowMeter;

	String str;

	int Size;
	int PointsSize;
	int index1 = 0;
	int index2 = 0;
	 int  size;

	float fl;

	_di_IXMLNode rootNode1;

	_di_IXMLDocument document = interface_cast<Xmlintf::IXMLDocument>
		(new TXMLDocument(NULL));

	UnicodeString SerialNum;
	UnicodeString dDir, FileName;
	UnicodeString VER;
	 UnicodeString S;
	UnicodeString Result;

	_di_IXMLNodeList nodeList;

	// CheckStoragePermission_(this);

	XmlDoc = new TXMLDocument(NULL);




	fname = TSettingsClass::Dir +
		System::Ioutils::TPath::DirectorySeparatorChar + "TestMeters" +
		System::Ioutils::TPath::ExtensionSeparatorChar + "xml";

   /*
	if (!DirectoryExists(TSettingsClass::Dir)) {
		Result = L"Файл устройств не существует";
		throw Exception(L"Файл устройств не существует");
	}

	FileName = ExtractFileName(fname);
	 */
	if (!FileExists(fname)) {

		Result = L"Файл устройств не существует";
	   //	throw Exception(L"Файл типов не существует");
        ShowMessage(Format(L"Файл '%s' не существует. Создан новый файл.",
	 ARRAYOFCONST((fname))));
		 SaveToFile(nullptr,0);

	}

	document = LoadXMLDocument(fname);

	if (document == NULL) {

		Result = L"Файл пустой или поврежден";
		throw Exception(Result);

	}

	TFlowMeter::FlowMeters.clear();

	rootNode = document->ChildNodes->FindNode(L"Devices");

	if (rootNode == NULL) {
		Result = L"Файл пустой или поврежден";
		throw Exception(Result);
	}


	// САМАЯ РАНЯЯ ВЕРСИЯ !


	 VER = rootNode->Attributes[L"VER"];


  //	if (VER == "4.0") {
   /*
		Size = rootNode->Attributes[L"DeviceCount"];

		for (int j = 0; j < Size; j++) {

			sampleNode = rootNode->ChildNodes->FindNode(L"Device" + IntToStr(j));

			if (sampleNode != NULL) {

				 if (sHSC== nullptr)
				 {
					FlowMeter = new TFlowMeter(false);
				  } else
				  {
					FlowMeter = new TFlowMeter(sHSC, false);
				  }


				FlowMeter->ID_Order = sampleNode->Attributes[L"ID_Order"];
				FlowMeter->Active = StrToInt(sampleNode->Attributes[L"Active"]);

				if (FlowMeter->Active == 1)
				 {
					ActiveFlowMeter = FlowMeter;
				 }

				 //try
				 {

				FlowMeter->DeviceType = sampleNode->Attributes[L"DeviceType"];
				FlowMeter->TypeHash =  StrToInt(sampleNode->Attributes[L"TypeHash"]);

				InitType (FlowMeter);

				FlowMeter->Modification 		=  (sampleNode->Attributes[L"Modification"]);
				FlowMeter->SerialNum 			= sampleNode->Attributes[L"Serial"];
				FlowMeter->year_production 		= sampleNode->Attributes[L"YearOfProduction"];
				FlowMeter->VerificationInterval = StrToInt(sampleNode->Attributes[L"VerificationInterval"]);
				FlowMeter->CheckType 			= StrToInt(sampleNode->Attributes[L"CheckType"]);

				FlowMeter->Adress 			=	GetAttribute(L"Adress",sampleNode);
				FlowMeter->doc_number 		=	GetAttribute(L"doc_number",sampleNode);


				FlowMeter->CertificateNum =
					sampleNode->Attributes[L"CertificateNum"];

			 //   FlowMeter->CheckType = StrToInt( sampleNode->Attributes[L"CheckType"]);

				FlowMeter->Kp = sampleNode->Attributes[L"Kp"];
				FlowMeter->Qmax = sampleNode->Attributes[L"Qmax"];
				  }

			}
			else
			{
				Result = L"Файл пустой или поврежден";
				throw Exception(Result);
			}

			sampleNode2 = sampleNode->ChildNodes->FindNode(L"Points");

			PointsSize = sampleNode2->Attributes[L"PointsCount"];

			if (PointsSize > 0) {
				FlowMeter->Points.clear();

				for (int i = 0; i < PointsSize; i++) {

					sampleNode2 = sampleNode->ChildNodes->FindNode
						(L"Point" + IntToStr(i));

					if (sampleNode != NULL) {
						FlowMeter->Point.Name =
					   		sampleNode2->Attributes[L"Name"];
						FlowMeter->Point.Qrate =
							StrToFloat_(sampleNode2->Attributes[L"Qrate"]);
						FlowMeter->Point.Q =
							StrToFloat_(sampleNode2->Attributes[L"Q"]);
						FlowMeter->Point.Volume =
							StrToFloat_(sampleNode2->Attributes[L"Volume"]);
						FlowMeter->Point.Time =
							StrToFloat_(sampleNode2->Attributes[L"Time"]);
						FlowMeter->Point.Error =
							StrToFloat_(sampleNode2->Attributes[L"Error"]);

						FlowMeter->Points.push_back(FlowMeter->Point);
					}
				}

				sampleNode2 = sampleNode->ChildNodes->FindNode(L"DataPoints");

				PointsSize = sampleNode2->Attributes[L"DataPointsCount"];

				for (int i = 0; i < PointsSize; i++) {

					sampleNode2 = sampleNode->ChildNodes->FindNode
						(L"DataPoint" + IntToStr(i));

					if (sampleNode2!= NULL) {
						FlowMeter->DataPoint.Name =
							sampleNode2->Attributes[L"Name"];
						FlowMeter->DataPoint.Comment =
							sampleNode2->Attributes[L"Comment"];
						FlowMeter->DataPoint.Q =
							StrToFloat_(sampleNode2->Attributes[L"Q"]);
						FlowMeter->DataPoint.Time =
							StrToFloat_(sampleNode2->Attributes[L"Time"]);
						FlowMeter->DataPoint.Imp =
							StrToFloat_(sampleNode2->Attributes[L"Imp"]);
						FlowMeter->DataPoint.EtlVolume =
							StrToFloat_(sampleNode2->Attributes[L"EtlVolume"]);

						FlowMeter->DataPoint.Volume =
							StrToFloat_(sampleNode2->Attributes[L"Volume"]);
						FlowMeter->DataPoint.VolumeBefore =
							StrToFloat_(sampleNode2->Attributes[L"VolumeBefore"]);
						 FlowMeter->DataPoint.VolumeAfter =
							StrToFloat_(sampleNode2->Attributes[L"VolumeAfter"]);

						FlowMeter->DataPoint.VolumeAfter =
							StrToFloat_(sampleNode2->Attributes[L"Temp"]);
					   FlowMeter->DataPoint.Pres =
							StrToFloat_(sampleNode2->Attributes[L"Pres"]);
					   FlowMeter->DataPoint.Date =
							(sampleNode2->Attributes[L"Data"]);

						 S =  sampleNode2->Attributes[L"Error"];

						fl = StrToFloat(S);

						FlowMeter->DataPoint.Error =  fl;


						FlowMeter->DataPoints.push_back(FlowMeter->DataPoint);
					}
				}
			}



		 //   TFlowMeter::FlowMeters.push_back(FlowMeter);


		}
		 */

  //	}

  //	else if (VER == XMLVERFLOWMETERS) {
		Size = rootNode->Attributes[L"DeviceCount"];

		for (int j = 0; j < Size; j++) {

			sampleNode = rootNode->ChildNodes->FindNode(L"Device" + IntToStr(j));

			if (sampleNode != NULL) {

				 if (sHSC== nullptr)
				 {
					FlowMeter = new TFlowMeter(false);
				  } else
				  {
					FlowMeter = new TFlowMeter(sHSC, false);
				  }


				FlowMeter->ID_Order = sampleNode->Attributes[L"ID_Order"];
				FlowMeter->Active = StrToInt(sampleNode->Attributes[L"Active"]);

				if (FlowMeter->Active == 1)
				 {
					ActiveFlowMeter = FlowMeter;
				 }


				FlowMeter->DeviceType = sampleNode->Attributes[L"DeviceType"];
				FlowMeter->TypeHash =  StrToInt(sampleNode->Attributes[L"TypeHash"]);
				InitType(FlowMeter);
				FlowMeter->Modification =  (sampleNode->Attributes[L"Modification"]);

				FlowMeter->Modifications  = GetAttribute(L"Modifications",sampleNode);

				FlowMeter->SerialNum = sampleNode->Attributes[L"Serial"];
				FlowMeter->CertificateNum = sampleNode->Attributes[L"CertificateNum"];
				FlowMeter->VerificationInterval = StrToInt(sampleNode->Attributes[L"VerificationInterval"]);
				FlowMeter->CheckType = StrToInt(sampleNode->Attributes[L"CheckType"]);
				FlowMeter->year_production = sampleNode->Attributes[L"YearOfProduction"];

				FlowMeter->Adress 			=	GetAttribute(L"Adress",sampleNode);


				FlowMeter->Kp = sampleNode->Attributes[L"Kp"];
				FlowMeter->Qmax = sampleNode->Attributes[L"Qmax"];
				FlowMeter->Status = StrToInt(sampleNode->Attributes[L"Status"]);
				FlowMeter->SendStatus = StrToInt(sampleNode->Attributes[L"SendStatus"]);
			}
			else
			{
				Result = L"Файл пустой или поврежден";
				throw Exception(Result);
			}

			sampleNode2 = sampleNode->ChildNodes->FindNode(L"Points");

			PointsSize = sampleNode2->Attributes[L"PointsCount"];

			if (PointsSize > 0) {
				FlowMeter->Points.clear();

				for (int i = 0; i < PointsSize; i++) {

					sampleNode2 = sampleNode->ChildNodes->FindNode
						(L"Point" + IntToStr(i));

					if (sampleNode != NULL) {
						FlowMeter->Point.Name =
							sampleNode2->Attributes[L"Name"];
						FlowMeter->Point.Qrate =
							StrToFloat_(sampleNode2->Attributes[L"Qrate"]);
						FlowMeter->Point.Q =
							StrToFloat_(sampleNode2->Attributes[L"Q"]);
						FlowMeter->Point.Volume =
							StrToFloat_(sampleNode2->Attributes[L"Volume"]);
						FlowMeter->Point.Time =
							StrToFloat_(sampleNode2->Attributes[L"Time"]);
						FlowMeter->Point.Error =
							StrToFloat_(sampleNode2->Attributes[L"Error"]);


					   FlowMeter->Point.Accuracy =
							StrToFloat_(GetAttribute(L"Accuracy",sampleNode2,L"10"));
					  /*
					   if (FlowMeter->Point.Name == L"Qmax") {
						   FlowMeter->Point.RagePlus =    10;
						   FlowMeter->Point.RageMinus =  90;

					   }   else
					   {
					   FlowMeter->Point.RagePlus = 10;
					   FlowMeter->Point.RageMinus = 10;
					   }     */

					   FlowMeter->Point.RagePlus =
							StrToFloat_(GetAttribute(L"RagePlus",sampleNode2,L"10"));

						 if (FlowMeter->Point.Name == L"Qmax") {

						//FlowMeter->Point.RageMinus =
						 //	StrToFloat_(GetAttribute(L"RageMinus",sampleNode2,L"90"));
						  FlowMeter->Point.RageMinus = 90;

					   }   else
					   {
						FlowMeter->Point.RageMinus =
							StrToFloat_(GetAttribute(L"RageMinus",sampleNode2,L"10"));
					   }


					   FlowMeter->Point.IsRageFree =
							StrToInt(GetAttribute(L"IsRageFree",sampleNode2,L"0"));

						GetAttribute(L"ProtocolNumTitle",rootNode);


						FlowMeter->AddPoint(FlowMeter->Point);
					}
				}
               }
				sampleNode2 = sampleNode->ChildNodes->FindNode(L"DataPoints");

				PointsSize = sampleNode2->Attributes[L"DataPointsCount"];

				for (int i = 0; i < PointsSize; i++) {

					sampleNode2 = sampleNode->ChildNodes->FindNode
						(L"DataPoint" + IntToStr(i));

					if (sampleNode2!= NULL) {
						FlowMeter->DataPoint.Name =
							sampleNode2->Attributes[L"Name"];
						FlowMeter->DataPoint.Comment =
							sampleNode2->Attributes[L"Comment"];
						FlowMeter->DataPoint.Q =
							StrToFloat_(sampleNode2->Attributes[L"Q"]);
						FlowMeter->DataPoint.Time =
							StrToFloat_(sampleNode2->Attributes[L"Time"]);
						FlowMeter->DataPoint.Imp =
							StrToFloat_(sampleNode2->Attributes[L"Imp"]);
						FlowMeter->DataPoint.EtlVolume =
							StrToFloat_(sampleNode2->Attributes[L"EtlVolume"]);

						FlowMeter->DataPoint.Volume =
							StrToFloat_(sampleNode2->Attributes[L"Volume"]);
						FlowMeter->DataPoint.VolumeBefore =
							StrToFloat_(sampleNode2->Attributes[L"VolumeBefore"]);
						 FlowMeter->DataPoint.VolumeAfter =
							StrToFloat_(sampleNode2->Attributes[L"VolumeAfter"]);

						FlowMeter->DataPoint.Temp =
							StrToFloat_(sampleNode2->Attributes[L"Temp"]);
					   FlowMeter->DataPoint.Pres =
							StrToFloat_(sampleNode2->Attributes[L"Pres"]);
					   FlowMeter->DataPoint.Date =
							(sampleNode2->Attributes[L"Data"]);

					  str = sampleNode2->Attributes[L"Error"];
						FlowMeter->DataPoint.Error =
							StrToFloat_(sampleNode2->Attributes[L"Error"]);


						FlowMeter->DataPoints.push_back(FlowMeter->DataPoint);
					}
				}




		 //   TFlowMeter::FlowMeters.push_back(FlowMeter);


		}





  /* }	else
	{

				Result = L"Версия не может быть обработана";
				throw Exception(Result);
 *///	}





	return ActiveFlowMeter;
}

void TFlowMeter::AddToList()
{
	TFlowMeter*  meter =  this;

	FlowMeters.insert(FlowMeters.begin(),meter);
}

void TFlowMeter::StaticInit(THscDevice *HSCDev)
{
	FlowMeters.clear();
	sHSC =  HSCDev;
}

TFlowMeter* TFlowMeter::GetDevice (int Hash)
{
	for (int j = 0; j < FlowMeters.size(); j++)
	{
	  if (FlowMeters[j]->Hash == Hash) {
			return  FlowMeters[j];
	  }
	}

	return nullptr;
}

float TFlowMeter::GetVolumeError(void)
{
  float error;

   if (EtalonMeter!=nullptr) {
		error = (VolumeValue->GetFloatValue() - EtalonMeter->VolumeValue->GetFloatValue())
		/ EtalonMeter->VolumeValue->GetFloatValue();
   }
}

void TFlowMeter::SetEtalon(TFlowMeter* Etalon)
{
		EtalonMeter =  Etalon;
}


void TFlowMeter::SetAsEtalon()
{

   if(TSettingsClass::EtalonCHNum>0)
   {
	 EtalonMeter->SetChannel(TSettingsClass::EtalonCHNum);
   } else
   {
	EtalonMeter->SetChannel(3);
   }

	EtalonMeter->Name = "Etalon";
	EtalonMeter->IsEtalon = true;

	EtalonMeter->SetKoef(100);
}


 // Расчёт времени до достижения предела по объему в мс
int TFlowMeter::TimeToEndVolumeLimit(float pointVolume,float DelayCoef, float TimeCoef)
{
	float add_volume=0, volume =0, add_time=0, flow;



	flow = FlowValue->GetFloatValue(L"л/с");  //Получаем л/с
	volume =  VolumeValue->GetFloatValue() ;
	//Прогноз по объему на следующую секунду в л
	add_volume = volume +  flow;
	pointVolume = pointVolume - DelayCoef*flow;

	if ((add_volume > pointVolume) && (pointVolume > 0)) {
			add_time = ((pointVolume - volume)/ flow);
			if (add_time<0) {
				return 1;
			}
			add_time =  add_time *1000;
			add_time =   add_time * TimeCoef ;
			return add_time;
		}

	return 0;

}

UnicodeString TFlowMeter::JSonConvert(TFlowMeter *AFlowMeter)
{

	bool success = true;
	int k=0, l = 0;
	XmlDoc = nullptr;
	rootNode = nullptr;
	sampleNode = nullptr;

	 TJSONArray *jDevices;
	 TJSONArray *jPoints;

	TJSONArray *jDataPoints;
	 TJSONObject * jDevice;
	 TJSONObject *jObject;

	// TJSONObject *jObject = new TJSONObject();
	 jObject = new TJSONObject();

	 TJSONObject *jPoint = new TJSONObject();
	 TJSONObject *jDataPoint;
	//
	// Создадим главную ветку и добавим узел об устройстве

	jObject->AddPair(L"VER", XMLVER);




	// CheckStoragePermission_(this);

	if (!FlowMeters.empty()) {

			k=  FlowMeters.size();
			while (l < FlowMeters.size())
			{
			if ((FlowMeters[l]->SerialNum=="")&&(FlowMeters[l]->DataPoints.empty()))
			{

				  TFlowMeter::FlowMeters.erase(TFlowMeter::FlowMeters.begin()+l);

			}   else
			{
				 l=l+1;
			}

			}



		jObject->AddPair( new TJSONPair(L"DeviceCount", IntToStr((int) FlowMeters.size())));

		jDevices = new TJSONArray();
		jObject->AddPair(L"Devices",jDevices);


		for (int j = 0; j < FlowMeters.size(); j++) {

			jDevice = new TJSONObject();
			jDevice->AddPair( new TJSONPair( L"Device Num" , IntToStr(j)));

			 {
			jDevice->AddPair( new TJSONPair(L"ID_Device", FlowMeters[j]->Hash));
			jDevice->AddPair( new TJSONPair(L"ID_Order", FlowMeters[j]->ID_Order));
			jDevice->AddPair( new TJSONPair(L"miOwner", FlowMeters[j]->miOwner));



			if (AFlowMeter==nullptr)
			{
			   FlowMeters[j]->Active = 0;
			}
			else if (AFlowMeter->Hash==FlowMeters[j]->Hash)
			{
				 FlowMeters[j]->Active = 1;
			}
			else
			{
				FlowMeters[j]->Active = 0;
			}


			jDevice->AddPair( new TJSONPair(L"Active", IntToStr(FlowMeters[j]->Active)));
			jDevice->AddPair( new TJSONPair(L"DeviceType", FlowMeters[j]->DeviceType));
			jDevice->AddPair( new TJSONPair(L"manufactureNum", FlowMeters[j]->SerialNum));
			jDevice->AddPair( new TJSONPair(L"mitypeNumber",
				FlowMeters[j]->CertificateNum));
			jDevice->AddPair( new TJSONPair(L"docTitle",
				FlowMeters[j]->docTitle));
			jDevice->AddPair( new TJSONPair(L"means",
				FlowMeters[j]->means));

			 jDevice->AddPair( new TJSONPair(L"Modification",
				FlowMeters[j]->Modification));




			jDevice->AddPair( new TJSONPair(L"CheckType", IntToStr(FlowMeters[j]->CheckType)));
			jDevice->AddPair( new TJSONPair(L"DN", FlowMeters[j]->DN));

			jDevice->AddPair( new TJSONPair(L"validDate", FlowMeters[j]->validDate));




			jDevice->AddPair( new TJSONPair(L"Kp", FloatToStr(FlowMeters[j]->Kp)));
			jDevice->AddPair( new TJSONPair(L"Qmax", FloatToStr(FlowMeters[j]->Qmax)));

			jDevice->AddPair( new TJSONPair(L"temperature", FlowMeters[j]->temperature));
			jDevice->AddPair( new TJSONPair(L"pressure", FlowMeters[j]->pressure));
			jDevice->AddPair( new TJSONPair(L"hymidity", FlowMeters[j]->hymidity));
			jDevice->AddPair( new TJSONPair(L"validDate", FlowMeters[j]->validDate));
			jDevice->AddPair( new TJSONPair(L"validDate", FlowMeters[j]->validDate));

			 jDevice->AddPair( new TJSONPair(L"Result", FlowMeters[j]->Result));

			 jDevice->AddPair( new TJSONPair(L"Data1", ""));
			 jDevice->AddPair( new TJSONPair(L"Data2", ""));
			 jDevice->AddPair( new TJSONPair(L"Data3", ""));

			jPoints = new TJSONArray();

			jDevice->AddPair(L"PointsCount",IntToStr((int) FlowMeters[j]->Points.size()));
			jDevice->AddPair(L"Points",jPoints);

			if (!FlowMeters[j]->Points.empty()) {
				for (int i = 0; i < FlowMeters[j]->Points.size(); i++) {

				 jPoint = new TJSONObject();

				   jPoint->AddPair(L"Point Num", IntToStr(i));

					jPoint->AddPair(L"Name",
						FlowMeters[j]->Points[i].Name);
					jPoint->AddPair(L"Qrate",
						FloatToStrF(FlowMeters[j]->Points[i].Qrate, ffNumber,
						10, 2));
					jPoint->AddPair(L"Q",
						FloatToStrF(FlowMeters[j]->Points[i].Q, ffNumber,
						10, 2));
					jPoint->AddPair(L"Volume",
						FloatToStrF(FlowMeters[j]->Points[i].Volume,
						ffNumber, 10, 2));
					jPoint->AddPair(L"Time",
						FloatToStrF(FlowMeters[j]->Points[i].Time, ffNumber,
						10, 0));
					jPoint->AddPair(L"Error",
						FloatToStrF(FlowMeters[j]->Points[i].Error, ffNumber,
						10, 2));

				   jPoints->AddElement(jPoint);
				}

			}


			jDataPoints = new TJSONArray();
			jDevice->AddPair(L"DataPointsCount",
				IntToStr((int) FlowMeters[j]->DataPoints.size()));
			jDevice->AddPair(L"DataPoints",jDataPoints);



			if (!FlowMeters[j]->DataPoints.empty()) {
				for (int i = 0; i < FlowMeters[j]->DataPoints.size(); i++) {

					jDataPoint = new TJSONObject();

					jDataPoint->AddPair(L"DataPoint" , IntToStr(i));

					jDataPoint->AddPair(L"Comment",
						FlowMeters[j]->DataPoints[i].Comment);

					jDataPoint->AddPair(L"Q",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Q, ffNumber,
						10, 2));
					jDataPoint->AddPair(L"Time",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Time, ffNumber,
						10, 0));
					jDataPoint->AddPair(L"Imp",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Imp, ffNumber,
						10, 2));
					jDataPoint->AddPair(L"EtlVolume",
						FloatToStrF(FlowMeters[j]->DataPoints[i].EtlVolume,
						ffNumber, 10, 2));
					jDataPoint->AddPair(L"Volume",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Volume,
						ffNumber, 10, 2));
					jDataPoint->AddPair(L"VolumeBefore",
						FloatToStrF(FlowMeters[j]->DataPoints[i].VolumeBefore,
						ffNumber, 10, 2));
					jDataPoint->AddPair(L"VolumeAfter",
						FloatToStrF(FlowMeters[j]->DataPoints[i].VolumeAfter,
						ffNumber, 10, 2));


					jDataPoint->AddPair(L"Temp",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Temp, ffNumber,
						10, 2));

					 jDataPoint->AddPair(L"Pres",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Pres, ffNumber,
						10, 2));

					 jDataPoint->AddPair(L"TempAir",
						FloatToStrF(FlowMeters[j]->DataPoints[i].TempAir, ffNumber,
						10, 2));

					 jDataPoint->AddPair(L"PresAir",
						FloatToStrF(FlowMeters[j]->DataPoints[i].PresAir, ffNumber,
						10, 2));

					 jDataPoint->AddPair(L"Humidity",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Humidity, ffNumber,
						10, 2));

					 jDataPoint->AddPair(L"Data",
						FlowMeters[j]->DataPoints[i].Date);
						//FlowMeters[j]->

					jDataPoint->AddPair(L"Error",
						FloatToStrF(FlowMeters[j]->DataPoints[i].Error, ffNumber,
						10, 2));

					jDataPoints->AddElement(jDataPoint);

				}

			}


		  }


		   jDevices->AddElement(jDevice);

		}



	}  else
	{

	}

	 return jObject->ToString();

}

 void TFlowMeter::ApiSent()
{

	int k=0, l = 0;


	if (!FlowMeters.empty()) {

			k=  FlowMeters.size();
			while (l < FlowMeters.size())
			{
			if (FlowMeters[l]->SendStatus==1) //L"Отправляется")
			 {

				  FlowMeters[l]->SendStatus=2; //L"Отправлен";

			 }
			   l=l+1;
			}
	 }

}

int TFlowMeter::ApiCheckResult()
{
	float fl=0;
	bool success = false;
	int k=0, l = 0;

	if (!FlowMeters.empty()) {

			k=  FlowMeters.size();
			while (l < FlowMeters.size())
			{
			if ((FlowMeters[l]->SerialNum=="")&&(FlowMeters[l]->DataPoints.empty()))
			{
				  TFlowMeter::FlowMeters.erase(TFlowMeter::FlowMeters.begin()+l);

			}   else
			{
				 l=l+1;
			}

			}


		for (int j = 0; j < FlowMeters.size(); j++) {
			int st = FlowMeters[j]->SendStatus;
			System::UnicodeString res = FlowMeters[j]->Result;

		if ((res == L"Годен")&&(st!=2))
		{
			 success = true;
		}
		else if ((FlowMeters[j]->Result == L"Не годен")&&(FlowMeters[j]->SendStatus!=2))
		{

			return 1;

		}
		else
			{

			}
		}
} else
{
	return -1;
}

	if (success!=true) {
		return -1;
	}

	return 0;
}

UnicodeString TFlowMeter::ApiConvert()
{
	float fl=0;
	bool success = true;
	int k=0, l = 0;
	XmlDoc = nullptr;
	rootNode = nullptr;
	sampleNode = nullptr;

	TJSONArray *jDevices;
	TJSONArray *jPoints;
	TJSONArray *jDataPoints;

	TJSONObject * jDevice;
	TJSONObject *jObject;

	// TJSONObject *jObject = new TJSONObject();
	jObject = new TJSONObject();

	TJSONObject *jPoint = new TJSONObject();
	TJSONObject *jDataPoint;
	//
	// Создадим главную ветку и добавим узел об устройстве





	// CheckStoragePermission_(this);

	if (!FlowMeters.empty()) {

			k=  FlowMeters.size();
			while (l < FlowMeters.size())
			{
			if ((FlowMeters[l]->SerialNum=="")&&(FlowMeters[l]->DataPoints.empty()))
			{

				  TFlowMeter::FlowMeters.erase(TFlowMeter::FlowMeters.begin()+l);

			}   else
			{
				 l=l+1;
			}

			}


		jObject->AddPair(L"email", TSettingsClass::eMail);

		jDevices = new TJSONArray();
		jObject->AddPair(L"poverkas",jDevices);


		for (int j = 0; j < FlowMeters.size(); j++) {

		if (TSettingsClass::DataDestination==4)
		  {
			  FlowMeters[j]->Data1 = TSettingsClass::TempData;
          }


		if (((FlowMeters[j]->Result == L"Годен")||(FlowMeters[j]->Result ==
				 L"Не годен"))&&(FlowMeters[j]->SendStatus != 2)) {

		FlowMeters[j]->SendStatus=1;

	// TJSONObject *jObject = new TJSONObject();
		jDevice = new TJSONObject();

		TJSONObject *jPoint = new TJSONObject();
		TJSONObject *jDataPoint;
	//
	// Создадим главную ветку и добавим узел об устройстве

	jDevice->AddPair(new TJSONPair(L"order_id", new TJSONNumber(StrToInt_(FlowMeters[j]->ID_Order))));
   //	  jDevice->AddPair(new TJSONPair(L"order_id", new TJSONNumber(900)));

	jDevice->AddPair(L"mi_owner", FlowMeters[j]->miOwner);
	jDevice->AddPair(L"active", IntToStr(FlowMeters[j]->Active));
	jDevice->AddPair(L"device_type", FlowMeters[j]->DeviceType);
	jDevice->AddPair(L"manufacture_num", FlowMeters[j]->SerialNum);
	jDevice->AddPair(L"mitype_number", FlowMeters[j]->CertificateNum);

	jDevice->AddPair(L"Title", TSettingsClass::Title);


	jDevice->AddPair(L"doc_title", FlowMeters[j]->docTitle);
	jDevice->AddPair(L"Adress", FlowMeters[j]->Adress);
	jDevice->AddPair(L"AllMeans", TSettingsClass::AllMeans);

   //!!!Верное	jDevice->AddPair(L"ProtocolNum", TSettingsClass::ProtocolNumTitle+IntToStr(TSettingsClass::ProtocolNum++));
	jDevice->AddPair(L"ProtocolNum", new TJSONNumber(TSettingsClass::ProtocolNum++));


	jDevice->AddPair(L"means", TSettingsClass::Means);

	jDevice->AddPair(L"modification", FlowMeters[j]->Modification);
	jDevice->AddPair(new TJSONPair(L"dn", new TJSONNumber(StrToInt_(FlowMeters[j]->DN))));
	jDevice->AddPair(L"next_verification", FlowMeters[j]->validDate);
	jDevice->AddPair(L"vrf_date", FlowMeters[j]->vrfDate);
	jDevice->AddPair(L"check_type", "2");//IntToStr(FlowMeters[j]->CheckType));
	jDevice->AddPair(L"kp", FloatToStr(FlowMeters[j]->Kp));
	jDevice->AddPair(L"q_max", FloatToStr(FlowMeters[j]->Qmax));
	jDevice->AddPair(L"temperature", FlowMeters[j]->temperature);
	jDevice->AddPair(L"temp_water", FlowMeters[j]->tempWater);
	jDevice->AddPair(L"pressure", FlowMeters[j]->pressure);
	jDevice->AddPair(L"hymidity", FlowMeters[j]->hymidity);
	jDevice->AddPair(L"result", FlowMeters[j]->Result);
	jDevice->AddPair(L"year_production", FlowMeters[j]->year_production);
	jDevice->AddPair(L"data1", FlowMeters[j]->Data1);
	jDevice->AddPair(L"data2", FlowMeters[j]->Data2);
	jDevice->AddPair(L"data3", FlowMeters[j]->Data3);
	jDevice->AddPair(new TJSONPair(L"doc_number", new TJSONNumber(StrToInt_(FlowMeters[j]->doc_number))));
	jDevice->AddPair(L"sign_cipher", TSettingsClass::SignCipher);
	jDevice->AddPair(L"porveritel_fio", TSettingsClass::Performer);


			jPoints = new TJSONArray();

			//jDevice->AddPair(L"data_points",IntToStr((int) Points.size()));
			jDevice->AddPair(L"data_points",jPoints);



			if (!FlowMeters[j]->DataPoints.empty()) {

			vector <tDataPoint>  dataPoints;

			dataPoints = FlowMeters[j]->SortDataVector(FlowMeters[j]->UsedDataPoints, -1,0,0);

				for (int i = 0; i < dataPoints.size(); i++) {




				 jPoint = new TJSONObject();




				   fl = dataPoints[i].Q/1000;
				   jPoint->AddPair(L"q", FloatToStrF(fl, ffNumber,
						10, 5));

					jPoint->AddPair(L"time",
						FloatToStrF(dataPoints[i].Time, ffNumber,
						10, 0));
					jPoint->AddPair(L"imp",
						FloatToStrF(dataPoints[i].Imp, ffNumber,
						10, 0));

					fl =  dataPoints[i].EtlVolume/1000;

					jPoint->AddPair(L"etl_volume", FloatToStrF(fl, ffNumber,10, 5));

					fl = dataPoints[i].Volume/1000;

					jPoint->AddPair(L"volume", FloatToStrF(fl, ffNumber, 10, 5));

					fl = dataPoints[i].VolumeBefore/1000;

					jPoint->AddPair(L"volume_before",
						FloatToStrF(fl, ffNumber, 10, 5));

					 fl = dataPoints[i].VolumeAfter/1000;

					jPoint->AddPair(L"volume_after",
						FloatToStrF(fl, ffNumber, 10, 5));



					jPoint->AddPair(L"temp",
						FloatToStrF(dataPoints[i].Temp, ffNumber,
						10, 2));
					jPoint->AddPair(L"pres",
						FloatToStrF(dataPoints[i].Pres, ffNumber,
						10, 2));
					jPoint->AddPair(L"point_humidity",
						FloatToStrF(dataPoints[i].Humidity, ffNumber,
						10, 2));
					jPoint->AddPair(L"temp_air",
						FloatToStrF(dataPoints[i].TempAir, ffNumber,
						10, 2));
					jPoint->AddPair(L"data",
						" ");
					jPoint->AddPair(L"error",
						FloatToStrF(dataPoints[i].Error, ffNumber,
						10, 2));

				   jPoints->AddElement(jPoint);
				}

			}


		jDevices->AddElement(jDevice);

		}
	 }

	}  else
	{

	}

	 return jObject->ToString();

}



UnicodeString TFlowMeter::JSonConvert()
{

	bool success = true;
	int k=0, l = 0;
	XmlDoc = nullptr;
	rootNode = nullptr;
	sampleNode = nullptr;

	 TJSONArray *jDevices;
	 TJSONArray *jPoints;

	TJSONArray *jDataPoints;
	 TJSONObject * jDevice;
	 TJSONObject *jObject;

	// TJSONObject *jObject = new TJSONObject();
	 jObject = new TJSONObject();

	 TJSONObject *jPoint = new TJSONObject();
	 TJSONObject *jDataPoint;
	//
	// Создадим главную ветку и добавим узел об устройстве
	 jObject->AddPair(L"email", TSettingsClass::eMail);
	jObject->AddPair(new TJSONPair(L"order_id", new TJSONNumber(StrToInt_(ID_Order))));
	jObject->AddPair(L"mi_owner", miOwner);
	jObject->AddPair(L"active", IntToStr(Active));
	jObject->AddPair(L"device_type", DeviceType);
	jObject->AddPair(L"manufacture_num", SerialNum);
	jObject->AddPair(L"mitype_number", CertificateNum);
	jObject->AddPair(L"doc_title", docTitle);
	jObject->AddPair(L"means", means);
	jObject->AddPair(L"modification", Modification);
	jObject->AddPair(new TJSONPair(L"dn", new TJSONNumber(StrToInt_(DN))));
	jObject->AddPair(L"next_verification", validDate);
	jObject->AddPair(L"vrf_date", vrfDate);
	jObject->AddPair(L"check_type", IntToStr(CheckType));
	jObject->AddPair(L"kp", FloatToStr(Kp));
	jObject->AddPair(L"q_max", FloatToStr(Qmax));
	jObject->AddPair(L"temperature", temperature);
	jObject->AddPair(L"temp_water", tempWater);
	jObject->AddPair(L"pressure", pressure);
	jObject->AddPair(L"hymidity", hymidity);
	jObject->AddPair(L"result", Result);
	jObject->AddPair(L"year_production", year_production);
	jObject->AddPair(L"data1", year_production);
	jObject->AddPair(L"data2", year_production);
	jObject->AddPair(L"data3", year_production);
	jObject->AddPair(L"doc_number", doc_number);
	jObject->AddPair(L"sign_cipher", sign_cipher);
	jObject->AddPair(L"porveritel_fio", porveritel_fio);


			jPoints = new TJSONArray();

			//jDevice->AddPair(L"data_points",IntToStr((int) Points.size()));
			jObject->AddPair(L"data_points",jPoints);

			if (!DataPoints.empty()) {
				for (int i = 0; i < Points.size(); i++) {

				 jPoint = new TJSONObject();

				   jPoint->AddPair(L"q", FloatToStrF(DataPoints[i].Q, ffNumber,
						10, 2));

					jPoint->AddPair(L"time",
						FloatToStrF(DataPoints[i].Time, ffNumber,
						10, 0));
					jPoint->AddPair(L"imp",
						FloatToStrF(DataPoints[i].Imp, ffNumber,
						10, 0));
					jPoint->AddPair(L"etl_volume",
						FloatToStrF(DataPoints[i].EtlVolume, ffNumber,
						10, 0));
					jPoint->AddPair(L"volume",
					FloatToStrF(DataPoints[i].Volume, ffNumber,
						10, 0));
					jPoint->AddPair(L"volume_before",
						FloatToStrF(DataPoints[i].VolumeBefore, ffNumber,
						10, 0));
					jPoint->AddPair(L"volume_after",
						FloatToStrF(DataPoints[i].VolumeAfter, ffNumber,
						10, 0));
					jPoint->AddPair(L"temp",
						FloatToStrF(DataPoints[i].Temp, ffNumber,
						10, 0));
					jPoint->AddPair(L"pres",
						FloatToStrF(DataPoints[i].Pres, ffNumber,
						10, 0));
					jPoint->AddPair(L"point_humidity",
						FloatToStrF(DataPoints[i].Humidity, ffNumber,
						10, 0));
					jPoint->AddPair(L"temp_air",
						FloatToStrF(DataPoints[i].TempAir, ffNumber,
						10, 0));
					jPoint->AddPair(L"data",
						" ");
					jPoint->AddPair(L"error",
						FloatToStrF(DataPoints[i].Error, ffNumber,
						10, 0));

				   jPoints->AddElement(jPoint);
				}

			}



	 return jObject->ToString();

}

int TFlowMeter::GetCountInOrder(int iD_Order)
{
	int i=0;

	for (int j = 0; j < FlowMeters.size(); j++) {

		if (FlowMeters[j]->ID_Order ==  iD_Order)
		{
			i=i+1;
		}

	}

	return i;
}


bool TFlowMeter::IsFlowInPoint(float Q)
{


		return (IsFlowInPoint( Q, Point));



}

bool TFlowMeter::IsFlowInPoint(float Q, tPoint point)
{
	float  Qp, Q1, Q2;
	tPoint pnt = point;
	Qp = pnt.Q;

	if ((point.RagePlus!=0)||(point.RageMinus!=0)) {
	Q1 = Qp - Qp * (point. RageMinus/100);
	Q2 = Qp + Qp * (point. RagePlus/100);
	}  else
	{
		Q1 = Qp - Qp * 0.1;
		Q2 = Qp + Qp * 0.1;
	}



	if ((Q1<Q)&&(Q2>Q))
	{
		return true;
	}   else
	{
		return false;
	}


}
#pragma package(smart_init)




