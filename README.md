# FANTASY BRAWL

Fantasy Brawl is a Battle Royale game developed by CheckTheDog that features intense action in a PvP map. 
4 players have to give their best in order to survive the struggle! 
Moreover, each player is in control of a totally different hero each with its own characteristics and special ability! 
Get ready warrior! A fierce battle against the strongest is about to begin!

- Fantasy Brawl's Wiki: [Wiki](https://github.com/CheckTheDog/Fantasy-Brawl/wiki)
- Fantasy Brawl's Website: [Website](https://checkthedog.github.io/Fantasy-Brawl/)

### CONTROLS

- Player Movement (Gamepad Exclusive):

  - Move around: Left joystick, movement in any direction

- Player Abilities(Gamepad Exclusive):

  - Aim: Move RIGHT JOYSTICK
  - Basic Attack: Press / keep pressed RIGHT BUMPER. 
  - Super Attack: Press LEFT BUMPER, then release it.
  - Shield: Press A, deactivate it halfway pressing A again.

Tips: Auto-aim will be activated when being near enemy players, no need to move the right joystick.


- ESC opens pause menu (to be changed in future versions of the game)

### AUTHORS: CheckTheDog

A pleasure! We are a team of 8 university students of UPC Barcelona with a clear mindset, 
turning our game into a one of a kind experience! 
To do so, each member helds its own responsibility, which in turn allows the team to work efficiently 
and effectively towards our goal, making this game a reality! Know the team!

**Team Lead:** *Aitor Simona*
* Github account: [AitorSimona](https://github.com/AitorSimona)

**Code:** _Didac Romero_
* Github account: [DidacRomero](https://github.com/DidacRomero)

**Code:** _Marc Garcia_
* Github account: [marcgreig](https://github.com/marcgreig)

**Management:** _Vladislav Bulantsev_
* Github account: [vlaad96](https://github.com/vlaad96)

**Art & Audio:** _Roger Sànchez_
* Github account: [RogerJimbo](https://github.com/RogerJimbo)

**Design:** _Jacobo Galofré_
* Github account: [sherzock](https://github.com/sherzock)

**UI:** _Òscar Larios_
* Github account: [Megaoski](https://github.com/Megaoski)

**QA:** _Pol Bosch_
* Github account: [OhCaXulu](https://github.com/OhCaXulu)

### SOCIAL

Caught your interest? Follow us on twitter! Updates on the game's development are on their way!

Our Twitter account: [Our Twitter](https://twitter.com/dog_check?lang=es)

### TOOLS USED

- Microsoft Visual Studio 2017
- Tiled
- Adobe Photoshop
- Piskel
- Brofiler
- SDL, STL and pugi xml

### DISCLAIMER

Copyright Disclaimer, allowance is made for "fair use" for purposes such as criticism, comment, news reporting,
 teaching, scholarship, and research. Fair use is a use permitted by copyright statute that might otherwise be infringing. 
Non-profit, educational or personal use tips the balance in favor of fair use." All content is owned by its respective 
creators, no copyright infringement is intended, its use is solely and strictly educational.

We do not own most of the sprites, tilesets, music or sfx presented in this game. Authors credited below:

- Player sprites, Tilesets & songs, SFX are property of  Square Enix, original from Final Fantasy Tactics for 
Sony's Playstation

- Shield and IDCircle animations (circle on the bottom of players) sprites by Pol

### LICENSE

© 2019 CheckTheDog

Licensed under the [MIT License](LICENSE.txt)

### TASK LIST

Aitor:

- Base Code
- Entity System
- Collision System
- Player Movement
- Player Aim
- Character animations (implementation)
- Player particle shooting
- Particle system improvements (put to work)
- Player basic attack
- All charachters Super Attacks
- Player Shield
- Particles launched in all directions with rotated sprite
- Particle collisions and damage, player death
- Cameras inside map bounds and fixed to players
- Players spawning position and full reset at new game
- UI transition improvement (black quad at the back of the screen)
- Scores (kills + winner) in code
- Music Plays
- Improved Sprite Ordering
- In-game UI (code behaviour)
- Player-Character Independence
- Adapted Player related stuff and particles to Scale system
- Auto-Aim
- Added IDCircle code management
- Aim path on basic & Super attacks
- In-game UI adjustments
- Added margin (code)
- Added Arrows
- Added random music at game start
- In-Game UI multiple adjustments and scaling

Didac:

- Gamepad Input management (all buttons, joysticks and haptic features)
- Storm
- Fx plays
- Gamepad Vibration
- Scale system 

Pol:

- All 4 character animations (Wendolin, Trakt, Simon, Meliadoul):
  - 8 direction Idles (creation).
  - 8 direction movement (Up, Down, Left, Right creation; Diagonals taken from sprites).
  - 8 direction attack frames (Up, Down, Left, Right creation; Diagonals taken from sprites).
- All 4 character sprites into Tiled object layers (+ Adjustments).
- Particle Textures.
- Crystal Textures.
- Tileset for the 1st map.
- Map layout in Tiled.
- Automap feature in Tiled adapted to our Tileset (Automatically creating all layers of tiles including collisions, from a ground basic layer).
- Shield Animation (Shield desintegration 40 frames + shield vanishing 4 frames).
- IDCircle Animations (4 colours, 4 different textures per colour).
- Margin + mid-screen texture (ART)
- Redone UI In-game bars in pixel art

Vladislav: 

- Basic Particle System (rough)
- Basic Website

Jacobo:

- Initial Sprite Ordering implementation
- 4 camera system
- Camera culling
- Improved Sprite Ordering

Oscar:

- Main menus, working UI workflow
- Pause menu
- Placeholders (temporal art)
- Character Selection Menu
- Scoreboard Menu
- In game UI (HP Bar + Super Attack Bar)
- In-game UI adjustments
- Options Menu (sliders + apply button)

Roger:
- In game UI Art (legacy version)
