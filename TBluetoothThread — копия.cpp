//---------------------------------------------------------------------------
#pragma hdrstop


#ifdef _ANDROID_
#include <Androidapi.Helpers.hpp>
#endif

#include <System.Permissions.HPP>
//---------------------------------------------------------------------------
#include "TBluetoothThread.h"
//---------------------------------------------------------------------------
TBluetoothThread::TBluetoothThread()
{
  btState       = BT_NOINIT;
  btDeviceIndex = 0;

  OnConnect    = nullptr;
  OnDisconnect = nullptr;
}
//---------------------------------------------------------------------------
__fastcall TBluetoothThread::~TBluetoothThread()
{
  if(txFifo != nullptr)
  {
    delete txFifo;
    txFifo = nullptr;
  }

  if(Critical != nullptr)
  {
    delete Critical;
    Critical = nullptr;
  }
}
//---------------------------------------------------------------------------



//---------------------------------------------------------------------------
void __fastcall TBluetoothThread::OnConnected(void)
{
  if  (btClientSocket!=NULL)
  {
   if(OnConnect != nullptr) OnConnect(this, btClientSocket->Connected);
   } else
   {
    if(OnConnect != nullptr) OnConnect(this, false);
   }
}
//---------------------------------------------------------------------------
void __fastcall TBluetoothThread::OnDisconnected(void)
{
  if(OnDisconnect != nullptr) OnDisconnect(this);
}
//---------------------------------------------------------------------------
void __fastcall TBluetoothThread::OnReceive(void)
{
  if(OnDataReceive != nullptr) OnDataReceive(this, rxDataBytes);
}
//---------------------------------------------------------------------------
void TBluetoothThread::Initialize(void)
{
  txFifo = nullptr;

  if(Bluetooth != nullptr)
    delete Bluetooth;

  if(WDT != nullptr)
    delete WDT;

  Bluetooth         = nullptr;
  btDiscoverDevices = nullptr;
  btDevice          = nullptr;
  btClientSocket    = nullptr;
  WDT               = nullptr;

  OnDiscoverEnd = nullptr;
  OnDiscoverWdt = nullptr;

  Bluetooth = new TBluetooth(dynamic_cast<TComponent*>(this));
  Bluetooth->OnDiscoveryEnd = BluetoothDiscoverEnd;
  Bluetooth->Enabled        = true;

  //Bluetooth->LastServiceList()
   //LastServiceList

  WDT = new TTimer(dynamic_cast<TComponent*>(this));
  WDT->Enabled = false;
  WDT->OnTimer = OnWDT;

  txFifo = new TRingFifo(TX_FIFO_SIZE);

  Critical = new TCriticalSection;

  txMsgParam.msgLength    = 0;
  txMsgParam.respTimeout  = 0;

  btState = BT_INIT;
}
//---------------------------------------------------------------------------
void TBluetoothThread::DiscoverDevices(void)
{
  Bluetooth->DiscoverDevices(DISCOVER_DEVICES_TIMEOUT);
  WDT->Interval = DISCOVER_DEVICES_TIMEOUT +  500;
  WDT->Enabled  = true;
}
//---------------------------------------------------------------------------
uint8_t TBluetoothThread::Connect(uint32_t deviceIndex)
{
  Integer status;
  TBluetoothServiceList *btServices = nullptr;
  GUID socketId;

  do
  {
  // Сами ищем устройство
  if (deviceIndex == 0)
   {

   #ifdef __ANDROID__
   CheckBtPermission();
   #endif
   if(!btPermissionGranted)
  {
  ShowMessage(L"Нет доступа к Bluetooth!");
    break;
  }


    status = 0;
	try
    {
    PairedDevicesList =   Bluetooth-> PairedDevices();


    if(PairedDevicesList->Count==0)
      return 2;    // нет устройств для анализа

      UnicodeString str; // =  PairedDevicesList->Items[0]->DeviceName;
      UnicodeString const &conststr = "HSC";

      int pos;

    for(int32_t i = 0; i < PairedDevicesList->Count; i++)
     {
       str =  PairedDevicesList->Items[i]->DeviceName;
       pos = str.Pos(conststr);
        if (pos>0)
        {

            btDevice      = PairedDevicesList->Items[i];
    		btDeviceIndex = i;
    		btState       = BT_CONNECT;

			/*
			btServices = btDevice->GetServices();
			if(btServices == nullptr) break;
			if(btServices->Count <= 0)
      			break;

			socketId       = btServices->Items[0].UUID;
			btClientSocket = btDevice->CreateClientSocket(socketId, false);
			if(btClientSocket != nullptr)
    			{
     		 if(btClientSocket->Connected)
      		  btClientSocket->Close();
    			}       */
              return 1; // Всё нашли
        } else
        {
              //
        }

      }
            return 3; // нет подходящих Устройств

         }
    catch (...)
    {
     ShowMessage("BlueTooth выключен");
     return 0; // Выключен блютуф

    }



   }

  else if  (deviceIndex <100)
  {
    if(deviceIndex >= btDiscoverDevices->Count)
      break;

    if(btClientSocket != nullptr)
    {
      if(btClientSocket->Connected)
        btClientSocket->Close();
    }

    btDevice      = btDiscoverDevices->Items[deviceIndex];
    btDeviceIndex = deviceIndex;
    btState       = BT_CONNECT;

  }else
  {
  deviceIndex = deviceIndex -100;
    if(deviceIndex >= PairedDevicesList->Count)
      break;

    if(btClientSocket != nullptr)
    {
      if(btClientSocket->Connected)
        btClientSocket->Close();
    }

    btDevice      = PairedDevicesList->Items[deviceIndex];
    btDeviceIndex = deviceIndex;
    btState       = BT_CONNECT;

  }





  } while(0);
}
//---------------------------------------------------------------------------
void TBluetoothThread::Transmit(TBytes &txBytes, const TxMsgParam_t &msgParam)
{
  Critical->Enter();
  //
  // Тут просто кладем в передающее FIFO параметры передаваемого сообщения...
  txFifo->Push(const_cast<TxMsgParam_t*>(&msgParam), sizeof(TxMsgParam_t));
  //
  // И само передаваемое сообщение. Далее в потоке мы эти данные вытащим из FIFO
  // и перенаправим уже в Bluetooth сокет для непосредственно передачи в канал.
  txFifo->Push(&txBytes[0], txBytes.Length);

  Critical->Leave();
}
//---------------------------------------------------------------------------
void TBluetoothThread::Disconnect(void)
{
  if(btClientSocket != nullptr)
  {
    if(btClientSocket->Connected)
      btState = BT_DISCONNECT;
  }
}
//---------------------------------------------------------------------------
bool TBluetoothThread::IsConnected(void) const
{
  bool retCode = false;

  if(btClientSocket != nullptr)
    retCode = btClientSocket->Connected;

  return retCode;
}
//---------------------------------------------------------------------------
// Функция, выполняемая в потоке
void __fastcall TBluetoothThread::Execute()
{
  for(;;)
  {
    switch(btState)
    {
      case BT_NOINIT:
      case BT_INIT:
        break;

      case BT_CONNECT:
        Connecting();
        break;

      case BT_READY:
        DataExchange();
          break;

      case BT_RECIVING:
        DataExchange();
          break;

      case BT_DISCONNECT:
        Disconnecting();
        break;

      case BT_FAILURE:
      {
        DataExchange();


      }
      default:
        break;
    }
  }
}



    void TBluetoothThread::ConnectPairedHSC()
{
	uint32_t deviceIndex;
    PairedDevicesList =   Bluetooth-> PairedDevices();
    uint8_t Tag=0;

       for(int32_t i = 0; i < PairedDevicesList->Count; i++)
     {
      if(PairedDevicesList->Items[i]->DeviceName.Length())
      {
        if (PairedDevicesList->Items[i]->DeviceName == "HSC-COMPACT2")
        {
        //
        // Сохраним в свойстве Tag номер BT устройства в списке доступных
        // BT устройств.
        deviceIndex = i;
        }
      }
     }

    //  if(deviceIndex >= PairedDevicesList->Count)
    //  break;

    if(btClientSocket != nullptr)
    {
      if(btClientSocket->Connected)
        btClientSocket->Close();
    }

    btDevice      = PairedDevicesList->Items[deviceIndex];
    btDeviceIndex = deviceIndex;
    btState       = BT_CONNECT;
}


    TBluetoothDeviceList*  TBluetoothThread::PairedDevices(void)
    {
       PairedDevicesList =   Bluetooth-> PairedDevices();

        return PairedDevicesList ;


    }
