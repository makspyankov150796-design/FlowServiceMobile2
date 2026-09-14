//---------------------------------------------------------------------------
#ifndef HscTypedefs_h
#define HscTypedefs_h
//---------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
//---------------------------------------------------------------------------
#define PULSE_CHANNEL_COUNT  5  //< Количество импульсных каналов
//---------------------------------------------------------------------------
// Структура сведений об устройстве
typedef struct TDeviceInfo
{
  uint16_t DeviceRevision;
  uint16_t ProtocolVersion;
  uint16_t deviceAddr;
} DeviceInfo_t, *pDeviceInfo_t;
//
// Структура данных текущих промежуточных данных о состоянии проливки
typedef struct TMeasureData
{
  uint16_t time;                               //< Текущее время проливки
  uint16_t state;                              //< Текущее состояние проливки
  uint16_t pulsesCount[PULSE_CHANNEL_COUNT];   //< Количество импульсов за последнюю секунду
} MeasureData_t, *pMeasureData_t;
//
// Общая структура данных измерения
typedef struct TTotalData
{
  MeasureData_t MeasureData;                    //< Промежуточные данные

  uint32_t totalPulsesCnt[PULSE_CHANNEL_COUNT]; //< Общее количество импульсов за измерение

  uint32_t  error[PULSE_CHANNEL_COUNT];         //< Погрешность по каждому каналу

} TotalData_t, *pTotalData_t;
//
// Определение текущих состояний проливки
// Поле "state" структуры "MeasureData_t"
#define MEASURE_STATE_READY     0
#define MEASURE_STATE_EXECUTE   1
#define MEASURE_STATE_COMPLETE  2
//---------------------------------------------------------------------------


// В каком состоянии блок переменных
typedef enum ESTATUSSETTINGS
{
  EMPTY = 0,   				//< Не заполнен
  WROTE,
  READYTOWRITE,          	//< Измене, Заполнен и приготовлен к записи
  READYTOREAD,       		//< Прочитан и может быть занесен в интерфейс
  READERROR,      			//< Ошибка причтении
  READYTOSAVE        		//< Подготовлен для сохранения в файл
} eStatus;

//---------------------------------------------------------------------------
// Перечисление возможных состояний старта
typedef enum EStartSettings
{
  START_SETTINGS_BLOCK = 0,   //< Блокировка
  START_SETTINGS_NORMAL,          //<запуск счета импульсов после записи в ячейку 0x0005 1цы или достижения ограничений
  START_SETTINGS_START,       //<Мгновенный Безусловный запуск проливки
  START_SETTINGS_STARTRISE_INT,      //< Старт проливки по фронту сигнала внутренней синхронизации
  START_SETTINGS_STARTFALL_INT,		//< Старт проливки по спаду сигнала внутренней синхронизации
  START_SETTINGS_STARTRISE_EXT,		//< Старт проливки по фронту сигнала внешней синхронизации
  START_SETTINGS_STARTFALL_EXT,		//< Старт проливки по спаду сигнала внешней синхронизации
  START_SETTINGS_STARTRISE_CH,		//< Старт проливки по первому фронту X-го канала
  START_SETTINGS_STARTFALL_CH,		//< Старт проливки по первому спаду X-го канала
  START_SETTINGS_STARTPAUS_CH		//< Старт при начале пачки импульсов (для импульсного выхода)
} eStartSettings;

typedef enum EStopSettings
{
  STOP_SETTINGS_BLOCK = 0,   //< Блокировка
  STOP_SETTINGS_NORMAL = 1,          //<запуск счета импульсов после записи в ячейку 0x0005 1цы или достижения ограничений
  STOP_SETTINGS_STOP = 2,       //<Мгновенный Безусловный запуск проливки
  STOP_SETTINGS_STOPFALL_INT = 3,      //< Старт проливки по фронту сигнала внутренней синхронизации
  STOP_SETTINGS_STOPRISE_INT = 4,		//< Старт проливки по спаду сигнала внутренней синхронизации
  STOP_SETTINGS_STOPFALL_EXT=5 ,		//< Старт проливки по фронту сигнала внешней синхронизации
  STOP_SETTINGS_STOPRISE_EXT =6,		//< Старт проливки по спаду сигнала внешней синхронизации
  STOP_SETTINGS_STOPRISE_CH = 7 ,		//< Старт проливки по первому фронту X-го канала
  STOP_SETTINGS_STOPFALL_CH = 8,		//< Старт проливки по первому спаду X-го канала
  STOP_SETTINGS_STOPRISECOND_CH = 9,		//< Старт при начале пачки импульсов (для импульсного выхода)
  STOP_SETTINGS_STOPFALLCOND_CH = 10,		//< Старт при начале пачки импульсов (для импульсного выхода)
  STOP_SETTINGS_STOPPAUSECOND_CH = 9,		//< Старт при начале пачки импульсов (для импульсного выхода)
} eStopSettings;

