/* 62070501022 – Modified 2021-09-23 – Fixed pointer declaretion
      in all file to make consistency code */

/* 62070501022 – Modified 2021-09-23 – Create c file network Builder
    that build network from file to fix God module in social network module */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "abstractGraph.h"
#include "abstractSocialNetwork.h"
#include "abstractNetworkBuilder.h"

/* declaration of function not in abstractGraph.h, to avoid warnings */
VERTEX_T* findVertexByKey(char* key, VERTEX_T** pPred);

/* 62070501022 – Modified 2021-09-23 – Create function createVertexByUser
    to build vertex by user */

void createVertexByUser(USER_T* pUser)
{
  int status = 0;
  char* username;
  username = pUser->username;
  status = addVertex(username,pUser);
  if (status == 0)
    printf(">> Memory allocation error adding user '%s'\n",username);
  else if (status < 0)
    printf(">> ERROR in data file! User '%s' already exists\n",username);
  else
    printf("Successfully added user '%s' to the social network\n",username);
}

/* 62070501022 – Modified 2021-09-23 – Create function createUser
    to read user and call function createVertexByUser */

int createUser(char* inputline)
{
  USER_T* pUser = NULL;
  char username[32];
  /* 62070501022 – Modified 2021-09-23 – Rename username2 to friend
      so that make code easy to understand*/
  char friend[32];
  char firstname[32];
  char lastname[32];
  char birthday[16];
  int status = 0;
  if (sscanf(inputline,"%s %s %s %s",username,firstname,lastname,birthday) != 4)
    printf("Invalid input line, skipping: '%s'",inputline);
  else
    {
      VERTEX_T* dummy;
      VERTEX_T* v = findVertexByKey(username,&dummy);
      if (v != NULL)
        printf(">> ERROR in data file! User '%s' already exists\n",username);
      else
        {
        pUser = calloc(1,sizeof(USER_T));
        if (pUser != NULL)
          {
          strcpy(pUser->username,username);
          strcpy(pUser->firstname,firstname);
          strcpy(pUser->lastname,lastname);
          strcpy(pUser->birthday,birthday);
          /* 62070501022 – Modified 2021-09-23 – Use function createVertexByUser
            to make code easy to understand */
          createVertexByUser(pUser);
          }
        else
          {
          printf("Memory allocation error creating a user\n");
          return 0;
          }
        }
    }
  return 1;
}

/* 62070501022 – Modified 2021-09-23 – Create function createFriendEdge
    to build edge */

void createFriendEdge(char* inputline)
{
  char username[32];
  char friend[32];
  if (sscanf(inputline,"FRIEND %s %s",username,friend) != 2)
    printf("Invalid format for FRIEND line: '%s'",inputline);
  else
    addFriend(username, friend);
}

/* 62070501022 – Modified 2021-09-23 – Create function readFileToBuildNetwork
    to read file and call function to build edge and vertex
    so that make code easy to understand*/

int readFileToBuildNetwork(FILE* file)
{
  char inputline[128];
  int status = 1; /* used to test for errors */
  while (fgets(inputline,sizeof(inputline),file) != NULL)
    {
    printf("read %s", inputline);
    /* if this is a FRIEND specification, create an edge */
    if (strncmp(inputline,"FRIEND",strlen("FRIEND")) == 0)
      createFriendEdge(inputline);
    else /* this is a user (VERTEX) */
      {
        status = createUser(inputline);
        if (status == 0)
          return 0;
      }
    }
  return 1;
}

/* 62070501022 – Modified 2021-09-23 – Moved function buildNetwork to this module
    so that fix God module and use readFileToBuildNetwork function
    to make code easy to understand */

/* Initializes and builds the network based on
 * the information in the file.
 * Returns 1 if successful, 0 if any error occurred
 */
int buildNetwork(char* filename)
{
  FILE* file = NULL;
  char inputline[128];
  int status = 1; /* used to test for errors */
  /* try to open the file */
  file = fopen(filename,"r");
  if (file == NULL)
    {
    printf("Error - cannot open file '%s'\n", filename);
    return 0;
    }
  /* since we can open the file, go ahead and initialize the graph */
  status = initGraph(1000,0); /* this should be an UNDIRECTED graph */
  if (!status)
    {
    printf("Error initializing the graph\n");
    return status;
    }
  /* Let's read the file and build the network */
  status = readFileToBuildNetwork(file);
  if (status == 0)
    return 0;
  fclose(file);
  return 1;
}