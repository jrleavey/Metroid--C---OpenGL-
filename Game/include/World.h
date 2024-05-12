const int MAX_ROOMS = 20;
const int MAX_DOORS_PER_ROOM = 4; // Adjust based on your game's design

struct RoomTransition {
    int targetRoomId;
    int targetDoorId;
};

extern RoomTransition roomTransitions[MAX_ROOMS][MAX_DOORS_PER_ROOM];