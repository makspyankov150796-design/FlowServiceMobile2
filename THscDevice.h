#ifndef THscDeviceH
#define THscDeviceH
// ---------------------------------------------------------------------------
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
#include "TModBusDevice.h"
#include <list>

// #include <Vcl.ExtCtrls.hpp>

#include <FMX.Types.hpp>
// ---------------------------------------------------------------------------
/*
 Класс описывающий устройство HSC, к которому подуключаемся по Bluetooth
 и с ним взаимодействуем посредством этого беспроводного интерфейса.
 Тут создается поток для обмена данными с устройством. А передача данных
 в основное приложение происходит посредством вызова соответствующих
 CallBack функций для того или иного события.
 */
// ---------------------------------------------------------------------------
using namespace std;
// ---------------------------------------------------------------------------
#define HSC_DEVICE_ADDRESS      0x51  //< Адрес устройства для обмена данными с
// ним по протоколу Modbus RTU

#define HSC_BROADCAST_ADDRESS   0x00  //< Адрес для широковещательных сообщений


#define MAIN_TIMER_INTERVAL      100  //< Интервал для основного таймера (мс)

#define PING_INTERVAL           10000  //< Интервал пингования устройства

#define CONFIG_INTERVAL          3000  //< Интервал пингования устройства

#define MEASURE_PERIOD          1000  //< Период (мс) опроса устройства во время
// выполнения измерений (проливки)

// данные приходило >= этому времени (мс)
// назад, то будут сброшены все индексы
// ---------------------------------------------------------------------------
// Определение адресов для работы по Modbus RTU
#define DEVICE_REVISION_ADDR        0x0000  //< Адрес регистра ревизии устройства
#define PROTOCOL_VERSION_ADDR       0x0001  //< Адрес регистра версии протокола
#define DEVICE_SETTINGS_ADDR        0x0003  //< Адрес регистра версии протокола
#define DEVICE_CHARGE_ADDR        	0x000B  //< Адрес регистра версии протокола
#define DEVICE_CHECKSTATE_ADDR      0x000F  // Проверка состония и задане состония измерения
#define CHANNEL_EXT_SYN             0x000A
#define MEASURE_TIME_ADDR           0x0006  //< Адрес регистра времени измерений
#define MEASURE_PULSES_LIMIT_ADDR   0x0007  //< Адрес регистра ограничения
// количества импульсов измерения
#define START_CONFIG_ADDR           0x0004  //< Адрес регистра конфигурирования

#define CHANNEL_0_CONFIG_ADDR         0x4004 //< Адрес настройки первого канала
#define CHANNEL_1_CONFIG_ADDR         0x4014 //< Адрес настройки первого канала
#define CHANNEL_2_CONFIG_ADDR         0x4024 //< Адрес настройки первого канала
#define CHANNEL_3_CONFIG_ADDR         0x4034 //< Адрес настройки первого канала



// запуска и останова измерения
#define MEASURE_STATE_ADDR          0x0011  //< Адрес регистра состояния проливки

#define FINAL_TIME_ADDR           0x8000  //< Адрес регистра окончательного времени

#define FINAL_DATA_ADDR           0x8200  //< Адрес регистра окончательных данных

#define FIX_VALUES_REG_ADDR         0x9000  //< Регистр для фиксации значений
#define MEASURE_DATA_ADDR           0x9100  //< Адрес регистра текущих данных

#define IMP_DATA_ADDR           	0x9102  //< Адрес регистра текущих данных
// измерений (промежуточные результаты)
#define MEASURE_TOTAL_ADDR          0x9200  //< Адрес регистра итоговых данных

#define MEASURE_WIDTH_ADDR          0x9300  //< Адрес регистра итоговых данных


// измерения
#define RESET_MEASURE_ADDR          0x000F  //< Адрес регистра для сброса данных


#define COEF_NUM 20


#define COMPACT_CONFIG_ACCESS          0xC000  //< Адрес регистра доступа
#define COMPACT_CONFIG_PASS            0xC001  //< При записи Пароля - открывается доступ. При чтении возвращается текущий доступ.
#define COMPACT_CONFIG_PIN		       0xC001  //< При записи
#define COMPACT_CONFIG_SERVICEPASS     0xC002  //< Пароль 1
#define COMPACT_CONFIG_FACTORYPASS     0xC003  //< Пароль 2

#define COMPACT_CONFIG_KP              0xC004  //< Коэфициент преобразования
#define COMPACT_CONFIG_SYNCH           0xC006  //< Коэфициент преобразования

#define COMPACT_CONFIG_STATUS          0xC007  //< Коэфициент преобразования

#define COMPACT_CONFIG_ADDR          0xC001  //< Адрес регистра для сброса данных

