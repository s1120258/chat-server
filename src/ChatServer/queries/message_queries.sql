-- message_queries.sql

-- Check if messages table exists
CHECK_MESSAGES_TABLE = 
    SELECT EXISTS (
        SELECT FROM information_schema.tables 
        WHERE table_schema = 'public' 
        AND table_name = 'messages'
    );

-- Create messages table
CREATE_MESSAGES_TABLE = 
    CREATE TABLE IF NOT EXISTS messages (
        id SERIAL PRIMARY KEY,
        room_id INT REFERENCES rooms(id),
        user_id INT REFERENCES users(id),
        message TEXT NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );

-- Delete messages table
DELETE_MESSAGES_TABLE = 
    DROP TABLE IF EXISTS messages;

-- Fetch messages
FETCH_MESSAGES = 
    SELECT m.message, m.created_at, u.username 
    FROM messages m
    JOIN users u ON m.user_id = u.id
    WHERE m.room_id = :room_id
    ORDER BY m.created_at ASC

INSERT_MESSAGE = 
    INSERT INTO messages (room_id, user_id, message) VALUES (:room_id, :user_id, :message);