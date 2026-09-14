//---------------------------------------------------------------------------

#ifndef TFlowMeterH
#define TFlowMeterH

 #include <System.JSON.hpp>
#include "TFlowMeterType.h"
#include <System.Math.hpp>

 #include <time.h>
  #include <System.DateUtils.hpp >

#include <vector>

#include "TOrderClass.h"
#include "TMeterValue.h"


#include <REST.Client.hpp>
#include <REST.Response.Adapter.hpp>
#include <REST.Types.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
 #include "UserRoutines.h"

#include "TSettingsClass.h"

//---------------------------------------------------------------------------

#define CHANNEL 2
#define XMLVERFLOWMETERS "5.0"




typedef struct TCalibrPoint
{
  	uint8_t   Index;

    tPoint 	*Point;
    float	  Q;             //расход эталонный, чтобы не считать каждый раз   СПРАВОЧНО
    float	  Qtest;          //расход поверяемого прибора, чтобы не считать каждый раз СПРАВОЧНО
   	float     EtlVolume;     //Объем эталона     ОСНОВНОЕ
  	float     Volume;        //Объем прибора     СПРАВОЧНО
  	float	  Time;          //Время измерения   ОСНОВНОЕ
  	float	  Imp;           //Кол-во импульсов  ОСНОВНОЕ
    float     Coef;  //Коэф                      ОСНОВНОЕ
 	uint8_t   State;         //<Состоние
  	System::UnicodeString   Date;  //дата и время измерения
    float     Error;      //Погрешность  измерениы СПРАВОЧНО
    float     Rate;      //Погрешность  измерениы СПРАВОЧНО

}tCalibrPoint;

     typedef struct TCoef
{
  	uint8_t   Index;

    float	  Q1;     //
    float	  Q2;     //
   	float     K;     //
  	float     b;    //


}tCoef;


class TFlowMeter : public TObject
{
	public:

	TFlowMeter(THscDevice *HSCDevice, int ID_Order, bool IsEtalon);
	TFlowMeter(THscDevice *HSCDevice, bool IsEtalon);
	TFlowMeter(bool IsEtalon);
	virtual __fastcall ~TFlowMeter();

        void SetHSC(THscDevice *HSCDevice) {HSCDevice = this->HSCDevice;};

    TFlowMeterType *Type;

	void SetType (TFlowMeterType *Type);
	bool SetType (int typeHash);

	int Hash;
	int DeviceHash;
    int TypeHash;

    bool IsEtalon;
    int  Active;

    int CheckType;

	int Status = 0; // 0 - Создан , 1 - Есть данные, 1 - Есть данные, не годен,
				// 2 - Есть данные, годен,
	UnicodeString GetStatus();

	int SendStatus=0;   // 0 - не отправлен 1- отправляется  2- отправлен
	UnicodeString GetSendStatus();

	   void	SetSendStatus(UnicodeString text);


	UnicodeString Name;

	//
	// Тип поверяемого устройства
	UnicodeString DeviceType;
    UnicodeString Modifications;
	UnicodeString Modification;

	int ID_Order;

	int  VerificationInterval;

	UnicodeString miOwner;
	//
	// Серийный номер поверяемого устройства
	UnicodeString SerialNum;
	//
	// Номер ГРСИ поверяемого устройства
	UnicodeString CertificateNum;

	UnicodeString DN;
	UnicodeString docTitle;

    UnicodeString Adress;

	UnicodeString doc_number;
	static UnicodeString sign_cipher;
	static UnicodeString porveritel_fio;

	UnicodeString means;

	UnicodeString year_production;

	UnicodeString K1,P1,K2,P2;

	UnicodeString tempWater, temperature, pressure, hymidity;

	UnicodeString vrfDate;
	UnicodeString validDate;

	UnicodeString Data1, Data2, Data3;

	UnicodeString Date1, Date2;

	UnicodeString Result = "-";

	TDateTime DateTime;    //дата и время измерения

	//
	// Коэффициент преобразования
	float Kp;

	//
	// Калибровочные коэфициенты
	float K[100];
	float Q[100];

	//Настройки прибора

	float Qmax;
	float Qmin;

	//Текущий расход
	float Flow (void);      // Расход с поправками
	float GetRawFlow(void); // Расход без поправок

	//Текущий объем
	float Volume(void);


	//

	uint8_t GetChannel(void);
	void SetChannel(uint8_t Channel);

	float GetImpKoef(void){return Kp;};
	float GetImpCoef(void){return Kp;};
	float GetKoef(void) {return Kp;};
	UnicodeString GetKoefStr(void) {return FloatToStr(Kp);};
	void SetKoef(float K) {Kp=K;};
	void SaveCoef(void);
	void SetImpKoef(float K){Kp=K;};


	float GetVolume(void);
	float GetFlow(void);
	float GetTime(void);
	float GetTotalImp(void);

	float GetRawWidthFlow(void);
	float GetWidthFlow(void);
	float GetWidthFlowVolume(void);