//---------------------------------------------------------------------------
// Функция завершения сканирования устройств
void __fastcall TBluetoothThread::BluetoothDiscoverEnd(System::TObject* const Sender,
                                                  TBluetoothDeviceList* const ADeviceList)
{


  PairedDevicesList =   Bluetooth-> PairedDevices();
  btDiscoverDevices = ADeviceList;
  WDT->Enabled = false;
  //
  // Передадим данные для отображения на форме
  if(OnDiscoverEnd != nullptr)
   OnDiscoverEndPaired(Sender, ADeviceList,PairedDevicesList);
  //  OnDiscoverEnd(Sender, ADeviceList);
}
//---------------------------------------------------------------------------
void __fastcall TBluetoothThread::OnWDT(TObject *Sender)
{
  WDT->Enabled = false;
  Bluetooth->CancelDiscovery();

  PairedDevicesList =   Bluetooth-> PairedDevices();


    if(OnDiscoverEnd != nullptr)
       OnDiscoverEndPaired(Sender, nullptr,PairedDevicesList);

}
//---------------------------------------------------------------------------

void __fastcall TBluetoothThread::Connecting(void)
{
  TBluetoothServiceList *btServices = nullptr;
  GUID socketId;

  do
  {
    if(btClientSocket != NULL)
	{
      if(btClientSocket->Connected)
      {
        btClientSocket->Close();
      }
    }

    if (nullptr == btDevice)
    {
      break;

        Debug->MSG.Direction="Connect";
        Debug->MSG.Type="BT";
        Debug->MSG.Data= L"nullptr == btDevice";
        Debug->AddMSG ("Error Connecting");

        btState = BT_INIT;

    }
    /*
    if (btDeviceIndex<100)
    {
    btDevice = btDiscoverDevices->Items[btDeviceIndex];
     } else
     {
    btDevice = PairedDevicesList->Items[btDeviceIndex-100];
   }
     */
    // Если нет сопряжения, то надо сопрягать.
    if (!btDevice->IsPaired) {
      if (!Bluetooth->CurrentAdapter->Pair(btDevice))
      {
        Debug->MSG.Direction="Connect";
        Debug->MSG.Type="BT";
        Debug->MSG.Data= L"nullptr == btDevice";
        Debug->AddMSG ("Error Connectinh");

        btState = BT_INIT;

        break;
      }
    }



    btServices = btDevice->GetServices();



    if(btServices == nullptr)
    {
        Debug->MSG.Direction="Connect";
        Debug->MSG.Type="BT";
        Debug->MSG.Data= L"btServices == nullptr";
        Debug->AddMSG ("Error Connecting");

        btState = BT_INIT;

    break;
    }

    if(btServices->Count <= 0)
     {
        Debug->MSG.Direction="Connect";
        Debug->MSG.Type="BT";
        Debug->MSG.Data= L"btServices->Count <= 0";
        Debug->AddMSG ("Error Connecting");

        btState = BT_INIT;

      break;

     }

    socketId       = btServices->Items[0].UUID;
	btClientSocket = btDevice->CreateClientSocket(socketId, false);

    if (nullptr == btClientSocket)
    {
        Debug->MSG.Direction="Connect";
        Debug->MSG.Type="BT";
        Debug->MSG.Data= L"nullptr == btClientSocket";
        Debug->AddMSG ("Error Connecting");

      break;

    }

	btClientSocket->Connect();


    btState = (btClientSocket->Connected) ? BT_READY : BT_INIT;

  } while (0);


  TThread::Synchronize(NULL, OnConnected);
}
//---------------------------------------------------------------------------
// Функция, выполняющаяя обмен данными с ведомым устройством по Bluetooth
void TBluetoothThread::DataExchange(void)
{
  uint32_t bytesPulled  = 0;
  const  uint32_t ticks = GetTickCount();
  static uint32_t rxFrameTicks = 0;
  static uint32_t txFrameTicks = 0;

  do
  {
    //
    // Проверим состояние сокета
    if(btClientSocket->Connected == false)
	{
	  btState = BT_INIT;
	  break;
	}
	//
	// В качестве параметра передаем таймаут ожидания в мс
	try
	{
	rxDataBytes = btClientSocket->ReceiveData(100);
	} catch (...)
	{
	//Разрыв соединения
	}
    //
    // Если что-то приняли...
    if(rxDataBytes.Length > 0)
    {
        Debug->MSG.Direction="Input";
        Debug->MSG.Type="RRD";

        for (int i=0;i<rxDataBytes.Length;i++)
        {
        Debug->MSG.Data=Debug->MSG.Data+" "+IntToHex(rxDataBytes[i]);
        }

       Debug->AddMSG ("ReciveData");

       rxFrameTicks = GetTickCount();
      memset(&txMsgParam, 0, sizeof(TxMsgParam_t));
      OnReceive();
      rxDataBytes.set_length(0);
    }

    //Если передаваемое сообщение отработано: либо не получен ответ в ожидаемое время, либо всё корректно
     if(IsReciving == true) // В работу берем сообщение. Ждем ответ.
     { break; }
    //
    // Прошло достаточно ли времени от момента приема сообщения, до отправки следующего...
    if((ticks - rxFrameTicks) < MSG_SEPARATE_DELAY ||
       (ticks - txFrameTicks) < MSG_SEPARATE_DELAY)
     { break; }
    //
    // Если в данный момент не ждем ответа от ведомого, смотрим есть ли что-то
    // на  передачу в FIFO...
    Critical->Enter();
    //
    // Если есть что-то для передачи...
    if(txFifo->IsEmpty() == false)
    {
      // Сначала из FIFO извлечем параметры отправляемого сообщения
      txFifo->Pull(&txMsgParam, sizeof(TxMsgParam_t), bytesPulled);

      txMsgParam.status = 1; // В работу берем сообщение. Ждем ответ.
      IsReciving = true;


      txDataBytes.set_length(txMsgParam.msgLength);

        Debug->MSG.Direction="Output";
        Debug->MSG.Type="WD";

        Debug->MSG.Data= L"ID: "+ IntToStr(txMsgParam.ID) +  L", msgLength:"+IntToStr(txMsgParam.msgLength);

       Debug->AddMSG ("txMsgParam");

      //
      // Затем, зная параметры, извлечем само отправляемое сообщение
      txFifo->Pull(&txDataBytes[0], txMsgParam.msgLength, bytesPulled);

        if ((txMsgParam.callback>0)&&(OnDataSent!=NULL))
        {
           OnDataSent(txMsgParam.callback);
        }

        SendData();

      txFrameTicks = GetTickCount();
      //txDataBytes.set_length(0);
    }

    Critical->Leave();
  } while(0);
}

