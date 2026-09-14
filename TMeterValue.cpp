// ---------------------------------------------------------------------------

#pragma hdrstop

#include "TMeterValue.h"
// ---------------------------------------------------------------------------
#pragma package(smart_init)

TMeterValue::TMeterValue() {

}

float TMeterValue::AverageApply() {

}

float TMeterValue::FilterApply() {
	float flt, val, limit, mean = 0;
	int delta, pos;
	bool nulls;

	if (filter_order > 0) {

		if (values.size() > filter_order) {
			delta = filter_order;

		}
		else {
			delta = values.size();

			if (temp_delta > delta) {
				temp_delta = delta;
			}

		}

		if (temp_delta < filter_order) {

			if (temp_delta < delta) {
				delta = temp_delta;
				temp_delta++;
			}
		}
		else {
			// temp_delta = delta;
		}

		nulls = false;

		for (int i = 0; i < delta; i++) {
			val = values[i];
			mean = mean + val;

			if (val == 0) {
				nulls = true;
			}

			if (i == 2) {
				ShortMean = mean / 3;

				if ((LastMean != 0) && (!nulls)) {

					limit = ((ShortMean * 3 * Error) / 100);

					if ((fabs(LastMean - ShortMean)) >
						((ShortMean * 3 * Error) / 100)) {
						delta = 3;
						temp_delta = 3;
					}

					else {

					}
				}
			}

		}

		Mean = mean / delta;
		LastMean = Mean;
		flt = Mean;
	}
	else {
		Mean = flt;
		flt = (float)value;
	}

	AverValues.push_front(Mean);

	if (AverValues.size() > ARRAY_SIZE) {
		AverValues.pop_back();
	}

	return flt;

}





float TMeterValue::GetFloatValue() {
	return value;
}

float TMeterValue::GetDoubleValue() {
	return value;
}

float TMeterValue::GetDoubleValue(UnicodeString DimName) {
	float dbl;

	dbl = value * GetDimRate(DimName);

	return value*GetDimRate(DimName);
}

float TMeterValue::GetDoubleMeanValue(UnicodeString DimName) {
	float dbl;

	dbl = Mean * GetDimRate(DimName);

	return dbl;
}

float TMeterValue::GetFloatValue(uint8_t Dim) {
	return GetFloatValue() * GetDimRate(Dim);
}

float TMeterValue::GetFloatValue(UnicodeString DimName) {
	float flt = GetFloatValue() * GetDimRate(DimName);

	return flt;
}

UnicodeString TMeterValue::GetStringValue(uint8_t IntSigns, uint8_t FractSigns)
{
	return FloatToStrF(GetFloatValue(), ffNumber, IntSigns, FractSigns);
}

UnicodeString TMeterValue::GetStringValue(uint8_t IntSigns, uint8_t FractSigns,
	uint8_t Dim) {
	float flt;
	flt = GetFloatValue() * GetDimRate(Dim);
	return FloatToStrF(flt, ffNumber, IntSigns, FractSigns);

}

UnicodeString TMeterValue::GetStringValue(uint8_t IntSigns, uint8_t FractSigns,
	UnicodeString Dim) {
	float flt;

	flt = GetFloatValue() * GetDimRate(Dim);
	return FloatToStrF(flt, ffNumber, IntSigns, FractSigns);

}

UnicodeString TMeterValue::GetStringDimensionsValue(uint8_t IntSigns,
	uint8_t FractSigns) {
	for (int i = 0; i < sizeof(Dimensions); i++) {
		if (Dimensions[i].Rate == 1) {

			return FloatToStrF(GetFloatValue(), ffNumber, IntSigns, FractSigns)
				+ Dimensions[i].Name;
		}
	}

	return FloatToStrF(GetFloatValue(), ffNumber, IntSigns, FractSigns);
}

void TMeterValue::SetFilter(int i) {
	filter_order = i;
}

void TMeterValue::SetValue(UnicodeString Value) {
	float flt;
	if (TryStrToFloat_(Value, flt)) {
		SetValue(flt);
	}
}



