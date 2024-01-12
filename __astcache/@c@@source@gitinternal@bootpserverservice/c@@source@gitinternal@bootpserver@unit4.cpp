//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TBootPProxy *BootPProxy;
//---------------------------------------------------------------------------
__fastcall TBootPProxy::TBootPProxy(TComponent* Owner)
	: TForm(Owner)
{
	IdUDPServer1->Active =true;

        pme("Sizeof (BootPMagic) : %d",sizeof(BootPMagic));

}
//---------------------------------------------------------------------------


void __fastcall TBootPProxy::HexDump(const TIdBytes packet)
{
	if (Memo3->Lines->Count > 1000) Memo3->Lines->Clear();
	char buf[80];
	String s = "00: ";
	int i=0;
	for (int x=0;x<packet.size();x++)
		{
			sprintf(buf, "%02X ",packet[x]);
			s += buf;
			if (((x+1) % 16)==0) {
				Memo3->Lines->Add(s);
				sprintf(buf, "%02X: ",++i);
				s = buf;
			}
		}
		Memo3->Lines->Add(s);
		Memo3->Lines->Add("---------------------------");
        //scroll to bottom of text
		Memo3->SelStart = Memo3->Lines->Text.Length();
		Memo3->SelLength = 1;
		Memo3->ClearSelection();
}


void  TBootPProxy::pme(const char* fmt, ...)
{
	//Check length and clear after 1000 lines
	if (MemoDebug->Lines->Count > 1000) MemoDebug->Lines->Clear();
	va_list args;
	va_start(args, fmt);
	char buf[200];
	vsprintf(buf,fmt,args);
	MemoDebug->Lines->Add(buf);
	va_end(args);
	//scroll to bottom of text
	MemoDebug->SelStart = MemoDebug->Lines->Text.Length();
	MemoDebug->SelLength = 1;
	MemoDebug->ClearSelection();
}

	char msg[310] =
	"\x02\x01\x06\x00\x00\x00\x00\x00\x00\x00\x00\x00\x18\x01\x10\x07"
	"\x18\x01\x01\x07\x18\x01\x01\x6f\x00\x00\x00\x00\x00\x80\x8e\x00"
	"\x19\x3b\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x44\x45\x53\x4b"
	"\x54\x4f\x50\x2d\x4c\x34\x43\x4f\x44\x33\x4d\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x63\x82\x53\x63"
	"\x01\x04\xff\x00\x00\x00\x1c\x04\xff\xff\xff\xff\xff\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
	"\x00\x00\x00\x09\x00\x00\x00\x00\x00\x00\x00\x00";


void __fastcall TBootPProxy::BN_testClick(TObject *Sender)
{
	pme("test Pushed");
	SendBootPResponse();



}
//---------------------------------------------------------------------------

void __fastcall TBootPProxy::SendCannedBootPResponse()
{
	TIdBytes mb = RawToBytes(msg,300);
	//HexDump(mb);
	const char *Host = "255.255.255.255";
	int Port = 68;
	TIdUDPClient *IdUDPClient = new TIdUDPClient(NULL);
	IdUDPClient->ReuseSocket = rsTrue;
	IdUDPClient->BroadcastEnabled = true;
	IdUDPClient->SendBuffer(Host,Port,mb);
}

void __fastcall TBootPProxy::BN_QuitClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TBootPProxy::IdUDPServer1UDPRead(TIdUDPListenerThread *AThread, const TIdBytes AData,
          TIdSocketHandle *ABinding)
{
	pme("Udp read packet of size:%d",AData.size());
	HexDump(AData);
	ParseBootP(AData);
	SendBootPResponse();
}
//---------------------------------------------------------------------------


void __fastcall TBootPProxy::ParseBootP(TIdBytes packet)
{
	pme("In bootP Parse");

	pme("Sizeof struct is %d",sizeof(BootPHdr));
	pme("Sizeof unsigned int %d",sizeof(unsigned int));

	pme("Sizeof unsigned short %d",sizeof(unsigned short));

		pme("Sizeof BootPMagic %d",sizeof(BootPMagic));


	//byte *p = packet.data();

	bhdr = (struct BootPHdr*)  packet.data();
	const char *OP_CODE[] = {"","Boot Request","Boot Reply"};
	pme("Op Code: %02X which is %s", bhdr->OpCode, OP_CODE[bhdr->OpCode]);
	const char *HARDWARE_TYPE[] = {"","Ethernet","Boot Reply"};
	pme("HardwareType: %02X which is %s", bhdr->HardwareType, OP_CODE[bhdr->HardwareType]);
	pme("Hardware Address Length: %02X", bhdr->HardwareAddressLength);
	pme("Hops : %02X", bhdr->Hops);

    pme("Transaction ID : %02X", bhdr->TransactionID);
	pme("Seconds Elapsed : %02X", bhdr->SecondsElapsed);
	pme("Flags : %02X", bhdr->Flags);
	if ((bhdr->Flags & 0b1000000000000000)==0) {
        pme("Broadcast Flag: Unicast");
	}

	pme("Client IP Address %d.%d.%d.%d",bhdr->ClientIP[0],bhdr->ClientIP[1],bhdr->ClientIP[2],bhdr->ClientIP[3]);
	pme("Your IP Address %d.%d.%d.%d",bhdr->YourIP[0],bhdr->YourIP[1],bhdr->YourIP[2],bhdr->YourIP[3]);
	pme("Server IP Address %d.%d.%d.%d",bhdr->ServerIP[0],bhdr->ServerIP[1],bhdr->ServerIP[2],bhdr->ServerIP[3]);
	pme("Gateway IP Address %d.%d.%d.%d",bhdr->GatewayIP[0],bhdr->GatewayIP[1],bhdr->GatewayIP[2],bhdr->GatewayIP[3]);

    pme("Client MAC Address %02X:%02X:%02X:%02X:%02X:%02X:",bhdr->CHAddr[0],bhdr->CHAddr[1],bhdr->CHAddr[2],bhdr->CHAddr[3],bhdr->CHAddr[4],bhdr->CHAddr[5]);

	pme("Server Name%s:",bhdr->CHAddr[0],bhdr->ServerName);

    b_ParsedAtLeastOneBootPRequest = true;

}

