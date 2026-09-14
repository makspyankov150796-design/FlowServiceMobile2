// ---------------------------------------------------------------------------
#pragma hdrstop
// ---------------------------------------------------------------------------
#include "THscDevice.h"

TTimer *THscDevice::mainTimer ;


// ---------------------------------------------------------------------------
THscDevice::THscDevice() {

    DeviceAddr = HSC_DEVICE_ADDRESS;


	mainTimer = new TTimer(NULL);
	mainTimer->Enabled = false;
	mainTimer->Interval = MAIN_TIMER_INTERVAL;
	mainTimer->OnTimer = OnMainTimer;

	OnChangeState = nullptr;
	OnDataReceive = nullptr;


	Conditions.tempAir = COND_TEMP_AIR_DEFAULT;
	Conditions.tempWater = COND_TEMP_WATER_DEFAULT;
	Conditions.pressureAir = COND_PRESS_AIR_DEFAULT;
	Conditions.pressureWater = COND_PRESS_WATER_DEFAULT;
	Conditions.humidityAir = COND_HUM_AIR_DEFAULT;
	Conditions.densityWater = COND_DENS_WATER_DEFAULT;

	ChangeState(HSC_STATE_STANDBY);

	memset(&DeviceInfo, 0, sizeof(DeviceInfo_t));
	memset(&verifyData, 0, sizeof(TotalData_t));

	memset(&Settings, 0, sizeof(tSettings));
	memset(&CumData, 0, sizeof(tCumulativeData));
	memset(&SecData, 0, sizeof(tSecData));

	CumData.ImpTotal[0] = 0;

	PhotoBefore = nullptr;
	PhotoAfter = nullptr;

    PingEnable = true;

    Temp = 0;
}

// ---------------------------------------------------------------------------
__fastcall THscDevice::~THscDevice() {

	delete mainTimer;
	//
	// Надо удалить объекты фотографий...
	if (PhotoBefore != nullptr) {
		delete PhotoBefore;
		PhotoBefore = nullptr;
	}

	if (PhotoAfter != nullptr) {
		delete PhotoAfter;
		PhotoAfter = nullptr;
	}
}

void THscDevice::AfterConnect(void)
{
   // Read_ConfigData();
	//ChangeState(HSC_STATE_PING);
   ChangeState(HSC_STATE_CONFIG);

	mainTimer->Enabled = true;
}


void THscDevice::Disconnect(void) {

	ChangeState(HSC_STATE_STANDBY);
	TModBusDevice::Disconnect();
}


// ---------------------------------------------------------------------------
void THscDevice::StartMonitor(void) {
	TBytes txData;

	memset(&verifyData, 0, sizeof(TotalData_t));
	verifyTime = 0;
	ResetDeviceData(); // < Сбросим данные предыдущих измерений
	//
	// Установка времени проливки...
	txData.set_length(2);
	txData[0] = static_cast<uint8_t>((0 & 0xFF00) >> 8);
	txData[1] = static_cast<uint8_t>(0 & 0x00FF);
	WriteDataToDevice(MEASURE_TIME_ADDR, txData);
	//
	// Отключаем ограничение по импульсам
	txData.set_length(4);
	for (uint32_t i = 0; i < 4; i++) {
		txData[i] = 0;
	}
	WriteDataToDevice(MEASURE_PULSES_LIMIT_ADDR, txData);


	txData.set_length(4);

	if (CalibrState) {

	} else
	{
	//
	// 0x0001 - Запуск по команде, 0x0001 - остановка по времени
	txData[1] = 0x0001;
	txData[3] = 0x0001;
	WriteDataToDevice(START_CONFIG_ADDR, txData);

	}
	//
	// Запуск испытания
	txData.set_length(2);
	txData[0] = 0x00;
	txData[1] = 0x01;
	WriteDataToDevice(MEASURE_STATE_ADDR, txData);


	measureTimeTicks = 0;
	ChangeState(HSC_STATE_MONITOR);
}

// ---------------------------------------------------------------------------
void THscDevice::StopMonitor(void) {
	TBytes txData;

    	// Остановка испытания
	txData.set_length(2);
	txData[0] = 0x00;
	txData[1] = 0x03;
	WriteDataToDevice(MEASURE_STATE_ADDR, txData);


	measureTimeTicks = 0;
	ChangeState(HSC_STATE_PING);
}


// ---------------------------------------------------------------------------
void THscDevice::StartMeasure(uint16_t time) {
	TBytes txData;

	memset(&verifyData, 0, sizeof(TotalData_t));
	verifyTime = time;
	ResetDeviceData(); // < Сбросим данные предыдущих измерений
	//
	// Установка времени проливки...
	txData.set_length(2);
	txData[0] = static_cast<uint8_t>((time & 0xFF00) >> 8);
	txData[1] = static_cast<uint8_t>(time & 0x00FF);
	WriteDataToDevice(MEASURE_TIME_ADDR, txData);
	//
	// Отключаем ограничение по импульсам
	txData.set_length(4);
	for (uint32_t i = 0; i < 4; i++) {
		txData[i] = 0;
	}
	WriteDataToDevice(MEASURE_PULSES_LIMIT_ADDR, txData);


	txData.set_length(4);

	if (CalibrState) {

	} else
	{
	//
	// 0x0001 - Запуск по команде, 0x0001 - остановка по времени
	//txData[1] = StartType;
	//txData[3] = StopType;
	//WriteDataToDevice(START_CONFIG_ADDR, txData);

	}
	//
	// Запуск испытания
	txData.set_length(2);
	txData[0] = 0x00;
	txData[1] = 0x01;
	WriteDataToDeviceCallBack(MEASURE_STATE_ADDR, txData,1);

	measureTimeTicks = 0;
	ChangeState(HSC_STATE_STARTTEST);
}

