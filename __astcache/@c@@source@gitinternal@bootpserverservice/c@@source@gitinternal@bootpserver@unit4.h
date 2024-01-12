//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Controls.Presentation.hpp>
#include <FMX.Memo.hpp>
#include <FMX.Memo.Types.hpp>
#include <FMX.ScrollBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPClient.hpp>
#include <IdUDPServer.hpp>
#include <IdIPAddress.hpp>
#include <FMX.Edit.hpp>
#include <IdGlobal.hpp>
#include <IdSocketHandle.hpp>
#include <FMX.Edit.hpp>
#include <IdGlobal.hpp>
#include <IdSocketHandle.hpp>
#include <IdGlobal.hpp>
#include <IdSocketHandle.hpp>
#include <FMX.Edit.hpp>
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


class TBootPProxy : public TForm
{
__published:	// IDE-managed Components
	TIdUDPClient *IdUDPClient1;
	TMemo *MemoDebug;
	TButton *BN_test;
	TMemo *Memo3;
	TButton *BN_Quit;
	TIdUDPServer *IdUDPServer1;
	TEdit *TE_IPtoSend;
	TEdit *TE_Gateway;
	TLabel *Label1;
	TLabel *Label2;
	void __fastcall BN_testClick(TObject *Sender);
	void __fastcall BN_QuitClick(TObject *Sender);
	void __fastcall IdUDPServer1UDPRead(TIdUDPListenerThread *AThread, const TIdBytes AData,
          TIdSocketHandle *ABinding);
private:	// User declarations
public:		// User declarations
	__fastcall TBootPProxy(TComponent* Owner);

	void  pme(const char* fmt, ...);
	void HexDump(const TIdBytes packet);
    void SendCannedBootPResponse();
	void SendBootPResponse();

	void ParseBootP(TIdBytes packet);

    struct BootPHdr *bhdr;
	bool b_ParsedAtLeastOneBootPRequest = false;


};
//---------------------------------------------------------------------------
extern PACKAGE TBootPProxy *BootPProxy;
//---------------------------------------------------------------------------
#endif
