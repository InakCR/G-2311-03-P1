
#include <redes2/irc.h>
#include <redes2/irctad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main() {
  char *arraylist, *ejemplo, **arrayNicks;
  long numberOfUsers;
  int numStrings, i, num;
  if (IRCTADUser_New("user", "nick", "realname", NULL, "host", "1.1.1.1",
                     12345) == IRC_OK) {
    printf("11\n");
  }
  if (IRCTADUser_New("user", "nick1", "realname", NULL, "host", "1.1.1.1",
                     12345) == IRC_OK) {
    printf("12\n");
  }
  if (IRCTADUser_New("user", "nick2", "realname", NULL, "host", "1.1.1.1",
                     12345) == IRC_OK) {
    printf("13\n");
  }
  if (IRCTADUser_New("user", "nick3", "realname", NULL, "host", "1.1.1.1",
                     12345) == IRC_OK) {
    printf("14\n");
  }
  IRCTADUser_ShowAll();
  if (UTestNick("nick3")) {
    printf("Existe Nick\n");
  }
  if (UTestNick("nick5")) {
    printf("Existe Nick\n");
  }
  if (IRCTAD_Join("#canal", "nick", "usermode", NULL) == IRC_OK) {
    printf("10\n");
  }
  if (IRCTAD_Join("#canal", "nick1", "usermode", NULL) == IRC_OK) {
    printf("9\n");
  }
  if (IRCTAD_Join("#canal", "nick2", "usermode", NULL) == IRC_OK) {
    printf("8\n");
  }
  if (IRCTAD_Join("#canal", "nick3", "usermode", NULL) == IRC_OK) {
    printf("7\n");
  }
  IRCTAD_Part("#canal", "nick3");
  long temp = IRCTAD_ListNicksOnChannel("#canal", &arraylist, &numberOfUsers);
  if (temp == IRC_OK) {
    printf("1\n");
  } else if (temp == IRCERR_NOENOUGHMEMORY) {
    printf("2\n");
  } else if (temp == IRCERR_NOVALIDCHANNEL) {
    printf("3\n");
  } else if (temp == IRCERR_NOVALIDUSER) {
    printf("4\n");
  } else {
    printf("5\n");
  }
  IRCParse_ParseLists(arraylist, &arrayNicks, &num);
  for (i = 0; i < numberOfUsers; i++) {
    printf("%s\n", arrayNicks[i]);
  }
  /*
IRC_BuildStringsFromList(&ejemplo, 512, ' ', &numStrings, &arraylist, 4);

printf ("%s | %d", *ejemplo, numStrings);
*/
  // IRCMsg_RplNamReply (&ejemplo, "SERVER", "nick", "=", "#canal",
  // arraylist);
  printf("%s | %ld\n", arraylist, numberOfUsers);
  // printf ("%s\n", ejemplo);
}
