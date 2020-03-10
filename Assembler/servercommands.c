#include "poweroff.h"

#define SET_ACTION(a) { ps->action=a; DisplaySettings(hWnd,ps); ReturnClientOK(hWnd,ps); }
#define SET_WHO(w) { ps->who=w; DisplaySettings(hWnd,ps); ReturnClientOK(hWnd,ps); }
#define SET_WHEN(w) { ps->when=w; DisplaySettings(hWnd,ps); ReturnClientOK(hWnd,ps); }
#define SET_SCHEDULE(s) { ps->schedule.schedule=s; ReturnClientOK(hWnd,ps); }
#define SET_BOOLEAN(p,b) \
  { \
    if (value==NULL) \
      ReturnClientError(hWnd,ps,"No value specified"); \
    else if (!_stricmp(value,"on")) \
      { p=1; DisplaySettings(hWnd,ps); ReturnClientOK(hWnd,ps); } \
    else if (!_stricmp(value,"off")) \
      { p=0; DisplaySettings(hWnd,ps); ReturnClientOK(hWnd,ps); } \
    else \
      ReturnClientError(hWnd,ps,"Invalid value specified"); \
  }
#define ADD_DAY(d) { if (str[0]) { strcat(str,","); strcat(str,d);} else strcpy(str,d); }
#define CHECK_IF_AUTHENTICATED { if (!ps->authenticated) { ReturnClientError(hWnd,ps,"Access is denied"); CloseClientSocket(hWnd,ps); return; } }

char *Trim(char *str)
{
  int x;
  
  while (str[0]==' ' || str[0]=='\t' || str[0]=='\n') /* trim spaces at the front */
    memmove(&str[0],&str[1],strlen(str));
  for (x=strlen(str)-1;x>=0 && (str[x]==' ' || str[x]=='\t' || str[x]=='\n');x--) /* trim spaces at the end */
    str[x]=0;
  return str;
}

int ReplaceString(char *str,char *srch,char *repl)
{
  size_t x;
  int replaced=0;
  
  for (x=0;x<strlen(str);x++)
  {
    if (x>strlen(str)-strlen(srch))
    {
      break;
    }
    if (memcmp(&str[x],srch,strlen(srch))==0)
    {
      memmove(&str[x],&str[x+strlen(srch)],strlen(str)-x-strlen(srch)+1);
      memmove(&str[x+strlen(repl)],&str[x],strlen(str)-x+1);
      memcpy(&str[x],repl,strlen(repl));
      x+=strlen(repl)-1;
      replaced=1;
    }
  }
  return replaced;
}

void ReturnClientError(HWND hWnd,PowerSettings *ps, char *error)
{
  char str[500];

  Log("ReturnClientError start");
  if (ps->clientsocket!=INVALID_SOCKET)
  {
    sprintf(str,"%d %s\r\n",COMMAND_ERROR,error);
    Log("Sending error %s to client",str);
    if (send(ps->clientsocket,str,strlen(str),0)==SOCKET_ERROR)
    {
      ShowSocketError(ps,"send");
      CloseClientSocket(hWnd,ps);
      return;
    }
  }
  Log("ReturnClientError end");
}

void ReturnClientInfo(HWND hWnd,PowerSettings *ps, char *info, ...)
{
  va_list param;
  char tmpstr[500],str[500];

  Log("ReturnClientInfo start");
  va_start(param,info);
  vsprintf(tmpstr,info,param);
  va_end(param);

  sprintf(str,"%d %s\r\n",COMMAND_INFO,tmpstr);
  Log("sending info %s to client",str);
  if (send(ps->clientsocket,str,strlen(str),0)==SOCKET_ERROR)
  {
    ShowSocketError(ps,"send");
    CloseClientSocket(hWnd,ps);
    return;
  }
  Log("ReturnClientInfo end");
}

