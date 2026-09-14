//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "FormTypeSelection.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TTypeSelectionForm *TypeSelectionForm;
//---------------------------------------------------------------------------
__fastcall TTypeSelectionForm::TTypeSelectionForm(TComponent* Owner)
	: TForm(Owner)
{

 EditSearch->Text = "";



}
//---------------------------------------------------------------------------
void __fastcall TTypeSelectionForm::FormShow(TObject *Sender)
{
 int i;
   /*       for (int i=0;i<CoefStringGrid->RowCount;i++)
	{
	   CoefStringGrid->Cells[0][i]= "";
	   CoefStringGrid->Cells[1][i]= "";
	   CoefStringGrid->Cells[2][i]= "";
	   CoefStringGrid->Cells[3][i]= "";
	   CoefStringGrid->Cells[4][i]= "";
	   CoefStringGrid->Cells[5][i]= "";
	   CoefStringGrid->Cells[6][i]= "";
	}

   */
	TFlowMeterType::LoadTypesFromFile();

			  if (FlowMeter!=nullptr)
		{
			if (TryStrToInt(FlowMeter->year_production,i)) {
			   EditYear->Text = FlowMeter->year_production;
			}

		  if (FlowMeter->Type!=nullptr)
		   {
			 InitHashTag =  FlowMeter->Type->Hash;
			 HashTag = InitHashTag;
		   }
		}

	 StringGrid1->Row = -1;
	UpdateGrid();
	 UpdateList();
	 FillListView();

   }
//---------------------------------------------------------------------------
void __fastcall TTypeSelectionForm::DeleteConfigButtonClick(TObject *Sender)
{

		UnicodeString S=StringGrid1->Cells[15][StringGrid1->Row];

  //TFlowMeterType::MeterTypes.erase(TFlowMeterType::MeterTypes.begin()+StringGrid1->Row);
  //TFlowMeterType::MeterTypes[StringGrid1->Col]->erase();

		 TFlowMeterType::Erase(S);

	UpdateGrid();
}
//---------------------------------------------------------------------------

void TTypeSelectionForm::UpdateGrid()
{
   int ARow = -1;
   int index=0;
   int hash=-1, it;

	UnicodeString S;



	   if (!TFlowMeterType::MeterTypes.empty())
	{




	  StringGrid1->RowCount = 0;

	for (int j=0; j<TFlowMeterType::MeterTypes.size();j++)
	{
	  if (  (!(TFlowMeterType::filter))||(

(TFlowMeterType::filter)&&(TFlowMeterType::MeterTypes[j]->Visible) )) {



	  StringGrid1->RowCount = StringGrid1->RowCount+1;
	  StringGrid1->Cells[0][index] = TFlowMeterType::MeterTypes[j]-> CertificateNum;
	  StringGrid1->Cells[1][index] = TFlowMeterType::MeterTypes[j]-> DeviceType;
	  StringGrid1->Cells[2][index] = TFlowMeterType::MeterTypes[j]-> DeviceName;
	  StringGrid1->Cells[3][index] = TFlowMeterType::MeterTypes[j]-> Modification;
	  StringGrid1->Cells[4][index] = TFlowMeterType::MeterTypes[j]-> VerificationNum;
	  StringGrid1->Cells[5][index] = TFlowMeterType::MeterTypes[j]-> VerificationInterval1;
	  StringGrid1->Cells[6][index] = TFlowMeterType::MeterTypes[j]-> VerificationInterval2;
	  StringGrid1->Cells[7][index] = TFlowMeterType::MeterTypes[j]-> SerialNum;
	  StringGrid1->Cells[8][index] = FloatToStrF(TFlowMeterType::MeterTypes[j]->Qmax,ffNumber,10,2);
	  StringGrid1->Cells[9][index] = TFlowMeterType::MeterTypes[j]-> Description;
	  StringGrid1->Cells[10][index] = TFlowMeterType::MeterTypes[j]-> Date1;
	  StringGrid1->Cells[11][index] = TFlowMeterType::MeterTypes[j]-> Date2;
	  StringGrid1->Cells[12][index] = TFlowMeterType::MeterTypes[j]-> Manufacturer;
	  StringGrid1->Cells[13][index] = TFlowMeterType::MeterTypes[j]-> Data1;
	  StringGrid1->Cells[14][index] = TFlowMeterType::MeterTypes[j]-> Data2;
	  StringGrid1->Cells[15][index] = IntToStr(TFlowMeterType::MeterTypes[j]-> Hash);

	  if (HashTag==TFlowMeterType::MeterTypes[j]-> Hash)
	  {
			 ARow=index;
	  }

	  index = index+1;
		}
	}
	  StringGrid1->Row = ARow;
	}

	}


