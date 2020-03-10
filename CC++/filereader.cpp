#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>

using namespace std;

class CRoute
{
public:
	CRoute();
	int getAnzWaypoint();
	void setDataWaypoint(CFileReader *cRoutenfile);
	double calculateDistance(int iStartindex);
	double calculateTotalDistance(void);
	m_CWaypoint cFlugroute[20];
private:
	int m_iAnzahlWaypoint;
};

CRoute::CRoute()
{
	m_iAnzahlWaypoint=0;
}

int CRoute::getAnzWaypoint()
{
	return m_iAnzahlWaypoint;
}

void CRoute::setDataWaypoint(CFileReader *cRoutenfile)
{


}

double calculateDistance(int iStartindex)
{
}

double calculateTotalDistance(void)
{

}


class CWaypoint
{
public:
	CWaypoint();
	setKoordinaten(double dx,double dy);
	double getXPos();
	double getXPos();
	void setWaypointInfo(char sWaypointInfo[]);
	char * getWaypointInfo();
private:
	double m_dXpos;
	double m_dYpos;
	char m_sWaypointInfo[80];
};

CWaypoint
::CWaypoint
()
{
	m_dXpos=0;
	m_dYpos=0;
	m_sWaypointInfo="Keine Waypointinformation";
}

CWaypoint::setKoordinaten(double dx,double dy)
{
	m_dXpos=dx;
	m_dYpos=dy;
}

double CWaypoint::getXPos()
{
	return m_dXpos;
}

double CWaypoint::getXPos()
{
	return m_dYpos;
}

void  CWaypoint::setWaypointInfo(char sWaypointInfo[])
{
	strcpy(m_sWaypointInfo,sWaypointInfo);
}

char CWaypoint::*getWaypointInfo()
{
	return m_sWaypointInfo;
}


class CFileReader
{
public:
	CFileReader();
	CFileReader(char sfilename[]);
	int getAnzahlZeilen();
	getFileContent(int iindex);
	bool readFile();
private:
	FILE *m_Filepointer;
	m_sFilename[50];
	m_sFileContent[20][80];
	m_iAnzZeilen;

};

CFileReader::CFileReader()
{
	m_Filepointer=NULL;
	m_iAnzZeilen=0;
}

CFileReader::CFileReader(char sfilename[])
{
	
	m_Filepointer=NULL;
	m_iAnzZeilen=0;
}

int CFileReader::getAnzahlZeilen()
{
	return m_iAnzZeilen;
}

CFileReader::getFileContent(int iindex)
{
	return m_sFileContent[iindex];
}

bool CFileReader::readFile()
{
	m_Filepointer=m_sFileName;


	fgets(,,*m_sFilename);

}

void main (void)
{

}