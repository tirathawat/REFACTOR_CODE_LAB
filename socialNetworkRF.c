/* 62070501022 – Modified 2021-09-23 – Fixed pointer declaretion
      in all file to make consistency code */

/**
 * Simple social network
 * This code reads a file that defines vertices and edges
 * and builds a graph using the graph ADT defined in abstractGraph.h.
 * It then allows the user to search for users, display their friends,
 * suggest new friends, and add new friends.
 * Version created 16 September 2021 with deliberate Code Smells,
 * for CPE327 Refactoring Exercise
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "abstractGraph.h"
#include "abstractMenu.h"
#include "abstractSocialNetwork.h"
#include "abstractNetworkBuilder.h"

/* declaration of function not in abstractGraph.h, to avoid warnings */
VERTEX_T* findVertexByKey(char* key, VERTEX_T** pPred);


/* 62070501022 – Modified 2021-09-23 – Create function add friend
      to reduce duplicate code and useful to use in other module  */

void addFriend(char* username, char* friendname)
{
  int status = 1;
  status = addEdge(username,friendname);
  if (status == 0)
    printf("\t>> User '%s' does not exist\n",friendname);
  else if (status < 0)
    printf("\t>> User '%s' is already your friend\n",friendname);
  else
    printf("\t>> User '%s' is now your friend\n",friendname);
}


/* Create a new user and add to the social network.
 *   username     Username for the new user
 * Returns pointer to the USER_T constructed by this function
 * or NULL if an error.
 */
USER_T* addUser(char* username)
{
  USER_T* newUser = NULL;
  char input[32];
  newUser = calloc(1,sizeof(USER_T));
  if (newUser != NULL)
    {
    printf("\nCreating a new user profile for '%s'\n",username);
    strcpy(newUser->username,username);
    printf("\tWhat is your first name? ");
    fgets(input,sizeof(input),stdin);
    sscanf(input,"%s",newUser->firstname);
    printf("\tWhat is your last name? ");
    fgets(input,sizeof(input),stdin);
    sscanf(input,"%s",newUser->lastname);
    printf("\tWhat is your birthday (dd-mm-yyyy)? ");
    fgets(input,sizeof(input),stdin);
    sscanf(input,"%s",newUser->birthday);
    if (addVertex(username,newUser) != 1)
      {
      printf("Error adding new user\n");
      free(newUser);
      newUser = NULL;
      }
    }
  return newUser;
}


/* Create friend relationships with the focus user
 *  focusUser      User for whom we are creating friends
 *                 Assumes this user exists
 */
void addFriends(USER_T* focusUser)
{
  char friendname[32];
  printf("\nAdding friends for user '%s %s (%s)'\n",
    focusUser->firstname,
	  focusUser->lastname,
    focusUser->username);
  while (1)
    {
    printf("\tWho do you want to add (username)? (DONE to stop) ");
    fgets(friendname,sizeof(friendname),stdin);
    if (strcasecmp(friendname,"DONE") == 0)
      break;
    if (strcmp(friendname,focusUser->username) == 0)
      printf("\t>>You can't add yourself as a friend!\n");
    else
      /* 62070501022 – Modified 2021-09-23 – User addFriend function to
        to make code easy to understand  */
      addFriend(focusUser->username, friendname);
    }
}


/* Look through a list of existing friend usernames
 * to make sure the target username does not appear.
 *    targetUsername           Name we're looking for
 *    friendnames              Array of keys
 *    count                    Size of friendnames
 * Returns 1 if found, 0 if not found
 */
int searchExisting(char* targetUsername, char** friendnames, int count)
{
  int i = 0;
  int found = 0;
  for (i = 0; (i < count) && (!found); i++)
    if (strcmp(targetUsername,friendnames[i]) == 0)
      found = 1;
  return found;
}

/* 62070501022 – Modified 2021-09-23 – Fix indentation for function printFriends
      to follow code standard  */