void __fastcall TBootPProxy::SendBootPResponse()
{
	pme("sending constructed response");

	//instantiate and zerize the BootP response packet
	struct BootPHdr response;
	char * p = (char *) &response;
	for (int i=0 ; i < sizeof(BootPHdr); i++) {
		p[i]=0;
	}


	response.OpCode = 2;
	response.HardwareType= 0x01;
	response.HardwareAddressLength =6;
	response.Hops = 0;
	response.TransactionID = 0;
	response.SecondsElapsed = 0;
	response.Flags = 0;


	if (b_ParsedAtLeastOneBootPRequest == true) {
		response.ClientIP[0]=bhdr->ClientIP[0];
		response.ClientIP[1]=bhdr->ClientIP[1];
		response.ClientIP[2]=bhdr->ClientIP[2];
		response.ClientIP[3]=bhdr->ClientIP[3];

		response.CHAddr[0]=bhdr->CHAddr[0];
		response.CHAddr[1]=bhdr->CHAddr[1];
		response.CHAddr[2]=bhdr->CHAddr[2];
		response.CHAddr[3]=bhdr->CHAddr[3];
		response.CHAddr[4]=bhdr->CHAddr[4];
		response.CHAddr[5]=bhdr->CHAddr[5];
	}
	else
	{
		response.ClientIP[0]=24;
		response.ClientIP[1]=1;
		response.ClientIP[2]=16;
		response.ClientIP[3]=7;

		response.CHAddr[0]=0x00;
		response.CHAddr[1]=0x80;
		response.CHAddr[2]=0x8E;
		response.CHAddr[3]=0x00;
		response.CHAddr[4]=0x19;
		response.CHAddr[5]=0x3B;
	}


	const wchar_t *str = TE_IPtoSend->Text.c_str()	;
	char buffer[32];
	int ret = wcstombs ( buffer, str, sizeof(buffer) );
	struct sockaddr_in sa;
	inet_pton(AF_INET, buffer, &(sa.sin_addr));

    response.YourIP[0]= sa.sin_addr.S_un.S_un_b.s_b1;
	response.YourIP[1]= sa.sin_addr.S_un.S_un_b.s_b2;
	response.YourIP[2]= sa.sin_addr.S_un.S_un_b.s_b3;
	response.YourIP[3]= sa.sin_addr.S_un.S_un_b.s_b4;

	response.ServerIP[0]=24;
	response.ServerIP[1]=1;
	response.ServerIP[2]=1;
	response.ServerIP[3]=111;

	str = TE_Gateway->Text.c_str()	;
	ret = wcstombs ( buffer, str, sizeof(buffer) );
	inet_pton(AF_INET, buffer, &(sa.sin_addr));

	response.GatewayIP[0]= sa.sin_addr.S_un.S_un_b.s_b1;
	response.GatewayIP[1]= sa.sin_addr.S_un.S_un_b.s_b2;
	response.GatewayIP[2]= sa.sin_addr.S_un.S_un_b.s_b3;
	response.GatewayIP[3]= sa.sin_addr.S_un.S_un_b.s_b4;


	//response.GatewayIP[0]=24;
	//response.GatewayIP[1]=1;
	//response.GatewayIP[2]=1;
	//response.GatewayIP[3]=1;



	//response.ServerName[64];
	//response.BootFileName[128];
	//response.VendorSpecificArea;


	response.bpm.MagicCookie[0]=0x63;
	response.bpm.MagicCookie[1]=0x82;
	response.bpm.MagicCookie[2]=0x53;
	response.bpm.MagicCookie[3]=0x63;


	response.bpm._MagicOption1.OptionNum=1;
	response.bpm._MagicOption1.Length =4;
	response.bpm._MagicOption1.SubnetMask[0]=255;
	response.bpm._MagicOption1.SubnetMask[1]=0;
	response.bpm._MagicOption1.SubnetMask[2]=0;
	response.bpm._MagicOption1.SubnetMask[3]=0;

	response.bpm._MagicOption28.OptionNum=1;
	response.bpm._MagicOption28.Length =4;
	response.bpm._MagicOption28.SubnetMask[0]=255;
	response.bpm._MagicOption28.SubnetMask[1]=255;
	response.bpm._MagicOption28.SubnetMask[2]=255;
	response.bpm._MagicOption28.SubnetMask[3]=255;
    response.bpm.Option255 = 255;




	TIdBytes mb2 = RawToBytes(p,300);

	//HexDump(mb);
	const char *Host = "255.255.255.255";
	int Port = 68;
	TIdUDPClient *IdUDPClient = new TIdUDPClient(NULL);
	IdUDPClient->ReuseSocket = rsTrue;
	IdUDPClient->BroadcastEnabled = true;
	IdUDPClient->SendBuffer(Host,Port,mb2);
}
