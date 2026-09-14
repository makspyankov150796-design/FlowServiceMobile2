//---------------------------------------------------------------------------

#pragma hdrstop

#include "TModBusDevice.h"
#include "TDebug.h"


//TTimer *TModBusDevice::respTimer;


//---------------------------------------------------------------------------
TModBusDevice::TModBusDevice() {

	btThread = new TBluetoothThread(false);
	btThread->Initialize();

	btThread->OnDataReceive = ReceiveDataParser;
	btThread->OnConnect = OnBtConnect;        //???????????????????????

	btThread->OnRespError = OnRespTimer;

	rxFrame.set_length(0);

	respTimer = new TTimer(NULL);
	respTimer->Enabled = false;
	respTimer->Interval = RESPONSE_WAIT_TIMEOUT;
	respTimer->OnTimer = OnRespTimer;

	OnBluetoothConnect = OnBtConnect;

	txTicks = 0;
	measureTimeTicks = 0;


	respList.clear();

}

// ---------------------------------------------------------------------------
__fastcall TModBusDevice::~TModBusDevice() {
	delete btThread;


}

void TModBusDevice::AfterConnect(void)
{


}

// ---------------------------------------------------------------------------
void TModBusDevice::DiscoverBluetoothDevices(void) {
 //Проверка наличия БлюТуф
	btThread->DiscoverDevices();
}

// ---------------------------------------------------------------------------
void TModBusDevice::SetOnDiscoverEnd(TDiscoveryEndEvent OnDiscoverEndRoutine) {
	btThread->OnDiscoverEnd = OnDiscoverEndRoutine;
}

void TModBusDevice::SetOnPairedDiscoverEnd(TOnDiscoverEnd OnDiscoverEndPairedRoutine) {
	btThread->OnDiscoverEndPaired = OnDiscoverEndPairedRoutine;
}

 void TModBusDevice::SetOnDataSent(TOnDataSent onDataSent)
{
   btThread->OnDataSent = onDataSent;
}