#define CALIBR_ETLVOL_ADDR          0xC101  //< Эталон объем
#define CALIBR_TIME_ADDR            0xC201  //< Время
#define CALIBR_IMP_ADDR          	0xC301  //< Импульсы
#define CALIBR_COEF_ADDR          	0xC401  //< Коэфициенты
// предыдущих измерений
// ---------------------------------------------------------------------------
// Определение структуры данных условий испытаний
typedef struct TConditions {
	float tempAir; // < Температура воздуха (°C)
	float tempWater; // < Температура рабочей жидкости (°C)
	uint32_t pressureAir; // < Атмосферное давление (кПа)
	uint32_t pressureWater; // < Давление рабочей жидкости (кПа)
	uint32_t humidityAir; // < Относительная влажность воздуха (%)
	uint32_t densityWater; // < Плотность рабочей жидкости (кг/м3)
} Conditions_t, *pConditions_t;
//
// Значения по умолчанию для каждого из полей
#define COND_TEMP_AIR_DEFAULT         21.5f
#define COND_TEMP_WATER_DEFAULT       21.5f
#define COND_PRESS_AIR_DEFAULT       100U
#define COND_PRESS_WATER_DEFAULT     300U
#define COND_HUM_AIR_DEFAULT          50U
#define COND_DENS_WATER_DEFAULT     1000U
//
// Определение значений по умолчанию для инициализации структуры
#define CONDITIONS_INIT_DEFAULT  \
{                                \
  COND_TEMP_AIR_DEFAULT,         \
  COND_TEMP_WATER_DEFAULT,       \
  COND_PRESS_AIR_DEFAULT,        \
  COND_PRESS_WATER_DEFAULT,      \
  COND_HUM_AIR_DEFAULT,          \
  COND_DENS_WATER_DEFAULT,       \
};

#define IMP_BUFFER_LEN     100U

// ---------------------------------------------------------------------------
// Перечисление возможных состояний устройства
typedef enum EHscStates {
		HSC_STATE_STANDBY = 0,
	// < Подключение с bluetooth устройством не установлено
        HSC_STATE_CONFIG,  // После подключения считываем настройки
		HSC_STATE_PING,// < К Bluetooth устройству подключились, пингуем для контроля соединения
		HSC_STATE_STARTTEST, // < Послана команда для запуска счёта
		HSC_STATE_STOPTEST, // < Послана команда для остановки счёта
		HSC_STATE_EXECUTE, // < Выполнение основной операции
        HSC_STATE_MONITOR ,
	// HSC_STATE_UPDATE,        //<Обновление данных
		HSC_STATE_COMPLETE, // < Завершение выполнения основной операции
		HSC_STATE_FINALREAD, // < Считывание финальных данных
		HSC_STATE_FAILURE // < Сбой в работе устройства
	} HscStates_t;

//
// Функция обратного вызова - при смене состояния устройства
typedef void __fastcall(__closure * TOnChangeState)(System::TObject * Sender);
//
// Функция обратного вызова - при получении очередной порции данных при выполнении проливки
typedef void __fastcall(__closure * TOnMeasureDataRecv)(TObject * Sender,
	TotalData_t & Data);

// Функции обратного вызова для чтения данных из устройства
//
/*
 //
 //Свойства запуска измерения прочитаны
 typedef void __fastcall (__closure *TOnVersionRead)(TObject *Sender, tVersion Ver);

 //
 //Свойства запуска измерения прочитаны
 typedef void __fastcall (__closure *TOnSettingsRead)(TObject *Sender, tSettings Settings);

 //
 //Можно считывать состояние проливки
 typedef void __fastcall (__closure *TOnCheckStateRead)(TObject *Sender, tCheckState CheckState);

 //
 //Началось измерение
 typedef void __fastcall (__closure *TOnTestBegin)(TObject *Sender, TotalData_t &Data);

 //
 //Закончилось измерение
 typedef void __fastcall (__closure *TOnTestEnd)(TObject *Sender, TotalData_t &Data);

 //
 //Можно считывать промежуточные данные
 typedef void __fastcall (__closure *TOnSecDataRead)(TObject *Sender, tSecData Data);
 */
//
// Можно считывать итоговые данные
typedef void __fastcall (__closure * TOnDataRead)(TObject * Sender);

//
// Можно считывать настройки каналов
typedef void __fastcall (__closure * TOnCHSettingsRead)(TObject * Sender,
	TotalData_t & Data);



