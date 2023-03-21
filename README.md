Currently unnamed roguelike made for IT276

<b>CONTROLS</b>
+ WASD to move
+ aim w/ mouse
+ shoot w/ spacebar
+ e to activate ability
+ m to display money in terminal
<br>

<b>MIDTERM DELIVERABLES</b>

Common deliverables
+ Functioning Entity System
    + Player, enemies, etc. are part of the entity system, spawn in, and interact with each other
+ Fully interact-able environment
    + Collision system, can interact with walls (player and enemies are not able to pass through solid walls)
+ User Interface (HUD, Stats, Messages, menus)
    + Player's health, mana, and current ability are displayed in HUD
+ File read / write access (high score list, saved games, saved songs, etc)
    + Levels are loaded in from json files
    + Highest score and most rooms visited/highest room streak is stored in json (read from + written to if score is higher)
+ README for how to play and see all game features (this should be in the git repo)
    + This

<br>

Personal deliverables
+ 5 rooms that are randomly chosen
    + Rooms are randomly chosen from pool of currently 7 rooms (+ 1 starting room)
    + Go to next randomly chosen room via cave hole near top of each room
+ 5 player abilities/attacks
    + Trigger ability with E
    + Abilities
        + Triple Shot: when attacking, shoots 3 projectiles 
        + Heal: recover HP of player over time
        + Self-Destruct: when attacking, player does big damage in ring around self but loses HP (need to attack with spacebar after pressing E)
        + Rage Mode: lose ability to attack, but can run over enemies and kill them
        + Poison Darts: when attacking, projectiles can poison enemies
+ 5 different enemy types
    + Slime: does contact dmg; when it dies, splits into 2 smaller slimes
    + Hostile Crystal Guy: shoots projectiles at player; stays still and has high hp
    + Glowfly: shoots projectiles at player; flies fast and follows player around
    + Angry Block: does contact dmg; moves back and forth horizontally to try and block player
    + Sentient Drill: does contact dmg; charges forward in small bursts and follows player 
+ Loot (items that can be dropped/collected/spent)
    + Coin - adds 1 to player's money/currency
    + Heart - heals player for a little bit
    + Mana Drop - recovers a little bit of mana
    + Dropped by enemies after killing them
    + Can also appear in certain rooms by default
    + Can spend coins at shop (press m key to see current money in terminal)
+ Functioning shop
    + A random room that player can encounter
    + Shop items to purchase randomized - can buy a health or mana pickup (5 each) or new ability (20 each)

<br><br>
Built off of seed project from https://github.com/engineerOfLies/gameframework2d
\+ used/referenced code from https://github.com/engineerOfLies/gf3d and https://github.com/engineerOfLies/gfc
(thanks dj!)