void TMeterValue::SetValue(float Value) {

	if (ARRAY_SIZE > 0) {
		values.push_front(Value);

		if (values.size() > ARRAY_SIZE) {
			values.pop_back();
		}

		if (filter_order != -1) {
			value = FilterApply();
		}
		else {
			value = Value;
		}
	}
	else {
		filter_order = -1;
		value = Value;
	}

}

void TMeterValue::SetValue(float Value, uint8_t Dim) {
	float dbl;

	dbl = Value * Dimensions[Dim].Rate;

	SetValue(dbl);
}

void TMeterValue::SetValue(float Value, UnicodeString Dim) {
	float dbl;

	dbl = Value * GetDimRate(Dim);
	SetValue(dbl);

}

bool TMeterValue::StringCompare(UnicodeString str1, UnicodeString str2) {
	bool success = true;
	for (int i = 0; i < sizeof(str1); i++) {
		if (i < sizeof(str2)) {
			if (str1[i] == str2[i]) {

			}
			else {
				success = false;
				return success;
			}
		}
		else {
			success = false;
			return success;
		}

	}
	return success;
}

float TMeterValue::GetDimRate(UnicodeString Name) {
	UnicodeString DimName;
	for (int i = 0; i < (Dimensions.size()); i++) {
		DimName = Dimensions[i].Name;
		if (Name.CompareIC(DimName) == 0)
			// (StringCompare(Name,DimName))//(Name.CompareIC(DimName)==0)
		{
			return Dimensions[i].Rate;
		}
	}

	return 1;

}

float TMeterValue::GetDimRate(int Dim) {
	if (Dim < sizeof(Dimensions)) {
		return Dimensions[Dim].Rate;
	}

	return 1;
}

UnicodeString TMeterValue::GetDimName(int Dim) {
	if (Dim < sizeof(Dimensions)) {
		return Dimensions[Dim].Name;
	}

	return L" ";
}

UnicodeString TMeterValue::GetDimName(UnicodeString Name) {
	for (int i = 0; i < sizeof(Dimensions); i++) {
		if (Name == Dimensions[i].Name) {
			return Dimensions[i].Name;
		}
	}

	return L" ";
}

void TMeterValue::SetDimension(UnicodeString Name, float Rate) {
	Dimension Dim;
	Dim.Rate = Rate;
	Dim.Name = Name;
	Dimensions.push_back(Dim);
}

void TMeterValue::SetAsVolume() {
	SetDimension(L"λ", 1);
	SetDimension(L"μ3", 0.001);
}

void TMeterValue::SetAsFlowRate() {
	SetDimension(L"λ/χ", 1);
	SetDimension(L"μ3/χ", 0.001);
	SetDimension(L"λ/ρ", 0.000277778);
}

void TMeterValue::SetAsImp() {
	SetDimension(L"θμο", 1);
}

void TMeterValue::SetAsError() {
	SetDimension(L"%", 1);
}

UnicodeString TMeterValue::GetStringValue(uint8_t Dim) {

	float flt = GetFloatValue(Dim);

	// float flt =  GetFloatValue(0);





//
	if (flt < 0.0000001) {
		flt = 0;
	}
	UnicodeString Str = FloatToStr(flt);

	int i = Str.Pos(".");
	int FractPartCnt, IntPartCnt;
	int delta = 0;
	int acr = 5;

	if (i > 0) {
		Str[i] = ',';
	}
	else {
		i = Str.Pos(",");
	}

	if (i == 0) {
		FractPartCnt = 0;
		IntPartCnt = Str.Length();
	}
	else if (i == 1) {
		if (Str[1] == 0) {
			IntPartCnt = 0;
			FractPartCnt = Str.Length() - 2;
		}
	}

	else {
		IntPartCnt = i - 1;
		FractPartCnt = Str.Length() - i;
	}

	if (FractPartCnt > 0) {

		if (IntPartCnt < 5) {
			delta = Accuracy - IntPartCnt;
			if (delta < 0) {
				delta = 0;
			}

		}
		else {
			delta = 0;
		}

		FractPartCnt = delta;
	}

	return FloatToStrF(flt, ffNumber, 10, FractPartCnt);

}