// ---------------------------------------------------------------------------
// Класс устройства HSC наследуем от TObject чтоб упростить взаимодействие
// с формой пролиожения
class THscDevice : public TModBusDevice {
public:
	THscDevice();
	virtual __fastcall ~THscDevice();
	//
	void AfterConnect(void);
	//
	// Установить функцию при смене состояния
	void SetOnChangeState(TOnChangeState RoutinePtr) {
		OnChangeState = RoutinePtr;
	}

	void  Disconnect(void);

	//
	// Установить Call-back при приеме данных измерения
	void SetOnMeasureDataRecv(TOnMeasureDataRecv RoutinePtr) {
		OnDataReceive = RoutinePtr;
	}

	//
	// Функция получения текущих значений условий испытаний
	void GetConditions(Conditions_t &Cond) const {
		memcpy(&Cond, &Conditions, sizeof(Conditions_t));
	}

	//
	// Функция установки значений условий испытаний
	void SetConditions(const Conditions_t &Cond) {
		memcpy(&Conditions, &Cond, sizeof(Conditions_t));
	}

	//
	// Функция получения данных об устройстве
	void GetDeviceInfo(DeviceInfo_t &info) const {
		memcpy(&info, &DeviceInfo, sizeof(DeviceInfo_t));
	}

	//
	// Функция запуска проливки
	void StartMeasure(uint16_t time);
	void StartMeasureInstant(uint16_t time);
	//
	// Передача фотографии начала проливки
	void SetPhotoBeforeVerification(TBitmap *Photo);
	//
	// Передача фотографии после завершения проливки
	void SetPhotoAfterVerification(TBitmap *Photo);

	//
	// Функция получения текущего состояния HSC устройства
	HscStates_t GetState(void) const {
		return hscState;
	}
	//
	// Функция сохранения результатов поверки
	void SaveVerificationResults(void);

	// Переменные устройства HSC-COMPACT
	//
	//

	// Текущее время
	void StopMeasure(void);

	int16_t CurTime;
	// Реальное время

	float RealTime;
	//
	// Перечисление возможных состояний устройства

	// Получить среднее количество импульсов по каналу CH.
	// Период Среднего вычисляется таким образом, чтобы для расчёта было бы хотя
	// бы 100 импульсов. Набирается 100 импульсов и потом на основе этой 100 вычисляется среднее.
	float GetAvrImpulses(uint8_t Channel);
	uint8_t GetMaxChannel(void);
	float GetTotalImp(uint8_t Channel);
	float GetSecImp(uint8_t Channel);
	float GetTime(void);
	float GetWidth(uint8_t CH);
	float GetFinalImp(uint8_t Channel);

	float GetFinalTime(void);

	void StartTest(void);

	HscStates_t hscState;

	// Прочитать версию
	void SetOnVersionRead(TOnDataRead OnVerRead);
	void Read_Version();

	//
	// Настройки измерения
	void SetOnSettingsRead(TOnDataRead OnSettingsRead);
	void Read_Settings(); // Настройки готовы к считыванию,
	// обновлены после прочтения.

	void Write_Settings(tSettings *vSettings);
    void Write_ImpLimit(uint16_t ImpLimit);
	void Write_TimeLimit(uint16_t TimeLimit);
	void Write_Channel_State(uint16_t State);
	void Write_Channel_State(uint8_t Channel, uint16_t State);
	void Write_SYNCH(int syn);

    void Write_Coef(float K);
	//
	// Настройки измерения
	void SetOnCheckStateRead(TOnDataRead OnCheckStateRead);
	void Read_CheckState(); // Настройки готовы к считыванию,
	// обновлены после прочтения.

	int WriteCheckState(tCheckState *pCheckState);

	//
	// Чтение данных посекундно
	void SetOnSecDataRead(TOnDataRead OnDataRead);

	void SetOnImpDataRead(TOnDataRead OnDataRead);

	void Read_SecData(); // Настройки готовы к считыванию,
	// обновлены после прочтения.

	// Вызвается после чтения объема. В ней можно уже снимать все данные
	void SetOnDataRead(TOnDataRead OnDataRead);
	void Read_Data(); // Настройки готовы к считыванию,
	// обновлены после прочтения.

	// Вызвается после чтения объема. В ней можно уже снимать все данные
	void SetOnFinalDataRead(TOnDataRead OnDataRead);
	void Read_FinalData(); // Настройки готовы к считыванию,
	// обновлены после прочтения.
    void Read_FinalTime();

	// Вызвается после чтения конфигурации.
	void SetOnConfigRead(TOnDataRead OnDataRead);
	void SetOnConfigMainRead(TOnDataRead OnDataRead);
	void SetOnAccessRead(TOnDataRead OnDataRead) {OnAccessRead = OnDataRead;};
	void SetOnChargeRead(TOnDataRead OnDataRead);
	void SetOnWidthRead(TOnDataRead OnDataRead){OnWidthRead = OnDataRead;};