void TModBusDevice::PacketAnalyse(void)
{
	uint8_t i;
	uint8_t index = 3;
	uint16_t temp;
    uint8_t debug_index = 3;
	respTimer->Enabled = false;
	respTimer->Interval = RESPONSE_WAIT_TIMEOUT;
	//btThread->IsReciving = false;

	do {
		//
		// Если ничего от устройства не ждем, то и разбирать ничего не будем...
		if (respList.empty())
        {
		   Debug-> MSG.Type=L"ENT";
		   Debug-> MSG.Direction=L"PacketHandler";
		   //Debug->MSG.Data=IntToStr(frameLen);
		   Debug->AddMSG (L"Если ничего от устройства не ждем, то и разбирать ничего не будем...");
		   break;
        } else

        {
                Debug->MSG.Type=L"ENT";
            	Debug->MSG.Direction=L"PacketHandler";
                for (uint8_t i = 0; i < rxFrame.Length; i++) {
				Debug->MSG.Data=Debug->MSG.Data+L", "+IntToStr(rxFrame[i]);
                }

				 if (rxFrame.Length>6) {
				 Debug->data[0] =  rxFrame[3];
				Debug->data[1] =  rxFrame[4];
				Debug->data[2] =  rxFrame[5];
				Debug->data[3] =  rxFrame[6];
				 }


                Debug->Int   =  RegHandler_uint16(rxFrame, &debug_index);
                debug_index = 3;
                Debug->Float =  RegHandler_float(rxFrame, &debug_index);

                Debug->state = 1;

            	Debug->AddMSG (L"Данные к обработке: ");

                Debug->MSG.Type=L"ENT";
            	Debug->MSG.Direction=L"PacketHandler";

                Debug->MSG.Data= L"ID= "+IntToStr(respList.begin()->ID) + L", Адрес регистра: " + IntToStr(respList.begin()->registerAddr);
                Debug->MSG.Data= Debug->MSG.Data+L", Код функции: "+IntToStr(respList.begin()->codeFunction);

            	Debug->AddMSG (L"Данные ожидаемые: ");



        }
		//
		do {
			// Проверка на соответствие кода функции
			if (respList.begin()->codeFunction != rxFrame[1])
            {
            Debug->MSG.Type=L"ENT";
            Debug->MSG.Direction=L"PacketHandler";
            //Debug->MSG.Data=IntToStr(frameLen);
            Debug->AddMSG (L"Проверка на соответствие кода функции не пройдена!!!");
            break;
            }
			//
			// Если приняли ответ с кодом ошибки
			if (rxFrame[1] & FC_ERROR_FLAG) {

            Debug->MSG.Type=L"ENT";
            Debug->MSG.Direction=L"PacketHandler";
            //Debug->MSG.Data=IntToStr(frameLen);
            Debug->AddMSG (L"Приняли ответ с кодом ошибки");
			break;
			}
			else if (FC_READ_REGISTERS == rxFrame[1])
				// < Если просили значения регистров
			{
			   PacketHandler();
			}
			else if (FC_WRITE_REGISTERS == rxFrame[1])
				// < Если писали значения регистров
			{
				Debug->MSG.Type="ENT";
				Debug->MSG.Direction="PacketHandler";
				//Debug->MSG.Data=IntToStr(frameLen);
				Debug->AddMSG (L" Если писали значения регистров");
				uint16_t baseAddr = 0;

				baseAddr = (static_cast<uint16_t>(rxFrame[2]) << 8);
				baseAddr |= (static_cast<uint16_t>(rxFrame[3]));
			}

		}
		while (0);

		respList.pop_front();



		// < Убираем обработанный элемент из списка ожидания
        Debug->MSG.Type="ENT";
        Integer cnt= respList.size();
        Debug->MSG.Data=L"Кол-во элементов в списке: "+  IntToStr(cnt);
		Debug->AddMSG (L" Убираем обработанный элемент из списка ожидания");
		//
		// Если еще ждем какие-либо данные, то запустим таймер ожидания
		//respTimer->Enabled = !respList.empty();



        if (!respList.empty())
		{
	   //btThread->IsReciving = true;
        Debug->MSG.Type="ENT";
		Debug->AddMSG (L"еще ждем какие-либо данные, запустим таймер ожидания");
		}  else
		{
		//btThread->IsReciving = false;
		Debug->MSG.Type="ENT";
		Debug->AddMSG (L"Не ждем какие-либо данные, таймер ожидания останавливаем");
		}
	}
	while (0);
}





void TModBusDevice::PacketHandler(void)
{


}

// ---------------------------------------------------------------------------
// Обработчик таймера времени ожидания ответа от устройства
void __fastcall TModBusDevice::OnRespTimer(TObject *Sender) {
		respTimer->Enabled = false;
}

bool TModBusDevice::GetBtPermission(void)
{
    CheckBtPermission();
    return btThread->btPermissionGranted;
}


void TModBusDevice::CheckBtPermission(void)
{
    btThread->CheckBtPermission();
}

uint8_t TModBusDevice::Connect() {

	 return	btThread->Connect(DeviceBtIndex );
}


// ---------------------------------------------------------------------------
uint8_t TModBusDevice::Connect(uint32_t deviceIndex) {
	 DeviceBtIndex = deviceIndex;
	 return	btThread->Connect(deviceIndex);
}

// ---------------------------------------------------------------------------
void TModBusDevice::Disconnect(void) {
	respTimer->Enabled = false;
	btThread->IsReciving = false;
	respList.clear();
	ChangeStateDevice(STATE_STANDBY);
	btThread->Disconnect();
}

// ---------------------------------------------------------------------------
void TModBusDevice::SetOnBluetoothConnect(TOnBluetoothConnect OnBtConnect) {
	OnBluetoothConnect = OnBtConnect;
    //btThread->OnConnect = OnBtConnect;
}