void THscDevice::StartMeasureInstant(uint16_t time) {
	TBytes txData;

	memset(&verifyData, 0, sizeof(TotalData_t));
	verifyTime = time;
	ResetDeviceData(); // < Сбросим данные предыдущих измерений
	//
	// Установка времени проливки...
	txData.set_length(2);
	txData[0] = static_cast<uint8_t>((time & 0xFF00) >> 8);
	txData[1] = static_cast<uint8_t>(time & 0x00FF);
	WriteDataToDevice(MEASURE_TIME_ADDR, txData);
	//
	// Отключаем ограничение по импульсам
	txData.set_length(4);
	for (uint32_t i = 0; i < 4; i++) {
		txData[i] = 0;
	}
	WriteDataToDevice(MEASURE_PULSES_LIMIT_ADDR, txData);
	//
	// 0x0001 - Запуск по команде, 0x0001 - остановка по времени
	txData[1] = 0x01;
	txData[3] = 0x01;
	WriteDataToDevice(START_CONFIG_ADDR, txData);
	//
	// Запуск испытания
	txData.set_length(2);
	txData[0] = 0x00;
	txData[1] = 0x02;
	WriteDataToDeviceCallBack(MEASURE_STATE_ADDR, txData,1);

	measureTimeTicks = 0;
	ChangeState(HSC_STATE_STARTTEST);
}



void THscDevice::StartTest() {
	TBytes txData;

	//
	// Запуск испытания
	txData.set_length(2);
	txData[0] = 0x00;
	txData[1] = 0x01;
	WriteDataToDeviceCallBack(MEASURE_STATE_ADDR, txData,1);

	measureTimeTicks = 0;
	ChangeState(HSC_STATE_EXECUTE);
}

void THscDevice::StopMeasure() {
	TBytes txData;

	// Остановка испытания
	txData.set_length(2);
	txData[0] = 0x00;
	txData[1] = 0x03;
	WriteDataToDeviceCallBack( MEASURE_STATE_ADDR, txData,2);

	ChangeState(HSC_STATE_STOPTEST);
}

// ---------------------------------------------------------------------------
// Передача фотографии начала проливки
void THscDevice::SetPhotoBeforeVerification(TBitmap *Photo) {
	if (nullptr == PhotoBefore)
		PhotoBefore = new TBitmap();
	PhotoBefore->Assign(Photo);
}

// ---------------------------------------------------------------------------
// Передача фотографии после завершения проливки
void THscDevice::SetPhotoAfterVerification(TBitmap *Photo) {
	if (nullptr == PhotoAfter)
		PhotoAfter = new TBitmap();
	PhotoAfter->Assign(Photo);
}

// ---------------------------------------------------------------------------
// Функция сохранения результатов поверки
void THscDevice::SaveVerificationResults(void) {
	bool success = true;
	UnicodeString dirName = L"";
	TXMLDocument *XmlDoc = nullptr;
	_di_IXMLNode rootNode = nullptr;
	_di_IXMLNode sampleNode = nullptr;
	float rate = 0.0f;

	dirName = L"/storage/emulated/0/HSC Mobile/";

	do {
		if (!DirectoryExists(dirName)) {
			CreateDir(dirName);
		}

		dirName = dirName + DateToStr(Date()) + "_" + TimeToStr(Time());
		//
		// Если папка с таким именем существует, то будем дописывать "(номер)" к имени папки
		if (DirectoryExists(dirName)) {
			success = false;

			for (uint32_t i = 0; i < 1000; i++) {
				dirName += "(" + UIntToStr(i) + ")";
				if (!DirectoryExists(dirName)) {
					success = true;
					break;
				}
			}
		}

		if (false == success) {
			ShowMessage(L"Ошибка сохранения данных");
			break;
		}

		CreateDir(dirName);

		if (PhotoBefore != nullptr)
			PhotoBefore->SaveToFile(dirName + L"/PhotoBefore.jpg");
		if (PhotoAfter != nullptr)
			PhotoAfter->SaveToFile(dirName + L"/PhotoAfter.jpg");

		XmlDoc = new TXMLDocument(NULL);

		XmlDoc->DOMVendor = DOMVendors->Vendors[0];
		// < OMNI XML кроссплатформенный вендор
		XmlDoc->XML->Clear();
		XmlDoc->FileName = "";
		XmlDoc->Active = true;
		//
		// Создадим главную ветку и добавим узел об устройстве
		rootNode = XmlDoc->AddChild(L"report");
		sampleNode = rootNode->AddChild(L"Device");

		// sampleNode->SetAttribute(L"Serial", SerialNum);
		// sampleNode->SetAttribute(L"DeviceType", DeviceType);
		// sampleNode->SetAttribute(L"CertificateNum", CertificateNum);
		//
		// Теперь добавим информацию об условиях поверки
		sampleNode = rootNode->AddChild(L"Conditions");
		sampleNode->SetAttribute(L"TempAir", Conditions.tempAir);
		sampleNode->SetAttribute(L"PressureAir", Conditions.pressureAir);
		sampleNode->SetAttribute(L"HumidityAir", Conditions.humidityAir);
		sampleNode->SetAttribute(L"TempWater", Conditions.tempWater);
		sampleNode->SetAttribute(L"PressureWater", Conditions.pressureWater);
		sampleNode->SetAttribute(L"DensityWater", Conditions.densityWater);
		//
		// Теперь сохраним результаты выполнения поверки
		sampleNode = rootNode->AddChild(L"Results");
		sampleNode->SetAttribute(L"Time", verifyTime);

		rate = static_cast<float>(verifyData.totalPulsesCnt[0])
			/ verifyData.MeasureData.time;
		sampleNode->SetAttribute(L"Rate", rate);

		sampleNode->SetAttribute(L"Volume", verifyData.totalPulsesCnt[0]);

		XmlDoc->SaveToFile(dirName + L"/Report.xml");
		XmlDoc->Active = false;
		delete XmlDoc;

	}
	while (0);

	delete PhotoBefore;
	PhotoBefore = nullptr;

	delete PhotoAfter;
	PhotoAfter = nullptr;

	ChangeState(HSC_STATE_PING);
}

