-- room_queries.sql

-- Check if rooms table exists
CHECK_ROOMS_TABLE = 
    SELECT EXISTS (
        SELECT FROM information_schema.tables 
        WHERE table_schema = 'public' 
        AND table_name = 'rooms'
    );

-- Create rooms table
CREATE_ROOMS_TABLE = 
    CREATE TABLE IF NOT EXISTS rooms (
        id SERIAL PRIMARY KEY,
        name VARCHAR(100) UNIQUE NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );

-- Delete rooms table
DELETE_ROOMS_TABLE = 
    DROP TABLE IF EXISTS rooms;

-- Create room
PREPARE_ROOM = 
    INSERT INTO rooms (name) VALUES (:name);
