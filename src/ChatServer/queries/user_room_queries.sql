-- user_room_queries.sql

-- Check if rooms table exists
CHECK_USER_ROOMS_TABLE = 
    SELECT EXISTS (
        SELECT FROM information_schema.tables 
        WHERE table_schema = 'public' 
        AND table_name = 'user_rooms'
    );

-- Create rooms table
CREATE_USER_ROOMS_TABLE = 
    CREATE TABLE user_rooms (
        user_id INT REFERENCES users(id),
        room_id INT REFERENCES rooms(id),
        PRIMARY KEY (user_id, room_id)
    );

-- Fetch joined rooms
FETCH_JOINED_ROOMS = 
    SELECT r.id AS room_id, r.name AS room_name
    FROM rooms r
    JOIN user_rooms ur ON r.id = ur.room_id
    WHERE ur.user_id = :user_id;

-- Fetch users in room
FETCH_USERS_IN_ROOM = 
    SELECT u.id AS user_id, u.username AS username
    FROM users u
    JOIN user_rooms ur ON u.id = ur.user_id
    WHERE ur.room_id = :room_id;

-- Join room
JOIN_ROOM = 
    INSERT INTO user_rooms (user_id, room_id) VALUES (:user_id, :room_id);

-- Leave room
LEAVE_ROOM = 
    DELETE FROM user_rooms WHERE user_id = :user_id AND room_id = :room_id;
