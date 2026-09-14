//---------------------------------------------------------------------------

#ifndef FormPhotoCaptureH
#define FormPhotoCaptureH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Media.hpp>
#include <FMX.Objects.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>

#include <System.DateUtils.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Grid.hpp>
#include <FMX.Grid.Style.hpp>
#include <FMX.ScrollBox.hpp>
#include <System.Rtti.hpp>

#include "THscDevice.h"
#include "TFlowMeter.h"

#include <FMX.TabControl.hpp>
#include <FMX.Gestures.hpp>
#include <FMX.Ani.hpp>
#include <FMX.Effects.hpp>

#ifdef __ANDROID__
#include <AndroidApi.JNI.JavaTypes.hpp>
#include <Androidapi.Helpers.hpp>
#endif
#include <System.Permissions.hpp>
#include <System.SyncObjs.hpp>

#include "UserRoutines.h"

//---------------------------------------------------------------------------
class TPhotoCaptureForm : public TForm
{
__published:	// IDE-managed Components
	TStyleBook *StyleBook1;
	TToolBar *ToolBar1;
	TButton *Button1;
	TCameraComponent *Camera;
	TButton *Button4;
	TLayout *LayoutPictures;
	TStringGrid *StringGridTest;
	TStringColumn *StringColumn10;
	TStringColumn *StringColumn11;
	TStringColumn *StringColumn12;
	TStringColumn *StringColumn13;
	TLabel *Label1;
	TLabel *Label3;
	TLayout *LayoutPanel;
	TLayout *Layout7;
	TRectangle *Rectangle4;
	TEdit *TimeEdit;
	TLabel *LabelLimit;
	TLabel *LabelTime;
	TImage *Image2;
	TLayout *LayoutButtons;
	TButton *TestButton;
	TLayout *Layout24;
	TProgressBar *ProgressBar;   //static
	TLayout *Layout4;
	TEdit *EditBefore;
	TButton *VisibleButton;
	TButton *CameraButton;
	TButton *FocusButton;
	TButton *HighQButton;
	TTabControl *TabControl;
	TTabItem *BeforeTabItem;
	TTabItem *AfterTabItem;
	TLayout *Layout2;
	TImage *ImageAfter;
	TEdit *EditAfter;
	TLabel *Label2;
	TLabel *Label4;
	TImage *ImageBefore;
	TGestureManager *GestureManager1;
	TLayout *Layout5;
	TLayout *Layout6;
	TTimer *TimerOn;
	TButton *MediumQButton;
	TButton *CancelButton;
	TButton *Button6;
	TButton *ButtonMonitor;
	TLayout *Layout8;
	TEllipse *Light1;
	TGlowEffect *Glow1;
	TEllipse *Circle;
	TColorAnimation *Anim1;
	TColorAnimation *Anim2;
	TLabel *Label5;
	TButton *ButtonApplyBefore;
	TImage *ImageBeforeEditor;
	TButton *ButtonApplyAfter;
	TImage *ImageAfterEditor;
	TLayout *Layout10;
	TButton *PreviousPointButtonn;
	TButton *NextPointButton;
	TLabel *PointLabel;
	TImage *ImageConnectOff;
	TImage *ImageConnectOn;
	TImage *ImageConnectProcces;
	TEllipse *Ellipse1;
	TGlowEffect *GlowEffect1;
	TEllipse *Ellipse2;
	TColorAnimation *FlowColorAnimation1;
	TColorAnimation *FlowColorAnimation2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall CameraSampleBufferReady(TObject *Sender, const TMediaTime ATime);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall TestButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall OnTimer(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall EditBeforeEnter(TObject *Sender);
	void __fastcall CameraButtonClick(TObject *Sender);
	void __fastcall MediumQButtonClick(TObject *Sender);
	void __fastcall FocusButtonClick(TObject *Sender);
	void __fastcall HighQButtonClick(TObject *Sender);
	void __fastcall LowQButtonClick(TObject *Sender);
	void __fastcall TabControlGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall BeforeTabItemGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall AfterTabItemGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall ImageAfterGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall EditBeforeExit(TObject *Sender);
	void __fastcall TimerOnTimer(TObject *Sender);
	void __fastcall VisibleButtonClick(TObject *Sender);
	void __fastcall EditAfterEnter(TObject *Sender);
	void __fastcall EditAfterExit(TObject *Sender);
	void __fastcall ImageBeforeClick(TObject *Sender);
	void __fastcall ImageAfterClick(TObject *Sender);
	void __fastcall EditAfterKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall EditBeforeKeyDown(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
          TShiftState Shift);
	void __fastcall CancelButtonClick(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall EditAfterValidate(TObject *Sender, UnicodeString &Text);
	void __fastcall EditBeforeValidate(TObject *Sender, UnicodeString &Text);
	void __fastcall ButtonMonitorClick(TObject *Sender);
	void __fastcall LabelLimitGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall TimeEditExit(TObject *Sender);
	void __fastcall ImageBeforeGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall ImageBeforeEditorGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall ButtonApplyBeforeClick(TObject *Sender);
	void __fastcall ButtonApplyAfterClick(TObject *Sender);
	void __fastcall ImageAfterEditorGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall NextPointButtonClick(TObject *Sender);
	void __fastcall PreviousPointButtonnClick(TObject *Sender);




private:	// User declarations

  bool camPermissionGranted;      //< Разрешение на доступ к камере
  bool storagePremissionGranted;  //< Разрешение на доступ к хранилищу

  void AfterSpillage(void);

      //
    // Объект TBitmap для сохранения фото перед проливкой
    TBitmap *PhotoBefore;
    //
    // Объект TBitmap для сохранения фото после выполнения проливки
    TBitmap *PhotoAfter;
    //
    // Список ожидаемых ответов
    void __fastcall CameraGetPhoto(void);
    void __fastcall CameraFocus(void);
    //Состояние системы
    uint8_t state;
    uint8_t CameraState;
    uint8_t newstate;

    THscDevice *HSC;



public:		// User declarations

	static TTimer *SpillTimer;

	TFlowMeter *EtalonMeter;
	TFlowMeter *TestMeter;

	__fastcall TPhotoCaptureForm(TComponent* Owner);
    void SaveVerificationResults(void);
    void CheckCamPermission(void);
	void CheckStoragePermission(void);

	void UpdatePointChange(void);

    void __fastcall   DataReadFormsUpdate(void);
    void __fastcall   DataReadFormsFinalUpdate(void);


    void SetHscDevice(THscDevice *hsc) {TPhotoCaptureForm::HSC = hsc;};
    void  ChangeState(uint8_t newstate);
    float error, flowEtalon, flowTest, volume, volumeTest, volumeEtalon, time;

	  void __fastcall UpdateState(void);
		void EditImageBeforeOpen();
		void EditImageAfterOpen();
		void EditImageBeforeClose();
		void EditImageAfterClose();
	  int Active;
	  int aTime=0;

	int ImageBeforelWidth;
	int ImageBeforeHeight;
	int ImageBeforeAngle;


	TPointF FLastMousePos;
	  int	InitialWidth;
		int InitialHeight;
	float InitialDistance ;
	float  LocationStartX ;
	float  LocationStartY ;
    TPointF position;
	int AngleStart;
};
//---------------------------------------------------------------------------
extern PACKAGE TPhotoCaptureForm *PhotoCaptureForm;
//---------------------------------------------------------------------------
#endif