UnicodeString TMeterValue::GetStringMeanValue(uint8_t Dim) {

	float flt = Mean;

	UnicodeString Str = FloatToStr(flt);

	int i = Str.Pos(L".");
	int FractPartCnt, IntPartCnt;
	int delta;
	int acr = 5;

	if (i > 0) {
		Str[i] = ',';
	}
	else {
		i = Str.Pos(L",");
	}

	if (i == 0) {
		FractPartCnt = 0;
		IntPartCnt = Str.Length();
	}
	else if (i == 1) {
		if (Str[1] == 0) {
			IntPartCnt = 0;
			FractPartCnt = Str.Length() - 2;
		}
	}

	else {
		IntPartCnt = i - 1;
		FractPartCnt = Str.Length() - i;
	}

	if (FractPartCnt > 0) {

		if (IntPartCnt < 5) {
			delta = acr - IntPartCnt;
			if (delta < 0) {
				delta = 0;
			}
		}
		else {
			delta = 0;
		}

	}

	return GetStringValue(10, FractPartCnt, Dim);

}

UnicodeString TMeterValue::GetStringMeanValue() {

	float flt = Mean;
	if (flt < 0.01) {
		flt = 0;
	}
	UnicodeString Str = FloatToStr(flt);

	int i = Str.Pos(".");
	int FractPartCnt, IntPartCnt;
	int delta = 0;
	int acr = 5;

	if (i > 0) {
		Str[i] = ',';
	}
	else {
		i = Str.Pos(",");
	}

	if (i == 0) {
		FractPartCnt = 0;
		IntPartCnt = Str.Length();
	}
	else if (i == 1) {
		if (Str[1] == 0) {
			IntPartCnt = 0;
			FractPartCnt = Str.Length() - 2;
		}
	}

	else {
		IntPartCnt = i - 1;
		FractPartCnt = Str.Length() - i;
	}

	if (FractPartCnt > 0) {

		if (IntPartCnt < 5) {
			delta = Accuracy - IntPartCnt;
			if (delta < 0) {
				delta = 0;
			}
		}
		else {
			delta = 0;
		}

		FractPartCnt = delta;
	}

	return FloatToStrF(flt, ffNumber, 10, FractPartCnt);

}

UnicodeString TMeterValue::GetStringValue() {

	float flt = GetFloatValue(0);
	if (flt < 0.01) {
		flt = 0;
	}
	UnicodeString Str = FloatToStr(flt);

	if (Accuracy>0) {

	int i = Str.Pos(".");
	int FractPartCnt, IntPartCnt;
	int delta = 0;
	int acr = 5;

	if (i > 0) {
		Str[i] = ',';
	}
	else {
		i = Str.Pos(",");
	}

	if (i == 0) {
		FractPartCnt = 0;
		IntPartCnt = Str.Length();
	}
	else if (i == 1) {
		if (Str[1] == 0) {
			IntPartCnt = 0;
			FractPartCnt = Str.Length() - 2;
		}
	}

	else {
		IntPartCnt = i - 1;
		FractPartCnt = Str.Length() - i;
	}

	if (FractPartCnt > 0) {

		if (IntPartCnt < 5) {
			delta = Accuracy - IntPartCnt;
			if (delta < 0) {
				delta = 0;
			}
		}
		else {
			delta = 0;
		}

		FractPartCnt = delta;
	}

	return FloatToStrF(flt, ffNumber, 10, FractPartCnt);

	} else
	{
	  return FloatToStrF(flt, ffNumber, 10, 0);
    }


}

void TMeterValue::Reset() {
	filter_rd = 0;
	filter_wr = 0;
	filter_cnt = 0;
	LastMean = 0;

	values.clear();
	AverValues.clear();

}

bool TMeterValue::IsStable(int lim) {

	float flt, limit, val, mean = 0;
	int delta, start_pos = 0, pos;
	bool nulls;

	if (filter_order > 0) {

		if (AverValues.size() > filter_order) {
			delta = filter_order;
		}
		else {
			return false;
		}

		nulls = false;

		limit = ((Mean * lim) / 100);

		for (int i = 0; i < filter_order; i++) {
			val = AverValues[i];
			if ((fabs(Mean - val)) > limit) {
				return false;
			}
		}
	}
	return true;
}
