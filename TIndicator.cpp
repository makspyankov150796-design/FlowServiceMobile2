//---------------------------------------------------------------------------

#include <fmx.h>

#pragma hdrstop

#include "TIndicator.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TIndicator *)
{
	new TIndicator(NULL);
}
//---------------------------------------------------------------------------
__fastcall TIndicator::TIndicator(TComponent* Owner)
	: TCircle(Owner)
{
}
//---------------------------------------------------------------------------
namespace Tindicator
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TIndicator)};
		RegisterComponents(L"Samples", classes, 0);
	}
}
//---------------------------------------------------------------------------
