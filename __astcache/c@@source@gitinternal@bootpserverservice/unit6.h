//---------------------------------------------------------------------------
#ifndef Unit6H
#define Unit6H
//---------------------------------------------------------------------------
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.SvcMgr.hpp>
#include <vcl.h>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdGlobal.hpp>
#include <IdSocketHandle.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPServer.hpp>
#include <IdUDPClient.hpp>
//---------------------------------------------------------------------------


	struct MagicOption
	{
		 byte OptionNum;
		 byte Length;
		 byte SubnetMask[4];
	};

	struct BootPMagic {
		byte MagicCookie[4];
		struct MagicOption _MagicOption1;
		struct MagicOption _MagicOption28;
		byte Option255;
        byte Padding[47];
	};


	struct BootPHdr {
		byte OpCode;
		byte HardwareType;
		byte HardwareAddressLength;
		byte Hops;

		unsigned int TransactionID;
		unsigned short SecondsElapsed;
		unsigned short Flags;
		byte ClientIP[4];
		byte YourIP[4];
		byte ServerIP[4];
		byte GatewayIP[4];
		byte CHAddr[16];

		char ServerName[64];
		char BootFileName[128];


		struct BootPMagic bpm;
		//byte VendorSpecificArea[64];



	};



class TBootP : public TService
{
__published:    // IDE-managed Components
	TIdUDPServer *IdUDPServer1;
	void __fastcall IdUDPServer1UDPRead(TIdUDPListenerThread *AThread, const TIdBytes AData,
          TIdSocketHandle *ABinding);
	void __fastcall ServiceStop(TService *Sender, bool &Stopped);
	void __fastcall ServiceAfterInstall(TService *Sender);
private:        // User declarations
public:         // User declarations
	__fastcall TBootP(TComponent* Owner);
	TServiceController __fastcall GetServiceController(void);
	void ParseBootP(TIdBytes packet);
    void SendBootPResponse();
    void  pme(const char* fmt, ...);

	friend void __stdcall ServiceController(unsigned CtrlCode);


    struct BootPHdr *bhdr;
	bool b_ParsedAtLeastOneBootPRequest = false;


};
//---------------------------------------------------------------------------
extern PACKAGE TBootP *BootP;
//---------------------------------------------------------------------------
#endif
