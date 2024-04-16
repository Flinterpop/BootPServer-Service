//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit7.h"

#include <CkGlobal.h>
#include <CkSsh.h>
#include <CkStringBuilder.h>
#include <regex>
#include <vector>
#include <sstream>
#include <bitset>



//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm7 *Form7;
//---------------------------------------------------------------------------
__fastcall TForm7::TForm7(TComponent* Owner)
	: TForm(Owner)
{
	ChilkatUnlock();
    GetNTPStatus();
}
//---------------------------------------------------------------------------

int LicenseState=0;
CkGlobal glob;

void TForm7::ChilkatUnlock(void)
	{
	// The Chilkat API can be unlocked for a fully-functional 30-day trial by passing any
	// string to the UnlockBundle method.  A program can unlock once at the start. Once unlocked,
	// all subsequently instantiated objects are created in the unlocked state.
	//
	// After licensing Chilkat, replace the "Anything for 30-day trial" with the purchased unlock code.
	// To verify the purchased unlock code was recognized, examine the contents of the LastErrorText
	// property after unlocking.  For example:
	//CkGlobal glob;
	bool success = glob.UnlockBundle("RABDFX.CB1022025_F3DD8X5hkG0r");
	if (success != true) {
		LicenseState = 1;
		//pme(glob.lastErrorText());
		return;
	}

	int status = glob.get_UnlockStatus();
	if (status == 2) {
		LicenseState = 2;
		//pme("Unlocked using purchased unlock code.");
	}
	else {
	    LicenseState = 3;
		//pme("Unlocked in trial mode.");
    }

    // The LastErrorText can be examined in the success case to see if it was unlocked in
	// trial more, or with a purchased unlock code.
	//pme(glob.lastErrorText());

		RE_Debug->Lines->Add(glob.lastErrorText());
    }


bool TForm7::Login(CkSsh *ssh)
{

	int port = 22;
	bool success = ssh->Connect("192.168.1.22",port);
	if (success != true) {
		pme(ssh->lastErrorText());
		return success;
	}

	// Authenticate using login/password:
	success = ssh->AuthenticatePw("tadmin","peanuts");
	if (success != true) {
		//pme(ssh.lastErrorText());
		RE_Debug->Lines->Add(glob.lastErrorText());
		return success;
	}
	return success; //should be true

}






//---------------------------------------------------------------------------
void __fastcall TForm7::BN_QuitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------



void __fastcall TForm7::BN_ClearClick(TObject *Sender)
{
    RE_Debug->Lines->Clear();
}
//---------------------------------------------------------------------------




void TForm7::RunCommand(char *cmd, std::vector<struct keywordPair> *KPs)
{
	CkSsh ssh;
	if (false == Login(&ssh)) {
		pme("Login failed");
		return;
	}

	pmeF("Running> %s\n",cmd);

	const char *strOutput = ssh.quickCommand(cmd ,"ansi");
	if (ssh.get_LastMethodSuccess() != true) {
		pme(ssh.lastErrorText());
		return;
	}

	if (0 == KPs) pme(strOutput);
	else PrintWithSyntax(strOutput, KPs);
}



void TForm7::RunCommandLine(char *cmd, std::vector<struct keywordPair> *KPs)
{
	CkSsh ssh;
	if (false == Login(&ssh)) {
		pme("Login failed");
		return;
	}

	pmeF("Running> %s\n",cmd);

	const char *strOutput = ssh.quickCommand(cmd	,"ansi");
	if (ssh.get_LastMethodSuccess() != true) {
		pme(ssh.lastErrorText());
		return;
	}

	if (0 == KPs) pme(strOutput);
	else PrintWithLineSyntax(strOutput, KPs);
}



std::vector<std::string> splitter(std::string in_pattern, std::string& content) {
	std::vector<std::string> split_content;

	std::regex pattern(in_pattern);
	copy( std::sregex_token_iterator(content.begin(), content.end(), pattern, -1),
	std::sregex_token_iterator(),back_inserter(split_content));
	return split_content;
}