	void Read_ConfigData(); // Настройки готовы к считыванию,
	// обновлены после прочтения.

     void Read_ConfigAccess(void);

     void Read_PIN(void);

     void Save_PIN(int pin);
	 void Write_PIN(int pin);
	 void Write_STATUS(int status);

	  void Save_CalibrData(float cEtlVolume[20], float cTime[20], float cImp [20], float cCoefs[20]);
	  void Save_Flash(void);
      void Load_Flash(void);

	  void StartMonitor(void);
	void StopMonitor(void);

    	//
	// Функция смены состояния устройства
	void ChangeState(HscStates_t newState);


	//
	// Настройки канала
	TChannelSettings ChannelSetrings[15];

	//
	// Запуск измерения

	tVersion Version;
	tCheckState CheckState;
	tSettings Settings;
	tSecData SecData;
	tCumulativeData CumData;
	tFinalData FinalData;

    bool PingEnable;
    bool PingOff(void) { PingEnable = false; return false;};
    bool PingOn(void)  { PingEnable = true; return true; };

 	//
	// Функция выполнения пинга (выполняется в состоянии HSC_STATE_PING)
	void PingDevice(void);
    void ConfigDevice(void);
    //void ConfigDevice(void);
    //void SaveConfig(void);

	float cEtlVolume[20], cTime[20], cImp[20], cCoef[20];
    uint16_t Password1,Password2,Password3;
    float Kp;
    uint8_t SYNCH;
    uint8_t Access;
	uint8_t Temp;

	 uint8_t StartType = 1;
	 uint8_t StopType = 1;


	uint16_t Charge;
	int GetValueCharge(void);

	bool CalibrState = false;
	bool PeriodState = false;

   UnicodeString GetCharge(void);

protected:


	//
	// Структура данных об условиях проведения поверки
	Conditions_t Conditions;
	//
	// Таймер где крутится основная логика работы устройства
	static TTimer *mainTimer;
	//
	// Объект TBitmap для сохранения фото перед проливкой
	Fmx::Graphics::TBitmap *PhotoBefore;
	//
	// Объект TBitmap для сохранения фото после выполнения проливки
	Fmx::Graphics::TBitmap *PhotoAfter;

	//
	// Структура данных о версии устройства
	DeviceInfo_t DeviceInfo;
	//
	// Данные выполнения измерения
	TotalData_t verifyData;
	//
	// Время в секундах выполнения проливки
	uint16_t verifyTime;



	//
	// При смене состояния
	TOnChangeState OnChangeState;
	//
	// При приеме очередных данных измерения
	TOnMeasureDataRecv OnDataReceive;


	//
	// Функция обработки принятого пакета
	void PacketHandler(void);
	//
	// Обработчик срабатывания таймера ожидания ответа
	void __fastcall OnMainTimer(TObject *Sender);

	//
	// Обработчик таймера времени ожидания ответа от устройства
	void __fastcall OnRespTimer(TObject *Sender);


	//
	// Функция выполнения опроса устройства во время выполнеия проливки
	void MeasureExecute(void);
	void MonitorExecute(void);

	//
	// Сброс данных о предыдущих измерениях в устройстве
	void ResetDeviceData(void);

	// Обратные вызовы HSC
	// void __fastcall  OnVersionRead(TObject *Sender, tVersion Ver);

	/*
	 TOnVersionRead OnVersionRead;
	 //Структура через которую производится
	 //задание и чтение настроен.
	 TOnSettingsRead OnSettingsRead;

	 TOnCheckStateRead OnCheckStateRead;

	 TOnSecDataRead OnSecDataRead;
	 */

	TOnDataRead OnDataRead;
	TOnDataRead OnVersionRead;
	TOnDataRead OnSettingsRead;
	TOnDataRead OnCheckStateRead;
	TOnDataRead OnSecDataRead;
	TOnDataRead OnImpDataRead;
	TOnDataRead OnFinalDataRead;
    TOnDataRead OnConfigRead;
	TOnDataRead OnConfigMainRead;
	TOnDataRead OnAccessRead;
	TOnDataRead OnChargeRead;
	TOnDataRead OnWidthRead;
	// Archive Impulses of last 100 sec
	uint16_t Impulses[16][IMP_BUFFER_LEN];
	float    Widths[16][IMP_BUFFER_LEN];

	uint8_t rd_imp;
	uint8_t cnt_imp;
	uint8_t wr_imp;

	uint8_t rd_width;
	uint8_t cnt_width;
	uint8_t wr_width;

	uint16_t data;

private:



};
// ---------------------------------------------------------------------------
#endif  // THscDeviceH
// ---------------------------------------------------------------------------
