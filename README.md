# Zombie Shooter

![Zombie Shooter Banner](https://github.com/SuhaibAnwaar/Zombie-Shooter/blob/aacc586d490829446ef8bd0324648b104bd179b3/Main%20Menu.png)
> A wave-based survival shooter built with Unreal Engine and C++

## 🎮 Play the Game
**[► Play on itch.io](https://suhaib-a.itch.io/zombieshooter)**

## 📖 Overview

Zombie Shooter is a fast-paced, wave-based survival game where the player must fend off increasingly difficult waves of zombie enemies. Featuring an advanced weapon system with five unique ammunition types and intelligent enemy AI, the game challenges players to survive as long as possible against relentless undead hordes.

## ✨ Key Features

### Advanced Weapon System
- **Standard Ammo** - Regular projectiles for consistent damage
- **Fire Ammo** - Burns enemies over time with fire damage
- **Ice Ammo** - Slows enemy movement speed
- **Phase Ammo** - Penetrates through multiple enemies in a line
- **Spray Ammo** - Fires three explosive projectiles for area damage

### Intelligent Enemy AI
- **Chase Behavior** - Zombies actively pursue and track the player
- **Melee Combat** - Realistic punching animations when zombies reach the player
- **Dynamic Pathfinding** - Enemies navigate around obstacles to reach the player

### Progressive Difficulty
- **Wave System** - Each round spawns more zombies than the last
- **Escalating Challenge** - Enemy count increases to create mounting tension
- **Survival Gameplay** - Test your skills as difficulty ramps up

## 🛠️ Technical Implementation

### Built With
- **Engine:** Unreal Engine 4
- **Language:** C++
- **Tools:** Visual Studio, Blueprint (minimal)

### Core Systems

**Weapon System**
```cpp
- Multi-ammo type implementation
- Projectile physics and collision detection
- Damage calculation system for each ammo type
- Area-of-effect mechanics for explosive projectiles
```

**AI System**
```cpp
- NavMesh-based pathfinding
- Player tracking and chase behavior
- Animation state machine for melee attacks
- Line-of-sight detection
```

**Spawn System**
```cpp
- Wave progression logic
- Dynamic enemy spawning
- Round management
```

## 📸 Screenshots

![Gameplay 1](https://github.com/SuhaibAnwaar/Zombie-Shooter/blob/aacc586d490829446ef8bd0324648b104bd179b3/Game%20Image%201.png)
*Wave-based survival action*

![Gameplay 2](https://github.com/SuhaibAnwaar/Zombie-Shooter/blob/aacc586d490829446ef8bd0324648b104bd179b3/Game%20Image%202.png)
*Advanced weapon system with multiple ammo types*

![Gameplay 3](https://github.com/SuhaibAnwaar/Zombie-Shooter/blob/aacc586d490829446ef8bd0324648b104bd179b3/Game%20Image%203.png)
*Zombie AI with chase behavior*

### Controls
- **WASD** - Movement
- **Space** - Jump 
- **Mouse** - Aim
- **Q** - Pause
- **Left Click** - Shoot
- **Right CLick** - Reload
- **Mouse Scroll Wheel** - Switch Multi ammo
- **1** - Multi Ammo
- **2** - Phase Ammo
- **3** - Spray Ammo

## 💡 Development Highlights

### Challenges Solved
- **Projectile Penetration** - Implemented ray tracing for phase ammo to detect and damage multiple enemies in a single shot
- **Enemy Spawning** - Created a balanced spawn system that increases difficulty without overwhelming players early
- **Performance Optimization** - Used object pooling for projectiles to maintain smooth frame rates during intense combat

### What I Learned
- Advanced C++ programming in Unreal Engine
- AI behavior implementation with NavMesh
- Complex weapon system architecture with multiple damage types
- Game balance and difficulty curve design
- Collision detection and projectile physics

## 🎯 Future Improvements

If I were to expand this project, I would add:
- [ ] Multiple levels with varied environments
- [ ] Power-ups and pickups system
- [ ] Boss zombies with unique behaviors
- [ ] Leaderboard and score tracking
- [ ] More weapon types

## 📝 Project Information

- **Development Time:** June 2022
- **Type:** Personal Project / Portfolio Piece
- **Status:** Complete and playable

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👤 Author

**Muhammad Suhaib Anwaar**
- itch.io: [suhaib-a](https://suhaib-a.itch.io)
- Email: suhaibmu@hotmail.co.uk

## 🙏 Acknowledgments

- Unreal Engine documentation and community
- Abertay University for foundational game development education

---

⭐ If you found this project interesting, please consider giving it a star!

**[► Play the full game on itch.io](https://suhaib-a.itch.io/zombieshooter)**
