--drop table if exists game_players;
--drop table if exists games;
--drop table if exists tokens;
--drop table if exists players;

Create table if not EXISTS games ( 
    game_id BIGSERIAL Primary key 
);

Create table if not EXISTS players (
    player_id BIGSERIAL Primary key,
    user_name varchar(255) not null unique,
    user_email varchar(255) not null unique,
    user_hash varchar(255) not null
);

Create table game_players (
    game_id BIGSERIAL references games(game_id),
    player_id BIGSERIAL references players(player_id),
    PRIMARY KEY (game_id, player_id)
);

Create table tokens (
    player_id serial references players(player_id),
    PRIMARY KEY(player_id),
    token_hash TEXT NOT NULL,
    token_start TIMESTAMP DEFAULT NOW() not null,
    token_end TIMESTAMP not null
);

GRANT CONNECT ON DATABASE shotgun TO shotgun_admin;
GRANT USAGE ON SCHEMA public TO shotgun_admin;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO shotgun_admin;
GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO shotgun_admin;
