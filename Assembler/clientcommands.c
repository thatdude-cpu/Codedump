#include "poweroff.h"

int SendClientCommand(HWND hWnd,PowerSettings *ps,char *errorstr,char *command,...)
{
  va_list param;
  char tmpstr[500],str[500];
  char *value;
  int ret;

  Log("SendClientCommand start");
  va_start(param,command);
  vsprintf(tmpstr,command,param);
  va_end(param);
  sprintf(str,"%s\n",tmpstr);

  if (ps->clientsocket!=INVALID_SOCKET)
  {
    Log("Sending %s to remote machine",str);
    if (send(ps->clientsocket,str,strlen(str),0)==SOCKET_ERROR)
    {
      CloseClientSocket(hWnd,ps);
      return -1;
    }
    Log("Waiting for a reply");
    if ((ret=recv(ps->clientsocket,str,500,0))==SOCKET_ERROR || ret<=0)
    {
      CloseClientSocket(hWnd,ps);
      return -1;
    }
    Log("Received %s from remote machine",str);
    str[ret]=0;
    while (strlen(str)>0 && (str[strlen(str)-1]=='\n' || str[strlen(str)-1]=='\r'))
      str[strlen(str)-1]='\0';
    value=strchr(str,' ');
    if (value)
    {
      *value='\0';
      value++;
      Trim(value);
    }
    if (str[0]=='2')
    {
      Log("SendClientCommand end");
      return 0;
    }
    else if (str[0]=='5')
    {
      strcpy(errorstr,value);
      Log("SendClientCommand ended with error %s",errorstr);
      return -2;
    }
    else
    {
      strcpy(errorstr,"Unknown response received");
      Log("SendClientCommand ended with error %s",errorstr);
      return -2;
    }
  }
  Log("SendClientCommand end");
  return 0;
}

int GetServerVersion(HWND hWnd,PowerSettings *ps,char *errorstr)
{
  int ret,version,release,major,minor,bugfix;
  char str[500],*value;

  Log("GetServerVersion start");
  if (ps->clientsocket!=INVALID_SOCKET)
  {
    Log("Sending VERSION to remote machine");
    if (send(ps->clientsocket,"VERSION\n",8,0)==SOCKET_ERROR)
    {
      CloseClientSocket(hWnd,ps);
      return -1;
    }
    Log("Waiting for a reply");
    if ((ret=recv(ps->clientsocket,str,500,0))==SOCKET_ERROR || ret<=0)
    {
      CloseClientSocket(hWnd,ps);
      return -1;
    }
    Log("Received %s from remote machine",str);
    str[ret]=0;
    while (strlen(str)>0 && (str[strlen(str)-1]=='\n' || str[strlen(str)-1]=='\r'))
      str[strlen(str)-1]='\0';
    value=strchr(str,' ');
    if (value)
    {
      *value='\0';
      value++;
      Trim(value);
    }
    if (str[0]=='2')
    {
      Log("SendClientCommand end");
      sscanf(value,"%d.%d.%d.%d",&release,&major,&minor,&bugfix);
      version=bugfix+minor*100+major*10000+release*1000000;
      Log("version=%d",version);
    }
    else if (str[0]=='5')
    {
      strcpy(errorstr,value);
      Log("SendClientCommand ended with error %s",errorstr);
      return -2;
    }
    else
    {
      strcpy(errorstr,"Unknown response received");
      Log("SendClientCommand ended with error %s",errorstr);
      return -2;
    }
  }
  Log("GetServerVersion end");
  return version;
}