void ReturnClientOK(HWND hWnd,PowerSettings *ps)
{
  char str[10];

  Log("ReturnClientOK start");
  sprintf(str,"%d OK\r\n",COMMAND_OK);
  Log("sending ok %s to client",str);
  if (send(ps->clientsocket,str,strlen(str),0)==SOCKET_ERROR)
  {
    ShowSocketError(ps,"send");
    CloseClientSocket(hWnd,ps);
    return;
  }
  Log("ReturnClientOK end");
}

void ShowClientHelp(HWND hWnd,PowerSettings *ps)
{
  Log("ShowClientHelp start");
  ReturnClientInfo(hWnd,ps,"You can use the following commands to control Poweroff");
  ReturnClientInfo(hWnd,ps,"QUIT\tTerminates the connection");
  ReturnClientInfo(hWnd,ps,"HELP\tShows this help");
  ReturnClientInfo(hWnd,ps,"PASSWORD p\tAuthenticates the client with a password");
  ReturnClientInfo(hWnd,ps,"ACTION a\tThe action to perform (logoff,reboot,shutdown,poweroff,standby,hibernate,lock,wol,monitor_off,monitor_on,no_action)");
  ReturnClientInfo(hWnd,ps,"WHO w\tWho must perform the action (local,remote)");
  ReturnClientInfo(hWnd,ps,"WHEN w\tWhen must the action be performed (immediate,scheduled,after_process)");
  ReturnClientInfo(hWnd,ps,"SIMULATE s\tSimulate the action (on,off)");
  ReturnClientInfo(hWnd,ps,"COMPUTER c\tThe remote computer name");
  ReturnClientInfo(hWnd,ps,"MAC m\tThe remote computer mac address (WOL)");
  ReturnClientInfo(hWnd,ps,"IP i\tThe remote computer ip address (WOL)");
  ReturnClientInfo(hWnd,ps,"SUBNET s\tThe remote computer subnet mask (WOL)");
  ReturnClientInfo(hWnd,ps,"WARNING w\tShow warning before doing the action (on,off)");
  ReturnClientInfo(hWnd,ps,"WARNTIME w\tThe number of seconds to show the warning");
  ReturnClientInfo(hWnd,ps,"MESSAGE m\tThe warning message");
  ReturnClientInfo(hWnd,ps,"PLAYSOUND p\tPlay a sound when showing the warning (on,off)");
  ReturnClientInfo(hWnd,ps,"SOUNDFILE s\tThe WAV file to play");
  ReturnClientInfo(hWnd,ps,"RUNPRG r\tRuns a program before doing the action (on,off)");
  ReturnClientInfo(hWnd,ps,"PROGRAM p\tThe program to run");
  ReturnClientInfo(hWnd,ps,"DIRECTORY d\tThe startup directory for the program");
  ReturnClientInfo(hWnd,ps,"SCHEDULE s\tThe type of schedule (fixed_day,daily,day_of_month,after_x_seconds)");
  ReturnClientInfo(hWnd,ps,"TIME t\tThe time when to do the action (hh:mm)");
  ReturnClientInfo(hWnd,ps,"DATE d\tThe date when to do the action (dd/mm/yyyy)");
  ReturnClientInfo(hWnd,ps,"DAYS d\tThe days of the week when to do the action (m,t,w,th,f,s,su)");
  ReturnClientInfo(hWnd,ps,"WEEKS d\tThe weeks of the month when to do the action (1,2,3,4,5)");
  ReturnClientInfo(hWnd,ps,"DAY d\tThe day of the month when to do the action (1-31)");
  ReturnClientInfo(hWnd,ps,"WAIT w\tThe number of seconds to wait before checking the time");
  ReturnClientInfo(hWnd,ps,"SECONDS w\tThe number of seconds to wait before doing the action");
  ReturnClientInfo(hWnd,ps,"PROCESS p\tWait for process p to finish");
  ReturnClientInfo(hWnd,ps,"FORCE f\tForce applications to close (on,off)");
  ReturnClientInfo(hWnd,ps,"CANCEL c\tAllow the user to cancel the action (on,off)");
  ReturnClientInfo(hWnd,ps,"SHOW\tShow current settings");
  ReturnClientInfo(hWnd,ps,"VERSION\tShow current version of poweroff");
  ReturnClientInfo(hWnd,ps,"DOIT\tDo the action");
  ReturnClientInfo(hWnd,ps,"STOP\tStop the current scheduled action");
  Log("ShowClientHelp end");
}

