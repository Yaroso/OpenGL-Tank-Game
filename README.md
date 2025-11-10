# 3D OpenGL Tank Game

This is a simple 3D tank game built with C++ and OpenGL. It was created as a second-year university project for EEE2041 (Computer Vision & Graphics).

The objective is to navigate your tank through three different mazes and collect all the coins. You can collect coins either by driving over them or by shooting them with the tank's turret. The game automatically progresses from one maze to the next defined in the mazeX.txt files from the assets folder.

### Gameplay

<img width="937" height="986" alt="image" src="https://github.com/user-attachments/assets/419a9a49-10be-487b-a4b4-3be700a94c2e" />
<img width="942" height="992" alt="image" src="https://github.com/user-attachments/assets/dea01414-49cb-430d-87d0-1e774cb2b633" />
<img width="945" height="992" alt="image" src="https://github.com/user-attachments/assets/212e511a-5d4d-4144-91a1-a6a69507a0fe" />
<img width="933" height="976" alt="image" src="https://github.com/user-attachments/assets/3d0245c5-c3db-4af1-96d3-dcd46bcb82c1" />


---

## Controls

* **WASD:** Drive the tank
* **Mouse:** Aim the turret
* **Spacebar:** Shoot
* **R:** Restart the current level

---

## 🛠️ How to Build

This project was built in Visual Studio and relies on `glut` and `glew`.

1.  Clone the repository.
2.  Open the `OpenGLProject.sln` file in Visual Studio.
3.  All dependencies (`glew`, `glut`) are included in the `External/` folder. The project is already configured to link to them.
4.  Build and run the solution (F5).
