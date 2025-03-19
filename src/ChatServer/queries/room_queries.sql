-- room_queries.sql

-- Check if rooms table exists
CHECK_ROOM_TABLE = 
    SELECT EXISTS (
        SELECT FROM information_schema.tables 
        WHERE table_schema = 'public' 
        AND table_name = 'rooms'
    );

-- Create rooms table
CREATE_ROOM_TABLE = 
    CREATE TABLE IF NOT EXISTS rooms (
        id SERIAL PRIMARY KEY,
        name VARCHAR(100) UNIQUE NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );

-- Delete rooms table
DELETE_ROOM_TABLE = 
    DROP TABLE IF EXISTS rooms;

-- Create room
CREATE_ROOM = 
    INSERT INTO rooms (name) VALUES (:name)

-- Join room
JOIN_ROOM = 
    INSERT INTO user_rooms (user_id, room_id) VALUES (:user_id, :room_id)

-- Leave room
LEAVE_ROOM = 
    DELETE FROM user_rooms WHERE user_id = :user_id AND room_id = :room_id