void ShowSettingsToClient(HWND hWnd,PowerSettings *ps)
{
  char str[500];

  Log("ShowSettingsToClient start");
  ReturnClientInfo(hWnd,ps,"These settings are currently in effect");
  switch (ps->action)
  {
    case LOGOFF:
      ReturnClientInfo(hWnd,ps,"ACTION\tLOGOFF");
      break;
    case REBOOT:
      ReturnClientInfo(hWnd,ps,"ACTION\tREBOOT");
      break;
    case SHUTDOWN:
      ReturnClientInfo(hWnd,ps,"ACTION\tSHUTDOWN");
      break;
    case POWEROFF:
      ReturnClientInfo(hWnd,ps,"ACTION\tPOWEROFF");
      break;
    case STANDBY:
      ReturnClientInfo(hWnd,ps,"ACTION\tSTANDBY");
      break;
    case HIBERNATE:
      ReturnClientInfo(hWnd,ps,"ACTION\tHIBERNATE");
      break;
    case LOCK:
      ReturnClientInfo(hWnd,ps,"ACTION\tLOCK");
      break;
    case WAKE_ON_LAN:
      ReturnClientInfo(hWnd,ps,"ACTION\tWOL");
      break;
    case MONITOR_OFF:
      ReturnClientInfo(hWnd,ps,"ACTION\tMONITOR_OFF");
      break;
    case MONITOR_ON:
      ReturnClientInfo(hWnd,ps,"ACTION\tMONITOR_ON");
      break;
    case NO_ACTION:
      ReturnClientInfo(hWnd,ps,"ACTION\tNO_ACTION");
      break;
    default:
      ReturnClientInfo(hWnd,ps,"ACTION\tUNKNOWN");
      break;
  }
  switch (ps->who)
  {
    case LOCAL_COMPUTER:
      ReturnClientInfo(hWnd,ps,"WHO\t\tLOCAL");
      break;
    case REMOTE_COMPUTER:
      ReturnClientInfo(hWnd,ps,"WHO\t\tREMOTE");
      break;
    default:
      ReturnClientInfo(hWnd,ps,"WHO\t\tUNKNOWN");
      break;
  }
  switch (ps->when)
  {
    case IMMEDIATE:
      ReturnClientInfo(hWnd,ps,"WHEN\tIMMEDIATE");
      break;
    case SCHEDULED:
      ReturnClientInfo(hWnd,ps,"WHEN\tSCHEDULED");
      break;
    case PROCESS:
      ReturnClientInfo(hWnd,ps,"WHEN\tAFTER_PROCESS");
      break;
    default:
      ReturnClientInfo(hWnd,ps,"WHEN\tUNKNOWN");
      break;
  }
  ReturnClientInfo(hWnd,ps,"SIMULATE\t%s",ps->simulate?"ON":"OFF");
  ReturnClientInfo(hWnd,ps,"COMPUTER\t%s",ps->remote.computer_name);
  ReturnClientInfo(hWnd,ps,"MAC\t\t%s",ps->remote.mac_address);
  ReturnClientInfo(hWnd,ps,"IP\t\t%s",ps->remote.ip_address);
  ReturnClientInfo(hWnd,ps,"SUBNET\t%s",ps->remote.subnet_mask);
  ReturnClientInfo(hWnd,ps,"WARNING\t%s",ps->options.warning?"ON":"OFF");
  ReturnClientInfo(hWnd,ps,"WARNTIME\t%d",ps->warning.seconds);
  strcpy(str,ps->warning.message);
  ReplaceString(str,"\r\n","\\n");
  ReturnClientInfo(hWnd,ps,"MESSAGE\t%s",str);
  ReturnClientInfo(hWnd,ps,"PLAYSOUND\t%s",ps->warning.play_sound?"ON":"OFF");
  ReturnClientInfo(hWnd,ps,"SOUNDFILE\t%s",ps->warning.sound_file);
  ReturnClientInfo(hWnd,ps,"RUNPRG\t%s",ps->options.run_program?"ON":"OFF");
  ReturnClientInfo(hWnd,ps,"PROGRAM\t%s",ps->program.program);
  ReturnClientInfo(hWnd,ps,"DIRECTORY\t%s",ps->program.directory);
  switch (ps->schedule.schedule)
  {
    case FIXED_DAY:
      ReturnClientInfo(hWnd,ps,"SCHEDULE\tFIXED_DAY");
      break;
    case DAILY:
      ReturnClientInfo(hWnd,ps,"SCHEDULE\tDAILY");
      break;
    case DAY_OF_MONTH:
      ReturnClientInfo(hWnd,ps,"SCHEDULE\tDAY_OF_MONTH");
      break;
    case AFTER_X_SECONDS:
      ReturnClientInfo(hWnd,ps,"SCHEDULE\tAFTER_X_SECONDS");
      break;
    default:
      ReturnClientInfo(hWnd,ps,"SCHEDULE\tUNKNOWN");
      break;
  }
  ReturnClientInfo(hWnd,ps,"TIME\t%02d:%02d",ps->schedule.time.wHour,ps->schedule.time.wMinute);
  ReturnClientInfo(hWnd,ps,"DATE\t%02d/%02d/%04d",ps->schedule.date.wDay,ps->schedule.date.wMonth,ps->schedule.date.wYear);
  str[0]='\0';
  if (ps->schedule.monday)
    ADD_DAY("m")
  if (ps->schedule.tuesday)
    ADD_DAY("t")
  if (ps->schedule.wednesday)
    ADD_DAY("w")
  if (ps->schedule.thursday)
    ADD_DAY("th")
  if (ps->schedule.friday)
    ADD_DAY("f")
  if (ps->schedule.saturday)
    ADD_DAY("s")
  if (ps->schedule.sunday)
    ADD_DAY("su")
  ReturnClientInfo(hWnd,ps,"DAYS\t%s",str);
  str[0]='\0';
  if (ps->schedule.week1)
    ADD_DAY("1")
  if (ps->schedule.week2)
    ADD_DAY("2")
  if (ps->schedule.week3)
    ADD_DAY("3")
  if (ps->schedule.week4)
    ADD_DAY("4")
  if (ps->schedule.week5)
    ADD_DAY("5")
  ReturnClientInfo(hWnd,ps,"WEEKS\t%s",str);
  ReturnClientInfo(hWnd,ps,"DAY\t\t%d",ps->schedule.day);
  ReturnClientInfo(hWnd,ps,"WAIT\t%d",ps->schedule.wait);
  ReturnClientInfo(hWnd,ps,"SECONDS\t%d",ps->schedule.seconds);
  ReturnClientInfo(hWnd,ps,"PROCESS\t%s",ps->process.process);
  ReturnClientInfo(hWnd,ps,"FORCE\t%s",ps->options.force?"ON":"OFF");
  ReturnClientInfo(hWnd,ps,"CANCEL\t%s",ps->options.allow_cancel?"ON":"OFF");
  Log("ShowSettingsToClient end");
}

