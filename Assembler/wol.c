#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "poweroff.h"

#define REPEAT_ADDRESS     20
#define MAC_ADDRESS_LENGTH  6
#define HEADER_LENGTH       6

unsigned char atox(char s)
{
    if (s >= 'a'){
        s -= 0x20;
    }
    if (s >= 'A'){
        s -= 'A';
        s += 10;
    } else if ('1' <= s && s <= '9') {
        s -= '1';
        s += 1;
    } else {
        s = 0;
    }
    return((unsigned char)s);
}

int get_broadcast_addr(char *broadcast,char *subnet,char *mask)
{
  long sn, ba, ma;
  struct in_addr ia;

  Log("get_broadcast_addr start");
  sn = ntohl(inet_addr(subnet));
  ma = ntohl(inet_addr(mask));
  ba = sn & ma;
  ba |= ~ma;
  ia.s_addr = htonl(ba);
  strcpy(broadcast, (char *)inet_ntoa(ia));
  Log("get_broadcast_addr end, addr=%s",broadcast);
  return(0);
}

int WakeOnLan(PowerSettings *ps,char *ip_address,char *mask,char *mac_address)
{
  int sd; /* Socket descriptor */
  unsigned char dest_mac_address[10];
  unsigned char sync[10] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  unsigned char buf[REPEAT_ADDRESS*MAC_ADDRESS_LENGTH+HEADER_LENGTH];
  int n; /* The number of octet sent to destination */
  struct sockaddr_in hostaddr;  /* holds socket address */
  char broadcastaddr[50];
  BOOL t=TRUE;
  
  Log("WakeOnLan start, ip=%s, mask=%s, mac=%s",ip_address,mask,mac_address);
  get_broadcast_addr(broadcastaddr, ip_address, mask);
  dest_mac_address[0] = atox(mac_address[0])*0x10 + atox(mac_address[1]);
  dest_mac_address[1] = atox(mac_address[2])*0x10 + atox(mac_address[3]);
  dest_mac_address[2] = atox(mac_address[4])*0x10 + atox(mac_address[5]);
  dest_mac_address[3] = atox(mac_address[6])*0x10 + atox(mac_address[7]);
  dest_mac_address[4] = atox(mac_address[8])*0x10 + atox(mac_address[9]);
  dest_mac_address[5] = atox(mac_address[10])*0x10 + atox(mac_address[11]);
  
  memcpy(buf, sync, HEADER_LENGTH);
  for (n = 0; n < REPEAT_ADDRESS; n++)
  {
    memcpy(&buf[HEADER_LENGTH+n*MAC_ADDRESS_LENGTH], dest_mac_address, MAC_ADDRESS_LENGTH);
  }
    
  /* Set Destination Address */
  hostaddr.sin_family = AF_INET;
  hostaddr.sin_addr.s_addr = inet_addr(broadcastaddr);
  
  /* Set Destination Port */
  hostaddr.sin_port = htons(7);
    
  /* Open an UDP socket */
  if ((sd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    ShowSocketError(ps,"socket");
    return -5;
  }

  /* Enable to broadcast */
  if (setsockopt(sd, SOL_SOCKET, SO_BROADCAST, (char *)&t, sizeof(t)) == SOCKET_ERROR)
  {
    ShowSocketError(ps,"setsockopt");
    closesocket(sd);
    return -6;
  }
  
  Log("Sending magic packet");
  /* Send *Magic data* to destination node */
  if ((n = sendto(sd, buf, REPEAT_ADDRESS*MAC_ADDRESS_LENGTH+HEADER_LENGTH, 0, (LPSOCKADDR)&hostaddr, sizeof(hostaddr))) == SOCKET_ERROR)
  {
    ShowSocketError(ps,"sendto");
    closesocket(sd);
    return -7;
  }

  /* Close the UDP socket */
  if (sd >= 0)
  {
    closesocket(sd);
  }

  Log("WakeOnLan end");
  return 0;
}