// Функция обработки принятого пакета
void THscDevice::PacketHandler(void) {

	uint8_t i;
	uint8_t index = 3;
	uint16_t temp;
    uint8_t debug_index = 3;
	respTimer->Enabled = false;
	respTimer->Interval = RESPONSE_WAIT_TIMEOUT;
	//btThread->IsReciving = false;


               Debug->AddMSG (L"// < Если просили значения регистров");
				//
				// Если это пинг (данные о версии устройства и протокола)
				if (respList.begin()->registerAddr == DEVICE_REVISION_ADDR) {

                Debug->MSG.Type=L"ENT";
            	Debug->MSG.Direction=L"PacketHandler";
            	Debug->MSG.Data=IntToStr(DEVICE_REVISION_ADDR);
            	Debug->AddMSG (L"это пинг (данные о версии устройства и протокола)");
				  //	if (DeviceInfo.ProtocolVersion == 0 ||
				  //		DeviceInfo.DeviceRevision == 0) {
						/*
						 DeviceInfo.DeviceRevision   = ((static_cast<uint16_t>(rxFrame[3]) << 8) & 0xFF00);
						 DeviceInfo.DeviceRevision  |=  (static_cast<uint16_t>(rxFrame[4]) & 0x00FF);
						 DeviceInfo.ProtocolVersion  = ((static_cast<uint16_t>(rxFrame[5]) << 8) & 0xFF00);
						 DeviceInfo.ProtocolVersion |=  (static_cast<uint16_t>(rxFrame[6]) & 0x00FF);
						 DeviceInfo.deviceAddr       = ((static_cast<uint16_t>(rxFrame[7]) << 8) & 0xFF00);
						 DeviceInfo.deviceAddr      |=  (static_cast<uint16_t>(rxFrame[8]) & 0x00FF);
						 */

						DeviceInfo.DeviceRevision =
							RegHandler_uint16(rxFrame, &index);
						DeviceInfo.ProtocolVersion =
							RegHandler_uint16(rxFrame, &index);
						DeviceInfo.deviceAddr =
							RegHandler_uint16(rxFrame, &index);

						Version.Version = DeviceInfo.DeviceRevision;
						Version.Protocol = DeviceInfo.ProtocolVersion;



                Debug->MSG.Type=L"DAT";
            	Debug->MSG.Direction=L"PacketHandler";
            	Debug->MSG.Data=IntToStr(DeviceInfo.DeviceRevision)+" , "+ IntToStr(DeviceInfo.ProtocolVersion);
            	Debug->AddMSG (L"DeviceRevision");

                ChangeState(HSC_STATE_PING);

						if (OnVersionRead != nullptr) {
							OnVersionRead(this);
						}
				}
				// Иначе, если это настройки
				if (respList.begin()->registerAddr == DEVICE_SETTINGS_ADDR) {


                Debug->MSG.Type=L"ENT";
            	Debug->MSG.Direction=L"PacketHandler";
            	Debug->MSG.Data=IntToStr(DEVICE_SETTINGS_ADDR);

                Debug->MSG.Data= Debug->MSG.Data+ L" Симуляция: "+
                L"index:"+IntToStr(index)+L" , значение:"+ IntToStr(rxFrame[index])+ IntToStr(rxFrame[index+1])   ;


					Settings.Simulator = RegHandler_bool(rxFrame, &index);

					// настройки старта измерения
                 Debug->MSG.Data= Debug->MSG.Data+ L" настройки старта измерения: "+
                 L"index:"+IntToStr(index)+L" , значение:"+ IntToStr(rxFrame[index])+ IntToStr(rxFrame[index+1])   ;

					data = RegHandler_uint16(rxFrame, &index);

					Settings.StartSettings =
				 			static_cast<eStartSettings>(data);

					// Канал синхронизации не равено 0
				   //	if ((data & 0x0FF0) > 0) {
						// Номер канала старта внешней синхронизации
					//	Settings.StartCHNum = data & 0x0FF0;
					//	Settings.StartSettings =
				   //			static_cast<eStartSettings>((data & 0xF000) + 4);
				   //	}
				  //	else
						// Канал синхронизации равен 0
				  //	{
				  //		Settings.StartCHNum = 0;
				  //		Settings.StartSettings =
				  //			static_cast<eStartSettings>(data);
				  //	}

					// Стоп

                 Debug->MSG.Data= Debug->MSG.Data+ L" настройки стоп измерения: "+
                 L"index:"+IntToStr(index)+L" , значение:"+ IntToStr(rxFrame[index])+ IntToStr(rxFrame[index+1])   ;

					data = RegHandler_uint16(rxFrame, &index);

                    	Settings.StopSettings =
							static_cast<eStopSettings>(data);

					// Канал синхронизации не равено 0
				   ///	if ((data & 0x0FF0) > 0) {
						// Номер канала старта внешней синхронизации
				   //		Settings.StopCHNum = data & 0x0FF0;
				   //		Settings.StopSettings =
				  //			static_cast<eStopSettings>(data & 0xF000 + 4);
				  //	}
				  //	else
						// Канал синхронизации равен 0
				  //	{
				 //		Settings.StopCHNum = 0;
				  //		Settings.StopSettings =
				 //			static_cast<eStopSettings>(data); ;
				 //	}


                 Debug->MSG.Data= Debug->MSG.Data+ L" Время: "+
                 L"index:"+IntToStr(index)+L" , значение:"+ IntToStr(rxFrame[index])+ IntToStr(rxFrame[index+1])   ;

					Settings.TimeLimit = RegHandler_uint16(rxFrame, &index);



                 Debug->MSG.Data= Debug->MSG.Data+ L" Импульсы: "+
                 L"index:"+IntToStr(index)+L" , значение:"+ IntToStr(rxFrame[index])+ IntToStr(rxFrame[index+1])   ;

					//Settings.ImpLimit = RegHandler_uint16(rxFrame, &index);
					Settings.ImpLimit = RegHandler_uint32(rxFrame, &index);
					Settings.IntSyn = RegHandler_uint16(rxFrame, &index);






					// index = 16
					data = RegHandler_uint16(rxFrame, &index);
					temp = data;
					if (data == 0)  {
						Settings.OutSynCH = 0;
						Settings.ExtSyn =
							static_cast<eExtSyn>(0);
					}


					else if ((data & 0xF00) > 0) {
						// Номер канала старта внешней синхронизации
						Settings.OutSynCH = data & 0x0FF;
						Settings.ExtSyn =
						   //???	static_cast<eExtSyn>(data & 0xF000 + 2);
						   static_cast<eExtSyn>(( (data & 0xF00) >> 8  ));
					}
					else
						// Канал синхронизации равен 0
					{
						Settings.OutSynCH = 0;
						Settings.ExtSyn = static_cast<eExtSyn>(data);
					}
					// index = 18
					Settings.EtalonCH = RegHandler_uint16(rxFrame, &index);

					index = 24;

					Settings.Errors = RegHandler_uint16(rxFrame, &index);

					Settings.Reset = RegHandler_uint16(rxFrame, &index);

					Debug->AddMSG (L"Настройки");

					if (OnSettingsRead != nullptr) {
						OnSettingsRead(this);

					Debug->MSG.Type="ENT";
					Debug->MSG.Direction=L"Settings";
					Debug->MSG.Data="";
					Debug->AddMSG (L"Запуск обработчика");
					}

				}

				if (respList.begin()->registerAddr == DEVICE_CHECKSTATE_ADDR) {

									Debug->MSG.Type="ENT";
				Debug->MSG.Direction="PacketHandler";
				//Debug->MSG.Data=IntToStr(frameLen);
				Debug->AddMSG ("registerAddr == DEVICE_CHECKSTATE_ADDR");


					CheckState.Reset = RegHandler_bool(rxFrame, &index);

					// Последнее измерения номер
					CheckState.Num = RegHandler_uint16(rxFrame, &index);

					// Статус измерения
					data = RegHandler_uint16(rxFrame, &index);

					// Канал =0
					if ((data & 0x0FF0) > 0) {
						// Номер канала

					}
					else
						// Канал равен 0
					{
						CheckState.State = data;
					}

					// Стоп
					CheckState.Time = RegHandler_uint16(rxFrame, &index); ;

					// 9
					CheckState.Over = RegHandler_bool(rxFrame, &index);

					if (OnCheckStateRead != nullptr) {
						OnCheckStateRead(this);
					}

				}

				if (respList.begin()->registerAddr == DEVICE_CHARGE_ADDR) {

				Debug->MSG.Type="ENT";
				Debug->MSG.Direction="PacketHandler";
				//Debug->MSG.Data=IntToStr(frameLen);
				Debug->AddMSG ("registerAddr == DEVICE_CHARGE_ADDR");


					// Последнее измерения номер
					Charge = RegHandler_uint16(rxFrame, &index);


						if ( OnChargeRead!= nullptr) {
						OnChargeRead(this);
					}

				}


				// Иначе, если приняли текущие данные выполнения проливки
				else if (respList.begin()->registerAddr == MEASURE_DATA_ADDR) {

				Debug->MSG.Type="ENT";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data="  ";



					if (24 == rxFrame[2]) { /*
						 verifyData.MeasureData.time   = ((static_cast<uint16_t>(rxFrame[3]) << 8) & 0xFF00);
						 verifyData.MeasureData.time  |=  (static_cast<uint16_t>(rxFrame[4]) & 0x00FF);
						 verifyData.MeasureData.state  = ((static_cast<uint16_t>(rxFrame[5]) << 8) & 0xFF00);
						 verifyData.MeasureData.state |=  (static_cast<uint16_t>(rxFrame[6]) & 0x00FF);
						 */
						verifyData.MeasureData.time =
							RegHandler_uint16(rxFrame, &index);

					   Debug->MSG.Data= Debug->MSG.Data + L"Время:  "+IntToStr(verifyData.MeasureData.time)+ L", ";

						verifyData.MeasureData.state =
							RegHandler_uint16(rxFrame, &index);

						Debug->MSG.Data= Debug->MSG.Data + L"Состояние:  "+IntToStr(verifyData.MeasureData.state)+ L", ";


						SecData.Time = verifyData.MeasureData.time;
						SecData.State = verifyData.MeasureData.state;

						if (MEASURE_STATE_EXECUTE ==
							verifyData.MeasureData.state) {
							// Теперь счетчики импульсов по каналам...
							for (i = 0; i < PULSE_CHANNEL_COUNT; i++) {

								verifyData.MeasureData.pulsesCount[i] =
									   RegHandler_uint16(rxFrame, &index);

							   Debug->MSG.Data= Debug->MSG.Data + L"Индекс: "+IntToStr(index) + L", Импульсы: "+IntToStr(verifyData.MeasureData.pulsesCount[i]);

								SecData.ImpPerSec[i] =
									verifyData.MeasureData.pulsesCount[i];
								// архив импульсов по секундам
								Impulses[i][rd_imp]=
								verifyData.MeasureData.pulsesCount[i];
							}

							if (rd_imp++ > IMP_BUFFER_LEN) {
								rd_imp = 0;
							}
							if (cnt_imp++ > IMP_BUFFER_LEN) {
								cnt_imp = IMP_BUFFER_LEN;
							}

							ChangeState(HSC_STATE_EXECUTE);
						}
						else if (MEASURE_STATE_COMPLETE ==
							verifyData.MeasureData.state)
						{
							ChangeState(HSC_STATE_COMPLETE);
						}

					Debug->AddMSG (L"приняли текущие данные выполнения проливки");

						if (OnSecDataRead != nullptr) {
							OnSecDataRead(this);
						}
					}
				}


				//

				else if (respList.begin()->registerAddr == IMP_DATA_ADDR) {

				Debug->MSG.Type="ENT";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data="  ";

					 //  index = index+4;

							for (i = 0; i < PULSE_CHANNEL_COUNT; i++) {

								verifyData.MeasureData.pulsesCount[i] =
									   RegHandler_uint16(rxFrame, &index);

							   Debug->MSG.Data= Debug->MSG.Data + L"Индекс: "+IntToStr(index) + L", Импульсы: "+IntToStr(verifyData.MeasureData.pulsesCount[i]);

								SecData.ImpPerSec[i] =
									verifyData.MeasureData.pulsesCount[i];
								// архив импульсов по секундам
								Impulses[i][rd_imp]=
								verifyData.MeasureData.pulsesCount[i];
							}

							if (rd_imp++ > IMP_BUFFER_LEN) {
								rd_imp = 0;
							}
							if (cnt_imp++ > IMP_BUFFER_LEN) {
								cnt_imp = IMP_BUFFER_LEN;
							}





						  //	ChangeState(HSC_STATE_COMPLETE);


					Debug->AddMSG (L"приняли текущие данные выполнения проливки");

						if (OnImpDataRead != nullptr) {
							OnImpDataRead(this);
						}

				}

					else if (respList.begin()->registerAddr == FINAL_DATA_ADDR) {
					index = 3;

				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";


					for (uint8_t i = 0; i < PULSE_CHANNEL_COUNT; i++) {
						FinalData.ImpFinal[i] =
							RegHandler_float(rxFrame, &index);

					Debug->MSG.Data= Debug->MSG.Data + "  "+IntToStr(index)+":"+FloatToStrF(FinalData.ImpFinal[i],ffNumber,10,2)+ ", ";

					}
				   Debug->AddMSG (L" Итоговые данные по импульсам");

					if (OnFinalDataRead != nullptr)
						OnFinalDataRead(this);

					ChangeState(HSC_STATE_FINALREAD);

				}

					else if (respList.begin()->registerAddr == MEASURE_WIDTH_ADDR) {
					index = 3;

				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";


					for (uint8_t i = 0; i < PULSE_CHANNEL_COUNT; i++) {
						SecData.Width[i] =
							RegHandler_float(rxFrame, &index);

						Widths[i][rd_width]=  SecData.Width[i];

						   Debug->MSG.Data= Debug->MSG.Data + L"Индекс: "+IntToStr(index) + L", Длительность: "+FloatToStr(SecData.Width[i]);

								// архив импульсов по секундам
					}

					if (rd_width++ > IMP_BUFFER_LEN) {
					rd_width = 0;
					}
					if (cnt_width++ > IMP_BUFFER_LEN) {
							cnt_width = IMP_BUFFER_LEN;
					}


				   Debug->AddMSG (L" Итоговые данные по импульсам");

					if (OnWidthRead != nullptr)
						OnWidthRead(this);



				}
				// MEASURE_TOTAL_ADDR
				// Иначе, если приняли данные результатов выполнения измерений
				else if (respList.begin()->registerAddr == MEASURE_TOTAL_ADDR) {


				Debug->MSG.Type="ENT";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";

					index = 7;

					for (uint8_t i = 0; i < PULSE_CHANNEL_COUNT; i++) {

						verifyData.totalPulsesCnt[i] =
							RegHandler_uint32(rxFrame, &index);

						Debug->MSG.Data= Debug->MSG.Data + "  "+IntToStr(index)+":"+FloatToStrF(verifyData.totalPulsesCnt[i],ffNumber,10,2)+ ", ";

						CumData.ImpTotal[i] = verifyData.totalPulsesCnt[i];
					}

					Debug->AddMSG (L" Приняли данные результатов выполнения измерений:");
					//
					// А теперь передадим полученные данные основной форме
					if (OnDataReceive != nullptr)
						OnDataReceive(this, verifyData);

					if (OnDataRead != nullptr)
						OnDataRead(this);

				}

				// FINAL_TIME_ADDR
				// Иначе, если приняли данные результатов выполнения измерений

				else if (respList.begin()->registerAddr == FINAL_TIME_ADDR) {

				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= L"   "+IntToStr(index)+" : "+IntToStr(rxFrame[index])+","+IntToStr(rxFrame[index+1])+","+IntToStr(rxFrame[index+2])+","+IntToStr(rxFrame[index+3]);

					FinalData.Time = RegHandler_float(rxFrame, &index);

					Debug->MSG.Data= Debug->MSG.Data + L" время: "+FloatToStrF(FinalData.Time,ffNumber,10,2)+ ", ";

				   Debug->AddMSG (L" Итоговые данные по времени");
				}

				// FINAL_DATA_ADDR
				// Иначе, если приняли данные результатов выполнения измерений

				else if (respList.begin()->registerAddr == FINAL_DATA_ADDR) {
					index = 3;

				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";


					for (uint8_t i = 0; i < PULSE_CHANNEL_COUNT; i++) {
						FinalData.ImpFinal[i] =
							RegHandler_float(rxFrame, &index);

					Debug->MSG.Data= Debug->MSG.Data + "  "+IntToStr(index)+":"+FloatToStrF(FinalData.ImpFinal[i],ffNumber,10,2)+ ", ";

					}
				   Debug->AddMSG (L" Итоговые данные по импульсам");

					if (OnFinalDataRead != nullptr)
						OnFinalDataRead(this);

					ChangeState(HSC_STATE_FINALREAD);

				}

				// COMPACT_CONFIG_ACCESS
				// Конфигурация Чемодана


				else if (respList.begin()->registerAddr == COMPACT_CONFIG_ACCESS) {
					index = 3;

				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";

				Access =   RegHandler_uint16(rxFrame, &index);

				Debug->MSG.Data= Debug->MSG.Data + " "+IntToStr(index)+": "+IntToStr(Access);

				Debug->AddMSG (L" Доступ к энергонезависимой памяти");

				if (OnAccessRead != nullptr)
				{
				  OnAccessRead(this);
				}


				}

				else if (respList.begin()->registerAddr == COMPACT_CONFIG_PASS) {
					index = 3;

				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";

				Password1 =   RegHandler_uint16(rxFrame, &index);

				Debug->MSG.Data= Debug->MSG.Data + " "+IntToStr(index)+": "+IntToStr(Password1);

				Debug->AddMSG (L"Пользовательский пароль");
				}

				else if (respList.begin()->registerAddr == COMPACT_CONFIG_SERVICEPASS) {
					index = 3;

				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";

				Password1 =   RegHandler_uint16(rxFrame, &index);

				Debug->MSG.Data= Debug->MSG.Data + " "+IntToStr(index)+": "+IntToStr(Password1);

				Debug->AddMSG (L"Сервисный пароль");
				}

				else if (respList.begin()->registerAddr == COMPACT_CONFIG_FACTORYPASS) {
					index = 3;

				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";

				Password2 =   RegHandler_uint16(rxFrame, &index);

				Debug->MSG.Data= Debug->MSG.Data + " "+IntToStr(index)+": "+IntToStr(Password2);

				Debug->AddMSG (L"Заводской пароль");
				}

				else if (respList.begin()->registerAddr == COMPACT_CONFIG_KP) {
					index = 3;

				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";

				Kp =   RegHandler_float(rxFrame, &index);

				Debug->MSG.Data= Debug->MSG.Data + " "+IntToStr(index)+": "+FloatToStr(Kp);

				Debug->AddMSG (L"Коэфициент преобразования");
				}


				else if (respList.begin()->registerAddr == COMPACT_CONFIG_SYNCH)
				{
					index = 3;

				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";

				SYNCH = RegHandler_uint16(rxFrame, &index);  //RegHandler_uint8(rxFrame, &index);;

				Debug->MSG.Data= Debug->MSG.Data + " "+IntToStr(index)+": "+IntToStr(SYNCH);

				Debug->AddMSG (L"Синхронизация");
				}


				// COMPACT_CONFIG_ADDR
				// Конфигурация Чемодана
				else if (respList.begin()->registerAddr == CALIBR_ETLVOL_ADDR) {
					index = 3;
				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";


					for (uint8_t i = 0; i < COEF_NUM ; i++) {
						cEtlVolume[i] =
							RegHandler_float(rxFrame, &index);

					Debug->MSG.Data= Debug->MSG.Data + "  "+IntToStr(index)+":"+FloatToStrF(cEtlVolume[i],ffNumber,10,2)+ ", ";

					}
				   Debug->AddMSG (L" Чтение Объемов эталона");

				}

				// CALIBR_TIME_ADDR
				// Конфигурация Чемодана
				else if (respList.begin()->registerAddr == CALIBR_TIME_ADDR) {
					index = 3;
				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";


					for (uint8_t i = 0; i < COEF_NUM ; i++) {
						cTime[i] =
							RegHandler_float(rxFrame, &index);

					Debug->MSG.Data= Debug->MSG.Data + "  "+IntToStr(index)+":"+FloatToStrF(cTime[i],ffNumber,10,2)+ ", ";

					}
				   Debug->AddMSG (L" Чтение Объемов эталона");

				}


				// CALIBR_IMP_ADDR
				// Конфигурация Чемодана
				else if (respList.begin()->registerAddr == CALIBR_IMP_ADDR) {
					index = 3;
				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";


					for (uint8_t i = 0; i < COEF_NUM; i++) {
						cImp[i] =
							RegHandler_float(rxFrame, &index);

					Debug->MSG.Data= Debug->MSG.Data + "  "+IntToStr(index)+":"+FloatToStrF(cImp[i],ffNumber,10,2)+ ", ";

					}
				   Debug->AddMSG (L" Чтение Объемов эталона");

				}


				// CALIBR_COEF_ADDR
				// Конфигурация Чемодана
				else if (respList.begin()->registerAddr == CALIBR_COEF_ADDR) {
					index = 3;
				Debug->MSG.Type="RRD";
				Debug->MSG.Direction="PacketHandler";
				Debug->MSG.Data= "   ";


					for (uint8_t i = 0; i < COEF_NUM ; i++) {
						cCoef[i] =
							RegHandler_float(rxFrame, &index);

					Debug->MSG.Data= Debug->MSG.Data + "  "+IntToStr(index)+":"+FloatToStrF(cCoef[i],ffNumber,10,2)+ ", ";

					}
				   Debug->AddMSG (L" Чтение Объемов эталона");


						if (OnConfigRead != nullptr)
					   {
						OnConfigRead(this);
						 }

					   if (hscState==HSC_STATE_CONFIG)
					   {
						 ChangeState(HSC_STATE_PING);
					   }
				}


}

