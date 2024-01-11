//---------------------------------------------------------------------------
#include "Unit6.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TService6 *Service6;
//---------------------------------------------------------------------------
__fastcall TService6::TService6(TComponent* Owner)
	: TService(Owner)
{
}

void __stdcall ServiceController(unsigned CtrlCode)
{
	Service6->Controller(CtrlCode);
}

TServiceController __fastcall TService6::GetServiceController(void)
{
	return (TServiceController) ServiceController;
}

//---------------------------------------------------------------------------
void __fastcall TService6::IdUDPServer1UDPRead(TIdUDPListenerThread *AThread, const TIdBytes AData,
          TIdSocketHandle *ABinding)
{
//
}
//---------------------------------------------------------------------------
