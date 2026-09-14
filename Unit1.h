//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.ListView.Adapters.Base.hpp>
#include <FMX.ListView.Appearances.hpp>
#include <FMX.ListView.hpp>
#include <FMX.ListView.Types.hpp>
#include <FMX.Types.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.StdCtrls.hpp>
#include <System.JSON.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.EngExt.hpp>
#include <Data.Bind.ObjectScope.hpp>
#include <Fmx.Bind.DBEngExt.hpp>
#include <Fmx.Bind.Editors.hpp>
#include <FMX.Edit.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.ScrollBox.hpp>
#include <REST.Client.hpp>
#include <REST.Response.Adapter.hpp>
#include <REST.Types.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <System.Net.HttpClient.hpp>
#include <System.Net.HttpClientComponent.hpp>
#include <System.Net.URLClient.hpp>

#include "TFlowMeter.h"




//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TStyleBook *StyleBook1;
	TRESTClient *RESTClient1;
	TRESTRequest *RESTRequest1;
	TRESTResponse *RESTResponse1;
	TRESTResponseDataSetAdapter *RESTResponseDataSetAdapter1;
	TBindingsList *BindingsList1;
	TMemo *MemoContent;
	TLinkControlToField *LinkControlToFieldContent;
	TPanel *Panel1;
	TEdit *EditParamspattern;
	TLabel *LabelParamspattern;
	TLinkControlToField *LinkControlToFieldParamspattern;
	TMemo *Memo1;
	TButton *Button1;
	TButton *Button2;
	TMemo *Memo2;
	TNetHTTPClient *NetHTTPClient1;
	TNetHTTPRequest *NetHTTPRequest1;
	TButton *Button3;
	TButton *Button4;
	TButton *Button5;
	TMemo *Memo3;
	void __fastcall SpeedButton2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall NetHTTPRequest1RequestCompleted(TObject * const Sender, IHTTPResponse * const AResponse);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
