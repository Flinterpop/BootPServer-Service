﻿// CodeGear C++Builder
// Copyright (c) 1995, 2022 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'IdIPAddress.pas' rev: 35.00 (Windows)

#ifndef IdipaddressHPP
#define IdipaddressHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>
#include <SysInit.hpp>
#include <System.Classes.hpp>
#include <IdGlobal.hpp>

//-- user supplied -----------------------------------------------------------

namespace Idipaddress
{
//-- forward type declarations -----------------------------------------------
class DELPHICLASS TIdIPAddress;
//-- type declarations -------------------------------------------------------
#ifndef _WIN64
#pragma pack(push,4)
#endif /* not _WIN64 */
class PASCALIMPLEMENTATION TIdIPAddress : public System::TObject
{
	typedef System::TObject inherited;
	
protected:
	unsigned FIPv4;
	Idglobal::TIdIPVersion FAddrType;
	System::UnicodeString __fastcall GetIPv4AsString();
	System::UnicodeString __fastcall GetIPv6AsString();
	System::UnicodeString __fastcall GetIPAddress();
	
public:
	Idglobal::TIdIPv6Address IPv6;
	__fastcall virtual TIdIPAddress();
	__classmethod TIdIPAddress* __fastcall MakeAddressObject(const System::UnicodeString AIP)/* overload */;
	__classmethod TIdIPAddress* __fastcall MakeAddressObject(const System::UnicodeString AIP, const Idglobal::TIdIPVersion AIPVersion)/* overload */;
	int __fastcall CompareAddress(const System::UnicodeString AIP, bool &VErr);
	Idglobal::TIdBytes __fastcall HToNBytes();
	__property unsigned IPv4 = {read=FIPv4, write=FIPv4, nodefault};
	__property System::UnicodeString IPv4AsString = {read=GetIPv4AsString};
	__property System::UnicodeString IPv6AsString = {read=GetIPv6AsString};
	__property Idglobal::TIdIPVersion AddrType = {read=FAddrType, write=FAddrType, nodefault};
	__property System::UnicodeString IPAsString = {read=GetIPAddress};
public:
	/* TObject.Destroy */ inline __fastcall virtual ~TIdIPAddress() { }
	
};

#ifndef _WIN64
#pragma pack(pop)
#endif /* not _WIN64 */

//-- var, const, procedure ---------------------------------------------------
}	/* namespace Idipaddress */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_IDIPADDRESS)
using namespace Idipaddress;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// IdipaddressHPP