void TTypeSelectionForm::UpdateList()
{

   int index=0;

	UnicodeString S;
	UnicodeString str;
	TValue value;



	ListView1->BeginUpdate();
try {



	ListView1->Items->Clear();
	   if (!TFlowMeterType::MeterTypes.empty())
	{

		for (int j=0; j<TFlowMeterType::MeterTypes.size();j++)
	   {

	  //if (  (!(TFlowMeterType::filter))||(
	  //	(TFlowMeterType::filter)&&(TFlowMeterType::MeterTypes[j]->Visible) )) {

		if (IsInInterval(TFlowMeterType::MeterTypes[j]-> Date1,
						  TFlowMeterType::MeterTypes[j]-> Date2,
							 EditYear->Text)!=-1)
		{

		TListViewItem* item = ListView1->Items->Add();

		item->Tag =  TFlowMeterType::MeterTypes[j]-> Hash;

		str = TFlowMeterType::MeterTypes[j]-> CertificateNum;
		value = str;
		item->Data["CertificateNum"] = value ;

		 str = TFlowMeterType::MeterTypes[j]-> DeviceType;
		 value = str;
		 item->Data["DeviceName"] = value ;

		str = TFlowMeterType::MeterTypes[j]-> DeviceName;
		value = str;
		item->Data["Modification"] = value ;

		str = TFlowMeterType::MeterTypes[j]-> Date1 + L" - "+
			TFlowMeterType::MeterTypes[j]-> Date2;
		value = str;
		item->Data["Date"] = value ;

		str = TFlowMeterType::MeterTypes[j]-> Manufacturer;
		value = str;
		item->Data["Manufacturer"] = value ;

		str = TFlowMeterType::MeterTypes[j]-> Description;
		value = str;
		item->Data["Description"] = value ;


	  if (HashTag==TFlowMeterType::MeterTypes[j]-> Hash)
	  {
			 ListView1->ItemIndex=index;
	  }

	  index = index+1;
		}
			}
	}

   }

 catch (...) { }
ListView1->EndUpdate();

	}


void __fastcall TTypeSelectionForm::LoadConfigButtonClick(TObject *Sender)
{
	TFlowMeterType::LoadTypesFromFile();
	UpdateGrid();
}
//---------------------------------------------------------------------------




void __fastcall TTypeSelectionForm::ApplyButtonClick(TObject *Sender)
{
	 this -> Close();
}
//---------------------------------------------------------------------------


void   TTypeSelectionForm::SetFlowMeter(TFlowMeter *Meter)
{
	 FlowMeter = Meter;

}


void   TTypeSelectionForm::SetFlowMeterType()
{

	TFlowMeterType*Type;

	if (!(FlowMeter==nullptr))
	{
	 if (HashTag>0)
	 {
	  Type = TFlowMeterType::Get(HashTag);
	   if (Type!=nullptr) {
		 FlowMeter->SetType(Type);
	   }

	 }

	}

	 if (OnUpdate!=nullptr)
	 {
	 OnUpdate(this);
	 }


}

void   TTypeSelectionForm::SetOnUpdate(TOnUpdate onUpdate)
{
	 OnUpdate = onUpdate;

}

void __fastcall TTypeSelectionForm::LoadTypesMenuItemClick(TObject *Sender)
{
	TFlowMeterType::LoadTypesFromFile();
    UpdateGrid();
}
//---------------------------------------------------------------------------


