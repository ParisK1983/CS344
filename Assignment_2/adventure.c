#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <fcntl.h>

//Room node.//////////////////////////////////////////////////////////////////////////////
typedef struct room_node{
    int roomName;               //Index to the room array.
	int numOfConnections;		//Number of connections to the room.
	int roomType;				//Room type 0=Start_room, 1=Mid_Room, 2=End_Room.
    struct room_node *next; 	//Pointer to the connection.
}room, *room_p;

room_p roomArr[7];

//Function to initialize the name of the rooms.///////////////////////////////////////////
char** initNameRooms(char** rooms){
	rooms[0] = "Livingroom.txt";
	rooms[1] = "Kitchen.txt";
	rooms[2] = "Playroom.txt";
	rooms[3] = "Bathroom.txt";
	rooms[4] = "Office.txt";
	rooms[5] = "Bedroom.txt";
	rooms[6] = "Garage.txt";
	rooms[7] = "Garden.txt";
	rooms[8] = "Storageroom.txt";
	rooms[9] = "Roof.txt";
	return (rooms);
}

//Function to initialize the room types.//////////////////////////////////////////////////
char** initRoomTypes(char** type){
	type[0] = "Start_Room";
	type[1] = "Mid_Room";
	type[2] = "End_Room";
	return (type);
}

//Function to initialize the winning route array./////////////////////////////////////////
int* initWinningRoute(int* winningRoute){	
	winningRoute[0] = 11;
	winningRoute[1] = 11;
	winningRoute[2] = 11;
	winningRoute[3] = 11;
	winningRoute[4] = 11;
	winningRoute[5] = 11;
	return (winningRoute);
}

//Function to create rooms and connections.///////////////////////////////////////////////
room_p createRoom(int roomName, int roomType){
    room_p newRoom = malloc(sizeof(room));
	newRoom->roomName = roomName;
	newRoom->numOfConnections = 0;
	newRoom->roomType = roomType;
	newRoom->next = NULL;
    return newRoom;
}

//Function to initialize the room array.//////////////////////////////////////////////////
void initArr(){
    int roomNum = 0;
    for (roomNum = 0; roomNum < 7; roomNum++){
			roomArr[roomNum]= NULL;
	}
}

//Function to add a room in the room array.///////////////////////////////////////////////
//Returns 1 if the connection is added and 0 if not.
int addRoom(int src, int dest, int roomType){
	int exists = 0;
	int roomIdx = 0;
	room_p newRoom = createRoom(dest, roomType);
	room_p tmpRoom = roomArr[src];
	if (tmpRoom != NULL){
		if (tmpRoom->roomName == newRoom->roomName){
				exists = 1;
		}
		for (roomIdx = 0; roomIdx <= roomArr[src]->numOfConnections; roomIdx++){
			if (tmpRoom->roomName == newRoom->roomName){
				exists = 1;
			}
			tmpRoom = tmpRoom->next;
		}
		if (exists == 0){
			tmpRoom = roomArr[src]->next;
			roomArr[src]->next = newRoom;
			newRoom->next = tmpRoom;
			roomArr[src]->numOfConnections++;
			return (1);
		}else{
			return (0);
		}
	}else{
		roomArr[src] = newRoom;
		return (1);
	}
}

//Function for adding a connection to the room.///////////////////////////////////////////
//Returns 1 if the connection is added and 0 if not.
int addConnection(int src, int dest){
	int exists = 0;
	int roomIdx = 0;
	room_p newRoom = createRoom(dest, 1);
	room_p tmpRoom = roomArr[src];
	if (tmpRoom->roomName == newRoom->roomName){
			exists = 1;
	}
	for (roomIdx = 0; roomIdx <= roomArr[src]->numOfConnections; roomIdx++){
		if (tmpRoom->roomName == newRoom->roomName){
			exists = 1;
		}
		tmpRoom = tmpRoom->next;
	}
	if (exists == 0){
		tmpRoom = roomArr[src]->next;
		roomArr[src]->next = newRoom;
		newRoom->next = tmpRoom;
		roomArr[src]->numOfConnections++;
		return (1);
	}else{
		return (0);
	}
}

//Function to calculate and returns the random number of connections for every room.//////
int calcConnections(int roomIdx){
	int connections = 0;
	while (connections > 6 || connections < 3){
		connections = (rand() % 4) + 3;
		connections += roomArr[roomIdx]->numOfConnections;
	}
	connections -= roomArr[roomIdx]->numOfConnections;
	return (connections);	
}

