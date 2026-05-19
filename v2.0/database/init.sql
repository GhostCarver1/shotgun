drop table if exists game_players;
drop table if exists players;
drop table if exists games;

Create table if not EXISTS games ( 
    game_id serial Primary key 
);

Create table if not EXISTS players (
    player_id serial Primary key,
    user_name varchar(255) not null,
    user_email varchar(255) not null,
    user_hash varchar(255) not null
);

Create table game_players (
    game_id serial references games(game_id),
    player_id serial references players(player_id),
    PRIMARY KEY (game_id, player_id)
);

Insert into players (user_name, user_email, user_hash) values 
('Alice', 'alice@example.com', '$argon2id$v=19$m=65536,t=2,p=1$Oi+hYmQqZL73P4Fxl73Uyw$aa0MK11M1ywz4iw333v5TAUZKZjhu/w+pjWfwFvX0Wg'), -- amicable
('Bob', 'bob@example.com', '$argon2id$v=19$m=65536,t=2,p=1$qchCwdxaIbx8w8NxfphJOA$JDi6+dmKFDmPZPMb1Xy0S9S+eSOk5AdN2yS7qN7L/jY'), -- benevolent
('Charlie', 'charlie@example.com', '$argon2id$v=19$m=65536,t=2,p=1$pTMezX6UIJqv8z0trgHuig$LjgTh5oLRPfCe3/64HxyfqsYGKkAvFFBjWnZD6Wdd+8'); -- cunning

Insert into games (game_id) values (1),(2),(3);

Insert into game_players (game_id, player_id) values 
(1, 1),
(1, 2),
(2, 2),
(2, 3),
(3, 1),
(3, 3);

GRANT CONNECT ON DATABASE shotgun TO shotgun_admin;
GRANT USAGE ON SCHEMA public TO shotgun_admin;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO shotgun_admin;
GRANT ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public TO shotgun_admin;
