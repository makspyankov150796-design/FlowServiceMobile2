//---------------------------------------------------------------------------

#pragma hdrstop

#include "UserRoutines.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)





int StrLen_(UnicodeString Str)
{

 return Str.Length();
}


UnicodeString ToDayFGISFormat()
{
TDateTime dt = Now();
	Word Year, Month, Day, Hour, Min, Sec, MSec;


  // Year = Year + 4;
  // dt = EncodeDate(Year, Month, Day);
	DecodeDate(dt, Year, Month, Day);
	return  IntToStr(Year)+"-"+IntToStr(Month)+"-"+IntToStr(Day)+"+0"+
		IntToStr(TTimeZone::Local->UtcOffset.Hours)+":00";


}

UnicodeString  IncDateFGISFormat(int years)
{
TDateTime dt = Now();
	Word Year, Month, Day, Hour, Min, Sec, MSec;




   dt = IncYear(dt, years);
  // Year = Year + 4;
  // dt = EncodeDate(Year, Month, Day);
	DecodeDate(dt, Year, Month, Day);

	return  IntToStr(Year)+"-"+IntToStr(Month)+"-"+IntToStr(Day)+"+0"+
		IntToStr(TTimeZone::Local->UtcOffset.Hours)+":00";


}


UnicodeString StrTrim_(UnicodeString Str)
{
	UnicodeString str;
    char chr = 160U;
    UnicodeString chr1 =  chr;

    str = "";

    //str = StringReplace(str,chr,"", TReplaceFlags()<< rfReplaceAll << rfIgnoreCase );
    //str = StringReplace(str," ","",TReplaceFlags()<< rfReplaceAll << rfIgnoreCase);

    int j=1, len;

    len = Str.Length();

	for (int i=1;i<=len;i++)
   {


        if (((Str[i]>='0')&&(Str[i]<='9'))||(Str[i]==','))
        {
           str.SetLength(j);
           str[j] = Str[i]; j++;
        }
    	else if (Str[i]=='.')
    	{
          str.SetLength(j);
          str[j] = ',';  j++;
    	}
    	else
    	{

    	}


    }

    if (str=="")
    { str = "0"; }

    return str;
}

float  StrToFloat_(UnicodeString Str)
  {
 	UnicodeString S;
 	float f;

		S = StrTrim_(Str);

		if (TryStrToFloat(S, f))
		{
            return f;
		}   else
        {
            return 1/0;
        }

        return 0;

  }

int StrToInt_(UnicodeString Str)
  {
	UnicodeString S;
	int f;

		S = StrTrim_(Str);

		if (TryStrToInt(S, f))
		{
            return f;
		}   else
        {
			return 0;
		}

		return 0;

  }

bool  TryStrToInt_(UnicodeString Str, int &i)
{
     	UnicodeString S;
	int f;

		S = StrTrim_(Str);

		if (TryStrToInt(S, i))
		{
			return true;
		}   else
        {
			return false;
		}

		return false;

}

bool TryStrToFloat_(UnicodeString Str, float &Flt)
  {
 	UnicodeString S;
 	float f;

		S = StrTrim_(Str);

		if (TryStrToFloat(S, f))
		{
            Flt = f;
            return true;
		}   else
        {
			Flt = 1/0;
			return false;
		}

		return false;

  }


bool TryStrToDouble_(UnicodeString Str, float &Flt)
  {
 	UnicodeString S;
	float f;

		S = StrTrim_(Str);

		if (TryStrToFloat(S, f))
		{
			Flt = f;
            return true;
		}   else
        {
			Flt = 1/0;
			return false;
		}

		return false;

  }

