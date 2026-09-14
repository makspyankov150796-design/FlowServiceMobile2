// ---------------------------------------------------------------------------

#ifndef TMainFormH
#define TMainFormH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <System.Bluetooth.Components.hpp>
#include <System.Bluetooth.hpp>
#include <System.DateUtils.hpp>
#include <FMX.Gestures.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.TabControl.hpp>
#include <FMX.Types.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.Objects.hpp>
#include <FMX.ScrollBox.hpp>
#include <System.Rtti.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.ListView.Adapters.Base.hpp>
#include <FMX.ListView.Appearances.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.Media.hpp>
#include <System.Math.Vectors.hpp>
#include <System.Rtti.hpp>

#include <FMX.Platform.hpp>
#include <FMX.TreeView.hpp>
#include <FMX.MultiView.hpp>
#include <FMX.Menus.hpp>

#include <FMX.Dialogs.hpp>
#include <FMX.Platform.hpp>
#include <FMX.DialogService.hpp>
#include <System.UITypes.hpp>

#include <time.h>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.EngExt.hpp>
#include <FMX.Ani.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <FMX.ComboEdit.hpp>
#include <FMX.Effects.hpp>
#include <FMX.ImgList.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <System.Beacon.Components.hpp>
#include <System.Beacon.hpp>
#include <System.ImageList.hpp>
#include <System.Net.HttpClient.hpp>
#include <System.Net.HttpClientComponent.hpp>
#include <System.Net.URLClient.hpp>
#include <vector>

#include <fmx.h>
#include <FMX.Ani.hpp>
#include <FMX.Effects.hpp>
#include <System.Net.HttpClient.hpp>
#include <System.Net.HttpClientComponent.hpp>
#include <System.Net.URLClient.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <System.Beacon.Components.hpp>
#include <System.Beacon.hpp>
#include <FMX.ComboEdit.hpp>
#include <FMX.SearchBox.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <FMX.ImgList.hpp>
#include <System.ImageList.hpp>
#pragma hdrstop

#include <FMX.DialogService.Async.hpp>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"

#ifndef __ANDROID__
#include <FMX.DialogService.Sync.hpp>
#endif

#ifdef __ANDROID__
#include <AndroidApi.JNI.JavaTypes.hpp>
#include <Androidapi.Helpers.hpp>


#include <FMX.Platform.Android.hpp>
#include <Androidapi.JNI.JavaTypes.hpp>
#include <Androidapi.JNI.GraphicsContentViewText.hpp>
#include <Androidapi.Helpers.hpp>
//#include "System.Win.ComObj.hpp"

#endif

#include <System.Permissions.hpp>
#include <System.SyncObjs.hpp>
#include <System.JSON.hpp>

#include "THscDevice.h"
#include "TFlowMeter.h"

#include "UserRoutines.h"

#include "FormSettings.h"
#include "TDebug.h"
#include "FormOrders.h"




// #include <Printers.hpp>
// ---------------------------------------------------------------------------
#define BLUETOOTH_DISCOVER_TIME_MS  15000

#define BT_TXFIFO_SIZE  4096  //< Размер FIFO передачи для Bluetooth в байтах
#define BT_RXFIFO_SIZE  4096  //< Размер FIFO приема Bluetooth в байтах
// ---------------------------------------------------------------------------

//
// Объект класса устройства HSC
THscDevice *HSC;
TFlowMeter *EtalonMeter;
TFlowMeter *TestMeter;
// TOrderClass *ActiveOrder;

String str[12];

static System::Uitypes::TModalResult Result;