typedef enum EExtSyn
{
  EXTSYN_SETTINGS_BLOCK = 0,   //< Блокировка
  EXTSYN_SETTINGS_NORMAL = 1,          //<запуск счета импульсов после записи в ячейку 0x0005 1цы или достижения ограничений
  EXTSYN_SETTINGS_STARTRISESTOPFALL,      //< Старт проливки по фронту сигнала внутренней синхронизации
  EXTSYN_SETTINGS_STARTFALLSTOPRISE,		//< Старт проливки по спаду сигнала внутренней синхронизации
  EXTSYN_SETTINGS_STARTRISESTOPRISE,      //< Старт проливки по фронту сигнала внутренней синхронизации
  EXTSYN_SETTINGS_STARTFALLSTOPFALL,		//< Старт проливки по спаду сигнала внутренней синхронизации
} eExtSyn;



typedef struct TVersion
{
  UnicodeString	 	Version;       //< Ограничение по времени
  int 				Protocol;        //< Ограничение по импульсам
} tVersion;


typedef struct TSettings
{
  eStatus           	Status;          //Статус переменных
  bool              	Simulator;
  eStartSettings		StartSettings;  //<Настройки старта
  uint8_t               StartCHNum;      //Номер канала для старта
  eStopSettings     	StopSettings;   //<Настройки остановки измерения
  uint8_t               StopCHNum;      //Номер канала для старта
  uint16_t	 			TimeLimit;      //< Ограничение по времени
  uint32_t 				ImpLimit;       //< Ограничение по импульсам
  bool              	IntSyn;
  eExtSyn           	ExtSyn;         //< Внешняя синхронизация
  uint8_t	 			OutSynCH;       //< Выход синхронизации
  uint8_t	 			EtalonCH;       //<
  uint8_t	 			Errors;       //<
  uint8_t               Reset;          //< Сброс предыдущих настроек
} tSettings;

typedef struct TChannelSettings
{
  int	 			ID;       //< ИД Канала
 // EFreqCH	        tFreqCH;  //< Частота канала: Высокочастотный/Низкочастотный
//  EImpCH            tImpCH ;   // Импульный иои Частотный канал
 // ETypeCH           tTypeCH;   //< Тип Выхода: Пассивный/активный
  int	 			FilterEnable;       //< Включен Фильтр дребезга
  int	 			FilterTime;       //<   Время Фильтра
  int 				FreqLimit;        //< Предел частоты
  EExtSyn           WidthLimit;          //< Предел длительности

} tChannelSettings;

 typedef struct TCheckState
{
  eStatus           Status;        //Статус переменных
  bool              Reset;
  int               Num;
  int	        	State;         //<Состоние
  int 	 			Time;          //Секунда
  bool              Over;
}tCheckState;


typedef struct TSecData
{
  int	 			Time;          //Секунда
  int	        	State;         //<Состоние
  int	            ImpPerSec[15]; //Импульсов за последнюю секунду
  float	            Width[15]; //Импульсов за последнюю секунду
}tSecData;

typedef struct TCumulativeData
{
  int	 			Time;          //< Время
  int	        	State;         //< Состояние
  unsigned long int ImpTotal[15]; //< Имп нарастающим итогом
} tCumulativeData;

typedef struct TFinalData
{
  float	 			Time;          //< Время реальное
  float	            ImpFinal[15]; // Итоговое время
}tFinalData;















#endif // HscTypedefs_h
//---------------------------------------------------------------------------

