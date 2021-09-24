/* 62070501022 – Modified 2021-09-23 – Fixed pointer declaretion
      in all file to make consistency code */

/* 62070501022 – Modified 2021-09-23 – Create header file abstractSocialNetwork
      to defines the necessary functions for a social network */


#ifndef ABSTRACTSOCIALNEWWORK_H
#define ABSTRACTSOCIALNEWWORK_H


/* 62070501022 – Modified 2021-09-23 – Moved user struct to the header file
      so that other modules can use without duplicate code  */

/* Structure type for user in the social network
 * This will be the data stored at a vertex of the network.
 */
typedef struct
{
  char username[32];
  char firstname[32];
  char lastname[32];
  char birthday[16];
} USER_T;

USER_T* getUserProfile(char* username);

void suggestFriends(USER_T* targetUser);

int printFriends(USER_T* targetUser,  USER_T* pExcludeUser,
		 char** userFriends, int count);

void addFriend(char* username, char* friendname);

void addFriends(USER_T* focusUser);

USER_T* addUser(char* username);

#endif

