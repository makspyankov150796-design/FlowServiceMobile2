//---------------------------------------------------------------------------

#ifndef FormTypeSelectionH
#define FormTypeSelectionH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <System.Rtti.hpp>


#include "TFlowMeterType.h"
#include "TFlowMeter.h"
#include <FMX.Layouts.hpp>
#include <FMX.Menus.hpp>
#include <FMX.Edit.hpp>

#include <System.SysUtils.hpp>

#include <System.Character.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.SearchBox.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.ListView.Adapters.Base.hpp>
#include <FMX.ListView.Appearances.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>

typedef void __fastcall (__closure * TOnUpdate)(TObject * Sender);

//---------------------------------------------------------------------------
class TTypeSelectionForm : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TStringGrid *StringGrid1;
	TStringColumn *StringColumn2;
	TStringColumn *StringColumn3;
	TStringColumn *StringColumn4;
	TLayout *Layout27;
	TButton *ApplyButton;
	TButton *DeleteConfigButton;
	TButton *LoadConfigButton;
	TButton *SaveConfigButton;
	TPanel *Panel1;
	TButton *Button2;
	TButton *MenuButton;
	TPopupMenu *PopupMenu1;
	TMenuItem *LoadTypesMenuItem;
	TMenuItem *SaveTypesMenuItem;
	TMenuItem *DeleteTypesMenuItem;
	TButton *CancelButton;
	TMenuItem *CopyTypeMenuItem;
	TEdit *EditSearch;
	TStringColumn *StringColumn1;
	TStringColumn *StringColumn5;
	TStringColumn *StringColumn6;
	TStringColumn *StringColumn7;
	TStringColumn *StringColumn8;
	TButton *RefreshButton;
	TButton *Button1;
	TStringColumn *HASHStringColumn;
	TStringColumn *StringColumnValidFrom;
	TStringColumn *StringColumn10;
	TStringColumn *StringColumn11;
	TStringColumn *StringColumn12;
	TStringColumn *StringColumn13;
	TStringColumn *StringColumn14;
	TTabControl *TabControl1;
	TTabItem *TabItem1;
	TTabItem *TabItem2;
	TStringColumn *StringColumnValidTo;
	TEdit *EditYear;
	TListView *ListView1;
	TLabel *Label1;
	TTimer *TimerFilter;
	TPanel *Panel2;
	TLayout *Layout1;
	TButton *Button3;
	TButton *Button7;
	TStyleBook *StyleBook1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall DeleteConfigButtonClick(TObject *Sender);
	void __fastcall LoadConfigButtonClick(TObject *Sender);
	void __fastcall ApplyButtonClick(TObject *Sender);
	void __fastcall LoadTypesMenuItemClick(TObject *Sender);
	void __fastcall DeleteTypesMenuItemClick(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall CopyTypeMenuItemClick(TObject *Sender);
	void __fastcall MenuButtonClick(TObject *Sender);
	void __fastcall SaveTypesMenuItemClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall RefreshButtonClick(TObject *Sender);
	void __fastcall SaveConfigButtonClick(TObject *Sender);
	void __fastcall StringGrid1EditingDone(TObject *Sender, const int ACol, const int ARow);
	void __fastcall EditSearchKeyUp(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall ListView1Change(TObject *Sender);
	void __fastcall TabItem1Click(TObject *Sender);
	void __fastcall TimerFilterTimer(TObject *Sender);
	void __fastcall EditSearchKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall EditSearchExit(TObject *Sender);
	void __fastcall StringGrid1SelChanged(TObject *Sender);
	void __fastcall TabItem2Click(TObject *Sender);
	void __fastcall ListView1ItemClick(TObject * const Sender, TListViewItem * const AItem);
	void __fastcall EditYearExit(TObject *Sender);


private:	// User declarations

	 TFlowMeter *FlowMeter;
	 TOnUpdate OnUpdate;
	 UnicodeString Str;
	 int InitHashTag;
	 int HashTag;

public:		// User declarations
	__fastcall TTypeSelectionForm(TComponent* Owner);


	void UpdateGrid(void);
	void 	  UpdateList(void);

	void SetFlowMeter(TFlowMeter *Meter);
	void SetFlowMeterType();

    void SetOnUpdate(TOnUpdate OnUpdate);
	void Refresh ();
    void __fastcall FillListView();
};
//---------------------------------------------------------------------------
extern PACKAGE TTypeSelectionForm *TypeSelectionForm;
//---------------------------------------------------------------------------
#endif
