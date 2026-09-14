//---------------------------------------------------------------------------

#ifndef TOrderClassH
#define TOrderClassH

 #include <System.JSON.hpp>
#include "TFlowMeterType.h"
 #include <System.Math.hpp>

 #include <time.h>
 #include <System.DateUtils.hpp >



 class TOrderClass : public TObject
{
	  public:

	TOrderClass();
	virtual __fastcall ~TOrderClass();

	int ID_Order = 800;
	int Hash;

	 static void AddOrder(TOrderClass *Order);
	 void AddCurrOrder(tPoint Pnt);

	 static UnicodeString JSonConvert(TOrderClass *AOrder);
	 static  void StringConvert(UnicodeString string);

	static UnicodeString GetJsonValue(TJSONValue * JSONValue, UnicodeString Group, UnicodeString Class, int i,UnicodeString string);

	UnicodeString Performer; //	Наименование заявителя
	UnicodeString miOwner; //	Наименование заявителя
	UnicodeString Status;     // 0
	UnicodeString TelNumber;   //	Телефон заказчика


	UnicodeString Name; 		//	Имя заказчика
	UnicodeString Adress; 		//	Адрес заказчика

	UnicodeString AdTelNumber;
	UnicodeString AdressShrt;
	UnicodeString Enter;
	UnicodeString Floor;
	UnicodeString Flat;

	UnicodeString Price;

	UnicodeString meter_hot;
	UnicodeString meter_cold;
	UnicodeString meter_warm;
	UnicodeString meter_replace;
	UnicodeString is_moved;
	UnicodeString is_cancelled;

	UnicodeString IsCancelled;

	UnicodeString Date;			//	Дата исполнения
	UnicodeString Time1;		//	Время начала интервала
	UnicodeString Time2;		// Время окончания интервала
	UnicodeString Geo1, Geo2; //:"000.000, 000.0000",	Текущая геолокация
	UnicodeString Work;
	UnicodeString Data1;
	UnicodeString Data2;
	UnicodeString Data3;
	UnicodeString Comment;	//Комментарий

	int DevicesCount;	//Количество приборов в заказе

	int Active;



  UnicodeString GetStatus();
	void SetStatus( UnicodeString status);


	static void SaveToFile();
	static TOrderClass * LoadFromFile(void);
   //	static TOrderClass * LoadFromFile(TOrderClass *Order);

	static bool SetActive(TOrderClass *);
	static bool SetActive(int);
	static bool SetActiveHash(int);
	static TOrderClass * GetActiveOrder();
	static void SortOrders();


	static TOrderClass * TOrderClass::GetOrder(int);

	static void Delete(int);

	static vector <TOrderClass*> Orders;

	UnicodeString ApiConvert_editStatus();

	static TOrderClass *ActiveOrder  ;
	static UnicodeString dirName;
	static UnicodeString fname;

	private:

	static TXMLDocument *XmlDoc;
	static _di_IXMLNode rootNode;
	static _di_IXMLNode sampleNode;
	static _di_IXMLNode sampleNode2;
	static _di_IXMLNode sampleNode3;




};
//---------------------------------------------------------------------------
#endif