void TModBusDevice::SetOnBluetoothConnectMain(TOnBluetoothConnect OnBtConnect) {
	OnBluetoothConnectMain = OnBtConnect;
}

// ---------------------------------------------------------------------------
void TModBusDevice::SetOnBluetoothDisconnect(TNotifyEvent OnBtDisconnect) {
	btThread->OnDisconnect = OnBtDisconnect;
}

// ---------------------------------------------------------------------------
void TModBusDevice::OnWdtOverflow(TNotifyEvent WdtRoutine) {
	btThread->OnDiscoverWdt = WdtRoutine;
}

// ---------------------------------------------------------------------------
bool TModBusDevice::IsBluetoothConnected(void) const {
	bool connected = false;

	if (btThread != nullptr) {
		connected = btThread->IsConnected();
	}

	return connected;
}

// ---------------------------------------------------------------------------
void __fastcall TModBusDevice::ReceiveDataParser(TObject *Sender,
	const TBytes &rxData) {
	uint32_t index = rxFrame.Length;
	uint16_t frameLen = 0;
	static Int64 prewTicks = 0;
	const Int64 ticks = TThread::GetTickCount();
	//
	// Если предыдущие данные приходили очень давно, то надо сбросить индексы...
	if ((ticks - prewTicks) >= RECEIVE_DATA_TIMEOUT) {

		Debug->MSG.Type="RRD";
		Debug->MSG.Direction="ReceiveDataParser";

        Debug->MSG.Data=L" ticks= "+IntToStr(ticks)+L", prewTicks= "
        							+IntToStr(prewTicks)+L", Diff="+IntToStr(ticks - prewTicks)+
                                    L", RECIVE TIME OUT="+IntToStr(RECEIVE_DATA_TIMEOUT);
        Debug->AddMSG (L"ОЧИСТКА БУФЕРА");
		rxFrame.set_length(0);
		index = 0;
		frameLen = 0;

	}   else
    {
        Debug->AddMSG (L"AddData Не вышло ожидание: Продолжение пакета");
    }

	prewTicks = ticks;

	for (uint32_t i = 0; i < rxData.Length; i++) {
		rxFrame.set_length(rxFrame.Length + 1);
        rxFrameArray[index]= rxData[i];
		rxFrame[index++] = rxData[i];

		//
		// Если длину принимаемого сообщения еще не выяснили...
 		if (0 == frameLen && index >= 2) {
			if (FC_READ_REGISTERS == rxFrame[1] && index >= 3) {
				// Размер всего фрейма = размер заголовка + размер данных + размер CRC16
				frameLen = rxFrame[2]+sizeof(MrtuHeaderResp_t)+sizeof(uint16_t);

                 Debug->MSG.Type="ENT";
       			 Debug->MSG.Direction=L"FC_READ_REGISTERS == rxFrame[1]";
                 Debug->MSG.Data=IntToStr(frameLen);
        		 Debug->AddMSG ("LEN");

			}
			else if (FC_WRITE_REGISTERS == rxFrame[1]) {
				frameLen = 8;
                 Debug->MSG.Type="ENT";
       			 Debug->MSG.Direction=L"FC_WRITE_REGISTERS == rxFrame[1]";
                 Debug->MSG.Data=IntToStr(frameLen);
        		 Debug->AddMSG ("LEN");
			}
            else if (rxFrame[1] > 0x80)
            {
            	frameLen = 5;
                  Debug->MSG.Type="RRD";
       			 Debug->MSG.Direction=L"FC_ERROR_FLAG";
                 Debug->MSG.Data=L"RX FRAME: 0:"+IntToStr(rxFrame[0])+L", 1:"+IntToStr(rxFrame[1]);
        		 Debug->AddMSG (L"ПРИШЛО ОПОВЕЩЕНИЕ ОШИБКИ");


            }
		}

		else if (index >= frameLen && frameLen != 0) {
			// Приняли весь пакет, разберем его...
			Debug->AddMSG (L"Приняли весь пакет, разберем его...");
			if (CModbusRTU::IsCorrectFrame(rxFrame) == true) {
				Debug->AddMSG (L"IsCorrectFrame(rxFrame) = true");

				PacketAnalyse();
				
                btThread->btState = btThread->BT_READY;
				btThread->IsReciving = false;

			   //	TThread::Synchronize(NULL, Debug->OnRecieveCmplt);

			} else
			{
                                 Debug->MSG.Type="ENT";
       			 Debug->MSG.Direction=L"FC_READ_REGISTERS == rxFrame[1]";
                 Debug->MSG.Data=IntToStr(frameLen);
                Debug->AddMSG (L"IsCorrectFrame(rxFrame) = FALSE !!!!");

            }

			rxFrame.set_length(0);
			index = 0;
			frameLen = 0;
		}
	} // for(uint32_t i = 0; i < rxData.Length; i++)
}
// ---------------------------------------------------------------------------

