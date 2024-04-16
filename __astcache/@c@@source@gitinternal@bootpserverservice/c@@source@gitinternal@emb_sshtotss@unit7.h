//---------------------------------------------------------------------------

#ifndef Unit7H
#define Unit7H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>

#include <CkSsh.h>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdIcmpClient.hpp>
#include <IdRawBase.hpp>
#include <IdRawClient.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <vector>



struct PeerStatusWord
{
	int PSW;
	//

    //fields of PSW:
	int PSW_Status;
	bool config;
	bool authenb;
	bool auth;
	bool b_reach;
	bool bcst;
	int PSW_Select;
	int PSW_Count;
	int PSW_Event;

	const char *Select[8] = {"sel_reject","sel_falsetick","sel_excess","sel_outlier","sel_candidate","sel_backup","sel_sys.peer","sel_pps.peer"};
	const char *Event[16] = {"no zero","mobilize","demobilize","unreachable","reachable","restart","no_reply","rate_exceeded","access_denied","leap_armed","sys_peer","clock_event","bad_auth","popcorn","interleave_mode","interleave_error"};
};



struct SystemStatusWord {
	int SSW;      // system status word

	int LeapField;
	int Source;
	int Count;
	int Event;

	const char *leap[4] = {"leap_none","leap_add_sec","leap_del_sec","leap_alarm"};
	const char *source[10] = {"sync_unspec","sync_pps","sync_lf_radio","sync_hf_radio","sync_uhf_radio","sync_local","sync_ntp","sync_other","sync_wristwatch","sync_telephone"};
	const char *event[16] = {"unspecified","freq_not_set","freq_set","spike_detect","freq_mode","clock_sync","restart","panic_stop","no_system_peer","leap_armed","leap_disarmed",
	"leap_event","clock_step","kern","TAI...","stale leapsecond values"};
};


struct SystemPeerInfo {
	int SSW;      // system status word

    char tally;

	std::string PeerIP; //pmeF("Peer IP: %s",pnLine2Tokens[0].c_str());
	std::string Stratum;//	pmeF("Stratum : %s",pnLine2Tokens[2].c_str());
	int stratum;
	std::string Type;//pmeF("Type: %s",pnLine2Tokens[3].c_str());
	char type;
	std::string TimeSinceLast; //pmeF("Time Since last Pck Rx: %s secs",pnLine2Tokens[4].c_str());
	int timeSinceLast;
	int reach;

    std::string reachAsBits;

	std::string PollPeriod; //pmeF("Poll period: %s (s)",pnLine2Tokens[5].c_str());
	int pollPeriod;

	float delay;
	float offset;
	float jitter;



};




//---------------------------------------------------------------------------
class TForm7 : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *RE_Debug;
	TButton *BN_SSR;
	TButton *BN_TSS;
	TButton *BN_CTMA;
	TButton *BN_SaveLog;
	TButton *BN_Last100;
	TButton *BN_Quit;
	TButton *BN_Reboot;
	TGroupBox *GroupBox1;
	TCheckBox *CheckBox1;
	TButton *BN_RUN;
	TComboBox *CB_CommandList;
	TButton *BN_Clear;
	TButton *BN_NTP_Status;
	TButton *BN_NTP_Restart;
	TEdit *TE_as_Status;
	TEdit *TE_rv_Status;
	TLabel *Label1;
	TLabel *Label2;
	TIdIcmpClient *ICMP;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *TE_pn_Reach;
	TEdit *TE_ReachBITS;
	TShape *LITE_NTP;
	TCheckBox *CB_Debug;
	TLabel *LB_as_StatusDetails;
	TShape *LITE_rvStatus;
	TLabel *LB_rv_StatusDetails;
	TShape *LITE_asStatus;
	TShape *LITE_NTP_Ping;
	TTimer *Timer1;
	TShape *Lite_0;
	TPanel *Lite_Panel;
	TShape *Lite_1;
	TShape *Lite_2;
	TShape *Lite_3;
	TShape *Lite_4;
	TShape *Lite_5;
	TShape *Lite_6;
	TShape *Lite_7;
	void __fastcall BN_QuitClick(TObject *Sender);
	void __fastcall BN_RUNClick(TObject *Sender);
	void __fastcall BN_ClearClick(TObject *Sender);
	void __fastcall BN_SSRClick(TObject *Sender);
	void __fastcall BN_TSSClick(TObject *Sender);
	void __fastcall BN_CTMAClick(TObject *Sender);
	void __fastcall BN_Last100Click(TObject *Sender);
	void __fastcall BN_RebootClick(TObject *Sender);
	void __fastcall BN_NTP_StatusClick(TObject *Sender);
	void __fastcall BN_NTP_RestartClick(TObject *Sender);
	void __fastcall ICMPReply(TComponent *ASender, TReplyStatus * const AReplyStatus);
	void __fastcall Timer1Timer(TObject *Sender);


private:	// User declarations
public:		// User declarations


	struct keywordPair {
		std::string keyword;
		TColor color;
	};


	__fastcall TForm7(TComponent* Owner);

	void  pmeF(const char* fmt, ...);
	void  pme(const char* fmt);
	void ChilkatSample(void);
	void ChilkatUnlock(void);
	bool Login(CkSsh *ssh);
	void PrintWithSyntax(const char *inBuf, std::vector<struct keywordPair> *KPs);
    void PrintWithLineSyntax(const char *inBuf, std::vector<struct keywordPair> *KPs);
	void RunCommand(char *cmd, std::vector<struct keywordPair> *KPs = 0);
	void RunCommandLine(char *cmd, std::vector<struct keywordPair> *KPs);
	void GetPeerStatusWord(struct PeerStatusWord *psw);
	void GetSystemStatusWord(struct SystemStatusWord *SSW);       \
	void GetPeerInfo(struct SystemPeerInfo *PeerInfo);
    void GetNTPStatus();



	const char *RunCommand2(char *cmd);

    bool NTP_Pingable = false;

    bool g_debug=true;



	std::vector<struct keywordPair> status_keywords = { {"active",clBlue} , {"exited", clPurple}, {"running", clGreen}, {"auto-restart", clRed}, {"activiating", clRed} };
	std::vector<struct keywordPair> TSS_Conf_keywords = { {"hostif_address",clBlue} , {"hostif_netmask", clBlue}, {"hostif_gateway", clBlue}, {"mids_platform", clGreen},
	 {"etr_active", clRed} ,{"ntp_server", clRed},{"fom18_pass_through", clGreen},{"mids_type", clGreen},{"loghost", clLime},{"logport", clLime}};

	std::vector<struct keywordPair> CTMA_keywords = { {"<entry key=\"transmissionRateDelayWeight\">",clBlue}};


};
//---------------------------------------------------------------------------
extern PACKAGE TForm7 *Form7;
//---------------------------------------------------------------------------
#endif
