#include <iostream>
#include <map>
#include <string>
#include "Room.h"

using namespace std;

// Function to display the help menu
void displayHelp() {
    cout << "Please choose from the following: \n"
         << "     n for north --- move north (if possible)\n"
         << "      e for east --- move east (if possible)\n"
         << "     s for south --- move south (if possible)\n"
         << "      w for west --- move at west (if possible)\n"
         << "      l for look --- look at room\n"
         << "     x for exits --- show room exits\n"
         << "   c for connect --- connect this room to another (already made) room\n"
         << "    r for rename --- rename this room\n"
         << "      d for desc --- change description for this room\n"
         << "       a for add --- add new room\n"
         << "      q for quit --- exit program\n"
         << "              ? --- show commands (DUH)\n";
}

// Function to display room exits
void displayExits(Room* room) {
    cout << "  North -- " << (room->north() ? room->north()->name() : "Nothing.") << "\n";
    cout << "  East -- " << (room->east() ? room->east()->name() : "Nothing.") << "\n";
    cout << "  South -- " << (room->south() ? room->south()->name() : "Nothing.") << "\n";
    cout << "  West -- " << (room->west() ? room->west()->name() : "Nothing.") << "\n";
}

int main() {
    map<int, Room*> rooms; // Map to store rooms by ID
    int roomID = 0; // Unique room IDs
    Room* currentRoom = new Room("The void", "There is nothing but blackness in every direction.");
    rooms[roomID++] = currentRoom;

    string command;

    while (true) {
        cout << "\nEnter a command (? to list): " << endl;
        cin >> command;

        if (command == "q") {
            cout << "Quitting...\n";
            break;
        } else if (command == "?") {
            displayHelp();
        } else if (command == "l") {
            cout << currentRoom->name() << "\n" << currentRoom->description() << "\n";
        } else if (command == "x") {
            displayExits(currentRoom);
        } else if (command == "r") {
            cout << "Enter a new name for this room and hit <enter> ";
            string newName;
            cin.ignore();
            getline(cin, newName);
            currentRoom->set_name(newName);
        } else if (command == "d") {
            cout << "Enter a new description for this room and hit <enter> ";
            string newDescription;
            cin.ignore();
            getline(cin, newDescription);
            currentRoom->set_description(newDescription);
        } else if (command == "a") { // Add new room
            cout << "Connect new room in which direction? (N,S,E,W)" << endl;
            string dir;
            cin >> dir;

            Direction exitDir;
            if (dir == "N") exitDir = NORTH;
            else if (dir == "E") exitDir = EAST;
            else if (dir == "S") exitDir = SOUTH;
            else if (dir == "W") exitDir = WEST;
            else {
                cout << "Invalid direction.\n";
                continue;
            }

            if (currentRoom->north() && exitDir == NORTH ||
                currentRoom->east() && exitDir == EAST ||
                currentRoom->south() && exitDir == SOUTH ||
                currentRoom->west() && exitDir == WEST) {
                cout << "Exit already exists in that direction!\n";
                continue;
            }

            cout << "Which exit of the new room leads to this room?" << endl;
            string toDir;
            cin >> toDir;

            Direction toExitDir;
            if (toDir == "N") toExitDir = NORTH;
            else if (toDir == "E") toExitDir = EAST;
            else if (toDir == "S") toExitDir = SOUTH;
            else if (toDir == "W") toExitDir = WEST;
            else {
                cout << "Invalid direction.\n";
                continue;
            }

            // Automatically assign the name and description for the new room
            Room* newRoom = new Room("NewRoom", "This room is just waiting for shape and form");
            if (currentRoom->connect(exitDir, newRoom, toExitDir)) {
                rooms[roomID++] = newRoom;
                cout << "Room connection ok\n";
            } else {
                cout << "Room connection failed.\n";
                delete newRoom;
            }
        } else if (command == "c") { // Connect existing rooms
            // Ask for the direction to connect
            cout << "Connect new room in which direction? (N,S,E,W)" << endl;
            string dir;
            cin >> dir;

            Direction exitDir;
            if (dir == "N") exitDir = NORTH;
            else if (dir == "E") exitDir = EAST;
            else if (dir == "S") exitDir = SOUTH;
            else if (dir == "W") exitDir = WEST;
            else {
                cout << "Invalid direction.\n";
                continue;
            }

            // Display available rooms to connect
            cout << "Connect this room to which room? Choose a #\n";
            for (const auto& [id, room] : rooms) {
                cout << id << ": " << room->name() << "\n";
            }

            int roomChoice;
            cin >> roomChoice;

            // Validate room choice
            if (rooms.find(roomChoice) == rooms.end()) {
                cout << "Invalid room choice.\n";
                continue;
            }

            Room* targetRoom = rooms[roomChoice];

            // Determine the direction from the target room back to the current room
            cout << "Which exit of the new room leads to this room?" << endl;
            string toDir;
            cin >> toDir;

            Direction toExitDir;
            if (toDir == "N") toExitDir = NORTH;
            else if (toDir == "E") toExitDir = EAST;
            else if (toDir == "S") toExitDir = SOUTH;
            else if (toDir == "W") toExitDir = WEST;
            else {
                cout << "Invalid direction.\n";
                continue;
            }

            // Attempt to connect the rooms
            if (currentRoom->connect(exitDir, targetRoom, toExitDir)) {
                cout << "Room connection ok\n";
            } else {
                cout << "Room connection failed\n";
            }
        } else if (command == "n" || command == "s" || command == "e" || command == "w") {
            Room* nextRoom = nullptr;
            if (command == "n") nextRoom = currentRoom->north();
            else if (command == "s") nextRoom = currentRoom->south();
            else if (command == "e") nextRoom = currentRoom->east();
            else if (command == "w") nextRoom = currentRoom->west();

            if (nextRoom) {
                currentRoom = nextRoom;
            } else {
                cout << "You cannot go that way\n";
            }
        } else {
            cout << "Invalid command. Type '?' for help.\n";
        }
    }

    // Clean up dynamically allocated rooms
    for (auto& [id, room] : rooms) {
        delete room;
    }

    return 0;
}