void TForm7::PrintWithSyntax(const char *inBuf, std::vector<struct keywordPair> *KPs)
{
	std::vector<int> *v = new std::vector<int>;
	std::string sentence = inBuf;

	std::vector<std::string> lines = splitter(R"(\n)", sentence);     //split into lines that end in /n

	for (std::string line: lines){
			std::vector<std::string> tokens = splitter(R"( )", line);  //split into tokens that are separated by spaces
			for (std::string token: tokens) { //For each token...
				RE_Debug->SelAttributes->Color = clBlack;

				for (int i = 0; i < KPs->size(); i++) {
					if (token == KPs->at(i).keyword) RE_Debug->SelAttributes->Color = KPs->at(i).color;
				}
				RE_Debug->SelText = token.c_str();
				RE_Debug->SelText = " ";
			}
			RE_Debug->SelText = "\n";
	}
}


void TForm7::PrintWithLineSyntax(const char *inBuf, std::vector<struct keywordPair> *KPs)
{
	std::vector<int> *v = new std::vector<int>;
	std::string sentence = inBuf;

	std::vector<std::string> lines = splitter(R"(\n)", sentence);      //split into lines

	for (std::string line: lines){
		RE_Debug->SelAttributes->Color = clBlack;
		for (int i = 0; i < KPs->size(); i++) {
			//check for keyword match at beginning of line
			if (!strncmp(line.c_str(), KPs->at(i).keyword.c_str(), KPs->at(i).keyword.size())) RE_Debug->SelAttributes->Color = KPs->at(i).color;
		}

		RE_Debug->SelText = line.c_str();
		RE_Debug->SelText = "\n";
	}
	//RE_Debug->SelText = "\n";
}



/////////////////////////////////////////

//this is a printf for the Debug Memo widget
void  TForm7::pmeF(const char* fmt, ...)
{
	if (RE_Debug->Lines->Count > 1000) RE_Debug->Lines->Clear();

	va_list args;
	va_start(args, fmt);
	char buf[200];
	vsprintf(buf,fmt,args);

	RE_Debug->Lines->Add(buf);
	va_end(args);
	//RE_Debug->Lines->Add("\n");
}

void  TForm7::pme(const char* fmt)
{
	if (RE_Debug->Lines->Count > 1000) RE_Debug->Lines->Clear();
	RE_Debug->SelText = fmt;
	//RE_Debug->Lines->Add("\n");
}



////////////////////////////////////////

void __fastcall TForm7::BN_RUNClick(TObject *Sender)
{
	RawByteString s = UTF8Encode(CB_CommandList->Text);
	RunCommand(s.c_str());
}
//---------------------------------------------------------------------------


void __fastcall TForm7::BN_SSRClick(TObject *Sender)
{
	RE_Debug->Lines->Clear();
	char s[] = "systemctl list-units xds*";
	RunCommand(s, &status_keywords);
}
//---------------------------------------------------------------------------


void __fastcall TForm7::BN_TSSClick(TObject *Sender)
{
	RE_Debug->Lines->Clear();
	char s[] = "cat /lib/live/mount/config_medium/config/tss.conf*";
	RunCommandLine(s,&TSS_Conf_keywords);
}
//---------------------------------------------------------------------------

void __fastcall TForm7::BN_CTMAClick(TObject *Sender)
{
	RE_Debug->Lines->Clear();
	char s[] = "cat /lib/live/mount/config_medium/config/ctma.xml*";
	RunCommandLine(s,&CTMA_keywords);
}
//---------------------------------------------------------------------------


void __fastcall TForm7::BN_Last100Click(TObject *Sender)
{
    RE_Debug->Lines->Clear();
	char s[] = "echo peanuts | sudo -S tail -n 100 /var/log/xds.log";
	RunCommand(s, &status_keywords);
}
//---------------------------------------------------------------------------

void __fastcall TForm7::BN_RebootClick(TObject *Sender)
{
	char s[] = "echo peanuts | sudo -S reboot";
	RunCommand(s);
}
//---------------------------------------------------------------------------

void __fastcall TForm7::BN_NTP_RestartClick(TObject *Sender)
{
	char s[] = "echo peanuts | sudo -S systemctl restart ntp.service";
	RunCommand(s);
   	pme("\nDone...\n");
}
//---------------------------------------------------------------------------