void __fastcall TTypeSelectionForm::DeleteTypesMenuItemClick(TObject *Sender)
{
	UnicodeString S=StringGrid1->Cells[15][StringGrid1->Row];
		int i;
  //TFlowMeterType::MeterTypes.erase(TFlowMeterType::MeterTypes.begin()+StringGrid1->Row);
  //TFlowMeterType::MeterTypes[StringGrid1->Col]->erase();
	 if (TryStrToInt(S,i)) {
		 TFlowMeterType::Erase(i);
	 }


 // TFlowMeterType::MeterTypes[StringGrid1->Col]->erase();

      UpdateGrid();
}
//---------------------------------------------------------------------------


void __fastcall TTypeSelectionForm::FormHide(TObject *Sender)
{
	SetFlowMeterType();
}
//---------------------------------------------------------------------------


void __fastcall TTypeSelectionForm::CancelButtonClick(TObject *Sender)
{
	HashTag = InitHashTag;
		this -> Close();
}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::Button2Click(TObject *Sender)
{
         this -> Close();
}
//---------------------------------------------------------------------------


void __fastcall TTypeSelectionForm::CopyTypeMenuItemClick(TObject *Sender)
{

	//TFlowMeterType::MeterTypes.erase(TFlowMeterType::MeterTypes.begin()+StringGrid1->Row);

    UpdateGrid();
}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::MenuButtonClick(TObject *Sender)
{
	TPointF FP;
	// Initialize the coordinates to the origin of the button control.
	FP.X = 10;
	FP.Y = 10;
	// Transposes the coordinates in the context of the form.
	FP = MenuButton->LocalToAbsolute(FP);
	// Transposes the coordinates in the context of the screen.
	FP = ClientToScreen(FP);
	// Display the popup menu at the computed coordinates.
	PopupMenu1->Popup(FP.X, FP.Y);
}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::SaveTypesMenuItemClick(TObject *Sender)
{
		TFlowMeterType::SaveTypesToFile();
}
//---------------------------------------------------------------------------



void TTypeSelectionForm::Refresh ()
{


   String str1="",str2="";
   float Q2,Qetl,error;
   float Qrate, Flow, Volume, Time, Imp, Error;
   float f;
   bool succes=true;
   char chr = 160U;

   TFlowMeterType*Type;
	  // CoefStringGrid->Cells[6][i]= FloatToStrF(EtalonMeter->CalibrPoints[i].Error,ffNumber,10,1);

   for (int j=0;j<StringGrid1->RowCount;j++)
   {

		Type =  TFlowMeterType::Get(StringGrid1->Cells[15][j]);

	  if(j< TFlowMeterType::MeterTypes.size())
	  {
	  Type->CertificateNum = StringGrid1->Cells[0][j];
	  Type->DeviceType = StringGrid1->Cells[1][j];
	  Type->DeviceName = StringGrid1->Cells[2][j];
	  Type->Modification = StringGrid1->Cells[3][j];
	  Type->VerificationNum = StringGrid1->Cells[4][j];
	  Type->VerificationInterval1 = StringGrid1->Cells[5][j];
	  Type->VerificationInterval2 = StringGrid1->Cells[6][j];
	  Type-> SerialNum =   StringGrid1->Cells[7][j];
	  Type->Qmax = StrToFloat_(StringGrid1->Cells[8][j]);
	  Type->Description = StringGrid1->Cells[9][j];
	  Type-> Date1 =  StringGrid1->Cells[10][j];
	  Type-> Date2 =  StringGrid1->Cells[11][j];
	  Type-> Manufacturer =  StringGrid1->Cells[12][j];
	  Type-> Data1 =  StringGrid1->Cells[13][j];
	  Type-> Data2 =  StringGrid1->Cells[14][j];
	  }

   }






}
void __fastcall TTypeSelectionForm::Button1Click(TObject *Sender)
{
		UnicodeString S;
		int ARow = StringGrid1->Row;
		int i;
			TFlowMeterType*Type;
  //TFlowMeterType::MeterTypes.erase(TFlowMeterType::MeterTypes.begin()+StringGrid1->Row);
  //TFlowMeterType::MeterTypes[StringGrid1->Col]->erase();



	   if (ARow!=-1) {
		  Type=  TFlowMeterType::Get(StringGrid1->Cells[15][ARow]);

		  if (Type!=nullptr) {
			TFlowMeterType::DuplicateType(Type);
			UpdateGrid();
			return;
		  }

		}


	   {
		TFlowMeterType	*MeterType = new TFlowMeterType();
		MeterType->AddTypeToList();
		UpdateGrid();
	   }


}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::RefreshButtonClick(TObject *Sender)
{
	Refresh ();
	UpdateGrid();
}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::SaveConfigButtonClick(TObject *Sender)
{
	 TFlowMeterType::SaveTypesToFile();
}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::StringGrid1EditingDone(TObject *Sender, const int ACol,
		  const int ARow)
{
	  Refresh ();
}
//---------------------------------------------------------------------------