int SendRemoteCommand(HWND hWnd,PowerSettings *ps,char *errorstr)
{
  int ret,version;
  char str[500];

  Log("SendRemoteCommand start");
  if ((ret=OpenServerConnection(hWnd,ps,ps->remote.computer_name,errorstr))!=0)
    return ret;
  if ((version=GetServerVersion(hWnd,ps,errorstr))==0)
    return 1;
  if ((ret=SendClientCommand(hWnd,ps,errorstr,"PASSWORD %s",ps->remote.password))!=0)
    return ret;
  switch (ps->action)
  {
    case LOGOFF:
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION LOGOFF"))!=0)
        return ret;
      break;
    case REBOOT:
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION REBOOT"))!=0)
        return ret;
      break;
    case SHUTDOWN:
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION SHUTDOWN"))!=0)
        return ret;
      break;
    case POWEROFF:
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION POWEROFF"))!=0)
        return ret;
      break;
    case STANDBY:
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION STANDBY"))!=0)
        return ret;
      break;
    case HIBERNATE:
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION HIBERNATE"))!=0)
        return ret;
      break;
    case LOCK:
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION LOCK"))!=0)
        return ret;
      break;
    case WAKE_ON_LAN:
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION WAKE_ON_LAN"))!=0)
        return ret;
      break;
    case MONITOR_OFF:
      if (version<3000100)
      {
        strcpy(errorstr,"Version of poweroff server must be at least 3.0.1.0 to support this action");
        return -2;
      }
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION MONITOR_OFF"))!=0)
        return ret;
      break;
    case MONITOR_ON:
      if (version<3000100)
      {
        strcpy(errorstr,"Version of poweroff server must be at least 3.0.1.0 to support this action");
        return -2;
      }
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION MONITOR_ON"))!=0)
        return ret;
      break;
    case NO_ACTION:
      if (version<3000100)
      {
        strcpy(errorstr,"Version of poweroff server must be at least 3.0.1.0 to support this action");
        return -2;
      }
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION NO_ACTION"))!=0)
        return ret;
      break;
    default:
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"ACTION UNKNOWN"))!=0)
        return ret;
      break;
  }
  if ((ret=SendClientCommand(hWnd,ps,errorstr,"WHO LOCAL"))!=0)
    return ret;
  if (ps->remote.schedule_remote)
  {
    switch (ps->when)
    {
      case IMMEDIATE:
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"WHEN IMMEDIATE"))!=0)
          return ret;
        break;
      case SCHEDULED:
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"WHEN SCHEDULED"))!=0)
          return ret;
        break;
      case PROCESS:
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"WHEN AFTER_PROCESS"))!=0)
          return ret;
        break;
      default:
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"WHEN UNKNOWN"))!=0)
          return ret;
        break;
    }
  }
  else
  {
    if ((ret=SendClientCommand(hWnd,ps,errorstr,"WHEN IMMEDIATE"))!=0)
      return ret;
  }
  if ((ret=SendClientCommand(hWnd,ps,errorstr,"SIMULATE %s",ps->simulate?"ON":"OFF"))!=0)
    return ret;
  if (ps->options.warning)
  {
    if ((ret=SendClientCommand(hWnd,ps,errorstr,"WARNING %s",ps->options.warning?"ON":"OFF"))!=0)
      return ret;
    if ((ret=SendClientCommand(hWnd,ps,errorstr,"WARNTIME %d",ps->warning.seconds))!=0)
      return ret;
    strcpy(str,ps->warning.message);
    ReplaceString(str,"\r\n","\\n");
    ReplaceString(str,"\n","\\n");
    if ((ret=SendClientCommand(hWnd,ps,errorstr,"MESSAGE %s",str))!=0)
      return ret;
    if ((ret=SendClientCommand(hWnd,ps,errorstr,"PLAYSOUND %s",ps->warning.play_sound?"ON":"OFF"))!=0)
      return ret;
    if (ps->warning.play_sound)
    {
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"SOUNDFILE %s",ps->warning.sound_file))!=0)
        return ret;
    }
  }
  if ((ret=SendClientCommand(hWnd,ps,errorstr,"RUNPRG %s",ps->options.run_program?"ON":"OFF"))!=0)
    return ret;
  if (ps->options.run_program)
  {
    if ((ret=SendClientCommand(hWnd,ps,errorstr,"PROGRAM %s",ps->program.program))!=0)
      return ret;
    if ((ret=SendClientCommand(hWnd,ps,errorstr,"DIRECTORY %s",ps->program.directory))!=0)
      return ret;
  }
  if (ps->when==SCHEDULED)
  {
    switch (ps->schedule.schedule)
    {
      case FIXED_DAY:
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"SCHEDULE FIXED_DAY"))!=0)
          return ret;
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"DATE %02d/%02d/%04d",ps->schedule.date.wDay,ps->schedule.date.wMonth,ps->schedule.date.wYear))!=0)
          return ret;
        break;
      case DAILY:
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"SCHEDULE DAILY"))!=0)
          return ret;
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
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"DAYS %s",str))!=0)
          return ret;
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
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"WEEKS %s",str))!=0)
          return ret;
        break;
      case DAY_OF_MONTH:
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"SCHEDULE DAY_OF_MONTH"))!=0)
          return ret;
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"DAY %d",ps->schedule.day))!=0)
          return ret;
        break;
      case AFTER_X_SECONDS:
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"SCHEDULE AFTER_X_SECONDS"))!=0)
          return ret;
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"SECONDS %d",ps->schedule.seconds))!=0)
          return ret;
        break;
      default:
        if ((ret=SendClientCommand(hWnd,ps,errorstr,"SCHEDULE UNKNOWN"))!=0)
          return ret;
        break;
    }
    if (ps->schedule.schedule!=AFTER_X_SECONDS)
    {
      if ((ret=SendClientCommand(hWnd,ps,errorstr,"TIME %02d:%02d",ps->schedule.time.wHour,ps->schedule.time.wMinute))!=0)
        return ret;
    }
    if ((ret=SendClientCommand(hWnd,ps,errorstr,"WAIT %d",ps->schedule.wait))!=0)
      return ret;
  }
  if (ps->when==PROCESS)
  {
    if ((ret=SendClientCommand(hWnd,ps,errorstr,"PROCESS %s",ps->process.process))!=0)
      return ret;
  }
  if ((ret=SendClientCommand(hWnd,ps,errorstr,"FORCE %s",ps->options.force?"ON":"OFF"))!=0)
    return ret;
  if ((ret=SendClientCommand(hWnd,ps,errorstr,"CANCEL %s",ps->options.allow_cancel?"ON":"OFF"))!=0)
    return ret;
  if ((ret=SendClientCommand(hWnd,ps,errorstr,"DOIT"))!=0)
    return ret;
  CloseClientSocket(hWnd,ps);
  Log("SendRemoteCommand end");
  return 0;
}