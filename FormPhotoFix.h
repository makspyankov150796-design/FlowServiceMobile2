//---------------------------------------------------------------------------

#ifndef FormPhotoFixH
#define FormPhotoFixH
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

#include "System.DateUtils.hpp"




//---------------------------------------------------------------------------
class TPhotoFixForm : public TForm
{
__published:	// IDE-managed Components
	TStyleBook *StyleBook1;
	TToolBar *ToolBar1;
	TButton *Button1;
	TCameraComponent *Camera;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TLayout *Layout1;
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
	TTimer *TimerMesure;
	TButton *ButtonApplyBefore;
	TImage *ImageBeforeEditor;
	TButton *ButtonApplyAfter;
	TImage *ImageAfterEditor;
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
	void __fastcall TimerMesureTimer(TObject *Sender);
	void __fastcall ButtonApplyBeforeClick(TObject *Sender);
	void __fastcall ButtonApplyAfterClick(TObject *Sender);
	void __fastcall ImageBeforeEditorGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);
	void __fastcall ImageAfterEditorGesture(TObject *Sender, const TGestureEventInfo &EventInfo,
          bool &Handled);




private:	// User declarations

  bool camPermissionGranted;      //< Разрешение на доступ к камере
  bool storagePremissionGranted;  //< Разрешение на доступ к хранилищу

  void AfterSpillage(void);
  void	   StopMesure(void);
  void	   StartMesure(void);

   TTime BeginTime, EndTime;
   float DurationTime=0;
	  //
	// Объект TBitmap для сохранения фото перед проливкой
    TBitmap *PhotoBefore;
	//
    // Объект TBitmap для сохранения фото после выполнения проливки
	TBitmap *PhotoAfter;
    //
    // Список ожидаемых ответов
    void __fastcall CameraGetPhoto(void);

    //Состояние системы
    uint8_t state;
    uint8_t CameraState;
    uint8_t newstate;

	THscDevice *HSC;


	void EditImageBeforeOpen();
	void EditImageAfterOpen();
	void EditImageBeforeClose();
	void EditImageAfterClose();

    void __fastcall CameraFocus();

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

public:		// User declarations

	static TTimer *SpillTimer;

	TFlowMeter *EtalonMeter;
	TFlowMeter *TestMeter;

	__fastcall TPhotoFixForm(TComponent* Owner);
    void SaveVerificationResults(void);
    void CheckCamPermission(void);
    void CheckStoragePermission(void);
    void __fastcall   DataReadFormsUpdate(void);
    void __fastcall   DataReadFormsFinalUpdate(void);


	void SetHscDevice(THscDevice *hsc) {TPhotoFixForm::HSC = hsc;};
    void  ChangeState(uint8_t newstate);
	float error, flowEtalon, flowTest, volume, volumeTest, volumeEtalon, time;
	int SpillageTime;
	  void __fastcall UpdateState(void);



	  TOn OnTurnUpConnection;
	  void SetOnTurnUpConnection(TOn On);
};
//---------------------------------------------------------------------------
extern PACKAGE TPhotoFixForm *PhotoFixForm;
//---------------------------------------------------------------------------
#endif