float TModBusDevice::RegHandler_float(TBytes rxFrame, uint8_t *index) {
	float float_ptr;

	uint32_t data;
    uint8_t len;

    len = rxFrame.Length;

    if (len>*index+3)
              {
	data =  (static_cast<uint32_t>(rxFrame[(*index)++]) << 8 );// &  0xFF000000);
	data |= (static_cast<uint32_t>(rxFrame[(*index)++])      );// & 0x00FF0000);
	data |= (static_cast<uint32_t>(rxFrame[(*index)++]) << 24);// &  0x0000FF00);
	data |= (static_cast<uint32_t>(rxFrame[(*index)++]) << 16);     //; 			& 0x000000FF));

	memcpy(&float_ptr, &data, sizeof(data));

	return float_ptr;
    }   else
    {

    return 0;
     }

}

uint32_t TModBusDevice::RegHandler_uint32(TBytes rxFrame, uint8_t *index) {
	uint32_t data;

        uint8_t len;

    len = rxFrame.Length;

    if (len>*index+3)
{
  /*	data = ((static_cast<uint32_t>(rxFrame[(*index)++]) ) & 0xFF000000);
	data |= ((static_cast<uint32_t>(rxFrame[(*index)++]) << 16) & 0x00FF0000);
	data |= ((static_cast<uint32_t>(rxFrame[(*index)++]) << 8) & 0x0000FF00);
	data |= ((static_cast<uint32_t>(rxFrame[(*index)++]) << 24 & 0x000000FF));   */

    data =  (static_cast<uint32_t>(rxFrame[(*index)++]) << 8) ;
	data |= (static_cast<uint32_t>(rxFrame[(*index)++]) ) ;
	data |= (static_cast<uint32_t>(rxFrame[(*index)++]) << 24) ;
	data |= (static_cast<uint32_t>(rxFrame[(*index)++]) << 16 );



	return data;
    } else
    {
    return 0;
    }
}

uint16_t TModBusDevice::RegHandler_uint16(TBytes rxFrame, uint8_t *index) {
	uint16_t data;
    uint8_t len;

    len = rxFrame.Length;

    if (len>*index+1)
{
	data = ((static_cast<uint16_t>(rxFrame[(*index)++]) << 8) & 0xFF00);
	data |= ((static_cast<uint16_t>(rxFrame[(*index)++]) & 0x00FF));
       }

	return data;
}

uint8_t TModBusDevice::RegHandler_uint8(TBytes rxFrame, uint8_t *index) {
	uint8_t data;
    uint8_t len;
    uint8_t val=0;

    len = rxFrame.Length;


    if (len>*index+1)
{
    val = static_cast<uint8_t> (rxFrame[(*index)++]);
	//data = ((static_cast<uint8_t>(rxFrame[(*index)++]) & 0x00FF));
}
	return val;
}