CkSsh ssh;

const char *TForm7::RunCommand2(char *cmd)
{
	//CkSsh ssh;
	if (false == Login(&ssh)) {
		pme("Login failed");
		return 0;
	}

	pmeF("Running> %s\n",cmd);

	const char *strOutput = ssh.quickCommand(cmd	,"ansi");
	if (ssh.get_LastMethodSuccess() != true) {
		pme(ssh.lastErrorText());
		return 0;
	}

	return strOutput;
}






void TForm7::GetPeerStatusWord(struct PeerStatusWord *PSW)
{
	//ntpq -c as => NTP Associations
	const char *retVal = RunCommand2((char*)"ntpq -c as");
	if (retVal == 0){
		pme("Error in ntpq -c as");
		return;
	}

	if (CB_Debug->Checked) pme(retVal);

	std::string s = retVal;
	std::vector<std::string> aslines = splitter(R"(\n)", s);
	std::istringstream asLine2(aslines[2]);
	std::vector<std::string> asLine2Tokens(std::istream_iterator<std::string>{asLine2},std::istream_iterator<std::string>());
	std::string asPeerStatusWord = asLine2Tokens[2];

	// Peer Status Word
	//Status (0-4), Select (5-7), Count (8-11) and Code (12-15).
	int asStatus  = std::stoi(asPeerStatusWord, 0, 16);

	PSW->PSW = asStatus;
	PSW->PSW_Status = (asStatus & 0xF800);
	PSW->config = (asStatus & 0x8000);
	PSW->authenb = (asStatus & 0x4000);
	PSW->auth = (asStatus & 0x2000);
	PSW->b_reach = (asStatus & 0x1000);
	PSW->bcst = (asStatus & 0x0800);

	PSW->PSW_Select = (asStatus & 0x0700)>>8;

	PSW->PSW_Count = (asStatus & 0x00F0)>>4;
	PSW->PSW_Event = (asStatus & 0x000F);

    TE_as_Status->Text = asPeerStatusWord.c_str();


	std::string details="";
	if (PSW->bcst) details += "bcast, ";
	if (PSW->auth) details += "auth, ";
	if (PSW->config) details += "config, ";
	if (PSW->b_reach) details += "reach, ";
	details += PSW->Select[PSW->PSW_Select];


	if ( (CB_Debug->Checked)) {
		pmeF("bcst: %s",PSW->bcst?"T":"F");
		pmeF("reach: %s",PSW->b_reach?"T":"F");
		pmeF("auth: %s",PSW->auth?"T":"F");
		pmeF("authenb: %s",PSW->authenb?"T":"F");
		pmeF("config: %s",PSW->config?"T":"F");
		pmeF("Select is %d which is %s",PSW->PSW_Select, PSW->Select[PSW->PSW_Select]);
		pmeF("Last event is %d which is %s",PSW->PSW_Event, PSW->Event[PSW->PSW_Event]);
		pmeF("Count is %d",PSW->PSW_Count);
	}

	LB_as_StatusDetails->Caption = details.c_str();

	if (PSW->config && 	PSW->b_reach) {
		if (PSW->PSW_Select==6) { //6 is sys_sel.peer
			LITE_asStatus->Brush->Color =clLime;
		}
		else LITE_asStatus->Brush->Color =clYellow;
	}
	else LITE_asStatus->Brush->Color = clRed;

	if (PSW->PSW_Select==0)  // 0 is reject
		LITE_asStatus->Brush->Color = clRed;
}