// ---------------------------------------------------------------------------
void __fastcall THscDevice::OnMainTimer(TObject *Sender) {
	switch (hscState) {
	case HSC_STATE_STANDBY: // < Хуем груши околачиваем.... :-)
		break;

	case HSC_STATE_CONFIG: // < Подключились, мониторим подключение, ждем команды юзера
        ConfigDevice();
		break;

	case HSC_STATE_PING: // < Подключились, мониторим подключение, ждем команды юзера
		PingDevice();
	   //	MonitorExecute();
		break;

	case HSC_STATE_STARTTEST: // < Выполняем проливку, основная функция устройства
		MeasureExecute();
		break;

	case HSC_STATE_EXECUTE: // < Выполняем проливку, основная функция устройства
		MeasureExecute();
		break;

    case HSC_STATE_MONITOR: // < Выполняем проливку, основная функция устройства
		MonitorExecute();
		break;

	case HSC_STATE_STOPTEST: // < Выполняем проливку, основная функция устройства
		MeasureExecute();
		break;

	case HSC_STATE_COMPLETE: // < Операция выполнена, подведение итогов
		break;

	case HSC_STATE_FAILURE 	: // < Произошел сбой, обработка ошибки работы устройства
         {
		 PingDevice();
         btThread->btState       = btThread->BT_READY;
         }
		break;

	case HSC_STATE_FINALREAD:
       break;

	default:
		break;
	}
}

