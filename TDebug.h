//---------------------------------------------------------------------------

#ifndef TDebugH
#define TDebugH
//---------------------------------------------------------------------------


#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Objects.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.ScrollBox.hpp>

#include <vector>
//#include <System.SysUtils>
//---------------------------------------------------------------------------
//Функция обратного вызова при добавлении нового сообщения

typedef void __fastcall(__closure * TOnAdd)();            //




typedef struct TMSG
{
   UnicodeString msg;
   TDateTime Time;
   UnicodeString Note;
   UnicodeString Direction;
   UnicodeString Data;
   UnicodeString Type;

} MSGdef;

class TDebug //: public TObject
{
public:
	 TDebug();
	 ~TDebug();

	 int8_t state;
	 int8_t data [4];

	 int16_t Int;
	 float Float;
	 int32_t IntL;
	 String Str;

	 void SetOnAddMSG (TOnAdd OnAdd);


	 int IsSet = 0;
	 TMSG MSG;

	std::vector <TMSG> MSGS;

	 void AddMSG (String Data);
	 UnicodeString ViewMSG();

	static bool active ;

	TOnAdd OnTransfer;
	TOnAdd OnTransferCmplt;
	TOnAdd OnRecieve;
	TOnAdd OnRecieveCmplt;
	TOnAdd OnError;



private:	// User declarations
	TOnAdd OnAddMsg;


};

extern TDebug *Debug ;

 #endif