bool CheckStoragePermission_(TObject *Sender)
{
  #ifdef __ANDROID__
 /*
  DynamicArray<String> permissions;
  permissions.Length = 1;
  permissions[0] = JStringToString(TJManifest_permission::JavaClass->WRITE_EXTERNAL_STORAGE);

  PermissionsService()->RequestPermissions(permissions,
		[Sender](const DynamicArray<String> APermissions,
			   const DynamicArray<TPermissionStatus> AGrantResults)
	{
	  if ((AGrantResults.Length == 1) and(AGrantResults[0] == TPermissionStatus::Granted))
	  {
		return  true;
	  }
	  else
	  {
		return false;
	  }
	} );
  */
	   #endif

  return true;
}

 bool TryDecodeDate_(UnicodeString Date, TDateTime &date)
{
	TDateTime date1;
   Word Year, Month, Day;
  int Y;

	if(TryStrToDate(Date, date))
	{
	return true;
	}
	else if (TryStrToInt_(Date, Y))
	{
	Year = Y;

	if ((Y>2000)&&(Y<2100)) {
	Month = 1;
	Day = 12;
	date = EncodeDate(Year,Month,Day);
	return true;
	} else
{
	 return false;
}
	}

	else
	{

	 return false;
	}

		 return false;
}

 bool TryDecodeDate_(UnicodeString Date, TDateTime &date, int L )
{
	TDateTime date1;
   Word Year, Month, Day;
  int Y;

	if(TryStrToDate(Date, date))
	{
	return true;
	}
	else if (TryStrToInt_(Date, Y))
	{
	Year = Y;

	if ((Y>1990)&&(Y<2100)) {

	if (L==0) {
	Month = 1;
	Day = 12;
	}       else
	{
	Month = 12;
	Day = 29;
    }

	date = EncodeDate(Year,Month,Day);
	return true;
	} else
{
	 return false;
}
	}

	else
	{

	 return false;
	}

		 return false;
}



int IsInInterval(UnicodeString DateFrom, UnicodeString DateTo, UnicodeString Date)
{
 TDateTime date11, date22, date1, date2;


  int  CompResult1,CompResult2;

	if (TryDecodeDate_(DateFrom, date1, 0))
	if (TryDecodeDate_(DateTo, date2, 1))
	if (TryDecodeDate_(Date, date11, 1))
	if (TryDecodeDate_(Date, date22, 0))
	{

	  CompResult1 = CompareDate(date11,date1);
	  CompResult2 = CompareDate(date2,date22);

	  if ((CompResult1==1)&&(CompResult2==1)) {
		  return 1;
	  }  else
	   {
		   return -1;
       }
	}

		  return 0;
}



UnicodeString GetAttribute(UnicodeString atr, _di_IXMLNode  rootNode)
{

		if (rootNode->HasAttribute(atr)) {
		  return rootNode->Attributes[atr];
		}

return L"";
}


UnicodeString GetAttribute(UnicodeString atr, _di_IXMLNode  rootNode, UnicodeString bydefault )
{

		if (rootNode->HasAttribute(atr)) {
		  return rootNode->Attributes[atr];
		}

return bydefault;
}


int GetIntAttribute(UnicodeString atr, _di_IXMLNode  rootNode)
{
   int i=0;

		if (rootNode->HasAttribute(atr)) {

		   if (TryStrToInt(rootNode->Attributes[atr], i)) {
			 return i;
		   } else
		   {
			return -1;
		   }

		}

return -2;
}


int GetIntAttribute(UnicodeString atr, _di_IXMLNode  rootNode, int bydefault)
{
   int i=0;

		if (rootNode->HasAttribute(atr)) {

		   if (TryStrToInt(rootNode->Attributes[atr], i)) {
			 return i;
		   } else
		   {
			return -1;
		   }

		} else
		{
		   return bydefault;
        }

return -2;
}

void CheckPhonePermission(void)
{
#ifdef __ANDROID__
  /*
  DynamicArray<String> permissions;
  permissions.Length = 1;
 // CALL_PHONE
   permissions[0] = JStringToString(TJManifest_permission::JavaClass->CALL_PHONE);

  //permissions[1] = JStringToString(TJManifest_permission::JavaClass->MANAGE_OWN_CALLS);
  //permissions[2] = JStringToString(TJManifest_permission::JavaClass->MANAGE_ONGOING_CALLS);

  PermissionsService()->RequestPermissions(permissions,
		[this](const DynamicArray<String> APermissions,
			   const DynamicArray<TPermissionStatus> AGrantResults)
    {
	  if ((AGrantResults.Length == 1) and(AGrantResults[0] == TPermissionStatus::Granted))
	  {
	   // storagePremissionGranted = true;
	  }
	  else
	  {
	   //	storagePremissionGranted = false;
	  }
	} );   */
#endif

#ifndef __ANDROID__
  // storagePremissionGranted = true;
#endif

	}
