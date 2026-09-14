//---------------------------------------------------------------------------

#ifndef TFlowMeterTypeH
#define TFlowMeterTypeH

#include <list>
#include <vector>


#include "THscDevice.h"

#include "UserRoutines.h"
#include "TSettingsClass.h"

//---------------------------------------------------------------------------

typedef struct TPoint
{
  uint8_t   Index;
  float     Q;                //Заданный расход
  float     Qrate;            //Часть от Qmax
  System::UnicodeString   Name;
  uint8_t   Num;                 //Кол-во измерений
  uint8_t   State;         //<Состоние
							 //	0 - нет данных
                             // 3 - есть данные, но они плохие
                             // 5 - есть данные для передачи


  int16_t 	Time;          //Время измерения
  int32_t 	Imp;          //Ограничение измерения по импульсам
  float     Volume;     //Ограничение по объеъму

  float     Accuracy;   //Точность выхода на расход

  float     RagePlus;     //Границы точки +
  float     RageMinus;   //Границы точки -
  uint8_t   IsRageFree; //Границы не фиксированы

  float     Error;      //Погрешность допустимая измерениы

  int16_t 	Pause;      //Время ожидания перед проливом

}tPoint;


typedef struct TDataPoint
{
	tPoint 	 *Point;
	uint8_t  Index;
	System::UnicodeString   Comment;
	System::UnicodeString   Name;
	float     Q;
	float     Qt;
	uint8_t   State;         //<Состоние
							 //	0 - нет данных
							//
							// 1 - точка не соответствует расходу
							// 2 - точка соответствует расходу, но не соответсвует время
							// 3 - точка соответствует расходу, но не соответсвует погрешность
							// 4 - точка соответствует расходу,  соответсвует время соответсвует погрешность
							// 5- точка для финального отчета

  	float	  Time;          //Время измерения
  	float	  Imp;           //Кол-во импульсов
  	float     EtlVolume;     //Объем эталона

  	float     Volume;        //Объем прибора
	float    VolumeBefore;        //Объем прибора
	float    VolumeAfter;        //Объем прибора

	float     Temp;         //Температура
	float     Pres;         //Давление

	float     TempAir;         //Температура
	float     PresAir;         //Давление
	float     Humidity;         //Давление

	System::UnicodeString   Date;  //дата и время измерения

    float     Error;      //Погрешность  измерениы

	int IsUsed=0; // точка Использовалась

    TDateTime DateTime;


}tDataPoint;






class TFlowMeterType : public TObject
{
 public:

     TFlowMeterType(void);
	 __fastcall ~TFlowMeterType();

	bool Visible;

    int Hash;
	//
	// Тип поверяемого устройства
	UnicodeString DeviceType;

	//
	// Тип поверяемого устройства
	UnicodeString DeviceName;

	UnicodeString Name;
	//
	// Серийный номер поверяемого устройства
	UnicodeString SerialNum;
	//
	// Номер ГРСИ поверяемого устройства
	UnicodeString CertificateNum;
    //Модификация
	UnicodeString Modification;
	//Методика поверки
	UnicodeString VerificationNum;

	UnicodeString DN;
    //Описание
	UnicodeString Description;
	//МежПоверочный интервал

	UnicodeString Date1;
	//МежПоверочный интервал

	UnicodeString Date2;
	//МежПоверочный интервал


	UnicodeString Manufacturer;
	//МежПоверочный интервал

		UnicodeString Data1;
	//МежПоверочный интервал

		UnicodeString Data2;
	//МежПоверочный интервал

		UnicodeString Data3;
	//МежПоверочный интервал

	UnicodeString  VerificationInterval1;
	UnicodeString  VerificationInterval2;
	//Метод поверки

	int CheckType;

        // Коэффициент преобразования
    float Kp;

    float Qmax;

    tPoint 					Point;
    vector <tPoint> 		Points;


	uint8_t AddPointData (float Qrate, float Q, float Volume, float vTime, float Error, float RM, float RP);

    void AddPoint(tPoint Pnt);
    void AddCurrPoint(tPoint Pnt);
    void ClearPoints(void);

    void AddPoints( vector <tPoint> Points);

    static TXMLDocument *XmlDoc;
    static _di_IXMLNode rootNode;
  	static _di_IXMLNode sampleNode;
    static _di_IXMLNode sampleNode2;
    static _di_IXMLNode sampleNode3;
	static  UnicodeString dirName;
    static  System::UnicodeString fname;

	static void SaveTypesToFile(void);
    static void LoadTypesFromFile(void);

	static void DuplicateType(TFlowMeterType* MeterType);
    //static void EraseType(TFlowMeterType* MeterType);

    void AddTypeToList (void);

	static TFlowMeterType* Get(int Hash);
	static TFlowMeterType* Get(UnicodeString hash);

	static  void Erase (int Hash);
	static  void Erase (UnicodeString hash);

	static bool filter;


	  static vector <TFlowMeterType*> MeterTypes;

	  static vector <int> FilterList;

      static std::vector<tPoint>::iterator it;


};



#endif