bool TModBusDevice::RegHandler_bool(TBytes rxFrame, uint8_t *index) {
	bool data;
    (*index)++;
	data = (static_cast<bool>(rxFrame[(* index)++]) & 0x00FF);

	return data;
}

// ---------------------------------------------------------------------------
// Событие Когда ModBus устройство подключено!
void __fastcall TModBusDevice::OnBtConnect(TObject *Sender, bool connected) {
	// Передадим результаты выполнения подключения форме
	if (OnBluetoothConnect != nullptr) {
		OnBluetoothConnect(Sender, connected);
	}

	if (OnBluetoothConnectMain != nullptr) {
		OnBluetoothConnectMain(Sender, connected);
	}
	//
	// Если подключение установлено успешно, запустим основной таймер устройства
	// установим соответствующее состояние устройства
	if (connected) {


		ChangeStateDevice(STATE_CONNECT);
        AfterConnect();
//		mainTimer->Interval = MAIN_TIMER_INTERVAL;
//		mainTimer->Enabled = true;

	}
	else {
		ChangeStateDevice(STATE_STANDBY);
//		mainTimer->Enabled = false;
	}
}

// Запись одного регмтстра uint16_t
void TModBusDevice::WriteReg(uint16_t regAddr, uint16_t Data) {
	TBytes txData;

	txData.set_length(2);

	txData[0] = static_cast<uint8_t>((Data & 0xFF00) >> 8);
	txData[1] = static_cast<uint8_t>(Data & 0x00FF);

	WriteDataToDevice( regAddr, txData);

}

// Запись одного регмтстра uint8_t
 void TModBusDevice::WriteReg(uint16_t addr, uint16_t regAddr, uint8_t Data) {
	TBytes txData;

	txData.set_length(2);

	txData[0] = 0;
	txData[1] = Data;

	WriteDataToDevice( regAddr, txData);
}

// Запись одного регмтстра bool
void TModBusDevice::WriteReg(uint16_t addr, uint16_t regAddr, bool Data) {
	TBytes txData;

	txData.set_length(2);

	txData[0] = 0;
	txData[1] = static_cast<uint8_t>(Data);

	WriteDataToDevice(regAddr, txData);
}


void TModBusDevice::WriteReg(uint16_t regAddr, float Data) {

	WriteReg(0x00, regAddr, Data);
}

// Запись одного регмтстра float
void TModBusDevice::WriteReg(uint16_t addr, uint16_t regAddr, float Data) {

	TBytes txData;
    float  float_ptr;
    uint32_t val;

    float_ptr = Data;

    txData.set_length(4);

    memcpy(&val, &float_ptr, sizeof(val));

    txData[1] =   val & 0xFF;
    txData[0] =  (((val)>> 8));
    txData[3] =  (((val)>> 16));
	txData[2] =  (((val)>> 24));

	WriteDataToDevice(regAddr, txData);
}



void TModBusDevice::WriteReg(uint16_t regAddr, float *fData, int size) {

	TBytes txData;
    float  float_ptr;
    uint32_t val;
    uint8_t index;

	float *fptr;
   int len = size / sizeof(float);

   fptr = fData;

   //Label1->Text = IntToStr((int)sizeof(fData));

   txData.set_length(4*len);

   for (int i=0; i<len; i++)
   {

    float_ptr = *fptr;
    fptr ++;

    memcpy(&val, &float_ptr, sizeof(val));

    txData[1+i*4] =   val & 0xFF;
    txData[0+i*4] =  (((val)>> 8));
    txData[3+i*4] =  (((val)>> 16));
	txData[2+i*4] =  (((val)>> 24));
    }

	WriteDataToDevice(regAddr, txData);
}

