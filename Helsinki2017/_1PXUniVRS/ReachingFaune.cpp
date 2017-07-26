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
#include "ReachingFaune.h"

ReachingFaune::ReachingFaune():lastAttemptTime(0),currentLine(""),lecture(false),message("")
{
	sprintf(serverName,"%s","faune.xyz");
}

ReachingFaune::~ReachingFaune()
{}

void ReachingFaune::check_page()
{ 
 if (client.available()) {
   // read incoming bytes:
   char inChar = client.read();
   // add incoming byte to end of line:
   currentLine += inChar;
   // si on a un retour chariot, c'est que ce n'est pas encore le debut, on supprime:
   if (inChar == '\n') {
     currentLine = "";
   }
   // si on trouve le message <text>, c'est
   // que le message suit:
   
   if ( currentLine.endsWith("<info>")) {
     // debut du message, on vide la string message:
     lecture = true;
     message = "";
     inChar = client.read(); // lire le caractere suivant
   }
   // on lit caractere par caractere,
   // et les ajoute au message
   if (lecture) {
     if (inChar != '<') {
       message += inChar;
     }
     else {
       // isi vous avez un "<",
       // c'est la fin du message:
       lecture = false;
       // fermer la connexion au serveur:
       client.stop();
     }
   }
 }

SetLecture(lecture);
}
 
boolean ReachingFaune::connectToServer()
{
   boolean bool_value; 
   // attempt to connect, and wait a millisecond:
   Serial.println("connecting to server...");
   if (client.connect(serverName, 80)) 
   {
   Serial.println("making HTTP request...");
   // make HTTP GET request:
   client.println("GET /update.php HTTP/1.1"); 
   client.println("Host: faune.xyz");
   client.println();
   bool_value=true;
    }
  else {
    Serial.println("connection failed");
    bool_value=false;
  }
 // remettre le compteur a zero pour la prochaine connexion:
 //lastAttemptTime = millis();
 SetLastAttemptTime(millis());
 return bool_value;
}
void ReachingFaune::parse_web(char* input_string,char (&line_xy_rgb)[7][64])
{
  char *token;
  token = strtok(input_string, ",");
  int i = 1;
  while (token != NULL ) {
    sprintf(line_xy_rgb[i - 1], "%s", token);
    token = strtok(NULL, ",");
    i++;
  }
}