//Function to print the graph with rooms and connections./////////////////////////////////
void displayGraph(){
    int roomIdx = 0;
	int connIdx = 0;
    for (roomIdx = 0; roomIdx < 7; roomIdx++){
        room_p tmpRoom = roomArr[roomIdx];
        printf("\n%d-%d-%d: ",tmpRoom->roomName,tmpRoom->roomType,tmpRoom->numOfConnections);
        for (connIdx = 1; connIdx <= roomArr[roomIdx]->numOfConnections; connIdx++){
            printf("%d->",tmpRoom->next->roomName);
            tmpRoom = tmpRoom->next;
        }
    }
}

//Function to initialize the 7 rooms./////////////////////////////////////////////////////
int* initRooms(int* usedRooms){
	srand(time(NULL));
	int roomName = 0;
	int roomType = 4;
	int roomIdx = 0;
	int _roomIdx = 0;
	int exists = 0;
	int startRoomExists = 0;
	int endRoomExists = 0;

//Initializes the room array.
	for (roomIdx = 0; roomIdx < 7; roomIdx++){
		roomName = rand() % 10;
		if (roomIdx != 0){
			for (_roomIdx = 0; _roomIdx < roomIdx; _roomIdx++){
				if (roomArr[_roomIdx]->roomName == roomName){
					exists = 1;
					break;
				}
			}
			if (exists == 1){
				roomIdx--;
				exists = 0;
			}else{
				addRoom(roomIdx, roomName, roomType);
				usedRooms[roomIdx] = roomName;
			}
		}else{
			addRoom(roomIdx, roomName, roomType);
			usedRooms[roomIdx] = roomName;
		}
	}
	
//Initializes the room types.
	for (roomIdx = 0; roomIdx < 7; roomIdx++){
		roomName = rand() % 10;
		if (startRoomExists == 0 && endRoomExists == 0){
			roomType = rand() % 3;
			if (roomType == 0){
				startRoomExists = 1;
			}else if (roomType == 2){
				endRoomExists = 1;
			}
		}else if (startRoomExists == 0 && endRoomExists != 0){
			roomType = rand() % 2;
			if (roomType == 0){
				startRoomExists = 1;
			}
		}else if (startRoomExists != 0 && endRoomExists == 0){
			roomType = (rand() % 2) + 1;
			if (roomType == 2){
				endRoomExists = 1;
			}
		}else{
			roomType = 1;
		}
	
		if (roomIdx == 5){
			if (startRoomExists == 0){
				roomType = 0;
				startRoomExists = 1;
			}
		}
		if (roomIdx == 6){
			if (endRoomExists == 0){
				roomType = 2;
				endRoomExists = 1;
			}
		}
		roomArr[roomIdx]->roomType = roomType;
	}
	return (usedRooms);
}

//Function to create one winning route. Returns the array with the path./////////////////
int* calcWinningRoute(int* winningRoute){
	int _roomType = 4;
	int roomIdx = 0;
	int startRoomIdx = 0;
	int endRoomIdx = 0;
	for (roomIdx = 0; roomIdx < 6; roomIdx++){
		_roomType = roomArr[roomIdx]->roomType;
		if (_roomType == 0){
			startRoomIdx = roomIdx;
		}else if (_roomType == 2){
			endRoomIdx = roomIdx;
		}else{
			winningRoute[roomIdx] = roomArr[roomIdx]->roomName;
		}
	}
	if (winningRoute[0] != 11){
		for (roomIdx = 1; roomIdx < 5; roomIdx++){
			if (winningRoute[roomIdx] == 11){
				winningRoute[roomIdx] = winningRoute[0];
			}
		}
	}
	if (winningRoute[5] != 11){
		for (roomIdx = 1; roomIdx < 5; roomIdx++){
			if (winningRoute[roomIdx] == 11){
				winningRoute[roomIdx] = winningRoute[5];
			}
		}
	}
	winningRoute[0] = roomArr[startRoomIdx]->roomName;
	winningRoute[5] = roomArr[endRoomIdx]->roomName;
	return (winningRoute);
}