void TModBusDevice::WriteReg(uint16_t addr, uint16_t regAddr, uint32_t Data) {
	TBytes txData;

	txData.set_length(4);

	txData[0] = static_cast<uint8_t>((Data & 0xFF00) >> 8);
	txData[1] = static_cast<uint8_t>(Data & 0x00FF);

	txData[2] = static_cast<uint8_t>((Data & 0xFF000000) >> 8);
	txData[3] = static_cast<uint8_t>(Data & 0x00FF0000);

	WriteDataToDevice(regAddr, txData);

}

void TModBusDevice::ChangeStateDevice(States_t newState) {

 //	if (OnChangeDeviceState != nullptr) {
 //		OnChangeDeviceState(this);
 // 	}

	State = newState;
}


// ---------------------------------------------------------------------------
// Функция отправки данных в устройство

void TModBusDevice::WriteDataToDeviceCallBack(uint16_t regAddr,
	TBytes &txBytes,uint8_t CallBack)
    {

	TBytes txFrame;
	ResponseData_t respData = {0, 0, 0, 0};
	TxMsgParam_t msgParam = MSG_PARAM_STD_RESPONSE;

	txFrame.set_length(0);


        Debug->MSG.Type="WD";
        Debug->MSG.Direction = "WriteDataToDevice";
        Debug->MSG.Data=L"regAdress: "+Debug->MSG.Data+IntToHex(regAddr);
        Debug->MSG.Data=Debug->MSG.Data+" txBytes: ";
        for (uint8_t i=0;i<txBytes.Length;i++)
        {
        Debug->MSG.Data=Debug->MSG.Data+IntToHex(txBytes[i])+", ";
        }
		Debug->AddMSG (L" Записываем данные");


	CModbusRTU::WriteRegisters(txFrame, DeviceAddr, regAddr, txBytes);

        Debug->MSG.Type="WD";
        Debug->MSG.Direction = "WriteDataToDevice";
        Debug->MSG.Data=L"tx frame: ";
        for (uint8_t i=0;i<txFrame.Length;i++)
        {
        Debug->MSG.Data=Debug->MSG.Data+IntToHex(txFrame[i])+", ";
        }
        Debug->AddMSG (L" Записываем фрэйм");

	respData.codeFunction = txFrame[1];
   //	respData.registerAddr = MEASURE_TIME_ADDR; ???
    respData.registerAddr = 0x00;
	respData.length = 8;

	 if (++RespID == 100) {RespID = 0;}
	respData.ID = RespID;
	msgParam.ID = RespID;

	respList.push_back(respData);

	if (false == respTimer->Enabled)
	{
	respTimer->Interval = RESPONSE_WAIT_TIMEOUT;
	//respTimer->Enabled = true;
	}

	Debug->MSG.Type="WD";
	Debug->MSG.Direction=L"respList OUTPUT";
	Debug->MSG.Data="ID= "+IntToStr(respData.ID)+", RegAddr= "+respData.registerAddr;
	Debug->AddMSG (L"Регистрация ответа:");

	//TThread::Synchronize(NULL, Debug->OnTransfer);

	msgParam.msgLength = txFrame.Length;
	msgParam.callback = CallBack;
	btThread->Transmit(txFrame, msgParam);
	txTicks = TThread::GetTickCount();

	}