void TBluetoothThread::SendData(void)
{

if(btClientSocket->Connected == true)
    {



  btClientSocket->SendData(txDataBytes);

  Debug->MSG.Direction="Output";
  Debug->MSG.Type="RWD";

  for (int i=0;i<txDataBytes.Length;i++)
  {
  Debug->MSG.Data=Debug->MSG.Data+" "+IntToHex(txDataBytes[i]);
  }

  Debug->AddMSG ("SendData");
  }
}




//---------------------------------------------------------------------------
void TBluetoothThread::Disconnecting(void)
{
  if(btClientSocket != NULL)
  {
    if(btClientSocket->Connected)
    {
      btClientSocket->Close();
      TThread::Synchronize(NULL, OnDisconnected);
    }
  btState = BT_INIT;
  }
}

void TBluetoothThread::CheckBtPermission(void)
{
#ifdef __ANDROID__

  DynamicArray<String> permissions;
  permissions.Length = 1;
  permissions[0] = JStringToString(TJManifest_permission::JavaClass->ACCESS_FINE_LOCATION);

  PermissionsService()->RequestPermissions(permissions,
		[this](const DynamicArray<String> APermissions,
			   const DynamicArray<TPermissionStatus> AGrantResults)
	{
	  if ((AGrantResults.Length == 1) and(AGrantResults[0] == TPermissionStatus::Granted))
	  {
		btPermissionGranted = true;
	  }
	  else
	  {
		btPermissionGranted = false;
	  }
	} );

#endif

 #ifndef __ANDROID__
btPermissionGranted = true;
 #endif
    }



//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
