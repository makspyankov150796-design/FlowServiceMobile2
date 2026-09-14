//---------------------------------------------------------------------------
#ifndef TBluetoothThreadH
#define TBluetoothThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Types.hpp>
#include <System.UITypes.hpp>
#include <System.Variants.hpp>
#include <System.Bluetooth.Components.hpp>
#include <System.Bluetooth.hpp>
#include <stdint.h>
#include "TRingFifo.h"
#include <System.SyncObjs.hpp>
#include <FMX.Types.hpp>
#include "UserRoutines.h"

#include "TDebug.h"


#ifdef __ANDROID__
#include <AndroidApi.JNI.JavaTypes.hpp>
#include <Androidapi.Helpers.hpp>
#endif

//---------------------------------------------------------------------------
#define DISCOVER_DEVICES_TIMEOUT  10000  //< Время поиска устройств BT в мс

#define RX_FIFO_SIZE              4096  //< Размер FIFO приемника (байт)
#define TX_FIFO_SIZE              4096  //< Размер FIFO передатчика (байт)

#define MSG_SEPARATE_DELAY         100  //< Пауза между приемом сообщения и отправкой следующего (мс)
#define RSP_SEPARATE_DELAY         1000 //< Ожидание ответа (мс)
//---------------------------------------------------------------------------
// Тип CallBack-функции, вызываемой при подключении к устройству
typedef void __fastcall (__closure *TOnBluetoothConnect)(System::TObject* Sender, bool connected);

// Тип CallBack-функции, вызываемой при ошибке
//typedef void __fastcall (__closure *TOn)();

//
// Тип CallBack-функции, вызываемой при приеме данных по Bluetooth
typedef void __fastcall (__closure *TOnDataReceive)(System::TObject *Sender, const TBytes &rxBytes);

typedef void __fastcall (__closure *TOnDiscoverEnd)(System::TObject* const Sender,
  										TBluetoothDeviceList* const ADeviceList,
                                        TBluetoothDeviceList* const PairedDeviceList);

   //Функция отправленного сообщения
 typedef void __fastcall (__closure *TOnDataSent)(uint8_t callback);


//
// Структура параметров передачи
typedef struct TTTxMsgParam
{
  uint16_t ID;
  uint16_t status;
  uint16_t msgLength;         //< Длина сообщения в байтах
  uint16_t respTimeout;       //< Время (мс) ожидания ответа от ведомого
  uint16_t repeat;
  uint16_t callback;
} TxMsgParam_t, *pTxMsgParam_t;
//
// Определения стандартных вариантов инициализации структуры параметров передачи
#define MSG_PARAM_STD_RESPONSE  { 0, 0, 0, 500, 3 }
#define MSG_PARAM_ZERO_INIT     { 0, 0, 0,  0, 0 }
//---------------------------------------------------------------------------
class TBluetoothThread : public TThread
{
  public:

    void  SendData(void);

	static 	UnicodeString str;


    void TBluetoothThread::CheckBtPermission();
     bool btPermissionGranted;       //< Разрешение на доступ к Bluetooth модулю

    __fastcall TBluetoothThread();
    __fastcall TBluetoothThread(bool ACreateSuspended) : TThread(ACreateSuspended){}
    __fastcall ~TBluetoothThread();
    //
    // События для синхронизации с формой
    TDiscoveryEndEvent OnDiscoverEnd;  //< Для связи с формой приложения

    TOnDiscoverEnd OnDiscoverEndPaired; //< Для связи с формой приложения

	TOn   OnRespError;
	void __fastcall RespError();

    TNotifyEvent OnDiscoverWdt;        //< При срабатывании WDT для синхронизации с формой

    TOnBluetoothConnect OnConnect;     //< При подключении к устройству
	void __fastcall OnConnected(void);
    void __fastcall OnErrorConnected(void);

    TNotifyEvent OnDisconnect;         //< При отключении устройства
    void __fastcall OnDisconnected(void);

    TOnDataReceive OnDataReceive;      //< При приеме данных
    void __fastcall OnReceive(void);

    TOnDataSent OnDataSent;
   // void __fastcall SetOnDataSent(TOnDataSent onDataSent);


    //
    // Функция инициализации класса
    void Initialize(void);
    //
    // Выполнить поиск устройств Bluetooth
    void DiscoverDevices(void);
    //
    // Функция выполнения подключения к Bluetooth устройству
    uint8_t  Connect(uint32_t deviceIndex);
    //
    //  Функция передачи данных в канал Bluetooth

    void ConnectPairedHSC(void);


    void Transmit(TBytes &txBytes, const TxMsgParam_t &msgParam);
    //
    // Функция выполнения отключения от Bluetooth устройства
    void Disconnect(void);
    //
    // Подключено?
    bool IsConnected(void) const;
    //
    // Возможные состояния
    enum eStates
	{
      BT_NOINIT = 0,    //< Класс неинициализирован
      BT_INIT,          //< Класс инициализирован
      BT_CONNECT,       //< Выполнение подключения
      BT_READY,         //< Подключение выполнено, класс готов
      BT_RECIVING,      //< Ждем ответа
      BT_DISCONNECT,    //< Выполнение отключения
	  BT_FAILURE,       //< Сбой в работе класса
    } btState;
    //
    // Классы исключений
    class ESocketUnconneced { };  //< Сокет не подключен

    class EWrongParams { };       //< Передача некорректных аргументов

    TBluetoothDeviceList*  PairedDevices(void);

    bool IsReciving;

  protected:
	void __fastcall Execute(void);



  private:
	TBluetooth            *Bluetooth;          //< Класс доступа к Bluetooth
	TBluetoothDeviceList  *btDiscoverDevices;   //< Доступные устройства
	TBluetoothDevice      *btDevice;            //< Указатель на устройства
    TBluetoothSocket      *btClientSocket;      //< Клиентский сокет Bluetooth

    TBluetoothDeviceList* PairedDevicesList;


    //
    // Критическая секция для работы с передающим FIFO. Так как передачи
    // отделены друг от друга сначала в FIFO будем ложить структуру параметров
    // передачи.
    TCriticalSection     *Critical;
    //
    // Тут храним параметры отправляемого в данный момент сообщения
    TxMsgParam_t txMsgParam;
    //
    // FIFO передачи
    TRingFifo *txFifo;
    //
    // Таймер, используемый в качестве сторожевого для операций Bluetooth
	static TTimer *WDT;
	static TTimer *ConnectTimer;
    //
    // Индекс устройства в списке, к которому подключаемся
    uint32_t btDeviceIndex;
	//
    // Сюда кладем принятое сообщение и передаем на обработку
    TBytes rxDataBytes;
    //
    // Сюда извлекаем отправляемое сообщение из FIFO и отдаем на передачу в сокет
    TBytes txDataBytes;
    //
    // Функция завершения сканирования устройств
    void __fastcall BluetoothDiscoverEnd(System::TObject* const Sender,
  										TBluetoothDeviceList* const ADeviceList);
    //

    // Функция, вызываемая при срабатывании таймера WDT
	void __fastcall OnWDT(TObject *Sender);
	void __fastcall OnConnectTimer(TObject *Sender);
	//
    // Функция выполнения подключения к Bluetooth устройству
    void __fastcall Connecting(void);
    //
    // Основная рабочая функция, обеспечивающая обмен данными через Bluetooth
    void __fastcall DataExchange(void);
    //
    // Функция выполнения отключения устройства Bluetooth
    void Disconnecting(void);
};
//---------------------------------------------------------------------------
#endif  // TBluetoothThreadH
//---------------------------------------------------------------------------