// ---------------------------------------------------------------------------
class TMainTabbedForm : public TForm {
__published: // IDE-managed Components
	TStyleBook *StyleBook1;
	TTabControl *MainTabs;
	TTabItem *TabItemCondition;
	TLayout *Layout14;
	TPanel *Panel4;
	TLayout *Layout16;
	TRectangle *Rectangle8;
	TEdit *TempEdit;
	TLabel *Label19;
	TImage *Image8;
	TLayout *Layout17;
	TRectangle *Rectangle9;
	TEdit *PressureTemp;
	TLabel *Label20;
	TImage *Image9;
	TLayout *Layout18;
	TRectangle *Rectangle10;
	TEdit *HumidityEdit;
	TLabel *Label21;
	TImage *Image10;
	TLabel *Label22;
	TLayout *Layout19;
	TPanel *Panel5;
	TLayout *Layout20;
	TRectangle *Rectangle11;
	TEdit *TempWaterEdit;
	TLabel *Label23;
	TImage *Image11;
	TLayout *Layout21;
	TRectangle *Rectangle12;
	TEdit *PresWaterEdit;
	TLabel *Label24;
	TImage *Image12;
	TLayout *Layout22;
	TRectangle *Rectangle13;
	TEdit *DensityEdit;
	TLabel *Label25;
	TImage *Image13;
	TLabel *Label26;
	TToolBar *ToolBar2;
	TButton *Button8;
	TButton *Button9;
	TTabItem *TabItemDevice;
	TToolBar *V;
	TButton *AddTypeButton;
	TButton *RefreshDeviceButton;
	TLayout *MesurmentMeanLayout;
	TPanel *Panel1;
	TLayout *Layout1;
	TRectangle *Rectangle1;
	TLabel *Label4;
	TLayout *Layout2;
	TRectangle *Rectangle2;
	TEdit *CertificateNumEdit;
	TLabel *Label1;
	TImage *Image3;
	TLayout *LayoutSerialNumber;
	TRectangle *Rectangle3;
	TEdit *SerialNumEdit;
	TLabel *Label2;
	TImage *Image4;
	TLabel *Label5;
	TLayout *Layout5;
	TTabItem *TabItemCheck;
	TToolBar *ToolBar1;
	TSwitch *SwitchConnection1;
	TLabel *LabelConnection1;
	TLayout *Layout6;
	TPanel *Panel2;
	TLayout *Layout7;
	TRectangle *Rectangle4;
	TEdit *TimeEdit;
	TLabel *Label6;
	TLabel *LabelTime;
	TImage *Image1;
	TLayout *Layout8;
	TRectangle *Rectangle5;
	TEdit *VolumeEdit;
	TLabel *LabelLimit;
	TLabel *LabelVolume;
	TImage *Image5;
	TLayout *Layout10;
	TButton *PreviousPointButton;
	TButton *NextPointButton;
	TLabel *PointLabel;
	TLayout *Layout9;
	TRectangle *Rectangle6;
	TEdit *FlowEdit;
	TLabel *LabelFlowRate;
	TLabel *LabelFlow;
	TImage *Image6;
	TLabel *Label9;
	TLayout *LayoutCheckType;
	TPanel *Panel3;
	TLayout *Layout15;
	TLayout *Layout13;
	TButton *ButtonMonitor;
	TTabItem *TabItemResults;
	TTabItem *TabItemMain;
	TToolBar *ToolBar3;
	TButton *Button12;
	TButton *ButtonSendData;
	TEdit *Edit13;
	TToolBar *ToolBar4;
	TButton *Button21;
	TButton *TypeMenuButton;
	TButton *DebugButton;
	TButton *DevicesButton;
	TCameraComponent *Camera;
	TGestureManager *GestureManager1;
	TStringGrid *StringGridTest;
	TStringColumn *StringColumn10;
	TStringColumn *StringColumn11;
	TStringColumn *StringColumn12;
	TStringColumn *StringColumn13;
	TProgressBar *ProgressBar;
	TLayout *Layout24;
	TButton *RefreshButton;
	TLayout *Layout25;
	TStringGrid *PointsStringGrid;
	TStringColumn *StringColumn1;
	TStringColumn *StringColumn2;
	TStringColumn *StringColumn3;
	TStringColumn *StringColumn4;
	TButton *Button6;
	TButton *CameraButton;
	TLabel *Label10;
	TStringColumn *StringColumn6;
	TLayout *Layout26;
	TLayout *Layout28;
	TRectangle *Rectangle15;
	TEdit *QmaxEdit;
	TLabel *SynchInLabel;
	TImage *Image15;
	TLayout *LayoutCoef;
	TRectangle *Rectangle16;
	TEdit *KpEdit1;
	TLabel *LabelCoef;
	TImage *Image16;
	TLayout *Layout31;
	TLabel *Z;
	TLayout *Layout30;
	TLabel *Label16;
	TVertScrollBox *VertScrollBox1;
	TButton *DelPointsButton;
	TButton *SavePointsButton;
	TButton *LoadPointsButton;
	TButton *ModifyTypeButton;
	TButton *Button7;
	TStringColumn *StringColumn14;
	TButton *ButtonDelPoint;
	TButton *TypeSelectButton;
	TPopupMenu *PopupMenu1;
	TMenuItem *AddTypeMenuItem;
	TMenuItem *ModifyTypeMenuItem;
	TMenuItem *RefreshDeviceMenuItem;
	TButton *AddDeviceButton;
	TLayout *Layout32;
	TLayout *Layout33;
	TLayout *Layout34;
	TButton *RefreshDevicesButton;
	TButton *DeleteConfigButton;
	TButton *DeleteRowButton;
	TButton *LoadDevicesButton;
	TButton *SaveDevicesButton;
	TLayout *Layout35;
	TLayout *Layout36;
	TLabel *LabelVer;
	TStringGrid *DataPointsGrid;
	TLayout *Layout37;
	TButton *Button1;
	TButton *ClearDataButton;
	TButton *DeleteRowDataButton;
	TStringColumn *StringColumn15;
	TStringColumn *StringColumn16;
	TStringColumn *StringColumn17;
	TStringColumn *StringColumn18;
	TStringColumn *StringColumn19;
	TPopupMenu *PopupMenu2;
	TMenuItem *AddDataPoint;
	TMenuItem *MenuItem2;
	TMenuItem *MenuItem3;
	TButton *DeviceSelectButton;
	TButton *Button2;
	TButton *Button5;
	TExpander *Expander2;
	TButton *TestButton;
	TEllipse *Light1;
	TGlowEffect *Glow1;
	TEllipse *Circle;
	TColorAnimation *Anim1;
	TColorAnimation *Anim2;
	TLayout *Layout38;
	TButton *Button22;
	TButton *AddTypeButn;
	TButton *ModifyTypeButn;
	TNetHTTPClient *NetHTTPClient1;
	TButton *CancelButton;
	TProgressBar *ChargeProgressBar;
	TLabel *ChargeLabel;
	TLayout *Layout29;
	TStringGrid *OrdersStringGrid;
	TStringColumn *IDOrderStringColumn;
	TStringColumn *StringColumn21;
	TStringColumn *StringColumn23;
	TLayout *Layout39;
	TButton *RefreshOrders;
	TButton *ButtonDeleteOrder;
	TButton *Button25;
	TButton *Button26;
	TBeaconDevice *BeaconDevice1;
	TStringColumn *StringColumn24;
	TExpander *OrdersExpander;
	TStringColumn *StringColumn22;
	TStringColumn *StringColumn25;
	TStringColumn *NameStringColumn;
	TStringColumn *StatusStringColumn;
	TButton *Button27;
	TComboEdit *ComboEdit1;
	TButton *AddOrder;
	TStringColumn *CommentStringColumn;
	TButton *BindButton;
	TTabItem *Tests;
	TButton *Button20;
	TEdit *Edit1;
	TLabel *Label7;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TEdit *Edit5;
	TLabel *Label14;
	TButton *Button23;
	TButton *Button28;
	TMemo *Memo2;
	TButton *Button29;

