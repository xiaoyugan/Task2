git: https://github.com/xiaoyugan/Task2

1. The meaning of each data table
    (1) activityInfo.csv
        game id, game name, player username, start time, duration 
    (2) adminlist.csv
        uer type, admin username, admin user password, admin user email
    (3) gamelist.csv
        game id, game name, price, description, version number, age restriction
    (4) gamestudio.csv
        user type, gamestudio name, gamestudio password, gamestudio email, gamestudio gamelist
    (5) guestlist.csv
        guest email
    (6) playerlist.csv
        user type, player username, player password, player email, player age, player wallet, player gamelist
    (7) purchaseHistory.csv
        player username, game id, game title, purchase time

2. Some of login usernames and passwords(More please get from the table)
    (1) player: username player1, password 12345, age 2
                       username player2, password 12345, age 8
                       username player3, password 12345, age 18
    (2) admin: username admin1, password 12345
                       username admin2, password 12345
    (3) gamestudio: username gamestudio1, password 12345
                                 username gamestudio2, password 12345
    (2) guest: guest1@
                      guest2@

3. Data is stored in tables in real time to prevent data loss caused by abnormal exit.

4. Game: uniqued id, game title, price, description, version number, age restriction

5. Guest
    (1) If the users do not log in and choose to enter Vapor as a guest, they need to enter the email address. After entering the guest interface, the guests can view all the games and sign up.
    (2) the guests only have the email and do not have name, password ,etc.
    (3) Vapor will keep track of the guests` email and when they visit Vapor again, it will  display a friendly message inviting them to sign-up. 
    (4) If the guests choose to sign up, their email will  be removed from the guestlist. And they can use their username to log in.
    (5) when they sign up successfully, the interface will be jumped from the visitor interface to the player interface. 
    (6) when a guest sign-up, the system determines whether the user name already exists. 

6. Player
    (1) the player has uniqued username, password, email, wallet, bag(their own games), age. They represents a system user who owns games
    (2) players can view all the Vapor`s games
    (3) players can view their own games
    (4) players can buy the game that they do not have. when they buy games, the system will judge their 'bag', 'wallet' and the age restriction.
    (5) players can search games by game title, description, price and age restriction.
    (6) players can gift another player with one of their games. The recipient is identified by his logon code and, if it exists and the friend does not have this game and the age of the friend meets the age limit of this game, the game will be transferred from one bag to the other. And the friend must be a player.
    (7) players can see how much money is in the wallet.
    (8) players can play their own games(  the date and time as well as the length of time each player spends in each game will be collected and stored and the admin user can view these information )
    (9) players can top up
    (10) players can view the most popular game ( By comparing the length of time all games are played )
    (11) players can view the average game price

7. Admin
    (1) the admin has uniqued username, password, email. They represents a system user who has privileges to modify the system.
    (2) admins can view all the Vapor`s games
    (3) admins can view all the users information
    (4) admins can add games to Vapor (Resolve different games by game ID)
    (5) admins can modify the game`s title, description and price
    (6) admins can remove games
    (7) admins can view all game purchase records (with player, games and data of purchase) 
    (8) admins can view all player activity information. ( the date and time as well as the length of time each player spends in each game )
    (9) admins can view the most popular game ( by comparing the length of time all games are played )
    (10) admins can view the average game price

8. GameStudio
    (1) the game studio has uniqued username, password, email, bag(their games) 
    (2) game studios can view all of their own games
    (3) game studios can can gift another player with one of their games. The recipient is identified by his logon code and, if it exists and the friend does not have this game and the age of the friend meets the age limit of this game, the game will be gifted successfully. And the friend must be a player.
    (4) game studios can play their own games
    (5) game studios can upload their new games and add this game to their gamelist
    (6) game studios can set a new version of their games





