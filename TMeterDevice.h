//---------------------------------------------------------------------------

#ifndef TMeterDeviceH
#define TMeterDeviceH
//---------------------------------------------------------------------------


#include <list>
#include <vector>


#include "THSCDevice.h"
#include <System.Math.hpp>
#include <System.IOUtils.hpp>
#include "UserRoutines.h"

//---------------------------------------------------------------------------



#define XMLVER "2.0"

class TMeterDevice : public TObject
{
 public:

     TMeterDevice(void);
	 __fastcall ~TMeterDevice();


    int Hash;
	//
	// Тип поверяемого устройства
	UnicodeString DeviceType;
	//
	// Серийный номер поверяемого устройства
	UnicodeString SerialNum;
	//
	// Номер ГРСИ поверяемого устройства
	UnicodeString CertificateNum;

    static TXMLDocument *XmlDoc;
    static _di_IXMLNode rootNode;
  	static _di_IXMLNode sampleNode;
    static _di_IXMLNode sampleNode2;
    static _di_IXMLNode sampleNode3;
    static  UnicodeString dirName;
    static  System::UnicodeString fname;

    static void SaveToFile(void);
    static void LoadFromFile(void);

    void AddToList (void);

    static vector <TDevice*> Devicess;

    static std::vector<tPoint>::iterator it;


};





#endif
