-- user_queries.sql

-- Check if users table exists
CHECK_USERS_TABLE = 
    SELECT EXISTS (
        SELECT FROM information_schema.tables 
        WHERE table_schema = 'public' 
        AND table_name = 'users'
    );

-- Create users table
CREATE_USERS_TABLE = 
    CREATE TABLE IF NOT EXISTS users (
        id SERIAL PRIMARY KEY,
        username VARCHAR(50) UNIQUE NOT NULL,
        password_hash CHAR(64) NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );

-- Delete users table
DELETE_USERS_TABLE = 
    DROP TABLE IF EXISTS users;

-- Register user
REGISTER_USER = 
    INSERT INTO users (username, password_hash) VALUES (:username, :password_hash);

-- Authenticate user
AUTHENTICATE_USER = 
    SELECT password_hash FROM users WHERE username = :username;

-- Get user ID
GET_USER_ID = 
    SELECT id FROM users WHERE username = :username;

-- Get user name
GET_USER_NAME = 
    SELECT username FROM users WHERE id = :id;