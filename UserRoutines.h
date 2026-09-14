 //---------------------------------------------------------------------------

#ifndef UserRoutinesH
#define UserRoutinesH
//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
//#include "ustring.h"
#ifdef __ANDROID__
#include <AndroidApi.JNI.JavaTypes.hpp>
#include <Androidapi.Helpers.hpp>
#endif

#include <System.Permissions.hpp>
#include <System.SyncObjs.hpp>

	#include <System.DateUtils.hpp>
#include <System.SysUtils.hpp>

#include <System.Math.hpp>
#include <System.IOUtils.hpp>

#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>

#include <FMX.DialogService.hpp>
#include <FMX.PhoneDialer.hpp>


#ifndef __ANDROID__
#include "values.h"
#endif


#include <float.h>

using namespace std;

typedef void __fastcall (__closure * TOn)(TObject * Sender);

UnicodeString StrTrim_(UnicodeString Str);
float  StrToFloat_(UnicodeString Str);
 bool  TryStrToFloat_(UnicodeString Str, float &Flt);
 bool  TryStrToDouble_(UnicodeString Str, float &Flt);
bool CheckStoragePermission_(TObject *Sender) ;
 int  StrToInt_(UnicodeString Str);

bool  TryStrToInt_(UnicodeString Str, int &i);


 UnicodeString ToDayFGISFormat();

 UnicodeString  IncDateFGISFormat(int years);


bool TryDecodeDate_(UnicodeString Date, TDateTime &date, int L );
bool TryDecodeDate_(UnicodeString Date, TDateTime &date);
int IsInInterval(UnicodeString DateFrom, UnicodeString DateTo, UnicodeString Date);
UnicodeString GetAttribute(UnicodeString atr, _di_IXMLNode  rootNode);
int GetIntAttribute(UnicodeString atr, _di_IXMLNode  rootNode);

UnicodeString GetAttribute(UnicodeString atr, _di_IXMLNode  rootNode, UnicodeString bydefault);
int  GetIntAttribute(UnicodeString atr, _di_IXMLNode  rootNode, int bydefault);


void CheckPhonePermission(void);

#endif