void TModBusDevice::WriteDataToDevice(uint16_t regAddr,
	TBytes &txBytes)
	{
	if (IsBluetoothConnected())
	{
	TBytes txFrame;
	ResponseData_t respData = {0, 0, 0, 0};
	TxMsgParam_t msgParam = MSG_PARAM_STD_RESPONSE;

	txFrame.set_length(0);

		try
{
	  //  Debug->MSG.Type="WD";
	 //   Debug->MSG.Direction = "WriteDataToDevice";
	  //	Debug->MSG.Data=L"regAdress: "+Debug->MSG.Data+IntToStr(regAddr);
	 //   Debug->MSG.Data=Debug->MSG.Data+" txBytes: ";
		for (uint8_t i=0;i<txBytes.Length;i++)
		{
	 //   Debug->MSG.Data=Debug->MSG.Data+IntToStr(txBytes[i])+", ";
		}
		Debug->AddMSG (L" Записываем данные");
		   } catch (...)
		{

		}



	CModbusRTU::WriteRegisters(txFrame, DeviceAddr, regAddr, txBytes);

		Debug->MSG.Type="WD";
		Debug->MSG.Direction = "WriteDataToDevice";
		Debug->MSG.Data=L"tx frame: ";
		for (uint8_t i=0;i<txFrame.Length;i++)
		{
		Debug->MSG.Data=Debug->MSG.Data+IntToHex(txFrame[i])+", ";
		}
		Debug->AddMSG (L" Записываем фрэйм");

	respData.codeFunction = txFrame[1];
   //	respData.registerAddr = MEASURE_TIME_ADDR; ???
    respData.registerAddr = 0x00;
	respData.length = 8;

     if (++RespID == 100) {RespID = 0;}
    respData.ID = RespID;
    msgParam.ID = RespID;

	respList.push_back(respData);

	if (false == respTimer->Enabled)
	{
	respTimer->Interval = RESPONSE_WAIT_TIMEOUT;
    //respTimer->Enabled = true;
	}

    Debug->MSG.Type="WD";
    Debug->MSG.Direction=L"respList OUTPUT";
    Debug->MSG.Data=L"ID= "+IntToStr(respData.ID)+L", RegAddr= "+respData.registerAddr;
	Debug->AddMSG (L"Регистрация ответа:");

	// TThread::Synchronize(NULL, Debug->OnTransfer);

	msgParam.msgLength = txFrame.Length;
    msgParam.callback = 0;

    error_status=0;

	btThread->Transmit(txFrame, msgParam);
 	txTicks = TThread::GetTickCount();
 }
}
// ---------------------------------------------------------------------------
// Функция отправки запроса на чтение значений регистров устройства
void TModBusDevice::ReadDataFromDevice(uint16_t regAddr,
	uint16_t count) {
	TBytes txFrame;
    if (IsBluetoothConnected())
    {

	ResponseData_t respData = {0, 0, 0};
	TxMsgParam_t msgParam = MSG_PARAM_STD_RESPONSE;

	txFrame.set_length(0);
	CModbusRTU::ReadRegisters(txFrame, DeviceAddr, regAddr, count);

    Debug->MSG.Type="RD";
        Debug->MSG.Direction = L"ReadDataFromDevice";
        Debug->MSG.Data=L"tx frame: ";
        for (uint8_t i=0;i<txFrame.Length;i++)
        {
        Debug->MSG.Data=Debug->MSG.Data+IntToHex(txFrame[i])+", ";
        }
        Debug->AddMSG (L" Записываем фрэйм");


	msgParam.msgLength = txFrame.Length;
	//
	// Подготовим и положим в контейнер данные, необходимые для анализа ответа
	// от устройства
	respData.codeFunction = txFrame[1];
	respData.registerAddr = regAddr;
	respData.length = count * 2;

    if (++RespID == 100) {RespID = 0;}
    respData.ID = RespID;

    msgParam.ID = RespID;

    Debug->MSG.Type="RD";
    Debug->MSG.Direction=L"respList INPUT";
    Debug->MSG.Data=L"ID= "+IntToStr(respData.ID)+L", RegAddr= "+respData.registerAddr;
    Debug->AddMSG (L"Регистрация ответа:");

	respList.push_back(respData);

	if (false == respTimer->Enabled)
	{
		respTimer->Interval = RESPONSE_WAIT_TIMEOUT;
	  //	respTimer->Enabled = true;
	}

     error_status=0;

	btThread->Transmit(txFrame, msgParam);
    }
}


//


#pragma package(smart_init)