//Function to add the connections in every room.//////////////////////////////////////////
void addConnections(int* winningRoute, int* usedRooms){
	int i = 0;
	int k = 0;
	int m = 0;
	int j = 0;
	int n = 2;
	int c = 0;
	int src = 0;
	int roomConnections = 0;
	for(i=0;i<5;i++){
		for (k = 0; k < 6; k++){
			if (roomArr[k]->roomName == winningRoute[i]){
				addConnection(k,winningRoute[i+1]);
			}
			if (roomArr[k]->roomName == winningRoute[i+1]){
				addConnection(k,winningRoute[i]);
			}
		}
	}
	for (i = 0; i < 7; i++){
		roomConnections = calcConnections(i);
		src = roomArr[i]->roomName;
		for (k = 0; k < roomConnections; k++){
			j = rand() % 7;
			n = addConnection(i, usedRooms[j]);
			if (n == 1){
				c++;
				for (m = 0; m < 7; m++){
					if (roomArr[m]->roomName == usedRooms[j]){
						//addConnection(m,roomArr[i]->roomName);
						n = 2;
						break;
					}
				}
			}else{
				k--;
				n = 2;
			}
		}
	}
}

//Function to convert the integer room names to the actual room names.///////////////////
char* intToName(int intName){
	char* roomName = "";
	switch (intName){
		case 0:
			roomName = "Livingroom.txt";
			break;
		case 1:
			roomName = "Kitchen.txt";
			break;
		case 2:
			roomName = "Playroom.txt";
			break;
		case 3:
			roomName = "Bathroom.txt";
			break;
		case 4:
			roomName = "Office.txt";
			break;
		case 5:
			roomName = "Bedroom.txt";
			break;
		case 6:
			roomName = "Garage.txt";
			break;
		case 7:
			roomName = "Garden.txt";
			break;
		case 8:
			roomName = "Storageroom.txt";
			break;
		default:
			roomName = "Roof.txt";
	}
	return (roomName);
}
	
//Function to convert the integer room types to the actual room types.///////////////////	
char* intToType(int intType){
	char* roomType = "";
	switch (intType){
		case 0:
			roomType = "Start_Room";
			break;
		case 1:
			roomType = "Mid_Room";
			break;
		default:
			roomType = "End_Room";
	}
	return (roomType);
}

//Function to create the directory.//////////////////////////////////////////////////////
char* mkDir(){
	int pid = getpid();
	char *prefix = "kalathas.rooms.";
	int bufSize = 40;
	char *dirName = malloc(bufSize);
	snprintf(dirName, bufSize, "%s%d",prefix, pid);
	mkdir(dirName, 0755);
	return (dirName);
}

//Functions to create the rooms.//////////////////////////////////////////////////////////
void mkRooms(char* dirName){
	int roomIdx = 0;
	int connIdx = 0;
	int _connIdx = 0;
	int bufSize = 40;
	char *room = malloc(bufSize);
	char *roomName = malloc(bufSize);
	char *roomType = malloc(bufSize);
	for (roomIdx = 0; roomIdx < 7; roomIdx++){
		room_p tmpRoom = roomArr[roomIdx];
		roomName = intToName(tmpRoom->roomName);
		roomType = intToType(tmpRoom->roomType);
		_connIdx = tmpRoom->numOfConnections;
		
		snprintf(room, bufSize, "%s/%s",dirName,roomName);
		FILE *fcheck = fopen(room, "r");
		if (fcheck < 0){										
				printf("ERROR: CANNOT OPEN FILE.\n");				
				
			}
		
		if (fcheck != NULL){
			roomIdx--;
		}else{
			FILE* f = fopen(room, "w");
			if (f < 0){
				fprintf(stderr, "Error: Cannot create rooms\n");
				exit(1);
			}
			fprintf(f, "ROOM NAME: %s\n",roomName);
			for (connIdx = 0; connIdx < _connIdx; connIdx++){
				fprintf(f, "CONNECTION %d: %s\n",(connIdx + 1),intToName(tmpRoom->next->roomName));
				tmpRoom = tmpRoom->next;
			}
			fprintf(f, "ROOM TYPE: %s",roomType);
			fclose(f);
		}
	}
}

//Functions to create the user's route.//////////////////////////////////////////////////////////
void route(char* dirName, char* roomName){
	int bufSize = 40;
	char *tmpRoute = malloc(bufSize);
	snprintf(tmpRoute, bufSize, "%s/%s",dirName,"tmpRoute.txt");
	FILE* f = fopen(tmpRoute, "a");
	if (f < 0){
		fprintf(stderr, "Error: Cannot create tmpRoute.\n");
		exit(1);
	}
	fprintf(f, "%s\n",roomName);
	fclose(f);	
}

