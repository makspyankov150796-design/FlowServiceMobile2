//---------------------------------------------------------------------------

#ifndef FormDeviceSlectionH
#define FormDeviceSlectionH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include "THscDevice.h"
#include <FMXTee.Chart.hpp>
#include <FMXTee.Engine.hpp>
#include <FMXTee.Procs.hpp>
#include <FMXTee.Series.hpp>
//---------------------------------------------------------------------------
class TDeviceSelectionForm : public TForm
{
__published:	// IDE-managed Components
	TToolBar *ToolBar1;
	TButton *Button1;
	TStyleBook *StyleBook1;
	TListBox *DiscoverDevicesList;
	TButton *ButtonScan;
	TButton *ButtonConnect;
	TAniIndicator *Spinner;
	TLabel *StatusLabel;
	void __fastcall ButtonScanClick(TObject *Sender);
	void __fastcall ButtonConnectClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall DiscoverDevicesListChangeCheck(TObject *Sender);
	void __fastcall DiscoverDevicesListChange(TObject *Sender);
private:	// User declarations
 //
  // Имена полей таблицы измерений
  static const UnicodeString GridFieldName[4];

  bool btPermissionGranted;       //< Разрешение на доступ к Bluetooth модулю
  bool camPermissionGranted;      //< Разрешение на доступ к камере
  bool storagePremissionGranted;  //< Разрешение на доступ к хранилищу
  //
  // Объект класса устройства HSC
  THscDevice *HSC;

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
  void __fastcall Bluetooth1DiscoveryEnd(TObject * const Sender, TBluetoothDeviceList * const ADeviceList);
  void __fastcall BluetoothPairedDiscoveryEnd(TObject * const Sender,
  												TBluetoothDeviceList * const ADeviceList,
  												TBluetoothDeviceList * const PairedDeviceList);
  void __fastcall PairedDevicesUpdate(void);
  //
  // Вызывается при превышении времени сканирования
  void __fastcall DiscoverWdtOverflow(TObject *Sender);
  //
  // Вызывается при подключении к Bluetooth устройству
  void __fastcall OnBluetoothConnect(TObject *Sender, bool connected);
  //
  // Вызывается при отключении устройства Bluetooth
  void __fastcall OnBluetoothDisconnect(TObject *Sender);
  //
  // Вызывается всякий раз, когда в классе HSC Device меняется состояние
  void __fastcall OnHscDeviceChangeState(TObject *Sender, HscStates_t newState);
  //
  // Функция, синхронизируемая, для обработки смены состояния устройства
  void __fastcall OnDeviceChangeState(void);
  //
  // Когда прилетели очередные данные во время измерения
  void __fastcall OnDataReceive(TObject *Sender, TotalData_t &Data);
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
  //
  // Функция "забирания" фото из камеры
  void __fastcall CameraGetPhoto(void);
public:		// User declarations
	__fastcall TDeviceSelectionForm(TComponent* Owner, THscDevice *HSC);
	void __fastcall Init(THscDevice *HSC);
};
//---------------------------------------------------------------------------
extern PACKAGE TDeviceSelectionForm *DeviceSelectionForm;
//---------------------------------------------------------------------------
#endif
