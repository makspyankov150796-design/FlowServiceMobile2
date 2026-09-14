//---------------------------------------------------------------------------

#ifndef TMeterValueH
#define TMeterValueH
//---------------------------------------------------------------------------
  #include "UserRoutines.h"
#include <math.h>
 // #include <ustring.h>

#include <vector>
#include <deque>

 #include <time.h>
  #include <System.DateUtils.hpp >

//#define ARRAY_SIZE 100

typedef struct TDimension
{
	UnicodeString Name;
	float Rate;
}Dimension;

class TMeterValue
{
	private:

	int filter_order=-1;
	int filter_rd=0;
	int filter_wr=0;
	int filter_cnt=0;

	int average_order=1;

	float LastMean = 0;
	float ShortMean;

	float value;

	//float values[ARRAY_SIZE];

	vector <Dimension> Dimensions;

	public:

	float Mean;

	UnicodeString Name;
	UnicodeString Description;

	TMeterValue();
	//virtual  __fastcall ~TMeterValue();

	uint8_t Accuracy=5;
	uint8_t Error=5;   //Допустиммая погрешность в процентах
	int ARRAY_SIZE = 100;

	float MaxValue;
	float MinValue;

	float GetFloatValue();
	float GetFloatValue(uint8_t Dim);
	float GetFloatValue(UnicodeString);



	float FilterApply();
	float AverageApply();

	float GetDoubleValue();
	float GetDoubleValue(UnicodeString);

	float GetDoubleMeanValue(UnicodeString DimName);

	bool IsStable(int lim);

	UnicodeString GetStringValue();
	UnicodeString GetStringValue(uint8_t IntSigns, uint8_t FractSigns);
	UnicodeString GetStringValue(uint8_t IntSigns, uint8_t FractSigns, uint8_t Dim);
	UnicodeString GetStringValue(uint8_t IntSigns, uint8_t FractSigns, UnicodeString Dim);

	UnicodeString GetStringValue(uint8_t Dim);             //Auto Transform
	UnicodeString GetStringValue(UnicodeString Dim);

	UnicodeString GetStringMeanValue(uint8_t Dim);
	UnicodeString GetStringMeanValue();

	UnicodeString GetStringDimensionsValue(uint8_t IntSigns, uint8_t FractSigns);

	void SetFilter(int);
 	void SetValue (double Value);
	void SetValue (float Value);
	void SetValue (float Value, uint8_t Dim);
	void SetValue (float Value, UnicodeString Dimensions);
	void SetValue (UnicodeString Value);

	void SetDimension (UnicodeString Dimensions, float DimRate);
	float GetDimRate (UnicodeString Name);
	float GetDimRate (int Dim);
	UnicodeString GetDimName(int Dim);
	UnicodeString GetDimName(UnicodeString Name);

	 void Reset();

	 void SetAsVolume();
	 void SetAsFlowRate();
	 void SetAsImp();
    void SetAsError() ;

	bool StringCompare(UnicodeString str1 ,UnicodeString str2) ;

	 std::deque <float> values;
	 std::deque <float> AverValues;
	   int temp_delta=3;

		int Short_Mean_index =3; //Порядок фильтра при резком изменении.


};








#endif
