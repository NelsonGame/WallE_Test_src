
typedef struct testcommands 
{
	char cmdname[20]  ;
	char cmdsyntax[50] ;
} testcommands ;

testcommands testfwcmds[] =
{ 
	{"GPS TEST", "$AMT,GPS,1*00\r\n"},
	{"SENSORS TEST", "$AMT,SENSOR,100*00\r\n"},
	{"SOUNDER TEST", "$AMT,OBJECT,SOUNDER,TEST*00"},
	{"PIMIC TEST", "$AMT,OBJECT,POWER*00"},
	{"MFI TEST", "$AMT,OBJECT,MFI*00"}
} ;