// ---------------------------------------------------------------------------
// Обработчик таймера времени ожидания ответа от устройства
void __fastcall THscDevice::OnRespTimer(TObject *Sender) {

    //Первое отсутствие ответа - нужен повтор
   if (error_status==0)
   {
        btThread->SendData();
		//respTimer->Enabled = true;
        Debug->MSG.Type="ERR";
        Integer cnt= respList.size();
        Debug->MSG.Data=L"error_status==0 "+  IntToStr(cnt);
		Debug->AddMSG (L" Ошибка: не получен ответ, повторяем отправку");

        error_status=1;
   }
    //Второе отсутствие ответа - нужен повтор
   else if (error_status==1)
   {
        btThread->SendData();
	   // respTimer->Enabled = true;
        Debug->MSG.Type="ERR";
        Integer cnt= respList.size();
        Debug->MSG.Data=L"error_status==1 "+  IntToStr(cnt);
		Debug->AddMSG (L" Ошибка: повторно не получен ответ, повторяем отправку");

        error_status=2;

   }

   else if (error_status==2)              //Здесь что-то не работает
	  {
		respTimer->Enabled = false;       // Остановка повторных запросов!

		Debug->MSG.Type="ERR";
		Integer cnt= respList.size();
		Debug->MSG.Data=L"Кол-во элементов в списке: "+  IntToStr(cnt);
		Debug->AddMSG (L" Убираем НЕ обработанный элемент из списка ожидания");

		error_status=0;
		ChangeState(HSC_STATE_FAILURE);
		  btThread->IsReciving = false;
		respList.clear();
	   //	TThread::Synchronize(NULL, Debug->OnError);
   }

   //












     //	respList.pop_front();



		// < Убираем обработанный элемент из списка ожидания



}

