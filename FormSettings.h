// ---------------------------------------------------------------------------

#ifndef FormSettingsH
#define FormSettingsH
// ---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Objects.hpp>
#include <System.SysUtils.hpp>
#include "ustring.h"

#include <FMX.DialogService.Async.hpp>

#include "THscDevice.h"
#include "TFlowMeter.h"

#include "TSettingsClass.h"

#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMXTee.Chart.hpp>
#include <FMXTee.Engine.hpp>
#include <FMXTee.Procs.hpp>
#include <FMXTee.Series.hpp>
#include <System.Rtti.hpp>
#include <FMX.Ani.hpp>
#include <FMX.Effects.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <System.Net.HttpClient.hpp>
#include <System.Net.HttpClientComponent.hpp>
#include <System.Net.URLClient.hpp>

//#ifdef _ANDROID_
//extern THscDevice *HSC;
//extern TFlowMeter *EtalonMeter;
//extern TFlowMeter *TestMeter;
//#endif

using namespace std;

// ---------------------------------------------------------------------------
class THSCSettingsForm : public TForm {
__published: // IDE-managed Components
	TStyleBook *SettingsStyleBook;
	TToolBar *SettingsToolBar1;
	TButton *Button1;
	TButton *RefreshButton;
	TLayout *SettingsLayout6;
	TPanel *Panel2;
	TLayout *Layout7;
	TRectangle *Rectangle4;
	TLabel *Label6;
	TLayout *Layout8;
	TRectangle *Rectangle5;
	TLabel *Label7;
	TImage *Image5;
	TLayout *Layout9;
	TRectangle *Rectangle6;
	TEdit *Edit6;
	TLabel *Label8;
	TImage *Image6;
	TLayout *Layout3;
	TRectangle *Rectangle3;
	TEdit *Edit3;
	TLabel *Label3;
	TImage *Image4;
	TComboBox *Imp1StateComboBox;
	TLayout *SettingsLayout23;
	TPanel *Panel1;
	TLayout *Layout2;
	TRectangle *Rectangle1;
	TLabel *SimLabel;
	TImage *Image2;
	TLayout *Layout4;
	TRectangle *Rectangle2;
	TEdit *EditInputSynch;
	TLabel *SynchInLabel;
	TImage *Image3;
	TLayout *Layout5;
	TRectangle *Rectangle7;
	TEdit *EditOutSynch;
	TLabel *Label4;
	TImage *Image7;
	TLayout *Layout10;
	TRectangle *Rectangle8;
	TEdit *EditEtalon;
	TLabel *Label5;
	TImage *Image8;
	TLabel *Z;
	TComboBox *SimComboBox;
	TLayout *Layout11;
	TRectangle *Rectangle9;
	TLabel *Label9;
	TImage *Image1;
	TComboBox *ComboBox4;
	TLayout *SettingsLayout3;
	TPanel *Panel3;
	TLayout *Layout13;
	TRectangle *Rectangle10;
	TLabel *Label11;
	TImage *Image9;
	TComboBox *SynchComboBox;
	TLayout *Layout14;
	TRectangle *Rectangle11;
	TEdit *EtalonImpCoefEdit;
	TLabel *Label12;
	TImage *Image10;
	TLayout *Layout15;
	TRectangle *Rectangle12;
	TEdit *EditEtalonImpChannel;
	TLabel *Label13;
	TImage *Image11;
	TLayout *Layout16;
	TRectangle *Rectangle13;
	TEdit *Edit8;
	TLabel *Label14;
	TImage *Image12;
	TVertScrollBox *SettingsScroll;
	TLayout *SettingsLayout1;
	TPanel *PanelVer;
	TLayout *LayoutVer1;
	TRectangle *Rectangle14;
	TImage *Image13;
	TLayout *Layout19;
	TRectangle *Rectangle15;
	TEdit *ProtocolEdit;
	TLabel *Label17;
	TImage *Image14;
	TLayout *Layout20;
	TRectangle *Rectangle16;
	TEdit *Edit10;
	TLabel *Label18;
	TImage *Image15;
	TLayout *Layout21;
	TRectangle *Rectangle17;
	TEdit *EditCharge;
	TLabel *Label19;
	TImage *Image16;
	TLabel *LabelVer;
	TEdit *VersionEdit;
	TLabel *Label16;
	TLayout *Layout1;
	TRectangle *Rectangle18;
	TLabel *Label20;
	TImage *Image17;
	TComboBox *FlowCalcComboBox;
	TLayout *Layout6;
	TRectangle *Rectangle19;
	TLabel *Label21;
	TImage *Image18;
	TComboBox *ComboBox7;
	TLayout *Layout12;
	TRectangle *Rectangle20;
	TLabel *Label22;
	TImage *Image19;
	TComboBox *ExtSynchComboBox;
	TLayout *Layout17;
	TRectangle *Rectangle21;
	TLabel *Label23;
	TImage *Image20;
	TComboBox *StopSpillComboBox;
	TLayout *Layout18;
	TRectangle *Rectangle22;
	TLabel *Label24;
	TImage *Image21;
	TComboBox *StartSpillComboBox;
	TListBoxItem *ListBoxItem1;
	TListBoxItem *ListBoxItem2;
	TListBoxItem *ListBoxItem25;
	TListBoxItem *ListBoxItem26;
	TLayout *Layout22;
	TLayout *Layout23;
	TButton *PasswordButton;
	TCheckBox *VersionCheckBox;
	TCheckBox *SettingsCheckBox;
	TLayout *Layout24;
	TRectangle *Rectangle23;
	TEdit *TimeLimitEdit;
	TLabel *Label1;
	TImage *Image22;
	TLayout *Layout25;
	TRectangle *Rectangle24;
	TEdit *ImpLimitEdit;
	TLabel *Label2;
	TImage *Image23;
	TTimer *FormTimer;
	TLayout *Layout26;
	TStringGrid *CoefStringGrid;
	TChart *Chart1;
	TStringColumn *StringColumn1;
	TStringColumn *StringColumn2;
	TStringColumn *StringColumn3;
	TLayout *Layout27;
	TButton *RefreshConfigButton;
	TButton *DeleteConfigButton;
	TButton *AddRowButton;
	TButton *DeleteRowButton;
	TStringColumn *StringColumn4;
	TButton *LoadConfigButton;
	TButton *SaveConfigButton;
	TEdit *ValueEdit;
	TLabel *ValueLabel;
	TLayout *Layout28;
	TLabel *ValueLabel1;
	TStringColumn *StringColumn5;
	TStringColumn *StringColumn6;
	TStringColumn *StringColumn7;
	TLineSeries *Series1;
	TLayout *Layout29;
	TLabel *Label10;
	TCheckBox *EtalonCheckBox;
	TPanel *Panel4;
	TCheckBox *CalibrCheckBox;
	TButton *Button3;
	TLang *Lang1;
	TLayout *Layout30;
	TLayout *Layout35;
	TLabel *Label28;
	TLayout *Layout36;
	TLabel *Label29;
	TCheckBox *CheckBox2;
	TEllipse *Light1;
	TGlowEffect *Glow1;
	TEllipse *Circle;
	TColorAnimation *Anim1;
	TColorAnimation *Anim2;
	TEllipse *Ellipse1;
	TGlowEffect *GlowEffect1;
	TEllipse *Ellipse2;
	TColorAnimation *Animation1;
	TColorAnimation *Animation2;
	TLayout *Layout42;
	TPanel *Panel6;
	TLayout *Layout53;
	TLabel *Label46;
	TLayout *Layout31;
	TRectangle *Rectangle25;
	TLabel *Label15;
	TEdit *PerformerEdit;
	TImage *Image24;
	TLayout *Layout32;
	TRectangle *Rectangle26;
	TLabel *Label25;
	TEdit *IDPerformerEdit;
	TImage *Image25;
	TLayout *Layout33;
	TRectangle *Rectangle27;
	TLabel *Label26;
	TEdit *OrgNameEdit;
	TImage *Image26;
	TLayout *Layout34;
	TRectangle *Rectangle28;
	TLabel *Label27;
	TEdit *IDOrgEdit;
	TImage *Image27;
	TLayout *Layout37;
	TRectangle *Rectangle34;
	TLabel *Label35;
	TEdit *EditDir;
	TImage *Image33;
	TLayout *Layout38;
	TRectangle *Rectangle30;
	TLabel *Label31;
	TEdit *EditEmail;
	TImage *Image29;
	TLayout *LayoutSound;
	TRectangle *Rectangle29;
	TLabel *Label30;
	TEdit *EditStopSoundFile;
	TImage *Image28;
	TPanel *Panel5;
	TLayout *Layout39;
	TRectangle *Rectangle31;
	TLabel *Label32;
	TEdit *EditSignCipher;
	TImage *Image30;
	TLayout *Layout40;
	TRectangle *Rectangle32;
	TLabel *Label33;
	TEdit *EditDocNum;
	TImage *Image31;
	TLayout *Layout41;
	TRectangle *Rectangle33;
	TLabel *Label34;
	TEdit *EditMeans;
	TImage *Image32;
	TLayout *Layout43;
	TRectangle *Rectangle35;
	TLabel *Label36;
	TComboBox *ComboBoxDoc;
	TImage *Image34;
	TLayout *Layout44;
	TRectangle *Rectangle36;
	TLabel *Label37;
	TComboBox *ComboBoxAdress;
	TImage *Image35;
	TLayout *Layout45;
	TRectangle *Rectangle37;
	TLabel *Label38;
	TImage *Image36;
	TMemo *MemoTitle;
	TLayout *Layout46;
	TRectangle *Rectangle38;
	TLabel *Label39;
	TImage *Image37;
	TMemo *MemoMeans;
	TLayout *Layout47;
	TRectangle *Rectangle39;
	TLabel *Label40;
	TEdit *EditProtocolNumTitle;
	TImage *Image38;
	TLayout *Layout49;
	TRectangle *Rectangle41;
	TLabel *Label42;
	TEdit *EditProtocolNum;
	TImage *Image40;
	TButton *ButtonDownload;
	TNetHTTPClient *NetHTTPClient1;
	TLayout *Layout48;
	TRectangle *Rectangle40;
	TLabel *Label41;
	TComboBox *ComboBoxOrders;
	TImage *Image39;
	TLayout *Layout50;
	TRectangle *Rectangle42;
	TEdit *EditStartCHNum;
	TLabel *Label43;
	TImage *Image41;
	TLayout *Layout51;
	TRectangle *Rectangle43;
	TEdit *EditStopCHNum;
	TLabel *Label44;
	TImage *Image42;
	TLayout *Layout52;
	TRectangle *Rectangle44;
	TEdit *EditTestCHNum;
	TLabel *Label45;
	TImage *Image43;
	TLayout *Layout54;
	TRectangle *Rectangle45;
	TLabel *Label47;
	TComboBox *ComboBox1;
	TImage *Image44;
	TLayout *Layout55;
	TRectangle *Rectangle46;
	TLabel *Label48;
	TComboBox *ComboBoxDestination;
	TImage *Image45;
	TLayout *Layout56;
	TRectangle *Rectangle47;
	TEdit *EditPackage;
	TLabel *Label49;
	TImage *Image46;

