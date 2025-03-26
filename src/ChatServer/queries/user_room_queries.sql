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

-- Join room
JOIN_ROOM = 
    INSERT INTO user_rooms (user_id, room_id) VALUES (:user_id, :room_id);

-- Leave room
LEAVE_ROOM = 
    DELETE FROM user_rooms WHERE user_id = :user_id AND room_id = :room_id;
