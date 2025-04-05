# Zombie Survival Game

## Overview

This project is a study project aimed at creating a survival game where the player must survive against waves of spawning zombies. The game is developed using Unreal Engine and involves various gameplay mechanics such as shooting, aiming, and reloading.

## Bugs to fix
- [ ] The enemies stay blocked in the jump points if have more than 1 enemy at same place;
- [x] FPS drops when have a lot of enemies on the screen;

## Features

- **Player Mechanics**: The player can shoot, aim, and reload their weapon.
  - [x] Aim
  - [x] Shoot
  - [x] Ammo
  - [x] Reload
  - [x] Health
  - [ ] Score
  - [ ] Anim Montage per gun, using different animations for each gun
  - [ ] Finish inventory component
  

- **Visual Effects**: Includes visual effects for shooting impacts and muzzle flashes.
  - [x] Muzzle Flash
  - [x] Bullet Impact
  - [ ] Blood Splatter

## Getting Started
In this game I'm using **C++ and Blueprints**, combining the power of C++ with the ease of use of Blueprints to create a fun and engaging game. The project is developed in **Unreal Engine 5.4**. It's a simple project to show my skills in Unreal Engine and C++.

### Prerequisites

- Unreal Engine 5.4 or later;
- Visual Studio, Rider or other IDE;

### Installation

1. Clone the repository:
   ```sh
   git clone https://github.com/jaovito/ZombieSurvival.git
   ```
2. Open the project in Unreal Engine.

### Building the Project

1. Open the `.uproject` file in Unreal Engine.
2. Click on `File` -> `Generate Visual Studio project files`.
3. Open the generated `.sln` file in Visual Studio.
4. Build the project in Visual Studio.

## Usage

- **Movement**: Use `W`, `A`, `S`, `D` keys to move the player.
- **Aiming**: Right-click to aim.
- **Shooting**: Left-click to shoot.
- **Reloading**: Press `R` to reload.

# Mechanics
Here is the section about the game mechanics, like movement, shoot, aim, enemies AI, guns customization, etc.

### Base Movement
The base movement system consists of two distinct modes:
1. **Standard Movement**: This is the default mode, allowing the player to move freely in any direction, independent of the camera's orientation. The player can look around without affecting the movement direction, providing a more exploratory control scheme.
2. **Strafe Movement (Aiming Mode)**: Activated when aiming with a weapon, this mode enables the player to strafe — moving while keeping the character's orientation aligned with the camera's forward direction. This is particularly useful for tactical gameplay, allowing precise movement and aiming simultaneously.

https://github.com/user-attachments/assets/f2cf1a4d-0ebd-421f-9d0c-b302d50ae78a

### Shoot and Reload
The shooting mechanic is activated by pressing the left mouse button while aiming. For console controllers, this is mapped to the right trigger. The player must aim to shoot, which ensures a more tactical gameplay approach.

Reloading can be triggered manually by pressing the R key on the keyboard or the equivalent button on the controller. Additionally, if the player attempts to shoot without ammo, the weapon automatically initiates the reload animation, streamlining the combat experience.

This system includes:
- Manual and automatic reload behavior.
- Ammo HUD updates and visual feedback when running out of bullets.
- Full integration with animation notifies to finish reloading only after the animation completes.

https://github.com/user-attachments/assets/360eb3ca-c99e-4b31-8c1e-9805d0a20fb4

### Weapon customization
Weapon customization in this project is designed to be highly flexible and modular. A base C++ class named Gun was created to handle all the core firearm logic — such as aiming, shooting, and reloading — which are common to most weapons.
Everything else is fully customizable through Blueprint inheritance. This includes:
- Input actions for aiming and shooting;
- Shooting animations;
- Fire rate and delay between shots;
- Damage value;
- Impact effects (Niagara or others);
- Muzzle flash effects;
- Skeletal mesh of the weapon, etc.

With this structure, it's easy to create two Blueprint classes derived from the Gun base class — one can be a pistol, while the other can be a rifle — each with completely different behavior and visuals, yet sharing the same core logic.

https://github.com/user-attachments/assets/0f53a6a2-82ae-4508-9e3b-c62b14c66ba4

https://github.com/user-attachments/assets/c6ebea03-ec30-4987-a8cf-d0d90540ae8d

### Enemy Behavior and Combat
Enemies in this project are designed to follow and attack the nearest player character.

The logic responsible for determining the nearest player and directing the enemies is handled by the enemy spawn location class `(AEnemySpawn)`. This centralized approach avoids running expensive calculations on each individual enemy, significantly improving performance, especially during large waves.

Once an enemy is within a certain distance of the player, it attempts to attack. The attack system uses hit traces to check if the attack actually connects with the player. If a successful hit is registered, the player’s health is reduced.

When the player’s health reaches zero, ragdoll physics are enabled on the character’s mesh to simulate death, and a death screen or message is shown on the HUD.

This system ensures efficient AI behavior and realistic combat mechanics, while keeping performance optimized even with many enemies on screen.

https://github.com/user-attachments/assets/066b4854-0102-4705-b304-6679b04ca232

### Wave-Based Enemy Spawning & Enemy Damage System

The enemy spawning system is inspired by classic zombie wave games. It uses a dedicated class called `EnemySpawn`, which is placed as a Blueprint actor in predefined regions throughout the map.

Each `EnemySpawn` Blueprint can be configured with several properties:
- The type of enemy to spawn
- The number of enemies per wave
- Whether the spawn point is active or not
- The initial wave number
- The amount of health added to enemies each wave

When the last enemy from the current wave dies, the spawn system automatically triggers the next wave. Each new wave increases the enemy count and boosts their health based on the current wave level, progressively scaling the difficulty.

Regarding enemy death, once an enemy’s health reaches zero, **ragdoll physics** are applied to simulate a realistic fall. After a short delay, the enemy’s physics simulation and animation updates are disabled. This helps maintain performance by reducing unnecessary computation while keeping the body visible on the ground for visual realism.

The player damages enemies by hitting them with either hit-scan or projectile-based weapons. When a hit is registered (using collision or trace), damage is applied based on the weapon's stats. If the enemy’s health reaches zero, the kill logic is triggered, contributing to wave progression.

https://github.com/user-attachments/assets/bc5f4f5b-f8be-4506-a8dd-63614f424bbb

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Acknowledgements

- Unreal Engine by Epic Games
- Community tutorials and resources

---

Happy coding and enjoy surviving the zombie apocalypse!