void THscDevice::ConfigDevice(void) {
	static uint32_t pingTicks = 0;
	const uint32_t ticksPassed = TThread::GetTickCount() - pingTicks;
	//
	// Пингуем устройство не чаще, чем PING_INTERVAL мс
	if  ((ticksPassed >= CONFIG_INTERVAL)&&(PingEnable))
    {
  	 	Read_ConfigData();
		pingTicks = TThread::GetTickCount();
	}
}


// ---------------------------------------------------------------------------
void THscDevice::PingDevice(void) {
	static uint32_t pingTicks = 0;
	const uint32_t ticksPassed = TThread::GetTickCount() - pingTicks;
	//
	// Пингуем устройство не чаще, чем PING_INTERVAL мс
	if  ((ticksPassed >= PING_INTERVAL)&&(PingEnable))
    {
		ReadDataFromDevice(DEVICE_REVISION_ADDR, 3);
		ReadDataFromDevice(DEVICE_CHARGE_ADDR, 1);
		pingTicks = TThread::GetTickCount();
	}




}

// ---------------------------------------------------------------------------
// Функция выполнения опроса устройства во время выполнеия проливки
void THscDevice::MeasureExecute(void) {
	TBytes txData;
	const uint32_t ticks = TThread::GetTickCount();
	const uint32_t ticksElapsed = ticks - measureTimeTicks;

	if (ticksElapsed >= MEASURE_PERIOD) {
		//
		// Защелка для отсечки: сохранение в регистрах  данных о последней секунде
		txData.set_length(2);
		txData[0] = 0x00;
		txData[1] = 0x00;
		WriteDataToDevice(FIX_VALUES_REG_ADDR, txData);

		measureTimeTicks = ticks;
		//
		// Отправим запрос на чтение промежуточных результатов
		ReadDataFromDevice(MEASURE_DATA_ADDR, 12);

		//
		// Запросим данные объема
		ReadDataFromDevice(MEASURE_TOTAL_ADDR, 22);
	}
}


