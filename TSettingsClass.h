//---------------------------------------------------------------------------

#ifndef TSettingsClassH
#define TSettingsClassH

#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>

#include "UserRoutines.h"
//#include "TFlowMeter.h"
//---------------------------------------------------------------------------

#define XMLVER "4.0"

class TSettingsClass : public TObject
{
	  public:

	TSettingsClass();

	virtual __fastcall ~TSettingsClass();

//	static vector <tCalibrPoint>  CalibrPoints;

	static  UnicodeString Performer;
	static  UnicodeString OrgName;

	static  UnicodeString StopSoundFile;
	static  UnicodeString UserDir;

	static int   IDPerformer;
	static int 		IDOrg;

	//static int 	Dimension;

	static  UnicodeString eMail;
	static  UnicodeString SignCipher;

	static  UnicodeString DocNum;
	static  UnicodeString Means;

	static  UnicodeString AllMeans;
	static  UnicodeString Title;

	static  UnicodeString Package;

	static  int IsDocNumIndividual;
	static  int IsAdressIndividual;
	static  int IsCheckFill;

	static  UnicodeString Dir;
	static  UnicodeString Units;

	static  UnicodeString TempData;

	static  int DataDestination;

	static  int status;

	static  int IsOrdersEnabled;
	static  int IsUsePeriod;

	static  UnicodeString ProtocolNumTitle;
	static  int	ProtocolNum ;

	static void SaveToFile(void);
	static void LoadFromFile(void);

	static  int EtalonCHNum;
	static  int TestCHNum;



	private:

	static TXMLDocument *XmlDoc;
	static _di_IXMLNode rootNode;
	static _di_IXMLNode sampleNode;
	static _di_IXMLNode sampleNode2;
	static _di_IXMLNode sampleNode3;







};




#endif
