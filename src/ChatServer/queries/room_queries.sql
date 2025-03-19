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