//Main function.////////////////////////////////////////////////////////////////////////////
int main(){
	int bufSize = 40;									//Initialize the variables.
	char** rooms = malloc(10 * bufSize);				//
	char** roomTypes = malloc(3 * bufSize);				//
	int* winningRoute = malloc(6 * sizeof(int));		//
	int* usedRooms = malloc(7 * sizeof(int));			//
	char* line = malloc(bufSize);
	char* line2 = malloc(bufSize);
	char* line3 = malloc(bufSize);
	char* fileName = malloc(bufSize);
	char* tmpRoute = malloc(bufSize);
	int steps = 0;
	
	int exists = 2;
	char* roomChoice = malloc(bufSize);
	
	initArr();											//Creates the rooms
	char* dirName = mkDir();							//and their connections.
	rooms = initNameRooms(rooms);						//
	roomTypes = initRoomTypes(roomTypes);				//
	usedRooms = initRooms(usedRooms);
	winningRoute = initWinningRoute(winningRoute);
	calcWinningRoute(winningRoute);
	addConnections(winningRoute, usedRooms);
	mkRooms(dirName);
	char* roomName = intToName(winningRoute[0]);
/////////////////////////////////////////////////////////////////////////////////////////////
/*	int b = 0;
	printf("------------------------------------------------------------------------\n");	//If you uncomment this will
	printf("Winning route: ");															                            	//print the winning route.
	for(b = 0; b < 6; b++)																                              	//Duplicates are possible but the
		printf("%s, ",intToName(winningRoute[b]));										                    	//first room is the start room
	printf("\n------------------------------------------------------------------------");	//and the last the end room.
	printf("\n\n");																			                                  //
*/

	do{
		snprintf(fileName, bufSize, "%s/%s",dirName,roomName);								//Opens the 
		FILE *roomFile = fopen(fileName, "r");												        //start room.
		if (roomFile < 0){										
				printf("ERROR: CANNOT OPEN FILE.\n");				
				return (0);
			}
		
		while (fscanf (roomFile, "%s %s %s", line, line2, line3) != EOF){					//For every room
			if (strcmp(line3, "End_Room") == 0){											              //checks if it is
				printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");				  //the end room.
				printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n",steps);				//If it is then
				snprintf(tmpRoute, bufSize, "%s/%s",dirName,"tmpRoute.txt");				  //prints the winning message
				FILE *route = fopen(tmpRoute, "r");										              	//and the user's path.
				while (fscanf (route, "%s",line) != EOF){								            	//
					printf("%s\n",line);												                      	//
				}																		                                	//
				fclose(route);														                        		//
				remove(tmpRoute);														                        	//
				return (0);																                          	//
			}
		}

		do{
			rewind(roomFile);															                           	//Rewind the file pointer
			fscanf (roomFile, "%s %s %s", line, line2, line3);					        			//and prints the room data
			printf("\nCURRENT LOCATION: %s\n",line3);							              			//to the user. Reads the next 
			printf("POSSIBLE CONNECTIONS: ");											                   	//room from the user.
			while (strcmp(line2, "TYPE:") != 0){									                		//
				fscanf (roomFile, "%s %s %s",line,line2,line3);				          				//
				if ((strcmp(line2, "TYPE:") != 0) && (strcmp(line, "NAME:") != 0)){			//
					if (strcmp(line2, "1:") == 0){									                  		//
						printf("%s",line3);												                        	//
					}else{																                              	//
						printf(", %s",line3);									                        			//
					}																                                  		//
				}else if (strcmp(line2, "TYPE:") == 0){				              						//
					printf(".\n");														                          	//
				}																                                  			//
			}																                                  				//
			printf("WHERE TO? >");						                      									//
			scanf("%s",roomChoice);							                      								//
			rewind(roomFile);
			
			while (fscanf (roomFile, "%s %s %s",line,line2,line3) != EOF){					//Checks if the user choice
				if (strcmp(line3, roomChoice) == 0){			              							//is valid by comparing
					exists = 1;														                          		//all the connections
					break;                                                              //with the given room name.
				}else{																                            		//
					exists = 0;														                          		//
				}																	                                		//
			}																		                                		//

			if (exists != 0){												                        				//If the connection exists
				roomName = roomChoice;										                    				//sets the roomName variable
				route(dirName, roomName);                                             //to that room. If not loops
				steps++;                                                              //and shows the same room data
			}else{																	                            		//until a valid room connection is given.
				printf("\nHUH? I DON’T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");				
			}																				
		}while (exists == 0);																
	}while (exists != 0);
    return (0);
}
