/* Module to control menu for socialNetwork program
 *   Created by Sally Goldin 16 September 2021
 */
#include <stdio.h>
#include <string.h>
#include "abstractMenu.h"
#include "abstractSocialNetwork.h"

/* Present a menu of actions and ask the user which one they
 * want. Keep asking until a valid option is provided.
 * Return the selected option.
 */
int getMenuChoice()
{
  char input[32];
  int choice = 0;
  printf("\nAvailable actions:\n");
  printf("\t1 - Create a new user\n");
  printf("\t2 - Show a user's friends\n");
  printf("\t3 - Suggest new friends\n");
  printf("\t4 - Add new friends\n");
  printf("\t5 - Exit the program\n");
  while ((choice < 1) || (choice > 5))
    {
    printf("What action? ");  
    fgets(input,sizeof(input),stdin);
    sscanf(input,"%d",&choice);
    }
  return choice;
}

void createNewUserOption(char *username) {
  USER_T * myProfile = NULL;
  printf("\nEnter new username: ");
  myProfile = getUserProfile(username);
  if (myProfile == NULL)
    {
    myProfile = addUser(username);
    }
  else
    {
    printf("\tSorry, that username is already in use\n");
    }
}

void printFriendsOption(char *username) {
  USER_T * myProfile = NULL;
  printf("\nPrint friends for what user? ");
  myProfile = getUserProfile(username);
  if (myProfile != NULL)
    {
    printFriends(myProfile,NULL,NULL,0);
    }
  else
    {
    printf("\tNo user exists with that username\n");
    }
}

void suggestFriendsOption(char *username) {
  USER_T * myProfile = NULL;
  int friendcount = 0;
  printf("\nSuggest friends for what user? ");
  myProfile = getUserProfile(username);
  if (myProfile != NULL)
    {
    friendcount = printFriends(myProfile,NULL,NULL,0);
    if (friendcount > 0)
      suggestFriends(myProfile);
    }
  else
    {
    printf("\tNo user exists with that username\n");
    }
}

void addFriendOption(char *username) {
  USER_T * myProfile = NULL;
  printf("\nAdd friends for what user? ");
  myProfile = getUserProfile(username);
  if (myProfile != NULL)
    {
    addFriends(myProfile);
    }
  else
    {
    printf("\tNo user exists with that username\n");
    }
}


void runMenu() {
  int option = 0;
  char username[32] = "";
  while (option != 5)
    {
    option = getMenuChoice();
    switch(option)
      {
      case 1:
          createNewUserOption(username);
	        break;
      case 2:
          printFriendsOption(username);
          break;
      case 3:
          suggestFriendsOption(username);
          break;
      case 4:
          addFriendOption(username);
          break;
      case 5:
	        break;
      }
    }
}

