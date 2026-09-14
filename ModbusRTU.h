//---------------------------------------------------------------------------
#ifndef ModbusRTUH
#define ModbusRTUH
//---------------------------------------------------------------------------
#include <stdint.h>
#include <System.SysUtils.hpp>
//---------------------------------------------------------------------------
// Объявление некоторых кодов функций для протокола Modbus RTU
#define FC_READ_REGISTERS    0x03  //< Чтение значений из нескольких регистров
#define FC_WRITE_REGISTERS   0x10  //< Запись значений в несколько регистров

#define FC_ERROR_FLAG        0x80  //< Флаг-признак пакета с кодом ошибки Modbus RTU
//---------------------------------------------------------------------------
// Структура заголовка пакета запроса Modbus RTU
typedef struct TMRtuHeaderReq
{
  uint8_t   slaveAddress;        //< Адрес ведомого устройства
  uint8_t   functionCode;        //< Код функции Modbus RTU
  uint16_t  startAddress;        //< Начальный адрес
  uint16_t  wordCount;           //< Количество элементов

} MRtuHeaderReq_t, *pMRtuHeaderReq_t;
//
// Структура заголовка пакета ответа Modbus RTU
typedef struct TMrtuHeaderResp
{
  uint8_t   slaveAddress;        //< Адрес ведомого
  uint8_t   functionCode;        //< Код функции
  uint8_t   length;              //< Количество байт в ответе
} MrtuHeaderResp_t, *pMrtuHeaderResp_t;
//---------------------------------------------------------------------------
class CModbusRTU
{
  public:
    CModbusRTU();
    ~CModbusRTU();

    static void ReadRegisters(TBytes &txFrame, uint8_t slaveAddr, uint16_t startAddress, uint16_t count);

    static void WriteRegisters(TBytes &txFrame, uint8_t slaveAddr, uint16_t startAddress, const TBytes &txBytes);

    static bool IsCorrectFrame(const TBytes &rxFrame);

  protected:

  private:
    //
    // Таблица для расчета CRC16
    static const uint16_t CRCTable[256];
    //
    // Функция расчета CRC16
    static uint16_t CalculateCRC16(const void *vpData, uint32_t dataLen);
};
//---------------------------------------------------------------------------
#endif  // ModbusRTUH
//---------------------------------------------------------------------------