void ProcessClientCommand(HWND hWnd,PowerSettings *ps,char *command)
{
  char *value;

  Log("ProcessClientCommand start, command=%s",command);
  if (strlen(command)>0 && command[strlen(command)-1]=='\n')
    command[strlen(command)-1]='\0';
  value=strchr(command,' ');
  if (value)
  {
    *value='\0';
    value++;
    Trim(value);
  }
  Log("command=%s, value=%s",command,value);
  if (!_stricmp(command,"quit"))
  {
    ReturnClientInfo(hWnd,ps,"Bye bye");
    CloseClientSocket(hWnd,ps);
  }
  else if (!_stricmp(command,"help"))
    ShowClientHelp(hWnd,ps);
  else if (!_stricmp(command,"password"))
  {
    if (value==NULL)
    {
      if (ps->remote_control.password[0]!='\0')
        ReturnClientError(hWnd,ps,"Invalid password");
      else
      {
        ps->authenticated=1;
        ReturnClientOK(hWnd,ps);
      }
    }
    else if (strcmp(EncodePassword(value),ps->remote_control.password))
      ReturnClientError(hWnd,ps,"Invalid password");
    else
    {
      ps->authenticated=1;
      ReturnClientOK(hWnd,ps);
    }
  }
  else if (!_stricmp(command,"action"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else if (!_stricmp(value,"logoff"))
      SET_ACTION(LOGOFF)
    else if (!_stricmp(value,"reboot"))
      SET_ACTION(REBOOT)
    else if (!_stricmp(value,"shutdown"))
      SET_ACTION(SHUTDOWN)
    else if (!_stricmp(value,"poweroff"))
      SET_ACTION(POWEROFF)
    else if (!_stricmp(value,"standby"))
      SET_ACTION(STANDBY)
    else if (!_stricmp(value,"hibernate"))
      SET_ACTION(HIBERNATE)
    else if (!_stricmp(value,"lock"))
      SET_ACTION(LOCK)
    else if (!_stricmp(value,"wol"))
      SET_ACTION(WAKE_ON_LAN)
    else if (!_stricmp(value,"monitor_off"))
      SET_ACTION(MONITOR_OFF)
    else if (!_stricmp(value,"monitor_on"))
      SET_ACTION(MONITOR_ON)
    else if (!_stricmp(value,"no_action"))
      SET_ACTION(NO_ACTION)
    else
      ReturnClientError(hWnd,ps,"Invalid value specified");
  }
  else if (!_stricmp(command,"who"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else if (!_stricmp(value,"local"))
      SET_WHO(LOCAL_COMPUTER)
    else if (!_stricmp(value,"remote"))
      SET_WHO(REMOTE_COMPUTER)
    else
      ReturnClientError(hWnd,ps,"Invalid value specified");
  }
  else if (!_stricmp(command,"when"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else if (!_stricmp(value,"immediate"))
      SET_WHEN(IMMEDIATE)
    else if (!_stricmp(value,"scheduled"))
      SET_WHEN(SCHEDULED)
    else if (!_stricmp(value,"after_process"))
      SET_WHEN(PROCESS)
    else
      ReturnClientError(hWnd,ps,"Invalid value specified");
  }
  else if (!_stricmp(command,"simulate"))
  {
    CHECK_IF_AUTHENTICATED;
    SET_BOOLEAN(ps->simulate,value);
  }
  else if (!_stricmp(command,"computer"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      strcpy(ps->remote.computer_name,value);
      ReturnClientOK(hWnd,ps);
    }
  }
  else if (!_stricmp(command,"mac"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      if (IsValidMACAddress(value))
      {
        strcpy(ps->remote.mac_address,value);
        ReturnClientOK(hWnd,ps);
      }
      else
        ReturnClientError(hWnd,ps,"Invalid MAC address");
    }
  }
  else if (!_stricmp(command,"ip"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      if (IsValidIPAddress(value))
      {
        strcpy(ps->remote.ip_address,value);
        ReturnClientOK(hWnd,ps);
      }
      else
        ReturnClientError(hWnd,ps,"Invalid IP address");
    }
  }
  else if (!_stricmp(command,"subnet"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      if (IsValidIPAddress(value))
      {
        strcpy(ps->remote.subnet_mask,value);
        ReturnClientOK(hWnd,ps);
      }
      else
        ReturnClientError(hWnd,ps,"Invalid subnet mask");
    }
  }
  else if (!_stricmp(command,"warning"))
  {
    CHECK_IF_AUTHENTICATED;
    SET_BOOLEAN(ps->options.warning,value);
  }
  else if (!_stricmp(command,"warntime"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      ps->warning.seconds=atoi(value);
      ReturnClientOK(hWnd,ps);
    }
  }
  else if (!_stricmp(command,"message"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ps->warning.message[0]='\0';
    else
    {
      strcpy(ps->warning.message,value);
      ReplaceString(ps->warning.message,"\\n","\r\n");
    }
    ReturnClientOK(hWnd,ps);
  }
  else if (!_stricmp(command,"playsound"))
  {
    CHECK_IF_AUTHENTICATED;
    SET_BOOLEAN(ps->warning.play_sound,value);
  }
  else if (!_stricmp(command,"soundfile"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      strcpy(ps->warning.sound_file,value);
      ReturnClientOK(hWnd,ps);
    }
  }
  else if (!_stricmp(command,"runprg"))
  {
    CHECK_IF_AUTHENTICATED;
    SET_BOOLEAN(ps->options.run_program,value);
  }
  else if (!_stricmp(command,"program"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      strcpy(ps->program.program,value);
      ReturnClientOK(hWnd,ps);
    }
  }
  else if (!_stricmp(command,"directory"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ps->program.directory[0]='\0';
    else
      strcpy(ps->program.directory,value);
    ReturnClientOK(hWnd,ps);
  }
  else if (!_stricmp(command,"schedule"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else if (!_stricmp(value,"fixed_day"))
      SET_SCHEDULE(FIXED_DAY)
    else if (!_stricmp(value,"daily"))
      SET_SCHEDULE(DAILY)
    else if (!_stricmp(value,"day_of_month"))
      SET_SCHEDULE(DAY_OF_MONTH)
    else if (!_stricmp(value,"after_x_seconds"))
      SET_SCHEDULE(AFTER_X_SECONDS)
    else
      ReturnClientError(hWnd,ps,"Invalid value specified");
  }
  else if (!_stricmp(command,"time"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      int hours,minutes;

      if (sscanf(value,"%d:%d",&hours,&minutes)!=2 || hours<0 || hours>23 || minutes<0 || minutes>59)
        ReturnClientError(hWnd,ps,"Invalid time specified");
      else
      {
        GetLocalTime(&ps->schedule.time);
        ps->schedule.time.wHour=hours;
        ps->schedule.time.wMinute=minutes;
        if (ps->schedule.date.wYear==0)
          GetLocalTime(&ps->schedule.date);
        ReturnClientOK(hWnd,ps);
      }
    }
  }
  else if (!_stricmp(command,"date"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      int days,months,years;

      if (sscanf(value,"%d/%d/%d",&days,&months,&years)!=3 || days<0 || days>31 || months<0 || months>12 || years<0 || years>9999)
        ReturnClientError(hWnd,ps,"Invalid date specified");
      else
      {
        GetLocalTime(&ps->schedule.date);
        ps->schedule.date.wDay=days;
        ps->schedule.date.wMonth=months;
        ps->schedule.date.wYear=years;
        ps->schedule.schedule=FIXED_DAY;
        ReturnClientOK(hWnd,ps);
      }
    }
  }
  else if (!_stricmp(command,"days"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      char days[10][10];
      int n,y,error=0;

      ps->schedule.monday=0;
      ps->schedule.tuesday=0;
      ps->schedule.wednesday=0;
      ps->schedule.thursday=0;
      ps->schedule.friday=0;
      ps->schedule.saturday=0;
      ps->schedule.sunday=0;
      n=sscanf(value,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",days[0],days[1],days[2],days[3],days[4],days[5],days[6]);
      for (y=0;y<n;y++)
      {
        if (!strcmp(days[y],"m"))
          ps->schedule.monday=1;
        else if (!strcmp(days[y],"t"))
          ps->schedule.tuesday=1;
        else if (!strcmp(days[y],"w"))
          ps->schedule.wednesday=1;
        else if (!strcmp(days[y],"th"))
          ps->schedule.thursday=1;
        else if (!strcmp(days[y],"f"))
          ps->schedule.friday=1;
        else if (!strcmp(days[y],"s"))
          ps->schedule.saturday=1;
        else if (!strcmp(days[y],"su"))
          ps->schedule.sunday=1;
        else
          error=1;
      }
      if (error)
        ReturnClientError(hWnd,ps,"Invalid day specified");
      else
        ReturnClientOK(hWnd,ps);
    }
  }
  else if (!_stricmp(command,"weeks"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      char days[10][10];
      int n,y,error=0;

      ps->schedule.week1=0;
      ps->schedule.week2=0;
      ps->schedule.week3=0;
      ps->schedule.week4=0;
      ps->schedule.week5=0;
      n=sscanf(value,"%[^,],%[^,],%[^,],%[^,],%s",days[0],days[1],days[2],days[3],days[4]);
      for (y=0;y<n;y++)
      {
        if (!strcmp(days[y],"1"))
          ps->schedule.week1=1;
        else if (!strcmp(days[y],"2"))
          ps->schedule.week2=1;
        else if (!strcmp(days[y],"3"))
          ps->schedule.week3=1;
        else if (!strcmp(days[y],"4"))
          ps->schedule.week4=1;
        else if (!strcmp(days[y],"5"))
          ps->schedule.week5=1;
        else
          error=1;
      }
      if (error)
        ReturnClientError(hWnd,ps,"Invalid week specified");
      else
        ReturnClientOK(hWnd,ps);
    }
  }
  else if (!_stricmp(command,"day"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      ps->schedule.day=atoi(value);
      ReturnClientOK(hWnd,ps);
    }
  }
  else if (!_stricmp(command,"seconds"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      ps->schedule.seconds=atoi(value);
      ReturnClientOK(hWnd,ps);
    }
  }
  else if (!_stricmp(command,"wait"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      ps->schedule.wait=atoi(value);
      ReturnClientOK(hWnd,ps);
    }
  }
  else if (!_stricmp(command,"process"))
  {
    CHECK_IF_AUTHENTICATED;
    if (value==NULL)
      ReturnClientError(hWnd,ps,"No value specified");
    else
    {
      strcpy(ps->process.process,value);
      ReturnClientOK(hWnd,ps);
    }
  }
  else if (!_stricmp(command,"force"))
  {
    CHECK_IF_AUTHENTICATED;
    SET_BOOLEAN(ps->options.force,value);
  }
  else if (!_stricmp(command,"cancel"))
  {
    CHECK_IF_AUTHENTICATED;
    SET_BOOLEAN(ps->options.allow_cancel,value);
  }
  else if (!_stricmp(command,"show"))
  {
    CHECK_IF_AUTHENTICATED;
    ShowSettingsToClient(hWnd,ps);
  }
  else if (!_stricmp(command,"version"))
  {
    ReturnClientInfo(hWnd,ps,"%s",POWEROFF_VERSION);
  }
  else if (!_stricmp(command,"doit"))
  {
    char str[500];

    CHECK_IF_AUTHENTICATED;
    if (!SanityCheck(hWnd,ps,str))
    {
      ReturnClientOK(hWnd,ps);
      DoIt(hWnd,ps);
    }
    else
      ReturnClientError(hWnd,ps,str);
  }
  else if (!_stricmp(command,"stop"))
  {
    if (ps->active_timer)
    {
      KillTimer(hWnd,ps->active_timer);
      ps->active_timer=0;
      ReturnClientOK(hWnd,ps);
    }
    else
      ReturnClientError(hWnd,ps,"No active command");
  }
  else
  {
    ReturnClientError(hWnd,ps,"Invalid command");
  }
  Log("ProcessClientCommand end");
}

