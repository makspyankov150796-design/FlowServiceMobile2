//---------------------------------------------------------------------------

#ifndef TModBusDeviceH
#define TModBusDeviceH
//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <System.Types.hpp>
#include <System.UITypes.hpp>
#include <System.Variants.hpp>
#include <FMX.GRAPHICS.hpp>
#include <FMX.DIALOGS.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <stdint.h>
#include "TRingFifo.h"
#include <System.SyncObjs.hpp>
#include "TBluetoothThread.h"
#include "ModbusRTU.h"
#include "HscTypedefs.h"

//#include "System.Classes.TList
#include <list>


using namespace std;

#define RESPONSE_WAIT_TIMEOUT   1000  //< Время (мс) ожидания ответа от устройства
#define RECEIVE_DATA_TIMEOUT     400  //< Таймаут приема данных, если предыдущие


//
// Структура данных для определения типа ожидаемого ответа от устройства
// будут складываться в std::list в том же порядке, в каком сообщения
// подаются в очередь на отправку в канал Bluetooth. Затем, после получения
// ответов, эти данные будут из контейнера извлекаться.
typedef struct TResponseData {
    uint8_t ID;
	uint8_t codeFunction; // < Код функ ции Modbus RTU
	uint16_t registerAddr; // < Начальный адрес
	uint16_t length; // < Длина (в байтах) ожидаемого ответа
} ResponseData_t, *pResponseData_t;

typedef enum EStates {
		STATE_STANDBY = 0,
	// < Подключение с bluetooth устройством не установлено
		STATE_CONNECT,// < К Bluetooth устройству подключились, пингуем для контроля соединения
		STATE_FAILURE // < Сбой в работе устройства
	} States_t;

class TModBusDevice : public TObject {
public:
	TModBusDevice();
	virtual __fastcall ~TModBusDevice();
	//
	// Поиск Bluetooth устройств
	void DiscoverBluetoothDevices(void);
	//
	// Подключиться к Bluetooth устройству
	uint8_t Connect(uint32_t deviceIndex);
	uint8_t Connect();
	void TModBusDevice::CheckBtPermission(void);

    bool TModBusDevice::GetBtPermission(void);

    virtual void AfterConnect(void);
	//
	// Отключиться от Bluetooth устройства

	void Disconnect(void);
	//
	// Задание CallBack-функции выполняемой при подключении к устройству
	void SetOnBluetoothConnect(TOnBluetoothConnect OnBtConnect);
	void SetOnBluetoothConnectMain(TOnBluetoothConnect OnBtConnect);
	//
	// Задание CallBack-функции выполняемой при отключении от устройства
	void SetOnBluetoothDisconnect(TNotifyEvent OnBtDisconnect);
	//
	//// Задание CallBack-функции выполняемой при завершении поиска устройств
	void SetOnPairedDiscoverEnd(TOnDiscoverEnd OnDiscoverEndPairedRoutine);

    void SetOnDiscoverEnd(TDiscoveryEndEvent OnDiscoverEndRoutine);

    //  Задание CallBack-функции выполняемой при отправке специальных сообщений
   void __fastcall SetOnDataSent(TOnDataSent onDataSent);


	//                      OnDiscoverEndPairedRoutine
	// Задание CallBack-функции выполняемой при истечении времени ожидания
	// подключения к Bluetooth устройству и поиска устройств
	void OnWdtOverflow(TNotifyEvent WdtRoutine);


	//
	// Проверка наличия подключения Bluetooth сокета
	bool IsBluetoothConnected(void) const ;

    //
	// Функция отправки данных в устройство
	void WriteDataToDevice(uint16_t regAddr,
		TBytes &txBytes);

    void WriteDataToDeviceCallBack(uint16_t regAddr,
		TBytes &txBytes, uint8_t CallBack);
	//
	// Функция отправки запроса на чтение значений регистров устройства
	void ReadDataFromDevice(uint16_t regAddr,
		uint16_t count);




	void WriteReg(uint16_t regAddr, uint16_t Data);
	void WriteReg(uint16_t regAddr, float Data);

	void WriteReg(uint16_t addr, uint16_t regAddr, uint32_t Data);
	void WriteReg(uint16_t addr, uint16_t regAddr, uint16_t Data);
	void WriteReg(uint16_t addr, uint16_t regAddr, float Data);
	void WriteReg(uint16_t regAddr, float *Data, int count);
	void WriteReg(uint16_t addr, uint16_t regAddr, bool Data);
	void WriteReg(uint16_t addr, uint16_t regAddr, uint8_t Data);

	float RegHandler_float(TBytes rxFrame, uint8_t *index);
	uint8_t RegHandler_uint8(TBytes rxFrame, uint8_t *index);
	uint16_t RegHandler_uint16(TBytes rxFrame, uint8_t *index);
	uint32_t RegHandler_uint32(TBytes rxFrame, uint8_t *index);
	bool RegHandler_bool(TBytes rxFrame, uint8_t *index);


	//
	// Поток, где происходит прием/передача данных по Bluetooth
	TBluetoothThread *btThread;

protected:
	uint32_t DeviceBtIndex = 0;
	uint16_t DeviceAddr;
	uint8_t RespID = 0;
	States_t State;

	//
	// Сюда будем складывать принятые данные для разбора
	TBytes rxFrame;

	uint8_t rxFrameArray[640];
	//
	// Таймер для отсчета времени ожидания ответа от устройства
	//static
		TTimer *respTimer;
	//
	// Время в тиках отправки сообщения устройству (1 тик - 1мс) для определения времени ожидания ответа
	uint32_t txTicks;
	//
	// Счетчик времени выполнеия измерений, по нему синхронизируем отправку запросов данных
	uint32_t measureTimeTicks;
	//
	// Список ожидаемых ответов
	list <ResponseData_t> respList;
   //  Tlist <ResponseData_t> respList;
	//
	// Функция обратного вызова при завершении процедуры подключения к Bluetooth устройству
	TOnBluetoothConnect OnBluetoothConnect;
	// < Вызывается для передачи данных о результате подключения форме приложения
	TOnBluetoothConnect OnBluetoothConnectMain;
	// < Вызывается для передачи данных о результате подключения форме приложения

	//
	// Функция-обработчик выполения операции подключения к Bluetooth устройству
	void __fastcall OnBtConnect(TObject *Sender, bool connected);
	//
	// Функция разбора принятых данных
	void __fastcall ReceiveDataParser(TObject *Sender, const TBytes &rxData);
	//
	// Функция обработки принятого пакета
	virtual void PacketHandler(void);

	void __fastcall PacketAnalyse(void);

    	// Функция смены состояния устройства
    virtual void ChangeStateDevice(States_t newState);

	//
	// Обработчик таймера времени ожидания ответа от устройства
	virtual void __fastcall OnRespTimer(TObject *Sender);

	//
	// Сброс данных о предыдущих измерениях в устройстве
	void ResetDeviceData(void);

      uint8_t error_status;

    private:

};

#endif