void TForm7::GetSystemStatusWord(struct SystemStatusWord *SSW)
{

	// ntpq -c rv => NTP System Variables
	const char *retVal = RunCommand2((char*)"ntpq -c rv");
	if (retVal == 0){
		pme("Error in ntpq -c rv");
		return;
	}
	if (CB_Debug->Checked) pme(retVal);

	std::string s_rv = retVal;

	// An object of regex for pattern to be searched
	std::regex r("status=[0-9A-Fa-f]+");

	// flag type for determining the matching behavior
	// here it is for matches on 'string' objects
	std::smatch m;

	// regex_search() for searching the regex pattern
	// 'r' in the string 's_rv'. 'm' is flag for determining matching behavior.
	regex_search(s_rv, m, r);

	SSW->SSW = 0;

	//int rvStatus = 0;      // system status word
	// for each loop
	for (auto x : m)
	{
		std::cout << x << " ";
		std::string an = x;
		if (CB_Debug->Checked) pmeF("Match: %s\n",an.c_str());
		std::string an2=an.substr(7,4);

		TE_rv_Status->Text = an2.c_str();
		SSW->SSW = std::stoi(an2, 0, 16);
	}

	if (SSW->SSW<=0 ) {
			pme("Get SystemStatusWord Failed");
			return;
	}

	std::string details ="";

	SSW->LeapField = (SSW->SSW & 0xC000) >>14;
	details += SSW->leap[SSW->LeapField];
	details += ", ";

	SSW->Source = (SSW->SSW & 0x3F00) >>8;
	details += SSW->source[SSW->Source];
	details += ", ";

	SSW->Count = (SSW->SSW & 0x00F0) >>4;
	SSW->Event = (SSW->SSW & 0x000F);
	details += SSW->event[SSW->Event];

	if (CB_Debug->Checked)
	{
		pmeF("LeapField: %d which is %s",SSW->LeapField, SSW->leap[SSW->LeapField]);
		pmeF("Source: %d which is %s",SSW->Source, SSW->source[SSW->Source]);
		pmeF("Last Event: %d which is %s",SSW->Event, SSW->event[SSW->Event]);
	}

	if (SSW->LeapField == 0) {
		LITE_rvStatus->Brush->Color = clLime;
		if (SSW->Event != 5) { //clock_sync
			LITE_rvStatus->Brush->Color = clRed;
		}
	}
	else
	    LITE_rvStatus->Brush->Color = clRed;

	LB_rv_StatusDetails->Caption=details.c_str();
}




void TForm7::GetPeerInfo(struct SystemPeerInfo *PeerInfo)
{

	// ntpq -pn PEER Info =======================================================
	const char *retVal = RunCommand2((char*)"ntpq -pn");
	if (retVal == 0){
		pme("Error in ntpq -pn");
		return;
	}
	if (CB_Debug->Checked) pme(retVal);

	std::string s_pn = retVal;

	std::vector<std::string> pnlines = splitter(R"(\n)", s_pn);
	//pmeF("Num Lines: %d",pnlines.size());

	std::istringstream pnLine2(pnlines[2]);
	std::vector<std::string> pnLine2Tokens(std::istream_iterator<std::string>{pnLine2},std::istream_iterator<std::string>());



	PeerInfo->PeerIP = pnLine2Tokens[0];
	PeerInfo->stratum = std::stoi(pnLine2Tokens[2].c_str());

	PeerInfo->type = pnLine2Tokens[3].c_str()[0];
	PeerInfo->timeSinceLast = std::stoi(pnLine2Tokens[4].c_str());
	PeerInfo->pollPeriod = std::stoi(pnLine2Tokens[5].c_str());

	std::string pnReach = pnLine2Tokens[6];
	PeerInfo->reach  = std::stoi(pnReach, 0, 8);

	PeerInfo->reachAsBits = std::bitset<8>(PeerInfo->reach).to_string();


	PeerInfo->delay = std::atof(pnLine2Tokens[7].c_str());
	PeerInfo->offset = std::atof(pnLine2Tokens[8].c_str());
	PeerInfo->jitter = std::atof(pnLine2Tokens[9].c_str());


	if (CB_Debug->Checked)
	{
		pmeF("Peer IP: %s",pnLine2Tokens[0].c_str());
		pmeF("Stratum : %s",pnLine2Tokens[2].c_str());
		pmeF("Type: %s",pnLine2Tokens[3].c_str());
		pmeF("Time Since last Pck Rx: %s secs",pnLine2Tokens[4].c_str());
		pmeF("Poll period: %s (s)",pnLine2Tokens[5].c_str());

		pmeF("Reach is %s",pnReach.c_str());
		pmeF("Reach is %s ",PeerInfo->reachAsBits.c_str());

		pmeF("Delay (network RTT): %s",pnLine2Tokens[7].c_str());
		pmeF("Offset: %s (ms)",pnLine2Tokens[8].c_str());
		pmeF("Jitter: %s (ms?)",pnLine2Tokens[9].c_str());

	}


	((PeerInfo->reach & 0x01)!=0)?Lite_0->Brush->Color=clLime:Lite_0->Brush->Color=clRed;
	((PeerInfo->reach & 0x02)!=0)?Lite_1->Brush->Color=clLime:Lite_1->Brush->Color=clRed;
	((PeerInfo->reach & 0x04)!=0)?Lite_2->Brush->Color=clLime:Lite_2->Brush->Color=clRed;
	((PeerInfo->reach & 0x08)!=0)?Lite_3->Brush->Color=clLime:Lite_3->Brush->Color=clRed;
	((PeerInfo->reach & 0x10)!=0)?Lite_4->Brush->Color=clLime:Lite_4->Brush->Color=clRed;
	((PeerInfo->reach & 0x20)!=0)?Lite_5->Brush->Color=clLime:Lite_5->Brush->Color=clRed;
	((PeerInfo->reach & 0x40)!=0)?Lite_6->Brush->Color=clLime:Lite_6->Brush->Color=clRed;
	((PeerInfo->reach & 0x80)!=0)?Lite_7->Brush->Color=clLime:Lite_7->Brush->Color=clRed;


	TE_pn_Reach->Text = pnReach.c_str();
	TE_ReachBITS->Text = PeerInfo->reachAsBits.c_str();


	if ('*' == pnLine2Tokens[0][0]) {
		if (CB_Debug->Checked) pme("All Good - Tally indicates System Peer");
		if ((PeerInfo->reach & 0x01)!=0) LITE_NTP->Brush->Color = clLime;
		else LITE_NTP->Brush->Color = clYellow;
	}
	else
	    LITE_NTP->Brush->Color = clRed;

	//int when = std::stoi(pnLine2Tokens[4].c_str());
	//int pollPeriod = std::stoi(pnLine2Tokens[5].c_str());

}