void THscDevice::MonitorExecute(void) {
	TBytes txData;
	const uint32_t ticks = TThread::GetTickCount();
	const uint32_t ticksElapsed = ticks - measureTimeTicks;

	if (ticksElapsed >= MEASURE_PERIOD)
	{
		//
		// Защелка для отсечки: сохранение в регистрах  данных о последней секунде
		txData.set_length(2);
		txData[0] = 0x00; //
		txData[1] = 0x00; //
		WriteDataToDevice(FIX_VALUES_REG_ADDR, txData);

		measureTimeTicks = ticks;
		//
		// Отправим запрос на чтение промежуточных результатов
			 ReadDataFromDevice(MEASURE_WIDTH_ADDR, 10);
			 ReadDataFromDevice(IMP_DATA_ADDR, 10);

		 if (PeriodState) {

		 }  else
		{

		}
		//
		// Запросим данные объема
	  //  ReadDataFromDevice(MEASURE_TOTAL_ADDR, 22);
	}
}




// ---------------------------------------------------------------------------
void THscDevice::ChangeState(HscStates_t newState) {


	hscState = newState;

	if (OnChangeState != nullptr) {
		OnChangeState(this);
	}

}

// ---------------------------------------------------------------------------
// Сброс данных о предыдущих измерениях в устройстве
void THscDevice::ResetDeviceData(void) {
	TBytes DummyBytes;

	DummyBytes.set_length(0);
	WriteDataToDevice(RESET_MEASURE_ADDR, DummyBytes);
}

float THscDevice::GetAvrImpulses(uint8_t CH) {
	uint16_t Sum = 0;
	uint8_t cnt;
	uint8_t i = 0;
	// imp_cnt - колтчество записей в буфере
	// imp_rd  - текущая позиция для чтения

	// for (uint8_t i=0; i<imp_cnt; i++)
	do {
		if (rd_imp >= i) {
			cnt = rd_imp - i;
		}
		else {
			cnt = cnt_imp + rd_imp - i;
		}

		Sum += Impulses[CH, cnt];
		i++;

	}
	while ((Sum < 100) && (i < cnt_imp));

	return Sum / i;

}

void THscDevice::SetOnChargeRead(TOnDataRead OnRead) {
	OnChargeRead= OnRead;
}

void THscDevice::SetOnVersionRead(TOnDataRead OnVerRead) {
	OnVersionRead = OnVerRead;
}

void THscDevice::SetOnSettingsRead(TOnDataRead OnSetRead) {
	OnSettingsRead = OnSetRead;
}

void THscDevice::SetOnCheckStateRead(TOnDataRead OnCheckSt) {
	OnCheckStateRead = OnCheckSt;
}

void THscDevice::SetOnSecDataRead(TOnDataRead OnSecDat) {
	OnSecDataRead = OnSecDat;
}

void THscDevice::SetOnImpDataRead(TOnDataRead OnSecDat) {
	OnImpDataRead = OnSecDat;
}


void THscDevice::SetOnFinalDataRead(TOnDataRead OnDat) {
	OnFinalDataRead = OnDat;
}

void THscDevice::SetOnDataRead(TOnDataRead OnDat) {
	OnDataRead = OnDat;
}

void THscDevice::SetOnConfigRead(TOnDataRead OnDat) {
	OnConfigRead = OnDat;
}

void THscDevice::SetOnConfigMainRead(TOnDataRead OnDat) {
	OnConfigMainRead = OnDat;
}


void THscDevice::Read_Version(void) {
	ReadDataFromDevice(DEVICE_REVISION_ADDR, 3);
}

void THscDevice::Read_Settings(void) {
	ReadDataFromDevice( DEVICE_SETTINGS_ADDR, 13);
}

void THscDevice::Read_CheckState(void) {
	ReadDataFromDevice(DEVICE_CHECKSTATE_ADDR, 5);
}

void THscDevice::Read_ConfigAccess(void) {

    ReadDataFromDevice(COMPACT_CONFIG_ACCESS, 1);

}
void THscDevice::Read_ConfigData(void) {

	ReadDataFromDevice( DEVICE_CHARGE_ADDR, 1);
    ReadDataFromDevice(COMPACT_CONFIG_ACCESS, 5);
   	ReadDataFromDevice(COMPACT_CONFIG_PASS, 1);
    ReadDataFromDevice(COMPACT_CONFIG_KP, 	2);
    ReadDataFromDevice(COMPACT_CONFIG_SYNCH,1);
    ReadDataFromDevice(CALIBR_ETLVOL_ADDR,  40);
    ReadDataFromDevice(CALIBR_TIME_ADDR, 	40);
    ReadDataFromDevice(CALIBR_IMP_ADDR, 	40);
    ReadDataFromDevice(CALIBR_COEF_ADDR, 	40);


    Temp = Temp+1;
    }





void THscDevice::Read_PIN(void) {
	ReadDataFromDevice(COMPACT_CONFIG_SERVICEPASS, 1);
}

void THscDevice::Save_PIN(int pin) {
	uint32_t addr;

	WriteReg(COMPACT_CONFIG_SERVICEPASS, (uint16_t)pin);
}

void THscDevice::Write_PIN(int pin) {
	uint32_t addr;

	WriteReg(COMPACT_CONFIG_PIN , (uint16_t)pin);

}

void THscDevice::Write_STATUS(int status) {
	uint32_t addr;
   if (DeviceInfo.DeviceRevision>50091) {
		WriteReg(COMPACT_CONFIG_STATUS , (uint16_t)status);
   }


}


