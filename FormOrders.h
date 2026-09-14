//---------------------------------------------------------------------------

#ifndef FormOrdersH
#define FormOrdersH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.ListView.Adapters.Base.hpp>
#include <FMX.ListView.Appearances.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.Types.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <System.JSON.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Data.Bind.ObjectScope.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <Fmx.Bind.Editors.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.ScrollBox.hpp>
#include <REST.Client.hpp>
#include <REST.Response.Adapter.hpp>
#include <REST.Types.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <System.Net.HttpClient.hpp>
#include <System.Net.HttpClientComponent.hpp>
#include <System.Net.URLClient.hpp>

#include "TFlowMeter.h"
#include <FMX.Ani.hpp>
#include <FMX.Effects.hpp>
#include <FMX.ImgList.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Objects.hpp>
#include <System.ImageList.hpp>

#include "TOrderClass.h"
#include "FormSettings.h"
#include <FMX.TabControl.hpp>

#include "TMainForm.h"
#include <FMX.ListBox.hpp>
#include <FMX.WebBrowser.hpp>


//---------------------------------------------------------------------------
class TOrdersForm : public TForm
{
__published:	// IDE-managed Components
	TStyleBook *StyleBook1;
	TPanel *Panel1;
	TButton *ButtonEnter;
	TButton *CancelButton;
	TEllipse *Light1;
	TGlowEffect *Glow1;
	TEllipse *Circle;
	TColorAnimation *Anim1;
	TColorAnimation *Anim2;
	TImageList *ImageList1;
	TButton *ButtonCall;
	TGridPanelLayout *GridPanelLayout1;
	TButton *ButtonCancel;
	TListView *ListViewOrders_;
	TNetHTTPClient *NetHTTPClientOrders;
	TTabControl *TabControl;
	TTabItem *TabItem1;
	TTabItem *TabItem2;
	TMemo *Memo1;
	TTabItem *TabItem3;
	TListBox *ListBox1;
	TListBoxGroupHeader *ListBoxGroupHeader1;
	TListBoxItem *ListBoxItemName;
	TListBoxItem *ListBoxItemAdress;
	TListBoxItem *ListBoxItemTel;
	TListBoxItem *ListBoxItemEnter;
	TListBoxGroupHeader *ListBoxGroupHeader2;
	TTabItem *TabItem4;
	TWebBrowser *WebBrowser;
	TListBoxItem *ListBoxItemFloor;
	TListBoxItem *ListBoxItemApart;
	TListBoxItem *ListBoxItemDate;
	TListBoxItem *ListBoxItem1;
	TListBoxItem *ListBoxItem2;
	TListBoxItem *ListBoxItem3;
	TListBoxItem *ListBoxItem4;
	TListBoxItem *ListBoxItem5;
	TListBoxItem *ListBoxItem6;
	TMemo *MemoComment;
	TListBoxGroupHeader *ListBoxGroupHeader3;
	TListBoxItem *ListBoxItemPrice;
	TListBoxItem *ListBoxItem8;
	TListBoxItem *ListBoxItemMoved;
	TListBoxItem *ListBoxItemCancelled;
	TLabel *LabelPrice;
	TLabel *LabelHotMeter;
	TLabel *LabelColdMeter;
	TLabel *LabelReplace;
	TLabel *LabelWarm;
	TLabel *LabelEntrance;
	TLabel *LabelFloor;
	TLabel *LabelFlat;
	TLabel *LabelPriceTotal;
	TLabel *LabelMoved;
	TLabel *LabelCancelled;
	TLabel *LabelElse;
	TToolBar *ToolBar1;
	TLabel *Label1;
	TToolBar *ToolBar3;
	TSpeedButton *SpeedButton2;
	TLabel *Label2;
	TToolBar *ToolBar4;
	TSpeedButton *SpeedButton3;
	TLabel *Label3;
	TToolBar *ToolBar5;
	TSpeedButton *SpeedButton4;
	TLabel *Label4;
	TLabel *LabelDate;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button5;
	TButton *Button6;
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall ButtonRefreshClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ListViewOrders_ItemClick(TObject * const Sender, TListViewItem * const AItem);
	void __fastcall ButtonCallClick(TObject *Sender);
	void __fastcall ListViewOrders_Paint(TObject *Sender, TCanvas *Canvas, const TRectF &ARect);
	void __fastcall ButtonEnterClick(TObject *Sender);
	void __fastcall ButtonCancelClick(TObject *Sender);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall NetHTTPClientOrdersRequestCompleted(TObject * const Sender, IHTTPResponse * const AResponse);
	void __fastcall SpeedButton3Click(TObject *Sender);
	void __fastcall SpeedButton4Click(TObject *Sender);
	void __fastcall ListViewOrders_ButtonClick(TObject * const Sender, TListItem * const AItem,
          TListItemSimpleControl * const AObject);




private:	// User declarations

       TDateTime DTime;
public:		// User declarations
	__fastcall TOrdersForm(TComponent* Owner);

	void UpdateList();
	void UpdateListBox();
	int  HashTag = 0;
	UnicodeString TelNumber;
	void Refresh();
};
//---------------------------------------------------------------------------
extern PACKAGE TOrdersForm *OrdersForm;
//---------------------------------------------------------------------------
#endif