	float GetFinalVolume(void);
	float GetFinalRawVolume(void);

	float GetFinalFlow(void);
	float GetFinalRawFlow(void);

    int GetRawSecImp(void);

	void ResetTest(void);
	float GetFlowVolume(void);

	float GetFinalImp(void);

	void ReadPIN(void);
	void SavePIN(uint8_t pin);

	void RestoreTypePoints(void);


	void WritePIN(uint8_t pin);

	void  Write_Channel_State(uint16_t State);


	uint8_t AddPointData (UnicodeString Name, float Qrate, float Q, float Volume, float vTime, float Error, float RageMinus,float RagePlus);

	void AddPoint(tPoint Pnt);
	void AddCurrPoint(tPoint Pnt);
	void ClearPoints(void);

	tPoint GetCurrentPoint();
	tPoint SetNextPoint();
	tPoint SetPreviousPoint();



	void SaveDataPoint(TFlowMeter *Etalon);
	tDataPoint GetDataPoint(void) {return DataPoint;}
	void AddDataPoint(tPoint Pnt);



	int8_t AddCalibrPoint(tCalibrPoint Pnt);
	void ClearCalibrPoint(tCalibrPoint Pnt);
	void AddCurrentCalibrPoint(void);
	void AddDataCalibrPoint(void);
	float Rate(float Q);

	void ClearCalibraion(void);

	void SaveCalibrData(void);
	void LoadCalibrData(void);

	int8_t UpdateCoefs(void);

	int8_t AddCalibrData(float vEtlVolume, float vTime, float vImp, float vCoef);



	 tPoint 				Point;
	 tDataPoint 			DataPoint;
	 tCalibrPoint           CalibrPoint;
	 tCoef                  Coef;

	 vector <tPoint> 		Points;
	 vector <tDataPoint> 	DataPoints;
	 vector <tDataPoint> 	UsedDataPoints;
     vector <tCalibrPoint>  CalibrPoints;
     vector <tCoef>   		Coefs;

     // Обратные функции обновления состояния

     void  __fastcall OnConfigRead(TObject * Sender);

     int PointIndex;



    static TXMLDocument *XmlDoc;
    static _di_IXMLNode rootNode;
  	static _di_IXMLNode sampleNode;
    static _di_IXMLNode sampleNode2;
	static _di_IXMLNode sampleNode3;
	static  UnicodeString dirName;
	static  System::UnicodeString fname;


	static  TJSONValue *jValue;
	static  TJSONArray *jArray;
	static  TJSONObject *jObject ;

	static void SaveToFile(TFlowMeter *FlowMeter, int IsBackUp);
	static TFlowMeter * LoadFromFile(void);
	static TFlowMeter * LoadFromFile(THscDevice *HSCDevice);

	static TFlowMeter* GetDevice (int Hash);

	static UnicodeString JSonConvert(TFlowMeter *AFlowMeter);

	static UnicodeString ApiConvert();

	static void ApiSent();
	static  int ApiCheckResult();

	UnicodeString JSonConvert();


	UnicodeString StringStreamConvert();

	void AddToList (void);


	 static vector <TFlowMeter*> FlowMeters;
	 static std::vector<TFlowMeter*>::iterator fit;

	 static std::vector<tPoint>::iterator it;

	 static void StaticInit(THscDevice *HSCDev);
	 static  THscDevice *sHSC;
	 static  TFlowMeter *ActiveFlowMeter;

	 static bool InitType(TFlowMeter* FM);

	static int GetCountInOrder(int iD_Order);

	 TMeterValue *FlowValue;
	 TMeterValue *VolumeValue;
	 TMeterValue *ImpValue;
	 TMeterValue *VolumeErrorValue;

	 void  SetValues(void);
	 void  SetMonitorValues(void);
	 void  SetFinalValues(void);

	 float GetVolumeError();

	 void SetEtalon(TFlowMeter*);
	 void SetAsEtalon(void);
	 bool IsFlowInPoint(float Q);
	 bool IsFlowInPoint(float Q, tPoint point);
	 bool IsDataPointGood(tDataPoint &dataPoint, tPoint point);

	void SortDataPoints(int maxmin);
    vector <tDataPoint> SortDataVector(vector <tDataPoint> vect, int State,int State2,int maxmin);

	 bool GetPointUseFlow(float Q, tPoint &point);

	 int TimeToEndVolumeLimit(float pointVolume, float DelayCoef, float TimeCoef);

	 int CheckStatus();
	 int CheckFullStatus();
	 void IncImpSumMonitor(void);



  protected:

  private:

     uint8_t Channel;
     THscDevice *HSCDevice, *HSC;

	 TFlowMeter* EtalonMeter;

     //архив последних 100 секунд импульсов
     uint16_t Impulses[100];
     uint8_t wr_imp; // индекс записи в архив
     uint8_t rd_imp; // индекс чтения из архива

     float VolSum, ImpSum;

     void Init(bool IsEtalon);


 };



#endif