void TForm7::GetNTPStatus()
{

	struct PeerStatusWord PSW;
	GetPeerStatusWord(&PSW);

	struct SystemStatusWord SSW;
	GetSystemStatusWord(&SSW);

	struct SystemPeerInfo PeerInfo;
	GetPeerInfo(&PeerInfo);

	pmeF("ntpq -c as Status is %04X",PSW.PSW);
	pmeF("ntpq -c rv Status is %04X",SSW.SSW);

	pmeF("ntpq -pn Reach is %s",PeerInfo.reachAsBits.c_str());



	//ping the NTP from the PC (not TSS)
	for (int i=0; i < 2; i++) {
			try {
				ICMP->Ping();
				Sleep(1000);
				Application->ProcessMessages();

			} catch (...) {
				pme("Ping Failed");
                LITE_NTP_Ping->Brush->Color = clRed;
			}
	}
}


void __fastcall TForm7::BN_NTP_StatusClick(TObject *Sender)
{
	GetNTPStatus();
}
//---------------------------------------------------------------------------

void __fastcall TForm7::ICMPReply(TComponent *ASender, TReplyStatus * const AReplyStatus)
{

	if (CB_Debug->Checked) {
		UnicodeString Result;
		Result = "Received " + UnicodeString(AReplyStatus->BytesReceived) +
		" bytes from " + AReplyStatus->FromIpAddress +
		", time =  " + UnicodeString(AReplyStatus->MsRoundTripTime) +
		" ms, ttl " + UnicodeString((int)AReplyStatus->TimeToLive);

		RE_Debug->Lines->Add(Result.c_str());
	}

	if (AReplyStatus->MsRoundTripTime < 100) {
		NTP_Pingable = true;
		LITE_NTP_Ping->Brush->Color =clLime;
		std::string we = std::to_string(AReplyStatus->MsRoundTripTime);
		we = we + " ms";
	}
	else LITE_NTP_Ping->Brush->Color =clRed;


}
//---------------------------------------------------------------------------


void __fastcall TForm7::Timer1Timer(TObject *Sender)
{
	GetNTPStatus();
}
//---------------------------------------------------------------------------