/* This function will print the names of all the friends
 * of the passed user. It will return the number of friends
 * found (which could be 0).
 * The function is used both for simply printing friends of a user (Case1)
 * and for suggesting new friends (by printing the friends of friends) (Case 2)
 *   pUser                  User whose friends we are printing
 *   pExcludeUser           If non-NULL, we are printing suggestions (Case 2)
 *                          So we will not print the header, and we
 *                          will exclude this username if he or she
 *                          appears in the suggestion list
 *   userFriends            Only specified if pExcludeUser is non-null (Case 2)
 *                          holds the keys for all of pExcludeUser's friends
 *                          so we don't suggest someone with whom the
 *                          user is already connected.
 *   count                  0 if userFriends is NULL, else size of userFriends
 */
int printFriends(USER_T* targetUser,  USER_T* pExcludeUser,
		 char** userFriends, int count)
{
  int i = 0;
  int friendCount = 0;
  int adjustedFriendCount = 0;  /* number of friends who were not excluded */
  char** friendKeys = getAdjacentVertices(targetUser->username,&friendCount);
  if (friendCount > 0)
    {
    adjustedFriendCount = friendCount;
    if (pExcludeUser == NULL)
      printf("Here is a list of your current friends:\n");
    for (i = 0; i < friendCount; i++)
      {
      USER_T * friend = findVertex(friendKeys[i]);
      if (friend == NULL)
	      printf("Something is wrong - can't find USER_T for key '%s'\n",
	        friendKeys[i]);
      else if (friend == pExcludeUser || searchExisting(friendKeys[i],userFriends,count))
      	adjustedFriendCount--;
      else
        {
        /* exclude people who are already my friends */
        printf("\t%s %s (%s) - Birthday %s\n",
            friend->firstname,
            friend->lastname,
            friend->username,
            friend->birthday);
        }
      }
    }
  else
    {
    if (pExcludeUser == NULL) /* Case 1, when there are 0 adjacents */
	    printf("You do not have any friends yet\n");
    }
  if (friendKeys != NULL)
    free(friendKeys);
  return adjustedFriendCount;
}

/* 62070501022 – Modified 2021-09-23 – Fix indentation for function suggestFriends
      to follow code standard  */

/* Suggest as new friends people who are friends of your friends.
 *   pUser -- focus user for whom we are suggesting friends.
 */
void suggestFriends(USER_T* targetUser)
{
 int i = 0;
 int dummyCount = 0;
 int friendCount = 0;
 char** friendKeys = getAdjacentVertices(targetUser->username, &friendCount);
 if (friendCount > 0)
    {
    printf("\n-- Here are some people you might like to know --\n");  
    for (i = 0; i < friendCount; i++)
      {
      USER_T * friend = findVertex(friendKeys[i]);
      if (friend != NULL)
        {
        printf("  Friends of %s whom you might like: \n",friend->username);  
        dummyCount = printFriends(friend,targetUser,friendKeys,friendCount);
        if (dummyCount == 0)
          printf("\t>> %s has no friends who aren't already your friends\n",friend->username);
        }
      else
	      printf("\t\tSomething is very wrong - no user associated with username '%s'?\n", friendKeys[i]);
      }
    }
}

/* 62070501022 – Modified 2021-09-23 – Create function getUserProfile
      to reduce duplicate code and useful to use */

USER_T* getUserProfile(char* username)
{
  USER_T* myProfile = NULL;
  char inputline[32];
  fgets(inputline,sizeof(inputline),stdin);
  sscanf(inputline,"%s",username);
  myProfile = findVertex(username);
  return myProfile;
}

/* 62070501022 – Modified 2021-09-23 – Use function buildNetwork and runMenu 
      in other module to make code easy to understand*/

/* main function builds the graph, then enters a loop 
 * doing different operations 
 */
int main(int argc, char* argv[])
{
  int status = 1;
  USER_T* myProfile = NULL;
  if (argc < 2)
    {
    printf("Please provide an input file name: ./socialNetwork [file]\n");
    exit(0);
    }
  status = buildNetwork(argv[1]);
  if (!status)
    {
    printf("Fatal error building the social network\n");
    exit(1);
    }
  runMenu();
  printf("\nGoodbye!\n");
}