	void __fastcall SetHscDevice (THscDevice *THSC) ;
    void __fastcall SetFlowMeter (TFlowMeter *TMeter) ;
    void __fastcall SetTestMeter (TFlowMeter *TMeter) ;

	void __fastcall Button1Click(TObject *Sender);
	void __fastcall RefreshButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall SimComboBoxChange(TObject *Sender);
	void __fastcall SettingsCheckBoxChange(TObject *Sender);
	void __fastcall VersionCheckBoxChange(TObject *Sender);
	void __fastcall FormTimerTimer(TObject *Sender);
	void __fastcall SettingsChange(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall RefreshConfigButtonClick(TObject *Sender);
	void __fastcall EtalonImpCoefEditExit(TObject *Sender);
	void __fastcall ValueEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall SaveConfigButtonClick(TObject *Sender);
	void __fastcall LoadConfigButtonClick(TObject *Sender);
	void __fastcall EtalonCheckBoxChange(TObject *Sender);
	void __fastcall SynchComboBoxChange(TObject *Sender);
	void __fastcall StartSpillComboBoxChange(TObject *Sender);
	void __fastcall StopSpillComboBoxChange(TObject *Sender);
	void __fastcall ExtSynchComboBoxChange(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall CalibrCheckBoxChange(TObject *Sender);
	void __fastcall DeleteRowButtonClick(TObject *Sender);
	void __fastcall FlowCalcComboBoxChange(TObject *Sender);
	void __fastcall Imp1StateComboBoxChange(TObject *Sender);
	void __fastcall PerformerEditExit(TObject *Sender);
	void __fastcall ValueEditExit(TObject *Sender);
	void __fastcall PerformerEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall IDPerformerEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall NameOrgEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall IDOrgEditKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall IDPerformerEditExit(TObject *Sender);
	void __fastcall IDOrgEditExit(TObject *Sender);
	void __fastcall OrgNameEditExit(TObject *Sender);
	void __fastcall EditStopSoundFileExit(TObject *Sender);
	void __fastcall EditEmailExit(TObject *Sender);
	void __fastcall EditSignCipherExit(TObject *Sender);
	void __fastcall EditDocNumExit(TObject *Sender);
	void __fastcall EditMeansExit(TObject *Sender);
	void __fastcall EditDirExit(TObject *Sender);
	void __fastcall ComboBoxDocChange(TObject *Sender);
	void __fastcall ComboBoxAdressChange(TObject *Sender);
	void __fastcall MemoTitleExit(TObject *Sender);
	void __fastcall MemoMeansExit(TObject *Sender);
	void __fastcall EditProtocolNumTitleExit(TObject *Sender);
	void __fastcall EditProtocolNumExit(TObject *Sender);
	void __fastcall ButtonDownloadClick(TObject *Sender);
	void __fastcall EtalonImpChannelEditChange(TObject *Sender);
	void __fastcall ComboBoxOrdersChange(TObject *Sender);
	void __fastcall EditInputSynchExit(TObject *Sender);
	void __fastcall EditStopCHNumExit(TObject *Sender);
	void __fastcall EditStartCHNumExit(TObject *Sender);
	void __fastcall EditOutSynchExit(TObject *Sender);
	void __fastcall EditTestCHNumExit(TObject *Sender);
	void __fastcall EditInputSynchChange(TObject *Sender);
	void __fastcall ComboBoxDestinationChange(TObject *Sender);


private: // User declarations


	tVersion Version;
	tCheckState CheckState;
	tSettings Settings;

	THscDevice *HSC;

    UnicodeString strng;

public: // User declarations


	TFlowMeter *EtalonMeter;
	TFlowMeter *TestMeter;

	void __fastcall  OnVersionRead(TObject *Sender);
	void __fastcall  OnSettingsRead(TObject *Sender);
	void __fastcall  OnAccessRead(TObject *Sender);
	void __fastcall  OnConfigRead(TObject *Sender);

    void UpdateCalibrGrid();
	void UpdateChart();

	void UpdateUserSettings();

   //UnicodeString StrTrim(UnicodeString);

	__fastcall THSCSettingsForm(TComponent* Owner);


	 TSettingsClass *UserSettings;
};

// ---------------------------------------------------------------------------
extern PACKAGE THSCSettingsForm *HSCSettingsForm;
// ---------------------------------------------------------------------------
#endif
