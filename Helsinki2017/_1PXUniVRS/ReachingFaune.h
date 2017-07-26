/***********************************************************************/
/* O. Dadoun & N. Darrot July 2017 
 ReachingFaune to access         
  faune.xyz values               

  ____                 _     _             _____
|  _ \ ___  __ _  ___| |__ (_)_ __   __ _|  ___|_ _ _   _ _ __   ___
| |_) / _ \/ _` |/ __| '_ \| | '_ \ / _` | |_ / _` | | | | '_ \ / _ \
|  _ <  __/ (_| | (__| | | | | | | | (_| |  _| (_| | |_| | | | |  __/
|_| \_\___|\__,_|\___|_| |_|_|_| |_|\__, |_|  \__,_|\__,_|_| |_|\___|
                                    |___/
                                                                      */
/***********************************************************************/
#ifndef _ReachingFaune_
#define _ReachingFaune_H_
#include <SPI.h>
#include <Ethernet2.h>

class ReachingFaune
{
	public:
		ReachingFaune();
		~ReachingFaune();
		boolean connectToServer(); 
		void check_page();
		
		void parse_web(char* input_string,char (&line_xy_rgb)[7][64]);
	
	private:
		EthernetClient client;
		char serverName[256];
		unsigned long lastAttemptTime;
		String currentLine;
		boolean lecture;
		String message;

	public:
		String GetMessage(){return message;};
		void SetMessage(String itsmessage){message=itsmessage;};

		boolean GetLecture(){return lecture;};
		void SetLecture(boolean itslecture){lecture=itslecture;};
		
		EthernetClient GetClient(){return client;};

		unsigned long GetLastAttemptTime(){return lastAttemptTime;};
    void SetLastAttemptTime(unsigned long itslastAttemptTime){lastAttemptTime=itslastAttemptTime;};

    String GetServerName() {return String(serverName);};
};
#endif
