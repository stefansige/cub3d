**Cub3D Project README**

Welcome to Cub3D, a project developed as part of the curriculum at 42 School. In this project, we dive into the world of raycasting, a fundamental technique for rendering 3D environments in a 2D space. Inspired by the classic game Wolfenstein 3D, Cub3D challenges you to implement a simplified version of a raycasting engine using the C programming language and the minilibx graphic library.

**Project Overview:**

Cub3D is a graphic project that introduces students to the concepts of raycasting, textures, and basic game development principles. The objective is to create a simple 3D game where players navigate through a maze-like environment from a first-person perspective.

**Features:**

- **Custom Raycasting Engine:** Implement a basic raycasting engine to render a 3D environment in a 2D space.
- **Texture Mapping:** Apply textures to walls, floors, and ceilings to enhance the visual experience.
- **Movement Controls:** Allow players to move forward, backward, and rotate within the game environment.
- **Collision Detection:** Implement collision detection to prevent players from moving through walls or other obstacles.

**Getting Started:**

To get started with Cub3D, follow these steps:

1. Clone the repository to your local machine:

   ```bash
   git clone https://github.com/stefansige/cub3d.git
   ```

2. Navigate to the project directory:

   ```bash
   cd cub3d
   ```

3. Compile the project using the provided Makefile:

   ```bash
   make
   ```

4. Run the executable with a map file:

   ```bash
   ./cub3d map.cub
   ```

**Controls:**

- **W:** Move forward
- **S:** Move backward
- **A:** Move left
- **D:** Move right
- ➡:  Rotate right
- ⬅:  Rotate Left
- **ESC:** Exit the game