void THscDevice::Write_SYNCH(int syn)
{

	WriteReg(COMPACT_CONFIG_SYNCH , (uint16_t)syn);

}


void THscDevice::Write_Coef(float fltK)
{
	WriteReg(COMPACT_CONFIG_KP, (float)fltK);

}



void THscDevice::Save_Flash(void)
{

   WriteReg(COMPACT_CONFIG_ACCESS, (uint16_t)1);

}
void THscDevice::Load_Flash(void)
{

   WriteReg(COMPACT_CONFIG_ACCESS, (uint16_t)0);

}

void THscDevice::Save_CalibrData(float cEtlVolume[20], float cTime[20], float cImp [20], float cCoefs[20])
{

   WriteReg(CALIBR_ETLVOL_ADDR, cEtlVolume,80 /*sizeof(cEtlVolume)*/);
   WriteReg(CALIBR_TIME_ADDR , cTime, 80/*sizeof(cTime)*/);
   WriteReg(CALIBR_IMP_ADDR, cImp, 80/*sizeof(cImp)*/);
   WriteReg(CALIBR_COEF_ADDR, cCoefs,80 /*sizeof(cCoefs)*/);


}


// Записать настройки
void THscDevice::Write_Settings(tSettings *vSettings) {
	TBytes txData;
	uint32_t addr;
	uint16_t data = 0;

    Settings = *vSettings;

	addr = DEVICE_SETTINGS_ADDR;

	// Write Simulation

	WriteReg(addr, (uint16_t)Settings.Simulator);

	//Settings.StartCHNum = 1;
	if (Settings.StartSettings==0) {
		data = 0;
	}
	else if ((Settings.StartCHNum > 0)&&(Settings.StartSettings>6)) {
		data = Settings.StartCHNum;
		data |= (static_cast<uint8_t>(Settings.StartSettings)-6 )<< 8;
	}
	else {
		data = Settings.StartSettings;
	}

	WriteReg(addr + 1, data);

	if (Settings.StopSettings==0) {
		data = 0;
	}
	else if ((Settings.StopCHNum > 0)&&(Settings.StopSettings>6)) {
		data = Settings.StopCHNum;
		data |= static_cast<uint8_t>(Settings.StopSettings-6) << 8;
	}
	else {
		data = Settings.StopSettings;
	}

	WriteReg(addr + 2, data);

	WriteReg(addr + 3, Settings.TimeLimit);

	WriteReg(addr + 4, (uint16_t)Settings.ImpLimit);

	WriteReg(addr + 6, (uint16_t)Settings.IntSyn);


	if (Settings.ExtSyn==0) {
		data = 0;
	}
	else if ((static_cast<uint8_t>(Settings.ExtSyn) > 0)&&(static_cast<uint8_t>(Settings.OutSynCH)>0)) {
		data = static_cast<uint8_t>(Settings.OutSynCH);
		data |= static_cast<uint8_t>(Settings.ExtSyn) << 8;
	}
	else {
		data = static_cast<uint8_t>(Settings.ExtSyn);
	}

	WriteReg(addr + 7, data);
}


void THscDevice::Write_Channel_State(uint16_t State) {
	TBytes txData;
	uint32_t addr;
	uint16_t data = 0;

	addr = CHANNEL_1_CONFIG_ADDR;

	WriteReg(addr, State);

}

void THscDevice::Write_Channel_State(uint8_t Channel, uint16_t State)
{
	TBytes txData;
	uint32_t addr;
	uint16_t data = 0;
	 //	Chh =   5+ (C<<4)+ 0x4000;
	addr =  4+ (Channel<<4)+0x4000;

	WriteReg(addr, State);

}

void THscDevice::Write_TimeLimit(uint16_t TimeLimit) {
	TBytes txData;
	uint32_t addr;
	uint16_t data = 0;

	addr = DEVICE_SETTINGS_ADDR;

	WriteReg(addr + 3, TimeLimit);

}

void THscDevice::Write_ImpLimit(uint16_t ImpLimit) {
	TBytes txData;
	uint32_t addr;
	uint16_t data = 0;

	addr = DEVICE_SETTINGS_ADDR;

	WriteReg(addr + 4, ImpLimit);

}

void THscDevice::Read_SecData(void) {
	ReadDataFromDevice( MEASURE_DATA_ADDR, 12);
}

void THscDevice::Read_FinalData(void) {
	ReadDataFromDevice(FINAL_DATA_ADDR, 24);
}

void THscDevice::Read_FinalTime(void) {
	ReadDataFromDevice(FINAL_TIME_ADDR, 2);
}

uint8_t THscDevice::GetMaxChannel(void) {
	uint8_t cnt;

	cnt = PULSE_CHANNEL_COUNT;
	return cnt;
}

float THscDevice::GetTotalImp(uint8_t CH) {
	float Imp;
	Imp = static_cast<float>(CumData.ImpTotal[CH]);
	return Imp;
}

float THscDevice::GetSecImp(uint8_t CH) {
    float SecImp;
    SecImp =  (float) SecData.ImpPerSec[CH];

	return (float)SecImp;
}

float THscDevice::GetWidth(uint8_t CH) {
	float Width;
	Width =  (float) SecData.Width[CH];

	return (float)Width;
}

float THscDevice::GetTime(void) {
	return (float)SecData.Time;
}

float THscDevice::GetFinalTime(void)  {
	return (float)FinalData.Time;
}

float THscDevice::GetFinalImp(uint8_t CH) {
	return (float)FinalData.ImpFinal[CH];
}

UnicodeString THscDevice::GetCharge(void)
{
	float  f,p,d;
	f = Charge;
	d = ((f*0.026)-2.9);
	p =  (float)(d*83.3333);     //  =(((ADC*0,026)-2,9)/1,2)*100

	 if (p>100) {return L"100%"; }
	 if (p<0) { return L"-";}

	 return (FloatToStrF(p,ffNumber,10,0)+'%');


}

int THscDevice::GetValueCharge(void)
{
	float  f,p,d;
	f = Charge;
	d = ((f*0.026)-2.9);
	p =  (float)(d*83.3333);

	 if (p>100) {return 100; }
	 if (p<0) { return 0;}

	 return (int)p;


}
// ---------------------------------------------------------------------------
#pragma package(smart_init)
// ---------------------------------------------------------------------------
