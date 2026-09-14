//---------------------------------------------------------------------------

#ifndef FormDebugH
#define FormDebugH
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
#include <System.SysUtils.hpp>

#include "THscDevice.h"

#include "TDebug.h"
#include <FMX.Ani.hpp>
#include <FMX.Effects.hpp>
#include <vector>
//#include <System.SysUtils>
//---------------------------------------------------------------------------
//Функция обратного вызова при добавлении нового сообщения
 /*
typedef void __fastcall(__closure * TOnAddMSG)(TObject * Sender);

class TMSG
{
public:
	TMSG();
	__fastcall ~TMSG();


    UnicodeString msg(void);

   TDateTime Time;
   String Note;
   String Direction;
   String Data;

private:	// User declarations



};

class TDebug : public TObject
{
public:
	 __fastcall  TDebug();
	 __fastcall ~TDebug();

     void SetOnAddMSG (TOnAddMSG OnAddMSG);

     TMSG MSG;

    std::vector <TMSG> MSGS;

     void AddMSG (String Data);

private:	// User declarations
     TOnAddMSG OnAddMsg;

};
*/
//--------------------------------------

class TDebugForm : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TButton *Button1;
	TButton *Button2;
	TLayout *Layout1;
	TPanel *Panel1;
	TLayout *Layout2;
	TRectangle *Rectangle1;
	TLabel *Label1;
	TImage *Image2;
	TComboBox *DirectionComboBox;
	TLayout *Layout4;
	TRectangle *Rectangle2;
	TEdit *AdrEdit;
	TLabel *PeriodButton;
	TImage *Image3;
	TLayout *Layout5;
	TRectangle *Rectangle7;
	TEdit *Edit2;
	TLabel *Label4;
	TImage *Image7;
	TLayout *Layout10;
	TRectangle *Rectangle8;
	TLayout *Layout6;
	TPanel *Panel2;
	TLayout *Layout8;
	TRectangle *Rectangle5;
	TImage *Image5;
	TLayout *Layout9;
	TRectangle *Rectangle6;
	TImage *Image6;
	TLayout *Layout3;
	TRectangle *Rectangle3;
	TEdit *Edit3;
	TLabel *Label3;
	TImage *Image4;
	TStyleBook *StyleBook1;
	TMemo *DebugMemo;
	TButton *CleanButton;
	TCheckBox *CheckBoxReadData;
	TCheckBox *CheckBoxWriteData;
	TCheckBox *CheckBox3;
	TCheckBox *CheckBoxSys;
	TCheckBox *CheckBoxDataOnly;
	TCheckBox *CheckBoxPreFrame;
	TButton *PlayButton;
	TButton *ButtonRefresh;
	TButton *PauseButton;
	TLabel *Label2;
	TLabel *Label5;
	TLabel *Label7;
	TLabel *Label11;
	TCheckBox *SetOnCheckBox;
	TEdit *DataWriteEdit;
	TListBoxItem *ListBoxItem1;
	TListBoxItem *ListBoxItem2;
	TEdit *DataReadEdit;
	TComboBox *DataReadComboBox;
	TListBoxItem *ListBoxItem3;
	TListBoxItem *ListBoxItem4;
	TListBoxItem *ListBoxItem5;
	TComboBox *DataWriteComboBox;
	TListBoxItem *ListBoxItem6;
	TListBoxItem *ListBoxItem7;
	TListBoxItem *ListBoxItem8;
	TButton *Button3;
	TListBoxItem *ListBoxItem9;
	TListBoxItem *ListBoxItem10;
	TLabel *Label6;
	TEllipse *Light1;
	TGlowEffect *GlowGreen1;
	TEllipse *Circle;
	TColorAnimation *AnimGreen1;
	TColorAnimation *AnimGreen2;
	TLabel *Label8;
	TEllipse *Ellipse1;
	TGlowEffect *GlowYellow1;
	TEllipse *Ellipse2;
	TColorAnimation *AnimYellow1;
	TColorAnimation *AnimYellow2;
	TLabel *Label9;
	TEllipse *Ellipse3;
	TGlowEffect *GlowRed1;
	TEllipse *Ellipse4;
	TColorAnimation *AnimRed1;
	TColorAnimation *AnimRed2;
	TTimer *Timer1;
	TTimer *Timer2;
	TTimer *Timer3;
	TCheckBox *CheckBoxActive;
	TImage *Image8;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CleanButtonClick(TObject *Sender);
	void __fastcall PauseButtonClick(TObject *Sender);
	void __fastcall PlayButtonClick(TObject *Sender);
	void __fastcall SetOnCheckBoxChange(TObject *Sender);
	void __fastcall ButtonRefreshClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall DirectionComboBoxChange(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall DataReadComboBoxChange(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Label6Click(TObject *Sender);
	void __fastcall Label8Click(TObject *Sender);
	void __fastcall Label9Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall CheckBoxActiveChange(TObject *Sender);
private:	// User declarations

		 THscDevice *HSC;

	void __fastcall DisplayMSG (TMSG MSG);

public:		// User declarations
	__fastcall TDebugForm(TComponent* Owner);

   void __fastcall OnAddMSGForm();

   void __fastcall OnRecieve();
   void __fastcall OnRecieveCmplt();
   void __fastcall OnTransfer();
   void __fastcall OnTransferCmplt();
   void __fastcall OnError();

	   void SetHscDevice(THscDevice *hsc) {TDebugForm::HSC = hsc;};

  // TDebug *Debug;
};
//---------------------------------------------------------------------------
extern PACKAGE TDebugForm *DebugForm;
//---------------------------------------------------------------------------
#endif