void __fastcall TTypeSelectionForm::EditSearchKeyUp(TObject *Sender, WORD &Key, System::WideChar &KeyChar,
		  TShiftState Shift)
{   /*
	int hash;
	TFlowMeterType *Type;
	UnicodeString M, text, S;
	bool IsContain;

	text = EditSearch->Text;

	if (text =="") {
				   TFlowMeterType::filter = 0;
				  // SearchEdit->ResetFocus();
	}


   else	if (Key == 13U) {
		EditSearch->ResetFocus();
	}

  //	else if ((System::Character::IsLetter((System::WideChar)KeyChar))||(System::Character::IsNumber((System::WideChar)KeyChar)))
   else
  {
	TFlowMeterType::filter = 1;

	  for (int j=0;j<TFlowMeterType::MeterTypes.size();j++)   //StringGrid1->RowCount
   {
	  IsContain = false;
	  Type = TFlowMeterType::MeterTypes[j];
	  hash = Type->Hash;// StrToInt(StringGrid1->Cells[8][j]);


	  if (Type!=nullptr) {
	   for (int i=0;i<8;i++)
		{
	   //	if(j< TFlowMeterType::MeterTypes.size())

	  S = TFlowMeterType::MeterTypes[j]-> CertificateNum;
			 if (StrPos(S.c_str(),text.c_str()))
		 {
		   IsContain = true;
		   Type -> Visible = true;
		 }
	  S = TFlowMeterType::MeterTypes[j]-> DeviceType;
				 if (StrPos(S.c_str(),text.c_str()))
		 {
		   IsContain = true;
		   Type -> Visible = true;
		 }
	  S = TFlowMeterType::MeterTypes[j]-> Modification;
				 if (StrPos(S.c_str(),text.c_str()))
		 {
		   IsContain = true;
		   Type -> Visible = true;
		 }
	  S = TFlowMeterType::MeterTypes[j]-> Description;
				 if (StrPos(S.c_str(),text.c_str()))
		 {
		   IsContain = true;
		   Type -> Visible = true;
		 }

	  S = TFlowMeterType::MeterTypes[j]-> Name;
				 if (StrPos(S.c_str(),text.c_str()))
		 {
		   IsContain = true;
		   Type -> Visible = true;
		 }

		  S = TFlowMeterType::MeterTypes[j]-> Data1;
				 if (StrPos(S.c_str(),text.c_str()))
		 {
		   IsContain = true;
		   Type -> Visible = true;
		 }

		 S = TFlowMeterType::MeterTypes[j]-> Data2;
				 if (StrPos(S.c_str(),text.c_str()))
		 {
		   IsContain = true;
		   Type -> Visible = true;
		 }

		 S = TFlowMeterType::MeterTypes[j]-> Data3;
				 if (StrPos(S.c_str(),text.c_str()))
		 {
		   IsContain = true;
		   Type -> Visible = true;
		 }
		}


	}


	 if (IsContain) {
		Type -> Visible = true;
	 }  else
{
	  Type -> Visible = false;
}
  }

  }
   UpdateGrid(); */
}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::FillListView()
{

 }

void __fastcall TTypeSelectionForm::ListView1Change(TObject *Sender)
{
	UnicodeString  text;
	//FlowMeter->Type->Hash = ((TListViewItem*)(ListView1->Selected))->Tag;
	HashTag =  ((TListViewItem*)(ListView1->Selected))->Tag;
   //	EditSearchList->Text = text;
	 ///ListView1->Selected->Text;
}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::TabItem1Click(TObject *Sender)
{
		 UpdateList();
}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::TimerFilterTimer(TObject *Sender)

