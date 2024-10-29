//----------------------------------------------------
// file: Log.h
//----------------------------------------------------

#ifndef ELogH
#define ELogH
enum TMsgDlgType
{
	mtCustom =0,
	mtError =1,
	mtInformation = 2,
	mtConfirmation = 4,
	mtSkip = 8,

};
enum TMsgDlgButtons
{
	mbYes=1,
	mbNo=2,
	mbCancel=4,
	mbOK = 8,
	mbSkip = 16,
	mrNone = 0,
	mrYes,
	mrNo,
	mrCancel,
	mrOK,
	mrSkip,
};
class ECORE_API CLog{
public:
	bool 		in_use;
public:
				CLog	(){in_use=false;}
	void 		Msg   	(TMsgDlgType mt, LPCSTR _Format, ...);
	int 		DlgMsg 	(TMsgDlgType mt, LPCSTR _Format, ...);
	int 		DlgMsg 	(TMsgDlgType mt, int btn, LPCSTR _Format, ...);
	void Close();
};

inline void Log(const char*Text,const char*Text2){ Msg("%s %s",Text,Text2); }

void ECORE_API ELogCallback(LPCSTR txt);

extern ECORE_API CLog ELog;

#endif /*_INCDEF_NETDEVICELOG_H_*/