	TMediaPlayer *MediaPlayer1;
	TBluetoothLE *BluetoothLE1;
	TLayout *Layout40;
	TRectangle *Rectangle17;
	TLabel *Label18;
	TLayout *Layout41;
	TRectangle *Rectangle18;
	TEdit *EditModification;
	TLabel *Label27;
	TImage *Image19;
	TImage *Image18;
	TComboEdit *ComboEditTypeName;
	TEdit *EditDeviceType;
	TTimer *TimerFilter;
	TButton *ButtonComboDown;
	TVertScrollBox *VertScrollBox2;
	TButton *AddButton;
	TButton *Button3;
	TNetHTTPRequest *NetHTTPRequest1;
	TBindingsList *BindingsList1;
	TLabel *Label30;
	TLabel *Label31;
	TButton *Button24;
	TLabel *Label33;
	TButton *Button30;
	TLabel *Label29;
	TMemo *Memo4;
	TLabel *Label32;
	TLabel *Label28;
	TEdit *Edit6;
	TMemo *Memo3;
	TLabel *Label34;
	TButton *ButtonJSON;
	TLayout *LayoutMesurementTool;
	TLayout *Layout43;
	TRectangle *Rectangle21;
	TLayout *LayoutYearProduction;
	TRectangle *Rectangle22;
	TEdit *EditYear;
	TLabel *Label37;
	TImage *Image22;
	TLayout *Layout45;
	TLabel *Label38;
	TLayout *Layout11;
	TRectangle *Rectangle19;
	TEdit *EditYears;
	TLabel *Label35;
	TImage *Image20;
	TImage *Image21;
	TButton *ButtonInterval2;
	TLabel *Label36;
	TButton *ButtonInterval1;
	TLayout *Layout27;
	TRectangle *Rectangle20;
	TLabel *Label12;
	TComboBox *ComboBoxType;
	TImage *Image17;
	TLayout *Layout23;
	TRectangle *Rectangle14;
	TLabel *Label3;
	TImage *Image14;
	TEdit *TestMeterKoefEdit;
	TLayout *Layout3;
	TRectangle *Rectangle7;
	TEdit *EditVerificationInterval;
	TLabel *Label17;
	TImage *Image7;
	TLayout *Layout12;
	TRectangle *Rectangle23;
	TEdit *EditDN;
	TLabel *Label39;
	TButton *Button31;
	TImage *Image23;
	TLayout *LayoutTool;
	TLayout *Layout4;
	TLabel *Label40;
	TEdit *Edit7;
	TLayout *Layout42;
	TLabel *Label41;
	TEdit *Edit8;
	TButton *Button32;
	TLabel *Label42;
	TLayout *LayoutHandle;
	TComboBox *ComboBoxUnits;
	TEdit *EditAfter;
	TEdit *EditBefore;
	TLabel *LabelAfter;
	TLabel *LabelBefore;
	TLabel *LabelDevice;
	TLayout *LayoutCommon;
	TLayout *LayoutDoc;
	TRectangle *Rectangle24;
	TEdit *EditDoc;
	TLabel *Label43;
	TButton *Button14;
	TButton *Button33;
	TImage *Image24;
	TLayout *LayoutAdress;
	TRectangle *Rectangle25;
	TLabel *Label44;
	TImage *Image25;
	TLayout *Layout49;
	TLabel *Label45;
	TMemo *MemoAdress;
	TLayout *LayoutPrice;
	TRectangle *Rectangle26;
	TEdit *Edit9;
	TLabel *Label46;
	TButton *Button34;
	TButton *Button35;
	TImage *Image26;
	TButton *ButtonCancelStatus;
	TIdHTTP *IdHTTP1;
	TIdSSLIOHandlerSocketOpenSSL *IdSSLIOHandlerSocketOpenSSL1;
	TExpander *Expander3;
	TListView *ListViewOrders_;
	TButton *ButtonDownLoadOrders_;
	TNetHTTPClient *NetHTTPClientOrders;
	TTimer *Timer1;
	TLayout *Layout46;
	TRectangle *Rectangle27;
	TLabel *Label47;
	TImage *Image27;
	TComboEdit *ComboEditOrders;
	TLabel *Label8;
	TLayout *Layout47;
	TPanel *Panel6;
	TListBox *ListBox1;
	TListBoxItem *ListBoxItem1;
	TListBoxGroupHeader *ListBoxGroupHeader1;
	TListBoxGroupHeader *ListBoxGroupHeader2;
	TListBoxItem *ListBoxItemDevice;
	TListBoxItem *ListBoxItemCheck;
	TListBoxItem *ListBoxItemResults;
	TListBoxGroupHeader *Настройки;
	TListBoxItem *ListBoxItemSettings;
	TListBoxItem *ListBoxItemConnections;
	TListBoxItem *ListBoxItemVersion;
	TListBoxItem *ListBoxItemOrders;
	TSwitch *SwitchConnection2;
	TLabel *LabelConnection;
	TListBoxItem *ListBoxItemConditions;
	TButton *ButtonTypeDetails;
	TButton *Button4;
	TButton *Button15;
	TLabel *Label11;
	TPanel *Panel7;
	TPanel *Panel8;
	TStyleBook *StyleBook2;
	TVertScrollBox *VertScrollBox4;
	TListBoxItem *ListBoxItem2;
	TComboBox *ComboBoxType1;
	TLabel *LabelOrder;
	TLabel *LabelDeviceNumber;
	TImage *ImageConnectOff;
	TImageList *ImageList;
	TImage *ImageConnectOn;
	TImage *ImageConnectProcces;
	TComboEdit *ComboEditModification;
	TLang *Lang1;
	TButton *ButtonDeleteDeviceName;
	TImage *ImageConnectOff1;
	TImage *ImageConnectOn1;
	TImage *ImageConnectProcces1;
	TTabItem *TabItemTestBLE;
	TLabel *LabelConnect;
	TLabel *Label48;
	TLabel *Label49;
	TLabel *Label50;
	TLabel *LabelStatus;
	TLabel *LabelRequest;
	TLabel *LabelAnswer;
	TMemo *MemoConnect;
	TLabel *Label15;
	TTimer *TimerError;
	TStringGrid *DevicesGrid;
	TStringColumn *StringColumn20;
	TStringColumn *StringColumn26;
	TStringColumn *StringColumn27;
	TStringColumn *StringColumn28;
	TStringColumn *StringColumn29;
	TStringColumn *StringColumn30;
	TDateTimeColumn *DateTimeColumn2;
	TStringColumn *StringColumn31;
	TButton *Button10;
	TEdit *Edit10;
	TLabel *Label51;
	TStringGrid *StringGrid1;
	TStringColumn *StringColumn5;
	TStringColumn *StringColumn7;
	TLabel *Label52;
	TMemo *Memo1;
	TCheckBox *CheckBoxData;
	TButton *Button11;
	TLabel *Label13;
	TButton *Button13;
	TStringColumn *StringColumn8;
	TStringColumn *StringColumn9;
	TLayout *Layout44;
	TRectangle *Rectangle28;
	TLabel *Label53;
	TImage *Image2;
	TLayout *Layout48;
	TMediaPlayer *MediaPlayer2;
	TEdit *KpEdit2;
	TButton *Button16;
	TStringColumn *StringColumn32;
	TStringColumn *StringColumn33;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall SwitchConnection1Click(TObject *Sender);
	void __fastcall Button11Click(TObject *Sender);
	void __fastcall AddTypeButtonClick(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall DevicesButtonClick(TObject *Sender);
	void __fastcall TestButtonClick(TObject *Sender);
	void __fastcall TimeEditKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall NextPointButtonClick(TObject *Sender);
	void __fastcall RefreshDeviceButtonClick(TObject *Sender);
	void __fastcall DebugButtonClick(TObject *Sender);
	void __fastcall SwitchConnection1Switch(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CameraButtonClick(TObject *Sender);
	void __fastcall Button23Click(TObject *Sender);
	void __fastcall ButtonClick(TObject *Sender);
	void __fastcall TempEditKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall PressureTempKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall HumidityEditKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall TempWaterEditKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall PresWaterEditKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall DensityEditKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall ButtonMonitorClick(TObject *Sender);
	void __fastcall RefreshButtonClick(TObject *Sender);
	void __fastcall SavePointsButtonClick(TObject *Sender);
	void __fastcall DelPointsButtonClick(TObject *Sender);
	void __fastcall QmaxEditKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall KpEdit1KeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall QmaxEditChange(TObject *Sender);
	void __fastcall PointsStringGridEditingDone(TObject *Sender, const int ACol,
		const int ARow);
	void __fastcall ButtonDelPointClick(TObject *Sender);
	void __fastcall DeleteRowDataButtonClick(TObject *Sender);
	void __fastcall TypeSelectButton2Click(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ModifyTypeButtonClick(TObject *Sender);
	void __fastcall PointsStringGridSelectCell(TObject *Sender, const int ACol,
		const int ARow, bool &CanSelect);
	void __fastcall PreviousPointButtonClick(TObject *Sender);
	void __fastcall LoadPointsButtonClick(TObject *Sender);
	void __fastcall AddTypeMenuItemClick(TObject *Sender);
	void __fastcall ModifyTypeMenuItemClick(TObject *Sender);
	void __fastcall RefreshDeviceMenuItemClick(TObject *Sender);
	void __fastcall SerialNumEditKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall SerialNumEditExit(TObject *Sender);
	void __fastcall TabItemResultsClick(TObject *Sender);
	void __fastcall DataPointsGridSelectCell(TObject *Sender, const int ACol,
		const int ARow, bool &CanSelect);
	void __fastcall AddDataPointClick(TObject *Sender);
	void __fastcall DevicesGrid3SelChanged(TObject *Sender);
	void __fastcall SaveDevicesButtonClick(TObject *Sender);
	void __fastcall AddDeviceButtonClick(TObject *Sender);
	void __fastcall LoadDevicesButtonClick(TObject *Sender);
	void __fastcall DeleteRowButtonClick(TObject *Sender);
	void __fastcall DeleteConfigButtonClick(TObject *Sender);
	void __fastcall RefreshDevicesButtonClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall ClearDataButtonClick(TObject *Sender);
	void __fastcall Image5DblClick(TObject *Sender);
	void __fastcall VolumeEditValidate(TObject *Sender, UnicodeString &Text);
	void __fastcall VolumeEditKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall ComboBoxTypeChange(TObject *Sender);
	void __fastcall DeviceSelectButtonClick(TObject *Sender);
	void __fastcall TimeEditExit(TObject *Sender);
	void __fastcall Button22Click(TObject *Sender);
	void __fastcall Button12Click(TObject *Sender);
	void __fastcall PointLabelDblClick(TObject *Sender);
	void __fastcall PointLabelGesture(TObject *Sender,
		const TGestureEventInfo &EventInfo, bool &Handled);
	void __fastcall TypeMenuButtonClick(TObject *Sender);
	void __fastcall EditNameKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall EditNameExit(TObject *Sender);
	void __fastcall CertificateNumEditKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall CertificateNumEditExit(TObject *Sender);
	void __fastcall ModifyTypeButnClick(TObject *Sender);
	void __fastcall LabelLimitGesture(TObject *Sender,
		const TGestureEventInfo &EventInfo, bool &Handled);
	void __fastcall VolumeEditExit(TObject *Sender);
	void __fastcall EditBeforeKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall EditAfterKeyDown(TObject *Sender, WORD &Key,
		System::WideChar &KeyChar, TShiftState Shift);
	void __fastcall EditBeforeExit(TObject *Sender);
	void __fastcall EditBeforeValidate(TObject *Sender, UnicodeString &Text);
	void __fastcall EditAfterValidate(TObject *Sender, UnicodeString &Text);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall TabItemCheckClick(TObject *Sender);
	void __fastcall EditAfterExit(TObject *Sender);
	void __fastcall ButtonSendDataClick(TObject *Sender);
  //	void __fastcall KpEdit2Change(TObject *Sender);
	void __fastcall Memo1DblClick(TObject *Sender);
	void __fastcall AddOrderClick(TObject *Sender);
	void __fastcall RefreshOrdersClick(TObject *Sender);
	void __fastcall OrdersStringGridSelChanged(TObject *Sender);
	void __fastcall OrdersExpanderExpandedChanged(TObject *Sender);
	void __fastcall Button27Click(TObject *Sender);
	void __fastcall Button26Click(TObject *Sender);
	void __fastcall BindButtonClick(TObject *Sender);
	void __fastcall Edit1Exit(TObject *Sender);
	void __fastcall Edit2Exit(TObject *Sender);
	void __fastcall Button28Click(TObject *Sender);
	void __fastcall Button20Click(TObject *Sender);
	void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall Image5Click(TObject *Sender);
	void __fastcall Edit3Exit(TObject *Sender);
	void __fastcall Edit5Exit(TObject *Sender);
	void __fastcall TimerStopTimer(TObject *Sender);
	void __fastcall EditDeviceTypeExit(TObject *Sender);
	void __fastcall EditModificationExit(TObject *Sender);
	void __fastcall ComboEditTypeNameExit(TObject *Sender);
	void __fastcall ComboEditTypeNameChange(TObject *Sender);
	void __fastcall ComboEditTypeNameKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall ComboEditTypeNameEnter(TObject *Sender);
	void __fastcall TimerFilterTimer(TObject *Sender);
	void __fastcall ButtonComboDownClick(TObject *Sender);
	void __fastcall ButtonDeleteOrderClick(TObject *Sender);
	void __fastcall AddButtonClick(TObject *Sender);
	void __fastcall NetHTTPClient1RequestCompleted(TObject * const Sender, IHTTPResponse * const AResponse);
	void __fastcall Button24Click(TObject *Sender);
	void __fastcall Edit6KeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall Button30Click(TObject *Sender);
	void __fastcall ButtonGetDataClick(TObject *Sender);
	void __fastcall ButtonJSONClick(TObject *Sender);
	void __fastcall EditYearExit(TObject *Sender);
	void __fastcall TabItemDeviceClick(TObject *Sender);
	void __fastcall ButtonInterval1Click(TObject *Sender);
	void __fastcall EditVerificationIntervalExit(TObject *Sender);
	void __fastcall ButtonInterval2Click(TObject *Sender);
	void __fastcall EditDNExit(TObject *Sender);
	void __fastcall ComboBoxUnitsChange(TObject *Sender);
	void __fastcall Button32Click(TObject *Sender);
	void __fastcall EditYearsExit(TObject *Sender);
	void __fastcall ButtonCancelStatusClick(TObject *Sender);
	void __fastcall EditDocExit(TObject *Sender);
	void __fastcall MemoAdressExit(TObject *Sender);
	void __fastcall MainTabsChange(TObject *Sender);
	void __fastcall NetHTTPRequest1RequestError(TObject * const Sender, const UnicodeString AError);
	void __fastcall TabItemConditionClick(TObject *Sender);
	void __fastcall ButtonDownLoadOrdersClick(TObject *Sender);
	void __fastcall ButtonDownLoadOrders_Click(TObject *Sender);
	void __fastcall ListViewOrders_ItemClick(TObject * const Sender, TListViewItem * const AItem);
	void __fastcall NetHTTPClientOrdersRequestCompleted(TObject * const Sender, IHTTPResponse * const AResponse);
	void __fastcall ListViewOrders_ButtonClick(TObject * const Sender, TListItem * const AItem,
          TListItemSimpleControl * const AObject);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall ComboEditOrdersKeyUp(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall ComboEditOrdersChange(TObject *Sender);
	void __fastcall ListBoxItemDeviceClick(TObject *Sender);
	void __fastcall ListBoxItemCheckClick(TObject *Sender);
	void __fastcall ListBoxItemConditionsClick(TObject *Sender);
	void __fastcall ListBoxItemResultsClick(TObject *Sender);
	void __fastcall ListBoxItemVersionClick(TObject *Sender);
	void __fastcall ButtonTypeDetailsClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall ListBoxItemSettingsClick(TObject *Sender);
	void __fastcall ListBoxItemConnectionsClick(TObject *Sender);
	void __fastcall НастройкиClick(TObject *Sender);
	void __fastcall SwitchConnection2Switch(TObject *Sender);
	void __fastcall SwitchConnection2Click(TObject *Sender);
	void __fastcall ListBoxItemOrdersClick(TObject *Sender);
	void __fastcall ComboBoxType1Change(TObject *Sender);
	void __fastcall TabItemMainPaint(TObject *Sender, TCanvas *Canvas, const TRectF &ARect);
	void __fastcall ComboEditOrdersGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall ToolBar3Gesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall TabItemResultsPaint(TObject *Sender, TCanvas *Canvas, const TRectF &ARect);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall AddTypeButnClick(TObject *Sender);
	void __fastcall ComboEditModificationChange(TObject *Sender);
	void __fastcall ButtonDeleteDeviceNameClick(TObject *Sender);
	void __fastcall TestMeterKoefEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall TimerErrorTimer(TObject *Sender);
	void __fastcall Button10Click(TObject *Sender);
	void __fastcall StringGrid1DrawColumnCell(TObject *Sender, TCanvas * const Canvas,
          TColumn * const Column, const TRectF &Bounds, const int Row,
          const TValue &Value, const TGridDrawStates State);
	void __fastcall DataPointsGridDrawColumnCell(TObject *Sender, TCanvas * const Canvas,
          TColumn * const Column, const TRectF &Bounds, const int Row,
          const TValue &Value, const TGridDrawStates State);
	void __fastcall Button13Click(TObject *Sender);
	void __fastcall KpEdit1Change(TObject *Sender);
 /*	void __fastcall KpEdit2KeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall KpEdit2Enter(TObject *Sender);   */
	void __fastcall KpEdit1Enter(TObject *Sender);
	void __fastcall KpEdit2Change(TObject *Sender);
	void __fastcall KpEdit2Enter(TObject *Sender);
	void __fastcall KpEdit2KeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall Button16Click(TObject *Sender);








private: // User declarations

	void __fastcall OnSecDataRead(TObject *Sender);
	void __fastcall OnImpDataRead(TObject *Sender);
	void __fastcall OnDataRead(TObject *Sender);
	void __fastcall OnFinalDataRead(TObject *Sender);
	void __fastcall OnChargeRead(TObject *Sender);
	void __fastcall OnWidthRead(TObject *Sender);

	float temp[300][5];

	Integer inte = 0;

	void __fastcall  UpdateDevicesGrid();

	 void __fastcall  SaveTestMeters();

	void UpdateList();
	int HashTag;
	#ifdef __ANDROID__
	void sendTextViaIntent(System::UnicodeString text);
	bool __fastcall handleAppEvent(TApplicationEvent appEvent, TObject *context);
	void __fastcall handleActivityMessage(System::TObject* const Sender, TMessageBase* const m);


	bool __fastcall handleIntentAction(Androidapi::Jni::Graphicscontentviewtext::_di_JIntent data);
	#endif
public: //
    void UpdateDeviceChange();
	__fastcall TMainTabbedForm(TComponent* Owner);
	void __fastcall     SetTestMeterOrder ();
	static TTimer *TimerStop;

	void RefreshCond(int delta);

	void __fastcall StopMesurment(void);
	void __fastcall StartMesurment(void);
	void __fastcall StopMonitor(void);
	void __fastcall StartMonitor(void);

	void __fastcall SetMonitorSettings(int checktype);
	void __fastcall SetMesurmentSettings(int checktype);
;

	void __fastcall DataReadFormsUpdate(void);
	void __fastcall DataReadFormsFinalUpdate(void);

	// Имена полей таблицы измерений
	static const UnicodeString GridFieldName[4];

	bool btPermissionGranted; // < Разрешение на доступ к Bluetooth модулю
	bool camPermissionGranted; // < Разрешение на доступ к камере
	bool storagePremissionGranted; // < Разрешение на доступ к хранилищу

	 void TryFilesDowmload();

	void SwitchConnection(bool state);
	void __fastcall ConnectionUp(TObject * Sender);

	void MesureDataUpdate(void);

	void AfterSpillage(void);
	void AfterHandSpillage(void);

	bool need;
	//
	// Текущее состояние устройства
	HscStates_t deviceState;
	//
	// Структура значений условий проведения испытаний
	Conditions_t Conditions;
	//
	// Сюда будем класть очередные принятые данные измерений
	TotalData_t mData;
	//
	// Заданное время измерения
	uint16_t measureTime;

	//
	// Функция запроса доступа к модулю Bluetooth
	void CheckBtPermission(void);
	//
	// Функция запроса доступа к камере
	void CheckCamPermission(void);
	//
	// Проверка наличия доступа к хранилищу
	void CheckStoragePermission(void);
	//
	// Функция подгона размеров элементов управления вкладки "Подключение"
	void __fastcall TabConnectionResize(void);
	//
	// Функция подгона размеров элементов управления вкладки "Условия"
	void __fastcall TabConditionsResize(void);
	//
	// Функция подгона размеров элементов управления вкладки "Данные устройства"
	void __fastcall TabDeviceResize(void);
	//
	// Функция подгона размеров элементов управления вкладки "Данные устройства"
	void __fastcall TabVerificationResize(void);
	//
	// Вызывается по завершению сканирования Bluetooth
	void __fastcall Bluetooth1DiscoveryEnd(TObject * const Sender,
		TBluetoothDeviceList * const ADeviceList);
	//
	// Вызывается при превышении времени сканирования
	void __fastcall DiscoverWdtOverflow(TObject *Sender);
	//
	// Вызывается при подключении к Bluetooth устройству
	void __fastcall OnBluetoothConnectMain(TObject *Sender, bool connected);
	//
	// Вызывается при отключении устройства Bluetooth
	void __fastcall OnBluetoothDisconnect(TObject *Sender);
	//
	// Вызывается всякий раз, когда в классе HSC Device меняется состояние
	void __fastcall OnHscDeviceChangeState(TObject *Sender);

	void __fastcall OnConfigRead(TObject * Sender);

	void __fastcall  OnVersionRead(TObject *Sender);
	//
	// Функция, синхронизируемая, для обработки смены состояния устройства
	void __fastcall OnDeviceChangeState(void);
	//
	// Когда прилетели очередные данные во время измерения
	void __fastcall OnDataReceive(TObject *Sender, TotalData_t &Data);

	// Когда отправлен паект с необходимостью CallBack
	void __fastcall OnDataSent(uint8_t);

	void __fastcall OnTypeUpdate(TObject * Sender);
	void __fastcall TypeDeviceUpdate(void);
	void __fastcall UpdateTypeLayOut(void);
	void __fastcall UpdateCheckType();

	void AddDeviceType();
	void ModifyDeviceType();

	void TypeUpdate(void);

	void __fastcall RefreshPointsTable(void);

	void Refresh_Orders(void);

	//
	// Синхронизируемая функция обработки приема очередных данных измерения
	void __fastcall OnMeasureDataReceive(void);
	//
	// Переход к предыдущей странице TabControlMain
	void TabControlPrewTab(void);
	//
	// Переход к следующей странице TabControlMain
	void TabControlMainNextTab(void);
	//
	// Функция заполнения значениями вкладки условий проведения испытаний
	void FillConditionsTab(void);
	//
	// Функция перехода к следующему этапу (нажатие кнопки "Далее")
	bool NextTabStage(void);
	//
	// Установка значений  данных об условиях поверки
	bool SetConditionsData(void);
	//
	// Установка данных о поверяемом проиборе
	bool SetDeviceData(void);

	void KeyDownCheck(WORD &Key, System::WideChar &KeyChar);

	void SavePointsToFile(void);
	// User declarations


	void __fastcall UpdatePointChange();
	void UpdateCheckLayOut();
	void __fastcall UpdatePointsGrid();
	void UpdateOrdersGrid();
	void UpdateMesurmentMean();
	void UpdateMesurmentTool();
	void UpdateCommon();
	void UpdateStringGridTest();
	void ComboModFill();

	float error, avrflowEtalon = 0, flowEtalon, flowTest, volume, volumeEtalon,
		volumeTest, time, imp, impEtalon, impTest;

	int ShowStableFlow = 0;

    int units;

	 TMeterValue *Value;
	UnicodeString Str;
     void AutoFill();
	 void __fastcall  StopTimerEnabled();

	void SetUpDataFlowMeters();
	void  UpdateDevice();

	void __fastcall  AddFlowMeter();


	TDateTime DateTime;
	bool abort=false;
	static int FirstShow1;

	void CheckPhonePermission(void);
	#ifdef __ANDROID__
		void __fastcall onActivityResult(int requestCode, int resultCode, _di_JIntent data);
	#endif


};

// ---------------------------------------------------------------------------
extern PACKAGE TMainTabbedForm *MainTabbedForm;
// ---------------------------------------------------------------------------
#endif
