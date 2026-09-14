//---------------------------------------------------------------------------

#pragma hdrstop

#include "TDebug.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

	bool TDebug::active = false;

//---------------------------------------------------------------------------
TDebug::TDebug()
{
     MSG.Data = "!";

}

void TDebug::AddMSG (String LocData)
{

	  MSG.Time = Time();
	  MSG.msg =  LocData;
	// TEST !!!!!



		if (active)
	  {
		MSGS.push_back(MSG);
	  }

	  if (( IsSet!=0)&&(OnAddMsg!=nullptr)) {
		 OnAddMsg();
	  }


}



void TDebug::SetOnAddMSG (TOnAdd OnAddMSG)
{
	   IsSet = 1;
        OnAddMsg =  OnAddMSG;
}


UnicodeString TDebug::ViewMSG()
{
      String temp;
      temp = MSG.Direction+" "+DateTimeToStr(MSG.Time)+"::"+MSG.Data+" **"+MSG.Note;
      return temp;
}