{

	int hash;
	TFlowMeterType *Type;
	UnicodeString M, text, S;
	bool IsContain;

	text = EditSearch->Text;
	text = text.Trim();

	 if (text=="") {
		TFlowMeterType::filter = 0;
		TFlowMeterType::FilterList.clear();
		UpdateGrid();
		//TestMeter->Type = NULL;
	 }

  else if  ( TimerFilter->Tag == 1)
	{
	  TimerFilter->Tag = 0;
	}


	else if (text!=Str)
  {

  /*	if (text =="") {
				   TFlowMeterType::filter = 0;
				  // EditName->ResetFocus();
	}    */


  //	else if ((System::Character::IsLetter((System::WideChar)KeyChar))||(System::Character::IsNumber((System::WideChar)KeyChar)))
  // else




	TFlowMeterType::filter = 1;

	TFlowMeterType::FilterList.clear();
	  IsContain = false;
	  for (int j=0;j<TFlowMeterType::MeterTypes.size();j++)   //StringGrid1->RowCount
  {

	  Type = TFlowMeterType::MeterTypes[j];
	  hash = Type->Hash;// StrToInt(StringGrid1->Cells[8][j]);


	  if (Type!=nullptr) {

	   //	if(j< TFlowMeterType::MeterTypes.size())

	  S = TFlowMeterType::MeterTypes[j]-> CertificateNum;

	  S = S+ TFlowMeterType::MeterTypes[j]-> DeviceType;

	  S = S+ TFlowMeterType::MeterTypes[j]-> DeviceName;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Modification;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Description;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Manufacturer;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Data1;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Data2;

	  S = S+ TFlowMeterType::MeterTypes[j]-> Data3;


		if (StrPos(S.c_str(),text.c_str()))
		 {
		   if (IsInInterval(TFlowMeterType::MeterTypes[j]-> Date1,
						  TFlowMeterType::MeterTypes[j]-> Date2,
							 EditYear->Text)!=-1)
			{
				IsContain = true;
				Type -> Visible = true;
				TFlowMeterType::FilterList.push_back(j);
				EditSearch->TextSettings->FontColor = claWhite;
				EditSearch->Enabled = true;
			}  else
			  {
				Type -> Visible = false;
			  }

		 }   else
		 {
				Type -> Visible = false;
		 }



	}




  }

	if (!IsContain) {
	   EditSearch->TextSettings->FontColor = claGrey;
	   //ButtonComboDown->Enabled = false;
	 }

  UpdateGrid();

 }
	Str = text;
//---------------------------------------------------------------------------
	}

//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::EditSearchKeyDown(TObject *Sender, WORD &Key,
          System::WideChar &KeyChar, TShiftState Shift)
{
TimerFilter->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::EditSearchExit(TObject *Sender)
{
		TimerFilter->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::StringGrid1SelChanged(TObject *Sender)
{
	int ARow = StringGrid1->Row;
	int hash;

	if (!(FlowMeter==nullptr))
	{
	 if (ARow>=0)
	 {
		if (TryStrToInt(StringGrid1->Cells[15][ARow], hash))
	   {
			HashTag =  hash;
	   } else
		{
			HashTag = 0;
		}
	 }
	}
}
//---------------------------------------------------------------------------


void __fastcall TTypeSelectionForm::TabItem2Click(TObject *Sender)
{
	UpdateGrid();
	}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::ListView1ItemClick(TObject * const Sender, TListViewItem * const AItem)

{
	UnicodeString  text;
   //	FlowMeter->Type->Hash = ((TListViewItem*)(ListView1->Selected))->Tag;
	HashTag =  ((TListViewItem*)(ListView1->Selected))->Tag;


	if (FlowMeter!=nullptr) {
		if (FlowMeter->Type!=nullptr) {
		FlowMeter->Type->Hash = ((TListViewItem*)(ListView1->Selected))->Tag;
	}
	}

}
//---------------------------------------------------------------------------

void __fastcall TTypeSelectionForm::EditYearExit(TObject *Sender)
{
UpdateList();
}
//---------------------------------------------------------------------------

